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
#include "afxcmn.h"


// CBVHFrameRateDialog dialog

class CBVHFrameRateDialog : public CDialog
{
	DECLARE_DYNAMIC(CBVHFrameRateDialog)

public:
	CBVHFrameRateDialog(float* fFrameTime, int* nFrames, CWnd* pParent = NULL);   // standard constructor
	virtual ~CBVHFrameRateDialog();

// Dialog Data
	enum { IDD = IDD_RESAMPLE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_pNewFrameTimeEdit;
	CEdit m_pewFrameRateEdit;
	CEdit m_pNewAnimLengthEdit;
	CSpinButtonCtrl m_pNewFrameTimeSpinner;
	CSpinButtonCtrl m_pNewFramerateSpinner;
	CSpinButtonCtrl m_pNewAnimLengthSpinner;

	// vars
	float* m_pfFrameTime;
	int* m_pnFrames;
	float m_fTotalLengthSeconds;
	

	// keep orginal values just in case user cancels
	//float m_fFrameTimeRB;
	//int m_nFramesRB;
	
	BOOL OnInitDialog(void);
	void Update(void);

	afx_msg void OnDeltaposNewFramerateSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposNewFrameRateSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposNewAnimLengthSpinner(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeNewFrametime();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeNewFrameRate();
	afx_msg void OnBnClickedHidden2();
};
