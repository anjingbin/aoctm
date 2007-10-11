// GroupingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AocTM.h"
#include "GroupingDlg.h"
#include "grouping/grouping.h"
#include "playerinputdlg.h"

// CGroupingDlg dialog

IMPLEMENT_DYNAMIC(CGroupingDlg, CDialog)

CGroupingDlg::CGroupingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupingDlg::IDD, pParent), maindlg(NULL)
{
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
}

CGroupingDlg::~CGroupingDlg()
{
}

void CGroupingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYERS, m_Players);
	DDX_Control(pDX, IDC_SELECTED, m_Selected);
	DDX_Control(pDX, IDC_GROUP1, m_Group1);
	DDX_Control(pDX, IDC_GROUP2, m_Group2);
}


BEGIN_MESSAGE_MAP(CGroupingDlg, CDialog)
//	ON_WM_CREATE()
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYERS, &CGroupingDlg::OnNMDblclkPlayers)
	ON_BN_CLICKED(IDOK, &CGroupingDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_SELECTED, &CGroupingDlg::OnNMDblclkSelected)
	ON_BN_CLICKED(IDC_RESET, &CGroupingDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_NEW, &CGroupingDlg::OnBnClickedInput)
	ON_BN_CLICKED(IDC_KEEP, &CGroupingDlg::OnBnClickedKeep)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_DBLCLK, IDC_GROUP1, &CGroupingDlg::OnNMDblclkGroup1)
	ON_NOTIFY(NM_DBLCLK, IDC_GROUP2, &CGroupingDlg::OnNMDblclkGroup2)
	ON_COMMAND(ID_ACCELERATOR_LOAD, (AFX_PMSG)&CGroupingDlg::OnAcceleratorLoad)
END_MESSAGE_MAP()


// CGroupingDlg message handlers

BOOL CGroupingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Players.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 120, -1);

	m_Selected.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 155, -1);
	m_Selected.InsertColumn(1, _T("Rating"), LVCFMT_LEFT, 155, -1);

	m_Group1.InsertColumn(0, _T(""), LVCFMT_LEFT, 120);
	m_Group2.InsertColumn(0, _T(""), LVCFMT_LEFT, 120);

	Refresh();

	return TRUE;
}

void CGroupingDlg::OnNMDblclkPlayers(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	NMLISTVIEW *p = (NMLISTVIEW *)pNMHDR;
	if(p->iItem < 0)
		return;

	CString	name = m_Players.GetItemText(p->iItem, 0);
	int	index = m_Players.GetItemData(p->iItem);
	CString	str;

	int selected = m_Selected.GetItemCount();
	
	if(selected >= 8)
		return;

	for(int i = 0; i < selected; i++)
	{
		if(index == m_Selected.GetItemData(i))
			return;
	}
	
	InsertSelected(name, theApp.Players[index]->Rating, index);
}

void CGroupingDlg::OnBnClickedOk()
{
	int ratings[8];
	int group1[4];
	int group2[4];
	int group1TotalScore;
	int group2TotalScore;
	int delta;

	int i, selected = m_Selected.GetItemCount();

	if(selected <= 0)
		return;

	//huangjie, 07-08-03, only even count is permitted
	if(selected % 2 != 0)
	{
		AfxMessageBox(_T("even player count is permitted only"), MB_OK);
		return;
	}

	memset(group1, 0, sizeof(group1));
	memset(group2, 0, sizeof(group2));


	for(i = 0; i < selected; i++)
		ratings[i] = _ttoi(m_Selected.GetItemText(i, 1));
	
	Intermediator	inter(selected, ratings);
	inter.Grouping(group1, group2, group1TotalScore, group2TotalScore, delta);

	m_Group1.DeleteAllItems();
	m_Group2.DeleteAllItems();

	int j1 = 0, j2 = 0;

	for(i = 0; i < 4 /* XXX, maybe less than */; i++)
	{
		if(group1[i] > 0)
		{
			m_Group1.InsertItem(j1, m_Selected.GetItemText(group1[i] - 1, 0));
			m_Group1.SetItemData(j1++, group1[i] - 1);
		}
		if(group2[i] > 0)
		{
			m_Group2.InsertItem(j2, m_Selected.GetItemText(group2[i] - 1, 0));
			m_Group2.SetItemData(j2++, group2[i] - 1);
		}
	}

	CString	str;
	str.Format(_T("Group1: %d, Group2: %d, Delta Rating: %d"), group1TotalScore, group2TotalScore, delta);
	GetDlgItem(IDC_MSG)->SetWindowText(str);
}

void CGroupingDlg::OnNMDblclkSelected(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW *p = (NMLISTVIEW *)pNMHDR;

	if(p->iItem >= 0)
	{
		m_Selected.DeleteItem(p->iItem);
	}

	*pResult = 0;
}

void CGroupingDlg::OnBnClickedReset()
{
	m_Selected.DeleteAllItems();
	theApp.ClearCurrentPlayers();

	m_Group1.DeleteAllItems();
	m_Group2.DeleteAllItems();
	GetDlgItem(IDC_MSG)->SetWindowText(_T("Double click, Double click, Double click, ..."));
}

void CGroupingDlg::OnBnClickedInput()
{
	CPlayerInputDlg	dlg;
	if(dlg.DoModal() == IDOK)
	{
		INT_PTR index = theApp.Players.GetFirstSamePlayer(dlg.m_Name);
		if(index < 0)
		{
			CPlayer	*p = new CPlayer();
			p->NickNames.Add(dlg.m_Name);
			p->Rating = p->InitRating = dlg.m_iRating;
			index = theApp.Players.Add(p);
			InsertPlayers(dlg.m_Name, (int)index);
		}
		else	//update the rating to the input one
		{
			theApp.Players[index]->Rating = dlg.m_iRating;
			if(UpdateSelected(dlg.m_iRating, (int)index))
				return;
		}
		InsertSelected(dlg.m_Name, dlg.m_iRating, (int)index);
		
		
	}
}

void CGroupingDlg::OnBnClickedKeep()
{
	int selected = m_Selected.GetItemCount();

	if(selected == 0)
		return;

	theApp.ClearCurrentPlayers();

	for(int i = 0; i < selected; i++)
		theApp.CurrentPlayers[i] = m_Selected.GetItemData(i);

	theApp.bCurrent = true;
}

void CGroupingDlg::InsertPlayers(CString name, int data)
{
	int index = m_Players.GetItemCount();
	m_Players.InsertItem(index, name);
	m_Players.SetItemData(index, data);	//to save index to the PlayerDB
}

void CGroupingDlg::InsertSelected(CString name, int rating, int data)
{
	int selected = m_Selected.GetItemCount();

	if(selected >= 8)
		return;

	CString	str;

	m_Selected.InsertItem(selected, name);
	str.Format(_T("%d"), rating);
	m_Selected.SetItemText(selected, 1, str);
	m_Selected.SetItemData(selected, data);
}

bool CGroupingDlg::UpdateSelected(int rating, int data)
{
	CString str;
	for(int i = 0; i < m_Selected.GetItemCount(); i++)
	{
		if(m_Selected.GetItemData(i) == data)
		{
			str.Format(_T("%d"), rating);
			m_Selected.SetItemText(i, 1, str);
			return true;
		}
	}
	return false;
}

void CGroupingDlg::Refresh(void)
{
	m_Players.DeleteAllItems();
	m_Selected.DeleteAllItems();

	int i, index;
	for(i = 0; i < theApp.Players.GetCount(); i++)
		InsertPlayers(theApp.Players[i]->NickNames[0], i);

	if(theApp.bCurrent)
	{
		for(i = 0; i < 8 && theApp.CurrentPlayers[i] >= 0; i++)
		{
			index = theApp.CurrentPlayers[i];
			InsertSelected(theApp.Players[index]->NickNames[0], theApp.Players[index]->Rating, index);
		}
	}

	int group1 = 0, group2 = 0;
	for(int i = 0; i < m_Group1.GetItemCount(); i++)
	{
		group1 += _ttoi(m_Selected.GetItemText((int)m_Group1.GetItemData(i), 1));
		group2 += _ttoi(m_Selected.GetItemText((int)m_Group2.GetItemData(i), 1));
	}
	CString	str;
	if(m_Group1.GetItemCount() == 0)
		str = _T("Double click, Double click, Double click, ...");
	else
		str.Format(_T("Group1: %d, Group2: %d, Delta Rating: %d"), group1, group2, group1 - group2);
	GetDlgItem(IDC_MSG)->SetWindowText(str);
}

int CGroupingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	DragAcceptFiles();

	return 0;
}

void CGroupingDlg::OnDropFiles(HDROP hDropInfo)
{
	if(maindlg)
		maindlg->OnDropFiles(hDropInfo);

	Refresh();

	CDialog::OnDropFiles(hDropInfo);
}

void CGroupingDlg::OnExchange(int index1, int index2)
{
	int j1 = (int)m_Group1.GetItemData(index1), j2 = (int)m_Group2.GetItemData(index2);

	m_Group1.SetItemText(index1, 0, m_Selected.GetItemText(j2, 0));
	m_Group1.SetItemData(index1, j2);
	m_Group2.SetItemText(index2, 0, m_Selected.GetItemText(j1, 0));
	m_Group2.SetItemData(index2, j1);

	int group1 = 0, group2 = 0;
	for(int i = 0; i < m_Group1.GetItemCount() /* pubb, 07-09-04, if odd players? */; i++)
	{
		group1 += _ttoi(m_Selected.GetItemText((int)m_Group1.GetItemData(i), 1));
		group2 += _ttoi(m_Selected.GetItemText((int)m_Group2.GetItemData(i), 1));
	}

	CString	str;
	str.Format(_T("Group1: %d, Group2: %d, Delta Rating: %d"), group1, group2, group1 - group2);
	GetDlgItem(IDC_MSG)->SetWindowText(str);
}

void CGroupingDlg::OnNMDblclkGroup1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW *p = (NMLISTVIEW *)pNMHDR;
	*pResult = 0;

	if(p->iItem < 0)
		return;
	
	POSITION pos = m_Group2.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;

	OnExchange(p->iItem, m_Group2.GetNextSelectedItem(pos));
}

void CGroupingDlg::OnNMDblclkGroup2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW *p = (NMLISTVIEW *)pNMHDR;
	*pResult = 0;

	if(p->iItem < 0)
		return;
	
	POSITION pos = m_Group1.GetFirstSelectedItemPosition();
	if(pos == NULL)
		return;

	OnExchange(m_Group1.GetNextSelectedItem(pos), p->iItem);
}

void CGroupingDlg::OnAcceleratorLoad()
{
	if(maindlg && maindlg->OnAcceleratorLoad())
		Refresh();
}

BOOL   CGroupingDlg::PreTranslateMessage(MSG*   pMsg)
{   
	if(m_hAccelTable)
	{
		if(::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return(TRUE);
		}   
	}   
	return   CDialog::PreTranslateMessage(pMsg);   
}