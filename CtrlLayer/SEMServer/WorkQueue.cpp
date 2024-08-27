#include "stdafx.h"
#include "WorkQueue.h"
#include <assert.h>

typedef struct _THREAD_CONTEXT
{
   CWorkQueue* pWorkQueue;
   void*       pThreadData;
} THREAD_CONTEXT,*PTHREAD_CONTEXT;

CWorkQueue& CWorkQueue::Instance()
{
	static CWorkQueue workQueue;
	return workQueue;
}

/*------------------------------------------------------------------------
Create

  ��ʼ����������
  const unsigned int  nNumberOfThreads      - Ҫ�����Ĺ����������̵߳���Ŀ
  void*         *ThreadData                 - �͸��̵߳���Ҫִ�еĹ�����Ŀ
		
------------------------------------------------------------------------*/

bool CWorkQueue::Create(const unsigned int nNumberOfThreads, 
                              void*   *ThreadData  /*=NULL*/)
{
	if (m_bAlreadyCreated)
	{
		return true;
	}
      
   //��ʼ����������
	m_pWorkItemQueue = new WorkItemQueue();
   
	if(NULL == m_pWorkItemQueue )
	{      
		return false;
	}

	//����Semaphore����   
	m_phSincObjectsArray[SEMAPHORE_INDEX] = CreateSemaphore(NULL,0,LONG_MAX,NULL);

	if(m_phSincObjectsArray[SEMAPHORE_INDEX] == NULL)
	{      
		delete m_pWorkItemQueue;
		m_pWorkItemQueue = NULL;
		return false;
	}

	//����event �¼�����
	m_phSincObjectsArray[ABORT_EVENT_INDEX] = CreateEvent(NULL,TRUE,FALSE,NULL);


	if(m_phSincObjectsArray[ABORT_EVENT_INDEX]  == NULL)
	{      
		delete m_pWorkItemQueue;
		m_pWorkItemQueue = NULL;
		CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
		return false;
	}

	//�����߳̾������
	m_phThreads = new HANDLE[nNumberOfThreads];

	if(m_phThreads == NULL)
	{    
		delete m_pWorkItemQueue;
		m_pWorkItemQueue = NULL;
		CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
		CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
		return false;
	}

	unsigned int i;

	m_nNumberOfThreads = nNumberOfThreads;

	DWORD dwThreadId;
	PTHREAD_CONTEXT pThreadsContext ;

	//�������е��߳�
	for(i = 0 ; i < nNumberOfThreads ; i++ )
	{  
		//��ʼ��ÿ���̵߳������ģ����ڴ��ݸ��̺߳���
		pThreadsContext = new THREAD_CONTEXT;
		pThreadsContext->pWorkQueue  = this;
		pThreadsContext->pThreadData = ThreadData == NULL? NULL : ThreadData[i];	   
		//�����߳�
		m_phThreads[i] = CreateThread(NULL,
			0,
			CWorkQueue::ThreadFunc,
			pThreadsContext,
			0,
			&dwThreadId);

		if(m_phThreads[i] == NULL)
		{  
			delete pThreadsContext;
			m_nNumberOfThreads = i;
			Destroy();
			return false;
		}
	}

	m_bAlreadyCreated = TRUE;
	return true;
}


/*------------------------------------------------------------------------
InsertWorkItem

  ���빤�����񵽹�������
------------------------------------------------------------------------*/
bool CWorkQueue::InsertWorkItem(WorkItemBase* pWorkItem)
{

   assert(pWorkItem != NULL);
   //��ס
   CSingleLock lock(&m_CriticalSection,TRUE);
   //�������
   m_pWorkItemQueue->push(pWorkItem);
   //����
   lock.Unlock();
   //�����߳�
   if (!ReleaseSemaphore(m_phSincObjectsArray[SEMAPHORE_INDEX],1,NULL)) 
   { 
      assert(false);
      return false;     
   } 
   
 return true;
}

/*------------------------------------------------------------------------
RemoveWorkItem()

 �ӹ���������ȡ�����񣬲����Ƴ�
------------------------------------------------------------------------*/
WorkItemBase*  CWorkQueue::RemoveWorkItem()
{
   
   WorkItemBase* pWorkItem;
   //��ס
   CSingleLock lock(&m_CriticalSection,TRUE);
   //�Ӷ������Ƴ�����  
   pWorkItem = m_pWorkItemQueue->front();
   m_pWorkItemQueue->pop();
   //����
   lock.Unlock();
   assert(pWorkItem != NULL);
   return pWorkItem;
}
/*------------------------------------------------------------------------
ThreadFunc

  �����̺߳����������ȴ����������¼��ĵ��������뿪�߳��¼��Ĵ���

------------------------------------------------------------------------*/
unsigned long __stdcall CWorkQueue::ThreadFunc( void*  pParam )
{ 
   PTHREAD_CONTEXT       pThreadContext =  (PTHREAD_CONTEXT)pParam;//�̵߳Ĵ������ 
   WorkItemBase*         pWorkItem      = NULL;
   CWorkQueue*           pWorkQueue     = pThreadContext->pWorkQueue;//��������ָ��
   void*                 pThreadData    = pThreadContext->pThreadData;
   DWORD dwWaitResult; 
   for(;;)
   {
	  //�ȴ������¼�
	  dwWaitResult = WaitForMultipleObjects(NUMBER_OF_SYNC_OBJ,pWorkQueue->m_phSincObjectsArray,FALSE,INFINITE);

      switch(dwWaitResult - WAIT_OBJECT_0)
      {

      case ABORT_EVENT_INDEX: //�뿪�߳��¼�
		 delete pThreadContext;
         return 0; 
      case SEMAPHORE_INDEX://���������¼�
         //�õ��������еĵ�һ����������
         pWorkItem = pWorkQueue->RemoveWorkItem();     
         if(pWorkItem == NULL)
         {
            assert(false);
            break;
         }     
         //������Ӧ�Ĺ�������
         pWorkItem->DoWork(pThreadData);   
		 if (NULL != pWorkItem)
		 {
			 pWorkItem->Abort();
			 pWorkItem = NULL;
		 }
         break;
      default:
          assert(false);
		  delete pThreadContext;
          return 0; 

      }      
   }
   delete pThreadContext;
   return 1; 
}


/*------------------------------------------------------------------------
Destroy	

  �����߳��˳��¼����ȴ������̵߳Ľ���
------------------------------------------------------------------------*/
void CWorkQueue::Destroy()
{//  [5/10/2012 dongziheng]
  //�����˳��¼�	   
	__try
	{

		if(!SetEvent(m_phSincObjectsArray[ABORT_EVENT_INDEX]))
		{     
			return;
		}
		//�ȴ����е��߳̽���
		WaitForMultipleObjects(m_nNumberOfThreads,m_phThreads,true,INFINITE);

		//�������
		if (m_pWorkItemQueue)
		{
			while(!m_pWorkItemQueue->empty())
			{
				m_pWorkItemQueue->front()->Abort();
				m_pWorkItemQueue->pop();
			}  
			delete m_pWorkItemQueue;
		}
		if (m_phSincObjectsArray[SEMAPHORE_INDEX])
		{
			CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
		}
		if (m_phSincObjectsArray[ABORT_EVENT_INDEX])
		{
			CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
		}

		//�ر����е��߳̾��
		if (m_phThreads)
		{
			for(int i = 0 ; i < m_nNumberOfThreads ; i++)
			{
				if (m_phThreads[i])
				{
					CloseHandle(m_phThreads[i]);
				}
				m_phThreads[i] = NULL;
			}
			delete[] m_phThreads;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		
	}

	m_phSincObjectsArray[SEMAPHORE_INDEX] = NULL;
	m_phSincObjectsArray[ABORT_EVENT_INDEX] = NULL;
	m_phThreads = NULL;
	m_pWorkItemQueue = NULL;
	m_bAlreadyCreated = FALSE;
}

/*------------------------------------------------------------------------
�߳���
��ȡ�̶߳����е��߳���
------------------------------------------------------------------------*/
int CWorkQueue::GetWorkItemCount()
{
	return m_pWorkItemQueue->size();
}