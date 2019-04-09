
// NQueenDemoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "NQueenDemo.h"
#include "NQueenDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNQueenDemoDlg dialog



CNQueenDemoDlg::CNQueenDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NQUEENDEMO_DIALOG, pParent)
	, instance(new GASolver)
	, m_ResultBox(_T(""))
	, m_Board_Size(0)
	, m_Population_Size(0)
	, m_Generation_Limit(0)
	, m_CrossOverProbabilty(0)
	, m_MutationProbabilty(0)
	, m_CurrentGeneration(0)
	, m_ElapsedTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNQueenDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_ResultBox);
	DDX_Text(pDX, IDC_EDIT1, m_Board_Size);
	DDX_Text(pDX, IDC_EDIT3, m_Population_Size);
	DDX_Text(pDX, IDC_EDIT5, m_Generation_Limit);
	DDX_Text(pDX, IDC_EDIT6, m_CrossOverProbabilty);
	DDX_Text(pDX, IDC_EDIT7, m_MutationProbabilty);
	DDX_Text(pDX, IDC_EDIT4, m_CurrentGeneration);
	DDX_Text(pDX, IDC_EDIT8, m_ElapsedTime);
}

BEGIN_MESSAGE_MAP(CNQueenDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CNQueenDemoDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_THREAD_COMPLETED, &CNQueenDemoDlg::OnThreadCompleted)
	ON_BN_CLICKED(IDC_BUTTON2, &CNQueenDemoDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_THREAD_TERMINATED, &CNQueenDemoDlg::OnThreadTerminated)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CNQueenDemoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNQueenDemoDlg message handlers

BOOL CNQueenDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	m_Board_Size = 8;
	m_Population_Size = 1000;
	m_Generation_Limit = 100;
	m_CrossOverProbabilty = 0.8;
	m_MutationProbabilty = 0.25;

	CFont* myFont = new CFont();
	myFont->CreateFont(16, 0, 0, 0, FW_NORMAL, false, true,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Dejavu Sans Mono"));
	auto pResultBox = GetDlgItem(IDC_EDIT2);
	pResultBox->SetFont(myFont);

	auto pEditBox4 = GetDlgItem(IDC_EDIT4);
	pEditBox4->EnableWindow(0);
	auto pEditBox8 = GetDlgItem(IDC_EDIT8);
	pEditBox8->EnableWindow(0);

	UnlockControls();

	UpdateData(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNQueenDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNQueenDemoDlg::OnPaint()
{
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNQueenDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNQueenDemoDlg::LockControls()
{
	//auto pEditBox1 = GetDlgItem(IDC_EDIT1);
	//pEditBox1->EnableWindow(0);
	auto pButton1 = GetDlgItem(IDC_BUTTON1);
	pButton1->EnableWindow(0);
	auto pButton2 = GetDlgItem(IDC_BUTTON2);
	pButton2->EnableWindow();
}

void CNQueenDemoDlg::UnlockControls()
{
	//auto pEditBox1 = GetDlgItem(IDC_EDIT1);
	//pEditBox1->EnableWindow();
	auto pButton1 = GetDlgItem(IDC_BUTTON1);
	pButton1->EnableWindow();
	auto pButton2 = GetDlgItem(IDC_BUTTON2);
	pButton2->EnableWindow(0);
}

UINT CNQueenDemoDlg::ThreadProcGASolver(LPVOID Param)
{
	CNQueenDemoDlg* curWnd = reinterpret_cast<CNQueenDemoDlg*>(Param);
	curWnd->LockControls();
	return curWnd->ThreadProcGASolverRun();
}

UINT CNQueenDemoDlg::ThreadProcGASolverRun()
{
	instance->Reset();
	m_bIsThreadRunning = true;
	bool isTerminated = false;

	m_StartTime = GetTickCount();
	m_nTimer = SetTimer(1, m_iCountdown, NULL);

;	while (true) {
		DWORD dwRet = WaitForSingleObject(m_hKillEvent, 0);
		if (dwRet == WAIT_OBJECT_0) {
			isTerminated = true;
			m_bIsThreadRunning = false;
			break;
		}
		m_CurrentGeneration = instance->genCnt;
		if (instance->CheckStopCrt()) break;
		instance->Run();
	}

	if (isTerminated) PostMessageW(WM_THREAD_TERMINATED);
	else PostMessageW(WM_THREAD_COMPLETED);
	return 0;
}


void CNQueenDemoDlg::OnBnClickedButton1()
{
	UpdateData();
	m_ResultBox = _T("");
	UpdateData(0);

	auto oldInstance = instance;
	delete oldInstance;
	instance = new GASolver(m_Board_Size, m_Population_Size, m_Generation_Limit, m_CrossOverProbabilty, m_MutationProbabilty);

	m_hKillEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThread = AfxBeginThread(ThreadProcGASolver, this);
}



afx_msg LRESULT CNQueenDemoDlg::OnThreadCompleted(WPARAM wParam, LPARAM lParam)
{
	std::vector<int> res = instance->GetResult();

	if (res.empty()) {
		m_ResultBox = _T("Solution not found! Try again");
	}
	else {
		m_ResultBox = _T("Found a solution:\r\n\r\n");
		for (int i = 0; i < res.size(); i++) {
			CString tmp;
			tmp.Format(_T(" %d ,"), res[i]);
			m_ResultBox += tmp;
		}
		m_ResultBox += _T("\r\n\r\nIllustration:\r\n\r\n");
		for (int i = 0; i < m_Board_Size; i++) {
			for (int j = 0; j < m_Board_Size; j++) {
				if (res[j] == m_Board_Size - 1 - i)
					m_ResultBox += _T(" Q |");
				else if ((i + j) % 2 == 1)
					m_ResultBox += _T(" - |");
				else m_ResultBox += _T("   |");
			}
			m_ResultBox += _T("\r\n");
		}
	}

	m_bIsThreadRunning = false;
	UpdateData(0);
	UnlockControls();
	return 0;
}


void CNQueenDemoDlg::OnBnClickedButton2()
{
	SetEvent(m_hKillEvent);
	DWORD dwRet = WaitForSingleObject(m_hThread->m_hThread, 5000);
	if (dwRet == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThread, 1);
	}
}


afx_msg LRESULT CNQueenDemoDlg::OnThreadTerminated(WPARAM wParam, LPARAM lParam)
{
	m_ResultBox = _T("Terminated! Try again");
	UpdateData(0);
	UnlockControls();
	return 0;
}


void CNQueenDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bIsThreadRunning) {
		m_StopTime = GetTickCount();
		CString val2;
		val2.Format(_T("%f s"), double(m_StopTime - m_StartTime) / 1000.0);
		m_ElapsedTime = val2;
		UpdateData(FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CNQueenDemoDlg::OnBnClickedCancel()
{
	PostQuitMessage(0);
}
