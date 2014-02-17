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
 
// bvhTweakDlg.h : header file
//
#pragma once

// standard stuff
#include "afxcmn.h"
#include "afxwin.h"

// extra controls
#include "OpenGLControl.h"
#include "BVHTreeCtrl.h"
#include "NiceSlider.h"
#include "EasySize.h"
#include "BVHFileDialog.h"


// CbvhTweakDlg dialog
class CbvhTweakDlg : public CDialog
{
// Construction
public:
	CbvhTweakDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CbvhTweakDlg(void);

	// Dialog Data
	enum { IDD = IDD_BVHTWEAK_DIALOG };

	// getters / setters
	int  GetStatus();
	void SetStatus(int status);
	bool StatusIs(int statusToCompareTo);
	void SetEditingMode(bool bEdit);
	void LoadBVHFromLPTSTR(LPTSTR lpFileName);
	void LoadBVHFromString(CString sFileName);

	DECLARE_EASYSIZE

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadBVH();
	afx_msg void OnBnClickedSaveBVH();
	afx_msg void OnOK();

private:

	afx_msg void OnBnClickedPlayBtn();
	afx_msg void OnBnClickedStepFwdBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStopBtn();
	afx_msg void OnBnClickedStepRev();
	afx_msg void OnBnClickedResetView();
	afx_msg void OnBnClickedGndViewBtn();
	afx_msg void OnNMReleasedCaptureSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSliderRange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedClampX();
	afx_msg void OnBnClickedRotSel();
	afx_msg void OnBnClickedClampY();
	afx_msg void OnBnClickedClampZ();
	afx_msg void OnBnClickedTranslationSel();
	afx_msg void OnBnClickedTrimPtLeft();
	afx_msg void OnBnClickedTrimPtR();
	afx_msg void OnBnClickedTrimBtn();
	afx_msg void OnDeltaposFramerateSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedHelpBtn();

public: // so that option dialog has access
	afx_msg void OnBnClickedColourDisplay();
	afx_msg void OnBnClickedShowGround();
	//OnViewShading

private:
	afx_msg void OnBnClickedSetT();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedKnitEnds();
	afx_msg void OnBnClickedSlNamingClicked();
	afx_msg void OnInsertJoint();
	afx_msg void OnRemoveJoint();
	afx_msg void OnRenameJoint();

	void LoadBVH();
	void OnBnClickedDisp1stFrameCheck();
	void DisableControls();
	void EnableControls(void);
	void DisableSliderControls(void);
	void EnableSliderControls(void);

	// controls
	COpenGLControl m_oglWindow;
	CBVHTreeCtrl m_SkeletonTree;

	CNiceSliderCtrl m_TimeLine;

	CNiceSliderCtrl m_XSlider;
	CNiceSliderCtrl m_YSlider;
	CNiceSliderCtrl m_ZSlider;
	CEdit m_SliderRangeAdjust;

	// frame count display (in control section)
	CStatic m_FrameCountDisplay;	
	CEdit m_FrameTimeDisplay;

	// the file properties pane
	CStatic m_PropsPane;

	// the joint properties pane
	CStatic m_CurrentJointLabel;
	CStatic m_channelOrderDisplay;
	CStatic m_heirarchyDisplay;
	CStatic m_nChannelsDisplay;
	CStatic m_parentDisplay;

	// motion adjust buttons
	CButton m_ClampX;
	CButton m_ClampY;
	CButton m_ClampZ;
	CButton m_TranslationRadioBtn;
	CButton m_RotationRadioBtn;

	// view changing buttons
	CButton m_Disp1stFrameBtn;
	CButton m_ColourDisplayCheck;
	CButton m_ShowGroundCheck;
	CButton m_DefaultViewButton;
	CButton m_GroundViewButton;	

	// playback control buttons
	CButton m_PlayButton;
	CButton m_StopButton;
	CButton m_ReverseButton;
	CButton m_ForwardButton;
	//bool firstRun;

	// Edit buttons
	CButton m_KnitEndsButton;
	CButton m_SetTShapeBtn;
	CButton m_UseSLJointNamesBtn;

	// file buttons
	CButton m_saveButton;

	// edit buttons
	CButton m_TrimLeftBtn;
	CButton m_TrimRightBtn;
	CButton m_TrimButton;

	// misc spinners and labels
	CSpinButtonCtrl m_SliderRangeSpinner;
	CSpinButtonCtrl m_FrameTimeSpinner;
	CStatic m_OffsetStaticLabel;
	CStatic m_ClampBtnsLabel;
	CStatic m_SliderRangeLabel;
	CStatic m_FramerateLabelStatic;

	// vars
	int m_status;
	float m_fLastSliderPos;
	bool m_bNewScroll;
	int m_nTrimPointLeft;
	int m_nTrimPointRight;
	LPTSTR m_lpCmdLine;
	bool m_bGlobalEditing;
	

// Declare the variables needed for memory leak detection
#ifdef _DEBUG
    CMemoryState oldMemState, newMemState, diffMemState;
#endif

public:

	// added to interface easier with options dialog
	CBVHRegistrySettings* m_pRegistrySettings;

	// added since version 1.1.0.0
	void SetCmdLine(LPTSTR lpCmdLine);
	CButton m_RemoveTBtn;
	CButton m_Right_ViewButton;
	CButton m_LeftViewButton;
	CButton m_MotionBuilderFixBtn;
	CButton m_NoZerosButton;
	CButton m_ZeroButton;
	CStatic m_FpsDisplay;
	CStatic m_openGLWindow;
	CStatic m_buttonSpacer;	
	CStatic m_Transport_controls_group_box;
	// added since 1.5
	CButton m_FixLoopBtn;
	// added since 1.6
	CButton m_Resample_Button_Latest;
	CButton m_Centre_Btn;
	void UnlockApp(void);
	CEdit m_XValEditDisplay;
	CEdit m_YValEditDisplay;
	CEdit m_ZValEditDisplay;
	CEdit m_OffsetsDispX_E;
	CEdit m_OffsetsDispY_E;
	CEdit m_OffsetsDispZ_E;
	void UpdateInterface();
	bool m_bAvoidingDoubleEntry;
	void LockApp(void);
	void OnCancel(void);
	CStatic m_Scale_Display;
	CStatic m_AdjustmentInfoLabel;
	BOOL PreTranslateMessage(MSG* pMsg);
	// added since 1.8
	CButton m_ZoomInButton;
	CButton m_ZoomOutButton;

protected:
	CBrush m_greybrush;
	COLORREF m_greycolor;

public:
	afx_msg void OnBnClickedAddAbdomenBtn();
	afx_msg void OnBnClickedAbFixBtn();
	afx_msg void OnBnClickedRemoveT();
	afx_msg void OnBnClickedRightViewButton();
	afx_msg void OnBnClickedLeftViewButton();
	afx_msg void OnBnClickedMotionbuilderFix();
	afx_msg void OnBnClickedNoZeroBtn();
	afx_msg void OnBnClickedZeroBtn();
	afx_msg void OnBnClickedCheckQuality();
	afx_msg void OnStnClickedClampLabel();
	afx_msg void OnBnClickedScaleUp();
	afx_msg void OnBnClickedScaleDown();
	afx_msg void OnBnClickedAnimazooFix2();
	afx_msg void OnFileOpen32777();
	afx_msg void OnFileSave32778();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnFileExit32784();
	afx_msg void OnHelpOnlinehelpfiles();
	afx_msg void OnViewFront();
	afx_msg void OnViewRightview();
	afx_msg void OnViewLeftview();
	afx_msg void OnViewFloorlevelview();
	afx_msg void OnHelpAbout();
	afx_msg void OnFileReport();
	afx_msg void OnEditSwapchest();
	afx_msg void OnEditScaleup();
	afx_msg void OnEditScaledown();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnHelpDonate();
	afx_msg void OnViewDisplayfirstframe();
	afx_msg void OnHelpUsing();
	afx_msg void OnViewShowground();
	afx_msg void OnViewColourdisplay();
	afx_msg void OnBnClickedResampleBtn();
	afx_msg void OnHacksResample();
	afx_msg void OnLoopingMarkin();
	afx_msg void OnLoopingCrop();
	afx_msg void OnLoopingKnitends();
	afx_msg void OnLoopingMarkout();
	afx_msg void OnHacksSett();
	afx_msg void OnHacksRemovetstance();
	afx_msg void OnHacksSlify();
	afx_msg void OnHacksRenamejoints();
	afx_msg void OnHacksRemovehipsoffset();
	afx_msg void OnHacksRemovezerosfromfirstframe();
	afx_msg void OnHacksAddabdomen();
	afx_msg void OnEnChangeSliderRange();
	afx_msg void OnHelpForums();
	afx_msg void OnHelpFinder();
	afx_msg void OnHackLowpassfilter();
	afx_msg void OnHackSLLoopFix();
	afx_msg void OnBnClickedFixLoop();
	afx_msg void OnHackSetblendpose();
	afx_msg void OnBnClickedCentreButton();
	afx_msg void OnEnKillfocusFramerateEdit();
	afx_msg void OnBnClickedHidden();
	afx_msg void OnHackCentre();
	afx_msg void OnEnKillfocusSliderRange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTvnSelchangedBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusXEdit();
	afx_msg void OnEnKillfocusYEdit();
	afx_msg void OnEnKillfocusZEdit();
	afx_msg void OnEnKillfocusOffsetsDispxE();
	afx_msg void OnEnKillfocusOffsetsDispyE();
	afx_msg void OnEnKillfocusOffsetsDispzE();
	afx_msg void OnHackAttemptslbonesizing();
	afx_msg void OnEditUndo();
	afx_msg void OnEditTranslationmode();
	afx_msg void OnEditRotationmode();
	afx_msg void OnHackInterpolatechest();
	afx_msg void OnHackSquareshoulders();
	afx_msg void OnNMCustomdrawBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHelpBvhackergooglegroup();
	afx_msg void OnHackAttemptretargettoanimeeple();
	afx_msg void OnHackAttemptanimeeplenaming();
	afx_msg void OnSpineCombinechest();
	afx_msg void OnDestroy();
public:
	afx_msg void OnEnSetfocusXEdit();
public:
	afx_msg void OnEnSetfocusYEdit();
public:
	afx_msg void OnEnSetfocusZEdit();
public:
	afx_msg void OnBnClickedNotZero3();
public:
	afx_msg void OnPrepareNoZeroes();
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnViewShading();
	afx_msg void OnHackRoundshoulders();
	afx_msg void OnHackCraneneckshift();
	afx_msg void OnHackStraightenneckshift();
	afx_msg void OnHackReversetimeline();
	afx_msg void OnViewOptions();
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
private:
	float m_ZoomIncrement;
public:
	afx_msg void OnBnClickedZoomIn();
	afx_msg void OnBnClickedZoomOut();
	void OnClickedShowHead();
	afx_msg void OnDeleteDescendants();
	afx_msg void OnHipsTranslationZero();
	afx_msg void OnHipsRotationZero();
	afx_msg void OnHipsBothZero();
	afx_msg void OnUpperbodyAllabovehips();
	void OnZeroOutDescendants(void);
	afx_msg void OnZerooutJoint();
	afx_msg void OnUpperbodyAbdomenandup();
	afx_msg void OnUpperbodyChestandup();
	afx_msg void OnLowerbodyHips();
	afx_msg void OnLowerbodyHips32849();
	afx_msg void OnAllexceptRightarm();
	afx_msg void OnAllexceptLeftarm();
	afx_msg void OnAllexceptRightarmandhead();
	afx_msg void OnAllexceptLeftarmandhead();
	afx_msg void OnAllexceptRightarm32859();
	afx_msg void OnAllexceptLeftarm32860();
	void OnMergeJointWithParent(void);
	afx_msg void OnNMKillfocusBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult);
};
