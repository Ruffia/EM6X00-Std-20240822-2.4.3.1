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

  初始化工作队列
  const unsigned int  nNumberOfThreads      - 要创建的工作队列中线程的数目
  void*         *ThreadData                 - 送给线程的需要执行的工作项目
		
------------------------------------------------------------------------*/

bool CWorkQueue::Create(const unsigned int nNumberOfThreads, 
                              void*   *ThreadData  /*=NULL*/)
{
	if (m_bAlreadyCreated)
	{
		return true;
	}
      
   //初始化工作队列
	m_pWorkItemQueue = new WorkItemQueue();
   
	if(NULL == m_pWorkItemQueue )
	{      
		return false;
	}

	//创建Semaphore对象   
	m_phSincObjectsArray[SEMAPHORE_INDEX] = CreateSemaphore(NULL,0,LONG_MAX,NULL);

	if(m_phSincObjectsArray[SEMAPHORE_INDEX] == NULL)
	{      
		delete m_pWorkItemQueue;
		m_pWorkItemQueue = NULL;
		return false;
	}

	//创建event 事件对象
	m_phSincObjectsArray[ABORT_EVENT_INDEX] = CreateEvent(NULL,TRUE,FALSE,NULL);


	if(m_phSincObjectsArray[ABORT_EVENT_INDEX]  == NULL)
	{      
		delete m_pWorkItemQueue;
		m_pWorkItemQueue = NULL;
		CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
		return false;
	}

	//分配线程句柄数组
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

	//创建所有的线程
	for(i = 0 ; i < nNumberOfThreads ; i++ )
	{  
		//初始化每个线程的上下文，用于传递给线程函数
		pThreadsContext = new THREAD_CONTEXT;
		pThreadsContext->pWorkQueue  = this;
		pThreadsContext->pThreadData = ThreadData == NULL? NULL : ThreadData[i];	   
		//创建线程
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

  插入工作任务到工作队列
------------------------------------------------------------------------*/
bool CWorkQueue::InsertWorkItem(WorkItemBase* pWorkItem)
{

   assert(pWorkItem != NULL);
   //锁住
   CSingleLock lock(&m_CriticalSection,TRUE);
   //插入队列
   m_pWorkItemQueue->push(pWorkItem);
   //解锁
   lock.Unlock();
   //触发线程
   if (!ReleaseSemaphore(m_phSincObjectsArray[SEMAPHORE_INDEX],1,NULL)) 
   { 
      assert(false);
      return false;     
   } 
   
 return true;
}

/*------------------------------------------------------------------------
RemoveWorkItem()

 从工作队列中取出任务，并且移除
------------------------------------------------------------------------*/
WorkItemBase*  CWorkQueue::RemoveWorkItem()
{
   
   WorkItemBase* pWorkItem;
   //锁住
   CSingleLock lock(&m_CriticalSection,TRUE);
   //从队列中移除任务  
   pWorkItem = m_pWorkItemQueue->front();
   m_pWorkItemQueue->pop();
   //解锁
   lock.Unlock();
   assert(pWorkItem != NULL);
   return pWorkItem;
}
/*------------------------------------------------------------------------
ThreadFunc

  这是线程函数，用来等待工作任务事件的到来或者离开线程事件的触发

------------------------------------------------------------------------*/
unsigned long __stdcall CWorkQueue::ThreadFunc( void*  pParam )
{ 
   PTHREAD_CONTEXT       pThreadContext =  (PTHREAD_CONTEXT)pParam;//线程的传入参数 
   WorkItemBase*         pWorkItem      = NULL;
   CWorkQueue*           pWorkQueue     = pThreadContext->pWorkQueue;//工作队列指针
   void*                 pThreadData    = pThreadContext->pThreadData;
   DWORD dwWaitResult; 
   for(;;)
   {
	  //等待两个事件
	  dwWaitResult = WaitForMultipleObjects(NUMBER_OF_SYNC_OBJ,pWorkQueue->m_phSincObjectsArray,FALSE,INFINITE);

      switch(dwWaitResult - WAIT_OBJECT_0)
      {

      case ABORT_EVENT_INDEX: //离开线程事件
		 delete pThreadContext;
         return 0; 
      case SEMAPHORE_INDEX://工作任务事件
         //得到工作队列的第一个工作任务
         pWorkItem = pWorkQueue->RemoveWorkItem();     
         if(pWorkItem == NULL)
         {
            assert(false);
            break;
         }     
         //调用相应的工作函数
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

  设置线程退出事件，等待所有线程的结束
------------------------------------------------------------------------*/
void CWorkQueue::Destroy()
{//  [5/10/2012 dongziheng]
  //设置退出事件	   
	__try
	{

		if(!SetEvent(m_phSincObjectsArray[ABORT_EVENT_INDEX]))
		{     
			return;
		}
		//等待所有的线程结束
		WaitForMultipleObjects(m_nNumberOfThreads,m_phThreads,true,INFINITE);

		//清除队列
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

		//关闭所有的线程句柄
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
线程数
获取线程队列中的线程数
------------------------------------------------------------------------*/
int CWorkQueue::GetWorkItemCount()
{
	return m_pWorkItemQueue->size();
}