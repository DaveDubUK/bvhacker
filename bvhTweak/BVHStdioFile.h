#pragma once
#include "afx.h"

class CBVHStdioFile :
	public CStdioFile
{
public:
	CBVHStdioFile(void);
	CBVHStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags);
	~CBVHStdioFile(void);
	BOOL ReadString(CString& rString);
};
