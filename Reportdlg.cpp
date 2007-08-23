// Report.cpp : implementation file
//

#include "stdafx.h"
#include "AocTM.h"
#include "Reportdlg.h"
#include "csvfile.h"

// CReport dialog

IMPLEMENT_DYNAMIC(CReportDlg, CDialog)



CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDlg::IDD, pParent), m_pPlayerDB(NULL)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ReportList, m_List);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CReportDlg::OnBnClickedOk)
END_MESSAGE_MAP()

//Calculate and display the fee information for each player, default is OFF
//#define	_AocTM_FEE_

// CReport message handlers
BOOL CReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.InsertColumn( 0, _T("Player Name"), LVCFMT_LEFT, 120, -1);
	m_List.InsertColumn( 1, _T("Play Count"), LVCFMT_LEFT, 70, -1);
	m_List.InsertColumn( 2, _T("Win Count"), LVCFMT_LEFT, 70, -1);
	m_List.InsertColumn( 3, _T("Win Rate"), LVCFMT_LEFT, 80, -1);
	m_List.InsertColumn( 4, _T("Ratings"), LVCFMT_LEFT, 65, -1);
	m_List.InsertColumn( 5, _T("PayedFee"), LVCFMT_LEFT, 60, -1);

	//by mep for statistic
	/*m_List.InsertColumn( 5, _T("Min Feud Time"), LVCFMT_LEFT, 85, -1);
	m_List.InsertColumn( 6, _T("Avg Feud Time"), LVCFMT_LEFT, 85, -1);
	m_List.InsertColumn( 7, _T("Min Cstl Time"), LVCFMT_LEFT, 80, -1);
	m_List.InsertColumn( 8, _T("Avg Cstl Time"), LVCFMT_LEFT, 80, -1);
	m_List.InsertColumn( 9, _T("Min Impl Time"), LVCFMT_LEFT, 80, -1);
	m_List.InsertColumn( 10, _T("Avg Impl Time"), LVCFMT_LEFT, 80, -1);*/


#ifdef	_AocTM_FEE_
	m_List.InsertColumn( 5, _T("��Ǯ"), LVCFMT_LEFT, 80, -1);
#endif

	if(m_pPlayerDB)
	{
		int nItem;
		CString str;
		int totalplays = 0, playcount, wincount, i;

#ifdef	_AocTM_FEE_
		for(i = 0; i < m_pPlayerDB->GetCount(); i++)
			totalplays += m_pPlayerDB->GetAt(i)->PlayCount;
#endif
		for(i = 0; i < m_pPlayerDB->GetCount(); i++)
		{
			playcount = m_pPlayerDB->GetAt(i)->PlayCount;
			if(!playcount)
				continue;
			nItem = m_List.InsertItem(i, m_pPlayerDB->GetAt(i)->NickNames[0]);
			str.Format(_T("%d"), playcount);
			m_List.SetItemText(nItem, 1, str);
			wincount = m_pPlayerDB->GetAt(i)->WinCount;
			str.Format(_T("%d"), wincount);
			m_List.SetItemText(nItem, 2, str);
			str.Format(_T("%d.%d%%"), (100 * wincount / playcount), (10000 * wincount / playcount) % 100);
			m_List.SetItemText(nItem, 3, str);
			str.Format(_T("%d"), m_pPlayerDB->GetAt(i)->Rating);
			m_List.SetItemText(nItem, 4, str);
			str.Format(_T("%d"), m_pPlayerDB->GetAt(i)->GetPaidFee());
			m_List.SetItemText(nItem, 5, str);

			//by mep for statistic

			/*str.Format(_T("%d:%.2d"), m_pPlayerDB->GetAt(i)->MinFeud / 60, m_pPlayerDB->GetAt(i)->MinFeud % 60);
			m_List.SetItemText(nItem, 5, str);
			str.Format(_T("%d:%.2d"), m_pPlayerDB->GetAt(i)->AvgFeud / 60, m_pPlayerDB->GetAt(i)->AvgFeud % 60);
			m_List.SetItemText(nItem, 6, str);
			str.Format(_T("%d:%.2d"), m_pPlayerDB->GetAt(i)->MinCstl / 60, m_pPlayerDB->GetAt(i)->MinCstl % 60);
			m_List.SetItemText(nItem, 7, str);
			str.Format(_T("%d:%.2d"), m_pPlayerDB->GetAt(i)->AvgCstl / 60, m_pPlayerDB->GetAt(i)->AvgCstl % 60);
			m_List.SetItemText(nItem, 8, str);
			str.Format(_T("%d:%.2d"), m_pPlayerDB->GetAt(i)->MinImpl / 60, m_pPlayerDB->GetAt(i)->MinImpl % 60);
			m_List.SetItemText(nItem, 9, str);
			str.Format(_T("%d:%.2d"), m_pPlayerDB->GetAt(i)->AvgImpl / 60, m_pPlayerDB->GetAt(i)->AvgImpl % 60);
			m_List.SetItemText(nItem, 10, str);*/

#ifdef	_AocTM_FEE_
			str.Format(_T("%d"), 400 * playcount / totalplays);
			m_List.SetItemText(nItem, 5, str);
#endif
		}
	}
	return true;
}

void CReportDlg::OnBnClickedOk()
{
	if(!m_pPlayerDB)
		return;

	CFileDialog dlg(FALSE, _T(".csv"), _T("stat.csv"), OFN_OVERWRITEPROMPT, _T("CSV (Comma delimited) | *.csv"));
	if(dlg.DoModal() == IDCANCEL)
		return;
	CString	csvfile = dlg.GetPathName();

	CCsvFile	csv(csvfile, CFile::modeWrite | CFile::modeCreate);

	CStringArray	a;
	const CHeaderCtrl	* header = m_List.GetHeaderCtrl();
	int i;

	HDITEM hdi;
    enum   { sizeOfBuffer = 256 };
    TCHAR  lpBuffer[sizeOfBuffer];
    
    hdi.mask = HDI_TEXT;
    hdi.pszText = lpBuffer;
    hdi.cchTextMax = sizeOfBuffer;

	a.SetSize(header->GetItemCount());

	for(i = 0; i < header->GetItemCount(); i++)
	{
		header->GetItem(i, &hdi);
		a[i] = lpBuffer;
	}
	csv.WriteLine(a);

	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		for(int j = 0; j < m_List.GetColumnCount(); j++)
			a[j] = m_List.GetItemText(i, j);
		csv.WriteLine(a);
	}

	OnOK();
}