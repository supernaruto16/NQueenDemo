
// NQueenDemoDlg.h : header file
//

#pragma once
#include "GASolver.h"


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
	void LockControls();
	void UnlockControls();
	afx_msg void OnBnClickedButton1();
	CString m_ResultBox;
	int m_Board_Size;
	int m_Population_Size;
	int m_Generation_Limit;
	double m_CrossOverProbabilty;
	double m_MutationProbabilty;
};
