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

#include "dlgs.h"

// self-initialization message posted
const int MYWM_POSTINIT = WM_USER+1;

// CBVHFileDialog

class CBVHFileDialog : public CFileDialog
{
public:
   // reverse-engineered command codes for SHELLDLL_DefView
   enum LISTVIEWCMD
   {  ODM_VIEW_ICONS = 0x7029,
      ODM_VIEW_LIST  = 0x702b,
      ODM_VIEW_DETAIL= 0x702c,
      ODM_VIEW_THUMBS= 0x702d,
      ODM_VIEW_TILES = 0x702e,
   };	

	CBVHFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		LISTVIEWCMD DefaultView = ODM_VIEW_DETAIL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~CBVHFileDialog();

protected:
   CListCtrl m_wndList;          // list view showing files and folders

   enum { ID_LISTVIEW = lst2 };



   virtual BOOL OnInitDialog();         // handle init dialog
   virtual void OnInitDone();           // handle CDN_INITDONE
   afx_msg LRESULT OnPostInit(WPARAM wp, LPARAM lp); // the REAL 
                                                     // initialization
   LISTVIEWCMD mCurrentView;
   BOOL SetListView(LISTVIEWCMD cmd);


   DECLARE_MESSAGE_MAP()
   DECLARE_DYNAMIC(CBVHFileDialog)
//public:
	//BOOL OnInitDialog(void);
};


