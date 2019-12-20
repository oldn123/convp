
// convProjDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "convProj.h"
#include "convProjDlg.h"
#include "afxdialogex.h"


#include <iostream>
#include <fstream>
#include <thread>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CconvProjDlg 对话框



CconvProjDlg::CconvProjDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONVPROJ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CconvProjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CconvProjDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CconvProjDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SLN, &CconvProjDlg::OnBnClickedButtonSln)
END_MESSAGE_MAP()


// CconvProjDlg 消息处理程序

BOOL CconvProjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码



	auto pList = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST1));
	if (pList)
	{
		pList->SetExtendedStyle(LVS_EX_FULLROWSELECT);
		pList->InsertColumn(0, L"序号", 0, 40);
		pList->InsertColumn(1, L"文件", 0, 800);
	}

	SetDlgItemText(IDC_EDIT_FIXNAME, L"test");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CconvProjDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CconvProjDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CconvProjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

pugi::xml_document * CconvProjDlg::GetXDoc(const string & sFile)
{
	auto f = m_docMap.find(sFile);
	if (f != m_docMap.end())
	{
		return &f->second;
	}
	return &m_docMap[sFile];
}

void ModiOutConfig(pugi::xml_document * pDoc, const string &)
{

}

bool CconvProjDlg::FixFile(const wchar_t* sFile, const wchar_t * outfile)
{
	USES_CONVERSION;
	auto pDoc = GetXDoc(T2A(sFile));

	if (!Parse(sFile, pDoc))
	{
		return false;
	}

	AtlTrace(sFile);
	AtlTrace(L"\n");

	std::wstring strLink = L"/Project/ItemDefinitionGroup/Link";
	const wchar_t * strOutput = L"OutputFile";
	try
	{
		pugi::xpath_query query(strLink.c_str());
		pugi::xpath_value_type tt = query.return_type();
		if (tt == pugi::xpath_type_node_set)
		{
			pugi::xpath_node_set node_set = query.evaluate_node_set(pDoc->first_child());
			if (node_set.empty())
			{
				AtlTrace(L"未找到指定的xpath：%s", strLink.c_str());
				return false;
			}

			for (auto nodeItem = node_set.begin(); node_set.end() != nodeItem; nodeItem++)
			{
				auto fd = nodeItem->node().find_child([strOutput](xml_node node) {
					if (_tcscmp(node.name(), strOutput) == 0)
					{
						return true;
					}
					return false;
				});
				
				AtlTrace(nodeItem->parent().attribute(L"Condition").as_string());
				AtlTrace(L"------------");

				if (!fd.empty())
				{
					AtlTrace(fd.first_child().value());
					fd.first_child().set_value(outfile);
				}
				else
				{
					AtlTrace(L"can't find.");
					fd = nodeItem->node().append_child(strOutput);
					if (!fd.empty())
					{
						fd.first_child().set_value(outfile);
					}
				}
				AtlTrace(L"\n");
			}
		}

		wstring val = query.evaluate_string(pDoc->first_child());

		if (!query.return_type())
		{
			int x = 0;
		}
	}
	catch (const pugi::xpath_exception& )
	{
		TRACE(L"xpath查询错误：%s", strLink.c_str());
		return false;
	}

	return true;
}

bool CconvProjDlg::Parse(const wchar_t* sFile, pugi::xml_document * pDoc)
{
	if (sFile == NULL ||
		pDoc == NULL)
		return false;

	//加载文件
	if (!pDoc->load_file(sFile))
	{
		return false;
	}

	return true;
}

bool GetFilePath(const string & sBase, string & sPath)
{
	auto fd1 = sBase.rfind("\\");
	auto fd2 = sBase.rfind("/");
	auto fd3 = fd1;
	if (fd1 != sBase.npos && fd2 != sBase.npos)
	{
		fd3 = fd1 > fd2 ? fd1 : fd2;
	}
	else if (fd1 != sBase.npos)
	{
		fd3 = fd1;
	}
	else if (fd2 != sBase.npos)
	{
		fd3 = fd2;
	}
	else
	{
		return false;
	}

	sPath = sBase.substr(0, fd3 + 1);
	return true;
}

void GetFileList(const string & sBase,  vector<string> & sFiles)
{
	string sPath;
	if (sBase.find(".sln") != sBase.npos)
	{
		if (!GetFilePath(sBase, sPath))
		{
			return;
		}

		fstream f2;
		f2.open(sBase.c_str(), ios::in);

		char sbuf[1024];
		string sKeyWord = ".vcxproj\"";
		while (!f2.eof())
		{
			f2.getline(sbuf,  1024);    //按行读取f2中的数据
			OutputDebugStringA(sbuf);
			OutputDebugStringA("\n");
			string sData = sbuf;
			auto fd = sData.find(sKeyWord);
			if (fd != sData.npos)
			{
				auto fd0 = sData.rfind("\"", fd);
				if (fd0 != sData.npos)
				{
					auto subItem = sData.substr(fd0 + 1, fd - fd0 + sKeyWord.size() - 2);
					sFiles.push_back(sPath + subItem);
				}
			}
		}
		f2.close();
	}
	else
	{

	}
}

void RefreshFileList(string sPath, vector<string> & files, CListCtrl * pList)
{
	files.clear();
	GetFileList(sPath, std::ref(files));
	USES_CONVERSION;
	wchar_t sBuf[20] = {0};
	int nIdx = 0;
	for (auto iter : files)
	{
		nIdx++;
		_stprintf_s(sBuf, 20, L"%d", nIdx);
		auto idx = pList->InsertItem(0x7FFFF, sBuf, 0);
		pList->SetItemText(idx, 1, A2T(iter.c_str()));
	}
}

void CconvProjDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	CString sText;
	GetDlgItemText(IDC_EDIT_FIXNAME, sText);
	USES_CONVERSION;
	for (auto iter : m_files)
	{
		FixFile(A2T(iter.c_str()), sText);
	}
}


void CconvProjDlg::OnBnClickedButtonSln()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT	uFlag = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER
		| OFN_HIDEREADONLY | OFN_READONLY;


	CString strFilter = L".sln|*.sln";
	strFilter += _T("|");

	CFileDialog dlg(TRUE,
		_T("*.*"),
		NULL,
		uFlag,
		strFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_INPUT, dlg.GetPathName());

		USES_CONVERSION;
		thread t(RefreshFileList, T2A(dlg.GetPathName()), std::ref(m_files), static_cast<CListCtrl*>(GetDlgItem(IDC_LIST1)));
		t.detach();
	}


}
