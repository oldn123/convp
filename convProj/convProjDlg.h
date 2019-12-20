
// convProjDlg.h : 头文件
//

#pragma once
#include <map>
#include <string>
#include <vector>
using namespace std;
#include "pugixml/pugixml.hpp"
using namespace pugi;
// CconvProjDlg 对话框
class CconvProjDlg : public CDialogEx
{
// 构造
public:
	CconvProjDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONVPROJ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	map<string, pugi::xml_document> m_docMap;

	vector<string>	m_files;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


protected:
	bool Parse(const wchar_t* sFile, pugi::xml_document *);
	bool FixFile(const wchar_t* sFile, const wchar_t * outfile);

	pugi::xml_document * GetXDoc(const string & sFile);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSln();
};
