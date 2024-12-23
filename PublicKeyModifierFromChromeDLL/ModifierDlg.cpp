
// ModifierDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Modifier.h"
#include "ModifierDlg.h"
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


// CModifierDlg dialog



CModifierDlg::CModifierDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODIFIER_DIALOG, pParent)
	, m_strPublicKey(_T(""))
	, m_strInFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PUBLIC_KEY, m_strPublicKey);
	DDX_Text(pDX, IDC_EDIT_INFILE, m_strInFile);
}

BEGIN_MESSAGE_MAP(CModifierDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CModifierDlg::OnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CModifierDlg::OnClickedButtonBrowse)
END_MESSAGE_MAP()


// CModifierDlg message handlers

BOOL CModifierDlg::OnInitDialog()
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

void CModifierDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CModifierDlg::OnPaint()
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
HCURSOR CModifierDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CModifierDlg::OnClickedButtonModify()
{
	// TODO: Add your control notification handler code here
	DWORD offset = 0xCE7C729;  //offset of starting position of public key(-----BEGIN PUBLIC KEY-----)
	UpdateData(TRUE);

	ReplaceStringInDLL(m_strInFile, offset, m_strPublicKey);
	
}


void CModifierDlg::OnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	// Define the filter for the dialog
	CString filter = _T("Executable Files (*.exe;*.dll)|*.exe;*.dll|All Files (*.*)|*.*||");

	// Create the file dialog
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filter);

	// Show the dialog
	if (fileDlg.DoModal() == IDOK)
	{
		// Get the selected file name
		CString selectedFilePath = fileDlg.GetPathName();

		// Optionally, you can also get the file name without the path
		CString selectedFileName = fileDlg.GetFileName();

		// Use the file path or name as needed
		m_strInFile = selectedFilePath;

		UpdateData(FALSE);
	}
}

void CModifierDlg::ReplaceStringInDLL(const CString& dllPath, DWORD offset, const CString& newValue) {
	// Convert CString to std::string
	CT2A ansiPath(dllPath); // Convert CString to ANSI string
	std::string stdDllPath(ansiPath);

	// Open the DLL file in binary mode
	std::fstream file(stdDllPath, std::ios::binary | std::ios::in | std::ios::out);

	if (!file.is_open()) {
		AfxMessageBox(L"Failed to open file: " + dllPath);
		return;
	}

	// Seek to the specified offset
	file.seekp(offset);

	// Write the new string to the file
	CString modifiedValue = newValue;
	modifiedValue.Replace(_T("\r\n"), _T("\n"));
	CStringA ansiStr(modifiedValue);
	file.write(ansiStr, ansiStr.GetLength());
/*
	// Optionally fill the remaining space with null characters if needed
	std::streampos endPos = file.tellp();
	DWORD originalSize = newValue.GetLength(); // Set this to the original string's size

	if (endPos < originalSize) {
		// Fill with null characters to maintain size
		size_t paddingSize = originalSize - endPos;
		std::string padding(paddingSize, '\0');
		file.write(padding.c_str(), padding.size());
	}

	// Close the file*/
	file.close();
	AfxMessageBox(L"String replaced successfully!");
}
