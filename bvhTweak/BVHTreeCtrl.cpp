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
 
// BVHTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "bvhTweak.h"
#include "bvhTweakDlg.h"
#include "BVHTreeCtrl.h"


// CBVHTreeCtrl

IMPLEMENT_DYNAMIC(CBVHTreeCtrl, CTreeCtrl)

CBVHTreeCtrl::CBVHTreeCtrl()
{

}

CBVHTreeCtrl::~CBVHTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CBVHTreeCtrl, CTreeCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BVH_SKELETON, &CBVHTreeCtrl::OnNMCustomdrawBvhSkeleton)
	//ON_WM_PAINT()
END_MESSAGE_MAP()

// CBVHTreeCtrl message handlers
void CBVHTreeCtrl::Initialise(CBVHFile* BVHMotionFile)
{
	// clear out
	DeleteAllItems();

	// get ROOT joint
	BVHJoint* pJoint = BVHMotionFile->GetJoint(0);
	BVHJoint* pLastJoint = pJoint;
	ASSERT(pJoint);
	BVHJoint* pParent = NULL;
	
	// create insert struct
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = pJoint->JOINTName.GetBuffer();		// set name
	
	// insert ROOT joint into TreeCtrl & set heriarchy
	HTREEITEM hParent = InsertItem(&tvInsert);
	SetItemData(hParent, pJoint->heirarchy);		
	int lastJointHeirarchy = pJoint->heirarchy;	
	//int bLastJointWasEndSite = false;
	HTREEITEM hROOT = hParent;
	
	// get each joint in the file and insert it into the tree control
	for(int i=1;i<theApp.m_pBVHFile->GetNJoints();i++) {
		
		// get the next joint and the previous item's hierarchy
		pJoint = BVHMotionFile->GetJoint(i);
		pParent = BVHMotionFile->GetJoint(pJoint->parentIndex);

		ASSERT(pJoint);
		ASSERT(pParent);

		lastJointHeirarchy = (int)GetItemData(hParent);

		// keep heriarchy in check
		while(pJoint->heirarchy<lastJointHeirarchy) {

			lastJointHeirarchy = (int)GetItemData(hParent);
			hParent = GetParentItem(hParent);
		}

		// insert the joint's name and set it's heirarchy
		if(pLastJoint->isEndSite&&pLastJoint->heirarchy==pJoint->heirarchy) {
			hParent = GetParentItem(hParent);
		}
		hParent = InsertItem(pJoint->JOINTName.GetBuffer(), hParent, TVI_LAST);
		lastJointHeirarchy = pJoint->heirarchy;
		SetItemData(hParent, lastJointHeirarchy);
		EnsureVisible(hParent);
		pLastJoint = pJoint;
	}
	Select(hROOT, TVGN_ROOT | TVGN_CARET);//  TVGN_CARET
	SetIndent(30);

	theApp.m_pBVHFile->CalculateFigureHeight();
	
	SelectItem(GetRootItem());


	

	pJoint = NULL;
	pParent = NULL;
	pLastJoint = NULL;


	// change some colours
	m_greycolor=RGB(236,233,216);                      // dialog colour 
	//m_greybrush.CreateSolidBrush(m_greycolor);      // grey background
	//SetTextColor(RGB(255, 255, 255));
	this->SetLineColor(RGB(0, 0, 0));
	//this->select
	SetBkColor(m_greycolor);//RGB(0, 0, 128));

	// force repaint immediately
	Invalidate();
}

CString CBVHTreeCtrl::GetCurrentSelection()
{
	HTREEITEM hCurrent = GetSelectedItem();
	return GetItemText(hCurrent);
}

CString CBVHTreeCtrl::GetCurrentSelParent(void)
{
	HTREEITEM hParent = GetParentItem(GetSelectedItem());
	return GetItemText(hParent);
}


void CBVHTreeCtrl::SetCurrentSelection(CString sJointName)
{
	HTREEITEM hItem = GetFirstVisibleItem();
	CString sItemName = GetItemText(hItem);
	if(sItemName.Compare(sJointName)==0)
	{
		SelectItem(hItem);
		return;
	}
	else
	{
		while(hItem)
		{
			hItem = GetNextVisibleItem(hItem);
			sItemName = GetItemText(hItem);
			if(sItemName.Compare(sJointName)==0){
				SelectItem(hItem);
				return;
			}
		}
	}	
}
void CBVHTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    UINT uFlags;
    HTREEITEM htItem = HitTest(point, &uFlags);
    if ((htItem != NULL) && (uFlags & TVHT_ONITEM)) {
		Select(htItem, TVGN_DROPHILITE);
		CTreeCtrl::SelectItem(htItem);
    }
}

void CBVHTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//UINT uFlags;
	//HTREEITEM htItem = HitTest(point, &uFlags);
	//if ((htItem != NULL) && (uFlags & TVHT_ONITEM)) {
	//	Select(htItem, TVGN_DROPHILITE);
	//}
	CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CBVHTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
    UINT uFlags;
    CPoint ptTree = point;
    ScreenToClient(&ptTree);
    HTREEITEM htItem = HitTest(ptTree, &uFlags);

    if ((htItem != NULL) && (uFlags & TVHT_ONITEM)) {
            ShowPopupMenu( point );
            SetItemState(htItem, 0, TVIS_DROPHILITED);
			//CTreeCtrl::SelectItem(htItem);
    }
	else CTreeCtrl::OnContextMenu(pWnd, point);
}


void CBVHTreeCtrl::ShowPopupMenu( CPoint& point )
{
        if (point.x == -1 && point.y == -1){
                //keystroke invocation
                CRect rect;
                GetClientRect(rect);
                ClientToScreen(rect);

                point = rect.TopLeft();
                point.Offset(5, 5);
        }

        CMenu menu;
        VERIFY(menu.LoadMenu(IDR_JOINT_SUBMENU));

        CMenu* pPopup = menu.GetSubMenu(0);
        ASSERT(pPopup != NULL);
        CWnd* pWndPopupOwner = this;

        while (pWndPopupOwner->GetStyle() & WS_CHILD)
                pWndPopupOwner = pWndPopupOwner->GetParent();

        pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
                pWndPopupOwner);

		
}

// Capture keyboard messages
BOOL CBVHTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	// ignore keyboard commands except arrows up and down
    if(pMsg->message==WM_KEYDOWN)
    {
		if(pMsg->wParam==VK_UP)
		{
			HTREEITEM hCurrentlySelected = GetSelectedItem();
			HTREEITEM hRoot = GetRootItem();
			if(hCurrentlySelected!=hRoot) 
			{
				HTREEITEM hNextItem = GetPrevVisibleItem(hCurrentlySelected);
				SelectItem(hNextItem);
			}
			Invalidate();
			UpdateWindow();
			UpdateData();
			return true;//CWnd::PreTranslateMessage(pMsg);
		}
		else if(pMsg->wParam==VK_DOWN)
		{
			HTREEITEM hCurrentlySelected = GetSelectedItem();
			HTREEITEM hNextItem = GetNextVisibleItem (hCurrentlySelected);
			HTREEITEM hRoot = GetRootItem();
			if(hNextItem!=hRoot&&hNextItem!=NULL) SelectItem(hNextItem);
			if(hNextItem==NULL) SelectItem(hRoot);
			Invalidate();
			UpdateWindow();
			UpdateData();
			return true;//CWnd::PreTranslateMessage(pMsg);
		}
		//else return GetParent()->PreTranslateMessage(pMsg);
    } 
    return CWnd::PreTranslateMessage(pMsg);
}
//
// can only capture prepaint messages here??
//
void CBVHTreeCtrl::OnNMCustomdrawBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR); 

// change text color to white and background to dark blue


  //  
  //  switch (pNMCD->nmcd.dwDrawStage) 
  //  { 
		//case CDDS_PREPAINT: // Request prepaint notifications for each item 
		//	*pResult = CDRF_NOTIFYITEMDRAW; 
		//	break; 


		//case CDDS_ITEMPREPAINT: // Requested notification 
		//	if (pNMCD->nmcd.uItemState == CDIS_SELECTED) 
		//	{ 
		//		*pResult = CDRF_NEWFONT; 
		//	} 
		//	else 
		//		*pResult = CDRF_DODEFAULT; 
		//	break; 


		//default: 
		//	*pResult = CDRF_DODEFAULT; 
		//	break; 
  //  } 

	*pResult = 0;
}


