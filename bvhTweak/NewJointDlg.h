/*
	bvhacker - a program for animation previewing and editing

	bvhacker Copyright © 2006 to 2014 David Wooldridge  

		dave@davedub.co.uk
		http://davedub.co.uk/

	This file is part of bvhacker.

    bvhacker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    bvhacker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with bvhacker.  If not, see <http://www.gnu.org/licenses/>.

*/
 
#pragma once
#include "afxwin.h"
#include "resource.h"


// CNewJointDlg dialog

class CNewJointDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewJointDlg)

public:
	CNewJointDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewJointDlg();

// Dialog Data
	enum { IDD = IDD_JOINT_NAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_NewJointName;
	CString m_NewJoint;
	afx_msg void OnBnClickedCancel();
	CString GetName(void);
	BOOL OnInitDialog() ;
};
