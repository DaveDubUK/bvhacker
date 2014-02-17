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
#include "bvhTweakDlg.h"

// CBVHOptions dialog

class CBVHOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CBVHOptions)

public:
	CBVHOptions(CbvhTweakDlg* pParent);   // standard constructor
	virtual ~CBVHOptions();

// Dialog Data
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox mFileViewSelectList;
	BOOL OnInitDialog(void);
	afx_msg void OnCbnSelchangeFileViewSelect();

private:
	CbvhTweakDlg* m_pParent;
public:
	afx_msg void OnBnClickedShadingCheck();
	afx_msg void OnBnClickedShowGroundCheck();
	afx_msg void OnBnClickedColourDisplayCheck();
	CButton mShadingCheck;
	CButton mColourDisplayCheck;
	CButton mShowGroundPlaneCheck;
	CButton mShowHeadMesh;
	afx_msg void OnBnClickedShowHeadCheck();
};
