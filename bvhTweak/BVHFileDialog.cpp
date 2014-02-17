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


// BVHFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "bvhTweak.h"
#include "BVHFileDialog.h"


// CBVHFileDialog

IMPLEMENT_DYNAMIC(CBVHFileDialog, CFileDialog)
	
BEGIN_MESSAGE_MAP(CBVHFileDialog, CFileDialog)
   ON_MESSAGE(MYWM_POSTINIT, OnPostInit)
END_MESSAGE_MAP()

CBVHFileDialog::CBVHFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, LISTVIEWCMD DefaultView,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.lpstrTitle = _T("bvhacker: select a bvh file");
	mCurrentView = DefaultView;
}

CBVHFileDialog::~CBVHFileDialog()
{
}





// CBVHFileDialog message handlers



BOOL CBVHFileDialog::OnInitDialog(void)
{
   //CFileDialog::OnInitDialog();
   //PostMessage(MYWM_POSTINIT,0,0);
   //return TRUE;

   //TRACE(_T("CMyOpenDlg::OnInitDialog, hwnd=%p\n"), m_hWnd);
   CFileDialog::OnInitDialog();
   SetListView(mCurrentView); // this will fail
   PostMessage(MYWM_POSTINIT,0,0);
   return TRUE;
}

//////////////////
// Handle CDN_INITDONE: shows that list view still not created yet.
//
void CBVHFileDialog::OnInitDone()
{
   //TRACE(_T("CMyOpenDlg::OnInitDone\n"));
   CFileDialog::OnInitDone();
   SetListView(mCurrentView); // this will fail - too early to change the view?
}

//////////////////
// Handle MYWN_POSTINIT: finally, the list view is created.
//
LRESULT CBVHFileDialog::OnPostInit(WPARAM wp, LPARAM lp)
{
   //TRACE(_T("CMyOpenDlg::OnPostInit\n"));
   SetListView(mCurrentView); // this will succeed

   /*  ODM_VIEW_ICONS = 0x7029,
      ODM_VIEW_LIST  = 0x702b,
      ODM_VIEW_DETAIL= 0x702c,
											ODM_VIEW_THUMBS= 0x702d,
      ODM_VIEW_TILES = 0x702e,
	  */
   return 0;
}

//////////////////
// Change the list view to desired mode if the view exists.
// Display TRACE diagnostics either way.
//
BOOL CBVHFileDialog::SetListView(LISTVIEWCMD cmd)
{
   //TRACE(_T("CMyOpenDlg::SetListView: "));

   // note that real dialog is my parent, not me
   CWnd* pshell = GetParent()->GetDlgItem(lst2);
   if (pshell) {
      //TRACE(_T("hwnd=%p.\n"), m_wndList.GetSafeHwnd());
      pshell->SendMessage(WM_COMMAND, cmd);
      return TRUE;
   }
   //TRACE(_T("failed.\n"),m_wndList.GetSafeHwnd());
   return FALSE;
}
