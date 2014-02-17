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
 
#include "stdafx.h"
#include "NiceSlider.h"
#include "bvhTweakDlg.h"


IMPLEMENT_DYNAMIC(CNiceSliderCtrl, CSliderCtrl)


BEGIN_MESSAGE_MAP(CNiceSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CNiceSliderCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CNiceSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CbvhTweakDlg* pParent = (CbvhTweakDlg*)GetParent();	
	pParent->LockApp();

	m_bDragging = true;
	m_bDragChanged = false;
	SetCapture();
	SetFocus();

	if (SetThumb(point))
	{
		m_bDragChanged = true;
		PostMessageToParent(TB_THUMBTRACK);
		PostMessage(TB_THUMBTRACK);
	}
}


void CNiceSliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		if (SetThumb(point))
		{
			m_bDragChanged = true;
			PostMessageToParent(TB_THUMBTRACK);
			PostMessage(TB_THUMBTRACK);
		}
	}
	else
	{
		CSliderCtrl::OnMouseMove(nFlags, point);
	}
}

void CNiceSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		m_bDragging = false;
		::ReleaseCapture();
		if (SetThumb(point))
		{
			PostMessageToParent(TB_ENDTRACK);
			PostMessage(TB_THUMBTRACK);
			m_bDragChanged = true;
		}
		if (m_bDragChanged)
		{
			PostMessageToParent(TB_ENDTRACK);
			PostMessage(TB_THUMBTRACK);
			m_bDragChanged = false;
			CbvhTweakDlg* pParent = (CbvhTweakDlg*)GetParent();	
			pParent->UnlockApp();
		}
	}
	else
	{
		CSliderCtrl::OnLButtonUp(nFlags, point);
	}
}


bool CNiceSliderCtrl::SetThumb(const CPoint& point)
{
	const int nMin = GetRangeMin();
	const int nMax = GetRangeMax()+1;
	CRect rc;
	GetChannelRect(rc);
	double dPos;
	double dCorrectionFactor = 0.0;
	if (GetStyle() & TBS_VERT) 
	{
		// note: there is a bug in GetChannelRect, it gets the orientation of the rectangle mixed up
		dPos = (double)(point.y - rc.left)/(rc.right - rc.left);
	}
	else
	{
		dPos = (double)(point.x - rc.left)/(rc.right - rc.left);
	}
	// This correction factor is needed when you click inbetween tick marks
	// so that the thumb will move to the nearest one
	dCorrectionFactor = 0.5 *(1-dPos) - 0.5 *dPos;
	int nNewPos = (int)(nMin + (nMax-nMin)*dPos + dCorrectionFactor);
	const bool bChanged = (nNewPos != GetPos());
	if(bChanged)
	{
		SetPos(nNewPos);
	}
	return bChanged;
}

void CNiceSliderCtrl::PostMessageToParent(const int nTBCode) const
{
	CWnd* pWnd = GetParent();
	if(pWnd) pWnd->PostMessage(WM_HSCROLL, (WPARAM)((GetPos() << 16) | nTBCode), (LPARAM)GetSafeHwnd());
}

//void SetOwner(CbvhTweakDlg* pOwner)
//{
//	m_pOwner = (void*)pOwner;
//}

//LRESULT CNiceSliderCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if(message==TB_THUMBTRACK)
//	{
//		CWnd* pWnd = GetParent();
//		if(pWnd) pWnd->PostMessage(WM_HSCROLL, (WPARAM)((GetPos() << 16)), (LPARAM)GetSafeHwnd());
//	}
//	return CWnd::WindowProc(message, wParam, lParam); 
//}


