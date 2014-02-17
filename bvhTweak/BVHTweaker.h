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

class CBVHTweaker
{
	friend class CBVHFile;

public:

	CBVHTweaker(CBVHFile* BVHFile);
	~CBVHTweaker(void);

	// main funtions 
	void GetCurrentSelOffsets(GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint);// i.e. bones sizes
	void GetCurrentSelTranslations (GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint);// i.e. bone translations in motion data
	void GetCurrentSelRotations(GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint);
	void GetCurrentSelScales(GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint);
	void AddMotion(GLfloat x, GLfloat y, GLfloat z, int mode, CString CurrentlySelectedJoint);
	void ZeroOutChannel(int index, int mode, CString jointName);
	void TrimFile(int LeftIndex, int RightIndex);
	void KnitEnds(int scope);
	void SetTShape();
	void RemoveTShape();
	void SLify();
	void RenameJoints();// SL compatibility
	void RenameJointsAnimeeple();// Animeeple compatibility
	int  ReSizeJoints(); // SL version
	int  ReSizeJointsAnimeeple();// Animeeple version
	void ReSizeJoint(BVHJoint* pJoint, float X, float Y, float Z);
	void SetOffsetChannels(BVHJoint* pJoint, float X, float Y, float Z);
	void ResampleTimeLine(int nNewFrames);
	void RenameJoint();
	void RemoveJoint();
	//void RemoveJoint(BVHJoint* pCurrentJoint);
	void InsertJoint();
	void SwapAbsChestMotion();
	void InterpolateChestAbdomen();
	void AddAbdomen();
	void MotionBuilderHeightFix();
	void SetBlendPose();
	void SquareShoulders(bool up);

	// misc matrix ops
	//void InvertMatrix(GLfloat* fMatrixIn, GLfloat* fMatrixOut);
	//void ResetMatrix(float* Matrix);
	//void MultiplyMatricies(float* mResult, float* m1);

private:

	CBVHFile* m_pBVHFile;
public:	
	void MotionDataAdd(GLfloat xDelta, GLfloat yDelta, GLfloat zDelta, int signX, int signY, int signZ, int index, int channelOrder = XYZ);
	void MotionDataAdd(BVHJoint* pJoint, float X, float Y, float Z, bool bTranslating = false);
	void MotionFrameAdd(BVHJoint* pJoint, float X, float Y, float Z, int nFrameNumber);// just add one keyframe
	float GetDifference(float fParamOne, float fParamTwo);
	void MotionDataSwapChannels(int indexOne, int indexTwo);
	void MotionDatsaInvertChannel(int index);
	void RearrangeMotionChannels(BVHJoint* pJoint, int newOrder);
	void AssignValuesByRotOrder(BVHJoint* pJoint, float* pX, float* pY, float* pZ);// helper fn for RearrangeMotionChannels
	void MotionDataInvertChannel(int nMotionDataIndex);

	void GetRidOfZeros(void);
	// zeros out all rotation and all translation except hips y trans value
	void ZeroOut(void);
	void QualityCheck(void);
	void Scale(bool bScaleUp);
	void RemoveJoint(BVHJoint* pJoint);
	void Normalise(void);
	void LPFilter(void);
	void FixSLLoopBug(bool bAlreadySetUp=false);
	void CentreFigure(void);
	void HalfSample(void);
	void HackArmsAlignmentAxis(void);
	void CombineChestAbdomen();
private:
	void HackArmsAlignmentAxis(BVHJoint* pJoint, float X, float Y, float Z, int signX, int signY, int signZ, int channelOrder);
	void MatrixRotateZ(BVHJoint* pJoint, int sign = 1);
	void DoRotations(BVHMotion* motionData, int nMotionDataIndex, float fXoff, float fYoff, float fZoff);
public:
	void CraneNeck(bool bStraighten);
	void Reverse(void);
	void DeleteDescendants();//BVHJoint* pJoint);
	void ZeroJoint(CString szJointName, int mode);
private:
	//BVHJoint* GetJointByName(CString szJointName);
public:
	void ZeroOutToEnd(CString szJointStart = _T("current"));
private:
	BVHJoint* GetJointByName(CString szJointName);
public:
	void CombineWithParent(void);
};
