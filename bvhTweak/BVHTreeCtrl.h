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
 
// CBVHTreeCtrl class

#pragma once

#include "BVHFile.h"

class CBVHFile;

class CBVHTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CBVHTreeCtrl)

public:
	CBVHTreeCtrl();
	virtual ~CBVHTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	// message handlers
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
public:
	afx_msg void OnNMCustomdrawBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnPaint();

public:
	void Initialise(CBVHFile* BVHMotionFile);
	CString GetCurrentSelection(void);
	CString GetCurrentSelParent(void);
	void SetCurrentSelection(CString sJointName);
	void ShowPopupMenu(CPoint& point);
	
	//MFC nonsense
	BOOL PreTranslateMessage(MSG* pMsg);

protected:
	//CBrush m_greybrush;
	COLORREF m_greycolor;
};