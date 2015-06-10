#pragma once
#include "../xis/pane.h"
#include "LightControl.h"

// CStepTips 对话框

class CStepTips : public CPropertyPage
{
	DECLARE_DYNAMIC(CStepTips)

public:
	CStepTips(BOOL bOffset,BOOL bFinished);
	virtual ~CStepTips();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_STEP2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bOffset;
	BOOL m_bFinish;
	CXISLFunction *m_pXISLFunction;
	CString m_strMsg;
	CFont m_Font;
	CLightControl *m_pLightControl;
	CIni *m_pIni;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	virtual LRESULT OnWizardNext();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void SetXISLFucntion(CXISLFunction *pFunc) {m_pXISLFunction = pFunc;}
	void SetMessage(CString strMessage) {m_strMsg = strMessage;};
	void SetLightControl(CLightControl *pLightControl) {m_pLightControl = pLightControl;}
	void SetConfig(CIni *pIni) {m_pIni = pIni;}
};
