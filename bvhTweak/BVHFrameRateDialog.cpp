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
 
// BVHFrameRateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "bvhTweak.h"
#include "BVHFrameRateDialog.h"


// CBVHFrameRateDialog dialog

IMPLEMENT_DYNAMIC(CBVHFrameRateDialog, CDialog)

CBVHFrameRateDialog::CBVHFrameRateDialog(float* fFrameTime, int* nFrames, CWnd* pParent /*=NULL*/)
	: CDialog(CBVHFrameRateDialog::IDD, pParent)
{
	m_pfFrameTime = fFrameTime;
	m_pnFrames = nFrames;
	m_fTotalLengthSeconds = (*m_pfFrameTime)*(float)((*m_pnFrames));

	// keep rollback values
	//m_fFrameTimeRB = fFrameTime;
	//m_nFramesRB = nFrames;
}

CBVHFrameRateDialog::~CBVHFrameRateDialog()
{
}

void CBVHFrameRateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_FRAMETIME, m_pNewFrameTimeEdit);
	DDX_Control(pDX, IDC_NEW_FRAME_RATE, m_pewFrameRateEdit);
	DDX_Control(pDX, IDC_NEW_ANIMATION_LENGTH, m_pNewAnimLengthEdit);
	DDX_Control(pDX, IDC_NEW_FRAMERATE_SPINNER, m_pNewFrameTimeSpinner);
	DDX_Control(pDX, IDC_NEW_FRAME_RATE_SPINNER, m_pNewFramerateSpinner);
	DDX_Control(pDX, IDC_NEW_ANIM_LENGTH_SPINNER, m_pNewAnimLengthSpinner);
}


BEGIN_MESSAGE_MAP(CBVHFrameRateDialog, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NEW_FRAMERATE_SPINNER, &CBVHFrameRateDialog::OnDeltaposNewFramerateSpinner)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NEW_FRAME_RATE_SPINNER, &CBVHFrameRateDialog::OnDeltaposNewFrameRateSpinner)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NEW_ANIM_LENGTH_SPINNER, &CBVHFrameRateDialog::OnDeltaposNewAnimLengthSpinner)
	ON_EN_CHANGE(IDC_NEW_FRAMETIME, &CBVHFrameRateDialog::OnEnChangeNewFrametime)
	ON_BN_CLICKED(IDCANCEL, &CBVHFrameRateDialog::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_NEW_FRAME_RATE, &CBVHFrameRateDialog::OnEnChangeNewFrameRate)
	ON_BN_CLICKED(IDC_HIDDEN_2, &CBVHFrameRateDialog::OnBnClickedHidden2)
END_MESSAGE_MAP()


// CBVHFrameRateDialog message handlers

void CBVHFrameRateDialog::Update(void)
{
	TCHAR wsFrameTimeDisplay[10];
	sprintf(wsFrameTimeDisplay,_T("%0.*f"),3,*m_pfFrameTime);
	m_pNewFrameTimeEdit.SetWindowTextA(wsFrameTimeDisplay);

	float fFrameRate = 1000.f/(*m_pfFrameTime);
	TCHAR wsFrameRateDisplay[10];
	sprintf(wsFrameRateDisplay,_T("%0.*f"),1,fFrameRate);
	m_pewFrameRateEdit.SetWindowTextA(wsFrameRateDisplay);

	TCHAR wsFramesDisplay[10];
	sprintf(wsFramesDisplay,_T("%0.*d"),0,*m_pnFrames);
	m_pNewAnimLengthEdit.SetWindowTextA(wsFramesDisplay);
}

BOOL CBVHFrameRateDialog::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	Update();
	return TRUE;
}

// bad naming - should be 'frametime'
void CBVHFrameRateDialog::OnDeltaposNewFramerateSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	double incr = -0.025;
	if(pNMUpDown->iDelta<1) incr = 0.025;

	*m_pfFrameTime+=(float)incr;
	if(*m_pfFrameTime>5000) *m_pfFrameTime = 5000;
	if(*m_pfFrameTime<5) *m_pfFrameTime = 5;
	m_pNewFrameTimeSpinner.SetPos((int)*m_pfFrameTime);
	*m_pnFrames = (int)m_fTotalLengthSeconds/(int)(*m_pfFrameTime);
	Update();
	
	*pResult = 0;
}

void CBVHFrameRateDialog::OnDeltaposNewFrameRateSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	float incr = -0.5;
	if(pNMUpDown->iDelta<1) incr = 0.5;

	float fFrameRate = 1000/(*m_pfFrameTime);

	fFrameRate+=incr;
	if(fFrameRate>120) fFrameRate = 120;
	if(fFrameRate<2) fFrameRate = 2;
	m_pNewFramerateSpinner.SetPos((int)fFrameRate);

	*m_pfFrameTime = 1000/fFrameRate;
	float fFrames = m_fTotalLengthSeconds/(*m_pfFrameTime);
	*m_pnFrames = (int)fFrames;
	Update();

	*pResult = 0;
}

void CBVHFrameRateDialog::OnDeltaposNewAnimLengthSpinner(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int incr = -1;
	if(pNMUpDown->iDelta<1) incr = 1;

	*m_pnFrames+=incr;
	if(*m_pnFrames>100000) *m_pnFrames = 100000;
	if(*m_pnFrames<1) *m_pnFrames = 1;
	m_pNewAnimLengthSpinner.SetPos((int)*m_pnFrames);
	*m_pfFrameTime = m_fTotalLengthSeconds/(float)(*m_pnFrames);
	Update();

	*pResult = 0;
}
 
void CBVHFrameRateDialog::OnEnChangeNewFrametime()
{
	CString wsFrameTime;
	double dFrameTime;
	m_pNewFrameTimeEdit.GetWindowTextA(wsFrameTime);
	_stscanf(wsFrameTime, _T("%lf"), &dFrameTime);
	
	*m_pfFrameTime=(float)dFrameTime;
	if(*m_pfFrameTime>5000) *m_pfFrameTime = 5000;
	if(*m_pfFrameTime<5) *m_pfFrameTime = 5;
	m_pNewFrameTimeSpinner.SetPos((int)*m_pfFrameTime);
	*m_pnFrames = (int)m_fTotalLengthSeconds/(int)(*m_pfFrameTime);
	Update();
}

void CBVHFrameRateDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();

	*m_pfFrameTime = 0;
}

void CBVHFrameRateDialog::OnEnChangeNewFrameRate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CBVHFrameRateDialog::OnBnClickedHidden2()
{
	// Use a hidden Default button to catch any Returns

	// Make the IDOK button NOT a default button

	if (GetDlgItem(IDOK) == GetFocus())
		{
		// send vars back to main dialog for processing...
		
		CDialog::OnOK();
		return;
		}

	if (GetDlgItem(IDCANCEL) == GetFocus())
		{
		CDialog::OnCancel();
		return;
		}
}
