
// NQueenDemoDlg.h : header file
//

#pragma once
#include "GASolver.h"

#define WM_THREAD_COMPLETED (WM_USER + 1)
#define WM_THREAD_TERMINATED (WM_USER + 2)

// CNQueenDemoDlg dialog
class CNQueenDemoDlg : public CDialogEx
{
// Construction
public:
	CNQueenDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NQUEENDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	GASolver* instance;
public:
	int m_Board_Size;
	int m_Population_Size;
	int m_Generation_Limit;
	double m_CrossOverProbabilty;
	double m_MutationProbabilty;

	int m_CurrentGeneration;
	const int m_iFramePerSecond = 30;
	const int m_iCountdown = round(1000.0 / (double)m_iFramePerSecond);
	unsigned long long m_StartTime;
	unsigned long long m_StopTime;
	CString m_ElapsedTime;
	DWORD m_nTimer;
	
	CString m_ResultBox;

	void LockControls();
	void UnlockControls();

	CWinThread* m_hThread;
	HANDLE m_hKillEvent;
	UINT ThreadProcGASolverRun();
	static UINT ThreadProcGASolver(LPVOID Param);
	bool m_bIsThreadRunning = false;
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
protected:
	afx_msg LRESULT OnThreadCompleted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadTerminated(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
};
