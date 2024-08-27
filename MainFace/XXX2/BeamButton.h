
class CBeamButton : public CButton
{
    // ��ť����̬
    enum ButtonState {
        STATE_NORMAL,
        STATE_DETECTION,
        STATE_CONTRAL
    };
 
    ButtonState m_state;
	CString		m_strCaption;
	
public:
    CBeamButton();
	void	SetCaption( CString strCaption ){m_strCaption = strCaption; Invalidate(FALSE);}
 
    // ���ð�ť״̬
    void SetButtonState(ButtonState state);
	void OnLButtonDown(int nButtonState);
protected:
    // ���ư�ť
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
 
    DECLARE_MESSAGE_MAP()
};