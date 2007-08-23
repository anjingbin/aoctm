// ATMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATM.h"
#include "ATMDlg.h"
#include "playerdatabase.h"
#include "reportdlg.h"
#include "groupingdlg.h"
#include "progresswnd.h"
#include "Statdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CATMDlg dialog




CATMDlg::CATMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CATMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CATMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CATMDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_INIT, &CATMDlg::OnBnClickedInit)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_VIEWHISTORY, &CATMDlg::OnViewHistory)
	ON_COMMAND(ID_STATISTIC, &CATMDlg::OnStatistic)
	ON_COMMAND(ID_GROUPING, &CATMDlg::OnGrouping)
END_MESSAGE_MAP()


// CATMDlg message handlers

BOOL CATMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	GdiplusStartup(&gdiplusToken,   &gdiplusStartupInput,   NULL);     //FRED
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CATMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//fred
void CATMDlg::OnPaint()
{
	 Graphics   graphics(GetDC()->m_hDC);     
	 //Image   image(L"s1092909154.jpg"); 
	 Image   image(_T("..//s1092909154.jpg"),false); 
     graphics   .DrawImage(   &image,   0,   0   );   
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CATMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* pubb, 070724, the first function implementation */

void CATMDlg::OnDropFiles(HDROP hDropInfo)
{
	int	count =	DragQueryFile(hDropInfo,0xffffffff,NULL,0);
	TCHAR	strPath[4096];

	/* the first function, to count each player's play times */

	CRecgame *rg;
	CPlayerDatabase	thisplayers;		//store and show the effect of this Drag&Drop

	CProgressWnd	wndProgress;

	wndProgress.Create(this, _T("Progress..."), TRUE);
	wndProgress.SetRange(0,count);
	wndProgress.SetText(_T("Loading Recorded Game files..."));

	//by mep for performance
	theApp.Engine->BeginTx();

	for(int i = 0; i < count; i++)
	{
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled())
		{
			break;
		}

		DragQueryFile(hDropInfo,i,strPath,4096);

		rg = new CRecgame;

		if(!rg->Read(strPath))
		{
			delete rg;
			continue;
		}
		
		if(rg->PlayTime < TIME_4_INCOMPLETE)	//if play time less than 20 min, consider it an INCOMPLETE game
		{
			delete rg;
			continue;
		}

		//pubb, 07-08-02, Add to database and update rating in Add()
		/*
		theApp.Players.CountPlay(rg);
		theApp.Players.CountWin(rg);
		theApp.Players.UpdateRatings(rg);
		*/
		theApp.Recgames.Add(rg);

		thisplayers.Add(rg);
	}

	theApp.Engine->Commit();

	theApp.Recgames.UpdatePlayerDatabase(theApp.Engine);

	ShowReport(&thisplayers);

	wndProgress.Hide();

	CDialog::OnDropFiles(hDropInfo);
}

int CATMDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	DragAcceptFiles();

//	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

	return 0;
}

void CATMDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	/*CGroupingDlg	dlg;
	
	dlg.DoModal();*/
	
	CDialog::OnRButtonUp(nFlags, point);

	CMenu menu, *pSubMenu;
	if (!menu.LoadMenu(IDR_MENU1))
		return;
	if (!(pSubMenu = menu.GetSubMenu(0)))
		return;
	CPoint pos;
	GetCursorPos(&pos);
	SetForegroundWindow();  

	::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, 
					 GetSafeHwnd(), NULL);
	PostMessage(WM_NULL, 0, 0);
	menu.DestroyMenu();

}

void CATMDlg::OnBnClickedInit()
{
	theApp.Players.Reset(theApp.Engine);
}

void CATMDlg::OnClose()
{
	GdiplusShutdown(gdiplusToken); //fred

	CDialog::OnClose();
}

void CATMDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ShowReport(&theApp.Players);

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CATMDlg::ShowReport(CPlayerDatabase * players)
{
	CReportDlg	dlg;
	dlg.m_pPlayerDB = players;
	dlg.DoModal();
}

void CATMDlg::OnViewHistory()
{
	// TODO: Add your command handler code here
	ShowReport(&theApp.Players);
}

void CATMDlg::OnStatistic()
{
	// TODO: Add your command handler code here
	Statdlg dlg;
	dlg.m_pPlayerDB = &theApp.Players;
	dlg.DoModal();
}

void CATMDlg::OnGrouping()
{
	// TODO: Add your command handler code here
	CGroupingDlg	dlg;
	
	dlg.DoModal();
}