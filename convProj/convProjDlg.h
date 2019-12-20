
// convProjDlg.h : ͷ�ļ�
//

#pragma once
#include <map>
#include <string>
#include <vector>
using namespace std;
#include "pugixml/pugixml.hpp"
using namespace pugi;
// CconvProjDlg �Ի���
class CconvProjDlg : public CDialogEx
{
// ����
public:
	CconvProjDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONVPROJ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	map<string, pugi::xml_document> m_docMap;

	vector<string>	m_files;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
