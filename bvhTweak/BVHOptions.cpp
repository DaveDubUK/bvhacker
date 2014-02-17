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


// BVHOptions.cpp : implementation file
//

#include "stdafx.h"
#include "bvhTweak.h"
#include "BVHOptions.h"
#include "afxdialogex.h"


// CBVHOptions dialog

IMPLEMENT_DYNAMIC(CBVHOptions, CDialogEx)

CBVHOptions::CBVHOptions(CbvhTweakDlg* pParent /*=NULL*/)
	: CDialogEx(CBVHOptions::IDD, pParent)
{
	m_pParent = pParent;
}

CBVHOptions::~CBVHOptions()
{
}

void CBVHOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_VIEW_SELECT, mFileViewSelectList);
	DDX_Control(pDX, IDC_SHADING_CHECK, mShadingCheck);
	DDX_Control(pDX, IDC_COLOUR_DISPLAY_CHECK, mColourDisplayCheck);
	DDX_Control(pDX, IDC_SHOW_GROUND_CHECK, mShowGroundPlaneCheck);
	DDX_Control(pDX, IDC_SHOW_HEAD_CHECK, mShowHeadMesh);
}


BEGIN_MESSAGE_MAP(CBVHOptions, CDialogEx)
	ON_CBN_SELCHANGE(IDC_FILE_VIEW_SELECT, &CBVHOptions::OnCbnSelchangeFileViewSelect)
	ON_BN_CLICKED(IDC_SHADING_CHECK, &CBVHOptions::OnBnClickedShadingCheck)
	ON_BN_CLICKED(IDC_SHOW_GROUND_CHECK, &CBVHOptions::OnBnClickedShowGroundCheck)
	ON_BN_CLICKED(IDC_COLOUR_DISPLAY_CHECK, &CBVHOptions::OnBnClickedColourDisplayCheck)
	ON_BN_CLICKED(IDC_SHOW_HEAD_CHECK, &CBVHOptions::OnBnClickedShowHeadCheck)
END_MESSAGE_MAP()


// CBVHOptions message handlers
/*
   {  ODM_VIEW_ICONS = 0x7029,
      ODM_VIEW_LIST  = 0x702b,
      ODM_VIEW_DETAIL= 0x702c,
      ODM_VIEW_THUMBS= 0x702d,
      ODM_VIEW_TILES = 0x702e,

	  */

BOOL CBVHOptions::OnInitDialog(void)
{
	CDialogEx::OnInitDialog();
   
	// set the list of possible file views
	mFileViewSelectList.AddString("Tiles");
	mFileViewSelectList.AddString("Icons");
	mFileViewSelectList.AddString("List");
	mFileViewSelectList.AddString("Details");

	// make sure the currently selected one is displayed
	if(m_pParent->m_pRegistrySettings->DefaultFileView==0x702e) mFileViewSelectList.SetCurSel(0);
	if(m_pParent->m_pRegistrySettings->DefaultFileView==0x7029) mFileViewSelectList.SetCurSel(1);
	if(m_pParent->m_pRegistrySettings->DefaultFileView==0x702b) mFileViewSelectList.SetCurSel(2);
	if(m_pParent->m_pRegistrySettings->DefaultFileView==0x702c) mFileViewSelectList.SetCurSel(3);

	// set the check boxes to match current config
	mShadingCheck.SetCheck(m_pParent->m_pRegistrySettings->DisplayShading);
	mColourDisplayCheck.SetCheck(m_pParent->m_pRegistrySettings->DisplayColour);
	mShowGroundPlaneCheck.SetCheck(m_pParent->m_pRegistrySettings->DisplayGroundPlane);
	mShowHeadMesh.SetCheck(m_pParent->m_pRegistrySettings->DisplayHeadMesh);

	return TRUE;   // return TRUE unless you set the focus to a control
}


void CBVHOptions::OnCbnSelchangeFileViewSelect()
{
	if(mFileViewSelectList.GetCurSel()==0) m_pParent->m_pRegistrySettings->DefaultFileView=0x702e;
	if(mFileViewSelectList.GetCurSel()==1) m_pParent->m_pRegistrySettings->DefaultFileView=0x7029;
	if(mFileViewSelectList.GetCurSel()==2) m_pParent->m_pRegistrySettings->DefaultFileView=0x702b;
	if(mFileViewSelectList.GetCurSel()==3) m_pParent->m_pRegistrySettings->DefaultFileView=0x702c;
}


// pass all checkbox change messages to parent (TweakDlg)
void CBVHOptions::OnBnClickedShadingCheck()
{
	m_pParent->OnViewShading();
}
void CBVHOptions::OnBnClickedShowGroundCheck()
{
	m_pParent->OnBnClickedShowGround();
}
void CBVHOptions::OnBnClickedColourDisplayCheck()
{
	m_pParent->OnBnClickedColourDisplay();
}
void CBVHOptions::OnBnClickedShowHeadCheck()
{
	m_pParent->OnClickedShowHead();
}
