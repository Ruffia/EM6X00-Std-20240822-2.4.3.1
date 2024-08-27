#ifndef LineTracker_h
#define LineTracker_h

class CLineTracker: public CRectTracker
{
public:
	void SetLastRect( const CRect& rect );
	CLineTracker(LPCRECT lpSrcRect, UINT nStyle)
	{ m_rect = CRect( *lpSrcRect ); m_sizeMin = 0; }
	virtual ~CLineTracker(){}

	void Draw( CDC* pDC );
	void GetHandleRect(CPoint& pPoint, CRect& pRect);
	int HitTest(CPoint point);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest);
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo);
// Overridables
	virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd);
	virtual void AdjustRect(int nHandle, LPRECT lpRect);
	virtual void OnChangedRect(const CRect& rectOld);
	virtual UINT GetHandleMask() const;
};

class CRectTrackerEx: private CRectTracker
{
public:
	CRectTrackerEx();
	CRectTrackerEx(LPCRECT lpSrcRect, UINT nStyle);
	virtual ~CRectTrackerEx(){}

	void Draw( CDC* pDC );
	int HitTest(CPoint point);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest);
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert=FALSE, CWnd* pWndClipTo=NULL);

	void UpdateRect( int dx, int dy, double ds );
	void SetRect( const CRect& rect );
	void GetRect( CRect& rect );
	void SetRealRect( const CRect& rect );
	void GetRealRect( CRect& rect );
private:
	int m_dx;
	int m_dy;
	double m_doubleScale;
};

#endif