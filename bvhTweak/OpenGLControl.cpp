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
#include "OpenGLControl.h"
#include ".\openglcontrol.h"
#include "bvhTweakDlg.h"

COpenGLControl::COpenGLControl(void)
: m_fcx(0)
, m_fcy(0)
, m_bShowGround(true)
, m_bShowColouring(true)
, m_bHideHeadMesh(false)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 102.0f;   // Zoom on model in camera view (mustn't be zero)
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY	= 0.0f;		// Rotation on model in camera view
	m_bIsMaximized = false;
	m_bGroundView = false;
	m_bLighting = true;
}

COpenGLControl::~COpenGLControl(void)
{
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	m_fcx = (float)cx;
	m_fcy = (float)cy;

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set our current view perspective
	gluPerspective(50.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
		// If window resize token is "maximize"
		case SIZE_MAXIMIZED:
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly
			MoveWindow(6, 6, cx - 14, cy - 14);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;

			break;
		}

		// If window resize token is "restore"
		case SIZE_RESTORED:
		{
			// If the window is currently maximized
			if (m_bIsMaximized)
			{
				// Get the current window rect
				GetWindowRect(m_rect);

				// Move the window accordingly (to our stored old window)
				MoveWindow(m_oldWindow.left, m_oldWindow.top - 18, m_originalRect.Width() - 4, m_originalRect.Height() - 4);

				// Get the new window rect
				GetWindowRect(m_rect);

				// Store our old window as the new rect
				m_oldWindow = m_rect;
			}

			break;
		}
	}
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	oglInitialize();

	return 0;
}

void COpenGLControl::OnDraw(CDC *pDC)
{
}

void COpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	if(!theApp.m_IsLocked) {
		switch (nIDEvent)
		{
			case 1:
			{
				// Clear color and depth buffer bits
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Draw OpenGL scene
				oglDrawScene();

				// Swap buffers
				SwapBuffers(hdc);

				break;
			}

			default:
				break;
		}

		CWnd::OnTimer(nIDEvent);
	}
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);

	m_fLastX  = (float)point.x;
	m_fLastY  = (float)point.y;

	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		//m_fRotX += (float)0.5f * diffY;
		//if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f)) m_fRotX = 0.0f;

		m_fRotY += (float)0.5f * diffX;
		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f)) m_fRotY = 0.0f;

		m_fPosY += (float)1.0f * diffY;
		if ((m_fPosY > 360.0f) || (m_fPosY < -360.0f)) m_fPosY = 0.0f;
		//m_fPosX += (float)0.5f * diffX;

		//::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_HAND));//
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.5f * diffY;
		if (m_fZoom > 1000.0f) m_fZoom = 100.f;
		if (m_fZoom < 5.0f) m_fZoom = 5.0f;
		//m_fPosX += (float)0.5f * diffX;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		//m_fPosX += (float)0.5f * diffX;
		//m_fPosY -= (float)0.5f * diffY;
	}

	OnDraw(NULL);

	CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, _T("OpenGL"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow	   = rect;
	m_originalRect = rect;

	hWnd = parent;
}

void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;

	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	if(m_bShowColouring) 
		glClearColor(115.0f/255.0f, 165.0f/255.0f, 193.0f/255.0f, 1.0f);// sky blue
	else 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Todo: move lighting initialisation to here

	// set cursor (avoids bug where hourglass is shown after file load)
	::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_ARROW));//IDC_HAND

	// Send draw request
	OnDraw(NULL);
}

void COpenGLControl::oglDrawScene(void)
{
	// set colours
	GLfloat m_ColourWhite[] =  {1.f,1.f,1.f};
	GLfloat m_ColourBlack[] =  {0.f, 0.f, 0.f};
	GLfloat m_ColourGreen[] =  {0.561f, 0.682f, 0.459f};
	GLfloat m_ColourBlue[] =   {0.f, 0.f, 1.f};
	GLfloat m_ColourRed[] =    {1.f, 0.f, 0.f};
	GLfloat m_ColourYellow[] = {1.f, 0.54f, 0.f};

	float fLightOffset = theApp.m_pBVHFile->GetFigureHeight()*2.f;

	// Create light components
	GLfloat ambientLight[] =  { 0.4f, 0.4f, 0.4f, 1.f };
	GLfloat diffuseLight[] =  { 1.f, 1.f, 1.f, 1.f };
	GLfloat specularLight[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat specReflection[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat position[] =      { -fLightOffset, fLightOffset, 0.f, 1.f };

	CbvhTweakDlg* temp = (CbvhTweakDlg*)GetParent();
	if(!theApp.m_IsLocked&&temp->StatusIs(FILE_LOADED)) 
	{
		// set draw mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glMatrixMode(GL_MODELVIEW);

		// do a bit of lighting
		if(m_bLighting)
		{	 
			// first, some global light
			GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.f };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

			// enable color tracking (so can use normal colour change cmds with or without lighting)
			glEnable(GL_COLOR_MATERIAL);
			// set material properties which will be assigned by glColor
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			// set specular reflection
			glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
			glMateriali(GL_FRONT, GL_SHININESS, 512);

			// automatic normal vector scaling (computationally expensive)
			//glEnable(GL_NORMALIZE);


			// from http://www.falloutsoftware.com/tutorials/gl/gl8.htm
			// Assign created components to GL_LIGHT0
			glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
			glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
			glLightfv(GL_LIGHT0, GL_POSITION, position);

			glEnable(GL_LIGHTING);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_LIGHT0);

			// turn on antialiasing
			//glHint(GL_POINT_SMOOTH, GL_NICEST);	
			//glEnable(GL_POINT_SMOOTH );
		}
		else 
		{
			//glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
			glDisable(GL_LIGHTING);
			glShadeModel(GL_FLAT);
			glDisable(GL_LIGHT0);

			//glDisable(GL_LINE_SMOOTH);
		}
		
		// keep track so we can draw the head mesh
		int nHeadIndex = 0;

		// TODO: set the camera's zoom etc - not sure on this stuff
		glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();
		gluPerspective(50.0*m_fZoom, m_fcx/m_fcy, 0.01f, 2000.0f);

		glMatrixMode(GL_MODELVIEW_MATRIX);

		// clear current / remaining matrix
		glLoadIdentity();
		
		// get the current motion frame
		BVHMotion* motionFrame = theApp.m_pBVHFile->GetCurrentMotionFrame();

		// get the ground height
		GLfloat fGroundHeight = theApp.m_pBVHFile->GetGroundHeight();

		vector<BVHJoint>::iterator jointsItr = theApp.m_pBVHFile->m_joints.begin();
		while(jointsItr!=theApp.m_pBVHFile->m_joints.end())
		{
			BVHJoint* pJoint = &(*jointsItr);//theApp.m_pBVHFile->GetJoint(i);
			BVHJoint* pSelectedJoint = theApp.m_pBVHFile->GetSelectedJoint();
			BVHJoint* pParent = theApp.m_pBVHFile->GetJoint(pJoint->parentIndex);

			if(pJoint->internalName.Compare("head")==0) nHeadIndex = pJoint->index;

			// clear current / remaining matrix
			glLoadIdentity();

			// translations (OFFSETs)
			vector<float>::iterator offsetItr = pJoint->offsetChannels.begin();
			float zt = *offsetItr++;
			float yt = *offsetItr++;
			float xt = *offsetItr;

			// rotations
			float yr = 0;
			float xr = 0;
			float zr = 0;

			// deal with any position channels
			if(pJoint->isROOT)//pJoint->offsetChannelOrder!=NONE) 
			{
				xt += motionFrame->data[pJoint->nOffsetMotionDataIndex+0];
				if(pJoint->isROOT)
				{
					float y = motionFrame->data[pJoint->nOffsetMotionDataIndex+1];
					yt += y - fGroundHeight;
				}
				else yt += motionFrame->data[pJoint->nOffsetMotionDataIndex+1];
				zt += motionFrame->data[pJoint->nOffsetMotionDataIndex+2];
			} 
			else if(pJoint->offsetChannelOrder!=NONE)
			{
				// don't do this - do either rots or trans values, not both
				//xt += motionFrame->data[pJoint->nMotionDataIndex+0];
				//yt += motionFrame->data[pJoint->nMotionDataIndex+1];
				//zt += motionFrame->data[pJoint->nMotionDataIndex+2];
			}

			// save local transformation matrix
			glPushMatrix();

				// save translation
				glTranslatef(xt,yt,zt);

				// get and set rotations according to the channels order for the joint
				if(!pJoint->isEndSite&&pJoint->channelOrder!=NONE)
				{
					pJoint->rotationChannels.clear();

					switch(pJoint->channelOrder) {

						case XYZ: {
							xr = motionFrame->data[pJoint->nMotionDataIndex+0];
							yr = motionFrame->data[pJoint->nMotionDataIndex+1];
							zr = motionFrame->data[pJoint->nMotionDataIndex+2];

							glRotatef(xr,1,0,0);
							glRotatef(yr,0,1,0);	
							glRotatef(zr,0,0,1);

							// store the rotations
							pJoint->rotationChannels.push_back(xr);
							pJoint->rotationChannels.push_back(yr);
							pJoint->rotationChannels.push_back(zr);

							break;
						}
						case XZY: {
							xr = motionFrame->data[pJoint->nMotionDataIndex+0];
							zr = motionFrame->data[pJoint->nMotionDataIndex+1];
							yr = motionFrame->data[pJoint->nMotionDataIndex+2];
							
							glRotatef(xr,1,0,0);
							glRotatef(zr,0,0,1);
							glRotatef(yr,0,1,0);	

							// store the rotations
							pJoint->rotationChannels.push_back(xr);
							pJoint->rotationChannels.push_back(yr);
							pJoint->rotationChannels.push_back(zr);

							break;
						}
						case YZX: {
							yr = motionFrame->data[pJoint->nMotionDataIndex+0];
							zr = motionFrame->data[pJoint->nMotionDataIndex+1];
							xr = motionFrame->data[pJoint->nMotionDataIndex+2];
							
							glRotatef(yr,0,1,0);	
							glRotatef(zr,0,0,1);
							glRotatef(xr,1,0,0);

							// store the rotations
							pJoint->rotationChannels.push_back(xr);
							pJoint->rotationChannels.push_back(yr);
							pJoint->rotationChannels.push_back(zr);

							break;
						}
						case ZXY: {
							zr = motionFrame->data[pJoint->nMotionDataIndex+0];
							xr = motionFrame->data[pJoint->nMotionDataIndex+1];
							yr = motionFrame->data[pJoint->nMotionDataIndex+2];
							
							glRotatef(zr,0,0,1);
							glRotatef(xr,1,0,0);
							glRotatef(yr,0,1,0);	

							// store the rotations
							pJoint->rotationChannels.push_back(xr);
							pJoint->rotationChannels.push_back(yr);
							pJoint->rotationChannels.push_back(zr);

							break;
						}
						case ZYX: {
							zr = motionFrame->data[pJoint->nMotionDataIndex+0];
							yr = motionFrame->data[pJoint->nMotionDataIndex+1];
							xr = motionFrame->data[pJoint->nMotionDataIndex+2];
							
							glRotatef(zr,0,0,1);
							glRotatef(yr,0,1,0);	
							glRotatef(xr,1,0,0);
							
							// store the rotations
							pJoint->rotationChannels.push_back(xr);
							pJoint->rotationChannels.push_back(yr);
							pJoint->rotationChannels.push_back(zr);

							break;
						}
						case YXZ: {
							yr = motionFrame->data[pJoint->nMotionDataIndex+0];
							xr = motionFrame->data[pJoint->nMotionDataIndex+1];
							zr = motionFrame->data[pJoint->nMotionDataIndex+2];
							
							glRotatef(yr,0,0,1);
							glRotatef(xr,0,1,0);	
							glRotatef(zr,1,0,0);
							
							// store the rotations
							pJoint->rotationChannels.push_back(xr);
							pJoint->rotationChannels.push_back(zr);
							pJoint->rotationChannels.push_back(yr);

							break;
						}
						default: {
							//AfxMessageBox(_T("Unsupported joint order! Please report bug to support@davedub.co.uk"),MB_ICONSTOP);
							//pJoint = NULL;
							//pParent = NULL;
							//pSelectedJoint = NULL;
							//motionFrame = NULL;
							//temp = NULL;
							//theApp.m_IsLocked = true;
							//return;
							break;
						}
					}
				}

				if(pJoint->scaleChannelOrder!=NONE)
				{
					float fScaleX = motionFrame->data[pJoint->nScaleMotionDataIndex+0];
					float fScaleY = motionFrame->data[pJoint->nScaleMotionDataIndex+1];
					float fScaleZ = motionFrame->data[pJoint->nScaleMotionDataIndex+2];
					// don't use as yet...
				}

				// save in the joint's transform matrix member
				glGetFloatv(GL_MODELVIEW_MATRIX, pJoint->fTransformMatrix);

			glPopMatrix();

			ResetMatrix();

			// leak above here

			// gather a vector of pointers to all joints back to ROOT (excluding current joint)
			BVHJoint* tempJoint = NULL;
			int heirarchy = 0;   
			if(pJoint->isROOT) heirarchy = 0;
			else heirarchy = pParent->heirarchy;
			BVHJoint* pTempParent = pParent;
			vector<BVHJoint*> path;
			while(1+heirarchy>0&&!pJoint->isROOT) {
				path.push_back(pTempParent);
				tempJoint = theApp.m_pBVHFile->GetJoint(pTempParent->parentIndex);
				pTempParent = tempJoint; 
				heirarchy--;
			}

			// now iterate through path summing all the transformations
			vector<BVHJoint*>::iterator jointsPath = path.end();
			glPushMatrix();

				while(jointsPath!=path.begin()) {
					jointsPath--;
					tempJoint = *jointsPath;
					glMultMatrixf(tempJoint->fTransformMatrix);// save all transforms from ROOT
				}
				glGetFloatv(GL_MODELVIEW_MATRIX, m_fMatrixCurrent);

			glPopMatrix();
			tempJoint = NULL;
			path.clear();		

			// set the viewing transformation
			gluLookAt(m_fPosX, m_fPosY, m_fZoom,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
			glRotatef(m_fRotY, 0.f, 1.f, 0.f);
			//glRotatef(m_fRotX, 0.f, 1.f, 0.f);


			//glGetFloatv(GL_MODELVIEW_MATRIX, fViewMatrix);// save the view matrix transform for later
			DrawGround(fGroundHeight);

			// finally draw the joint using the compound transform (m_fMatrixCurrent)
			glPushMatrix();

				GLfloat Default = 0.f;
				if(m_bShowColouring) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					Default = 0.f;
				} else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					Default = 1.f;
				}

				// apply transformations from all joints from ROOT
				glMultMatrixf(m_fMatrixCurrent);

				// sneak a draw head in before the current translations are applied
				if(pJoint->parentIndex==nHeadIndex&&pJoint->isEndSite) 
				{
					glColor3f(Default,Default,1.f);
					DrawHead();
				}
				
				glTranslatef(xt,yt,zt);
				//glMultMatrixf(pJoint->fTransformMatrix);
				
				heirarchy = pJoint->heirarchy;



				////////////////////////////////////////////////////////////////////////////////////
				//																				  //
				//																				  //
				//								Start of draw code								  //
				//																				  //
				//																				  //
				////////////////////////////////////////////////////////////////////////////////////



				// first draw the selected bone (special case)
				if(pSelectedJoint==pJoint) {

					glColor3f(1.0f, 0.54f, 0.f);			// yellow (selected joint)
					if(heirarchy>4) 
					{
						double dRadius = 1.8/((double)heirarchy-3.0);
						if(dRadius<0.6)dRadius=0.6;
						DrawSphere(dRadius, 8, 8);
					}
					else DrawSphere(1.1, 8, 8);

					if(pJoint->isROOT) 
					{
						//DrawHips();
					} 
					else 
					{
						if(pJoint->isEndSite&&pJoint->parentIndex!=nHeadIndex) 
								DrawHandFoot(pJoint,true);
						// need to draw the next bone in yellow, not this one
						if(pJoint->isEndSite)glColor3f(1.0f,Default,Default);//red
						else glColor3f(Default,Default,1.0f);			// blue 
						DrawBone2(pJoint, 0.5f);
						//DrawBone(pJoint, 0.35f);
					}

				} 
				// draw un-selected joints 
				else {
					
					glColor3f(Default,Default,1.0f);			// blue
					if(heirarchy>4) 
					{
						double dRadius = 1.8/((double)heirarchy-3.0);
						if(dRadius<0.6)dRadius=0.6;
						DrawSphere(dRadius, 8, 8);
					}
					else DrawSphere(1.1, 8, 8);

					if(pJoint->isROOT) 
					{
						//DrawHips();
					}
					else if(pJoint->isEndSite) 
					{
						// draw hands or feet on end site (unless is head)
						if(pJoint->parentIndex!=nHeadIndex) 
								DrawHandFoot(pJoint,false);

						glColor3f(1.0f,Default,Default);			// red
						if(heirarchy>4) 
						{
							double dRadius = 1.8/((double)heirarchy-3.0);
							if(dRadius<0.6)dRadius=0.6;
							DrawSphere(dRadius, 8, 8);
						}
						else DrawSphere(1.1, 8, 8);
						if(pSelectedJoint==pParent&&!pJoint->isROOT) 
							 glColor3f(1.0f, 0.54f, 0.f);			// yellow (selected joint)
						else glColor3f(1.0f,Default,Default);			// red
						DrawBone2(pJoint, 0.5f);
						glColor3f(Default,Default,1.0f);			// blue
					}
					else 
					{							
						// if parent is selected
						if(pSelectedJoint==pParent&&!pJoint->isROOT) 
							 glColor3f(1.0f, 0.54f, 0.f);			// yellow (selected joint)
						else glColor3f(Default,Default,1.0f);			// blue

						//DrawBone(pJoint, 0.35f);
						DrawBone2(pJoint, 0.5f);
					}
				}

			glPopMatrix();

			jointsItr++;

			pJoint = NULL;
			pParent = NULL;
			pSelectedJoint = NULL;
		}
		motionFrame = NULL;
	}
	temp = NULL;
}
void COpenGLControl::DrawHandFoot(BVHJoint* pJoint, bool bSelected)
{
	float headTopH = 0.f;
	float headLowH = 0.f;
	float x = 0.f;

	GLfloat Default = 0.f;
	if(m_bShowColouring) {
		Default = 0.f;
		headTopH = 0.75f;
		headLowH = 0.75f;
		x = 2.0f;
	} else {
		Default = 1.f;
		headTopH = 1.0f;
		headLowH = 1.0f;
		x = 3.0f;
	}

	int orientation = AscertainBoneOrientation(pJoint);

	// Start Drawing 
	glBegin(GL_TRIANGLES);//GL_LINE_STRIP);//

		switch(orientation)
		{
		case N_X:
			{
				if(bSelected) glColor3f(1.0f, 0.54f, 0.f);		// yellow
				else glColor3f(Default,Default,1.f);			// Blue
				glVertex3f(0.0f,headTopH*1.0f, 0.0f);           // Top Of Triangle (Front)
				glVertex3f(x*-1.0f, 0.0f, 1.0f);				// Left Of Triangle (Front)
				glVertex3f(x*1.0f, 0.0f, 1.0f);					// Right Of Triangle (Front)

				glVertex3f( 0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Right)
				glVertex3f(x*1.0f, 0.0f, 1.0f);					// Left Of Triangle (Right)
				glVertex3f(x*1.0f, 0.0f, -1.0f);				// Right Of Triangle (Right)

				glVertex3f(x*0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Back)
				glVertex3f(x*1.0f, 0.0f, -1.0f);				// Left Of Triangle (Back)
				glVertex3f(x*-1.0f, 0.0f, -1.0f);				// Right Of Triangle (Back)

				glVertex3f(x*0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Left)
				glVertex3f(x*-1.0f, 0.0f,-1.0f);				// Left Of Triangle (Left)
				glVertex3f(x*-1.0f, 0.0f, 1.0f);				// Right Of Triangle (Left)

		//underneath
				if(bSelected) glColor3f(1.0f, 0.54f, 0.f);		// yellow
				else glColor3f( 1.0f,Default,Default);				// Red
				glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Front)
				glVertex3f(x*-1.0f, 0.0f, 1.0f);				// Left Of Triangle (Front)
				glVertex3f(x*1.0f, 0.0f, 1.0f);					// Right Of Triangle (Front)

				glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Right)
				glVertex3f(x*1.0f, 0.0f, 1.0f);					// Left Of Triangle (Right)
				glVertex3f(x*1.0f, 0.0f, -1.0f);				// Right Of Triangle (Right)

				glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Back)
				glVertex3f(x*1.0f, 0.0f, -1.0f);				// Left Of Triangle (Back)
				glVertex3f(x*-1.0f, 0.0f, -1.0f);				// Right Of Triangle (Back)

				glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Left)
				glVertex3f(x*-1.0f, 0.0f,-1.0f);				// Left Of Triangle (Left)
				glVertex3f(x*-1.0f, 0.0f, 1.0f);				// Right Of Triangle (Left)
				break;
			}
		case N_Z:
			{
				if(bSelected) glColor3f(1.0f, 0.54f, 0.f);		// yellow
				else glColor3f(Default,Default,1.f);			// Blue
				glVertex3f(0.0f,headTopH*1.0f, 0.0f);           // Top Of Triangle (Front)
				glVertex3f(1.0f, 0.0f, x*-1.0f);				// Left Of Triangle (Front)
				glVertex3f(1.0f, 0.0f, x*1.0f);					// Right Of Triangle (Front)

				glVertex3f(0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Right)
				glVertex3f(1.0f, 0.0f, x*1.0f);					// Left Of Triangle (Right)
				glVertex3f(-1.0f, 0.0f, x*1.0f);				// Right Of Triangle (Right)

				glVertex3f(0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Back)
				glVertex3f(-1.0f, 0.0f, x*1.0f);				// Left Of Triangle (Back)
				glVertex3f(-1.0f, 0.0f, x*-1.0f);				// Right Of Triangle (Back)

				glVertex3f(0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Left)
				glVertex3f(-1.0f, 0.0f,x*-1.0f);				// Left Of Triangle (Left)
				glVertex3f(1.0f, 0.0f, x*-1.0f);				// Right Of Triangle (Left)

		//underneath
				if(bSelected) glColor3f(1.0f, 0.54f, 0.f);		// yellow
				else glColor3f( 1.0f,Default,Default);				// Red

				glVertex3f(0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Front)
				glVertex3f(1.0f, 0.0f, x*-1.0f);				// Left Of Triangle (Front)
				glVertex3f(1.0f, 0.0f, x*1.0f);					// Right Of Triangle (Front)

				glVertex3f(0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Right)
				glVertex3f(1.0f, 0.0f, x*1.0f);					// Left Of Triangle (Right)
				glVertex3f(-1.0f, 0.0f, x*1.0f);				// Right Of Triangle (Right)

				glVertex3f(0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Back)
				glVertex3f(-1.0f, 0.0f, x*1.0f);				// Left Of Triangle (Back)
				glVertex3f(-1.0f, 0.0f, x*-1.0f);				// Right Of Triangle (Back)

				glVertex3f(0.0f,(GLfloat)(-1.0*headLowH), 0.0f);// Top Of Triangle (Left)
				glVertex3f(-1.0f, 0.0f,x*-1.0f);				// Left Of Triangle (Left)
				glVertex3f(1.0f, 0.0f, x*-1.0f);				// Right Of Triangle (Left)
				break;
			}
		case N_Y:
			{ 
				if(bSelected) glColor3f(1.0f, 0.54f, 0.f);		// yellow
				else glColor3f(Default,Default,1.f);			// Blue

				glVertex3f(headTopH*1.0f, 0.0f,0.0f);           // Top Of Triangle (Front)
				glVertex3f( 0.0f, x*-1.0f,1.0f);				// Left Of Triangle (Front)
				glVertex3f(0.0f, x*1.0f, 1.0f);					// Right Of Triangle (Front)

				glVertex3f(headTopH*1.0f,0.0f, 0.0f);			// Top Of Triangle (Right)
				glVertex3f(0.0f,x*1.0f,  1.0f);					// Left Of Triangle (Right)
				glVertex3f(0.0f, x*1.0f, -1.0f);				// Right Of Triangle (Right)

				glVertex3f(headTopH*1.0f,x*0.0f, 0.0f);			// Top Of Triangle (Back)
				glVertex3f( 0.0f, x*1.0f,-1.0f);				// Left Of Triangle (Back)
				glVertex3f(0.0f, x*-1.0f, -1.0f);				// Right Of Triangle (Back)

				glVertex3f(headTopH*1.0f,x*0.0f, 0.0f);			// Top Of Triangle (Left)
				glVertex3f(0.0f,x*-1.0f, -1.0f);				// Left Of Triangle (Left)
				glVertex3f(0.0f, x*-1.0f, 1.0f);				// Right Of Triangle (Left)

		//underneath
				if(bSelected) glColor3f(1.0f, 0.54f, 0.f);		// yellow
				else glColor3f( 1.0f,Default,Default);			// Red

				glVertex3f((GLfloat)(-1.0*headLowH),x*0.0f, 0.0f);// Top Of Triangle (Front)
				glVertex3f(0.0f, x*-1.0f, 1.0f);				// Left Of Triangle (Front)
				glVertex3f(0.0f, x*1.0f, 1.0f);					// Right Of Triangle (Front)

				glVertex3f((GLfloat)(-1.0*headLowH),x*0.0f, 0.0f);// Top Of Triangle (Right)
				glVertex3f(0.0f, x*1.0f, 1.0f);					// Left Of Triangle (Right)
				glVertex3f(0.0f, x*1.0f, -1.0f);				// Right Of Triangle (Right)

				glVertex3f((GLfloat)(-1.0*headLowH), x*0.0f,0.0f);// Top Of Triangle (Back)
				glVertex3f(0.0f, x*1.0f, -1.0f);				// Left Of Triangle (Back)
				glVertex3f(0.0f, x*-1.0f, -1.0f);				// Right Of Triangle (Back)

				glVertex3f((GLfloat)(-1.0*headLowH), x*0.0f,0.0f);// Top Of Triangle (Left)
				glVertex3f(0.0f,x*-1.0f, -1.0f);				// Left Of Triangle (Left)
				glVertex3f(0.0f, x*-1.0f, 1.0f);				// Right Of Triangle (Left)
				break;
			}
		}

	glEnd();
}
//
// depricated
//
void COpenGLControl::DrawHips()
{
	float headTopH = 0.4f;
	float headLowH = 0.6f;
	float x = 2.5;

	GLfloat Default = 0.f;
	if(m_bShowColouring) {
		Default = 0.f;
	} else {
		Default = 1.f;
	}

	glBegin(GL_TRIANGLES);					// Start Drawing A Triangle

		glColor3f(Default,Default,1.f);			// Blue
		glVertex3f( 0.0f,headTopH*1.0f, 0.0f);          // Top Of Triangle (Front)
		glColor3f(1.0f,Default,Default);			// Red
		glVertex3f(x*-1.0f, 0.0f, 1.0f);			// Left Of Triangle (Front)
		glVertex3f(x*1.0f, 0.0f, 1.0f);			// Right Of Triangle (Front)

		glColor3f(Default,Default,1.0f);			// Blue
		glVertex3f( 0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Right)
		glColor3f( 1.0f,Default,Default);			// Red
		glVertex3f(x*1.0f, 0.0f, 1.0f);			// Left Of Triangle (Right)
		glColor3f(Default,Default,1.0f);			// Blue
		glVertex3f(x*1.0f, 0.0f, -1.0f);			// Right Of Triangle (Right)

		glVertex3f(x*0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Back)
		glVertex3f(x*1.0f, 0.0f, -1.0f);			// Left Of Triangle (Back)
		glVertex3f(x*-1.0f, 0.0f, -1.0f);			// Right Of Triangle (Back)

		glVertex3f(x*0.0f,headTopH*1.0f, 0.0f);			// Top Of Triangle (Left)
		glVertex3f(x*-1.0f, 0.0f,-1.0f);			// Left Of Triangle (Left)
		glColor3f( 1.0f,Default,Default);			// Red
		glVertex3f(x*-1.0f, 0.0f, 1.0f);			// Right Of Triangle (Left)

//under
		glColor3f( Default,Default,1.0f);			// Blue
		glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);          // Top Of Triangle (Front)
		glColor3f( 1.0f,Default,Default);			// Red
		glVertex3f(x*-1.0f, 0.0f, 1.0f);			// Left Of Triangle (Front)
		glVertex3f(x*1.0f, 0.0f, 1.0f);			// Right Of Triangle (Front)

		glColor3f( Default,Default, 1.f);			// Blue
		glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);			// Top Of Triangle (Right)
		glColor3f( 1.0f,Default,Default);			// Red
		glVertex3f(x*1.0f, 0.0f, 1.0f);			// Left Of Triangle (Right)
		glColor3f(Default,Default,1.0f);			// Blue
		glVertex3f(x*1.0f, 0.0f, -1.0f);			// Right Of Triangle (Right)

		glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);			// Top Of Triangle (Back)
		glVertex3f(x*1.0f, 0.0f, -1.0f);			// Left Of Triangle (Back)
		glVertex3f(x*-1.0f, 0.0f, -1.0f);			// Right Of Triangle (Back)

		glVertex3f(x*0.0f,(GLfloat)(-1.0*headLowH), 0.0f);			// Top Of Triangle (Left)
		glVertex3f(x*-1.0f, 0.0f,-1.0f);			// Left Of Triangle (Left)
		glColor3f(1.0f,Default,Default);			// Red
		glVertex3f(x*-1.0f, 0.0f, 1.0f);			// Right Of Triangle (Left)

	glEnd();
}
//
// depricated
//
void COpenGLControl::DrawBone(BVHJoint* pJoint, GLfloat fWidth)
{ 
	// get offsets and (last) rotations from pJoint
	GLfloat fX = -pJoint->offsetChannels[2];
	GLfloat fY = -pJoint->offsetChannels[1];	
	GLfloat fZ = -pJoint->offsetChannels[0];

	if(m_bShowColouring) {
		fWidth *= 1.f;
	} else {
		fWidth *= 2.0f;
	}

	// Start Drawing 
	glBegin(GL_TRIANGLES);

		//glNormal3f( 1.f, 0.f, 0.f );
		glVertex3f( -fWidth, 0.0f, 0.0f );           // x plane element
		glVertex3f( fWidth, 0.0f, 0.0f );			
		glVertex3f( fX-fWidth, fY, fZ );

		glVertex3f( fWidth, 0.0f, 0.0f );			
		glVertex3f( fX+fWidth, fY, fZ );
		glVertex3f( fX-fWidth, fY, fZ ); 

		//glNormal3f( 0.f, 1.f, 0.f );
		glVertex3f( 0.f, -fWidth, 0.f );           // y plane element
		glVertex3f( 0.f, fWidth, 0.f );			
		glVertex3f( fX, fY-fWidth, fZ );

		glVertex3f( 0.f, fWidth, 0.f );			
		glVertex3f( fX, fY-fWidth, fZ );
		glVertex3f( fX, fY+fWidth, fZ );

		//glNormal3f( 0.f, 0.f, 1.f );
		glVertex3f( 0.f, 0.f, -fWidth );           // z plane element
		glVertex3f( 0.f, 0.f, fWidth );			
		glVertex3f( fX, fY, fZ-fWidth );

		glVertex3f( 0.f, 0.f, fWidth );			
		glVertex3f( fX, fY, fZ-fWidth );
		glVertex3f( fX, fY, fZ+fWidth );

	glEnd();
}

// sphere code from: http://ozark.hendrix.edu/~burch/cs/490/sched/feb8/
void COpenGLControl::DrawHead()
{
	if(m_bHideHeadMesh) return;

	double r = theApp.m_pBVHFile->GetFigureHeight()/18.f;
	int lats = 10;
	int longs = 10;

	//if(m_bShowColouring) {

	//} else {
	//	r*=1.1;
	//}

	// sphere is draw on side, so compensate
	glRotatef(90,1,0,0);
	
	int i, j;
	for(i = 0; i <= lats; i++) {
	   double lat0 = N_PI * (-0.5 + (double) (i - 1) / lats);
	   double z0  = sin(lat0);
	   double zr0 =  cos(lat0);

	   double lat1 = N_PI * (-0.5 + (double) i / lats);
	   double z1 = sin(lat1);
	   double zr1 = cos(lat1);

	   glBegin(GL_LINE_STRIP);
	   for(j = 0; j <= longs; j++) {
		   double lng = 2 * N_PI * (double) (j - 1) / longs;
		   double x = cos(lng);
		   double y = sin(lng);

		   Vector3f vNormalisedNormal((r * x * zr0), (GLfloat)(r * y * zr0), (GLfloat)(r * z0));
		   vNormalisedNormal.normalize();
		   glNormal3f(vNormalisedNormal.x,vNormalisedNormal.y,vNormalisedNormal.z);
		   //glNormal3f((GLfloat)(r * x * zr0), (GLfloat)(r * y * zr0), (GLfloat)(r * z0));
		   glVertex3f((GLfloat)(r * x * zr0), (GLfloat)(r * y * zr0), (GLfloat)(r * z0));

		   Vector3f vNormalisedNormal2((r * x * zr1), (GLfloat)(r * y * zr1), (GLfloat)(r * z1));
		   vNormalisedNormal2.normalize();
		   glNormal3f(vNormalisedNormal2.x,vNormalisedNormal2.y,vNormalisedNormal2.z);
		   //glNormal3f((GLfloat)(r * x * zr1), (GLfloat)(r * y * zr1), (GLfloat)(r * z1));
		   glVertex3f((GLfloat)(r * x * zr1), (GLfloat)(r * y * zr1), (GLfloat)(r * z1));
	   }
	   glEnd();
	}

	// decompensate
	glRotatef(-90,1,0,0);
}

// sphere code from: http://ozark.hendrix.edu/~burch/cs/490/sched/feb8/
void COpenGLControl::DrawSphere(double r, int lats, int longs)
{
	if(m_bShowColouring) {

	} else {
		r*=1.4;
	}
	
	int i, j;
	for(i = 0; i <= lats; i++) {
	   double lat0 = N_PI * (-0.5 + (double) (i - 1) / lats);
	   double z0  = sin(lat0);
	   double zr0 =  cos(lat0);

	   double lat1 = N_PI * (-0.5 + (double) i / lats);
	   double z1 = sin(lat1);
	   double zr1 = cos(lat1);

	   glBegin(GL_QUAD_STRIP);
	   for(j = 0; j <= longs; j++) {
		   double lng = 2 * N_PI * (double) (j - 1) / longs;
		   double x = cos(lng);
		   double y = sin(lng);

		   Vector3f vNormalisedNormal((r * x * zr0), (GLfloat)(r * y * zr0), (GLfloat)(r * z0));
		   vNormalisedNormal.normalize();
		   glNormal3f(vNormalisedNormal.x,vNormalisedNormal.y,vNormalisedNormal.z);
		   //glNormal3f((GLfloat)(r * x * zr0), (GLfloat)(r * y * zr0), (GLfloat)(r * z0));
		   glVertex3f((GLfloat)(r * x * zr0), (GLfloat)(r * y * zr0), (GLfloat)(r * z0));


		   Vector3f vNormalisedNormal2((r * x * zr1), (GLfloat)(r * y * zr1), (GLfloat)(r * z1));
		   vNormalisedNormal2.normalize();
		   glNormal3f(vNormalisedNormal2.x,vNormalisedNormal2.y,vNormalisedNormal2.z);
		   //glNormal3f((GLfloat)(r * x * zr1), (GLfloat)(r * y * zr1), (GLfloat)(r * z1));
		   glVertex3f((GLfloat)(r * x * zr1), (GLfloat)(r * y * zr1), (GLfloat)(r * z1));
	   }
	   glEnd();
	}
}

void COpenGLControl::ResetMatrix(void)
{
	// set identity matrix
	for(int i=0;i<16;i++) m_fMatrixCurrent[i]=0.0f;
	m_fMatrixCurrent[0] = 1.0f;
	m_fMatrixCurrent[5] = 1.0f;
	m_fMatrixCurrent[10] = 1.0f;
	m_fMatrixCurrent[15] = 1.0f;
}

void COpenGLControl::DrawGround(GLfloat fGroundHeight)
{
	if(!m_bShowGround) return;

	GLfloat s = 250.0;//  size of grid
	GLfloat n = 50.0;//   size of squares
	

	if(m_bShowColouring) {

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if(!m_bGroundView) 
		{
			float fBrightener = 1.65f;
			if(m_bLighting) glColor3f(0.561f*fBrightener, 
				                      0.682f*fBrightener, 
									  0.459f*fBrightener);	// green
			else glColor3f(0.561f, 0.682f, 0.459f);	// green
		}
		else 
		{
			glColor3f(0.f,0.f,0.f);	// black
		}

		glBegin(GL_QUADS);

			for(GLfloat i=-s;i<s;i+=n) {
				for(GLfloat j=-s;j<s;j+=n) {
					glNormal3f(  i, -fGroundHeight, j);
					glVertex3f(  i, -fGroundHeight, j);				// far Left
					glNormal3f(n+i, -fGroundHeight, j);
					glVertex3f(n+i, -fGroundHeight, j);				// far Right
					glNormal3f(n+i, -fGroundHeight, j+n);	
					glVertex3f(n+i, -fGroundHeight, j+n);				// near Right
					glNormal3f(  i, -fGroundHeight, j+n);
					glVertex3f(  i, -fGroundHeight, j+n);				// near Left
				}
			}
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.0f,0.0f,0.0f);	// black

		glBegin(GL_QUADS);
			for(GLfloat i=-s;i<s;i+=n) {
				for(GLfloat j=-s;j<s;j+=n) {
					glNormal3f(  i, -fGroundHeight, j);
					glVertex3f(  i, -fGroundHeight, j);				// far Left
					glNormal3f(n+i, -fGroundHeight, j);
					glVertex3f(n+i, -fGroundHeight, j);				// far Right
					glNormal3f(n+i, -fGroundHeight, j+n);	
					glVertex3f(n+i, -fGroundHeight, j+n);				// near Right
					glNormal3f(  i, -fGroundHeight, j+n);
					glVertex3f(  i, -fGroundHeight, j+n);				// near Leftt
				}
			}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	} else {

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(1.0f,1.0f,1.0f);	// white

		glBegin(GL_QUADS);
			for(GLfloat i=-s;i<s;i+=n) {
				for(GLfloat j=-s;j<s;j+=n) {
					glNormal3f(  i, -fGroundHeight, j);
					glVertex3f(  i, -fGroundHeight, j);				// far Left
					glNormal3f(n+i, -fGroundHeight, j);
					glVertex3f(n+i, -fGroundHeight, j);				// far Right
					glNormal3f(n+i, -fGroundHeight, j+n);	
					glVertex3f(n+i, -fGroundHeight, j+n);				// near Right
					glNormal3f(  i, -fGroundHeight, j+n);
					glVertex3f(  i, -fGroundHeight, j+n);				// near Left
				}
			}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

// set front view
void COpenGLControl::ResetView(void)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 102.0f;    // Zoom on model in camera view (mustn't be zero)
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY	= 0.0f;		// Rotation on model in camera view
	m_bGroundView=false;
}

void COpenGLControl::SetGroundView(void)
{
	m_fPosY = -theApp.m_pBVHFile->GetGroundHeight();		// Y position of model in camera view
	m_bGroundView=true;
}

void COpenGLControl::ToggleFrontView()
{
	if(m_fPosY<0)
	{
		// must be in ground view
		ResetView();
	}
	else
	{
		SetGroundView();
	}
}

void COpenGLControl::SetShowGround(int bShow)
{
	m_bShowGround = bShow;
}

void COpenGLControl::SetShowColours(int bShowColours)
{
	m_bShowColouring = bShowColours;

	// set new b/g colour
	if(m_bShowColouring) 
		glClearColor(115.0f/255.0f, 165.0f/255.0f, 193.0f/255.0f, 1.0f);// sky blue
	else 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void COpenGLControl::ShowHead(int bShow)
{
	m_bHideHeadMesh = !bShow;
}


void COpenGLControl::SetRightView(void)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 102.0f;    // Zoom on model in camera view (mustn't be zero)
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY	= 90.0f;		// Rotation on model in camera view
	m_bGroundView=false;
}

void COpenGLControl::SetLeftView(void)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 102.0f;    // Zoom on model in camera view (mustn't be zero)
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY	= -90.0f;		// Rotation on model in camera view
	m_bGroundView=false;
}

void COpenGLControl::ToggleSideView()
{
	if(m_fRotY>0)
	{
		// Set the Left View
		SetLeftView();
	}
	else
	{
		SetRightView();
	}
}

void COpenGLControl::PromptOnSize(UINT nType, CRect form, CRect window)
{
	int cy = window.bottom-window.top;//form.right;//+window.top;
	int cx = window.right-window.left;//form.bottom;//+window.left;

	CWnd::OnSize(nType, cx, cy);

	m_fcx = (float)cx;
	m_fcy = (float)cy;

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set our current view perspective
	gluPerspective(50.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
		// If window resize token is "maximize"
		case SIZE_MAXIMIZED:
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly
			MoveWindow(287, 11, cx, cy);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;

			break;
		}

		// If window resize token is "restore"
		case SIZE_RESTORED:
		{
			// If the window is currently maximized
			//if (m_bIsMaximized)
			{
				// Get the current window rect
				GetWindowRect(m_rect);

				// Move the window accordingly (to our stored old window)
				MoveWindow(287, 11, cx, cy);

				// Get the new window rect
				GetWindowRect(m_rect);

				// Store our old window as the new rect
				m_oldWindow = m_rect;
			}

			break;
		}
	}
}

void COpenGLControl::ReInitialise(void)
{
	ResetView();
	ResetMatrix();
}

void COpenGLControl::ToggleLighting(void)
{
	m_bLighting=1-m_bLighting;
}

void COpenGLControl::ApplyShading(int setting)
{
	m_bLighting=setting;
}

int COpenGLControl::AscertainBoneOrientation(BVHJoint* pJoint)
{
	GLfloat fX = fabs(pJoint->offsetChannels[2]);
	GLfloat fY = fabs(pJoint->offsetChannels[1]);	
	GLfloat fZ = fabs(pJoint->offsetChannels[0]);

	if(fX>fY&&fX>fZ) return N_X;
	if(fY>fX&&fY>fZ) return N_Y;
	if(fZ>fX&&fZ>fY) return N_Z;

	if(fX==fY&&fX>fZ) return N_XY;
	if(fX==fZ&&fX>fY) return N_XZ;
	if(fY==fZ&&fY>fX) return N_YZ;

	if(fX==fY&&fY==fZ) return N_XYZ;

	return 0;
}

void COpenGLControl::DrawBone2(BVHJoint* pJoint, GLfloat fWidth)
{
	// get offsets for pJoint
	GLfloat fX = pJoint->offsetChannels[2];
	GLfloat fY = pJoint->offsetChannels[1];	
	GLfloat fZ = pJoint->offsetChannels[0];

	// lats and longs
	int lats = 1;// still working on this - try setting it to 2...
	int longs = 32;// i.e. nFaces
	int orientation = AscertainBoneOrientation(pJoint);
	if(m_bShowColouring) {
		fWidth *= 1.f;
	} else {
		fWidth *= 2.0f;
		longs = 6;// i.e. nFaces
	}
	float fStep = N_TWO_PI/(float)longs;

	// Start Drawing 
	glBegin(GL_TRIANGLES);//GL_LINE_STRIP);//

		switch(orientation)
		{
		case N_X:
			{
				float fTheta=0.f;
				fX = -pJoint->offsetChannels[2];
				for(fTheta=0;fTheta<N_TWO_PI;fTheta+=fStep)
				{
					float fZ1 = fWidth*cosf(fTheta);// cos(0) = 1, cos(90) = 0
					float fY1 = fWidth*sinf(fTheta);// sin(0) = 0, sin(90) = 1
					float fZ2 = fWidth*cosf(fTheta+fStep);// cos(90) = 0
					float fY2 = fWidth*sinf(fTheta+fStep);// sin(90) = 1

					float fNormalZ = cosf(fTheta+(fStep/2.f));
					float fNormalY = sinf(fTheta+(fStep/2.f));

					glNormal3f(0.f,fNormalY,fNormalZ);
					glVertex3f(0.f,fY1,fZ1);		// 0,0,1
					glVertex3f(fX,fY1-fY,fZ1-fZ);	// 1,
					glVertex3f(0.f,fY2,fZ2);		

					glVertex3f(0.f,fY2,fZ2);		
					glVertex3f(fX, fY2-fY,fZ2-fZ);	
					glVertex3f(fX, fY1-fY,fZ1-fZ);		
				}
				break;
			}
		case N_Y:
			{
				float fY1=0.f;// y pos starting at joint root
				float fY2=0.f;// y pos ending at extent of joint
				float fSegment = 0.f;
				float fTheta=0.f;
		
				for(fTheta=0;fTheta<N_TWO_PI;fTheta+=fStep)
				{
					// offsets for drawn planes
					float fX1_total = fWidth*cosf(fTheta);// cos(0) = 1, cos(90) = 0
					float fZ1_total = fWidth*sinf(fTheta);// sin(0) = 0, sin(90) = 1
					float fX2_total = fWidth*cosf(fTheta+fStep);// cos(90) = 0
					float fZ2_total = fWidth*sinf(fTheta+fStep);// sin(90) = 1

					float fX1 = fX1_total;
					float fX2 = fX2_total;
					
					float fZ1 = fZ1_total;
					float fZ2 = fZ2_total;

					float fNormalX = cosf(fTheta+(fStep/2.f));
					float fNormalZ = sinf(fTheta+(fStep/2.f));

					// draw segments as specified by nLats
					for(fSegment=0.f;fSegment<lats;fSegment++)
					{
						// reduce X and Z components proportional to no. of lats
						fX1 = fX1_total*((fSegment+1)/lats);
						fZ1 = fZ1_total*((fSegment+1)/lats);

						fX2 = fX2_total*((fSegment+1)/lats);
						fZ2 = fZ2_total*((fSegment+1)/lats);

						// reduce Y component proportional to no. of lats
						fY1 = fSegment*(-fY/lats);//0.f;//
						fY2 = (fSegment+1)*(-fY/lats);//-fY;//

						// reduce parent joint offset proportional to no. of lats
						float fXOff = fX/lats;
						float fZOff = fZ/lats;				

						glNormal3f(fNormalX,fY1,fNormalZ);
						glVertex3f(fX1,fY1,fZ1);					// 1,0,0
						glVertex3f(fX1-fXOff,fY2,fZ1-fZOff);		// 1,1,0
						glVertex3f(fX2-fXOff,fY2,fZ2-fZOff);		// 0,0,1

						glVertex3f(fX1,fY1,fZ1);					// 0,0,1
						glVertex3f(fX2-fXOff,fY2,fZ2-fZOff);		// 0,1,1
						glVertex3f(fX2,fY1,fZ2);					// 1,1,0
					}
				}
				break;
			}
		case N_Z:
			{
				float fTheta=0.f;
				fZ = -pJoint->offsetChannels[0];
				for(fTheta=0;fTheta<N_TWO_PI;fTheta+=fStep)
				{
					float fX1 = fWidth*cosf(fTheta);// cos(0) = 1, cos(90) = 0
					float fY1 = fWidth*sinf(fTheta);// sin(0) = 0, sin(90) = 1
					float fX2 = fWidth*cosf(fTheta+fStep);// cos(90) = 0
					float fY2 = fWidth*sinf(fTheta+fStep);// sin(90) = 1

					float fNormalX = cosf(fTheta+(fStep/2.f));
					float fNormalY = sinf(fTheta+(fStep/2.f));

					glNormal3f(fNormalX,fNormalY,0.f);
					glVertex3f(fX1,fY1,0.f);		
					glVertex3f(fX1-fX,fY1-fY,fZ);	
					glVertex3f(fX2,fY2,0.f);		

					glVertex3f(fX2,fY2,0.f);		
					glVertex3f(fX2-fX, fY2-fY,fZ);	
					glVertex3f(fX1-fX, fY1-fY,fZ);	
				}
				break;
			}
		default:
			{
				//AfxMessageBox(_T("Ah - I have two identical dimensions for a bone offset.\nI didn't think that would happen very often!\n"));
			}
		}
	glEnd();
}

void COpenGLControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_CROSS));
	CWnd::OnLButtonDown(nFlags, point);
}

void COpenGLControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_ARROW));
	CWnd::OnLButtonUp(nFlags, point);
}

void COpenGLControl::OnMButtonDown(UINT nFlags, CPoint point)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_CROSS));
	CWnd::OnMButtonDown(nFlags, point);
}

void COpenGLControl::OnMButtonUp(UINT nFlags, CPoint point)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_ARROW));
	CWnd::OnMButtonUp(nFlags, point);
}

void COpenGLControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_CROSS));
	CWnd::OnRButtonDown(nFlags, point);
}

void COpenGLControl::OnRButtonUp(UINT nFlags, CPoint point)
{
	::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_ARROW));
	CWnd::OnRButtonUp(nFlags, point);
}

BOOL COpenGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int diffX = (int)(pt.x - m_fLastX);
	int diffY = (int)(pt.y - m_fLastY);

	m_fZoom -= (float)0.5f * zDelta;
	if (m_fZoom > 2200.0f) m_fZoom = 2200.f;
	if (m_fZoom < 5.0f) m_fZoom = 5.0f;
		
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGLControl::Zoom(float amount)
{
	m_fZoom += amount;
	if (m_fZoom > 2200.0f) m_fZoom = 2200.f;
	if (m_fZoom < 5.0f) m_fZoom = 5.0f;
}

