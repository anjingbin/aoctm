#pragma once
#include "xlistctrl\xlistctrl.h"


// CFeeDlg dialog

class CFeeDlg : public CDialog
{
	DECLARE_DYNAMIC(CFeeDlg)

public:
	CFeeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFeeDlg();

// Dialog Data
	enum { IDD = IDD_FEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog(void);
	void	Refresh(void);

public:
	CXListCtrl m_List;

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
