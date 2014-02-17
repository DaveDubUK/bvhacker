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
 
#ifndef NiceSlider_h
#define NiceSlider_h
//#include "bvhTweakDlg.h"


//class CbvhTweakDlg;

// Class that provides better mouse click handling for the standard windows slider control
class CNiceSliderCtrl : public CSliderCtrl
{
public:
	DECLARE_DYNAMIC(CNiceSliderCtrl)
		CNiceSliderCtrl() : m_bDragging(false), m_bDragChanged(false) {}

	//LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	//{{AFX_MSG(CRoundSliderCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	bool SetThumb(const CPoint& pt);
	void PostMessageToParent(const int nTBCode) const;
	bool m_bDragging;
	bool m_bDragChanged;
};

#endif // NiceSlider_h