
// convProj.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CconvProjApp: 
// �йش����ʵ�֣������ convProj.cpp
//

class CconvProjApp : public CWinApp
{
public:
	CconvProjApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CconvProjApp theApp;