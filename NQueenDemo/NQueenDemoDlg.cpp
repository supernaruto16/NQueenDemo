
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNQueenDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_ResultBox);
	DDX_Text(pDX, IDC_EDIT1, m_Board_Size);
}

BEGIN_MESSAGE_MAP(CNQueenDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CNQueenDemoDlg::OnBnClickedButton1)
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
	auto pEditBox1 = GetDlgItem(IDC_EDIT1);
	pEditBox1->EnableWindow(0);
	auto pButton1 = GetDlgItem(IDC_BUTTON1);
	pButton1->EnableWindow(0);
}

void CNQueenDemoDlg::UnlockControls()
{
	auto pEditBox1 = GetDlgItem(IDC_EDIT1);
	pEditBox1->EnableWindow();
	auto pButton1 = GetDlgItem(IDC_BUTTON1);
	pButton1->EnableWindow();
}


void CNQueenDemoDlg::OnBnClickedButton1()
{
	LockControls();

	UpdateData();
	m_ResultBox = _T("");
	UpdateData(0);

	instance->Reset();
	instance->SetBoardSize(m_Board_Size);
	instance->Run();
	std::vector<int> res = instance->GetResult();

	if (res.empty()) {
		m_ResultBox = _T("Solution not found! Try again");

	}
	else {
		for (int i = 0; i < m_Board_Size; i++) {
			for (int j = 0; j < m_Board_Size; j++) {
				if (res[j] == m_Board_Size - 1 - i)
					m_ResultBox += _T("Q");
				else m_ResultBox += _T("*");
			}
			m_ResultBox += _T("\r\n");
		}
	}
	UnlockControls();

	UpdateData(0);
}
