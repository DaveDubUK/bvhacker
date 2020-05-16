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
 
// bvhTweakDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bvhTweak.h"
#include "bvhTweakDlg.h"
#include "BVHAboutBox.h"
#include "BVHOptions.h"
#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()





// CbvhTweakDlg dialog




CbvhTweakDlg::CbvhTweakDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbvhTweakDlg::IDD, pParent)
	, m_fLastSliderPos(0)
	, m_bNewScroll(true)
	, m_nTrimPointLeft(0)
	, m_nTrimPointRight(0)
	, m_bAvoidingDoubleEntry(true)
	, m_ZoomIncrement(100.0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINFRAME);
	m_bGlobalEditing = true;
	m_pRegistrySettings = m_pRegistrySettings;
}

CbvhTweakDlg::~CbvhTweakDlg()
{
}


void CbvhTweakDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// kill our timers
	if(StatusIs(FILE_PLAYING))KillTimer(3);// GUI update timer
	KillTimer(2);// GUI timer
	m_oglWindow.KillTimer(1);// OpenGL draw timer
}

void CbvhTweakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BVH_SKELETON, m_SkeletonTree);
	DDX_Control(pDX, IDC_X_SLIDER, m_XSlider);
	DDX_Control(pDX, IDC_Y_SLIDER, m_YSlider);
	DDX_Control(pDX, IDC_Z_SLIDER, m_ZSlider);
	DDX_Control(pDX, IDC_TIMELINE, m_TimeLine);
	DDX_Control(pDX, IDC_FRAME_COUNT, m_FrameCountDisplay);
	DDX_Control(pDX, IDC_JOINT_LABEL, m_CurrentJointLabel);
	DDX_Control(pDX, IDC_SLIDER_RANGE, m_SliderRangeAdjust);
	DDX_Control(pDX, IDC_SPIN1, m_SliderRangeSpinner);
	DDX_Control(pDX, IDC_CLAMP_X, m_ClampX);
	DDX_Control(pDX, IDC_CLAMP_Y, m_ClampY);
	DDX_Control(pDX, IDC_CLAMP_Z, m_ClampZ);
	DDX_Control(pDX, IDC_OFF_SEL, m_TranslationRadioBtn);
	DDX_Control(pDX, IDC_ROT_SEL, m_RotationRadioBtn);
	DDX_Control(pDX, IDC_FRAMERATE_EDIT, m_FrameTimeDisplay);
	DDX_Control(pDX, IDC_FRAMERATE_SPIN, m_FrameTimeSpinner);
	DDX_Control(pDX, IDC_PROPS_PANE, m_PropsPane);
	DDX_Control(pDX, IDC_TRIM_PT_L, m_TrimLeftBtn);
	DDX_Control(pDX, IDC_TRIM_PT_R, m_TrimRightBtn);
	DDX_Control(pDX, IDC_CHANNEL_ORDER, m_channelOrderDisplay);
	DDX_Control(pDX, IDC_HEIRARCHY, m_heirarchyDisplay);
	DDX_Control(pDX, IDC_NCHANNELS, m_nChannelsDisplay);
	DDX_Control(pDX, IDC_PARENT, m_parentDisplay);
	//DDX_Control(pDX, IDC_COLOUR_DISPLAY, m_ColourDisplayCheck);
	//DDX_Control(pDX, ID_VIEW_SHADING, m_ShadingDisplayCheck);
	//DDX_Control(pDX, IDC_SHOW_GROUND, m_ShowGroundCheck);
	DDX_Control(pDX, IDC_RESET_VIEW, m_DefaultViewButton);
	DDX_Control(pDX, IDC_GND_VIEW_BTN, m_GroundViewButton);
	DDX_Control(pDX, IDC_TRIM_BTN, m_TrimButton);
	DDX_Control(pDX, IDC_BUTTON3, m_PlayButton);
	DDX_Control(pDX, IDC_BUTTON4, m_StopButton);
	DDX_Control(pDX, IDC_STEP_REV, m_ReverseButton);
	DDX_Control(pDX, IDC_STEP_FWD_BTN, m_ForwardButton);
	DDX_Control(pDX, IDC_OFFSETS_LABEL, m_OffsetStaticLabel);
	DDX_Control(pDX, IDC_CLAMP_LABEL, m_ClampBtnsLabel);
	DDX_Control(pDX, IDC_RANGE_LABEL, m_SliderRangeLabel);
	//DDX_Control(pDX, IDC_X, m_XLabel);
	//DDX_Control(pDX, IDC_Y, m_YLabel);
	//DDX_Control(pDX, IDC_Z, m_ZLabel);
	DDX_Control(pDX, IDC_TRIM_BTN2, m_KnitEndsButton);
	DDX_Control(pDX, IDC_TRIM_BTN3, m_SetTShapeBtn);
	DDX_Control(pDX, IDC_SL_NAMING_CLICKED, m_UseSLJointNamesBtn);
	DDX_Control(pDX, IDC_FRAMERATE, m_FramerateLabelStatic);
	DDX_Control(pDX, IDC_REMOVE_T, m_RemoveTBtn);
	DDX_Control(pDX, IDC_RIGHT_VIEW_BUTTON, m_Right_ViewButton);
	DDX_Control(pDX, IDC_LEFT_VIEW_BUTTON, m_LeftViewButton);
	DDX_Control(pDX, IDC_MOTIONBUILDER_FIX, m_MotionBuilderFixBtn);
	DDX_Control(pDX, IDC_ZERO_BTN, m_ZeroButton);
	DDX_Control(pDX, IDC_FRAMERATE2, m_FpsDisplay);
	DDX_Control(pDX, IDC_TRANS_CTRLS_GROUP_BOX, m_Transport_controls_group_box);
	DDX_Control(pDX, IDC_OPENGL, m_openGLWindow);
	DDX_Control(pDX, IDC_SPACER_BTN, m_buttonSpacer);
	DDX_Control(pDX, IDC_FIX_LOOP, m_FixLoopBtn);
	DDX_Control(pDX, IDC_CENTRE_BUTTON, m_Centre_Btn);
	DDX_Control(pDX, IDC_X_EDIT, m_XValEditDisplay);
	DDX_Control(pDX, IDC_Y_EDIT, m_YValEditDisplay);
	DDX_Control(pDX, IDC_Z_EDIT, m_ZValEditDisplay);
	DDX_Control(pDX, IDC_OFFSETS_DISPX_E, m_OffsetsDispX_E);
	DDX_Control(pDX, IDC_OFFSETS_DISPY_E, m_OffsetsDispY_E);
	DDX_Control(pDX, IDC_OFFSETS_DISPZ_E, m_OffsetsDispZ_E);
	DDX_Control(pDX, IDC_SCALE_DISPLAY, m_Scale_Display);
	DDX_Control(pDX, IDC_ADJUSTMENT_INFO, m_AdjustmentInfoLabel);
	DDX_Control(pDX, IDC_ZOOM_IN, m_ZoomInButton);
	DDX_Control(pDX, IDC_ZOOM_OUT, m_ZoomOutButton);
}
//
// message maps
//
BEGIN_MESSAGE_MAP(CbvhTweakDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CbvhTweakDlg::OnBnClickedPlayBtn)
	ON_BN_CLICKED(IDC_STEP_FWD_BTN, &CbvhTweakDlg::OnBnClickedStepFwdBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CbvhTweakDlg::OnBnClickedStopBtn)
	ON_BN_CLICKED(IDC_STEP_REV, &CbvhTweakDlg::OnBnClickedStepRev)
	ON_BN_CLICKED(IDC_RESET_VIEW, &CbvhTweakDlg::OnBnClickedResetView)
	ON_BN_CLICKED(IDC_GND_VIEW_BTN, &CbvhTweakDlg::OnBnClickedGndViewBtn)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Y_SLIDER, &CbvhTweakDlg::OnNMReleasedCaptureSlider)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CbvhTweakDlg::OnDeltaposSliderRange)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CLAMP_X, &CbvhTweakDlg::OnBnClickedClampX)
	ON_BN_CLICKED(IDC_ROT_SEL, &CbvhTweakDlg::OnBnClickedRotSel)
	ON_BN_CLICKED(IDC_OFF_SEL, &CbvhTweakDlg::OnBnClickedTranslationSel)
	ON_BN_CLICKED(IDC_CLAMP_Y, &CbvhTweakDlg::OnBnClickedClampY)
	ON_BN_CLICKED(IDC_CLAMP_Z, &CbvhTweakDlg::OnBnClickedClampZ)
	ON_BN_CLICKED(IDC_TRIM_PT_L, &CbvhTweakDlg::OnBnClickedTrimPtLeft)
	ON_BN_CLICKED(IDC_TRIM_PT_R, &CbvhTweakDlg::OnBnClickedTrimPtR)
	ON_BN_CLICKED(IDC_TRIM_BTN, &CbvhTweakDlg::OnBnClickedTrimBtn)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRAMERATE_SPIN, &CbvhTweakDlg::OnDeltaposFramerateSpin)
	ON_BN_CLICKED(IDC_COLOUR_DISPLAY, &CbvhTweakDlg::OnBnClickedColourDisplay)
	ON_BN_CLICKED(IDC_SHOW_GROUND, &CbvhTweakDlg::OnBnClickedShowGround)
	ON_BN_CLICKED(IDC_TRIM_BTN2, &CbvhTweakDlg::OnBnClickedKnitEnds)
	ON_BN_CLICKED(IDC_TRIM_BTN3, &CbvhTweakDlg::OnBnClickedSetT)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_SL_NAMING_CLICKED, &CbvhTweakDlg::OnBnClickedResampleBtn)
	ON_COMMAND(ID_INSERT_JOINT, &CbvhTweakDlg::OnInsertJoint)
	ON_COMMAND(ID_REMOVE_JOINT, &CbvhTweakDlg::OnRemoveJoint)
	ON_COMMAND(ID_RENAME_JOINT, &CbvhTweakDlg::OnRenameJoint)
	//ON_BN_CLICKED(IDC_REMOVE_T, &CbvhTweakDlg::OnBnClickedRemoveT)
	ON_BN_CLICKED(IDC_RIGHT_VIEW_BUTTON, &CbvhTweakDlg::OnBnClickedRightViewButton)
	ON_BN_CLICKED(IDC_LEFT_VIEW_BUTTON, &CbvhTweakDlg::OnBnClickedLeftViewButton)
	ON_BN_CLICKED(IDC_MOTIONBUILDER_FIX, &CbvhTweakDlg::OnBnClickedMotionbuilderFix)
	ON_BN_CLICKED(IDC_ZERO_BTN, &CbvhTweakDlg::OnBnClickedZeroBtn)
	ON_STN_CLICKED(IDC_CLAMP_LABEL, &CbvhTweakDlg::OnStnClickedClampLabel)
	ON_COMMAND(ID_FILE_OPEN32777, &CbvhTweakDlg::OnFileOpen32777)
	ON_COMMAND(ID_FILE_SAVE32778, &CbvhTweakDlg::OnFileSave32778)
	ON_BN_CLICKED(IDCANCEL, &CbvhTweakDlg::OnBnClickedCancel)
	ON_COMMAND(ID_FILE_EXIT32784, &CbvhTweakDlg::OnFileExit32784)
	ON_COMMAND(ID_HELP_ONLINEHELPFILES, &CbvhTweakDlg::OnHelpOnlinehelpfiles)
	ON_COMMAND(ID_VIEW_FRONT, &CbvhTweakDlg::OnViewFront)
	ON_COMMAND(ID_VIEW_RIGHTVIEW, &CbvhTweakDlg::OnViewRightview)
	ON_COMMAND(ID_VIEW_LEFTVIEW, &CbvhTweakDlg::OnViewLeftview)
	ON_COMMAND(ID_VIEW_FLOORLEVELVIEW, &CbvhTweakDlg::OnViewFloorlevelview)
	ON_COMMAND(ID_HELP_ABOUT, &CbvhTweakDlg::OnHelpAbout)
	ON_COMMAND(ID_FILE_REPORT, &CbvhTweakDlg::OnFileReport)
	ON_COMMAND(ID_EDIT_SWAPCHEST, &CbvhTweakDlg::OnEditSwapchest)
	ON_COMMAND(ID_HACK_INTERPOLATECHEST, &CbvhTweakDlg::OnHackInterpolatechest)
	ON_COMMAND(ID_EDIT_SCALEUP, &CbvhTweakDlg::OnEditScaleup)
	ON_COMMAND(ID_EDIT_SCALEDOWN, &CbvhTweakDlg::OnEditScaledown)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_COMMAND(ID_HELP_DONATE, &CbvhTweakDlg::OnHelpDonate)
	ON_COMMAND(ID_VIEW_DISPLAYFIRSTFRAME, &CbvhTweakDlg::OnViewDisplayfirstframe)
	ON_COMMAND(ID_HELP_USING, &CbvhTweakDlg::OnHelpUsing)
	ON_COMMAND(ID_VIEW_SHOWGROUND, &CbvhTweakDlg::OnViewShowground)
	ON_COMMAND(ID_VIEW_COLOURDISPLAY, &CbvhTweakDlg::OnViewColourdisplay)
	ON_COMMAND(ID_HACKS_RESAMPLE, &CbvhTweakDlg::OnHacksResample)
	ON_COMMAND(ID_LOOPING_MARKIN, &CbvhTweakDlg::OnLoopingMarkin)
	ON_COMMAND(ID_LOOPING_CROP, &CbvhTweakDlg::OnLoopingCrop)
	ON_COMMAND(ID_LOOPING_KNITENDS, &CbvhTweakDlg::OnLoopingKnitends)
	ON_COMMAND(ID_LOOPING_MARKOUT, &CbvhTweakDlg::OnLoopingMarkout)
	ON_COMMAND(ID_HACKS_SETT, &CbvhTweakDlg::OnHacksSett)
	ON_COMMAND(ID_HACKS_REMOVETSTANCE, &CbvhTweakDlg::OnHacksRemovetstance)
	ON_COMMAND(ID_HACKS_SLIFY, &CbvhTweakDlg::OnHacksSlify)
	ON_COMMAND(ID_HACKS_RENAMEJOINTS, &CbvhTweakDlg::OnHacksRenamejoints)
	ON_COMMAND(ID_HACKS_REMOVEHIPSOFFSET, &CbvhTweakDlg::OnHacksRemovehipsoffset)
	ON_COMMAND(ID_HACKS_REMOVEZEROSFROMFIRSTFRAME, &CbvhTweakDlg::OnHacksRemovezerosfromfirstframe)
	ON_COMMAND(ID_HACKS_ADDABDOMEN, &CbvhTweakDlg::OnHacksAddabdomen)
	ON_EN_CHANGE(IDC_SLIDER_RANGE, &CbvhTweakDlg::OnEnChangeSliderRange)
	ON_COMMAND(ID_HELP_FORUMS, &CbvhTweakDlg::OnHelpForums)
	ON_COMMAND(ID_HELP_FINDER, &CbvhTweakDlg::OnHelpFinder)
	ON_COMMAND(ID_HACK_LOWPASSFILTER, &CbvhTweakDlg::OnHackLowpassfilter)
	ON_COMMAND(ID_HACK_SLVWR, &CbvhTweakDlg::OnHackSLLoopFix)
	ON_BN_CLICKED(IDC_FIX_LOOP, &CbvhTweakDlg::OnBnClickedFixLoop)
	ON_COMMAND(ID_HACK_SETBLENDPOSE, &CbvhTweakDlg::OnHackSetblendpose)
	ON_BN_CLICKED(IDC_CENTRE_BUTTON, &CbvhTweakDlg::OnBnClickedCentreButton)
	ON_EN_KILLFOCUS(IDC_FRAMERATE_EDIT, &CbvhTweakDlg::OnEnKillfocusFramerateEdit)
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDC_HIDDEN, &CbvhTweakDlg::OnBnClickedHidden)
	ON_COMMAND(ID_HACK_CENTRE, &CbvhTweakDlg::OnHackCentre)
	ON_EN_KILLFOCUS(IDC_SLIDER_RANGE, &CbvhTweakDlg::OnEnKillfocusSliderRange)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_SELCHANGED, IDC_BVH_SKELETON, &CbvhTweakDlg::OnTvnSelchangedBvhSkeleton)
	ON_EN_KILLFOCUS(IDC_X_EDIT, &CbvhTweakDlg::OnEnKillfocusXEdit)
	ON_EN_KILLFOCUS(IDC_Y_EDIT, &CbvhTweakDlg::OnEnKillfocusYEdit)
	ON_EN_KILLFOCUS(IDC_Z_EDIT, &CbvhTweakDlg::OnEnKillfocusZEdit)
	ON_EN_KILLFOCUS(IDC_OFFSETS_DISPX_E, &CbvhTweakDlg::OnEnKillfocusOffsetsDispxE)
	ON_EN_KILLFOCUS(IDC_OFFSETS_DISPY_E, &CbvhTweakDlg::OnEnKillfocusOffsetsDispyE)
	ON_EN_KILLFOCUS(IDC_OFFSETS_DISPZ_E, &CbvhTweakDlg::OnEnKillfocusOffsetsDispzE)
	ON_COMMAND(ID_HACK_ATTEMPTSLBONESIZING, &CbvhTweakDlg::OnHackAttemptslbonesizing)
	ON_COMMAND(ID_EDIT_UNDO32823, &CbvhTweakDlg::OnEditUndo)
	ON_COMMAND(ID_EDIT_TRANSLATIONMODE, &CbvhTweakDlg::OnEditTranslationmode)
	ON_COMMAND(ID_EDIT_ROTATIONMODE, &CbvhTweakDlg::OnEditRotationmode)
	ON_COMMAND(ID_HACK_SQUARESHOULDERS, &CbvhTweakDlg::OnHackSquareshoulders)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BVH_SKELETON, &CbvhTweakDlg::OnNMCustomdrawBvhSkeleton)
	ON_COMMAND(ID_HELP_BVHACKERGOOGLEGROUP, &CbvhTweakDlg::OnHelpBvhackergooglegroup)
	ON_COMMAND(ID_HACK_ATTEMPTRETARGETTOANIMEEPLE, &CbvhTweakDlg::OnHackAttemptretargettoanimeeple)
	ON_COMMAND(ID_HACK_ATTEMPTANIMEEPLENAMING, &CbvhTweakDlg::OnHackAttemptanimeeplenaming)
	ON_COMMAND(ID_SPINE_COMBINECHEST, &CbvhTweakDlg::OnSpineCombinechest)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_EN_SETFOCUS(IDC_X_EDIT, &CbvhTweakDlg::OnEnSetfocusXEdit)
	ON_EN_SETFOCUS(IDC_Y_EDIT, &CbvhTweakDlg::OnEnSetfocusYEdit)
	ON_EN_SETFOCUS(IDC_Z_EDIT, &CbvhTweakDlg::OnEnSetfocusZEdit)
	ON_BN_CLICKED(IDC_NOT_ZERO_3, &CbvhTweakDlg::OnBnClickedNotZero3)
	ON_COMMAND(ID_PREPARE_, &CbvhTweakDlg::OnPrepareNoZeroes)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_VIEW_SHADING, &CbvhTweakDlg::OnViewShading)
	ON_COMMAND(ID_HACK_ROUNDSHOULDERS, &CbvhTweakDlg::OnHackRoundshoulders)
	ON_COMMAND(ID_HACK_CRANENECKSHIFT, &CbvhTweakDlg::OnHackCraneneckshift)
	ON_COMMAND(ID_HACK_STRAIGHTENNECKSHIFT, &CbvhTweakDlg::OnHackStraightenneckshift)
	ON_COMMAND(ID_HACK_REVERSETIMELINE, &CbvhTweakDlg::OnHackReversetimeline)
	ON_COMMAND(ID_VIEW_OPTIONS, &CbvhTweakDlg::OnViewOptions)
	ON_COMMAND(ID_VIEW_ZOOMIN, &CbvhTweakDlg::OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CbvhTweakDlg::OnViewZoomout)
	ON_BN_CLICKED(IDC_ZOOM_IN, &CbvhTweakDlg::OnBnClickedZoomIn)
	ON_BN_CLICKED(IDC_ZOOM_OUT, &CbvhTweakDlg::OnBnClickedZoomOut)
	ON_COMMAND(ID_DELETE_DESCENDANTS, &CbvhTweakDlg::OnDeleteDescendants)
	ON_COMMAND(ID_ZEROOUTALLDECENDANTS, &CbvhTweakDlg::OnZeroOutDescendants)
	ON_COMMAND(ID_MERGEWITHPARENT, &CbvhTweakDlg::OnMergeJointWithParent)
	ON_COMMAND(ID_HIPS_TRANSLATION, &CbvhTweakDlg::OnHipsTranslationZero)
	ON_COMMAND(ID_HIPS_ROTATION, &CbvhTweakDlg::OnHipsRotationZero)
	ON_COMMAND(ID_HIPS_BOTH, &CbvhTweakDlg::OnHipsBothZero)
	ON_COMMAND(ID_UPPERBODY_ALLABOVEHIPS, &CbvhTweakDlg::OnUpperbodyAllabovehips)
	ON_COMMAND(ID_ZEROOUT_JOINT, &CbvhTweakDlg::OnZerooutJoint)
	ON_COMMAND(ID_UPPERBODY_ABDOMENANDUP, &CbvhTweakDlg::OnUpperbodyAbdomenandup)
	ON_COMMAND(ID_UPPERBODY_CHESTANDUP, &CbvhTweakDlg::OnUpperbodyChestandup)
	ON_COMMAND(ID_LOWERBODY_HIPS, &CbvhTweakDlg::OnLowerbodyHips)
	ON_COMMAND(ID_LOWERBODY_HIPS32849, &CbvhTweakDlg::OnLowerbodyHips32849)
	ON_COMMAND(ID_ALLEXCEPT_RIGHTARM, &CbvhTweakDlg::OnAllexceptRightarm)
	ON_COMMAND(ID_ALLEXCEPT_LEFTARM, &CbvhTweakDlg::OnAllexceptLeftarm)
	ON_COMMAND(ID_ALLEXCEPT_RIGHTARMANDHEAD, &CbvhTweakDlg::OnAllexceptRightarmandhead)
	ON_COMMAND(ID_ALLEXCEPT_LEFTARMANDHEAD, &CbvhTweakDlg::OnAllexceptLeftarmandhead)
	ON_COMMAND(ID_ALLEXCEPT_RIGHTARM32859, &CbvhTweakDlg::OnAllexceptRightarm32859)
	ON_COMMAND(ID_ALLEXCEPT_LEFTARM32860, &CbvhTweakDlg::OnAllexceptLeftarm32860)
	ON_NOTIFY(NM_KILLFOCUS, IDC_BVH_SKELETON, &CbvhTweakDlg::OnNMKillfocusBvhSkeleton)
	END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CbvhTweakDlg)

	//EASYSIZE(control,left,top,right,bottom,options)
	//EASYSIZE(IDC_OPENGL,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_BORDER,ES_HCENTER)// OpenGL window
	EASYSIZE(IDC_OPENGL,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)// OpenGL window
	EASYSIZE(IDC_TIMELINE,ES_BORDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// Timeline scrubber
	EASYSIZE(IDC_BVH_SKELETON,ES_BORDER,ES_BORDER,ES_KEEPSIZE,IDC_STATIC,0) // Skeleton tree view
	
	EASYSIZE(IDC_ADJ_GROUP_BOX,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_BORDER,0)// Adjustments group box

	EASYSIZE(IDC_CLAMP_Z,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// 
	EASYSIZE(IDC_CLAMP_Y,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// clamp buttons
	EASYSIZE(IDC_CLAMP_X,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// 
	EASYSIZE(IDC_ZERO_BTN,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// 
	EASYSIZE(IDC_CLAMP_LABEL,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// clamp labal
	
	EASYSIZE(IDC_X_SLIDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_BORDER,0)// IDC_X_SLIDER
	EASYSIZE(IDC_Y_SLIDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_BORDER,0)// IDC_Y_SLIDER
	EASYSIZE(IDC_Z_SLIDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_BORDER,0)// IDC_Z_SLIDER

	EASYSIZE(IDC_X_EDIT,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_X_VALUE slider values (now editable)
	EASYSIZE(IDC_Y_EDIT,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_Y_VALUE
	EASYSIZE(IDC_Z_EDIT,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_Z_VALUE
	//EASYSIZE(IDC_X,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// IDC_X slider label
	//EASYSIZE(IDC_Y,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// IDC_Y
	//EASYSIZE(IDC_Z,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// IDC_Z

	//EASYSIZE(IDC_SLIDERS_ADJUSTMENT,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_SLIDERS_ADJUSTMENT
	EASYSIZE(IDC_ROT_SEL,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_ROT_SEL
	EASYSIZE(IDC_OFF_SEL,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// translation range offset
	EASYSIZE(IDC_RANGE_LABEL,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// IDC_RANGE_LABEL
	EASYSIZE(IDC_SLIDER_RANGE,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// IDC_SLIDER_RANGE
	EASYSIZE(IDC_SPIN1,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// IDC_SPIN1
	EASYSIZE(IDC_ADJUSTMENT_INFO,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// Adjustment information
	
	EASYSIZE(IDC_HACKS_AND_TWEAKS,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_HACKS_AND_TWEAKS
	EASYSIZE(IDC_TRIM_BTN3,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_TRIM_BTN3 (is actually Set T btn?)
	EASYSIZE(IDC_REMOVE_T,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_REMOVE_T
	//EASYSIZE(IDC_SLIFY,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_SLIFY
	EASYSIZE(IDC_FIX_LOOP,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_FIX_LOOP
	EASYSIZE(IDC_SL_NAMING_CLICKED,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_SL_NAMING_CLICKED
	EASYSIZE(IDC_MOTIONBUILDER_FIX,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_MOTIONBUILDER_FIX
	//EASYSIZE(IDC_NO_ZERO_BTN,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_NO_ZERO_BTN
	EASYSIZE(IDC_CENTRE_BUTTON,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// Centre figure btn

	// joint properties window
	EASYSIZE(IDC_JOINT_PROPERTIES_SURROUND,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_JOINT_PROPERTIES_SURROUND
	EASYSIZE(IDC_JOINT_LABEL,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_JOINT_LABEL
	EASYSIZE(IDC_CHANNEL_ORDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_CHANNEL_ORDER
	EASYSIZE(IDC_PARENT,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_PARENT
	EASYSIZE(IDC_NCHANNELS,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_NCHANNELS
	EASYSIZE(IDC_HEIRARCHY,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_HEIRARCHY
	EASYSIZE(IDC_OFFSETS_LABEL,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_OFFSETS_LABEL
	EASYSIZE(IDC_OFFSETS_DISPX_E,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_OFFSETS_DISPX_E
	EASYSIZE(IDC_OFFSETS_DISPY_E,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_OFFSETS_DISPY_E
	EASYSIZE(IDC_OFFSETS_DISPZ_E,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_OFFSETS_DISPZ_E
	EASYSIZE(IDC_SCALE_DISPLAY,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_SCALE_DISPLAY
	
	EASYSIZE(IDC_LOOP_GROUP_BOX,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// 'Hack it about' group box
	EASYSIZE(IDC_TRIM_PT_L,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// Mark in point button
	EASYSIZE(IDC_TRIM_PT_R,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// Mark out point button
	EASYSIZE(IDC_TRIM_BTN,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// Crop (Trim) button
	EASYSIZE(IDC_TRIM_BTN2,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)// Knit ends button

	EASYSIZE(IDC_EDIT_CTRLS_SURROUND,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)//Anim frame/length group box	
	EASYSIZE(IDC_FRAME_COUNT,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)// frame count display

	EASYSIZE(IDC_STATIC,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)// File properties box
	EASYSIZE(IDC_PROPS_PANE,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,IDC_TIMELINE,0) // File properties text box
	EASYSIZE(IDC_FRAMERATE,ES_BORDER,ES_KEEPSIZE,ES_KEEPSIZE,IDC_TIMELINE,0) // Frame time label
	EASYSIZE(IDC_FRAMERATE_EDIT,IDC_FRAMERATE,ES_KEEPSIZE,ES_KEEPSIZE,IDC_TIMELINE,0) // Framerate adjustable display
	EASYSIZE(IDC_FRAMERATE_SPIN,IDC_FRAMERATE_EDIT,ES_KEEPSIZE,ES_KEEPSIZE,IDC_TIMELINE,0)  // spinner for framerate adjustemnt
	EASYSIZE(IDC_FRAMERATE2,ES_BORDER,IDC_FRAMERATE_SPIN,ES_KEEPSIZE,IDC_TIMELINE,0)  // fps display
	
	EASYSIZE(IDC_VIEW_SECTION_SURROUND,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_VIEW_SECTION_SURROUND
	EASYSIZE(IDC_RESET_VIEW,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_RESET_VIEW
	EASYSIZE(IDC_GND_VIEW_BTN,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_GND_VIEW_BTN
	EASYSIZE(IDC_RIGHT_VIEW_BUTTON,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_RIGHT_VIEW_BUTTON
	EASYSIZE(IDC_LEFT_VIEW_BUTTON,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_LEFT_VIEW_BUTTON

	EASYSIZE(IDC_ZOOM_IN,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)//IDC_ZOOM_IN
	EASYSIZE(IDC_ZOOM_OUT,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)//IDC_ZOOM_IN

	//EASYSIZE(IDC_COLOUR_DISPLAY,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_COLOUR_DISPLAY
	//EASYSIZE(IDC_SHOW_GROUND,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_SHOW_GROUND
	//EASYSIZE(IDC_DISP_1ST_FRAME_CHECK,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)// IDC_DISP_1ST_FRAME_CHECK

	EASYSIZE(IDC_TRANS_CTRLS_GROUP_BOX,ES_BORDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0)//Transport controls group box	
	EASYSIZE(IDC_SPACER_BTN,  ES_BORDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_HCENTER);// spacer
	EASYSIZE(IDC_BUTTON3,     ES_KEEPSIZE,ES_KEEPSIZE,IDC_SPACER_BTN,ES_BORDER,0)// Play button
	EASYSIZE(IDC_BUTTON4,     ES_KEEPSIZE,ES_KEEPSIZE,IDC_SPACER_BTN,ES_BORDER,0)// Stop button
	EASYSIZE(IDC_STEP_REV,    IDC_SPACER_BTN,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)// Step back button
	EASYSIZE(IDC_STEP_FWD_BTN,IDC_SPACER_BTN,ES_KEEPSIZE,ES_KEEPSIZE,ES_BORDER,0)// Step fwd button

END_EASYSIZE_MAP

// CbvhTweakDlg message handlers

void CbvhTweakDlg::OnBnClickedHidden()
{
	// Use a hidden Default button to catch any Returns
	// Make the IDOK button NOT a default button

	if (GetDlgItem(IDOK) == GetFocus())
	{
		CDialog::OnOK();
		return;
	}

	if (GetDlgItem(IDCANCEL) == GetFocus())
	{
		CDialog::OnCancel();
		return;
	}

	if(GetDlgItem(IDC_FRAMERATE_EDIT) == GetFocus())
	{
		OnEnKillfocusFramerateEdit();
	}

	if(GetDlgItem(IDC_SLIDER_RANGE) == GetFocus())
	{
		OnEnKillfocusSliderRange();
	}

	if(GetDlgItem(IDC_X_EDIT) == GetFocus())
	{
		OnEnKillfocusXEdit();
	}

	if(GetDlgItem(IDC_Y_EDIT) == GetFocus())
	{
		OnEnKillfocusYEdit();
	}

	if(GetDlgItem(IDC_Z_EDIT) == GetFocus())
	{
		OnEnKillfocusZEdit();
	}

	if(GetDlgItem(IDC_OFFSETS_DISPX_E) == GetFocus())
	{
		OnEnKillfocusOffsetsDispxE();
	}

	if(GetDlgItem(IDC_OFFSETS_DISPY_E) == GetFocus())
	{
		OnEnKillfocusOffsetsDispyE();
	}

	if(GetDlgItem(IDC_OFFSETS_DISPZ_E) == GetFocus())
	{
		OnEnKillfocusOffsetsDispzE();
	}
}

BOOL CbvhTweakDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	 m_greycolor=RGB(236,233,216);                      // dialog colour (hopefully!)
	 m_greybrush.CreateSolidBrush(m_greycolor);      // grey background

	INIT_EASYSIZE;

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Get size and position of the picture control
	CRect rect;
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);
	
	// Setup the dialog's timer to update dynamic controls - DRW 2/2010
	SetTimer(2, 50, 0);

	// get our pointer to the registry settings
	m_pRegistrySettings = theApp.m_pRegistrySettings;

	// initialise sliders
	m_XSlider.SetRange(0,m_pRegistrySettings->SliderRange);
	m_YSlider.SetRange(0,m_pRegistrySettings->SliderRange);
	m_ZSlider.SetRange(0,m_pRegistrySettings->SliderRange);

	// set the current slider range
	TCHAR wsSliderRangeDisplay[32];
	int nSliderRange = (int)m_pRegistrySettings->SliderRange;
	wsprintf(wsSliderRangeDisplay, _T("%d"), nSliderRange/20);
	m_SliderRangeAdjust.SetWindowTextA(wsSliderRangeDisplay);
	m_SliderRangeSpinner.SetRange(-nSliderRange/20, nSliderRange/20);// TODO: this don't work!
	m_SliderRangeSpinner.SetPos(nSliderRange/20);

	// set the mode and status
	if(m_pRegistrySettings->EditMode)
	{
		m_RotationRadioBtn.SetCheck(1);
		m_status = MODE_ROTATING;
	}
	else
	{
		m_TranslationRadioBtn.SetCheck(1);
		m_status = MODE_TRANSLATING;
	}


	// set the display options
	// show first frame?
	//m_Disp1stFrameBtn.SetCheck(m_pRegistrySettings->DisplayFirstFrame);
	if(m_pRegistrySettings->DisplayFirstFrame) 
		GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_CHECKED); 
	else
		GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_UNCHECKED); 

	// show ground?
	m_oglWindow.SetShowGround(m_pRegistrySettings->DisplayGroundPlane);
	
	// show colours?
	m_oglWindow.SetShowColours(m_pRegistrySettings->DisplayColour);
	
	// use shading?
	m_oglWindow.ApplyShading(m_pRegistrySettings->DisplayShading);

	// set the icons on the control buttons
	m_PlayButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
							MAKEINTRESOURCE(IDI_PLAY_ICON), 
							IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
	m_StopButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
							MAKEINTRESOURCE(IDI_STOP_ICON), 
							IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
	m_ReverseButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
							MAKEINTRESOURCE(IDI_REV_ICON), 
							IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
	m_ForwardButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
							MAKEINTRESOURCE(IDI_FWD_ICON), 
							IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 

	DisableControls();

	// load a file from the command line (i.e. open when double clicked)
	// jeez, that was a real hassle - turns out the bloody cmd line args 
	// are supplied with double quotes! MFC, u SUCK!
	if(strcmp(m_lpCmdLine, _T(""))!=0) {
		// open file using cmdline	
		CString sPathName(m_lpCmdLine);
		CString sToken("\"");
		sPathName.Remove(*sToken.GetBuffer());
		LoadBVHFromString(sPathName);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CbvhTweakDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CbvhTweakDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CbvhTweakDlg::OnBnClickedLoadBVH()
{
	// there is a bug where the tooltips for files on the desktop cause a crash. see:
	// http://groups.google.co.uk/group/microsoft.public.vc.mfc/browse_thread/thread/d8702cd5caeef25e/870e838ca7733081?lnk=st&q=CFileDialog+disable+tooltips&rnum=2&hl=en#870e838ca7733081
	if(!theApp.m_IsLocked) {

		try {
			// open file dialog
			CString szFilter("BVH Files (*.bvh)|*.bvh||");
			CBVHFileDialog* dlg = new CBVHFileDialog(true, NULL, NULL, (CBVHFileDialog::LISTVIEWCMD) m_pRegistrySettings->DefaultFileView, //
											  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING,
											  szFilter, NULL );
			
			//CWnd* pshellwnd = dlg->GetDlgItem(lst2);
			//pshellwnd->SendMessage(WM_COMMAND, ODM_VIEW_DETAIL); // 0x702c
			//dlg.m_ofn.Flags |= OFN_ENABLESIZING;
			if(dlg->DoModal()==IDCANCEL) {
				delete dlg;
				dlg = NULL;
				UnlockApp();	
				return;
			}
			theApp.m_IsLocked = true;// can't call LockApp here, as it tries to save the state of the file before it's loaded;// can't set it above, as it forgets! seriously weird!
			
			// clean up in the right order
			if(theApp.m_pBVHQuaterniser) delete theApp.m_pBVHQuaterniser;
			theApp.m_pBVHQuaterniser = NULL;
			if(theApp.m_pBVHFile) delete theApp.m_pBVHFile;
			theApp.m_pBVHFile = NULL;
			if(theApp.m_pBVHTweaker) delete theApp.m_pBVHTweaker;
			theApp.m_pBVHTweaker = NULL;

			// load the new file
			bool bSuccess = true;
			theApp.m_pBVHFile = new CBVHFile();
			CString sPathName(dlg->GetPathName());
			if(!(theApp.m_pBVHFile->LoadBVH(sPathName)))
			{
				AfxMessageBox(_T("Problem loading file"),MB_ICONSTOP);
				UnlockApp();	
				return;
			}
			theApp.m_pBVHTweaker = new CBVHTweaker(theApp.m_pBVHFile);
			theApp.m_pBVHQuaterniser = new CBVHQuaterniser(theApp.m_pBVHFile, theApp.m_pBVHTweaker);
			delete dlg;
			dlg = NULL;

		} catch (...) {
			
			AfxMessageBox(_T("Problem with file"),MB_ICONSTOP);
			UnlockApp();	
			return;
		}
		LoadBVH();
		UnlockApp();		
	}
}


void CbvhTweakDlg::LoadBVHFromLPTSTR(LPTSTR lpFileName)
{
	if(!theApp.m_IsLocked) {

		theApp.m_IsLocked = true;
		
		try 
		{
			// clean up in the right order
			if(theApp.m_pBVHQuaterniser) delete theApp.m_pBVHQuaterniser;
			theApp.m_pBVHQuaterniser = NULL;
			if(theApp.m_pBVHFile) delete theApp.m_pBVHFile;
			theApp.m_pBVHFile = NULL;
			if(theApp.m_pBVHTweaker) delete theApp.m_pBVHTweaker;
			theApp.m_pBVHTweaker = NULL;

			// load the new file 
			theApp.m_pBVHFile = new CBVHFile();
			CString sPathName(lpFileName);
			//CString message = "Path name is "+sPathName+" and is all fucking good!";
			//AfxMessageBox(message);
			if (!(theApp.m_pBVHFile->LoadBVH(sPathName)))
			{
				AfxMessageBox(_T("Problem loading file"), MB_ICONSTOP);
				UnlockApp();
				return;
			}
			theApp.m_pBVHTweaker = new CBVHTweaker(theApp.m_pBVHFile);
			theApp.m_pBVHQuaterniser = new CBVHQuaterniser(theApp.m_pBVHFile, theApp.m_pBVHTweaker);

		} catch (char * str) {
			
			AfxMessageBox(str,MB_ICONSTOP);
			UnlockApp();	
			return;
		}
		LoadBVH();
		UnlockApp();
	}
}


void CbvhTweakDlg::LoadBVHFromString(CString sPathName)
{
	if(!theApp.m_IsLocked) {

		theApp.m_IsLocked = true;
		
		try 
		{
			// clean up in the right order
			if(theApp.m_pBVHQuaterniser) delete theApp.m_pBVHQuaterniser;
			theApp.m_pBVHQuaterniser = NULL;
			if(theApp.m_pBVHFile) delete theApp.m_pBVHFile;
			theApp.m_pBVHFile = NULL;
			if(theApp.m_pBVHTweaker) delete theApp.m_pBVHTweaker;
			theApp.m_pBVHTweaker = NULL;

			// load the new file 
			theApp.m_pBVHFile = new CBVHFile();
			//CString message = "Path name is "+sPathName+" and is all good!";
			if (!(theApp.m_pBVHFile->LoadBVH(sPathName)))
			{
				AfxMessageBox(_T("Problem loading file"), MB_ICONSTOP);
				UnlockApp();
				return;
			}
			theApp.m_pBVHTweaker = new CBVHTweaker(theApp.m_pBVHFile);
			theApp.m_pBVHQuaterniser = new CBVHQuaterniser(theApp.m_pBVHFile, theApp.m_pBVHTweaker);

		} catch (char * str) {
			
			AfxMessageBox(str,MB_ICONSTOP);
			UnlockApp();	
			return;
		}
		LoadBVH();
		UnlockApp();
	}
}

void CbvhTweakDlg::LoadBVH()
{
	// set the tree ctrl
	//m_status|=FILE_LOADED;
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);

	// set the animation times 
	int nFrames = theApp.m_pBVHFile->GetNFrames();
	m_TimeLine.SetRange(0, nFrames-1);

	// set the frame duration (mS)
	float nFrameTime = theApp.m_pBVHFile->GetFrameTime();
	if(StatusIs(FILE_PLAYING)) {// reset the frame timer
		KillTimer(3);
		SetTimer(3, (UINT)nFrameTime, 0);
	}
	TCHAR wsFrameDisplay[16];
	sprintf(wsFrameDisplay,_T("%0.*f"),4,nFrameTime);
	m_FrameTimeDisplay.SetWindowTextA(wsFrameDisplay);

	// initialise the trim points
	m_nTrimPointLeft = 0;
	m_nTrimPointRight = nFrames-1;

	m_status|=FILE_LOADED;
	m_status|=FILE_JUST_LOADED;

	// display filename in the dialog's caption
	CString szFileName;
	szFileName = theApp.m_pBVHFile->GetFileName();		 
	szFileName.Append(_T(" - davedub's bvhacker"));
	SetWindowText(szFileName);

	// associate the tree control
	theApp.m_pBVHFile->SetAssociatedTreeControl(&m_SkeletonTree);

	// reset the trim button labels
	m_TrimLeftBtn.SetWindowTextA(_T("Mark in"));
	m_TrimRightBtn.SetWindowTextA(_T("Mark out"));

	// go
	EnableControls();

	if(!m_pRegistrySettings->DisplayFirstFrame)theApp.m_pBVHFile->MotionStepFwd();
	
	::SetFocus(m_SkeletonTree.m_hWnd);

	return;
}

void CbvhTweakDlg::OnBnClickedSaveBVH()
{
	// stop the file if it's playing
	if(!theApp.m_IsLocked&&StatusIs(FILE_LOADED)) {

		if(StatusIs(FILE_PLAYING)) {
			m_status-=FILE_PLAYING;
			m_PlayButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
									MAKEINTRESOURCE(IDI_PLAY_ICON), 
									IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
		}
		KillTimer(3);
		theApp.m_pBVHFile->SetCurrentMotionFrame(0);
	}
	
	if(!theApp.m_IsLocked&&StatusIs(FILE_LOADED)) {

		theApp.m_IsLocked = true;

		try {

			CString szFilter("BVH Files (*.bvh)|*.bvh||");
			CBVHFileDialog* dlg = new CBVHFileDialog(false, _T("bvh"), NULL, (CBVHFileDialog::LISTVIEWCMD)m_pRegistrySettings->DefaultFileView, //CBVHFileDialog::ODM_VIEW_DETAIL,
											  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT  | OFN_ENABLESIZING,
											  szFilter, NULL );
			//dlg->SetDefExt(_T("bvh"));
			if(dlg->DoModal()==IDCANCEL) {
				delete dlg;
				dlg = NULL;
				return;
			}

			theApp.m_pBVHFile->SaveBVH(dlg->GetPathName());
			delete dlg;
			dlg = NULL;

			// display movie filename in the dialog's caption
			CString szFileName;
			szFileName = theApp.m_pBVHFile->GetFileName();		 
			szFileName.Append(_T(" - davedub's bvhacker"));
			SetWindowText(szFileName);

		} catch(...) {

			AfxMessageBox(_T("Problem saving file"),MB_ICONSTOP);
			UnlockApp();	
			return;
		}
	}
	UnlockApp();	
}

void CbvhTweakDlg::OnBnClickedPlayBtn()
{
	if(!theApp.m_IsLocked&&StatusIs(FILE_LOADED)) 
	{
		theApp.m_IsLocked = true;

		if(StatusIs(FILE_PLAYING)) {

			// then pause
			KillTimer(3);
			m_status-=FILE_PLAYING;
			m_PlayButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
									MAKEINTRESOURCE(IDI_PLAY_ICON), 
									IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
		} else {

			// then play
			float frameTime = theApp.m_pBVHFile->GetFrameTime();
			SetTimer(3, (UINT)frameTime, 0);
			SetStatus(FILE_PLAYING);
			m_PlayButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
									MAKEINTRESOURCE(IDI_PAUSE_ICON), 
									IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
		}
		theApp.m_IsLocked = false;
		UpdateInterface();
		GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
	}
}

void CbvhTweakDlg::OnBnClickedStepFwdBtn()
{
	if(!theApp.m_IsLocked&&StatusIs(FILE_LOADED)) {
		theApp.m_IsLocked = true;
		theApp.m_pBVHFile->MotionStepFwd();
		if(!m_pRegistrySettings->DisplayFirstFrame&&(theApp.m_pBVHFile->GetCurrentFrameIdx()==0))
			theApp.m_pBVHFile->MotionStepFwd();
	}
	theApp.m_IsLocked = false;
	UpdateInterface();
	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
}

void CbvhTweakDlg::OnBnClickedStepRev()
{
	if(!theApp.m_IsLocked&&StatusIs(FILE_LOADED)) {
		theApp.m_IsLocked = true;
		theApp.m_pBVHFile->MotionStepRev();
		if(!m_pRegistrySettings->DisplayFirstFrame&&(theApp.m_pBVHFile->GetCurrentFrameIdx()==0))
			theApp.m_pBVHFile->MotionStepRev();
	}
	theApp.m_IsLocked = false;
	UpdateInterface();
	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
}

// called manually after each user update performed via GUI
void CbvhTweakDlg::UpdateInterface()
{
	if(StatusIs(FILE_LOADED)) {

		CBVHFile* BVHFile =  theApp.m_pBVHFile;
		CString szCurrentJoint = m_SkeletonTree.GetCurrentSelection();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(szCurrentJoint);

		TCHAR wsProperty[128];
		TCHAR wsX[16]; 
		TCHAR wsY[16]; 
		TCHAR wsZ[16]; 

		// adjust sliders
		GLfloat X = 0;
		GLfloat Y = 0;
		GLfloat Z = 0;

		// decimal places to display on values in text controls
		int nPrecision = 0; 

		// Display any scale information
		TCHAR wsScaleInfo[128];
		if(pJoint->scaleChannelOrder!=NONE)
		{
			nPrecision = 1;
			theApp.m_pBVHTweaker->GetCurrentSelScales(&X,&Y,&Z, szCurrentJoint);
			sprintf(wsScaleInfo,_T("Scale: %0.*f  %0.*f  %0.*f"),nPrecision,X,nPrecision,Y,nPrecision,Z);
			m_Scale_Display.SetWindowTextA(wsScaleInfo);
		} 
		else
		{
			m_Scale_Display.SetWindowTextA(_T("No scale data"));
		}

		// display offsets in joint properties window	
		theApp.m_pBVHTweaker->GetCurrentSelOffsets(&X,&Y,&Z, szCurrentJoint);
		nPrecision = 4;
		sprintf(wsX,_T("%0.*f"),nPrecision,X);
		sprintf(wsY,_T("%0.*f"),nPrecision,Y);
		sprintf(wsZ,_T("%0.*f"),nPrecision,Z);	
		
		wsprintf(wsProperty, _T("X:  %s"), wsX);
		m_OffsetsDispX_E.SetWindowTextA(wsProperty);
		wsprintf(wsProperty, _T("Y:  %s"), wsY);
		m_OffsetsDispY_E.SetWindowTextA(wsProperty);
		wsprintf(wsProperty, _T("Z:  %s"), wsZ);
		m_OffsetsDispZ_E.SetWindowTextA(wsProperty);

		// display joint slider info
		if(StatusIs(MODE_TRANSLATING))
		{
			TCHAR wsProperty[64];
			if(pJoint->offsetChannelOrder!=NONE)
			{
				EnableSliderControls();
				if(pJoint->isROOT) wsprintf(wsProperty, _T("Joint translations"));
				else wsprintf(wsProperty, _T("Joint offsets"));
			}
			else
			{
				EnableSliderControls();
				wsprintf(wsProperty, _T("Joint offsets"));
			}
			m_AdjustmentInfoLabel.SetWindowTextA(wsProperty);
		}
		else if(StatusIs(MODE_ROTATING)) 
		{
			TCHAR wsProperty[64];
			if(pJoint->channelOrder!=NONE)
			{
				EnableSliderControls();
				if(pJoint->isROOT&&m_bGlobalEditing&&pJoint->channelOrder==ZXY) 
					wsprintf(wsProperty, _T("Joint rotations*"));
				else
					wsprintf(wsProperty, _T("Joint rotations"));
			}
			else
			{
				DisableSliderControls();
				wsprintf(wsProperty, _T("No rotation data"));
			}
			m_AdjustmentInfoLabel.SetWindowTextA(wsProperty);
		}

		// display translations or rotations on sliders and labels
		if(m_bGlobalEditing&&StatusIs(MODE_ROTATING)&&pJoint->isROOT)
		{
			Vector3<float> vGlobalRotations = theApp.m_pBVHQuaterniser->GetCurrentGlobalRotation(pJoint);
			X=vGlobalRotations.x;Y=vGlobalRotations.y;Z=vGlobalRotations.z;
		}
		else
		{
			if(StatusIs(MODE_ROTATING)) 
				theApp.m_pBVHTweaker->GetCurrentSelRotations(&X,&Y,&Z, szCurrentJoint);
			else if(StatusIs(MODE_TRANSLATING))
			{
				if(pJoint->offsetChannelOrder!=NONE)
					theApp.m_pBVHTweaker->GetCurrentSelTranslations(&X,&Y,&Z, szCurrentJoint);
				else theApp.m_pBVHTweaker->GetCurrentSelOffsets(&X,&Y,&Z, szCurrentJoint);
			}
		}

		nPrecision = 2;
		sprintf(wsX,_T("%0.*f"),nPrecision,X);
		sprintf(wsY,_T("%0.*f"),nPrecision,Y);
		sprintf(wsZ,_T("%0.*f"),nPrecision,Z);		

		m_XValEditDisplay.SetWindowTextA(wsX);
		m_YValEditDisplay.SetWindowTextA(wsY);
		m_ZValEditDisplay.SetWindowTextA(wsZ);

		// update the sliders
		float nSliderRangeDeg = (float)m_pRegistrySettings->SliderRange/20;
		
		if(X>nSliderRangeDeg) X=nSliderRangeDeg;
		if(Y>nSliderRangeDeg) Y=nSliderRangeDeg;
		if(Z>nSliderRangeDeg) Z=nSliderRangeDeg;

		if(X< -nSliderRangeDeg) X=-nSliderRangeDeg;
		if(Y< -nSliderRangeDeg) Y=-nSliderRangeDeg;
		if(Z< -nSliderRangeDeg) Z=-nSliderRangeDeg;

		m_XSlider.SetPos(10*((int)nSliderRangeDeg-(int)X));
		m_YSlider.SetPos(10*((int)nSliderRangeDeg-(int)Y));
		m_ZSlider.SetPos(10*((int)nSliderRangeDeg-(int)Z));

		// set the current joint label
		wsprintf(wsProperty, _T("Name: %s"), szCurrentJoint);
		m_CurrentJointLabel.SetWindowTextA(wsProperty);

		// set the joint ordering label
		switch(pJoint->channelOrder) {

			case XYZ: {
				wsprintf(wsProperty, _T("Rotation Order: XYZ"));
				break;
			}
			case XZY: {
				wsprintf(wsProperty, _T("Rotation Order: XZY"));
				break;
			}
			case YZX: {
				wsprintf(wsProperty, _T("Rotation Order: YZX"));
				break;
			}
			case ZXY: {
				wsprintf(wsProperty, _T("Rotation Order: ZXY"));
				break;
			}
			case ZYX: {
				wsprintf(wsProperty, _T("Rotation Order: ZYX"));
				break;
			}
			case YXZ: {
				wsprintf(wsProperty, _T("Rotation Order: YXZ"));
				break;
			}
			case NONE: {
				wsprintf(wsProperty, _T("No rotation data"));
				break;
			}
			default: {
				wsprintf(wsProperty, _T("Unsupported"));
				break;
			}
		}
		m_channelOrderDisplay.SetWindowTextA(wsProperty);
		
		// set the hierarchy label
		wsprintf(wsProperty, _T("Index, Depth: %d, %d"), pJoint->index, pJoint->heirarchy);
		m_heirarchyDisplay.SetWindowTextA(wsProperty);

		// set the n channels label
		CString szChannelsInfo = (_T(""));
		if(pJoint->offsetChannelOrder!=NONE) szChannelsInfo+=(_T("T"));
		if(pJoint->channelOrder!=NONE) szChannelsInfo+=(_T("R"));
		if(pJoint->scaleChannelOrder!=NONE) szChannelsInfo+=(_T("S"));
		if(pJoint->isEndSite) wsprintf(wsProperty, _T("No data channels"), pJoint->nChannels,szChannelsInfo);
		else wsprintf(wsProperty, _T("Channels: %d (%s)"), pJoint->nChannels,szChannelsInfo);
		
		m_nChannelsDisplay.SetWindowTextA(wsProperty);
		
		// set the parent label
		if(!pJoint->isROOT) {
			BVHJoint* pParent = theApp.m_pBVHFile->GetJoint(pJoint->parentIndex);
			int nStringLength = pParent->JOINTName.GetLength();
			if(nStringLength> 20) 
			{

			}
			wsprintf(wsProperty, _T("Parent: %s"), pParent->JOINTName);
			m_parentDisplay.SetWindowTextA(wsProperty);
			pParent = NULL;
		}
		else {
			wsprintf(wsProperty, _T("No Parent"));
			m_parentDisplay.SetWindowTextA(wsProperty);
		}

		// clean up
		pJoint = NULL;
		X=NULL;
		Y=NULL;
		Z=NULL;
	}
	
	// set the properties pane
	TCHAR wsProperties[256];
	CBVHFile* BVHFile =  theApp.m_pBVHFile;
	CString czFileName = _T("");
	int nMaxStrLen = 38;
	if(BVHFile->GetFileName().GetLength()>nMaxStrLen)
		czFileName = BVHFile->GetFileName().Left((nMaxStrLen/2)-3)+_T("...")+BVHFile->GetFileName().Right(nMaxStrLen/2);
	else czFileName = BVHFile->GetFileName();
	int armsAlignment = theApp.m_pBVHFile->GetArmsAlignment();
	if(armsAlignment==X_AXIS_ALIGNED_ARMS) sprintf(wsProperties, _T("File: %s \r\n\r\nNumber of joints (including End Sites): %d\r\n\r\nNumber of motion channels: %d\r\n\r\nFigure Height (including End Sites): %0.*f\r\n\r\nArms alignment: along X axis "),
							  czFileName, BVHFile->GetNJoints(), BVHFile->GetNParameters(), 1, BVHFile->GetFigureHeight());
	else if(armsAlignment==Y_AXIS_ALIGNED_ARMS) sprintf(wsProperties, _T("File: %s \r\n\r\nNumber of joints (including End Sites): %d\r\n\r\nNumber of motion channels: %d\r\n\r\nFigure Height (including End Sites): %0.*f\r\n\r\nArms alignment: along Y axis"),
							  czFileName, BVHFile->GetNJoints(), BVHFile->GetNParameters(), 1, BVHFile->GetFigureHeight());
	else sprintf(wsProperties, _T("File: %s \r\n\r\nNumber of joints (including End Sites): %d\r\n\r\nNumber of motion channels: %d\r\n\r\nFigure Height (including End Sites): %0.*f\r\n\r\nArms axis alignment unknown"),
							  czFileName, BVHFile->GetNJoints(), BVHFile->GetNParameters(), 1, BVHFile->GetFigureHeight());
	m_PropsPane.SetWindowTextA(wsProperties);
	czFileName = _T("");

	// do whether or not the file is loaded
	int nFrameNumber = theApp.m_pBVHFile->GetCurrentFrameIdx();
	int nTotalFrames = theApp.m_pBVHFile->GetNFrames();		
	float fFrameTime = theApp.m_pBVHFile->GetFrameTime();

	// set the frame duration
	TCHAR wsFrameTimeDisplay[16];
	sprintf(wsFrameTimeDisplay,_T("%0.*f"),4,theApp.m_pBVHFile->GetFrameTime());
	m_FrameTimeDisplay.SetWindowTextA(wsFrameTimeDisplay);

	// set the fps
	float fFPS = 1000/fFrameTime;
	TCHAR wsFPSDisplay[16];
	sprintf(wsFPSDisplay,_T(" =  %0.*f fps"),1,fFPS);
	m_FpsDisplay.SetWindowTextA(wsFPSDisplay);

	// update timeline
	int currentFrameIdx = theApp.m_pBVHFile->GetCurrentFrameIdx();
	m_TimeLine.SetPos((int)currentFrameIdx);

	// set the frame number display
	TCHAR wsFrameNumberDisplay[1280];
	sprintf(wsFrameNumberDisplay, _T("Animation time: %0.*f / %0.*f seconds                Frame number: %d / %d "), 3, ((float)nFrameNumber/(float)nTotalFrames)*(nTotalFrames*fFrameTime/1000), 3, (nTotalFrames*fFrameTime/1000),nFrameNumber+1, nTotalFrames);
	m_FrameCountDisplay.SetWindowTextA(wsFrameNumberDisplay);

	wsprintf(wsFrameNumberDisplay, _T(""));

	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();

	BVHFile = NULL;
}

void CbvhTweakDlg::OnTimer(UINT_PTR nIDEvent)
{
	// Sigh. The view gets changed on load because the ogl window captures 
	// the mouse click from the file dialog over it and applies it as a 
	// view change. This little hack restores the default view on file load
	if(StatusIs(FILE_JUST_LOADED)) {
		m_oglWindow.ResetView();
		m_status-=FILE_JUST_LOADED;
	}

	bool bFileLoaded = StatusIs(FILE_LOADED);

	if(!theApp.m_IsLocked&&bFileLoaded) {
		
		if(!theApp.m_IsLocked) {

			theApp.m_IsLocked = true;// don't use LockApp() here, as we don't want to save the state of the file every frame!
			
			// GUI control updates 
			if(nIDEvent==2) {

				CBVHFile* BVHFile =  theApp.m_pBVHFile;

				CString szCurrentJoint = m_SkeletonTree.GetCurrentSelection();
				BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(szCurrentJoint);
				TCHAR wsProperty[128];
				TCHAR wsX[16]; 
				TCHAR wsY[16]; 
				TCHAR wsZ[16]; 

				// adjust sliders
				GLfloat X = 0;
				GLfloat Y = 0;
				GLfloat Z = 0;

				if(StatusIs(FILE_PLAYING))
				{
					// display translations or rotations on sliders
					if(m_bGlobalEditing&&StatusIs(MODE_ROTATING)&&pJoint->isROOT)
					{
						Vector3<float> vGlobalRotations = theApp.m_pBVHQuaterniser->GetCurrentGlobalRotation(pJoint);
						X=vGlobalRotations.x;Y=vGlobalRotations.y;Z=vGlobalRotations.z;
					}
					else
					{
						if(StatusIs(MODE_ROTATING)) 
							theApp.m_pBVHTweaker->GetCurrentSelRotations(&X,&Y,&Z, szCurrentJoint);
						else if(StatusIs(MODE_TRANSLATING))
						{
							if(pJoint->offsetChannelOrder!=NONE)
								theApp.m_pBVHTweaker->GetCurrentSelTranslations(&X,&Y,&Z, szCurrentJoint);
							else theApp.m_pBVHTweaker->GetCurrentSelOffsets(&X,&Y,&Z, szCurrentJoint);
						}
					}

					int nPrecision = 2;
					sprintf(wsX,_T("%0.*f"),nPrecision,X);
					sprintf(wsY,_T("%0.*f"),nPrecision,Y);
					sprintf(wsZ,_T("%0.*f"),nPrecision,Z);		

					m_XValEditDisplay.SetWindowTextA(wsX);
					m_YValEditDisplay.SetWindowTextA(wsY);
					m_ZValEditDisplay.SetWindowTextA(wsZ);

					float nSliderRangeDeg = (float)m_pRegistrySettings->SliderRange/20;
					
					if(X>nSliderRangeDeg) X=nSliderRangeDeg;
					if(Y>nSliderRangeDeg) Y=nSliderRangeDeg;
					if(Z>nSliderRangeDeg) Z=nSliderRangeDeg;

					if(X< -nSliderRangeDeg) X=-nSliderRangeDeg;
					if(Y< -nSliderRangeDeg) Y=-nSliderRangeDeg;
					if(Z< -nSliderRangeDeg) Z=-nSliderRangeDeg;

					m_XSlider.SetPos(10*((int)nSliderRangeDeg-(int)X));
					m_YSlider.SetPos(10*((int)nSliderRangeDeg-(int)Y));
					m_ZSlider.SetPos(10*((int)nSliderRangeDeg-(int)Z));

					wsprintf(wsProperty, _T(""));
					wsprintf(wsX, _T(""));
					wsprintf(wsY, _T(""));
					wsprintf(wsZ, _T(""));
					pJoint = NULL;

					// update timeline
					int currentFrameIdx = theApp.m_pBVHFile->GetCurrentFrameIdx();
					m_TimeLine.SetPos((int)currentFrameIdx);
				}

				CDialog::OnTimer(nIDEvent);

				// do whether or not the file is loaded
				int nFrameNumber = theApp.m_pBVHFile->GetCurrentFrameIdx();
				int nTotalFrames = theApp.m_pBVHFile->GetNFrames();		
				float fFrameTime = theApp.m_pBVHFile->GetFrameTime();

				// set the frame number display
				TCHAR wsFrameNumberDisplay[1280];
				sprintf(wsFrameNumberDisplay, _T("Animation time: %0.*f / %0.*f seconds                Frame number: %d / %d "), 3, ((float)nFrameNumber/(float)nTotalFrames)*(nTotalFrames*fFrameTime/1000), 3, (nTotalFrames*fFrameTime/1000),nFrameNumber+1, nTotalFrames);
				m_FrameCountDisplay.SetWindowTextA(wsFrameNumberDisplay);

				wsprintf(wsFrameNumberDisplay, _T(""));

				BVHFile = NULL;
			}
			//separate timer for animation
			else if ((nIDEvent==3)&&!StatusIs(EDITING_PAUSE)) {	

				// advance one motion frame
				theApp.m_pBVHFile->MotionStepFwd();

				// advance another if not showing 1st frame and frame number = 0
				if(!m_pRegistrySettings->DisplayFirstFrame&&(theApp.m_pBVHFile->GetCurrentFrameIdx()==0))
					theApp.m_pBVHFile->MotionStepFwd();
			}
		}
	}
	theApp.m_IsLocked = false;// don't replace this with UnlockApp() - will cause problems
}

void CbvhTweakDlg::OnBnClickedStopBtn()
{
	if(!theApp.m_IsLocked&&StatusIs(FILE_LOADED)) 
	{
		theApp.m_IsLocked = true;
		if(StatusIs(FILE_PLAYING)) {
			m_status-=FILE_PLAYING;
			m_PlayButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
									MAKEINTRESOURCE(IDI_PLAY_ICON), 
									IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
		}
		KillTimer(3);
		theApp.m_pBVHFile->SetCurrentMotionFrame(0);
		if(!m_pRegistrySettings->DisplayFirstFrame)
			theApp.m_pBVHFile->MotionStepFwd();
		theApp.m_IsLocked = false;
		UpdateInterface();
	}
}


void CbvhTweakDlg::OnBnClickedResetView()
{
	m_oglWindow.ResetView();
	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
}

void CbvhTweakDlg::OnBnClickedGndViewBtn()
{
	m_oglWindow.SetGroundView();
	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
}

void CbvhTweakDlg::OnNMReleasedCaptureSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW  pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}

int CbvhTweakDlg::GetStatus(void)
{
	return m_status;
}

void CbvhTweakDlg::OnDeltaposSliderRange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int incr = 100;
	if(pNMUpDown->iDelta<1) incr = -100;
	m_pRegistrySettings->SliderRange = m_pRegistrySettings->SliderRange+incr;
	if(m_pRegistrySettings->SliderRange>7301) m_pRegistrySettings->SliderRange = 20;// oops - we looped right round there!
	if(m_pRegistrySettings->SliderRange>7200) m_pRegistrySettings->SliderRange = 7200;
	if(m_pRegistrySettings->SliderRange<20) m_pRegistrySettings->SliderRange = 20;
	m_XSlider.SetRange(0, m_pRegistrySettings->SliderRange);
	m_YSlider.SetRange(0, m_pRegistrySettings->SliderRange);
	m_ZSlider.SetRange(0, m_pRegistrySettings->SliderRange);
	m_SliderRangeSpinner.SetPos(m_pRegistrySettings->SliderRange/20);
	TCHAR wsSliderRangeDisplay[32];
	wsprintf(wsSliderRangeDisplay, _T("%d"), m_pRegistrySettings->SliderRange/20);
	m_SliderRangeAdjust.SetWindowTextA(wsSliderRangeDisplay);
	UpdateInterface();
	*pResult = 0;
}

void CbvhTweakDlg::OnDeltaposFramerateSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int incr = -1;
	if(pNMUpDown->iDelta<1) incr = 1;

	
	float nFrameTime = theApp.m_pBVHFile->GetFrameTime();
	
	// new for November 2009
	if(nFrameTime>100&&nFrameTime<1000) incr*=25;
	if(nFrameTime>1000) incr*=100;

	nFrameTime+=(float)incr;
	if(nFrameTime>5000) nFrameTime = 5000;
	if(nFrameTime<5) nFrameTime = 5;
	m_FrameTimeSpinner.SetPos((int)nFrameTime);
	theApp.m_pBVHFile->SetFrameTime(nFrameTime);
	if(StatusIs(FILE_PLAYING)) {
		KillTimer(3);
		SetTimer(3, (int)nFrameTime, 0);
	}
	*pResult = 0;

	UpdateInterface();
}

void CbvhTweakDlg::SetEditingMode(bool bEdit)
{
	switch(bEdit) {

		case true: 
			{
				if(!StatusIs(EDITING_PAUSE)) {
					m_status+=EDITING_PAUSE;
				}
				break;
			}
		case false:
			{
				if(StatusIs(EDITING_PAUSE)) { 
					m_status-=EDITING_PAUSE;
				}
				break;
			}
	}

	UpdateInterface();
}

bool CbvhTweakDlg::StatusIs(int statusToCompareTo)
{
	int test = m_status&statusToCompareTo;
	if(test==statusToCompareTo) return true;
	else return false;
}

void CbvhTweakDlg::SetStatus(int status)
{
	m_status|=status;
}

void CbvhTweakDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(StatusIs(FILE_LOADED)) {

		CNiceSliderCtrl* pSliderCtrl = (CNiceSliderCtrl*) pScrollBar;

		// if it's the timeline slider...
		if(pSliderCtrl == &m_TimeLine) {
			if(!m_pRegistrySettings->DisplayFirstFrame&&nPos==0)nPos++;
			theApp.m_pBVHFile->SetCurrentMotionFrame(nPos);
		}

		float fPos = (float)nPos;
		float fDelta = 0;
		if(m_bNewScroll) {
			fDelta = 0;
			m_bNewScroll = false;
		}
		else {
			fDelta = fPos-m_fLastSliderPos;
		}
		m_fLastSliderPos = fPos;

		//set editing mode if we are dragging
		if (nSBCode == TB_THUMBTRACK) SetEditingMode(true);
		if (nSBCode == TB_ENDTRACK) {
			SetEditingMode(false);
			m_fLastSliderPos = 0;
			m_bNewScroll = true;
		}

		// apply new adjustments
		int mode=0;
		CString szCurrentJoint = m_SkeletonTree.GetCurrentSelection();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(szCurrentJoint);
		StatusIs(MODE_TRANSLATING)?mode=MODE_TRANSLATING:mode=MODE_ROTATING;

		if(m_bGlobalEditing&&mode==MODE_ROTATING&&pJoint->isROOT)
		{
			float fDeltaReductionFactor=10.f;
			Vector3<float> vLocalRots(0.f,0.f,0.f);
			if(pSliderCtrl == &m_XSlider) vLocalRots.x=-fDelta/fDeltaReductionFactor;
			if(pSliderCtrl == &m_YSlider) vLocalRots.y=-fDelta/fDeltaReductionFactor;
			if(pSliderCtrl == &m_ZSlider) vLocalRots.z=-fDelta/fDeltaReductionFactor;
			bool bTest = theApp.m_pBVHQuaterniser->RotateGlobal(pJoint,vLocalRots);
			if(!bTest) {
				if(pSliderCtrl == &m_XSlider) {
					theApp.m_pBVHTweaker->AddMotion(-fDelta/10, 0.0, 0.0, mode, szCurrentJoint);
				}
				if(pSliderCtrl == &m_YSlider) {
					theApp.m_pBVHTweaker->AddMotion(0.0, -fDelta/10, 0.0, mode, szCurrentJoint);
				}
				if(pSliderCtrl == &m_ZSlider) {
					theApp.m_pBVHTweaker->AddMotion(0.0, 0.0, -fDelta/10, mode, szCurrentJoint);
				}		
			}
		}
		else
		{
			if(pSliderCtrl == &m_XSlider) {
				theApp.m_pBVHTweaker->AddMotion(-fDelta/10, 0.0, 0.0, mode, szCurrentJoint);
			}
			if(pSliderCtrl == &m_YSlider) {
				theApp.m_pBVHTweaker->AddMotion(0.0, -fDelta/10, 0.0, mode, szCurrentJoint);
			}
			if(pSliderCtrl == &m_ZSlider) {
				theApp.m_pBVHTweaker->AddMotion(0.0, 0.0, -fDelta/10, mode, szCurrentJoint);
			}
		}

		theApp.m_pBVHFile->CalculateFigureHeight();
		return;
	}
	UpdateInterface();
}

void CbvhTweakDlg::OnBnClickedRotSel()
{
	if(!StatusIs(MODE_ROTATING)) {
		m_status+=MODE_ROTATING;
		m_status-=MODE_TRANSLATING;
		m_pRegistrySettings->EditMode = TRUE;
	}
	UpdateInterface();
}

void CbvhTweakDlg::OnBnClickedTranslationSel()
{
	if(!StatusIs(MODE_TRANSLATING)) {
		m_status-=MODE_ROTATING;
		m_status+=MODE_TRANSLATING;
		m_pRegistrySettings->EditMode = FALSE;
	}
	UpdateInterface();
}

void CbvhTweakDlg::OnBnClickedClampX()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
		theApp.m_pBVHTweaker->ZeroOutChannel(N_X,  GetStatus(), m_SkeletonTree.GetCurrentSelection());
	UnlockApp();
	UpdateInterface();
}

void CbvhTweakDlg::OnBnClickedClampY()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
		theApp.m_pBVHTweaker->ZeroOutChannel(N_Y,  GetStatus(), m_SkeletonTree.GetCurrentSelection());
	UnlockApp();
	UpdateInterface();
}

void CbvhTweakDlg::OnBnClickedClampZ()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
		theApp.m_pBVHTweaker->ZeroOutChannel(N_Z,  GetStatus(), m_SkeletonTree.GetCurrentSelection());
	UnlockApp();
	UpdateInterface();
}

void CbvhTweakDlg::OnBnClickedTrimPtLeft()
{
	if(StatusIs(FILE_LOADED)) {
		LockApp();
		int nTrimPointLeft = theApp.m_pBVHFile->GetCurrentFrameIdx();
		if(nTrimPointLeft>m_nTrimPointRight)
		{
			UnlockApp();
			return;
		}
		m_nTrimPointLeft = nTrimPointLeft;
		TCHAR wsButtonLabel[16];
		wsprintf(wsButtonLabel, _T("L: %d"), m_nTrimPointLeft+1);
		m_TrimLeftBtn.SetWindowTextA(wsButtonLabel);
		UnlockApp();
	}
}

void CbvhTweakDlg::OnBnClickedTrimPtR()
{
	if(StatusIs(FILE_LOADED)) {
		LockApp();
		int nTrimPointRight = theApp.m_pBVHFile->GetCurrentFrameIdx();
		if(nTrimPointRight<m_nTrimPointLeft) 
		{
			UnlockApp();
			return;
		}
		m_nTrimPointRight = nTrimPointRight;
		TCHAR wsButtonLabel[16];
		wsprintf(wsButtonLabel, _T("R: %d"), m_nTrimPointRight+1);
		m_TrimRightBtn.SetWindowTextA(wsButtonLabel);
		UnlockApp();
	}
}

void CbvhTweakDlg::OnBnClickedTrimBtn()
{
	LockApp();
	if(StatusIs(FILE_LOADED)) {
		theApp.m_pBVHTweaker->TrimFile(m_nTrimPointLeft, m_nTrimPointRight);
		m_TimeLine.SetRange(0, theApp.m_pBVHFile->GetNFrames()-1);
		int newPos = 0;
		if(!m_pRegistrySettings->DisplayFirstFrame) newPos++;
		theApp.m_pBVHFile->SetCurrentMotionFrame(newPos);
		TCHAR wsButtonLabel[16];
		wsprintf(wsButtonLabel, _T("Mark in"));
		m_TrimLeftBtn.SetWindowTextA(wsButtonLabel);
		wsprintf(wsButtonLabel, _T("Mark out"));
		m_TrimRightBtn.SetWindowTextA(wsButtonLabel);
		m_nTrimPointLeft = 0;
		m_nTrimPointRight = theApp.m_pBVHFile->GetNFrames()-1;
		if(theApp.m_pBVHFile->GetNFrames()==1) 
		{
			m_pRegistrySettings->DisplayFirstFrame = TRUE;
			GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_CHECKED);
		}
	} 
	UnlockApp();
}

// new scope values under test (March 2008) - ideally make adjustable - DRW
// decided to just spread over entire length - June 2010
void CbvhTweakDlg::OnBnClickedKnitEnds()
{
	LockApp();
	int nBlendScope = theApp.m_pBVHFile->GetNFrames()/5;
	if(nBlendScope>32) nBlendScope=32;
	theApp.m_pBVHTweaker->KnitEnds(nBlendScope);
	//theApp.m_pBVHTweaker->TrimFile(0, theApp.m_pBVHFile->GetNFrames()-2);
	UnlockApp();
}

void CbvhTweakDlg::OnBnClickedHelpBtn()
{
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.bvhacker.com/help.html"),_T(""),NULL,0);
}

void CbvhTweakDlg::OnBnClickedDisp1stFrameCheck()
{
	//m_pRegistrySettings->DisplayFirstFrame = m_Disp1stFrameBtn.GetCheck();
	if(StatusIs(FILE_LOADED)) {	
		if(!m_pRegistrySettings->DisplayFirstFrame)
		{
			if(theApp.m_pBVHFile->GetCurrentFrameIdx()==0) theApp.m_pBVHFile->MotionStepFwd();
			GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_UNCHECKED); 
		}
		else
		{
			GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_CHECKED);
		}

	} else {
		//m_Disp1stFrameBtn.SetCheck(m_Disp1stFrameBtn.GetCheck());
	}
}

void CbvhTweakDlg::OnViewDisplayfirstframe()
{
	if(theApp.m_pBVHFile->GetNFrames()==1) 
	{
		m_pRegistrySettings->DisplayFirstFrame = TRUE;
		GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_CHECKED);
		return;
	}

	if(m_pRegistrySettings->DisplayFirstFrame)
	{
		m_pRegistrySettings->DisplayFirstFrame = FALSE;
		GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_UNCHECKED); 
		if(theApp.m_pBVHFile->GetCurrentFrameIdx()==0)
			theApp.m_pBVHFile->MotionStepFwd();
	}
	else
	{
		m_pRegistrySettings->DisplayFirstFrame = TRUE;
		GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_CHECKED);
	}
}

void CbvhTweakDlg::OnBnClickedColourDisplay()
{
	if(m_pRegistrySettings->DisplayColour) 
	{ 
		m_pRegistrySettings->DisplayColour = FALSE;
		GetMenu()->CheckMenuItem(ID_VIEW_COLOURDISPLAY, MF_UNCHECKED);
	}
	else 
	{
		m_pRegistrySettings->DisplayColour = TRUE;
		GetMenu()->CheckMenuItem(ID_VIEW_COLOURDISPLAY, MF_CHECKED);
	}

	m_oglWindow.SetShowColours(m_pRegistrySettings->DisplayColour);
}



void CbvhTweakDlg::OnViewColourdisplay()
{
	if(m_pRegistrySettings->DisplayColour) 
	{ 
		m_pRegistrySettings->DisplayColour = FALSE;
		GetMenu()->CheckMenuItem(ID_VIEW_COLOURDISPLAY, MF_UNCHECKED);
	}
	else 
	{
		m_pRegistrySettings->DisplayColour = TRUE;
		GetMenu()->CheckMenuItem(ID_VIEW_COLOURDISPLAY, MF_CHECKED);
	}
	
	m_ColourDisplayCheck.SetCheck(m_pRegistrySettings->DisplayColour);
	m_oglWindow.SetShowColours(m_pRegistrySettings->DisplayColour);
}



void CbvhTweakDlg::OnViewShading()
{
	if(m_pRegistrySettings->DisplayShading) 
	{ 
		m_oglWindow.ApplyShading(false);
		m_pRegistrySettings->DisplayShading = FALSE;
		//GetMenu()->CheckMenuItem(ID_VIEW_SHADING, MF_UNCHECKED);
	}
	else 
	{
		m_oglWindow.ApplyShading(true);
		m_pRegistrySettings->DisplayShading = TRUE;
		//GetMenu()->CheckMenuItem(ID_VIEW_SHADING, MF_CHECKED);
	}
	//m_ColourDisplayCheck.SetCheck(m_pRegistrySettings->DisplayShading);
}

void CbvhTweakDlg::OnBnClickedShowGround()
{
	if(m_pRegistrySettings->DisplayGroundPlane) 
	{ 
		m_pRegistrySettings->DisplayGroundPlane = FALSE;
		//GetMenu()->CheckMenuItem(ID_VIEW_SHOWGROUND, MF_UNCHECKED);
	}
	else 
	{
		m_pRegistrySettings->DisplayGroundPlane = TRUE;
		//GetMenu()->CheckMenuItem(ID_VIEW_SHOWGROUND, MF_CHECKED);
	}

	m_oglWindow.SetShowGround(m_pRegistrySettings->DisplayGroundPlane);
}


void CbvhTweakDlg::OnClickedShowHead()
{
	if(m_pRegistrySettings->DisplayHeadMesh) 
	{ 
		m_pRegistrySettings->DisplayHeadMesh = FALSE;
	}
	else 
	{
		m_pRegistrySettings->DisplayHeadMesh = TRUE;
	}
	m_oglWindow.ShowHead(m_pRegistrySettings->DisplayHeadMesh);
}
void CbvhTweakDlg::OnViewShowground()
{
	if(m_pRegistrySettings->DisplayGroundPlane) 
	{ 
		m_pRegistrySettings->DisplayGroundPlane = FALSE;
		//GetMenu()->CheckMenuItem(ID_VIEW_SHOWGROUND, MF_UNCHECKED);
	}
	else 
	{
		m_pRegistrySettings->DisplayGroundPlane = TRUE;
		//GetMenu()->CheckMenuItem(ID_VIEW_SHOWGROUND, MF_CHECKED);
	}

	//m_ShowGroundCheck.SetCheck(m_pRegistrySettings->DisplayGroundPlane);
	m_oglWindow.SetShowGround(m_pRegistrySettings->DisplayGroundPlane);
}

// set t shape
void CbvhTweakDlg::OnBnClickedSetT()
{
	LockApp();
	theApp.m_pBVHTweaker->SetTShape();
	int nFrames = theApp.m_pBVHFile->GetNFrames();
	m_TimeLine.SetRange(0, nFrames-1);
	UnlockApp();
}

//  remove t shape
void CbvhTweakDlg::OnBnClickedRemoveT()
{
	LockApp();
	theApp.m_pBVHTweaker->RemoveTShape();
	// reset the timeline slider range
	int nFrames = theApp.m_pBVHFile->GetNFrames();
	m_TimeLine.SetRange(0, nFrames-1);
	UnlockApp();
}

void CbvhTweakDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: I'm not sure this even works...
	if(theApp.m_IsLocked) SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	else SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	CDialog::OnMouseMove(nFlags, point);
}

void CbvhTweakDlg::OnBnClickedSlNamingClicked()
{
	int nJoints = theApp.m_pBVHFile->GetNJoints();
	if(nJoints>50) {
		AfxMessageBox(_T("Sorry, this figure has too many joints for bvhacker to make it SL compatible."),MB_ICONINFORMATION);
		return;
	} else if (nJoints<23) {
		AfxMessageBox(_T("Sorry, this figure does not have enough joints for bvhacker to make it SL compatible."),MB_ICONINFORMATION);
		return;
	}

	LockApp();
	theApp.m_pBVHTweaker->RenameJoints();
	// set the tree ctrl
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}


void CbvhTweakDlg::OnHackAttemptslbonesizing()
{
	int nJoints = theApp.m_pBVHFile->GetNJoints();
	if(nJoints>50) {
		AfxMessageBox(_T("This figure has too many joints for SL.\nTry deleting any joints that do not have rotation data."),MB_ICONINFORMATION );
		return;
	} else if (nJoints<23) {
		AfxMessageBox(_T("Sorry, this figure does not have enough joints for bvhacker to make it SL compatible.\nHave you tried adding an abdomen joint?"),MB_ICONINFORMATION );
		return;
	}



	LockApp();
	int nJointsResized = theApp.m_pBVHTweaker->ReSizeJoints();
	if(theApp.m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS)
	{
	    theApp.m_pBVHQuaterniser->SetArmsAlignment(ZXY);
		theApp.m_pBVHQuaterniser->SetArmsRotationOrder(ZXY);
	}

	// set the tree ctrl
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnInsertJoint()
{
	LockApp();
	theApp.m_pBVHTweaker->InsertJoint();
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	m_oglWindow.ReInitialise();
	UnlockApp();
}

void CbvhTweakDlg::OnRemoveJoint()
{
	LockApp();
	theApp.m_pBVHTweaker->RemoveJoint();
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	Sleep(500);
	UnlockApp();
}

void CbvhTweakDlg::OnRenameJoint()
{
	LockApp();
	theApp.m_pBVHTweaker->RenameJoint();
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::SetCmdLine(LPTSTR lpCmdLine)
{
	m_lpCmdLine = lpCmdLine;
}

void CbvhTweakDlg::OnBnClickedAddAbdomenBtn()
{
	LockApp();
	theApp.m_pBVHTweaker->AddAbdomen();
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();	
}

void CbvhTweakDlg::OnBnClickedAbFixBtn()
{
	LockApp();
	theApp.m_pBVHTweaker->SwapAbsChestMotion();
	//m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnBnClickedRightViewButton()
{
	m_oglWindow.SetRightView();
	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
}

void CbvhTweakDlg::OnBnClickedLeftViewButton()
{
	m_oglWindow.SetLeftView();
	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
}

void CbvhTweakDlg::OnBnClickedMotionbuilderFix()
{
	LockApp();
	theApp.m_pBVHTweaker->MotionBuilderHeightFix();
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnBnClickedNoZeroBtn()
{
	LockApp();
	theApp.m_pBVHTweaker->GetRidOfZeros();
	//m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnBnClickedZeroBtn()
{
	if(StatusIs(FILE_LOADED))
	{
		LockApp();
		theApp.m_pBVHTweaker->ZeroOutChannel(N_X,  GetStatus(), m_SkeletonTree.GetCurrentSelection());
		theApp.m_pBVHTweaker->ZeroOutChannel(N_Y,  GetStatus(), m_SkeletonTree.GetCurrentSelection());
		theApp.m_pBVHTweaker->ZeroOutChannel(N_Z,  GetStatus(), m_SkeletonTree.GetCurrentSelection());
		UnlockApp();
	}
	UpdateInterface();
}


void CbvhTweakDlg::OnBnClickedCheckQuality()
{
	LockApp();
	theApp.m_pBVHTweaker->QualityCheck();
	//m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}



void CbvhTweakDlg::OnStnClickedClampLabel()
{
	// TODO: Add your control notification handler code here
}

void CbvhTweakDlg::OnBnClickedScaleUp()
{
	LockApp();
	theApp.m_pBVHTweaker->Scale(true);
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnBnClickedScaleDown()
{
	LockApp();
	theApp.m_pBVHTweaker->Scale(false);
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::DisableControls(void)
{
	// deep breath, and...
	m_DefaultViewButton.EnableWindow(FALSE);
	m_GroundViewButton.EnableWindow(FALSE);
	m_TrimButton.EnableWindow(FALSE);
	m_PlayButton.EnableWindow(FALSE);
	m_StopButton.EnableWindow(FALSE);
	m_ReverseButton.EnableWindow(FALSE);
	m_ForwardButton.EnableWindow(FALSE);
	m_CurrentJointLabel.EnableWindow(FALSE);
	m_channelOrderDisplay.EnableWindow(FALSE);
	m_heirarchyDisplay.EnableWindow(FALSE);
	m_nChannelsDisplay.EnableWindow(FALSE);
	m_parentDisplay.EnableWindow(FALSE);
	m_OffsetsDispX_E.EnableWindow(FALSE);	
	m_OffsetsDispY_E.EnableWindow(FALSE);
	m_OffsetsDispZ_E.EnableWindow(FALSE);
	m_TimeLine.EnableWindow(FALSE);
	m_FrameCountDisplay.EnableWindow(FALSE);
	m_SliderRangeAdjust.EnableWindow(FALSE);
	m_FrameTimeDisplay.EnableWindow(FALSE);
	m_ClampX.EnableWindow(FALSE);
	m_ClampY.EnableWindow(FALSE);
	m_ClampZ.EnableWindow(FALSE);
	m_TranslationRadioBtn.EnableWindow(FALSE);
	m_RotationRadioBtn.EnableWindow(FALSE);
	//m_ColourDisplayCheck.EnableWindow(FALSE);
	//m_ShowGroundCheck.EnableWindow(FALSE);
	m_TrimLeftBtn.EnableWindow(FALSE);
	m_TrimRightBtn.EnableWindow(FALSE);
	m_SliderRangeSpinner.EnableWindow(FALSE);
	m_FrameTimeSpinner.EnableWindow(FALSE);
	m_OffsetStaticLabel.EnableWindow(FALSE);
	m_ClampBtnsLabel.EnableWindow(FALSE);
	m_SliderRangeLabel.EnableWindow(FALSE);
	m_XValEditDisplay.EnableWindow(FALSE);// 
	m_YValEditDisplay.EnableWindow(FALSE);
	m_ZValEditDisplay.EnableWindow(FALSE);
	m_XSlider.EnableWindow(FALSE);
	m_YSlider.EnableWindow(FALSE);
	m_XSlider.EnableWindow(FALSE);
	m_KnitEndsButton.EnableWindow(FALSE);
	m_SetTShapeBtn.EnableWindow(FALSE);
	m_UseSLJointNamesBtn.EnableWindow(FALSE);
	m_FramerateLabelStatic.EnableWindow(FALSE);
	m_RemoveTBtn.EnableWindow(FALSE);
	m_FixLoopBtn.EnableWindow(FALSE);
	m_Right_ViewButton.EnableWindow(FALSE);
	m_LeftViewButton.EnableWindow(FALSE);
	m_MotionBuilderFixBtn.EnableWindow(FALSE);
	m_ZeroButton.EnableWindow(FALSE);
	m_FpsDisplay.EnableWindow(FALSE);
	m_Centre_Btn.EnableWindow(FALSE);
	m_Scale_Display.EnableWindow(FALSE);
	m_AdjustmentInfoLabel.EnableWindow(FALSE);
	m_ZoomOutButton.EnableWindow(FALSE);
	m_ZoomInButton.EnableWindow(FALSE);

	// disable menu items
	GetMenu()->EnableMenuItem(ID_FILE_SAVE32778, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// file save 
	GetMenu()->EnableMenuItem(ID_FILE_REPORT, MF_BYCOMMAND | MF_DISABLED  | MF_GRAYED);// report 
	GetMenu()->EnableMenuItem(ID_HACKS_RESAMPLE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// resample 
	GetMenu()->EnableMenuItem(ID_INSERT_JOINT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// insert joint 
	GetMenu()->EnableMenuItem(ID_REMOVE_JOINT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// remove joint
	GetMenu()->EnableMenuItem(ID_RENAME_JOINT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// rename joint
	GetMenu()->EnableMenuItem(ID_EDIT_SCALEUP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// scale up 
	GetMenu()->EnableMenuItem(ID_EDIT_SCALEDOWN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// scale down
	GetMenu()->EnableMenuItem(ID_VIEW_FRONT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// front view
	GetMenu()->EnableMenuItem(ID_VIEW_RIGHTVIEW, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// right view
	GetMenu()->EnableMenuItem(ID_VIEW_LEFTVIEW, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// floor view 
	GetMenu()->EnableMenuItem(ID_VIEW_FLOORLEVELVIEW, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// left view 
	GetMenu()->EnableMenuItem(ID_VIEW_COLOURDISPLAY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// colour display check 
	GetMenu()->EnableMenuItem(ID_HACK_ROUNDSHOULDERS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// round shoulders menu item ID_HACK_ROUNDSHOULDERS
	GetMenu()->EnableMenuItem(ID_HACK_CRANENECKSHIFT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// crane neck menu item 
	GetMenu()->EnableMenuItem(ID_HACK_STRAIGHTENNECKSHIFT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// straighten neck menu item  
	GetMenu()->EnableMenuItem(ID_HACK_REVERSETIMELINE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// reverse entire animation
	GetMenu()->EnableMenuItem(ID_VIEW_SHADING, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// shading
	GetMenu()->EnableMenuItem(ID_VIEW_SHOWGROUND, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// show ground check
	GetMenu()->EnableMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// display first frame check 
	GetMenu()->EnableMenuItem(ID_LOOPING_MARKIN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// loop point in  
	GetMenu()->EnableMenuItem(ID_LOOPING_MARKOUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// loop point out
	GetMenu()->EnableMenuItem(ID_LOOPING_CROP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// crop  
	GetMenu()->EnableMenuItem(ID_LOOPING_KNITENDS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// knit ends 
	GetMenu()->EnableMenuItem(ID_HACKS_SETT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// Set t-stance 
	GetMenu()->EnableMenuItem(ID_HACKS_REMOVETSTANCE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//remove t-stance 
	GetMenu()->EnableMenuItem(ID_PREPARE_, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// !zero
	GetMenu()->EnableMenuItem(ID_HACKS_REMOVEHIPSOFFSET, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// remove hips offset 
	GetMenu()->EnableMenuItem(ID_HACKS_SLIFY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// SLify 
	GetMenu()->EnableMenuItem(ID_HACKS_RENAMEJOINTS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// rename joints 
	GetMenu()->EnableMenuItem(ID_EDIT_SWAPCHEST, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// swap chest abdomen 
	GetMenu()->EnableMenuItem(ID_HACK_INTERPOLATECHEST, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// interpolate chest abdomen 
	GetMenu()->EnableMenuItem(ID_HACKS_REMOVEZEROSFROMFIRSTFRAME, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// remove zeros from 1st frame
	GetMenu()->EnableMenuItem(ID_HACKS_ADDABDOMEN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// add abdomen 
	GetMenu()->EnableMenuItem(ID_HACK_SQUARESHOULDERS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// square shoulders
	GetMenu()->EnableMenuItem(ID_HACK_SLVWR, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// SL loop fix
	GetMenu()->EnableMenuItem(ID_HACK_SETBLENDPOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// SL loop fix ID_HACK_CENTRE
	GetMenu()->EnableMenuItem(ID_HACK_CENTRE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// centre
	GetMenu()->EnableMenuItem(ID_HACK_ATTEMPTSLBONESIZING, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// SL bone sizing
	GetMenu()->EnableMenuItem(ID_EDIT_UNDO32823, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// undo ID_EDIT_TRANSLATIONMODE 
	GetMenu()->EnableMenuItem(ID_EDIT_TRANSLATIONMODE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// translation mode   ID_EDIT_ROTATIONMODE
	GetMenu()->EnableMenuItem(ID_EDIT_ROTATIONMODE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// rotation mode ID_HACK_ATTEMPTANIMEEPLENAMING 
	GetMenu()->EnableMenuItem(ID_HACK_ATTEMPTANIMEEPLENAMING, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// animeeple naming  
	GetMenu()->EnableMenuItem(ID_SPINE_COMBINECHEST, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);// combine chest and abdomen
	GetMenu()->EnableMenuItem(ID_VIEW_ZOOMIN, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_VIEW_ZOOMIN
	GetMenu()->EnableMenuItem(ID_VIEW_ZOOMOUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_VIEW_ZOOMOUT

	GetMenu()->EnableMenuItem(ID_HIPS_TRANSLATION, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_HIPS_TRANSLATION
	GetMenu()->EnableMenuItem(ID_HIPS_ROTATION, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_HIPS_ROTATION
	GetMenu()->EnableMenuItem(ID_HIPS_BOTH, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_HIPS_BOTH
	GetMenu()->EnableMenuItem(ID_UPPERBODY_ALLABOVEHIPS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_UPPERBODY_ALLABOVEHIPS
	GetMenu()->EnableMenuItem(ID_UPPERBODY_ABDOMENANDUP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_UPPERBODY_ABDOMENANDUP
	GetMenu()->EnableMenuItem(ID_UPPERBODY_CHESTANDUP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_UPPERBODY_CHESTANDUP
	GetMenu()->EnableMenuItem(ID_LOWERBODY_HIPS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_LOWERBODY_HIPS
	GetMenu()->EnableMenuItem(ID_LOWERBODY_HIPS32849, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_LOWERBODY_HIPS32849
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_RIGHTARM, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_ALLEXCEPT_RIGHTARM
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_LEFTARM, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_ALLEXCEPT_LEFTARM
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_RIGHTARMANDHEAD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_ALLEXCEPT_RIGHTARMANDHEAD 
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_LEFTARMANDHEAD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_ALLEXCEPT_LEFTARMANDHEAD
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_RIGHTARM32859, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_ALLEXCEPT_RIGHTARM32859
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_LEFTARM32860, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);//ID_ALLEXCEPT_LEFTARM32860
}

void CbvhTweakDlg::EnableSliderControls(void)
{
	//m_OffsetsDispX_E.EnableWindow(TRUE);	
	//m_OffsetsDispY_E.EnableWindow(TRUE);
	//m_OffsetsDispZ_E.EnableWindow(TRUE);
	m_SliderRangeAdjust.EnableWindow(TRUE);
	m_SliderRangeSpinner.EnableWindow(TRUE);
	m_XValEditDisplay.EnableWindow(TRUE);// 
	m_YValEditDisplay.EnableWindow(TRUE);
	m_ZValEditDisplay.EnableWindow(TRUE);
	m_XSlider.EnableWindow(TRUE);
	m_YSlider.EnableWindow(TRUE);
	m_XSlider.EnableWindow(TRUE);
	m_ClampX.EnableWindow(TRUE);
	m_ClampY.EnableWindow(TRUE);
	m_ClampZ.EnableWindow(TRUE);
	m_ZeroButton.EnableWindow(TRUE);
}

void CbvhTweakDlg::DisableSliderControls(void)
{
	//m_OffsetsDispX_E.EnableWindow(FALSE);	
	//m_OffsetsDispY_E.EnableWindow(FALSE);
	//m_OffsetsDispZ_E.EnableWindow(FALSE);
	m_SliderRangeAdjust.EnableWindow(FALSE);
	m_SliderRangeSpinner.EnableWindow(FALSE);
	m_XValEditDisplay.EnableWindow(FALSE);// 
	m_YValEditDisplay.EnableWindow(FALSE);
	m_ZValEditDisplay.EnableWindow(FALSE);
	m_XSlider.EnableWindow(FALSE);
	m_YSlider.EnableWindow(FALSE);
	m_XSlider.EnableWindow(FALSE);
	m_ClampX.EnableWindow(FALSE);
	m_ClampY.EnableWindow(FALSE);
	m_ClampZ.EnableWindow(FALSE);
	m_ZeroButton.EnableWindow(FALSE);
}
void CbvhTweakDlg::EnableControls(void)
{
	// deep breath, and...
	m_DefaultViewButton.EnableWindow(TRUE);
	m_GroundViewButton.EnableWindow(TRUE);
	m_TrimButton.EnableWindow(TRUE);
	m_PlayButton.EnableWindow(TRUE);
	m_StopButton.EnableWindow(TRUE);
	m_ReverseButton.EnableWindow(TRUE);
	m_ForwardButton.EnableWindow(TRUE);
	m_CurrentJointLabel.EnableWindow(TRUE);
	m_channelOrderDisplay.EnableWindow(TRUE);
	m_heirarchyDisplay.EnableWindow(TRUE);
	m_nChannelsDisplay.EnableWindow(TRUE);
	m_parentDisplay.EnableWindow(TRUE);
	m_OffsetsDispX_E.EnableWindow(TRUE);	
	m_OffsetsDispY_E.EnableWindow(TRUE);
	m_OffsetsDispZ_E.EnableWindow(TRUE);
	m_TimeLine.EnableWindow(TRUE);
	m_FrameCountDisplay.EnableWindow(TRUE);
	m_SliderRangeAdjust.EnableWindow(TRUE);
	m_FrameTimeDisplay.EnableWindow(TRUE);
	m_ClampX.EnableWindow(TRUE);
	m_ClampY.EnableWindow(TRUE);
	m_ClampZ.EnableWindow(TRUE);
	m_TranslationRadioBtn.EnableWindow(TRUE);
	m_RotationRadioBtn.EnableWindow(TRUE);
	//m_Disp1stFrameBtn.EnableWindow(TRUE);
	//m_ColourDisplayCheck.EnableWindow(TRUE);
	//m_ShowGroundCheck.EnableWindow(TRUE);
	m_TrimLeftBtn.EnableWindow(TRUE);
	m_TrimRightBtn.EnableWindow(TRUE);
	m_SliderRangeSpinner.EnableWindow(TRUE);
	m_FrameTimeSpinner.EnableWindow(TRUE);
	m_OffsetStaticLabel.EnableWindow(TRUE);
	m_ClampBtnsLabel.EnableWindow(TRUE);
	m_SliderRangeLabel.EnableWindow(TRUE);
	m_XValEditDisplay.EnableWindow(TRUE);// 
	m_YValEditDisplay.EnableWindow(TRUE);
	m_ZValEditDisplay.EnableWindow(TRUE);
	m_XSlider.EnableWindow(TRUE);
	m_YSlider.EnableWindow(TRUE);
	m_XSlider.EnableWindow(TRUE);
	m_KnitEndsButton.EnableWindow(TRUE);
	m_SetTShapeBtn.EnableWindow(TRUE);
	m_UseSLJointNamesBtn.EnableWindow(TRUE);//is now the resample button - what a mess!
	m_FramerateLabelStatic.EnableWindow(TRUE);;
	m_RemoveTBtn.EnableWindow(TRUE);
	m_FixLoopBtn.EnableWindow(TRUE);
	m_Right_ViewButton.EnableWindow(TRUE);
	m_LeftViewButton.EnableWindow(TRUE);
	m_MotionBuilderFixBtn.EnableWindow(TRUE);
	m_ZeroButton.EnableWindow(TRUE);
	m_FpsDisplay.EnableWindow(TRUE);
	m_Transport_controls_group_box.EnableWindow(TRUE);
	m_Centre_Btn.EnableWindow(TRUE);
	m_Scale_Display.EnableWindow(TRUE);
	m_AdjustmentInfoLabel.EnableWindow(TRUE);
	m_ZoomOutButton.EnableWindow(TRUE);
	m_ZoomInButton.EnableWindow(TRUE);


	// now enable menu items
	GetMenu()->EnableMenuItem(ID_FILE_SAVE32778, MF_BYCOMMAND | MF_ENABLED);// file save 
	GetMenu()->EnableMenuItem(ID_FILE_REPORT, MF_BYCOMMAND | MF_ENABLED);// report 
	GetMenu()->EnableMenuItem(ID_HACKS_RESAMPLE, MF_BYCOMMAND | MF_ENABLED);// resample ///           
	GetMenu()->EnableMenuItem(ID_INSERT_JOINT, MF_BYCOMMAND | MF_ENABLED);// insert joint 
	GetMenu()->EnableMenuItem(ID_REMOVE_JOINT, MF_BYCOMMAND | MF_ENABLED);// remove joint
	GetMenu()->EnableMenuItem(ID_RENAME_JOINT, MF_BYCOMMAND | MF_ENABLED);// rename joint
	GetMenu()->EnableMenuItem(ID_EDIT_SCALEUP, MF_BYCOMMAND | MF_ENABLED);// scale up 
	GetMenu()->EnableMenuItem(ID_EDIT_SCALEDOWN, MF_BYCOMMAND | MF_ENABLED);// scale down
	GetMenu()->EnableMenuItem(ID_VIEW_FRONT, MF_BYCOMMAND | MF_ENABLED);// front view
	GetMenu()->EnableMenuItem(ID_VIEW_RIGHTVIEW, MF_BYCOMMAND | MF_ENABLED);// right view
	GetMenu()->EnableMenuItem(ID_VIEW_LEFTVIEW, MF_BYCOMMAND | MF_ENABLED);// floor view 
	GetMenu()->EnableMenuItem(ID_VIEW_FLOORLEVELVIEW, MF_BYCOMMAND | MF_ENABLED);// left view 
	GetMenu()->EnableMenuItem(ID_VIEW_COLOURDISPLAY, MF_BYCOMMAND | MF_ENABLED);// colour display check 
	GetMenu()->EnableMenuItem(ID_HACK_ROUNDSHOULDERS, MF_BYCOMMAND | MF_ENABLED);// round shoulders menu item
	GetMenu()->EnableMenuItem(ID_HACK_CRANENECKSHIFT, MF_BYCOMMAND | MF_ENABLED);// crane neck menu item 
	GetMenu()->EnableMenuItem(ID_HACK_STRAIGHTENNECKSHIFT, MF_BYCOMMAND | MF_ENABLED);// straighten neck menu item  
	GetMenu()->EnableMenuItem(ID_HACK_REVERSETIMELINE, MF_BYCOMMAND | MF_ENABLED);// reverse entire animation menu item
	GetMenu()->EnableMenuItem(ID_VIEW_SHOWGROUND, MF_BYCOMMAND | MF_ENABLED);// show ground check 
	GetMenu()->EnableMenuItem(ID_VIEW_SHADING, MF_BYCOMMAND | MF_ENABLED);// show OGL shading
	GetMenu()->EnableMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_BYCOMMAND | MF_ENABLED);// display first frame check 
	GetMenu()->EnableMenuItem(ID_LOOPING_MARKIN, MF_BYCOMMAND | MF_ENABLED);// loop point in  
	GetMenu()->EnableMenuItem(ID_LOOPING_MARKOUT, MF_BYCOMMAND | MF_ENABLED);// loop point out
	GetMenu()->EnableMenuItem(ID_LOOPING_CROP, MF_BYCOMMAND | MF_ENABLED);// crop  
	GetMenu()->EnableMenuItem(ID_LOOPING_KNITENDS, MF_BYCOMMAND | MF_ENABLED);// knit ends 
	GetMenu()->EnableMenuItem(ID_HACKS_SETT, MF_BYCOMMAND | MF_ENABLED);// Set t-stance 
	GetMenu()->EnableMenuItem(ID_HACKS_REMOVETSTANCE, MF_BYCOMMAND | MF_ENABLED);//remove t-stance 
	GetMenu()->EnableMenuItem(ID_PREPARE_, MF_BYCOMMAND | MF_ENABLED);// !zero
	GetMenu()->EnableMenuItem(ID_HACKS_REMOVEHIPSOFFSET, MF_BYCOMMAND | MF_ENABLED);// remove hips offset 
	GetMenu()->EnableMenuItem(ID_HACKS_SLIFY, MF_BYCOMMAND | MF_ENABLED);// SLify 
	GetMenu()->EnableMenuItem(ID_HACKS_RENAMEJOINTS, MF_BYCOMMAND | MF_ENABLED);// rename joints 
	GetMenu()->EnableMenuItem(ID_EDIT_SWAPCHEST, MF_BYCOMMAND | MF_ENABLED);// swap chest abdomen 
	GetMenu()->EnableMenuItem(ID_HACK_INTERPOLATECHEST, MF_BYCOMMAND | MF_ENABLED);// interpolate chest abdomen 
	GetMenu()->EnableMenuItem(ID_HACKS_REMOVEZEROSFROMFIRSTFRAME, MF_BYCOMMAND | MF_ENABLED);// remove zeros from 1st frame
	GetMenu()->EnableMenuItem(ID_HACKS_ADDABDOMEN, MF_BYCOMMAND | MF_ENABLED);// add abdomen 
	GetMenu()->EnableMenuItem(ID_HACK_SQUARESHOULDERS, MF_BYCOMMAND | MF_ENABLED);// fix sloped shoulders
	GetMenu()->EnableMenuItem(ID_HACK_SLVWR, MF_BYCOMMAND | MF_ENABLED);// SL loop fix
	GetMenu()->EnableMenuItem(ID_HACK_SETBLENDPOSE, MF_BYCOMMAND | MF_ENABLED);// SL loop fix ID_HACK_CENTRE
	GetMenu()->EnableMenuItem(ID_HACK_CENTRE, MF_BYCOMMAND | MF_ENABLED);// Centre 
	GetMenu()->EnableMenuItem(ID_HACK_ATTEMPTSLBONESIZING, MF_BYCOMMAND | MF_ENABLED);// SL bone sizing 
	GetMenu()->EnableMenuItem(ID_EDIT_UNDO32823, MF_BYCOMMAND | MF_ENABLED);// undo  
	GetMenu()->EnableMenuItem(ID_EDIT_TRANSLATIONMODE, MF_BYCOMMAND | MF_ENABLED);// translation mode ID_EDIT_ROTATIONMODE
	GetMenu()->EnableMenuItem(ID_EDIT_ROTATIONMODE, MF_BYCOMMAND | MF_ENABLED);// rotation mode 
	GetMenu()->EnableMenuItem(ID_HACK_ATTEMPTANIMEEPLENAMING, MF_BYCOMMAND | MF_ENABLED);// animeeple naming
	GetMenu()->EnableMenuItem(ID_SPINE_COMBINECHEST, MF_BYCOMMAND | MF_ENABLED);// combine chest and abdomen
	GetMenu()->EnableMenuItem(ID_VIEW_ZOOMIN, MF_BYCOMMAND | MF_ENABLED);//ID_VIEW_ZOOMIN
	GetMenu()->EnableMenuItem(ID_VIEW_ZOOMOUT, MF_BYCOMMAND | MF_ENABLED);//ID_VIEW_ZOOMOUT

	GetMenu()->EnableMenuItem(ID_HIPS_TRANSLATION, MF_BYCOMMAND | MF_ENABLED);//ID_HIPS_TRANSLATION
	GetMenu()->EnableMenuItem(ID_HIPS_ROTATION, MF_BYCOMMAND | MF_ENABLED);//ID_HIPS_ROTATION
	GetMenu()->EnableMenuItem(ID_HIPS_BOTH, MF_BYCOMMAND | MF_ENABLED);//ID_HIPS_BOTH
	GetMenu()->EnableMenuItem(ID_UPPERBODY_ALLABOVEHIPS, MF_BYCOMMAND | MF_ENABLED);//ID_UPPERBODY_ALLABOVEHIPS
	GetMenu()->EnableMenuItem(ID_UPPERBODY_ABDOMENANDUP, MF_BYCOMMAND | MF_ENABLED);//ID_UPPERBODY_ABDOMENANDUP
	GetMenu()->EnableMenuItem(ID_UPPERBODY_CHESTANDUP, MF_BYCOMMAND | MF_ENABLED);//ID_UPPERBODY_CHESTANDUP
	GetMenu()->EnableMenuItem(ID_LOWERBODY_HIPS, MF_BYCOMMAND | MF_ENABLED);//ID_LOWERBODY_HIPS
	GetMenu()->EnableMenuItem(ID_LOWERBODY_HIPS32849, MF_BYCOMMAND | MF_ENABLED);//ID_LOWERBODY_HIPS32849
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_RIGHTARM, MF_BYCOMMAND | MF_ENABLED);//ID_ALLEXCEPT_RIGHTARM
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_LEFTARM, MF_BYCOMMAND | MF_ENABLED);//ID_ALLEXCEPT_LEFTARM
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_RIGHTARMANDHEAD, MF_BYCOMMAND | MF_ENABLED);//ID_ALLEXCEPT_RIGHTARMANDHEAD
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_LEFTARMANDHEAD, MF_BYCOMMAND | MF_ENABLED);//ID_ALLEXCEPT_LEFTARMANDHEAD
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_RIGHTARM32859, MF_BYCOMMAND | MF_ENABLED);//ID_ALLEXCEPT_RIGHTARM32859
	GetMenu()->EnableMenuItem(ID_ALLEXCEPT_LEFTARM32860, MF_BYCOMMAND | MF_ENABLED);//ID_ALLEXCEPT_LEFTARM32860

}

// Do not close my app when the escape key is pressed!
//Disable OnCancel 
void CbvhTweakDlg::OnCancel()
{ 
} 

//Disable OnOK 
void CbvhTweakDlg::OnOK()
{ 
}

void CbvhTweakDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}

    //add the following code
    else if ((nID & 0xFFF0) == SC_CLOSE)
    {
        //if user clicked the "X"
        EndDialog(IDOK);   //Close the dialog with IDOK (or IDCANCEL)

        //---end of code you have added   
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}


void CbvhTweakDlg::OnFileOpen32777()
{
	OnBnClickedLoadBVH();
}

void CbvhTweakDlg::OnFileSave32778()
{
	OnBnClickedSaveBVH();
}

void CbvhTweakDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CbvhTweakDlg::OnFileExit32784()
{
	//OnCancel();
	EndDialog(IDOK);
}

void CbvhTweakDlg::OnHelpOnlinehelpfiles()
{
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.bvhacker.com/help.html"),_T(""),NULL,0);
}

void CbvhTweakDlg::OnHelpFinder()
{
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.bvhacker.com/donate.html"),_T(""),NULL,0);
}

void CbvhTweakDlg::OnHelpDonate()
{
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.bvhacker.com/donate.html"),_T(""),NULL,0);
}

void CbvhTweakDlg::OnHelpBvhackergooglegroup()
{
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://groups.google.com/group/bvhacker"),_T(""),NULL,0);
}
void CbvhTweakDlg::OnHelpUsing()
{
	//ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://groups.google.com/group/bvhacker"),_T(""),NULL,0);
}


void CbvhTweakDlg::OnHelpForums()
{
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://groups.google.com/group/bvhacker"),_T(""),NULL,0);
}

void CbvhTweakDlg::OnViewFront()
{
	m_oglWindow.ResetView();
}

void CbvhTweakDlg::OnViewRightview()
{
	m_oglWindow.SetRightView();
}

void CbvhTweakDlg::OnViewLeftview()
{
	m_oglWindow.SetLeftView();
}

void CbvhTweakDlg::OnViewFloorlevelview()
{
	m_oglWindow.SetGroundView();
}

void CbvhTweakDlg::OnHelpAbout()
{
	BVHAboutBox aboutBox = new BVHAboutBox();
	aboutBox.DoModal();
}

void CbvhTweakDlg::OnFileReport()
{
	OnBnClickedCheckQuality();
}


void CbvhTweakDlg::OnEditScaleup()
{
	LockApp();
	OnBnClickedScaleUp();
	UnlockApp();
}

void CbvhTweakDlg::OnEditScaledown()
{
	LockApp();
	OnBnClickedScaleDown();
	UnlockApp();
}

void CbvhTweakDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;

	if(m_oglWindow) 
	{
		CRect l_formRect; // {top=0 bottom=756 left=0 right=1020}
		CRect l_windowRect; //
		m_openGLWindow.GetClientRect(&l_formRect); 
		m_openGLWindow.GetWindowRect(&l_windowRect);

		m_oglWindow.PromptOnSize(nType, l_formRect, l_windowRect);
	}
}

void CbvhTweakDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(1028,810,fwSide,pRect);
}

void CbvhTweakDlg::OnBnClickedResampleBtn()
{
	// stop playing
	OnBnClickedStopBtn();

	LockApp();

	//m_status+=EDITING_PAUSE;

	theApp.m_pBVHTweaker->HalfSample();

	// adjust the timeline to display the new number of frames
	int nFrames = theApp.m_pBVHFile->GetNFrames();
	m_TimeLine.SetRange(0, nFrames-1);
	m_TimeLine.SetPos(0);

	// set the frame duration (mS)
	float fFrameTime = theApp.m_pBVHFile->GetFrameTime();
	if(StatusIs(FILE_PLAYING)) {// reset the frame timer
		KillTimer(3);
		SetTimer(3, (int)fFrameTime, 0);
	}
	TCHAR wsFrameDisplay[8];
	sprintf(wsFrameDisplay,_T("%0.*f"),1,fFrameTime);
	m_FrameTimeDisplay.SetWindowTextA(wsFrameDisplay);

	// initialise the trim points
	m_nTrimPointLeft = 0;
	m_nTrimPointRight = nFrames-1;

	//EnableControls();
	//m_status-=EDITING_PAUSE;
	UnlockApp();
}

void CbvhTweakDlg::OnHacksResample()
{
	OnBnClickedResampleBtn();
}

void CbvhTweakDlg::OnLoopingMarkin()
{
	OnBnClickedTrimPtLeft();
}

void CbvhTweakDlg::OnLoopingCrop()
{
	OnBnClickedTrimBtn();
	UpdateInterface();
}

void CbvhTweakDlg::OnLoopingKnitends()
{
	OnBnClickedKnitEnds();
}

void CbvhTweakDlg::OnLoopingMarkout()
{
	OnBnClickedTrimPtR();
}

void CbvhTweakDlg::OnHacksSett()
{
	OnBnClickedSetT();
}

void CbvhTweakDlg::OnHacksRemovetstance()
{
	OnBnClickedRemoveT();
}

void CbvhTweakDlg::OnHacksSlify()
{
	int nJoints = theApp.m_pBVHFile->GetNJoints();
	if(nJoints>24) {
		AfxMessageBox(_T("Sorry, this skeleton has too many joints for bvhacker to make it SL compatible. Please remove some joints and try again. You need 23 joints including End Sites."),MB_ICONINFORMATION);
		return;
	} else if (nJoints<23) {
		AfxMessageBox(_T("Sorry, this skeleton does not have enough joints for bvhacker to make it SL compatible. Please add some joints and try again. You need 23 joints including End Sites."),MB_ICONINFORMATION);
		return;
	}

	LockApp();
	theApp.m_pBVHTweaker->SLify();

	// set the tree ctrl
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);

	UnlockApp();
}

void CbvhTweakDlg::OnHacksRenamejoints()
{
	OnBnClickedSlNamingClicked();
}

void CbvhTweakDlg::OnHacksRemovehipsoffset()
{
	OnBnClickedMotionbuilderFix();
}

void CbvhTweakDlg::OnHacksRemovezerosfromfirstframe()
{
	OnBnClickedNoZeroBtn();
}

void CbvhTweakDlg::OnHacksAddabdomen()
{
	OnBnClickedAddAbdomenBtn();
}

void CbvhTweakDlg::OnEditSwapchest()
{
	OnBnClickedAbFixBtn();
}

void CbvhTweakDlg::OnHackInterpolatechest()
{
	LockApp();
	theApp.m_pBVHTweaker->InterpolateChestAbdomen();
	UnlockApp();
}

void CbvhTweakDlg::OnEnChangeSliderRange()
{
	//CString wsSliderRange;

	//m_SliderRangeAdjust.GetWindowTextA(wsSliderRange);

	//int stop = TRUE;
}

// still work in progress - I only just scraped a pass in DSP at uni!
void CbvhTweakDlg::OnHackLowpassfilter()
{
	LockApp();
	theApp.m_pBVHTweaker->LPFilter();
	UnlockApp();
}

void CbvhTweakDlg::OnHackSLLoopFix()
{
	LockApp();
	theApp.m_pBVHTweaker->FixSLLoopBug();
	UnlockApp();
}

void CbvhTweakDlg::OnBnClickedFixLoop()
{
	OnHackSLLoopFix();
}

void CbvhTweakDlg::OnHackCentre()
{
	OnBnClickedCentreButton();
}

void CbvhTweakDlg::OnHackSetblendpose()
{
	LockApp();
	theApp.m_pBVHTweaker->SetBlendPose();
	UnlockApp();
}
void CbvhTweakDlg::OnBnClickedCentreButton()
{
	// for IDC_CENTRE_BUTTON 
	LockApp();
	theApp.m_pBVHTweaker->CentreFigure();
	UnlockApp();
}

void CbvhTweakDlg::OnEnKillfocusFramerateEdit()
{
	LockApp();

	float nFrameTime = 0.f;
	char str_float[10];
	m_FrameTimeDisplay.GetWindowText(str_float,10);
	sscanf(str_float, "%f", &nFrameTime);

	if(nFrameTime==0){
		// user entered bad data - reset the display
		float nFrameTime = theApp.m_pBVHFile->GetFrameTime();
		TCHAR wsFrameDisplay[16];
		sprintf(wsFrameDisplay,_T("%0.*f"),4,nFrameTime);
		m_FrameTimeDisplay.SetWindowTextA(wsFrameDisplay);
		UnlockApp();
		return;
	}


	if(nFrameTime>5000) nFrameTime = 5000;
	if(nFrameTime<5) nFrameTime = 5;
	m_FrameTimeSpinner.SetPos((int)nFrameTime);
	theApp.m_pBVHFile->SetFrameTime(nFrameTime);
	if(StatusIs(FILE_PLAYING)) {// reset the frame timer
		KillTimer(3);
		SetTimer(3, (UINT)nFrameTime, 0);
	}
	TCHAR wsFrameDisplay[16];
	sprintf(wsFrameDisplay,_T("%0.*f"),4,nFrameTime);
	m_FrameTimeDisplay.SetWindowTextA(wsFrameDisplay);

	UnlockApp();
}

void CbvhTweakDlg::OnEnKillfocusSliderRange()
{
	LockApp();

	float fSliderRange = 0.f;
	char str_float[10];
	m_SliderRangeAdjust.GetWindowText(str_float,10);
	sscanf(str_float, "%f", &fSliderRange);
	fSliderRange*=20;

	if(fSliderRange==0){
		// user entered bad data - reset the display
		DWORD fSliderRange = m_pRegistrySettings->SliderRange;
		TCHAR wsFrameDisplay[16];
		sprintf(wsFrameDisplay,_T("%0.*f"),4,fSliderRange);
		m_FrameTimeDisplay.SetWindowTextA(wsFrameDisplay);
		UnlockApp();
		return;
	}

	if(fSliderRange>7200) fSliderRange = 7200;
	if(fSliderRange<20) fSliderRange = 20;
	m_pRegistrySettings->SliderRange = (DWORD)fSliderRange;

	m_XSlider.SetRange(0, m_pRegistrySettings->SliderRange);
	m_YSlider.SetRange(0, m_pRegistrySettings->SliderRange);
	m_ZSlider.SetRange(0, m_pRegistrySettings->SliderRange);
	m_SliderRangeSpinner.SetPos(m_pRegistrySettings->SliderRange/20);
	
	TCHAR wsSliderRangeDisplay[32];
	wsprintf(wsSliderRangeDisplay, _T("%d"), m_pRegistrySettings->SliderRange/20);
	m_SliderRangeAdjust.SetWindowTextA(wsSliderRangeDisplay);

	UnlockApp();
}

void CbvhTweakDlg::UnlockApp(void)
{
	//EnableControls();
	theApp.m_IsLocked = false;
	UpdateInterface();
	GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
}

void CbvhTweakDlg::LockApp(void)
{
	// TODO: save model state here
	//DisableControls();
	theApp.m_pBVHFile->SaveState();
	theApp.m_IsLocked = true;
}


// all of this (and more) just to change the background colour of the cEdit boxes with the xyz values in!
HBRUSH CbvhTweakDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr;

	switch (nCtlColor) 
	{
		case CTLCOLOR_EDIT:
		//case CTLCOLOR_MSGBOX:
		{
			switch (pWnd->GetDlgCtrlID())
			{
				// first CEdit control ID
				case IDC_X_EDIT:
				case IDC_Y_EDIT:
				case IDC_Z_EDIT:
				case IDC_OFFSETS_DISPX_E:
				case IDC_OFFSETS_DISPY_E:
				case IDC_OFFSETS_DISPZ_E:
				case IDC_BVH_SKELETON:
				case IDC_SLIDER_RANGE:
				case IDC_FRAMERATE_EDIT:
				{
					pDC->SetBkColor(m_greycolor);    // change the text background
					hbr = (HBRUSH) m_greybrush;    // apply colour to the ctrl background
					break;  
				}
				default:
				{
					hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
					break;
				}
			}
			break;
		}
		default:
		{
			hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
			break;
		}
	}
	return hbr;
}

void CbvhTweakDlg::OnTvnSelchangedBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if(!theApp.m_IsLocked)
	{
		UpdateInterface();
	}
	*pResult = 0;
}

void CbvhTweakDlg::OnEnKillfocusXEdit()
{
	// This function gets called twice every time. 
	// This nasty little boolean prevents the operation being performed twice
	m_bAvoidingDoubleEntry=!m_bAvoidingDoubleEntry;

	if(!m_bAvoidingDoubleEntry)
	{
		LockApp();
		
		// Get details of the currently selected joint
		CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(CurrentlySelectedJoint);

		// read the value from the updated 
		float fNewValue = 0.f;
		char str_floatNew[10];
		m_XValEditDisplay.GetWindowText(str_floatNew,10);
		sscanf(str_floatNew, "%f", &fNewValue);

		// get the original value
		GLfloat X = 0;
		GLfloat Y = 0;
		GLfloat Z = 0;

		if(StatusIs(MODE_TRANSLATING))
		{
			if(pJoint->offsetChannelOrder!=NONE)
				theApp.m_pBVHTweaker->GetCurrentSelTranslations(&X,&Y,&Z, CurrentlySelectedJoint);
			else
				theApp.m_pBVHTweaker->GetCurrentSelOffsets(&X,&Y,&Z, CurrentlySelectedJoint);
		}

		if(StatusIs(MODE_ROTATING))
			theApp.m_pBVHTweaker->GetCurrentSelRotations(&X,&Y,&Z, CurrentlySelectedJoint);
	
		float fOldValue = X;
		float fDifference = 0.f;
		
		if(fNewValue>fOldValue)
		{
			fDifference = fNewValue-fOldValue;
		}
		if(fNewValue<fOldValue)
		{
			fDifference = (fOldValue-fNewValue)*-1;
		}

		// bit of realistic bounds checking never does any harm...
		//if(fDifference>10000) fDifference = 10000;
		//if(fDifference<-10000) fDifference = -10000;

		if(StatusIs(MODE_ROTATING)) 
			theApp.m_pBVHTweaker->AddMotion(fDifference, 0, 0, MODE_ROTATING, CurrentlySelectedJoint);
		
		if(StatusIs(MODE_TRANSLATING)) 
		{
			if(pJoint->offsetChannelOrder!=NONE)
				theApp.m_pBVHTweaker->AddMotion(fDifference, 0, 0, MODE_TRANSLATING, CurrentlySelectedJoint);
			else
				theApp.m_pBVHTweaker->SetOffsetChannels(pJoint, fNewValue, pJoint->offsetChannels[1], pJoint->offsetChannels[0]);
		}
		

		// update the value in the edit box
		TCHAR wsX[16]; 
		//TCHAR wsY[16]; 
		//TCHAR wsZ[16]; 

		int nPrecision = 2;
		sprintf(wsX,_T("%0.*f"),nPrecision,fDifference);
		//sprintf(wsY,_T("%0.*f"),nPrecision,fDifference);
		//sprintf(wsZ,_T("%0.*f"),nPrecision,fDifference);		

		m_XValEditDisplay.SetWindowTextA(wsX);
		//m_YValEditDisplay.SetWindowTextA(wsY);
		//m_ZValEditDisplay.SetWindowTextA(wsZ);

		// clean up
		CurrentlySelectedJoint = "";
		pJoint = NULL;

		UnlockApp();
	}
}


void CbvhTweakDlg::OnEnKillfocusYEdit()
{
	// This function gets called twice every time. 
	// This nasty little boolean prevents the operation being performed twice
	m_bAvoidingDoubleEntry=!m_bAvoidingDoubleEntry;

	if(!m_bAvoidingDoubleEntry)
	{
		LockApp();
		
		// Get details of the currently selected joint
		CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();
		//BVHMotion* pFrame = theApp.m_pBVHFile->GetCurrentMotionFrame();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(CurrentlySelectedJoint);

		// read the value from the updated 
		float fNewValue = 0.f;
		char str_floatNew[10];
		m_YValEditDisplay.GetWindowText(str_floatNew,10);
		sscanf(str_floatNew, "%f", &fNewValue);

		// get the original value
		GLfloat X = 0;
		GLfloat Y = 0;
		GLfloat Z = 0;

		if(StatusIs(MODE_TRANSLATING))
		{
			if(pJoint->offsetChannelOrder!=NONE)
				theApp.m_pBVHTweaker->GetCurrentSelTranslations(&X,&Y,&Z, CurrentlySelectedJoint);
			else
				theApp.m_pBVHTweaker->GetCurrentSelOffsets(&X,&Y,&Z, CurrentlySelectedJoint);
		}

		if(StatusIs(MODE_ROTATING))
			theApp.m_pBVHTweaker->GetCurrentSelRotations(&X,&Y,&Z, CurrentlySelectedJoint);

		float fOldValue = Y;
		float fDifference = 0.f;
		
		if(fNewValue>fOldValue)
		{
			fDifference = fNewValue-fOldValue;
		}
		if(fNewValue<fOldValue)
		{
			fDifference = (fOldValue-fNewValue)*-1;
		}

		// bit of realistic bounds checking never does any harm...
		//if(fDifference>10000) fDifference = 10000;
		//if(fDifference<-10000) fDifference = -10000;

		if(StatusIs(MODE_ROTATING)) 
		{
			theApp.m_pBVHTweaker->AddMotion(0, fDifference, 0, MODE_ROTATING, CurrentlySelectedJoint);
		}
		if(StatusIs(MODE_TRANSLATING)) 
		{
			if(pJoint->offsetChannelOrder!=NONE)
				theApp.m_pBVHTweaker->AddMotion(0, fDifference, 0, MODE_TRANSLATING, CurrentlySelectedJoint);
			else
				theApp.m_pBVHTweaker->SetOffsetChannels(pJoint,pJoint->offsetChannels[2], fNewValue, pJoint->offsetChannels[0]);
		}

		// update the value in the edit box
		//TCHAR wsX[16]; 
		TCHAR wsY[16]; 
		//TCHAR wsZ[16]; 

		int nPrecision = 2;
		//sprintf(wsX,_T("%0.*f"),nPrecision,fDifference);
		sprintf(wsY,_T("%0.*f"),nPrecision,fDifference);
		//sprintf(wsZ,_T("%0.*f"),nPrecision,fDifference);		

		//m_XValEditDisplay.SetWindowTextA(wsX);
		m_YValEditDisplay.SetWindowTextA(wsY);
		//m_ZValEditDisplay.SetWindowTextA(wsZ);

		// clean up
		CurrentlySelectedJoint = "";
		pJoint = NULL;

		UnlockApp();
	}
}

void CbvhTweakDlg::OnEnKillfocusZEdit()
{
	// This function gets called twice every time. 
	// This nasty little boolean prevents the operation being performed twice
	m_bAvoidingDoubleEntry=!m_bAvoidingDoubleEntry;

	if(!m_bAvoidingDoubleEntry)
	{
		LockApp();
		
		// Get details of the currently selected joint
		CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();
		//BVHMotion* pFrame = theApp.m_pBVHFile->GetCurrentMotionFrame();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(CurrentlySelectedJoint);

		// read the value from the updated 
		float fNewValue = 0.f;
		char str_floatNew[10];
		m_ZValEditDisplay.GetWindowText(str_floatNew,10);
		sscanf(str_floatNew, "%f", &fNewValue);

		// get the original value
		GLfloat X = 0;
		GLfloat Y = 0;
		GLfloat Z = 0;

		if(StatusIs(MODE_TRANSLATING))
		{
			if(pJoint->offsetChannelOrder!=NONE)
				theApp.m_pBVHTweaker->GetCurrentSelTranslations(&X,&Y,&Z, CurrentlySelectedJoint);
			else
				theApp.m_pBVHTweaker->GetCurrentSelOffsets(&X,&Y,&Z, CurrentlySelectedJoint);
		}

		if(StatusIs(MODE_ROTATING))
			theApp.m_pBVHTweaker->GetCurrentSelRotations(&X,&Y,&Z, CurrentlySelectedJoint);

		float fOldValue = Z;
		float fDifference = 0.f;
		
		if(fNewValue>fOldValue)
		{
			fDifference = fNewValue-fOldValue;
		}
		if(fNewValue<fOldValue)
		{
			fDifference = (fOldValue-fNewValue)*-1;
		}

		// bit of realistic bounds checking never does any harm...
		//if(fDifference>10000) fDifference = 10000;
		//if(fDifference<-10000) fDifference = -10000;

		if(StatusIs(MODE_ROTATING)) 
		{
			theApp.m_pBVHTweaker->AddMotion(0, 0, fDifference, MODE_ROTATING, CurrentlySelectedJoint);
		}
		if(StatusIs(MODE_TRANSLATING)) 
		{
			if(pJoint->offsetChannelOrder!=NONE)
				theApp.m_pBVHTweaker->AddMotion(0, 0, fDifference, MODE_TRANSLATING, CurrentlySelectedJoint);
			else
				theApp.m_pBVHTweaker->SetOffsetChannels(pJoint, pJoint->offsetChannels[2], pJoint->offsetChannels[1], fNewValue);
		}

		// update the value in the edit box
		//TCHAR wsX[16]; 
		//TCHAR wsY[16]; 
		TCHAR wsZ[16]; 

		int nPrecision = 2;
		//sprintf(wsX,_T("%0.*f"),nPrecision,fDifference);
		//sprintf(wsY,_T("%0.*f"),nPrecision,fDifference);
		sprintf(wsZ,_T("%0.*f"),nPrecision,fDifference);		

		//m_XValEditDisplay.SetWindowTextA(wsX);
		//m_YValEditDisplay.SetWindowTextA(wsY);
		m_ZValEditDisplay.SetWindowTextA(wsZ);

		// clean up
		CurrentlySelectedJoint = "";
		pJoint = NULL;

		UnlockApp();
	}
}

void CbvhTweakDlg::OnEnKillfocusOffsetsDispxE()
{
	// This function gets called twice every time. 
	// This nasty little boolean prevents the operation being performed twice
	m_bAvoidingDoubleEntry=!m_bAvoidingDoubleEntry;

	if(!m_bAvoidingDoubleEntry)
	{
		LockApp();

		// Get details of the currently selected joint
		CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();
		//BVHMotion* pFrame = theApp.m_pBVHFile->GetCurrentMotionFrame();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(CurrentlySelectedJoint);

		// read the value from the updated 
		float fNewValue = 0.f;
		char str_floatNew[10];
		m_OffsetsDispX_E.GetWindowText(str_floatNew,10);
		str_floatNew[0] = ' ';// remove the 'X: '
		str_floatNew[1] = ' ';
		str_floatNew[2] = ' ';
		sscanf(str_floatNew, "%f", &fNewValue);

		// Set the new offswet value onto the joint, not the motion data
		theApp.m_pBVHTweaker->SetOffsetChannels(pJoint, fNewValue,pJoint->offsetChannels[1],pJoint->offsetChannels[0]);

		// update the value in the edit box
		TCHAR wsX[16]; 
		//TCHAR wsY[16]; 
		//TCHAR wsZ[16]; 

		int nPrecision = 2;
		sprintf(wsX,_T("%0.*f"),nPrecision,fNewValue);
		//sprintf(wsY,_T("%0.*f"),nPrecision,fDifference);
		//sprintf(wsZ,_T("%0.*f"),nPrecision,fDifference);		

		m_OffsetsDispX_E.SetWindowTextA(wsX);
		//m_YValEditDisplay.SetWindowTextA(wsY);
		//m_ZValEditDisplay.SetWindowTextA(wsZ);

		// clean up
		CurrentlySelectedJoint = "";
		pJoint = NULL;

		UnlockApp();
	}
}

void CbvhTweakDlg::OnEnKillfocusOffsetsDispyE()
{
	// This function gets called twice every time. 
	// This nasty little boolean prevents the operation being performed twice
	m_bAvoidingDoubleEntry=!m_bAvoidingDoubleEntry;

	if(!m_bAvoidingDoubleEntry)
	{
		LockApp();

		// Get details of the currently selected joint
		CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();
		//BVHMotion* pFrame = theApp.m_pBVHFile->GetCurrentMotionFrame();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(CurrentlySelectedJoint);

		// read the value from the updated 
		float fNewValue = 0.f;
		char str_floatNew[10];
		m_OffsetsDispY_E.GetWindowText(str_floatNew,10);
		str_floatNew[0] = ' ';// remove the 'X: '
		str_floatNew[1] = ' ';
		str_floatNew[2] = ' ';
		sscanf(str_floatNew, "%f", &fNewValue);

		// Set the new offswet value onto the joint, not the motion data
		theApp.m_pBVHTweaker->SetOffsetChannels(pJoint, pJoint->offsetChannels[2],fNewValue,pJoint->offsetChannels[0]);

		// update the value in the edit box
		//TCHAR wsX[16]; 
		TCHAR wsY[16]; 
		//TCHAR wsZ[16]; 

		int nPrecision = 2;
		//sprintf(wsX,_T("%0.*f"),nPrecision,fNewValue);
		sprintf(wsY,_T("%0.*f"),nPrecision,fNewValue);
		//sprintf(wsZ,_T("%0.*f"),nPrecision,fNewValue);		

		//m_OffsetsDispX_E.SetWindowTextA(wsX);
		m_YValEditDisplay.SetWindowTextA(wsY);
		//m_ZValEditDisplay.SetWindowTextA(wsZ);

		// clean up
		CurrentlySelectedJoint = "";
		pJoint = NULL;

		UnlockApp();	}
}

void CbvhTweakDlg::OnEnKillfocusOffsetsDispzE()
{
	// This function gets called twice every time. 
	// This nasty little boolean prevents the operation being performed twice
	m_bAvoidingDoubleEntry=!m_bAvoidingDoubleEntry;

	if(!m_bAvoidingDoubleEntry)
	{
		LockApp();

		// Get details of the currently selected joint
		CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();
		//BVHMotion* pFrame = theApp.m_pBVHFile->GetCurrentMotionFrame();
		BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(CurrentlySelectedJoint);

		// read the value from the updated 
		float fNewValue = 0.f;
		char str_floatNew[10];
		m_OffsetsDispZ_E.GetWindowText(str_floatNew,10);
		str_floatNew[0] = ' ';// remove the 'X: '
		str_floatNew[1] = ' ';
		str_floatNew[2] = ' ';
		sscanf(str_floatNew, "%f", &fNewValue);

		// Set the new offswet value onto the joint, not the motion data
		theApp.m_pBVHTweaker->SetOffsetChannels(pJoint, pJoint->offsetChannels[2],pJoint->offsetChannels[1],fNewValue);

		// update the value in the edit box
		//TCHAR wsX[16]; 
		//TCHAR wsY[16]; 
		TCHAR wsZ[16]; 

		int nPrecision = 2;
		//sprintf(wsX,_T("%0.*f"),nPrecision,fNewValue);
		//sprintf(wsY,_T("%0.*f"),nPrecision,fNewValue);
		sprintf(wsZ,_T("%0.*f"),nPrecision,fNewValue);		

		//m_OffsetsDispX_E.SetWindowTextA(wsX);
		//m_YValEditDisplay.SetWindowTextA(wsY);
		m_ZValEditDisplay.SetWindowTextA(wsZ);

		// clean up
		CurrentlySelectedJoint = "";
		pJoint = NULL;

		UnlockApp();
	}
}



void CbvhTweakDlg::OnEditUndo()
{
	// save the currently selected joint
	CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();

	theApp.m_IsLocked = true;
	bool bWasPlaying = false;
	if(StatusIs(FILE_PLAYING)) {
		m_status-=FILE_PLAYING;
		m_PlayButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
								MAKEINTRESOURCE(IDI_PLAY_ICON), 
								IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
		KillTimer(3);
		bWasPlaying = true;
	}
	m_status+=EDITING_PAUSE;
	theApp.m_pBVHFile->SwitchState();
	m_status-=EDITING_PAUSE;
	if(bWasPlaying)
	{
		m_status+=FILE_PLAYING;
		m_PlayButton.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), 
								MAKEINTRESOURCE(IDI_PAUSE_ICON), 
								IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR)); 
		SetTimer(3, (UINT)theApp.m_pBVHFile->GetFrameTime(), 0);
	}

	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UpdateInterface();
	m_SkeletonTree.SetCurrentSelection(CurrentlySelectedJoint);
	theApp.m_IsLocked = false;
}

void CbvhTweakDlg::OnEditTranslationmode()
{
	OnBnClickedTranslationSel(); // translation mode (T)
	CButton* RotBtn = (CButton*)GetDlgItem(IDC_ROT_SEL);
	CButton* TraBtn = (CButton*)GetDlgItem(IDC_OFF_SEL);
	RotBtn->SetCheck(false);
	TraBtn->SetCheck(true);
	RotBtn = NULL;
	TraBtn = NULL;
}

void CbvhTweakDlg::OnEditRotationmode()
{
	OnBnClickedRotSel(); // rotation mode (R)
	CButton* RotBtn = (CButton*)GetDlgItem(IDC_ROT_SEL);
	CButton* TraBtn = (CButton*)GetDlgItem(IDC_OFF_SEL);
	RotBtn->SetCheck(true);
	TraBtn->SetCheck(false);
	RotBtn = NULL;
	TraBtn = NULL;
}


void CbvhTweakDlg::OnHackSquareshoulders()
{
	LockApp();
	theApp.m_pBVHTweaker->SquareShoulders(true);
	UnlockApp();
}



void CbvhTweakDlg::OnHackRoundshoulders()
{
	LockApp();
	theApp.m_pBVHTweaker->SquareShoulders(false);
	UnlockApp();
}


void CbvhTweakDlg::OnNMCustomdrawBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//m_SkeletonTree.OnNMCustomdrawBvhSkeleton(pNMHDR, pResult);
	*pResult = 0;
}


void CbvhTweakDlg::OnHackAttemptretargettoanimeeple()
{
	int nJoints = theApp.m_pBVHFile->GetNJoints();

	if (nJoints<22) {
		AfxMessageBox(_T("Sorry, this figure does not have enough joints for bvhacker to make it Animeeple compatible.\nYou should only have one spine joint (Torso).\n"),MB_ICONINFORMATION );
		return;
	}

	LockApp();
	
	//int nJointsResized = theApp.m_pBVHTweaker->ReSizeJointsAnimeeple();
	theApp.m_pBVHQuaterniser->SetRotationOrder(YZX);

	//if(theApp.m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS)
	//{
	//    theApp.m_pBVHQuaterniser->SetArmsAlignment(ZXY);
	//	theApp.m_pBVHQuaterniser->SetArmsRotationOrder(ZXY);
	//}

	// set the tree ctrl
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnHackAttemptanimeeplenaming()
{
	LockApp();
	theApp.m_pBVHTweaker->RenameJointsAnimeeple();
	// set the tree ctrl
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnSpineCombinechest()
{
	LockApp();
	theApp.m_pBVHTweaker->CombineChestAbdomen();
	// set the tree ctrl
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnEnSetfocusXEdit()
{
	// stop play if playing, 
	if(StatusIs(FILE_PLAYING)) {// reset the frame timer
		OnBnClickedPlayBtn();
	}
}

void CbvhTweakDlg::OnEnSetfocusYEdit()
{
	// stop play if playing, 
	if(StatusIs(FILE_PLAYING)) {// reset the frame timer
		OnBnClickedPlayBtn();
	}
}

void CbvhTweakDlg::OnEnSetfocusZEdit()
{
	// stop play if playing, 
	if(StatusIs(FILE_PLAYING)) {// reset the frame timer
		OnBnClickedPlayBtn();
	}
}

void CbvhTweakDlg::OnBnClickedNotZero3()
{
	OnPrepareNoZeroes();
}

void CbvhTweakDlg::OnPrepareNoZeroes()
{
	LockApp();
	theApp.m_pBVHTweaker->GetRidOfZeros();
	//m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();	
}

// allow drag and drop bvh files to open
void CbvhTweakDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT i;
    UINT nFiles = ::DragQueryFile(hDropInfo, (UINT) -1, NULL, 0);
    for (i = 0; i < nFiles; i++)
    {
        TCHAR szFileName[_MAX_PATH];
        ::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
		CString sFileName(szFileName);
		CString sFileExtension = sFileName.Right(3);
		if(sFileExtension.CompareNoCase(_T("bvh"))==0) 
		{
			// just open the first one passed if more than one
			LoadBVHFromLPTSTR(szFileName);
			::DragFinish(hDropInfo);
			CDialog::OnDropFiles(hDropInfo);
			return;
		}
    }  
    ::DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}




void CbvhTweakDlg::OnHackCraneneckshift()
{
	LockApp();
	theApp.m_pBVHTweaker->CraneNeck(false);
	UnlockApp();
}


void CbvhTweakDlg::OnHackStraightenneckshift()
{
	LockApp();
	theApp.m_pBVHTweaker->CraneNeck(true);
	UnlockApp();
}



// Capture keyboard messages - keyboard shortcuts
BOOL CbvhTweakDlg::PreTranslateMessage(MSG* pMsg)
{
	// MFC likes to beep after certain keydowns, mainly because it's shit.
	MSG msg;// this effectively catches the beep message and removes it. MFC is crap sometimes.
	PeekMessage(&msg, GetSafeHwnd(), WM_CHAR, WM_CHAR, PM_REMOVE);

	if(pMsg->message==WM_KEYDOWN)
	{
		// we don't need the file to be loaded for these ones
		if(pMsg->wParam==79 && GetKeyState(VK_CONTROL)<0) OnBnClickedLoadBVH();// ctrl O - open file
		if(pMsg->wParam==81 && GetKeyState(VK_CONTROL)<0) EndDialog(IDOK);// ctrl Q - quit
		if(pMsg->wParam==80 && GetKeyState(VK_CONTROL)<0) 
		{
			OnViewOptions(); //  ctrl P - open preferences
			return true; // or crash!
		}

		// experimental (Q)
		if(pMsg->wParam==81)
		{
			//LockApp();
			//theApp.m_pBVHQuaterniser->SetArmsAlignment(ZXY);
			//theApp.m_pBVHQuaterniser->SetArmsRotationOrder(ZXY);
			//m_bGlobalEditing=!m_bGlobalEditing;
			//UnlockApp();
			//UpdateInterface();
			
		}
	}

    if(pMsg->message==WM_KEYDOWN&&StatusIs(FILE_LOADED))
    {
		// capture keyboard command here 
		if(pMsg->wParam==83 && GetKeyState(VK_CONTROL)<0) OnBnClickedSaveBVH();// ctrl S - save file

		// ctrl m(agic)
		if(pMsg->wParam==77 && GetKeyState(VK_CONTROL)<0){
			m_SkeletonTree.Initialise(theApp.m_pBVHFile);
			OnBnClickedMotionbuilderFix();
			OnBnClickedCentreButton();
			CString CurrentlySelectedJoint = m_SkeletonTree.GetCurrentSelection();
			BVHJoint* pJoint = theApp.m_pBVHFile->GetJoint(CurrentlySelectedJoint);
			theApp.m_pBVHTweaker->AddMotion(0, 0.5f, 0, MODE_TRANSLATING, CurrentlySelectedJoint);
			OnBnClickedSetT();
			theApp.m_pBVHTweaker->FixSLLoopBug(true);
		}


		// ctrl z
		if(pMsg->wParam==90 && GetKeyState(VK_CONTROL)<0) 
		{
			OnEditUndo();
			return true;// otherwise 'Z' fn gets called...
		}

		// toggle OpenGl lighting (ctrl+shift+L) - undocumented
		if(pMsg->wParam==76
		   &&GetKeyState(VK_CONTROL)<0
		   &&GetKeyState(VK_SHIFT)<0)
		{
			m_oglWindow.ToggleLighting();
			return true;//
		}


        if(pMsg->wParam==VK_SPACE)						OnBnClickedPlayBtn();// play / pause
		if(pMsg->wParam==VK_RIGHT)						OnBnClickedStepFwdBtn();// step fwd
		if(pMsg->wParam==VK_LEFT)						OnBnClickedStepRev();// step back
		if(pMsg->wParam==VK_HOME)						OnBnClickedStopBtn(); // go to start
		if(pMsg->wParam==VK_END){
			OnBnClickedStopBtn();						// go to end
			OnBnClickedStepRev();
		}

		// pass to skeleton view
		if(pMsg->wParam==VK_UP)						    return m_SkeletonTree.PreTranslateMessage(pMsg);
		if(pMsg->wParam==VK_DOWN)						return m_SkeletonTree.PreTranslateMessage(pMsg);

		if(pMsg->wParam==VK_INSERT)						OnInsertJoint(); // insert joint
		if(pMsg->wParam==VK_DELETE)						OnRemoveJoint(); // delete joint
		if(pMsg->wParam==VK_F2)							OnRenameJoint(); // rename joint

		// Truebones hacks
		if(pMsg->wParam==219									// crane neck (Shift+[)
		   &&GetKeyState(VK_SHIFT)<0)
		{
			OnHackCraneneckshift();
			return true;//
		}
		if(pMsg->wParam==221									// straighten neck (Shift+])
		   &&GetKeyState(VK_SHIFT)<0)
		{
			OnHackStraightenneckshift();
			return true;//
		}
		if(pMsg->wParam==219)								OnHackSquareshoulders();// raise shoulders ([)
		if(pMsg->wParam==221)								OnHackRoundshoulders();// lower shoulders  {])


		// ± on numpad
		if(pMsg->wParam==VK_ADD&&GetKeyState(VK_CONTROL)<0)
		{
			OnEditScaleup(); // scale up
			return CDialog::PreTranslateMessage(pMsg);
		}
		if(pMsg->wParam==VK_SUBTRACT&&GetKeyState(VK_CONTROL)<0)
		{
			OnEditScaledown(); // scale down
			return CDialog::PreTranslateMessage(pMsg);
		}

		// ± on main keyboard
		if(pMsg->wParam==189&&GetKeyState(VK_CONTROL)<0)
		{
			OnEditScaledown(); // scale down
			return CDialog::PreTranslateMessage(pMsg);
		}
		if(pMsg->wParam==187&&GetKeyState(VK_CONTROL)<0)
		{
			OnEditScaleup(); // scale up
			return CDialog::PreTranslateMessage(pMsg);
		}


		// common fns
		if(pMsg->wParam==83)							OnBnClickedSetT(); // set T (S)
		if(pMsg->wParam==68)							OnBnClickedRemoveT(); // remove T (D)
		if(pMsg->wParam==76)							OnBnClickedFixLoop(); // fix loop (L)
		if(pMsg->wParam==78)							OnBnClickedMotionbuilderFix(); // no offset (N)
		if(pMsg->wParam==67)							OnBnClickedCentreButton(); // centre (C)
		if(pMsg->wParam==90)							OnPrepareNoZeroes();// !Zero (Z)

		// mode changes
		if(pMsg->wParam==82)							OnEditRotationmode(); // Select rotation mode
		if(pMsg->wParam==84)							OnEditTranslationmode(); // Select translation mode

		// view changes
		if(pMsg->wParam==VK_ESCAPE)					m_oglWindow.ResetView();
		if(pMsg->wParam==70)							m_oglWindow.ToggleFrontView();
		if(pMsg->wParam==86)							m_oglWindow.ToggleSideView();
		if(pMsg->wParam==VK_ADD)						m_oglWindow.Zoom(-m_ZoomIncrement); // zoom in (numpad)
		if(pMsg->wParam==VK_SUBTRACT)				m_oglWindow.Zoom(m_ZoomIncrement); // zoom out (numpad)
		if(pMsg->wParam==189)						m_oglWindow.Zoom(m_ZoomIncrement); // zoom in (keyboard)
		if(pMsg->wParam==187)						m_oglWindow.Zoom(-m_ZoomIncrement); // zoom out (keyboard)

		// experimental (W)
		if(pMsg->wParam==87)
		{
			//LockApp();
			//theApp.m_pBVHTweaker->Reverse();
			//UnlockApp();
		}

		//GetDlgItem(IDC_BVH_SKELETON)->SetFocus();
		//return true;
		
		if (GetDlgItem(IDC_BVH_SKELETON)==GetFocus()) return true;// don't pass the message on if tree view has focus
    } 
    return CDialog::PreTranslateMessage(pMsg);
}

void CbvhTweakDlg::OnHackReversetimeline()
{
	LockApp();
	theApp.m_pBVHTweaker->Reverse();
	UnlockApp();
}


void CbvhTweakDlg::OnViewOptions()
{
	// save the current settings, just in case they cancel thier changes
	DWORD bColourDisplay = m_pRegistrySettings->DisplayColour;
	DWORD bShowGround = m_pRegistrySettings->DisplayGroundPlane;
	DWORD bOGLShading = m_pRegistrySettings->DisplayShading;
	DWORD dFileView = m_pRegistrySettings->DefaultFileView;

	// give them the dialog
	CBVHOptions* pOptionsDialog = new CBVHOptions(this);
	if(pOptionsDialog->DoModal()==IDCANCEL) {
		// need to reset reg settings to previous values
		m_pRegistrySettings->DisplayColour = bColourDisplay;
		m_pRegistrySettings->DisplayGroundPlane = bShowGround;
		m_pRegistrySettings->DisplayShading = bOGLShading;
		m_pRegistrySettings->DefaultFileView = dFileView;
	}
	delete pOptionsDialog;
	pOptionsDialog = NULL;

	// update display
	m_oglWindow.ApplyShading(m_pRegistrySettings->DisplayShading);
	m_oglWindow.SetShowGround(m_pRegistrySettings->DisplayGroundPlane);
	m_oglWindow.SetShowColours(m_pRegistrySettings->DisplayColour);
	//m_ShowGroundCheck.SetCheck(m_pRegistrySettings->DisplayGroundPlane);
	//m_ColourDisplayCheck.SetCheck(m_pRegistrySettings->DisplayColour);
}


void CbvhTweakDlg::OnViewZoomin()
{
	m_oglWindow.Zoom(-m_ZoomIncrement);
}


void CbvhTweakDlg::OnViewZoomout()
{
	m_oglWindow.Zoom(m_ZoomIncrement);
}


void CbvhTweakDlg::OnBnClickedZoomIn()
{
	m_oglWindow.Zoom(-m_ZoomIncrement);
}


void CbvhTweakDlg::OnBnClickedZoomOut()
{
	m_oglWindow.Zoom(m_ZoomIncrement);
}


void CbvhTweakDlg::OnDeleteDescendants()
{
	LockApp();
	theApp.m_pBVHTweaker->DeleteDescendants();
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
}

void CbvhTweakDlg::OnZerooutJoint()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroOut();
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}

void CbvhTweakDlg::OnZeroOutDescendants()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroOutToEnd();
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}

void CbvhTweakDlg::OnHipsTranslationZero()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
		theApp.m_pBVHTweaker->ZeroJoint("hip", MODE_TRANSLATING);
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnHipsRotationZero()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
		theApp.m_pBVHTweaker->ZeroJoint("hip", MODE_ROTATING);
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnHipsBothZero()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_TRANSLATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnUpperbodyAllabovehips()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("abdomen"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}



void CbvhTweakDlg::OnUpperbodyAbdomenandup()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("abdomen"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnUpperbodyChestandup()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("chest"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("chest"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnLowerbodyHips()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnLowerbodyHips32849()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnAllexceptRightarm()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("chest"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("lCollar"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("neck"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lCollar"));
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("neck"));


		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnAllexceptLeftarm()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("chest"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("rCollar"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("neck"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rCollar"));
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("neck"));


		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnAllexceptRightarmandhead()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("chest"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("lCollar"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lCollar"));

		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnAllexceptLeftarmandhead()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("chest"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("rCollar"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rCollar"));

		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnAllexceptRightarm32859()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("lCollar"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lCollar"));


		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnAllexceptLeftarm32860()
{
	LockApp();
	if(StatusIs(FILE_LOADED))
	{
		theApp.m_pBVHTweaker->ZeroJoint(_T("hip"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("abdomen"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroJoint(_T("rCollar"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rCollar"));

		theApp.m_pBVHTweaker->ZeroJoint(_T("lThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("lThigh"));
		theApp.m_pBVHTweaker->ZeroJoint(_T("rThigh"), MODE_ROTATING);
		theApp.m_pBVHTweaker->ZeroOutToEnd(_T("rThigh"));
		m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	}
	UnlockApp();
	UpdateInterface();
}


void CbvhTweakDlg::OnMergeJointWithParent(void)
{
	LockApp();
	theApp.m_pBVHTweaker->CombineWithParent();
	// set the tree ctrl
	m_SkeletonTree.Initialise(theApp.m_pBVHFile);
	UnlockApp();
	//UpdateInterface();
}


void CbvhTweakDlg::OnNMKillfocusBvhSkeleton(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
