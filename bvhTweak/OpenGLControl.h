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
#include "common.h"
#include "bvhTweak.h"

#include "math.h"

class COpenGLControl : public CWnd//,friend class CbvhTweakDlg
{
	public:
		UINT_PTR m_unpTimer;
		// View information variables
		float	 m_fLastX;
		float	 m_fLastY;
		float	 m_fPosX;
		float	 m_fPosY;
		float	 m_fZoom;
		float	 m_fRotX;
		float	 m_fRotY;
		bool	 m_bIsMaximized;

	private:
		// Window information
		CWnd  *hWnd;
		HDC   hdc;			
		HGLRC hrc;			
		int   m_nPixelFormat;
		CRect m_rect;
		CRect m_oldWindow;
		CRect m_originalRect;
		bool  m_bGroundView;
		bool m_bHideHeadMesh;

		// draw body parts
		//void DrawHead(float x, float y, float z);
		//void DrawBone(float x, float y, float z);
		void DrawHips();
		void DrawHandFoot(BVHJoint* pJoint, bool bSelected);
		void DrawSphere(double r, int lats, int longs);
		void DrawHead();
		void DrawGround(GLfloat fGroundHeight);		
		void ResetMatrix(void);

		//rotation matrix
		GLfloat m_fMatrixCurrent[16];

		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg	void OnDraw(CDC *pDC);
		afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);

		DECLARE_MESSAGE_MAP()
		float m_fcx;
		float m_fcy;
		int m_bShowGround;
		int m_bShowColouring;
		int m_bLighting;

	public:
		COpenGLControl();
		virtual ~COpenGLControl();

		void oglCreate(CRect rect, CWnd *parent);
		void oglInitialize();
		void oglDrawScene();

		void ResetView();
		void SetGroundView();

		void ToggleFrontView();
		void ToggleSideView();

		void SetShowGround(int bShow);
		void SetShowColours(int bShowColours);

		void Zoom(float amount);
		void ShowHead(int bShow);

// Declare the variables needed for memory leak detection
#ifdef _DEBUG
    CMemoryState oldMemState, newMemState, diffMemState;
#endif
	void DrawBone(BVHJoint* pJoint, GLfloat fWidth);
	void SetRightView(void);
	void SetLeftView(void);
	void PromptOnSize(UINT nType, CRect form, CRect window);
	void ReInitialise(void);
	void ToggleLighting(void);
	void ApplyShading(int setting);

private:
	int AscertainBoneOrientation(BVHJoint* pJoint);
	void DrawBone2(BVHJoint* pJoint, GLfloat fWidth);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};