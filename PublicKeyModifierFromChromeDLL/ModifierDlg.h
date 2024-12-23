
// ModifierDlg.h : header file
//

#pragma once
#include <fstream>

// CModifierDlg dialog
class CModifierDlg : public CDialogEx
{
// Construction
public:
	CModifierDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODIFIER_DIALOG };
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
public:
	CString m_strPublicKey;
	CString m_strInFile;
	afx_msg void OnClickedButtonModify();
	afx_msg void OnClickedButtonBrowse();
	void ReplaceStringInDLL(const CString& dllPath, DWORD offset, const CString& newValue);
};
