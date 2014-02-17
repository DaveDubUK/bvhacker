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
#include "common.h"
#include "BVHTreeCtrl.h"

class CBVHFile
{
	friend class COpenGLControl;
	friend class CBVHTweaker;
	friend class CBVHQuaterniser;

public:

	CBVHFile();
	~CBVHFile();

	// file ops
	bool LoadBVH(CString FilePath);
	bool SaveBVH(CString FilePath);

	// animation ops
	BVHMotion* MotionStepFwd();
	void MotionStepRev();

	// undo stuff
	void SaveState();
	void SwitchState();
	
	// getters / setters
	BVHJoint* GetJoint(int index);
	BVHJoint* GetJoint(CString JointName);
	BVHJoint* GetSelectedJoint();
	BVHMotion* GetCurrentMotionFrame();
	int GetCurrentFrameIdx();
	GLfloat GetGroundHeight();
	float GetFrameTime();
	int GetNFrames();
	void SetNFrames(int nFrames);
	CString GetFileName();
	int GetNJoints();
	int GetNParameters();
	GLfloat GetFigureHeight(void);
	int GetArmsAlignment();

	// misc
	void CalculateFigureHeight();
	int CalculateArmsAxisAlignment();
	void SetCurrentMotionFrame(int index);
	void SetFrameTime(float NewFrameTime);
	void SetAssociatedTreeControl(CBVHTreeCtrl* skeleton);

private:
	
	// the model
	vector<BVHJoint> m_joints;
	vector<BVHMotion> m_motion;
	int m_nJoints;
	int m_nFrames;
	float m_fFrameTime;
	int m_armsAlignment;
	int m_nParameters;
	GLfloat m_fGroundHeight;	
	GLfloat m_nFigureHeightMax;// hips to head 
	GLfloat m_nFigureHeightMin;// hips to feet
	
	// Ctrl Z version of the model
	vector<BVHJoint> m_jointsZ;
	vector<BVHMotion> m_motionZ;
	int m_nJointsZ;
	int m_nFramesZ;
	float m_fFrameTimeZ;
	int m_armsAlignmentZ;
	int m_nParametersZ;
	GLfloat m_fGroundHeightZ;	
	GLfloat m_nFigureHeightMaxZ;// hips to head 
	GLfloat m_nFigureHeightMinZ;// hips to feet	

	CBVHTreeCtrl* m_Skeleton;
	BVHMotion* m_pCurrentMotionFrame;
	int m_nCurrentFrameIdx;
	CString m_FileName;	


	// file load
	CString ProcessNameLine(CString line, int jointNo);
	CString AssignInternalName(CString sJointName);
	void ProcessOffsetLine(BVHJoint* joint, CString line, int jointNo);
	int ProcessChannelLine(BVHJoint* joint, CString line, int jointNo);
	bool ReadLine(CStdioFile* pFile, CString* line);
	bool WriteLine(CStdioFile* pFile, CString* line);
	void SetTabs(CString* line, int nTabs);
	void AppendSpace(CString* line);
	int Delimit(CString line);
	int DelimitR(CString line);// uses reverse find
};
