#pragma once
#include "afxwin.h"
#include "afxpropertygridctrl.h"
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>

// CAutoProcessAlgDlg 对话框
typedef struct _STRUCT_KEY_VALUE
{
	CString strKey;
	CString strValue;

	int GetInt() {return _ttoi(strValue);}
	double GetDouble() {return _ttof(strValue);}
}KeyValue,*PKeyValue;

typedef boost::shared_ptr<KeyValue> ShareKeyValue;

typedef vector<ShareKeyValue> VectorKeyValues,*PVectorKeyValues;

typedef struct _STRUCT_IMG_PROCESS_ALGS
{
	int nAlg;
	VectorKeyValues vAlgs;
}ImgProcessAlgs,*PImgProcessAlgs;

typedef boost::shared_ptr<ImgProcessAlgs> ShareImgProcessAlgs;

typedef vector<ShareImgProcessAlgs> VImgProcessAlgs,*PVImgProcessAlgs;

class CAutoProcessAlgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoProcessAlgDlg)

public:
	CAutoProcessAlgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoProcessAlgDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_ListBoxAlg;
	CListBox m_ListBoxApply;
	CMFCPropertyGridCtrl m_PGCArgs;
	int m_nLastSel;

	PVImgProcessAlgs m_pvImgProcessAlgs;

	void AddImgProcessAlg(int nIndex);
	void RemoveImgProcessAlg(int nItem);
	void MoveImgProcessAlg(int from,int to);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnLbnSelchangeList2();
	afx_msg LRESULT OnPropertyChanged( WPARAM wParam,LPARAM lParam );

	void SetImgProcessAlg(PVImgProcessAlgs pV);
};
