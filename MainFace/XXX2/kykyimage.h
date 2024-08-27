// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IKYKYImage wrapper class

class IKYKYImage : public COleDispatchDriver
{
public:
	IKYKYImage() {}		// Calls COleDispatchDriver default constructor
	IKYKYImage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IKYKYImage(const IKYKYImage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long OpenImageFromFile(LPCTSTR lpszFilePathName);
	long OpenImageFromUntitledCxImage(OLE_HANDLE* pitem);
};
