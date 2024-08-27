
class CBeamButton : public CButton
{
    // 按钮的三态
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
 
    // 设置按钮状态
    void SetButtonState(ButtonState state);
	void OnLButtonDown(int nButtonState);
protected:
    // 绘制按钮
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
 
    DECLARE_MESSAGE_MAP()
};