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
 
// NewJointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bvhTweak.h"
#include "NewJointDlg.h"


// CNewJointDlg dialog

IMPLEMENT_DYNAMIC(CNewJointDlg, CDialog)

CNewJointDlg::CNewJointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewJointDlg::IDD, pParent)
{
}

CNewJointDlg::~CNewJointDlg()
{
}

void CNewJointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_JOINT_NAME, m_NewJointName);
}

BOOL CNewJointDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();

   m_NewJoint = _T("");
   m_NewJointName.SetWindowText(m_NewJoint);
   ::SetFocus(m_NewJointName.m_hWnd);

   return FALSE;   // return TRUE unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(CNewJointDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNewJointDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNewJointDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNewJointDlg message handlers

void CNewJointDlg::OnBnClickedOk()
{
	m_NewJointName.GetWindowText(m_NewJoint);
	OnOK();
}

void CNewJointDlg::OnBnClickedCancel()
{
	m_NewJoint = _T("default");
	OnCancel();
}

CString CNewJointDlg::GetName(void)
{
	return m_NewJoint;
}
