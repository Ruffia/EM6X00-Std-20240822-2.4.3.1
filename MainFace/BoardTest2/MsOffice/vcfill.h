#if !defined(AFX_VCFILL_H__8916483F_4328_11D6_9373_B2FE5AB4317A__INCLUDED_)
#define AFX_VCFILL_H__8916483F_4328_11D6_9373_B2FE5AB4317A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CVcBrush;

/////////////////////////////////////////////////////////////////////////////
// CVcFill wrapper class

class CVcFill : public COleDispatchDriver
{
public:
	CVcFill() {}		// Calls COleDispatchDriver default constructor
	CVcFill(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVcFill(const CVcFill& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetStyle();
	void SetStyle(long nNewValue);
	CVcBrush GetBrush();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCFILL_H__8916483F_4328_11D6_9373_B2FE5AB4317A__INCLUDED_)
