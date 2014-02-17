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
 
#include "StdAfx.h"
#include "BVHTweaker.h"
#include "BVHFile.h"
#include "vMath.h"
#include "math.h"
#include "NewJointDlg.h"
#include "QCFile.h"
#include "BVHLPFilter.h"



CBVHTweaker::CBVHTweaker(CBVHFile* BVHFile)
{
	m_pBVHFile = BVHFile;
}

CBVHTweaker::~CBVHTweaker()
{
}

void CBVHTweaker::GetCurrentSelOffsets(GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint)
{
	BVHMotion* currentFrame = m_pBVHFile->GetCurrentMotionFrame();
	BVHJoint* currentJoint = m_pBVHFile->GetJoint(CurrentSelJoint);
	vector<GLfloat>::iterator offsetItr = currentJoint->offsetChannels.begin();

	*Z = *offsetItr++;
	*Y = *offsetItr++;
	*X = *offsetItr;
			
	currentFrame = NULL;
	currentJoint = NULL;
}

void CBVHTweaker::GetCurrentSelTranslations(GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint)
{
	BVHMotion* currentFrame = m_pBVHFile->GetCurrentMotionFrame();
	BVHJoint* currentJoint = m_pBVHFile->GetJoint(CurrentSelJoint);
	vector<GLfloat>::iterator offsetItr = currentJoint->offsetChannels.begin();

	if(currentJoint->offsetChannelOrder!=NONE) {
		
		int nOffsetDataOffset = currentJoint->nOffsetMotionDataIndex;
		*X = currentFrame->data[nOffsetDataOffset+0];
		*Y = currentFrame->data[nOffsetDataOffset+1];
		*Z = currentFrame->data[nOffsetDataOffset+2];

	} else
	{
		*X=0.f;
		*Y=0.f;
		*Z=0.f;
	}

	currentFrame = NULL;
	currentJoint = NULL;
}

void CBVHTweaker::GetCurrentSelScales(GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint)
{
	BVHMotion* currentFrame = m_pBVHFile->GetCurrentMotionFrame();
	BVHJoint* currentJoint = m_pBVHFile->GetJoint(CurrentSelJoint);
	int i = currentJoint->nScaleMotionDataIndex;

	if(currentJoint->isEndSite) {
		*X = 0.0;
		*Y = 0.0;
		*Z = 0.0;
		return;//no scales for an end site
	}	

	*X = currentFrame->data[i++];
	*Y = currentFrame->data[i++];
	*Z = currentFrame->data[i++];
}

void CBVHTweaker::GetCurrentSelRotations(GLfloat* X, GLfloat* Y, GLfloat* Z, CString CurrentSelJoint)
{
	BVHMotion* currentFrame = m_pBVHFile->GetCurrentMotionFrame();
	BVHJoint* currentJoint = m_pBVHFile->GetJoint(CurrentSelJoint);
	int i = currentJoint->nMotionDataIndex;
	if(currentJoint->isEndSite) {
		*X = 0.0;
		*Y = 0.0;
		*Z = 0.0;
		return;//no rots for an end site
	}
	switch(currentJoint->channelOrder) {

		case XYZ: {
			if(currentJoint->isROOT) {

				*X = currentFrame->data[3];
				*Y = currentFrame->data[4];
				*Z = currentFrame->data[5];

			} else {
				*X = currentFrame->data[i++];
				*Y = currentFrame->data[i++];
				*Z = currentFrame->data[i++];
			}
			break;
		}
		case XZY: {
			if(currentJoint->isROOT) {
				*X = currentFrame->data[3];
				*Z = currentFrame->data[4];
				*Y = currentFrame->data[5];
			} else {
				*X = currentFrame->data[i++];
				*Z = currentFrame->data[i++];
				*Y = currentFrame->data[i++];
			}
			break;
		}
		case YZX: {
			if(currentJoint->isROOT) {
				*Y = currentFrame->data[3];
				*Z = currentFrame->data[4];
				*X = currentFrame->data[5];
			} else {
				*Y = currentFrame->data[i++];
				*Z = currentFrame->data[i++];
				*X = currentFrame->data[i++];
			}
			break;
		}
		case ZXY: {
			if(currentJoint->isROOT) {
				*Z = currentFrame->data[3];
				*X = currentFrame->data[4];
				*Y = currentFrame->data[5];
			} else {
				*Z = currentFrame->data[i++];
				*X = currentFrame->data[i++];
				*Y = currentFrame->data[i++];
			}
			break;
		}
		case ZYX: {
			if(currentJoint->isROOT) {
				*Z = currentFrame->data[3];
				*Y = currentFrame->data[4];
				*X = currentFrame->data[5];
			} else {
				*Z = currentFrame->data[i++];
				*Y = currentFrame->data[i++];
				*X = currentFrame->data[i++];
			}
			break;
		}
		case YXZ: {
			if(currentJoint->isROOT) {
				*Y = currentFrame->data[3];
				*X = currentFrame->data[4];
				*Z = currentFrame->data[5];
			} else {
				*Y = currentFrame->data[i++];
				*X = currentFrame->data[i++];
				*Z = currentFrame->data[i++];
			}
			break;
		}
		case NONE: {
			// no motion channels, so rotations are zero
			*X = 0;
			*Y = 0;
			*Z = 0;
			break;
		}
		default: {
			AfxMessageBox(_T("Unsupported joint order! Please report this bug to support@davedub.co.uk"),MB_ICONSTOP);
		}
	}
	currentFrame = NULL;
	currentJoint = NULL;
}
void CBVHTweaker::AddMotion(GLfloat x, GLfloat y, GLfloat z, int mode, CString CurrentlySelectedJoint)
{
	BVHMotion* pFrame = m_pBVHFile->GetCurrentMotionFrame();
	BVHJoint* pJoint = m_pBVHFile->GetJoint(CurrentlySelectedJoint);

	switch(mode) {

		case MODE_TRANSLATING: {
			
			// translate the hips offset in motion data
			//if(pJoint->isROOT) {
			if(pJoint->offsetChannelOrder!=NONE&&pJoint->isROOT) 
				MotionDataAdd(x,y,z,1,1,1,pJoint->nOffsetMotionDataIndex);
			
			else {
				// translate the joint's offset values
				vector<float>::iterator offsetItr = pJoint->offsetChannels.begin();
				*offsetItr+=z;
				offsetItr++;
				*offsetItr+=y;
				offsetItr++;
				*offsetItr+=x;
			}

			// update figure height and arms alignment
			m_pBVHFile->CalculateFigureHeight();
			m_pBVHFile->m_armsAlignment = m_pBVHFile->CalculateArmsAxisAlignment();
			break;
		}
		   
		case MODE_ROTATING: {
				
			if(pJoint->isEndSite)return;

			switch(pJoint->channelOrder) {
				
				case XYZ: {
					if(pJoint->isROOT) MotionDataAdd(x,y,z,1,1,1,3, XYZ);
					else MotionDataAdd(x,y,z,1,1,1,pJoint->nMotionDataIndex, XYZ);
					break;
				}
				case XZY: {
					if(pJoint->isROOT) MotionDataAdd(x,y,z,1,1,1,3, XZY);
					else MotionDataAdd(x,y,z,1,1,1,pJoint->nMotionDataIndex, XZY);
					break;
				}
				case YZX: {
					if(pJoint->isROOT) MotionDataAdd(x,y,z,1,1,1,3, YZX);
					else MotionDataAdd(x,y,z,1,1,1,pJoint->nMotionDataIndex, YZX);
					break;
				}
				case ZXY: {
					if(pJoint->isROOT) MotionDataAdd(x,y,z,1,1,1,3, ZXY);
					else MotionDataAdd(x,y,z,1,1,1,pJoint->nMotionDataIndex, ZXY);
					break;
				}
				case ZYX: {
					if(pJoint->isROOT) MotionDataAdd(x,y,z,1,1,1,3, ZYX);
					else MotionDataAdd(x,y,z,1,1,1,pJoint->nMotionDataIndex, ZYX);
					break;
				}
				case YXZ: {
					if(pJoint->isROOT) MotionDataAdd(x,y,z,1,1,1,3, YXZ);
					else MotionDataAdd(x,y,z,1,1,1,pJoint->nMotionDataIndex, YXZ);
					break;
				}
				//default: AfxMessageBox(_T("Unsupported joint order - please contact dave@davedub.co.uk"),MB_ICONSTOP);
			}
			break;
		}	
	}
	pFrame = NULL;
	pJoint = NULL;
}
// adds values to a single motion frame
void CBVHTweaker::MotionFrameAdd(BVHJoint* pJoint, float X, float Y, float Z, int nFrameNumber)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin()+nFrameNumber;
	int nIndex = pJoint->nMotionDataIndex;
	
	switch(pJoint->channelOrder) 
	{
		case XYZ: {
			motionItr->data[nIndex+0]+=X;
			motionItr->data[nIndex+1]+=Y;
			motionItr->data[nIndex+2]+=Z;
			break;
		}
		case XZY: {
			motionItr->data[nIndex+0]+=X;
			motionItr->data[nIndex+2]+=Y;
			motionItr->data[nIndex+1]+=Z;
			break;
		}
		case YZX: {
			motionItr->data[nIndex+2]+=X;
			motionItr->data[nIndex+0]+=Y;
			motionItr->data[nIndex+1]+=Z;
			break;
		}
		case ZXY: {
			motionItr->data[nIndex+1]+=X;
			motionItr->data[nIndex+2]+=Y;
			motionItr->data[nIndex+0]+=Z;
			break;
		}
		case ZYX: {
			motionItr->data[nIndex+2]+=X;
			motionItr->data[nIndex+1]+=Y;
			motionItr->data[nIndex+0]+=Z;
			break;
		}
		case YXZ: {
			motionItr->data[nIndex+1]+=X;
			motionItr->data[nIndex+0]+=Y;
			motionItr->data[nIndex+2]+=Z;
			break;
		}
		default: AfxMessageBox(_T("Unsupported joint order - please report bug to dave@davedub.co.uk"),MB_ICONSTOP);
	}				
}
//
// Add all values to motion frames. Latest version, old ones shit!
//
void CBVHTweaker::MotionDataAdd(BVHJoint* pJoint, float X, float Y, float Z, bool bTranslating)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	int nIndex = pJoint->nMotionDataIndex;
	int channelOrder = pJoint->channelOrder;
	if(bTranslating) 
	{
		nIndex = pJoint->nOffsetMotionDataIndex;
		channelOrder = XYZ;
	}
	
	while(motionItr!=m_pBVHFile->m_motion.end())
	{
		switch(channelOrder) 
		{
			case XYZ: {
				motionItr->data[nIndex+0]+=X;
				motionItr->data[nIndex+1]+=Y;
				motionItr->data[nIndex+2]+=Z;
				break;
			}
			case XZY: {
				motionItr->data[nIndex+0]+=X;
				motionItr->data[nIndex+2]+=Y;
				motionItr->data[nIndex+1]+=Z;
				break;
			}
			case YZX: {
				motionItr->data[nIndex+2]+=X;
				motionItr->data[nIndex+0]+=Y;
				motionItr->data[nIndex+1]+=Z;
				break;
			}
			case ZXY: {
				motionItr->data[nIndex+1]+=X;
				motionItr->data[nIndex+2]+=Y;
				motionItr->data[nIndex+0]+=Z;
				break;
			}
			case ZYX: {
				motionItr->data[nIndex+2]+=X;
				motionItr->data[nIndex+1]+=Y;
				motionItr->data[nIndex+0]+=Z;
				break;
			}
			case YXZ: {
				motionItr->data[nIndex+1]+=X;
				motionItr->data[nIndex+0]+=Y;
				motionItr->data[nIndex+2]+=Z;
				break;
			}
			default: AfxMessageBox(_T("Unsupported joint order - please report bug to dave@davedub.co.uk"),MB_ICONSTOP);
		}
		motionItr++;
	}
}

// adds values to all motion frames
void CBVHTweaker::MotionDataAdd(GLfloat xDelta, GLfloat yDelta, GLfloat zDelta, int signX, int signY, int signZ, int index, int channelOrder)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	int nFrames = m_pBVHFile->GetNFrames();
	int nParams = m_pBVHFile->GetNParameters();

	for(int i=0;i<nFrames;i++) {
		for(int j=0;j<nParams;j++) {
			if(index==j) {

				switch(channelOrder) {

					case XYZ: {
						// add the deltas to the motion data values
						GLfloat x = motionItr->data[j+0]*signX;
						motionItr->data[j+0] = x+xDelta;
						GLfloat y = motionItr->data[j+1]*signY;
						motionItr->data[j+1] = y+yDelta;
						GLfloat z = motionItr->data[j+2]*signZ;
						motionItr->data[j+2] = z+zDelta;
						break;
					}
					case XZY: {
						// add the deltas to the motion data values
						GLfloat x = motionItr->data[j+0]*signX;
						motionItr->data[j+0] = x+xDelta;
						GLfloat y = motionItr->data[j+2]*signY;
						motionItr->data[j+2] = y+yDelta;
						GLfloat z = motionItr->data[j+1]*signZ;
						motionItr->data[j+1] = z+zDelta;
						break;
					}
					case YZX: {
						// add the deltas to the motion data values
						GLfloat x = motionItr->data[j+2]*signX;
						motionItr->data[j+2] = x+xDelta;
						GLfloat y = motionItr->data[j+0]*signY;
						motionItr->data[j+0] = y+yDelta;
						GLfloat z = motionItr->data[j+1]*signZ;
						motionItr->data[j+1] = z+zDelta;
						break;
					}
					case ZXY: {
						// add the deltas to the motion data values
						GLfloat x = motionItr->data[j+1]*signX;
						motionItr->data[j+1] = x+xDelta;
						GLfloat y = motionItr->data[j+2]*signY;
						motionItr->data[j+2] = y+yDelta;
						GLfloat z = motionItr->data[j+0]*signZ;
						motionItr->data[j+0] = z+zDelta;
						break;
					}
					case ZYX: {
						// add the deltas to the motion data values
						GLfloat x = motionItr->data[j+2]*signX;
						motionItr->data[j+2] = x+xDelta;
						GLfloat y = motionItr->data[j+1]*signY;
						motionItr->data[j+1] = y+yDelta;
						GLfloat z = motionItr->data[j+0]*signZ;
						motionItr->data[j+0] = z+zDelta;
						break;
					}
					case YXZ: {
						// add the deltas to the motion data values
						GLfloat x = motionItr->data[j+1]*signX;
						motionItr->data[j+1] = x+xDelta;
						GLfloat y = motionItr->data[j+0]*signY;
						motionItr->data[j+0] = y+yDelta;
						GLfloat z = motionItr->data[j+2]*signZ;
						motionItr->data[j+2] = z+zDelta;
						break;
					}
					default: AfxMessageBox(_T("Unsupported joint order - please report bug to support@davedub.co.uk"),MB_ICONSTOP);
				}
				break;
			}					
		}
		motionItr++;
	}
}

void CBVHTweaker::ZeroOutChannel(int index, int mode, CString jointName)
{
	BVHJoint* pJoint = GetJointByName(jointName); //m_pBVHFile->GetJoint(jointName);
	int offset = 0;
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	
	if((mode&MODE_TRANSLATING)>=MODE_TRANSLATING) {
			
		// translate the hips
		if(pJoint->offsetChannelOrder!=NONE) {
			
			offset = index+pJoint->nOffsetMotionDataIndex;
			for(int i=0;i<m_pBVHFile->GetNFrames();i++) {
				for(int i=0;i<m_pBVHFile->GetNParameters();i++) {
					if(offset==i)motionItr->data[offset] = 0.0;
				}
				motionItr++;
			}
		}
		else {
			pJoint->offsetChannels[N_Z-index] = 0;
		}
	}
	else if((mode&MODE_ROTATING)>=MODE_ROTATING) {

		switch(pJoint->channelOrder) {

			case XYZ: {
				switch(index) {
					case N_X: {
						offset = pJoint->nMotionDataIndex + index;
						break;
					}
					case N_Y: {
						offset = pJoint->nMotionDataIndex + index;
						break;
					}
					case N_Z: {
						offset = pJoint->nMotionDataIndex + index;
						break;
					}
					break;
				}
				break;
			}
			case XZY: {
				switch(index) {
					case N_X: {
						offset = pJoint->nMotionDataIndex + 0;
						break;
					}
					case N_Y: {
						offset = pJoint->nMotionDataIndex + 2;
						break;
					}
					case N_Z: {
						offset = pJoint->nMotionDataIndex + 1;
						break;
					}
					break;
				}
				break;
			}
			case YZX: {
				switch(index) {
					case N_X: {
						offset = pJoint->nMotionDataIndex + 2;
						break;
					}
					case N_Y: {
						offset = pJoint->nMotionDataIndex + 0;
						break;
					}
					case N_Z: {
						offset = pJoint->nMotionDataIndex + 1;
						break;
					}
					break;
				}
				break;
			}
			case ZXY: {
				switch(index) {
					case N_X: {
						offset = pJoint->nMotionDataIndex + 1;
						break;
					}
					case N_Y: {
						offset = pJoint->nMotionDataIndex + 2;
						break;
					}
					case N_Z: {
						offset = pJoint->nMotionDataIndex + 0;
						break;
					}
					break;
				}
				break;
			}
			case ZYX: {
				switch(index) {
					case N_X: {
						offset = pJoint->nMotionDataIndex + 2;
						break;
					}
					case N_Y: {
						offset = pJoint->nMotionDataIndex + 1;
						break;
					}
					case N_Z: {
						offset = pJoint->nMotionDataIndex + 0;
						break;
					}
					break;
				}
				break;
			}
			case YXZ: {
				switch(index) {
					case N_X: {
						offset = pJoint->nMotionDataIndex + 1;
						break;
					}
					case N_Y: {
						offset = pJoint->nMotionDataIndex + 0;
						break;
					}
					case N_Z: {
						offset = pJoint->nMotionDataIndex + 2;
						break;
					}
					break;
				}
				break;
			}
			case NONE: {
				// nothing to set!
				pJoint = NULL;
				return;
			}
			default: {
				AfxMessageBox(_T("Unsupported joint order - please report bug to support@davedub.co.uk"),MB_ICONSTOP);
			}
		}
		for(int i=0;i<m_pBVHFile->GetNFrames();i++) {
			for(int i=0;i<m_pBVHFile->GetNParameters();i++) {
				if(offset==i){
					motionItr->data[offset] = 0.0;
				}
			}
			motionItr++;
		}
	}
	pJoint = NULL;
}

void CBVHTweaker::TrimFile(int LeftIndex, int RightIndex)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();

	int nFrames = m_pBVHFile->m_nFrames;
	for(int i=0;i<nFrames;i++) {
		if(i<LeftIndex) {
			motionItr = m_pBVHFile->m_motion.erase(motionItr);
		}
		else if(i>RightIndex) {
			if(i==nFrames-1) {
				m_pBVHFile->m_motion.erase(motionItr);
			} else {
				motionItr = m_pBVHFile->m_motion.erase(motionItr);
			}
		} else motionItr++;
	}
	m_pBVHFile->m_nFrames = 1 + RightIndex - LeftIndex;
}

void CBVHTweaker::KnitEnds(int scope)
{
	//handle rotations
	float fScope = (float)scope;
	int nParams = m_pBVHFile->GetNParameters();
	int nFrames = m_pBVHFile->GetNFrames();
	for(int i=3;i<nParams;i++) {
		
		// first build up a vector of the motion values for parameter i
		vector<float> motionColumn;
		vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
		while(motionItr!=m_pBVHFile->m_motion.end()) {
			
			float motionDataValue = motionItr->data[i];
			motionColumn.push_back(motionDataValue);
			motionItr++;
		}

		float fStartVal = motionColumn[0];
		float fEndVal = motionColumn[nFrames-1];
		float fDifference = GetDifference(fStartVal, fEndVal);

		// Sanity check
		float fNewStartValue = fStartVal + fDifference/2;
		float fNewEndValue = fEndVal - fDifference/2;
		int test = (int)fNewStartValue-(int)fNewEndValue;
		//ASSERT(test==0);
		if(test!=0) {
			int stop = 0;
		}

		// then adjust the figures at the beginning of the column
		vector<float>::iterator columnItr = motionColumn.begin();
		float fAdjusterRatio = 0;
		float fFinalAdjustment = 0;
		for(float k=1;k<fScope;k++) {//k=1 to remove fix identical first and last frames bug
			fAdjusterRatio = (fScope-k)/fScope/2;
			fFinalAdjustment = fDifference*fAdjusterRatio;
			*columnItr+=fFinalAdjustment;
			columnItr++;
		}
		// then the end of the column
		columnItr = motionColumn.end(); 
		columnItr--;
		for(float j=fScope;j>0;j--) {
			fAdjusterRatio = j/fScope/2;
			fFinalAdjustment = -fDifference*fAdjusterRatio;
			*columnItr+=fFinalAdjustment;
			columnItr--;
		}

		// now copy the changed bits of the column back into the file's motion section
		motionItr = m_pBVHFile->m_motion.begin(); //Front
		columnItr = motionColumn.begin();
		for(float m=0;m<fScope;m++) {
			motionItr->data[i] = *columnItr;
			motionItr++;
			columnItr++;
		}
		motionItr = m_pBVHFile->m_motion.end();  //and back
		columnItr = motionColumn.end();
		columnItr--;
		motionItr--;
		for(float n=((float)nFrames);n>((float)nFrames-fScope);n--) {
			motionItr->data[i] = *columnItr;
			motionItr--;
			columnItr--;
		}
	}

	//handle translations - use entire length for scope
	fScope = (float)m_pBVHFile->GetNFrames()-1;//nParams-1;
	//nParams = m_pBVHFile->GetNParameters();
	//nFrames = m_pBVHFile->GetNFrames();
	for(int i=0;i<3;i++) {
		
		// first build up a vector of the motion values for parameter i
		vector<float> motionColumn;
		vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
		while(motionItr!=m_pBVHFile->m_motion.end()) {
			
			float motionDataValue = motionItr->data[i];
			motionColumn.push_back(motionDataValue);
			motionItr++;
		}

		float fStartVal = motionColumn[0];
		float fEndVal = motionColumn[nFrames-1];
		float fDifference = GetDifference(fStartVal, fEndVal);

		// Sanity check
		float fNewStartValue = fStartVal + fDifference/2;
		float fNewEndValue = fEndVal - fDifference/2;
		int test = (int)fNewStartValue-(int)fNewEndValue;
		//ASSERT(test==0);
		if(test!=0) {
			int stop = 0;
		}

		// then adjust the figures at the beginning of the column
		vector<float>::iterator columnItr = motionColumn.begin();
		float fAdjusterRatio = 0;
		float fFinalAdjustment = 0;
		for(float k=1;k<fScope;k++) {//k=1 to remove fix identical first and last frames bug
			fAdjusterRatio = (fScope-k)/fScope/2;
			fFinalAdjustment = fDifference*fAdjusterRatio;
			*columnItr+=fFinalAdjustment;
			columnItr++;
		}
		// then the end of the column
		columnItr = motionColumn.end(); 
		columnItr--; 
		for(float j=fScope;j>0;j--) {
			fAdjusterRatio = j/fScope/2;
			fFinalAdjustment = -fDifference*fAdjusterRatio;
			*columnItr+=fFinalAdjustment;
			columnItr--;
		}

		// now copy the changed bits of the column back into the file's motion section
		motionItr = m_pBVHFile->m_motion.begin(); //Front
		columnItr = motionColumn.begin();
		for(float m=0;m<fScope;m++) {
			motionItr->data[i] = *columnItr;
			motionItr++;
			columnItr++;
		}
		motionItr = m_pBVHFile->m_motion.end();  //and back
		columnItr = motionColumn.end();
		columnItr--;
		motionItr--;
		for(float n=((float)nFrames);n>((float)nFrames-fScope);n--) {
			motionItr->data[i] = *columnItr;
			motionItr--;
			columnItr--;
		}
	}
}

float CBVHTweaker::GetDifference(float fParamOne, float fParamTwo)
{
	float fDifference = 0;

	if(fParamTwo<fParamOne) {

		if((fParamOne>=0)&&(fParamTwo>=0)) {
			fDifference = fParamTwo - fParamOne;}
		if((fParamOne>=0)&&(fParamTwo<0)) {
			if((fParamOne>=90.0)&&(fParamTwo<-90.00)){
				//'switch' condition
				fParamTwo+=180;
				fParamOne=180-fParamOne;
			}
			fDifference = -1*(fParamOne + fabs(fParamTwo));}
		if((fParamOne<0)&&(fParamTwo<0)) {
			fDifference = fParamTwo + fabs(fParamOne);}
		//if(fabs(fDifference)>179.8)__asm int 3;

	} else if(fParamTwo>fParamOne) {

		if((fParamOne>=0)&&(fParamTwo>=0)) {
			fDifference = fParamTwo - fParamOne;}
		if((fParamOne<0)&&(fParamTwo>=0)) {
			if((fParamTwo>=90.0)&&(fParamOne<-90.00)){
				//'switch' condition
				fParamOne+=180;
				fParamTwo=180-fParamTwo;
			}
			fDifference = fParamTwo + fabs(fParamOne);}
		if((fParamOne<0)&&(fParamTwo<0)) {
			fDifference = fParamTwo + fabs(fParamOne);}
		if(fabs(fDifference)>20){
			//flawfinder - ignore
			int stop = TRUE;
		}

	} else fDifference = 0;	

	return fDifference;
}

void CBVHTweaker::SetTShape()
{
	// create a new line with all zeros except hip height
	int nParameters = m_pBVHFile->m_nParameters;
	BVHMotion newMotionLine;

	for(int i=0;i<nParameters;i++) {
		
		switch(i) {
			case 1: {
				newMotionLine.data.push_back(m_pBVHFile->m_fGroundHeight);
				break;
			}
			default: {
				newMotionLine.data.push_back(0.f);
				break;
			}
		}
	}

	// add it to the motion data
	m_pBVHFile->m_motion.insert(m_pBVHFile->m_motion.begin(),newMotionLine);
	m_pBVHFile->m_nFrames++;
	m_pBVHFile->SetCurrentMotionFrame(0);

	// nice try, but no cigar...
	//if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS) HackArmsAlignmentAxis();
}

void CBVHTweaker::RemoveTShape()
{
	if(m_pBVHFile->GetNFrames()>1)
	{
		vector<BVHMotion>::iterator itr = m_pBVHFile->m_motion.begin();
		m_pBVHFile->m_motion.erase(itr);
		m_pBVHFile->m_nFrames--;
		m_pBVHFile->SetCurrentMotionFrame(0);
	}
}

void CBVHTweaker::RenameJoints(void)
{
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = _T("");
	int nChestIndex = 0; // for error checking when assigning collars / shoulders / clavicles
	vector<CString> unaffectedJoints;
	int nJointsAffected = 0;
	bool bJointAffected = false;

	while(jointItr!=m_pBVHFile->m_joints.end()) {

		szJointName = jointItr->JOINTName.GetBuffer();

		// torso
		if((szJointName==_T("Hips"))||(szJointName==_T("Hip"))||(szJointName == _T("hip"))||(szJointName == _T("hips"))) {
			szJointName = _T("hip");
			nJointsAffected++;
			bJointAffected = true;
		}

		if(szJointName==_T("Abdomen")||szJointName==_T("Spine")||szJointName==_T("spine")||szJointName==_T("abdomen")
		   ||szJointName==_T("Waist")||szJointName==_T("Torso")) {
			szJointName = _T("abdomen");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("Chest2"))||szJointName==_T("Spine1")||szJointName==_T("spine1")||szJointName==_T("Spine2")
			||szJointName==_T("spine2")||(szJointName==_T("chest2"))||(szJointName==_T("Spine1"))||(szJointName==_T("Torso2"))
		    ||(szJointName == _T("chest"))||(szJointName==_T("Chest"))) {
			szJointName = _T("chest");
			nChestIndex = jointItr->index;
			nJointsAffected++;
			bJointAffected = true;
		}

		// legs
		if((szJointName==_T("LeftHip"))||(szJointName==_T("LeftUpLeg"))||(szJointName == _T("lThigh"))
			||(szJointName==_T("LeftUpperLeg"))) {
			szJointName = _T("lThigh");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftKnee"))||(szJointName==_T("LeftLowLeg"))||(szJointName==_T("LeftLeg"))
			||(szJointName == _T("lShin"))||(szJointName==_T("LeftLowerLeg"))) {
			szJointName = _T("lShin");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftAnkle"))||(szJointName==_T("LeftFoot"))||(szJointName == _T("lFoot"))) {
			szJointName = _T("lFoot");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightHip"))||(szJointName==_T("RightUpLeg"))||(szJointName == _T("rThigh"))
			||(szJointName==_T("RightUpperLeg"))) {
			szJointName = _T("rThigh");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightKnee"))||(szJointName==_T("RightLowLeg"))||(szJointName==_T("RightLeg"))
			||(szJointName == _T("rShin"))||(szJointName==_T("RightLowerLeg"))) {
			szJointName = _T("rShin");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightAnkle"))||(szJointName==_T("RightFoot"))||(szJointName == _T("rFoot"))) {
			szJointName = _T("rFoot");
			nJointsAffected++;
			bJointAffected = true;
		}
		
		// left arm clavicle
		if(szJointName==_T("LeftCollar")||szJointName == _T("lCollar")||szJointName==_T("LeftShoulder")
			||szJointName==_T("LeftCollar") ||szJointName == _T("lColr")) { // due to bug in Poser 6
			  if(jointItr->parentIndex==nChestIndex) 
			  {
				  szJointName = _T("lCollar");// ty animazoo for complicating that!
					nJointsAffected++;
					bJointAffected = true;
			  }
		}

		// left upper arm
		if(szJointName==_T("LeftUpArm")||szJointName==_T("LeftArm")||szJointName == _T("lShldr")
			||szJointName==_T("LeftShoulder")||szJointName==_T("LeftUpperArm")) {
			szJointName = _T("lShldr");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftElbow"))||(szJointName==_T("LeftLowArm"))||(szJointName==_T("LeftForeArm"))
			||(szJointName == _T("lForeArm"))||(szJointName==_T("LeftLowerArm"))) {
			szJointName = _T("lForeArm");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftWrist"))||(szJointName==_T("LeftHand"))||(szJointName == _T("lHand"))) {
			szJointName = _T("lHand");
			nJointsAffected++;
			bJointAffected = true;
		}

		// right arm clavicle
		if(szJointName==_T("RightCollar")||szJointName == _T("rCollar")||szJointName==_T("RightShoulder")
			||szJointName==_T("RightCollar")) {
			if(jointItr->parentIndex==nChestIndex) 
			{
				szJointName = _T("rCollar");// ty animazoo for complicating that!
				nJointsAffected++;
				bJointAffected = true;
			}
		}

		// right upper arm
		if(szJointName==_T("RightUpArm")||szJointName==_T("RightArm")||szJointName==_T("rShldr")
			||szJointName==_T("RightShoulder")||szJointName==_T("RightUpperArm")) {
			szJointName = _T("rShldr");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightElbow"))||(szJointName==_T("RightLowArm"))||(szJointName==_T("RightForeArm"))
			||(szJointName == _T("rForeArm"))||(szJointName==_T("RightLowerArm"))) {
			szJointName = _T("rForeArm");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightWrist"))||(szJointName==_T("RightHand"))||(szJointName==_T("rHand"))) {
			szJointName = _T("rHand");
			nJointsAffected++;
			bJointAffected = true;
		}

		// the neck up
		if(szJointName==_T("Neck")||(szJointName==_T("neck"))) {
			szJointName = _T("neck");
			nJointsAffected++;
			bJointAffected = true;
		}

		if(szJointName==_T("Head")|(szJointName==_T("head"))) {
			szJointName = _T("head");
			nJointsAffected++;
			bJointAffected = true;
		}

		if(!bJointAffected&&szJointName.Compare("End Site")) unaffectedJoints.push_back(szJointName);
		bJointAffected=false;
		
		// update the joint name
		jointItr->JOINTName = szJointName.GetBuffer();
		jointItr->internalName = szJointName.GetBuffer();
		jointItr++;
	}


	// notify user of result
	int nJointsNotCountingEndSites = m_pBVHFile->GetNJoints()-5;// we don't rename end sites - but are we sure there's 5 end sites?
	if(nJointsAffected==nJointsNotCountingEndSites)
	{
		char szTempString[1024]; 
		sprintf(szTempString, "All %d joints are correctly named", m_pBVHFile->GetNJoints()); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
		sprintf(szTempString,"");
		*szTempString = NULL;
	}
	else if(nJointsAffected==19)
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[1024]; 
		sprintf(szTempString, "%d joints in the file now have correct names\n\nThe following joint(s) will be ignored by SL and should probably be deleted:\n%s",(nJointsAffected+5),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
		sprintf(szTempString,"");
		*szTempString = NULL;
	}
	else
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[1024]; 
		sprintf(szTempString, "%d of %d joints in the file now have correct names\n\nThe following joint(s) need your attention:\n%s",(nJointsAffected+5),m_pBVHFile->GetNJoints(),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
		sprintf(szTempString,"");
		*szTempString = NULL;
	}
}

void CBVHTweaker::RenameJointsAnimeeple()
{
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = _T("");
	int nChestIndex = 0; // for error checking when assigning collars / shoulders / clavicles
	int nAbdomenIndex = 0;
	vector<CString> unaffectedJoints;
	int nJointsAffected = 0;
	bool bJointAffected = false;

	while(jointItr!=m_pBVHFile->m_joints.end()) {

		szJointName = jointItr->JOINTName.GetBuffer();

		// torso
		if((szJointName==_T("Hips"))||(szJointName==_T("Hip"))||(szJointName == _T("hip"))||(szJointName == _T("hips"))) {
			szJointName = _T("Hips");
			nJointsAffected++;
			bJointAffected = true;
		}

		if(szJointName==_T("Abdomen")||szJointName==_T("Spine")||szJointName==_T("abdomen")||szJointName==_T("Waist")) {
			szJointName = _T("Torso");
			nAbdomenIndex = jointItr->index;
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("Chest2"))||(szJointName==_T("chest2"))||(szJointName==_T("Spine1"))
		 ||(szJointName == _T("chest"))||(szJointName==_T("Chest"))) {
			szJointName = _T("Torso2");
			nChestIndex = jointItr->index;
			nJointsAffected++;
			bJointAffected = true;
		}

		// legs
		if((szJointName==_T("LeftHip"))||(szJointName==_T("LeftUpLeg"))||(szJointName == _T("lThigh"))) {
			szJointName = _T("LeftUpperLeg");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftKnee"))||(szJointName==_T("LeftLowLeg"))||(szJointName==_T("LeftLeg"))||(szJointName == _T("lShin"))) {
			szJointName = _T("LeftLowerLeg");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftAnkle"))||(szJointName==_T("LeftFoot"))||(szJointName == _T("lFoot"))) {
			szJointName = _T("LeftFoot");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightHip"))||(szJointName==_T("RightUpLeg"))||(szJointName == _T("rThigh"))) {
			szJointName = _T("RightUpperLeg");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightKnee"))||(szJointName==_T("RightLowLeg"))||(szJointName==_T("RightLeg"))||(szJointName == _T("rShin"))) {
			szJointName = _T("RightLowerLeg");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightAnkle"))||(szJointName==_T("RightFoot"))||(szJointName == _T("rFoot"))) {
			szJointName = _T("RightFoot");
			nJointsAffected++;
			bJointAffected = true;
		}
		
		// left arm clavicle
		if(szJointName==_T("LeftCollar")||szJointName == _T("lCollar")||szJointName==_T("LeftShoulder")
		  ||szJointName == _T("lColr")) { // due to bug in Poser 6
			  if(jointItr->parentIndex==nChestIndex||jointItr->parentIndex==nAbdomenIndex) 
			  {
				  szJointName = _T("LeftCollar");// ty animazoo for complicating that!
					nJointsAffected++;
					bJointAffected = true;
			  }
		}

		// left upper arm
		if(szJointName==_T("LeftUpArm")||szJointName==_T("LeftArm")||szJointName == _T("lShldr")||szJointName==_T("LeftShoulder")) {
			szJointName = _T("LeftUpperArm");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftElbow"))||(szJointName==_T("LeftLowArm"))||(szJointName==_T("LeftForeArm"))||(szJointName == _T("lForeArm"))) {
			szJointName = _T("LeftLowerArm");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("LeftWrist"))||(szJointName==_T("LeftHand"))||(szJointName == _T("lHand"))) {
			szJointName = _T("LeftHand");
			nJointsAffected++;
			bJointAffected = true;
		}

		// right arm clavicle
		if(szJointName==_T("RightCollar")||szJointName == _T("rCollar")||szJointName==_T("RightShoulder")) {
			if(jointItr->parentIndex==nChestIndex||jointItr->parentIndex==nAbdomenIndex) 
			{
				szJointName = _T("RightCollar");// ty animazoo for complicating that!
				nJointsAffected++;
				bJointAffected = true;
			}
		}

		// right upper arm
		if(szJointName==_T("RightUpArm")||szJointName==_T("RightArm")||szJointName==_T("rShldr")||szJointName==_T("RightShoulder")) {
			szJointName = _T("RightUpperArm");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightElbow"))||(szJointName==_T("RightLowArm"))||(szJointName==_T("RightForeArm"))||(szJointName == _T("rForeArm"))) {
			szJointName = _T("RightLowerArm");
			nJointsAffected++;
			bJointAffected = true;
		}

		if((szJointName==_T("RightWrist"))||(szJointName==_T("RightHand"))||(szJointName==_T("rHand"))) {
			szJointName = _T("RightHand");
			nJointsAffected++;
			bJointAffected = true;
		}

		// the neck up
		if(szJointName==_T("Neck")||(szJointName==_T("neck"))) {
			szJointName = _T("Neck");
			nJointsAffected++;
			bJointAffected = true;
		}

		if(szJointName==_T("Head")|(szJointName==_T("head"))) {
			szJointName = _T("Head");
			nJointsAffected++;
			bJointAffected = true;
		}

		if(!bJointAffected&&szJointName.Compare("End Site")) unaffectedJoints.push_back(szJointName);
		bJointAffected=false;
		
		// update the joint name
		jointItr->JOINTName = szJointName.GetBuffer();
		jointItr++;
	}

	int nJointsNotCountingEndSites = m_pBVHFile->GetNJoints()-5;// we don't rename end sites
	if(nJointsAffected==nJointsNotCountingEndSites)
	{
		char szTempString[256]; 
		sprintf(szTempString, "All %d joints are correctly named", m_pBVHFile->GetNJoints()); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}
	else if(nJointsAffected==19)
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[256]; 
		sprintf(szTempString, "%d joints in the file now have correct names\n\nThe following joint(s) will be ignored and should probably be deleted:\n%s",(nJointsAffected+5),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}
	else
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[256]; 
		sprintf(szTempString, "%d of %d joints in the file now have correct names\n\nThe following joint(s) need your attention:\n%s",(nJointsAffected+5),m_pBVHFile->GetNJoints(),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}
}

// resize joint taking note of channel rotation order
void CBVHTweaker::ReSizeJoint(BVHJoint* pJoint, float X, float Y, float Z)
{
	float* pf_large_dimension_new = NULL;
	float* pf_medium_dimension_new = NULL;
	float* pf_small_dimension_new = NULL;
	float* pf_large_dimension_old = NULL;
	float* pf_medium_dimension_old = NULL;
	float* pf_small_dimension_old = NULL;

	// first, order the new dimensions
	if(abs(X)>abs(Y)&&abs(X)>abs(Z)) pf_large_dimension_new = &X;
	if(abs(Y)>abs(X)&&abs(Y)>abs(Z)) pf_large_dimension_new = &Y;
	if(abs(Z)>abs(Y)&&abs(Z)>abs(X)) pf_large_dimension_new = &Z;
	if(pf_large_dimension_new==NULL)
	{
		pf_large_dimension_new = &X;// all the same then!
	}

	if(abs(X)>abs(Y)&&abs(X)<abs(Z)) pf_medium_dimension_new = &X;
	if(abs(X)>abs(Z)&&abs(X)<abs(Y)) pf_medium_dimension_new = &X;

	if(abs(Y)>abs(X)&&abs(Y)<abs(Z)) pf_medium_dimension_new = &Y;
	if(abs(Y)>abs(Z)&&abs(Y)<abs(X)) pf_medium_dimension_new = &Y;	

	if(abs(Z)>abs(Y)&&abs(Z)<abs(X)) pf_medium_dimension_new = &Z;
	if(abs(Z)>abs(X)&&abs(Z)<abs(Y)) pf_medium_dimension_new = &Z;
	if(pf_medium_dimension_new==NULL) 
	{
		if      (&X!=pf_large_dimension_new) pf_medium_dimension_new = &X;
		else if (&Y!=pf_large_dimension_new) pf_medium_dimension_new = &Y;
		else if (&Z!=pf_large_dimension_new) pf_medium_dimension_new = &Z;
	}

	if(abs(X)<abs(Y)&&abs(X)<abs(Z)) pf_small_dimension_new = &X;
	if(abs(Y)<abs(X)&&abs(Y)<abs(Z)) pf_small_dimension_new = &Y;
	if(abs(Z)<abs(Y)&&abs(Z)<abs(X)) pf_small_dimension_new = &Z;
	if(pf_small_dimension_new==NULL) 
	{
		if      (&X!=pf_large_dimension_new&&
			     &X!=pf_medium_dimension_new) pf_small_dimension_new = &X;
		else if (&Y!=pf_large_dimension_new&&
			     &Y!=pf_medium_dimension_new) pf_small_dimension_new = &Y;
		else if (&Z!=pf_large_dimension_new&&
			     &Z!=pf_medium_dimension_new) pf_small_dimension_new = &Z;
	}


	// now, order the old dimensions
	if(abs(pJoint->offsetChannels[0])>abs(pJoint->offsetChannels[1])&&
	   abs(pJoint->offsetChannels[0])>abs(pJoint->offsetChannels[2])) pf_large_dimension_old = &pJoint->offsetChannels[0];
	if(abs(pJoint->offsetChannels[1])>abs(pJoint->offsetChannels[0])&&
	   abs(pJoint->offsetChannels[1])>abs(pJoint->offsetChannels[2])) pf_large_dimension_old = &pJoint->offsetChannels[1];
	if(abs(pJoint->offsetChannels[2])>abs(pJoint->offsetChannels[0])&&
	   abs(pJoint->offsetChannels[2])>abs(pJoint->offsetChannels[1])) pf_large_dimension_old = &pJoint->offsetChannels[2];
	if(pf_large_dimension_old==NULL)
	{
		pf_large_dimension_old = &pJoint->offsetChannels[0];// all the same then!
	}

	if(abs(pJoint->offsetChannels[0])>abs(pJoint->offsetChannels[1])&&
	   abs(pJoint->offsetChannels[0])<abs(pJoint->offsetChannels[2])) pf_medium_dimension_old = &pJoint->offsetChannels[0];
	if(abs(pJoint->offsetChannels[0])>abs(pJoint->offsetChannels[2])&&
	   abs(pJoint->offsetChannels[0])<abs(pJoint->offsetChannels[1])) pf_medium_dimension_old = &pJoint->offsetChannels[0];

	if(abs(pJoint->offsetChannels[1])>abs(pJoint->offsetChannels[0])&&
	   abs(pJoint->offsetChannels[1])<abs(pJoint->offsetChannels[2])) pf_medium_dimension_old = &pJoint->offsetChannels[1];
	if(abs(pJoint->offsetChannels[1])>abs(pJoint->offsetChannels[2])&&
	   abs(pJoint->offsetChannels[1])<abs(pJoint->offsetChannels[0])) pf_medium_dimension_old = &pJoint->offsetChannels[1];

	if(abs(pJoint->offsetChannels[2])>abs(pJoint->offsetChannels[0])&&
	   abs(pJoint->offsetChannels[2])<abs(pJoint->offsetChannels[1])) pf_medium_dimension_old = &pJoint->offsetChannels[2];
	if(abs(pJoint->offsetChannels[2])>abs(pJoint->offsetChannels[1])&&
	   abs(pJoint->offsetChannels[2])<abs(pJoint->offsetChannels[0])) pf_medium_dimension_old = &pJoint->offsetChannels[2];
	if(pf_medium_dimension_old==NULL) 
	{
		if      (&pJoint->offsetChannels[0]!=pf_large_dimension_old) pf_medium_dimension_old = &pJoint->offsetChannels[0];
		else if (&pJoint->offsetChannels[1]!=pf_large_dimension_old) pf_medium_dimension_old = &pJoint->offsetChannels[1];
		else if (&pJoint->offsetChannels[2]!=pf_large_dimension_old) pf_medium_dimension_old = &pJoint->offsetChannels[2];
	}

	if(abs(pJoint->offsetChannels[0])<abs(pJoint->offsetChannels[1])&&
	   abs(pJoint->offsetChannels[0])<abs(pJoint->offsetChannels[2])) pf_small_dimension_old = &pJoint->offsetChannels[0];
	if(abs(pJoint->offsetChannels[1])<abs(pJoint->offsetChannels[0])&&
	   abs(pJoint->offsetChannels[1])<abs(pJoint->offsetChannels[2])) pf_small_dimension_old = &pJoint->offsetChannels[1];
	if(abs(pJoint->offsetChannels[2])<abs(pJoint->offsetChannels[0])&&
	   abs(pJoint->offsetChannels[2])<abs(pJoint->offsetChannels[1])) pf_small_dimension_old = &pJoint->offsetChannels[2];
	if(pf_small_dimension_old==NULL) 
	{
		if      (&pJoint->offsetChannels[0]!=pf_large_dimension_old&&
			     &pJoint->offsetChannels[0]!=pf_medium_dimension_old) pf_small_dimension_old = &pJoint->offsetChannels[0];
		else if (&pJoint->offsetChannels[1]!=pf_large_dimension_old&&
			     &pJoint->offsetChannels[1]!=pf_medium_dimension_old) pf_small_dimension_old = &pJoint->offsetChannels[1];
		else if (&pJoint->offsetChannels[2]!=pf_large_dimension_old&&
			     &pJoint->offsetChannels[2]!=pf_medium_dimension_old) pf_small_dimension_old = &pJoint->offsetChannels[2];
	}


	// now assign the correct values
	*pf_large_dimension_old = *pf_large_dimension_new;
	*pf_medium_dimension_old = *pf_medium_dimension_new;
	*pf_small_dimension_old = *pf_small_dimension_new;
}

void CBVHTweaker::SetOffsetChannels(BVHJoint* pJoint, float X, float Y, float Z)
{
	pJoint->offsetChannels[0]=Z;
	pJoint->offsetChannels[1]=Y;
	pJoint->offsetChannels[2]=X;
}

int  CBVHTweaker::ReSizeJoints(void)
{
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = jointItr->JOINTName.GetBuffer();
	vector<CString> unaffectedJoints;
	int nJointsAffected = 0;
	bool bJointAffected = false;

	while(jointItr!=m_pBVHFile->m_joints.end()) 
	{
		bool bAllValuesSame = false;
		if(jointItr->offsetChannels[0]==jointItr->offsetChannels[1]&&jointItr->offsetChannels[0]==jointItr->offsetChannels[2]) 
			bAllValuesSame = true;

		if(szJointName == _T("hip")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("abdomen")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 3.402643f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("chest")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 8.438565f, -0.680529f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lThigh"))
		{
			SetOffsetChannels(&(*jointItr), 4.98375f, -1.633274f, 1.361059f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lShin")) 
		{
			SetOffsetChannels(&(*jointItr), -2.041588f, -20.007560f, 0.0f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lFoot")) 
		{
			SetOffsetChannels(&(*jointItr), 0.0f, -19.190927f, -1.224953f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			SetOffsetChannels(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			//ReSizeJoint(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			jointItr--;		
		}
		if(szJointName == _T("rThigh")) 
		{
			SetOffsetChannels(&(*jointItr), -4.98375f, -1.633274f, 1.361059f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rShin")) 
		{
			SetOffsetChannels(&(*jointItr), 2.041588f, -20.007560f, 0.0f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rFoot")) 
		{
			SetOffsetChannels(&(*jointItr), 0.0f, -19.190927f, -1.224953f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			SetOffsetChannels(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			//ReSizeJoint(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			jointItr--;		
		}
		if(szJointName == _T("lCollar")) 
		{
			SetOffsetChannels(&(*jointItr), 3.23829f, 6.669187f, -0.816635f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lShldr")) 
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), 2.51505f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), 2.51505f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lForeArm"))
		{
			if(m_pBVHFile->CalculateArmsAxisAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -11.5177f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), 11.5177f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lHand")) 
		{
			if(m_pBVHFile->CalculateArmsAxisAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -9.0267f, 0.f, 0.f);		
			else SetOffsetChannels(&(*jointItr), 9.0267f, 0.f, 0.f);		
			// deal with end site now
			jointItr++; 
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -4.02847f, 0.0f, 0.f);
			else SetOffsetChannels(&(*jointItr), 4.02847f, 0.0f, 0.f);
			jointItr--;

			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rCollar")) 
		{
			SetOffsetChannels(&(*jointItr), -3.23829f, 6.669187f, -0.816635f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rShldr")) 
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -2.51505f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), -2.51505f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rForeArm"))
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -11.5177f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), -11.5177f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rHand")) 
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -9.0267f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), -9.0267f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -4.02847f, 0.0f, 0.f);
			else SetOffsetChannels(&(*jointItr), -4.02847f, 0.0f, 0.f);
			jointItr--;		
		}
		if(szJointName == _T("neck")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 9.8854f, -0.272212f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("head")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 3.130431f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			SetOffsetChannels(&(*jointItr), 0.f, 3.03925f, 0.f);
			jointItr--;		
		}

		if(!bJointAffected&&szJointName.Compare("End Site")) unaffectedJoints.push_back(szJointName);
		bJointAffected=false;

		jointItr++;
		if(jointItr!=m_pBVHFile->m_joints.end()) szJointName = jointItr->JOINTName.GetBuffer();
	}

	int nJointsNotCountingEndSites = m_pBVHFile->GetNJoints()-5;// we don't rename end sites
	if(nJointsAffected==nJointsNotCountingEndSites)
	{
		char szTempString[256]; 
		sprintf(szTempString, "All %d joints have been retargetted", m_pBVHFile->GetNJoints()); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}
	else if(nJointsAffected==19)
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[256]; 
		sprintf(szTempString, "%d joints in the file have been retargetted\n\nThe following joint(s) will be ignored by SL and should probably be deleted:\n%s",(nJointsAffected+5),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}
	else
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[1024]; 
		sprintf(szTempString, "%d of %d joints in the file have been retargetted\n\nThe following joint(s) need your attention:\n%s",(nJointsAffected+5),m_pBVHFile->GetNJoints(),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}

	// adjust the ground height - NOT FULLY ACCURATE - need to take waist height as adjuster, not overall height


	GLfloat fOldFigureHeight = m_pBVHFile->GetFigureHeight();
	SetTShape();
	m_pBVHFile->CalculateFigureHeight();
	RemoveTShape();
	GLfloat fHeightAdjustmentRatio = fOldFigureHeight / m_pBVHFile->GetFigureHeight();

	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) {
		float motionDataValue = 0;
		int i = 0;
		while(i<3) {
			
			motionDataValue = motionItr->data[i];
			motionDataValue /= fHeightAdjustmentRatio;
			motionItr->data[i] = motionDataValue;
			i++;
		}
		motionItr++;
	}

	// update any arms alignment changes
	m_pBVHFile->m_armsAlignment = m_pBVHFile->CalculateArmsAxisAlignment();

	return nJointsAffected;
}

int  CBVHTweaker::ReSizeJointsAnimeeple()
{
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = jointItr->internalName;
	vector<CString> unaffectedJoints;
	int nJointsAffected = 0;
	bool bJointAffected = false;

	while(jointItr!=m_pBVHFile->m_joints.end()) 
	{
		bool bAllValuesSame = false;
		if(jointItr->offsetChannels[0]==jointItr->offsetChannels[1]&&jointItr->offsetChannels[0]==jointItr->offsetChannels[2]) 
			bAllValuesSame = true;

		if(szJointName == _T("hip")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("abdomen")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 3.402643f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("chest")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 8.438565f, -0.680529f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lThigh"))
		{
			SetOffsetChannels(&(*jointItr), 4.98375f, -1.633274f, 1.361059f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lShin")) 
		{
			SetOffsetChannels(&(*jointItr), -2.041588f, -20.007560f, 0.0f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lFoot")) 
		{
			SetOffsetChannels(&(*jointItr), 0.0f, -19.190927f, -1.224953f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			SetOffsetChannels(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			//ReSizeJoint(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			jointItr--;		
		}
		if(szJointName == _T("rThigh")) 
		{
			SetOffsetChannels(&(*jointItr), -4.98375f, -1.633274f, 1.361059f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rShin")) 
		{
			SetOffsetChannels(&(*jointItr), 2.041588f, -20.007560f, 0.0f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rFoot")) 
		{
			SetOffsetChannels(&(*jointItr), 0.0f, -19.190927f, -1.224953f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			SetOffsetChannels(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			//ReSizeJoint(&(*jointItr), 0.0f, -2.449906f, 4.627600f);
			jointItr--;		
		}
		if(szJointName == _T("lCollar")) 
		{
			SetOffsetChannels(&(*jointItr), 3.23829f, 6.669187f, -0.816635f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lShldr")) 
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), 2.51505f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), 2.51505f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lForeArm"))
		{
			if(m_pBVHFile->CalculateArmsAxisAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -11.5177f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), 11.5177f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("lHand")) 
		{
			if(m_pBVHFile->CalculateArmsAxisAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -9.0267f, 0.f, 0.f);		
			else SetOffsetChannels(&(*jointItr), 9.0267f, 0.f, 0.f);		
			// deal with end site now
			jointItr++; 
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -4.02847f, 0.0f, 0.f);
			else SetOffsetChannels(&(*jointItr), 4.02847f, 0.0f, 0.f);
			jointItr--;

			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rCollar")) 
		{
			SetOffsetChannels(&(*jointItr), -3.23829f, 6.669187f, -0.816635f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rShldr")) 
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -2.51505f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), -2.51505f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rForeArm"))
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -11.5177f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), -11.5177f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("rHand")) 
		{
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -9.0267f, 0.f, 0.f);
			else SetOffsetChannels(&(*jointItr), -9.0267f, 0.f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			if(m_pBVHFile->GetArmsAlignment()==Y_AXIS_ALIGNED_ARMS&&!bAllValuesSame) ReSizeJoint(&(*jointItr), -4.02847f, 0.0f, 0.f);
			else SetOffsetChannels(&(*jointItr), -4.02847f, 0.0f, 0.f);
			jointItr--;		
		}
		if(szJointName == _T("neck")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 9.8854f, -0.272212f);
			nJointsAffected++;
			bJointAffected = true;
		}
		if(szJointName == _T("head")) 
		{
			SetOffsetChannels(&(*jointItr), 0.f, 3.130431f, 0.f);
			nJointsAffected++;
			bJointAffected = true;
			// deal with end site now
			jointItr++; 
			SetOffsetChannels(&(*jointItr), 0.f, 3.03925f, 0.f);
			jointItr--;		
		}

		if(!bJointAffected&&szJointName.Compare("End Site")) unaffectedJoints.push_back(szJointName);
		bJointAffected=false;

		jointItr++;
		if(jointItr!=m_pBVHFile->m_joints.end()) szJointName = jointItr->internalName;
	}

	int nJointsNotCountingEndSites = m_pBVHFile->GetNJoints()-5;// we don't rename end sites
	if(nJointsAffected==nJointsNotCountingEndSites)
	{
		char szTempString[256]; 
		sprintf(szTempString, "All %d joints have been retargetted", m_pBVHFile->GetNJoints()); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}
	else if(nJointsAffected==19)
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[256]; 
		sprintf(szTempString, "%d joints in the file have been retargetted\n\nThe following joint(s) will be ignored by SL and should probably be deleted:\n%s",(nJointsAffected+5),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}
	else
	{
		CString szUnaffectedJoints = "\n";
		vector<CString>::iterator Itr = unaffectedJoints.begin();
		while(Itr!=unaffectedJoints.end()) {
			szUnaffectedJoints.Append(*Itr);
			szUnaffectedJoints.Append("\n");
			Itr++;
		}
		char szTempString[1024]; 
		sprintf(szTempString, "%d of %d joints in the file have been retargetted\n\nThe following joint(s) need your attention:\n%s",(nJointsAffected+5),m_pBVHFile->GetNJoints(),szUnaffectedJoints); 
		AfxMessageBox(_T(szTempString),MB_ICONINFORMATION);
	}

	// adjust the ground height - NOT FULLY ACCURATE - need to take waist height as adjuster, not overall height


	GLfloat fOldFigureHeight = m_pBVHFile->GetFigureHeight();
	SetTShape();
	m_pBVHFile->CalculateFigureHeight();
	RemoveTShape();
	GLfloat fHeightAdjustmentRatio = fOldFigureHeight / m_pBVHFile->GetFigureHeight();

	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) {
		float motionDataValue = 0;
		int i = 0;
		while(i<3) {
			
			motionDataValue = motionItr->data[i];
			motionDataValue /= fHeightAdjustmentRatio;
			motionItr->data[i] = motionDataValue;
			i++;
		}
		motionItr++;
	}

	// update any arms alignment changes
	m_pBVHFile->m_armsAlignment = m_pBVHFile->CalculateArmsAxisAlignment();

	return nJointsAffected;
}

void CBVHTweaker::SLify(void)
{
	// tweaking vars
	float fCollarAngle = 0.0f;
	float fShoulderAngle = 0.0f;
	float fHipsAngle = 0.0f;
	float fArmFwdAngle = 0.0f;

	int iResponse = IDNO;
	int iResponse2 = IDNO;

	if(m_pBVHFile->GetArmsAlignment()!=X_AXIS_ALIGNED_ARMS) {
		iResponse = AfxMessageBox(_T("Align arms along X axis? (may cause rotation order errors)"),MB_YESNOCANCEL|MB_ICONQUESTION);
		if(iResponse==IDCANCEL) return;
		fCollarAngle = 19.1f;
		fShoulderAngle = 61.1f;
		iResponse2 = AfxMessageBox(_T("Tweak joints for Animazoo bvh format conversion?"),MB_YESNOCANCEL|MB_ICONQUESTION);
		if(iResponse2==IDCANCEL) return;
		if(iResponse2==IDYES) {
			fHipsAngle = 4.5f;
			fArmFwdAngle = 9.0f;		
		}
	}

	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = _T("");

	while(jointItr!=m_pBVHFile->m_joints.end()) {

		szJointName = jointItr->JOINTName.GetBuffer();

		// torso
		if((szJointName==_T("Hips"))||(szJointName==_T("Hip"))||(szJointName == _T("hip"))) {
			szJointName = _T("hip");
		
			// male and female both have zeroed out hips
			jointItr->offsetChannels[2] = 0.f;
			jointItr->offsetChannels[1] = 0.f;
			jointItr->offsetChannels[0] = 0.f;

			if(iResponse2==IDYES) 
				MotionDataAdd(fHipsAngle, 0.0f, 0.0f,1,1,1, jointItr->nMotionDataIndex, ZXY);
		}

		if((szJointName==_T("Abdomen"))||(szJointName==_T("abdomen"))
		 ||(szJointName==_T("Waist")) || szJointName==_T("lowerback")) {
			szJointName = _T("abdomen");

			// set sl offsets
			jointItr->offsetChannels[2] = 0.f;
			jointItr->offsetChannels[1] = 3.402643f;
			jointItr->offsetChannels[0] = 0.f;

			// swap the Abdomen and Chest motion - removes 'stiff back' effect
			if(iResponse2==IDYES) {
				MotionDataSwapChannels(jointItr->nMotionDataIndex+0, jointItr->nMotionDataIndex+3);
				MotionDataSwapChannels(jointItr->nMotionDataIndex+1, jointItr->nMotionDataIndex+4);
				MotionDataSwapChannels(jointItr->nMotionDataIndex+2, jointItr->nMotionDataIndex+5);
			}
		}

		if((szJointName==_T("Chest2"))||(szJointName==_T("chest2"))
		 ||(szJointName == _T("chest"))||(szJointName==_T("Chest"))) {
			szJointName = _T("chest");

			// set sl offsets
			jointItr->offsetChannels[2] = 0.f;
			jointItr->offsetChannels[1] = 8.438565f;
			jointItr->offsetChannels[0] = -0.680529f;
		}

		// legs
		if((szJointName==_T("LeftHip"))||(szJointName==_T("LeftUpLeg"))||(szJointName == _T("lThigh"))) {
			szJointName = _T("lThigh");
			
			// set sl offsets
			jointItr->offsetChannels[2] = 5.308129f;
			jointItr->offsetChannels[1] = -1.633274f;
			jointItr->offsetChannels[0] = 1.361059f;		
		}

		if((szJointName==_T("LeftKnee"))||(szJointName==_T("LeftLowLeg"))||(szJointName == _T("lShin"))) {
			szJointName = _T("lShin");
			
			// set sl offsets
			jointItr->offsetChannels[2] = -2.041588f;
			jointItr->offsetChannels[1] = -20.007560f;
			jointItr->offsetChannels[0] = 0.0f;			
		}

		if((szJointName==_T("LeftAnkle"))||(szJointName==_T("LeftFoot"))||(szJointName == _T("lFoot"))) {
			szJointName = _T("lFoot");
			
			// set sl offsets
			jointItr->offsetChannels[2] = 0.0f;
			jointItr->offsetChannels[1] = -19.190927f;
			jointItr->offsetChannels[0] = -1.224953f;

			// set for End Site too
			jointItr++; 
			jointItr->offsetChannels[2] = 0.0f;
			jointItr->offsetChannels[1] = -2.449906f;
			jointItr->offsetChannels[0] = 4.627600f;
			jointItr--;		
		}

		if((szJointName==_T("RightHip"))||(szJointName==_T("RightUpLeg"))||(szJointName == _T("rThigh"))) {
			szJointName = _T("rThigh");
			
			// set sl offsets
			jointItr->offsetChannels[2] = -5.308129f;
			jointItr->offsetChannels[1] = -1.633274f;
			jointItr->offsetChannels[0] = 1.361059f;		
		}

		if((szJointName==_T("RightKnee"))||(szJointName==_T("RightLowLeg"))|(szJointName == _T("rShin"))) {
			szJointName = _T("rShin");
			
			// set sl offsets
			jointItr->offsetChannels[2] = 2.041588f;
			jointItr->offsetChannels[1] = -20.007560f;
			jointItr->offsetChannels[0] = 0.0f;;
		}

		if((szJointName==_T("RightAnkle"))||(szJointName==_T("RightFoot"))|(szJointName == _T("rFoot"))) {
			szJointName = _T("rFoot");
			
			// set sl offsets
			jointItr->offsetChannels[2] = 0.000000f;
			jointItr->offsetChannels[1] = -19.190927f;
			jointItr->offsetChannels[0] = -1.224953f;
			
			// set for End Site too
			jointItr++; 
			jointItr->offsetChannels[2] = 0.000000f;
			jointItr->offsetChannels[1] = -2.449906f;
			jointItr->offsetChannels[0] = 4.627600f;
			jointItr--;
		}
		
		// left arm		
		if((szJointName==_T("LeftCollar"))||(szJointName == _T("lCollar"))
		  ||(szJointName == _T("lColr"))) { // due to bug in Poser 6
			szJointName = _T("lCollar");

			if(iResponse==IDYES) {
			
				// set sl offsets
				jointItr->offsetChannels[2] = 3.4707f;//3.402647f; //averaged with rCollar
				jointItr->offsetChannels[1] = 6.669187f;
				jointItr->offsetChannels[0] = -0.816635f;

				// move arm down to side, swap x and y
				MotionDataAdd(-fCollarAngle, 0.0f, 0.0f,1,1,1, jointItr->nMotionDataIndex);
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;
			
			} else {
							
				// set sl offsets
				jointItr->offsetChannels[2] = 3.4707f;//3.402647f; //averaged with rCollar
				jointItr->offsetChannels[1] = 6.669187f;
				jointItr->offsetChannels[0] = -0.816635f;
			}
		}

		if((szJointName==_T("LeftShoulder"))||(szJointName==_T("LeftUpArm"))||(szJointName == _T("lShldr"))) {
			szJointName = _T("lShldr");

			if(iResponse==IDYES) {

				// set sl offsets
				jointItr->offsetChannels[2] = 3.198488f;//3.266541f; //averaged with rShldr
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;

				// move arm down to side, swap x and y
				MotionDataInvertChannel(jointItr->nMotionDataIndex+2);
				if(iResponse2==IDYES) MotionDataAdd(-fShoulderAngle, -fArmFwdAngle, 0.0f,1,1,1, jointItr->nMotionDataIndex);
				else MotionDataAdd(-fShoulderAngle, 0.f, 0.f,1,1,1, jointItr->nMotionDataIndex);
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;
			
			} else {

				// set sl offsets
				jointItr->offsetChannels[2] = 3.198488f;//3.266541f; //averaged with rShldr
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
			}
		}

		if((szJointName==_T("LeftElbow"))||(szJointName==_T("LeftLowArm"))||(szJointName == _T("lForeArm"))) {
			szJointName = _T("lForeArm");
			
			if(iResponse==IDYES) {

				// set sl offsets
				jointItr->offsetChannels[2] = 10.1398875f;//10.071835f;// averaged with rForeArm
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
	 
				// swap x and y
				MotionDataInvertChannel(jointItr->nMotionDataIndex+2);
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;

			} else {
				
				jointItr->offsetChannels[2] = 10.1398875f;//10.071835f;// averaged with rForeArm
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
			}
		}

		if((szJointName==_T("LeftWrist"))||(szJointName==_T("LeftHand"))||(szJointName == _T("lHand"))) {
			szJointName = _T("lHand");
			
			if(iResponse==IDYES) {

				// set sl offsets
				jointItr->offsetChannels[2] = 8.370511f;//8.438563f; //averaged with rHand
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;

				// swap x and y
				MotionDataInvertChannel(jointItr->nMotionDataIndex+2);
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;

				// set offsets for End Site too
				jointItr++; 
				jointItr->offsetChannels[2] = 4.083176f;
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
				jointItr--;

			} else {

				// set sl offsets (arms y axis aligned)
				jointItr->offsetChannels[2] = 8.370511f;//8.438563f; //averaged with rHand
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;

				// set offsets for End Site too
				jointItr++; 
				jointItr->offsetChannels[2] = 4.083176f;
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
				jointItr--;
			}
		}

		// right arm
		if(szJointName==_T("RightCollar")||(szJointName == _T("rCollar"))) {
			szJointName = _T("rCollar");
	
			if(iResponse==IDYES) {

				// set sl offsets
				jointItr->offsetChannels[2] = -3.4707f;//-3.538753f;//averaged with lCollar
				jointItr->offsetChannels[1] = 6.669187f;
				jointItr->offsetChannels[0] = -0.816635f;

				// move arm down to side, swap x and y
				MotionDataAdd(fCollarAngle, 0.0f, 0.0f,1,1,1, jointItr->nMotionDataIndex);// i.e. delta z
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;
			
			} else {

				// set sl offsets
				jointItr->offsetChannels[2] = -3.4707f;//-3.538753f;//averaged with lCollar
				jointItr->offsetChannels[1] = 6.669187f;
				jointItr->offsetChannels[0] = -0.816635f;
			}
		}

		if((szJointName==_T("RightShoulder"))||(szJointName==_T("RightUpArm"))||(szJointName == _T("rShldr"))) {
			szJointName = _T("rShldr");

			if(iResponse==IDYES) {

				// set sl offsets
				jointItr->offsetChannels[2] = -3.198488f;//-3.130435f;//averaged with lShldr
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;

				// move arm down to side, swap x and y
				MotionDataInvertChannel(jointItr->nMotionDataIndex+1);
				if(iResponse2==IDYES) MotionDataAdd(fShoulderAngle, fArmFwdAngle, 0.0f,1,1,1, jointItr->nMotionDataIndex);
				else MotionDataAdd(fShoulderAngle, 0.f, 0.f,1,1,1, jointItr->nMotionDataIndex);
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;
			
			} else {

				// set sl offsets
				jointItr->offsetChannels[2] = -3.198488f;//-3.130435f;//averaged with lShldr
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
			}
		}

		if((szJointName==_T("RightElbow"))||(szJointName==_T("RightLowArm"))||(szJointName == _T("rForeArm"))) {
			szJointName = _T("rForeArm");
			
			if(iResponse==IDYES) {

				// set sl offsets
				jointItr->offsetChannels[2] = -10.1398875f;//-10.207940f; // averaged with lForeArm
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
	 
				// swap x and y
				MotionDataInvertChannel(jointItr->nMotionDataIndex+1);
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;

			} else {

				jointItr->offsetChannels[2] = -10.1398875f;//-10.207940f; // averaged with lForeArm
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
			}

		}

		if((szJointName==_T("RightWrist"))||(szJointName==_T("RightHand"))||(szJointName==_T("rHand"))) {
			szJointName = _T("rHand");

			if(iResponse==IDYES) {
			
				// set sl offsets
				jointItr->offsetChannels[2] = -8.370511f;//-8.302459f; //averaged with lHand
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
							
				// set for End Site too
				jointItr++; 
				jointItr->offsetChannels[2] = -4.083176f;
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
				jointItr--;
	  
				// swap x and y
				MotionDataInvertChannel(jointItr->nMotionDataIndex+1);
				jointItr->CHANNELSLine = _T("CHANNELS 3 Zrotation Yrotation Xrotation");
				jointItr->channelOrder = ZYX;
			
			} else {

				jointItr->offsetChannels[2] = -8.370511f;//-8.302459f; //averaged with lHand
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
							
				// set for End Site too
				jointItr++; 
				jointItr->offsetChannels[2] = -4.083176f;
				jointItr->offsetChannels[1] = 0.000000f;
				jointItr->offsetChannels[0] = 0.000000f;
				jointItr--;
			}
		}

		// the neck up
		if(szJointName==_T("Neck")||(szJointName==_T("neck"))) {
			szJointName = _T("neck");
			
			// set sl offsets
			jointItr->offsetChannels[2] = 0.000000f;
			jointItr->offsetChannels[1] = 10.207942f;
			jointItr->offsetChannels[0] = -0.272212f;
		}

		if(szJointName==_T("Head")|(szJointName==_T("head"))) {
			szJointName = _T("head");
			
			// set sl offsets
			jointItr->offsetChannels[2] = 0.000000f;
			jointItr->offsetChannels[1] = 3.130431f;
			jointItr->offsetChannels[0] = 0.000000f;
 						
			// set for End Site too (assuming no hair!)
			jointItr++; 
			jointItr->offsetChannels[2] = 0.000000f;
			jointItr->offsetChannels[1] = 3.130435f;
			jointItr->offsetChannels[0] = 0.000000f;
			jointItr--;
		}
		
		jointItr->JOINTName = szJointName.GetBuffer();
		jointItr++;
	}

	// adjust the ground height - NOT FULLY ACCURATE - need to take waist height as adjuster, not overall height
	GLfloat fOldFigureHeight = m_pBVHFile->GetFigureHeight();
	m_pBVHFile->CalculateFigureHeight();
	GLfloat fHeightAdjustmentRatio = fOldFigureHeight / m_pBVHFile->GetFigureHeight();

	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) {
		float motionDataValue = 0;
		int i = 0;
		while(i<3) {
			
			motionDataValue = motionItr->data[i];
			motionDataValue /= fHeightAdjustmentRatio;
			motionItr->data[i] = motionDataValue;
			i++;
		}
		motionItr++;
	}

	// update any arms alignment changes
	m_pBVHFile->m_armsAlignment = m_pBVHFile->CalculateArmsAxisAlignment();
}

void CBVHTweaker::MotionDataSwapChannels(int indexOne, int indexTwo)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	float motionDataValue = 0;
	while(motionItr!=m_pBVHFile->m_motion.end()) {
		
		motionDataValue = motionItr->data[indexOne];
		motionItr->data[indexOne] = motionItr->data[indexTwo];
		motionItr->data[indexTwo] = motionDataValue;
		motionItr++;
	}
}

// TODO: borked!
void CBVHTweaker::ResampleTimeLine(int nNewFrames)
{
	//Resampler resampler = new Resampler();

}
//	float fOldFrameTime = m_pBVHFile->GetFrameTime();
//	int nOldNFrames = m_pBVHFile->GetNFrames();
//	float fAnimationLength = fOldFrameTime*(float)nOldNFrames;
//	int nParameters = m_pBVHFile->GetNParameters();
//
//	//if(nOldNFrames<nNewFrames) 
//	//{
//	//	AfxMessageBox(_T("bvhacker currently supports only downsampling."));
//	//	return;
//	//}
//
//	GLfloat fStartVal = 0.f;
//	GLfloat fEndVal = 0.f;
//	GLfloat fMediator = 0.f;
//	//int nIndex = 0;
//
//	vector<BVHMotion>::iterator currentMotionItr = m_pBVHFile->m_motion.begin();
//	vector<BVHMotion> inflatedMotion;
//	vector<BVHMotion> newMotion;
//
//	int count = 0;
//
//	// first build up an inflated version of the motion data
//	while(currentMotionItr!=(m_pBVHFile->m_motion.end()-1)) {
//
//		// interpolate each joint and add it to inflatedMotion 
//		BVHMotion start = *currentMotionItr++;
//		BVHMotion end = *currentMotionItr--;
//
//		// flawfinder - ignore
//		if(count++==196)
//		{
//			int stop = TRUE;
//		}
//
//		for(float j=0;j<nNewFrames;j++) {
//
//			// create a new line of MOTION data
//			BVHMotion interpolated = start;
//			if(j!=0)  {
//				
//				for(int k=0;k<nParameters;k++) {
//
//					fStartVal = (GLfloat)start.data[k];
//					fEndVal = (GLfloat)end.data[k];
//					
//					//fMediator = GetDifference(fStartVal, fEndVal);
//
//					if((fStartVal>=90.0)&&(fEndVal<-90.00)){
//						//'switch' condition
//						if(j>(nNewFrames-2)) {
//							int stop = TRUE;
//						}
//						fMediator = -1*GetDifference(fStartVal, fEndVal);
//					}
//
//					else if ((fEndVal>=90.0)&&(fStartVal<-90.00)){
//						//'switch' condition
//						if(j>(nNewFrames-2)) {
//							int stop = TRUE;
//						}
//						fMediator = GetDifference(fStartVal, fEndVal);
//					}
//
//					else fMediator = GetDifference(fStartVal, fEndVal);
//
//					// adjust the start figure
//					fMediator*=(j/nNewFrames);
//
//					//if(k==5&&j==(nNewFrames-2)&&fabs(interpolated.data[k])+fMediator>180)
//					//{
//					//	int stop = TRUE;
//					//	interpolated.data[k]*=-1;
//					//	interpolated.data[k] += fMediator;
//					//	if(interpolated.data[k]>180) interpolated.data[k] = 180-(interpolated.data[k]-180);
//					//}
//
//					float fInterpolatedValue = interpolated.data[k] + fMediator;
//
//					// final work on switch condition
//					if(fInterpolatedValue>179.999999) {
//						int eek = TRUE;
//						fInterpolatedValue -= 360;
//					}
//					else if(fInterpolatedValue <-179.999999) {
//						int eek = TRUE;
//						fInterpolatedValue += 360;
//					}
//
//					interpolated.data[k] = fInterpolatedValue;
//				}
//			}
//			// Add the new line to the inflated motion vector
//			inflatedMotion.push_back(interpolated);
//		}
//		currentMotionItr++;
//	}
//	BVHMotion LastOne = *currentMotionItr;
//	inflatedMotion.push_back(LastOne);
//
//	// then pick off the BVHMotions we want straight into m_motion
//	m_pBVHFile->m_motion.clear();
//	int nIndex = 0;
//	vector<BVHMotion>::iterator inflatedMotionItr = inflatedMotion.begin();
//	inflatedMotionItr+=nNewFrames/2;
//	BVHMotion finalResult;
//	
//	while(inflatedMotionItr<inflatedMotion.end()-nOldNFrames) {
//		finalResult = *inflatedMotionItr;
//		m_pBVHFile->m_motion.push_back(finalResult);	
//		inflatedMotionItr+=(nOldNFrames-1);
//		nIndex+=(nOldNFrames-1);
//	}
//	finalResult = *inflatedMotionItr;
//	m_pBVHFile->m_motion.push_back(finalResult);
//
//	// update the internal status of the BVHFile
//	m_pBVHFile->m_nFrames = (int)m_pBVHFile->m_motion.size();
//	m_pBVHFile->m_nCurrentFrameIdx = 0;
//	m_pBVHFile->m_fFrameTime = (fAnimationLength/1000.f)/(float)nNewFrames;
//	m_pBVHFile->m_pCurrentMotionFrame = &(*m_pBVHFile->m_motion.begin());
//}

// helper function for RearrangeMotionChannels below
// this function is untested and may need the assigned values order reversed...
void CBVHTweaker::AssignValuesByRotOrder(BVHJoint* pJoint, float* pX, float* pY, float* pZ)
{
	float x = *pX;
	float y = *pY;
	float z = *pZ;


	switch (pJoint->channelOrder) 
	{
		case XYZ:
		{
			*pX = x;
			*pY = y;
			*pZ = z;
		}
		case XZY:
		{
			*pX = x;
			*pY = z;
			*pZ = y;
		}
		case YXZ:
		{
			*pX = y;
			*pY = x;
			*pZ = z;
		}
		case YZX:
		{
			*pX = y;
			*pY = z;
			*pZ = x;
		}
		case ZXY:
		{
			*pX = z;
			*pY = x;
			*pZ = y;
		}
		case ZYX:
		{
			*pX = z;
			*pY = y;
			*pZ = x;
		}
	}
}

void CBVHTweaker::RearrangeMotionChannels(BVHJoint* pJoint, int newOrder)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	int jointMotionIndex = pJoint->nMotionDataIndex;

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	//switch (pJoint->channelOrder) 
	//{
	//	case ZXY: // all animazoo joints all come out with this channel order
	//		{
				switch (newOrder) 
				{
					case XZY: 
						{
							while(motionItr!=m_pBVHFile->m_motion.end()) {
								x = motionItr->data[jointMotionIndex+0];
								y = motionItr->data[jointMotionIndex+1];
								z = motionItr->data[jointMotionIndex+2];
								AssignValuesByRotOrder(&(*pJoint), &x, &y, &z);
								motionItr->data[jointMotionIndex+0] = x;
								motionItr->data[jointMotionIndex+1] = z;
								motionItr->data[jointMotionIndex+2] = y;
								motionItr++;
							}						
						}
					case XYZ: 
						{
							while(motionItr!=m_pBVHFile->m_motion.end()) {
								x = motionItr->data[jointMotionIndex+0];
								y = motionItr->data[jointMotionIndex+1];
								z = motionItr->data[jointMotionIndex+2];
								AssignValuesByRotOrder(&(*pJoint), &x, &y, &z);
								motionItr->data[jointMotionIndex+0] = x;
								motionItr->data[jointMotionIndex+1] = z;
								motionItr->data[jointMotionIndex+2] = y;
								motionItr++;
							}						
						}
					case YZX: // for lCollar, lForeArm, rCollar, rForeArm
						{
							while(motionItr!=m_pBVHFile->m_motion.end()) {
								x = motionItr->data[jointMotionIndex+0];
								y = motionItr->data[jointMotionIndex+1];
								z = motionItr->data[jointMotionIndex+2];
								AssignValuesByRotOrder(&(*pJoint), &x, &y, &z);
								motionItr->data[jointMotionIndex+0] = x;
								motionItr->data[jointMotionIndex+1] = z;
								motionItr->data[jointMotionIndex+2] = y;
								motionItr++;
							}						
						}
					case ZYX: // for lShldr, lHand, rShldr, rHand
						{
							while(motionItr!=m_pBVHFile->m_motion.end()) {
								x = motionItr->data[jointMotionIndex+0];
								y = motionItr->data[jointMotionIndex+1];
								z = motionItr->data[jointMotionIndex+2];
								AssignValuesByRotOrder(&(*pJoint), &x, &y, &z);
								motionItr->data[jointMotionIndex+0] = x;
								motionItr->data[jointMotionIndex+1] = z;
								motionItr->data[jointMotionIndex+2] = y;
								motionItr++;
							}
						}
					case YXZ: // for bugfix
						{
							while(motionItr!=m_pBVHFile->m_motion.end()) {
								x = motionItr->data[jointMotionIndex+0];
								y = motionItr->data[jointMotionIndex+1];
								z = motionItr->data[jointMotionIndex+2];
								AssignValuesByRotOrder(&(*pJoint), &x, &y, &z);
								motionItr->data[jointMotionIndex+0] = x;
								motionItr->data[jointMotionIndex+1] = z;
								motionItr->data[jointMotionIndex+2] = y;
								motionItr++;
							}						
						}
					default: 
						{
						}
						}

}

void CBVHTweaker::RenameJoint()
{
	BVHJoint* pJoint = m_pBVHFile->GetSelectedJoint();
	CNewJointDlg *newJointDialog = new CNewJointDlg();
	INT_PTR result = newJointDialog->DoModal();
	if((result!=1)||newJointDialog->GetName()==_T("")) {
		pJoint = NULL;
		delete newJointDialog;
		newJointDialog = NULL;
		return;
	}
	pJoint->JOINTName = newJointDialog->GetName();
	pJoint = NULL;
	delete newJointDialog;
	newJointDialog = NULL;
}

void CBVHTweaker::RemoveJoint()
{
	BVHJoint* pCurrentJoint = m_pBVHFile->GetSelectedJoint();
	BVHJoint* pParent = m_pBVHFile->GetJoint(pCurrentJoint->parentIndex);
	int bIsEndSite = false;
	int nChannels = pCurrentJoint->nChannels;
	
	if(pCurrentJoint->isEndSite) {
		//AfxMessageBox(_T("You can't remove an End Site."),MB_ICONSTOP);
		//pCurrentJoint = NULL;
		//pParent = NULL;
		//return;
		bIsEndSite = true;
	}
	if(pCurrentJoint->isROOT) {
		AfxMessageBox(_T("You can't remove the ROOT."),MB_ICONSTOP);
		pCurrentJoint = NULL;
		pParent = NULL;
		return;
	}
	//int iResponse = AfxMessageBox(_T("This may case bvhacker to crash. Sorry, not finished yet. Do you want to continue?"),MB_YESNO|MB_ICONEXCLAMATION);
	//if(iResponse==IDCANCEL||iResponse==IDNO) {
	//	pCurrentJoint = NULL;
	//	pParent = NULL;
	//	return;
	//}

	// remove motion 
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	if(!bIsEndSite) {

		while(motionItr!=m_pBVHFile->m_motion.end()) {

			// remove the three rotation values for the joint being deleted
			for(int i=pCurrentJoint->nMotionDataIndex + pCurrentJoint->nChannels - 1;
					i>=pCurrentJoint->nMotionDataIndex;i--) {

				vector<GLfloat>::iterator dataItr = motionItr->data.begin()+i;
				motionItr->data.erase(dataItr);	
			}
			motionItr++;
		}
	}

	// update all affected joints
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	int bSubsequent = true;

	while(++jointItr<m_pBVHFile->m_joints.end()) {

		// decrement hierarchy on siblings only
		if(((jointItr->heirarchy)>pCurrentJoint->heirarchy)&&(bSubsequent)) {
			jointItr->heirarchy--;
		} else bSubsequent = false;

		// set parent indices on subsequent joints
		if(jointItr->parentIndex == pCurrentJoint->index) {// if directly attached to deleted joint
			jointItr->parentIndex = pCurrentJoint->parentIndex;
		}
		else if(jointItr->parentIndex > pCurrentJoint->parentIndex) jointItr->parentIndex--;

		//// check user has not attatched joints to an End Site!
		//pParent = m_pBVHFile->GetJoint(jointItr->parentIndex);
		//if(pParent->isEndSite) {
		//	jointItr->parentIndex--;
		//	if(jointItr->parentIndex<0) jointItr->parentIndex=0;
		//}

		// decrement index on all subsequent joints
		jointItr->index--;
		if(!jointItr->isEndSite) jointItr->nMotionDataIndex-=pCurrentJoint->nChannels;
	}

	// remove the selected joint from the joints vector in pBVHFile
	int deletedJointHierarchy = pCurrentJoint->heirarchy;
	jointItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	m_pBVHFile->m_joints.erase(jointItr);

	// update parameters in BVHFile
	m_pBVHFile->m_nJoints--;
	if(!bIsEndSite) m_pBVHFile->m_nParameters-=nChannels;
	motionItr = m_pBVHFile->m_motion.begin();
	m_pBVHFile->m_pCurrentMotionFrame = &(*m_pBVHFile->m_motion.begin());
	m_pBVHFile->m_nCurrentFrameIdx = 0;
	m_pBVHFile->CalculateFigureHeight();
	
	// clean up
	pCurrentJoint = NULL;
	pParent = NULL;
}

void CBVHTweaker::InsertJoint()
{
	BVHJoint* pCurrentJoint = m_pBVHFile->GetSelectedJoint();
	BVHJoint* pParent = m_pBVHFile->GetJoint(pCurrentJoint->parentIndex);
	if(pCurrentJoint->isROOT) {
		AfxMessageBox(_T("You can't add joints above the root.\nPlease select any other joint and try again."),MB_ICONINFORMATION);
		pCurrentJoint = NULL;
		pParent = NULL;
		return;
	}

	// get a name for the new joint
	CNewJointDlg *newJointDialog = new CNewJointDlg();
	INT_PTR result = newJointDialog->DoModal();
	if(result!=1) {
		delete newJointDialog;
		newJointDialog = NULL;
		pCurrentJoint = NULL;
		pParent = NULL;
		return;
	}

	// interpolate the new joint's offset from the current joint and it's parent
	vector<float> newOffsetChannels;
	vector<float>::iterator currentOffsets = pCurrentJoint->offsetChannels.begin();
	vector<float>::iterator parentOffsets = pParent->offsetChannels.begin();
	float replacementValue = 0.0f;
	while(currentOffsets < pCurrentJoint->offsetChannels.end() &&
		parentOffsets < pParent->offsetChannels.end()) {

		// if both params the same (avoid div by zero)
		if((*currentOffsets)==(*parentOffsets)) {
			replacementValue = (*currentOffsets);
		} else {
			// if both above zero
			if((*currentOffsets)>=0 && (*parentOffsets)>=0) {
				if((*currentOffsets)>(*parentOffsets)) {
					replacementValue = (((*currentOffsets)-(*parentOffsets))/2);
					replacementValue+=*parentOffsets;
				} else {
					replacementValue = (((*parentOffsets)-(*currentOffsets))/2);
					replacementValue+=*currentOffsets;
				}
			}

			// if currentOffsets only above zero
			else if((*currentOffsets)>=0 && (*parentOffsets)<0) {
				replacementValue = (((*currentOffsets)+(*parentOffsets))/2);
			}

			// if parentOffsets only above zero
			else if((*currentOffsets)<0 && (*parentOffsets)>=0) {
				replacementValue = (((*parentOffsets)+(*currentOffsets))/2);
			}

			// if both below zero
			else if((*currentOffsets)<0 && (*parentOffsets)<0) {
				if((*currentOffsets)>(*parentOffsets)) {
					replacementValue = (((*parentOffsets)-(*currentOffsets))/2);
					replacementValue+=*currentOffsets;
				} else {
					replacementValue = (((*currentOffsets)-(*parentOffsets))/2);
					replacementValue+=*parentOffsets;
				}
			}
		}
		newOffsetChannels.push_back(replacementValue);
		
		// reset for the next
		replacementValue = 0.0f;
		currentOffsets++;
		parentOffsets++;
	}		
	
	// create a new joint
	BVHJoint newJoint;
	if(!pCurrentJoint->isEndSite)
	{
		newJoint.channelOrder = pCurrentJoint->channelOrder;
		newJoint.CHANNELSLine = pCurrentJoint->CHANNELSLine;
		newJoint.nOffsetMotionDataIndex = 0;
		newJoint.nScaleMotionDataIndex = 0;
		newJoint.offsetChannelOrder = NONE;
		newJoint.scaleChannelOrder = NONE;
	}
	else
	{
		newJoint.channelOrder = pParent->channelOrder;
		newJoint.CHANNELSLine = pParent->CHANNELSLine;
	}
	newJoint.heirarchy = pCurrentJoint->heirarchy; // included later on all siblings
	newJoint.index = pCurrentJoint->index;         // included later on all subsequent joints
	newJoint.isEndSite = false;
	newJoint.isROOT = false;
	newJoint.JOINTName = newJointDialog->GetName();
	newJoint.nChannels = 3;
	if(pCurrentJoint->isEndSite) newJoint.nMotionDataIndex = pParent->nMotionDataIndex+3;
	else newJoint.nMotionDataIndex = pCurrentJoint->nMotionDataIndex; // inc on all subsequent joints
	newJoint.offsetChannels = newOffsetChannels;
	newJoint.parentIndex = pCurrentJoint->parentIndex;	// inc on all siblings
	newJoint.scaleChannels = pCurrentJoint->scaleChannels;

	// insert the new joint into the joints vector in pBVHFile
	vector<BVHJoint>::iterator tempItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	m_pBVHFile->m_joints.insert(tempItr, newJoint);

	// now update all affected joints
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	int bSubsequent = true;

	// deal with the next joint (special case)
	jointItr++;
	jointItr->index++;
	jointItr->heirarchy++;
	jointItr->parentIndex = newJoint.index;
	if(!jointItr->isEndSite) jointItr->nMotionDataIndex+=newJoint.nChannels;

	while(++jointItr<m_pBVHFile->m_joints.end()) {

		// siblings only
		if((jointItr->heirarchy>newJoint.heirarchy)&&bSubsequent) {
			jointItr->heirarchy++;
			jointItr->parentIndex++;
		} else bSubsequent = false;

		// update parent index for non-siblings (except those attached to the ROOT (and some others..?))
		if(jointItr->parentIndex>newJoint.heirarchy&&!bSubsequent) {
			jointItr->parentIndex++;
		}

		// for all subsequent joints
		jointItr->index++;
		if(!jointItr->isEndSite) jointItr->nMotionDataIndex+=newJoint.nChannels;
	}
		
	// update the motion data with zeros for new joint
	// for every line of motion data, 
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) {

		// add (nChannels) sets of zeroes for the new joint
		for(int i=newJoint.nMotionDataIndex;
				i<newJoint.nMotionDataIndex + newJoint.nChannels;i++) {

			vector<GLfloat>::iterator dataItr = motionItr->data.begin()+i;
			if((newJoint.JOINTName==_T("Abdomen"))&&i==newJoint.nMotionDataIndex+1) {
				motionItr->data.insert(dataItr, 9.0f);	
			} else {
				motionItr->data.insert(dataItr, 0.0f);	
			}
		}
		motionItr++;
	}

	// update parameters in BVHFile
	m_pBVHFile->m_nJoints++;
	m_pBVHFile->m_nParameters+=newJoint.nChannels;
	motionItr = m_pBVHFile->m_motion.begin();
	m_pBVHFile->m_pCurrentMotionFrame = &(*m_pBVHFile->m_motion.begin());
	m_pBVHFile->m_nCurrentFrameIdx = 0;
	m_pBVHFile->CalculateFigureHeight();

	// clean up
	delete newJointDialog;
	newJointDialog = NULL;
	pCurrentJoint = NULL;
	pParent = NULL;
}

void CBVHTweaker::MotionDataInvertChannel(int nMotionDataIndex)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	float motionDataValue = 0;
	while(motionItr!=m_pBVHFile->m_motion.end()) {
		
		motionDataValue = motionItr->data[nMotionDataIndex];
		motionDataValue*=-1;
		motionItr->data[nMotionDataIndex] = motionDataValue;
		motionItr++;
	}
}


void CBVHTweaker::AddAbdomen(void)
{
	BVHJoint* pCurrentJoint = NULL;
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) {

		if(jointItr->internalName ==_T("chest"))
		{
			pCurrentJoint = &(*jointItr);
		}
		if(jointItr->internalName ==_T("abdomen"))
		{
			AfxMessageBox(_T("The figure already has an abdomen"),MB_ICONSTOP);
			pCurrentJoint = NULL;
			return;
		}
		jointItr++;
	}
	if (pCurrentJoint==NULL) return;

	BVHJoint* pParent = m_pBVHFile->GetJoint(pCurrentJoint->parentIndex);

	// interpolate the new joint's offset from the current joint and it's parent
	vector<float> newOffsetChannels;
	vector<float>::iterator currentOffsets = pCurrentJoint->offsetChannels.begin();
	vector<float>::iterator parentOffsets = pParent->offsetChannels.begin();
	float replacementValue = 0.0f;
	while(currentOffsets < pCurrentJoint->offsetChannels.end() &&
		parentOffsets < pParent->offsetChannels.end()) {

		// if both params the same (avoid div by zero)
		if((*currentOffsets)==(*parentOffsets)) {
			replacementValue = (*currentOffsets);
		} else {
			// if both above zero
			if((*currentOffsets)>=0 && (*parentOffsets)>=0) {
				if((*currentOffsets)>(*parentOffsets)) {
					replacementValue = (((*currentOffsets)-(*parentOffsets))/2);
					replacementValue+=*parentOffsets;
				} else {
					replacementValue = (((*parentOffsets)-(*currentOffsets))/2);
					replacementValue+=*currentOffsets;
				}
			}

			// if currentOffsets only above zero
			else if((*currentOffsets)>=0 && (*parentOffsets)<0) {
				replacementValue = (((*currentOffsets)+(*parentOffsets))/2);
			}

			// if parentOffsets only above zero
			else if((*currentOffsets)<0 && (*parentOffsets)>=0) {
				replacementValue = (((*parentOffsets)+(*currentOffsets))/2);
			}

			// if both below zero
			else if((*currentOffsets)<0 && (*parentOffsets)<0) {
				if((*currentOffsets)>(*parentOffsets)) {
					replacementValue = (((*parentOffsets)-(*currentOffsets))/2);
					replacementValue+=*currentOffsets;
				} else {
					replacementValue = (((*currentOffsets)-(*parentOffsets))/2);
					replacementValue+=*parentOffsets;
				}
			}
		}
		newOffsetChannels.push_back(replacementValue);
		
		// reset for the next
		replacementValue = 0.0f;
		currentOffsets++;
		parentOffsets++;
	}		
	
	// create a new joint
	BVHJoint newJoint;
	if(!pCurrentJoint->isEndSite)
	{
		newJoint.internalName = _T("abdomen");
		newJoint.channelOrder = pCurrentJoint->channelOrder;
		newJoint.CHANNELSLine = pCurrentJoint->CHANNELSLine;
		newJoint.nOffsetMotionDataIndex = 0;
		newJoint.nScaleMotionDataIndex = 0;
		newJoint.offsetChannelOrder = NONE;
		newJoint.scaleChannelOrder = NONE;
	}
	else
	{
		newJoint.channelOrder = pParent->channelOrder;
		newJoint.CHANNELSLine = pParent->CHANNELSLine;
	}
	newJoint.heirarchy = pCurrentJoint->heirarchy; // included later on all siblings
	newJoint.index = pCurrentJoint->index;         // included later on all subsequent joints
	newJoint.isEndSite = false;
	newJoint.isROOT = false;
	newJoint.JOINTName = _T("abdomen");
	newJoint.nChannels = 3;
	if(pCurrentJoint->isEndSite) newJoint.nMotionDataIndex = pParent->nMotionDataIndex+3;
	else newJoint.nMotionDataIndex = pCurrentJoint->nMotionDataIndex; // inc on all subsequent joints
	newJoint.offsetChannels = newOffsetChannels;
	newJoint.parentIndex = pCurrentJoint->parentIndex;	// inc on all siblings
	newJoint.scaleChannels = pCurrentJoint->scaleChannels;

	// insert the new joint into the joints vector in pBVHFile
	vector<BVHJoint>::iterator tempItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	m_pBVHFile->m_joints.insert(tempItr, newJoint);

	// now update all affected joints
	int bSubsequent = true;

	// deal with the next joint (special case)
	jointItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	jointItr++;
	jointItr->index++;
	jointItr->heirarchy++;
	jointItr->parentIndex = newJoint.index;
	if(!jointItr->isEndSite) jointItr->nMotionDataIndex+=newJoint.nChannels;

	while(++jointItr<m_pBVHFile->m_joints.end()) {

		// siblings only
		if((jointItr->heirarchy>newJoint.heirarchy)&&bSubsequent) {
			jointItr->heirarchy++;
			jointItr->parentIndex++;
		} else bSubsequent = false;
		//if(jointItr->isEndSite) bSubsequent = false;

		// update parent index for non-siblings (except those attached to the ROOT)
		if(jointItr->parentIndex>0&&!bSubsequent) {
			jointItr->parentIndex++;
		}

		// for all subsequent joints
		jointItr->index++;
		if(!jointItr->isEndSite) jointItr->nMotionDataIndex+=newJoint.nChannels;
	}
		
	// update the motion data with zeros for new joint
	// for every line of motion data, 
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) {

		// add (nChannels) sets of zeroes for the new joint
		for(int i=newJoint.nMotionDataIndex;
				i<newJoint.nMotionDataIndex + newJoint.nChannels;i++) {

			vector<GLfloat>::iterator dataItr = motionItr->data.begin()+i;
			if((newJoint.JOINTName==_T("Abdomen"))&&i==newJoint.nMotionDataIndex+1) {
				motionItr->data.insert(dataItr, 9.0f);	
			} else {
				motionItr->data.insert(dataItr, 0.0f);	
			}
		}
		motionItr++;
	}

	// update parameters in BVHFile
	m_pBVHFile->m_nJoints++;
	m_pBVHFile->m_nParameters+=newJoint.nChannels;
	motionItr = m_pBVHFile->m_motion.begin();
	m_pBVHFile->m_pCurrentMotionFrame = &(*m_pBVHFile->m_motion.begin());
	m_pBVHFile->m_nCurrentFrameIdx = 0;
	m_pBVHFile->CalculateFigureHeight();

	// clean up
	pCurrentJoint = NULL;
	pParent = NULL;
}

void CBVHTweaker::SwapAbsChestMotion()
{
	BVHJoint* pJoint = NULL;
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) {

		if((jointItr->internalName ==_T("chest")))
		{
			pJoint = &(*jointItr);
		}
		jointItr++;
	}
	if (pJoint==NULL) return;//can'f find chest? then return

	// gets used for Poser converted bvh files
	MotionDataSwapChannels(pJoint->nMotionDataIndex+0, pJoint->nMotionDataIndex-3);
	MotionDataSwapChannels(pJoint->nMotionDataIndex+1, pJoint->nMotionDataIndex-2);
	MotionDataSwapChannels(pJoint->nMotionDataIndex+2, pJoint->nMotionDataIndex-1);
	pJoint = NULL;
}

// assumes chest and abdomen have same joint rotation order
// gets used for Poser converted bvh files
void CBVHTweaker::InterpolateChestAbdomen()
{
	BVHJoint* pChestJoint = NULL;
	BVHJoint* pAbdomenJoint = NULL;
	Vector3<float> vChestValues(0.f,0.f,0.f);
	Vector3<float> vAbdomenValues(0.f,0.f,0.f);
	Vector3<float> vCombinedValues(0.f,0.f,0.f);

	float fChestStiffness = 0.3f;// i.e. chest gets 30% of total rotation
	
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) 
	{
		if(jointItr->internalName ==_T("chest"))
		{
			pChestJoint = &(*jointItr);		
		}
		if(jointItr->internalName ==_T("abdomen"))
		{
			pAbdomenJoint = &(*jointItr);		
		}
		jointItr++;
	}
	if (pChestJoint==NULL||pAbdomenJoint==NULL)
	{
		AfxMessageBox(_T("bvhacker is unable to find the joints necessary for this operation"), MB_ICONSTOP);
		return;//can't find chest or abdomen? then return
	}
	
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// get values for both joints and combine
		vChestValues.x=motionItr->data[pChestJoint->nMotionDataIndex+0];
		vChestValues.y=motionItr->data[pChestJoint->nMotionDataIndex+1];
		vChestValues.z=motionItr->data[pChestJoint->nMotionDataIndex+2];

		vAbdomenValues.x=motionItr->data[pAbdomenJoint->nMotionDataIndex+0];
		vAbdomenValues.y=motionItr->data[pAbdomenJoint->nMotionDataIndex+1];
		vAbdomenValues.z=motionItr->data[pAbdomenJoint->nMotionDataIndex+2];

		vCombinedValues=vChestValues+vAbdomenValues;
		
		// spread the curves
		vChestValues=vCombinedValues*fChestStiffness;
		vAbdomenValues=vCombinedValues*(1.f-fChestStiffness);

		// write new values
		motionItr->data[pChestJoint->nMotionDataIndex+0]=vChestValues.x;
		motionItr->data[pChestJoint->nMotionDataIndex+1]=vChestValues.y;
		motionItr->data[pChestJoint->nMotionDataIndex+2]=vChestValues.z;

		motionItr->data[pChestJoint->nMotionDataIndex-3]=vAbdomenValues.x;
		motionItr->data[pChestJoint->nMotionDataIndex-2]=vAbdomenValues.y;
		motionItr->data[pChestJoint->nMotionDataIndex-1]=vAbdomenValues.z;

		motionItr++;
	}
	pChestJoint = NULL;
	pAbdomenJoint = NULL;
}

void CBVHTweaker::MotionBuilderHeightFix()
{
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) {

		if((jointItr->JOINTName == _T("hip")||
		   (jointItr->JOINTName ==_T("Hips"))))
		{
			jointItr->offsetChannels[0] = 0.f;
			jointItr->offsetChannels[1] = 0.f;
			jointItr->offsetChannels[2] = 0.f;
			return;
		}
		jointItr++;
	}
}

// don't leave loads of zeros in the first frame - SL will ignore the joint if you do
void CBVHTweaker::GetRidOfZeros(void)
{
	// dont miss this bit!
	float fThreshold = 0.0202555f;// educated guess at threshold - seems to work ok

	// search motion data for zeros. replace any found with fThreshold
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();

	for(int i=3;i<m_pBVHFile->m_nParameters;i++)
	{
		vector<GLfloat>::iterator dataItr = motionItr->data.begin()+i;
		if(fabs((double)*dataItr)<fThreshold) {
			dataItr = motionItr->data.erase(dataItr);
			motionItr->data.insert(dataItr, fThreshold);
		}
	}
}
// zeros out all rotation for the currently selected joint
void CBVHTweaker::ZeroOut(void)
{
	BVHJoint* pJoint = m_pBVHFile->GetSelectedJoint();
	int index = pJoint->nMotionDataIndex;
	bool bDoHipsTranslation = false;

	if(pJoint->isROOT)
	{
		int iResponse = AfxMessageBox(_T("Would you like to zero out the translation for the ROOT?"),MB_YESNO|MB_ICONQUESTION);
		if(iResponse==IDYES) bDoHipsTranslation = true;
	}

	// search motion data for current joint. replace values with zero for that joint
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		if(bDoHipsTranslation)
		{
			for(int i=0;i<3;i++)
			{
				vector<GLfloat>::iterator dataItr = motionItr->data.begin()+pJoint->nOffsetMotionDataIndex+i;
				dataItr = motionItr->data.erase(dataItr);
				if(i==1) motionItr->data.insert(dataItr, m_pBVHFile->GetGroundHeight());
				else motionItr->data.insert(dataItr, 0.f); 
			}
		}
		else
		{
			for(int i=0;i<3;i++)
			{
				vector<GLfloat>::iterator dataItr = motionItr->data.begin()+index+i;
				dataItr = motionItr->data.erase(dataItr);
				motionItr->data.insert(dataItr, 0.f);
			}
		}
		motionItr++;
	}
}

void CBVHTweaker::Scale(bool bScaleUp)
{
	// scale all the bones
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) {

		if(bScaleUp)
		{
			jointItr->offsetChannels[0] *= 2.f;
			jointItr->offsetChannels[1] *= 2.f;
			jointItr->offsetChannels[2] *= 2.f;
		}
		else
		{
			jointItr->offsetChannels[0] /= 2.f;
			jointItr->offsetChannels[1] /= 2.f;
			jointItr->offsetChannels[2] /= 2.f;
		}
		jointItr++;
	}

	// then scale the hips translation values - can produce unexpected results
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) {

		for(int i=0;i<3;i++)
		{
			vector<GLfloat>::iterator dataItr = motionItr->data.begin()+i;
			if(bScaleUp) *dataItr *=2.f;
			else *dataItr /=2.f;
		}
		motionItr++;
	}
}

void CBVHTweaker::QualityCheck(void)
{
	// check through data for each joint looking for exceeded human limits and log to file
	CQCFile* results = new CQCFile;
	bool bShinCheck = true;
	bool bFootCheck = true;
	results->Initialise();
	float fShinTolerance = 12.f;
	float fFootTolerance = 25.f;
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = _T("");

	float fAnimationLength = m_pBVHFile->GetFrameTime() * m_pBVHFile->GetNFrames() / 1000;
	float fFramesPerSecond = 1000/m_pBVHFile->GetFrameTime();
	int nFrames = m_pBVHFile->GetNFrames();
	int nJoints = m_pBVHFile->GetNJoints();
	int nArmsAlign = m_pBVHFile->GetArmsAlignment();
	CString szLogEntry;
	szLogEntry.Format("Animation report for file: %s\n\nAnimation length: %0.*fs\nAnimation rate: %0.*ffps\nNumber of frames: %d\n\n", 
		m_pBVHFile->GetFileName(), 
		1,fAnimationLength,
		0,fFramesPerSecond,
		nFrames);
	results->Log(szLogEntry);

	szLogEntry.Format("Note: the human limit tests assume that the animation joints are named using the SL joint naming convention. If the joint naming convention is wrong, human limits tests will not take place.\n\n");
	results->Log(szLogEntry);

	if(nJoints>24)
	{
		szLogEntry.Format("Warning: this animation has too many joints (%d). Ideally, there should be 24. This animation may not upload properly.\n", nJoints);
		results->LogWarning(szLogEntry);
	}

	if(nJoints<24)
	{
		szLogEntry.Format("Warning: this animation has too few joints (%d). Ideally, there should be at least 24 for uploading to Second Life.\n", nJoints);
		results->LogWarning(szLogEntry);
	}

	if(nArmsAlign!=X_AXIS_ALIGNED_ARMS) 
	{
		szLogEntry.Format("Warning: the arms are not aligned along the X axis. This will most likely cause errors with arm positioning once uploaded.\n", nJoints);
		results->LogWarning(szLogEntry);
	}

	if (nFrames>373&&nFrames<500)
	{
		szLogEntry.Format("Warning: this animation has a lot of frames (%d). Second Life may not accept it for upload.\n", nFrames);
		results->LogWarning(szLogEntry);
	}

	if (nFrames>500)
	{
		szLogEntry.Format("Warning: this animation has too many frames (%d). Second Life will not accept it for upload.\n", nFrames);
		results->LogWarning(szLogEntry);
	}

	if(fFramesPerSecond>20) 
	{
		szLogEntry.Format("Warning: this animation has a high framerate (%0.*ffps). Try using a framerate in the range 12 to 20 fps to reduce download times.\n", 
		0,fFramesPerSecond);
		results->LogWarning(szLogEntry);
	}

	if(nFrames<2) 
	{
		szLogEntry.Format("Warning: this animation only has one frame. You need at least two frames for Second Life upload. Did you forget to set a T-stance?\n");
		results->LogWarning(szLogEntry);
	}

	if(fAnimationLength>30) 
	{
		szLogEntry.Format("Warning: this animation is %0.*f seconds long. Second Life will not accept animations over 30 seconds in length.\n", 
		2,fAnimationLength);
		results->LogWarning(szLogEntry);
	}

	szLogEntry.Format("\n\n\nHuman limits report follows:\n\n");
	results->Log(szLogEntry);

	if(bShinCheck) results->Log("Checking for shin twist\n\n");
	else results->Log("Not checking for shin twist\n\n");
	if(bFootCheck) results->Log("Checking for twisted feet\n\n");
	else results->Log("Not checking for twisted feet\n\n");

	while(jointItr!=m_pBVHFile->m_joints.end()) 
	{
		szJointName = *(&jointItr->JOINTName);
		int index = jointItr->nMotionDataIndex;
		int nMotionIndex = 1;

		// go through motion data
		vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
		while(motionItr!=m_pBVHFile->m_motion.end()) 
		{
			vector<GLfloat>::iterator dataItr = motionItr->data.begin()+index;
			float fZ = *dataItr++;  
			float fX = *dataItr++;  
			float fY = *dataItr;  

			// test for ANY joint over 179.99 degrees
			if((fX > 179.99)||(fY > 179.99)||(fZ > 179.99)||(fX < -179.99)||(fY < -179.99)||(fZ < -179.99))
			{
				CString szBigError;
				szBigError.Format("Human limits: %s has rotation values over 179.99 degrees: x = %f, y = %f, z = %f at frame number %d. Is this intentional?\n", szJointName, fX, fY, fZ, nMotionIndex);
				results->LogProblem(szBigError);
			}

			if(szJointName=="abdomen")
			{
				if(fX > 26.0) results->LogProblem("Human limits: abdomen X rotation greater than 26 degrees. The skin will crease in an unnatural way.", nMotionIndex, fX);
			}

			if(szJointName=="chest")
			{
				if(fX > 6.0) results->LogProblem("Human limits: chest X rotation greater than 6 degrees. The mesh will break and protrude.", nMotionIndex, fX);
			}

			if(szJointName=="neck")
			{
				if(fX > 60.0) results->LogProblem("Human limits: neck X rotation greater than 60. I think you broke their neck.", nMotionIndex, fX);
				if(fY > 85.0) results->LogProblem("Human limits: neck Y rotation greater than 85. I think you broke their neck.", nMotionIndex, fY);
				if(fZ > 60.0) results->LogProblem("Human limits: neck Z rotation greater than 60. I think you broke their neck.", nMotionIndex, fZ);

				if(fX < -60.0) results->LogProblem("Human limits: neck X rotation less than -60. I think you broke their neck.", nMotionIndex, fX);
				if(fY < -85.0) results->LogProblem("Human limits: neck Y rotation less than -85. I think you broke their neck.", nMotionIndex, fY);
				if(fZ < -60.0) results->LogProblem("Human limits: neck Z rotation less than -60. I think you broke their neck.", nMotionIndex, fZ);
			}

			if(szJointName=="head")
			{
				if(fX > 60.0) results->LogProblem("Human limits: head X rotation greater than 60. I think you broke their neck.", nMotionIndex, fX);
				if(fY > 85.0) results->LogProblem("Human limits: head Y rotation greater than 85. I think you broke their neck.", nMotionIndex, fY);
				if(fZ > 60.0) results->LogProblem("Human limits: head Z rotation greater than 60. I think you broke their neck.", nMotionIndex, fZ);

				if(fX < -60.0) results->LogProblem("Human limits: head X rotation less than -60. I think you broke their neck.", nMotionIndex, fX);
				if(fY < -85.0) results->LogProblem("Human limits: head Y rotation less than -85. I think you broke their neck.", nMotionIndex, fY);
				if(fZ < -60.0) results->LogProblem("Human limits: head Z rotation less than -60. I think you broke their neck.", nMotionIndex, fZ);
			}

			if(szJointName=="lCollar")
			{
				//if(fX > 45.0) results->LogProblem("lCollar X rotation greater than 45. Broken collarbone.", nMotionIndex);
				//if(fY > 45.0) results->LogProblem("lCollar Y rotation greater than 45. Broken collarbone.", nMotionIndex);
				if(fZ > 75.0) results->LogProblem("Human limits: lCollar Z rotation greater than 75. Shoulder will distort.", nMotionIndex, fZ);

				//if(fX < -15) results->LogProblem("lCollar X rotation less than -15. Broken collarbone.", nMotionIndex);
				//if(fY < -45.0) results->LogProblem("lCollar Y rotation less than -45. Broken collarbone.", nMotionIndex);
				if(fZ < -50.0) results->LogProblem("Human limits: lCollar Z rotation less than -50. Shoulder will drop too low.", nMotionIndex, fZ);
			}

			if(szJointName=="lShldr")
			{
			}

			if(szJointName=="lForeArm")
			{
			}

			if(szJointName=="lHand")
			{
				//if(fX > 10.0) results->LogProblem("lHand X rotation greater than 10. Broken wrist.", nMotionIndex);
				if(fY > 50.0) results->LogProblem("Human limits: lHand Y rotation greater than 50. Broken wrist.", nMotionIndex, fY);
				if(fZ > 85.0) results->LogProblem("Human limits: lHand Z rotation greater than 85. Broken wrist.", nMotionIndex, fZ);

				//if(fX < -10.0) results->LogProblem("lHand X rotation less than -10. Broken wrist.", nMotionIndex);
				if(fY < -50.0) results->LogProblem("Human limits: lHand Y rotation less than -50. Broken wrist.", nMotionIndex, fY);
				if(fZ < -85.0) results->LogProblem("Human limits: lHand Z rotation less than -85. Broken wrist.", nMotionIndex, fZ);
			}

			if(szJointName=="rCollar")
			{
				//if(fX > 45.0) results->LogProblem("rCollar X rotation greater than 45. Broken collarbone.", nMotionIndex);
				//if(fY > 45.0) results->LogProblem("rCollar Y rotation greater than 45. Broken collarbone.", nMotionIndex);
				if(fZ > 50.0) results->LogProblem("Human limits: rCollar Z rotation greater than 50. Shoulder will drop too low.", nMotionIndex, fZ);

				//if(fX < -15) results->LogProblem("rCollar X rotation less than -15. Broken collarbone.", nMotionIndex);
				//if(fY < -45.0) results->LogProblem("rCollar Y rotation less than -45. Broken collarbone.", nMotionIndex);
				if(fZ < -75.0) results->LogProblem("Human limits: rCollar Z rotation less than -75. Shoulder will distort.", nMotionIndex, fZ);
			}

			if(szJointName=="rShldr")
			{
			}

			if(szJointName=="rForeArm")
			{
			}

			if(szJointName=="rHand")
			{
				//if(fX > 10.0) results->LogProblem("rHand X rotation greater than 10. Broken wrist.", nMotionIndex);
				if(fY > 50.0) results->LogProblem("Human limits: rHand Y rotation greater than 50. Broken wrist.", nMotionIndex, fY);
				if(fZ > 85.0) results->LogProblem("Human limits: rHand Z rotation greater than 85. Broken wrist.", nMotionIndex, fZ);

				//if(fX < -10.0) results->LogProblem("rHand X rotation less than -10. Broken wrist.", nMotionIndex);
				if(fY < -50.0) results->LogProblem("Human limits: rHand Y rotation less than -50. Broken wrist.", nMotionIndex, fY);
				if(fZ < -85.0) results->LogProblem("Human limits: rHand Z rotation less than -85. Broken wrist.", nMotionIndex, fZ);
			}

			if(szJointName=="lThigh")
			{
				if(fX > 80.0) results->LogProblem("Human limits: lThigh X rotation greater than 80.", nMotionIndex, fX);
				//if(fY > 90.0) results->LogProblem("lThigh Y rotation greater than 90.", nMotionIndex, fY);
				//if(fZ > 100.0) results->LogProblem("lThigh Z rotation greater than 100.", nMotionIndex, fZ);

				if(fX < -179.0) results->LogProblem("Human limits: lThigh X rotation less than -179.", nMotionIndex, fX);
				//if(fY < -90.0) results->LogProblem("lThigh Y rotation less than -90.", nMotionIndex, fY);
				//if(fZ < -60.0) results->LogProblem("lThigh Z rotation less than -60.", nMotionIndex, fZ);
			}

			if((szJointName=="lShin")&&(bShinCheck))
			{
				if(fX > 179.0) results->LogProblem("Human limits: lShin X rotation greater than 179.", nMotionIndex, fX);
				if(fY > fShinTolerance) results->LogProblem("Human limits: lShin Y rotation greater than tolerance.", nMotionIndex, fY);
				if(fZ > fShinTolerance) results->LogProblem("Human limits: lShin Z rotation greater than tolerance.", nMotionIndex, fZ);

				if(fX < -fShinTolerance) results->LogProblem("Human limits: lShin X rotation less than tolerance.", nMotionIndex, fX);
				if(fY < -fShinTolerance) results->LogProblem("Human limits: lShin Y rotation less than tolerance.", nMotionIndex, fY);
				if(fZ < -fShinTolerance) results->LogProblem("Human limits: lShin Z rotation less than tolerance.", nMotionIndex, fZ);
			}

			if((szJointName=="lFoot")&&(bFootCheck))
			{
				if(fX > 90.0) results->LogProblem("Human limits: lFoot X rotation greater than 90. Twisted ankle.", nMotionIndex, fX);
				if(fY > fFootTolerance) results->LogProblem("Human limits: lFoot Y rotation greater than tolerance. Twisted ankle.", nMotionIndex, fY);
				if(fZ > fFootTolerance) results->LogProblem("Human limits: lFoot Z rotation greater than tolerance. Twisted ankle.", nMotionIndex, fZ);

				if(fX < -(fFootTolerance*2.0)) results->LogProblem("Human limits: lFoot X rotation less than twice tolerance. Dislocated ankle.", nMotionIndex, fX);
				if(fY < -fFootTolerance) results->LogProblem("Human limits: lFoot Y rotation less than tolerance. Twisted ankle.", nMotionIndex, fY);
				if(fZ < -fFootTolerance) results->LogProblem("Human limits: lFoot Z rotation less than tolerance. Twisted ankle.", nMotionIndex, fZ);
			}

			if(szJointName=="rThigh")
			{
				if(fX > 80.0) results->LogProblem("Human limits: rThigh X rotation greater than 80.", nMotionIndex, fX);
				//if(fY > 90.0) results->LogProblem("rThigh Y rotation greater than 90.", nMotionIndex, fY);
				//if(fZ > 100.0) results->LogProblem("rThigh Z rotation greater than 100.", nMotionIndex, fZ);

				if(fX < -179.0) results->LogProblem("Human limits: rThigh X rotation less than -179.", nMotionIndex, fX);
				//if(fY < -90.0) results->LogProblem("rThigh Y rotation less than -90.", nMotionIndex, fY);
				//if(fZ < -60.0) results->LogProblem("rThigh Z rotation less than -60.", nMotionIndex, fZ);
			}

			if((szJointName=="rShin")&&(bShinCheck))
			{
				if(fX > 179.0) results->LogProblem("Human limits: rShin X rotation greater than 179.", nMotionIndex, fX);
				if(fY > fShinTolerance) results->LogProblem("Human limits: rShin Y rotation greater than tolerance.", nMotionIndex, fY);
				if(fZ > fShinTolerance) results->LogProblem("Human limits: rShin Z rotation greater than tolerance.", nMotionIndex, fZ);

				if(fX < -fShinTolerance) results->LogProblem("Human limits: rShin X rotation less than tolerance.", nMotionIndex, fX);
				if(fY < -fShinTolerance) results->LogProblem("Human limits: rShin Y rotation less than tolerance.", nMotionIndex, fY);
				if(fZ < -fShinTolerance) results->LogProblem("Human limits: rShin Z rotation less than tolerance.", nMotionIndex, fZ);
			}

			if((szJointName=="rFoot")&&(bFootCheck))
			{
				if(fX > 90.0) results->LogProblem("Human limits: rFoot X rotation greater than 90. Twisted ankle.", nMotionIndex, fX);
				if(fY > fFootTolerance) results->LogProblem("Human limits: rFoot Y rotation greater than tolerance. Twisted ankle.", nMotionIndex, fY);
				if(fZ > fFootTolerance) results->LogProblem("Human limits: rFoot Z rotation greater than tolerance. Twisted ankle.", nMotionIndex, fZ);

				if(fX < -(fFootTolerance*2.0)) results->LogProblem("Human limits: rFoot X rotation less than twice tolerance. Dislocated ankle.", nMotionIndex, fX);
				if(fY < -fFootTolerance) results->LogProblem("Human limits: rFoot Y rotation less than tolerance. Twisted ankle.", nMotionIndex, fY);
				if(fZ < -fFootTolerance) results->LogProblem("Human limits: rFoot Z rotation less than tolerance. Twisted ankle.", nMotionIndex, fZ);
			}

			nMotionIndex++;
			motionItr++;
		}
		jointItr++;
	}

	results->Close();
	delete results;
	results = NULL;
}


void CBVHTweaker::RemoveJoint(BVHJoint* pJoint)
{
	BVHJoint* pCurrentJoint = pJoint;
	BVHJoint* pParent = m_pBVHFile->GetJoint(pCurrentJoint->parentIndex);
	int bIsEndSite = false;
	int nChannels = pCurrentJoint->nChannels;
	
	if(pCurrentJoint->isEndSite) bIsEndSite = true;

	if(pCurrentJoint->isROOT) {
		AfxMessageBox(_T("You can't remove the ROOT.\nPlease select another joint and try again."),MB_ICONINFORMATION);
		pCurrentJoint = NULL;
		pParent = NULL;
		return;
	}
	//int iResponse = AfxMessageBox(_T("This may case bvhacker to crash. Sorry, not finished yet. Do you want to continue?"),MB_YESNO|MB_ICONEXCLAMATION);
	//if(iResponse==IDCANCEL||iResponse==IDNO) {
	//	pCurrentJoint = NULL;
	//	pParent = NULL;
	//	return;
	//}

	// remove motion 
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	if(!bIsEndSite) {

		while(motionItr!=m_pBVHFile->m_motion.end()) {

			// remove the three rotation values for the joint being deleted
			for(int i=pCurrentJoint->nMotionDataIndex + pCurrentJoint->nChannels - 1;
					i>=pCurrentJoint->nMotionDataIndex;i--) {

				vector<GLfloat>::iterator dataItr = motionItr->data.begin()+i;
				motionItr->data.erase(dataItr);	
			}
			motionItr++;
		}
	}

	// update all affected joints
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	int bSubsequent = true;

	while(++jointItr<m_pBVHFile->m_joints.end()) {

		// decrement hierarchy on siblings only
		if(((jointItr->heirarchy)>pCurrentJoint->heirarchy)&&(bSubsequent)) {
			jointItr->heirarchy--;
		} else bSubsequent = false;

		// set parent indices on subsequent joints
		if(jointItr->parentIndex == pCurrentJoint->index) {// if directly attached to deleted joint
			jointItr->parentIndex = pCurrentJoint->parentIndex;
		}
		else if(jointItr->parentIndex > pCurrentJoint->parentIndex) jointItr->parentIndex--;

		//// check user has not attatched joints to an End Site!
		//pParent = m_pBVHFile->GetJoint(jointItr->parentIndex);
		//if(pParent->isEndSite) {
		//	jointItr->parentIndex--;
		//	if(jointItr->parentIndex<0) jointItr->parentIndex=0;
		//}

		// decrement index on all subsequent joints
		jointItr->index--;
		if(!jointItr->isEndSite) jointItr->nMotionDataIndex-=pCurrentJoint->nChannels;
	}

	// remove the selected joint from the joints vector in pBVHFile
	int deletedJointHierarchy = pCurrentJoint->heirarchy;
	jointItr = m_pBVHFile->m_joints.begin()+pCurrentJoint->index;
	m_pBVHFile->m_joints.erase(jointItr);

	// update parameters in BVHFile
	m_pBVHFile->m_nJoints--;
	if(!bIsEndSite) m_pBVHFile->m_nParameters-=nChannels;
	motionItr = m_pBVHFile->m_motion.begin();
	m_pBVHFile->m_pCurrentMotionFrame = &(*m_pBVHFile->m_motion.begin());
	m_pBVHFile->m_nCurrentFrameIdx = 0;
	m_pBVHFile->CalculateFigureHeight();
	
	// clean up
	pCurrentJoint = NULL;
	pParent = NULL;
}

void CBVHTweaker::LPFilter()
{
	for(int i=0;i<m_pBVHFile->GetNParameters();i++)
	{
		//float* data = new float[m_pBVHFile->GetNFrames()];

		// Get a line of data
		vector<float> data;
		vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
		{
			while(motionItr!=m_pBVHFile->m_motion.end()) 
			{
				data.push_back(motionItr->data[i]);
				motionItr++;
			}
		}

		// filter the data
		CBVHLPFilter* filter = new CBVHLPFilter();
		filter->Filtering(&data);
		delete filter;

		// Write the data back
		vector<float>::iterator newDataitr = data.begin();
		motionItr = m_pBVHFile->m_motion.begin();
		{
			while(motionItr!=m_pBVHFile->m_motion.end()) 
			{
				float newval = *newDataitr;
				float oldval = motionItr->data[i];
				motionItr->data[i] = newval;
				motionItr++;
				newDataitr++;
			}
		}
	}
}

// the idea here is to provide a workaround for SL bug VWR-3783
// assuming the t-stance has been set for the first frame, we
// copy the 2nd frame and add it to the end and update the number of frames.
// We then calculate the In(%) value the user will need when uploading and 
// display it to them.
void CBVHTweaker::FixSLLoopBug(bool bAlreadySetUp)
{
	int iResponse = 0;

	if(!bAlreadySetUp)
	{
		// ask user if they have already added the t-stance
		iResponse = AfxMessageBox(_T("Have you already added the reference frame (T-Stance)?"),MB_YESNO|MB_ICONQUESTION);
		// if no t-stance it won't work, so add it
		if(iResponse==IDNO) 
		{
			iResponse = AfxMessageBox(_T("This will not work properly without the reference frame.\nAdd it now?"),MB_YESNO|MB_ICONQUESTION);
			if(iResponse==IDNO)
			{
				AfxMessageBox(_T("Cannot continue without reference frame. Aborting operation."),MB_ICONEXCLAMATION);
				return;
			}
			SetTShape();
		}
	}

	// Copy 2nd frame, paste onto end of motion data
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	motionItr++;
	BVHMotion motionDataLineToCopy = *motionItr;
	m_pBVHFile->m_motion.insert(m_pBVHFile->m_motion.end(),motionDataLineToCopy);
	//float oldNFrames = m_pBVHFile->GetNFrames();// new for Moon Metty's workaround, doesn't seem to work so well...
	m_pBVHFile->m_nFrames++;
	m_pBVHFile->SetCurrentMotionFrame(0);

	// I hate C++ string handling!
	float fInPercentValue = (100.f/(float)m_pBVHFile->GetNFrames())*2.f;
	//float fInPercentValue = 200.f / (oldNFrames+2.f);
	CString szInPercentValue;
	szInPercentValue.Format("%0.*f", 4, fInPercentValue);
	CString message;
	message.Format("In(%%) value to use for upload = %0.*f\nWould you like to copy this value to the clipboard?", 4, fInPercentValue);
	
	// Display In(%) value to user and offer to copy to clipboard
	if(!bAlreadySetUp) iResponse = AfxMessageBox(_T(message),MB_YESNO|MB_ICONQUESTION);
	else iResponse=IDYES;

	if(iResponse==IDYES)
	{
		// clipboard copy code from http://mc-computing.com/code/CopyToClipboard.cpp
		HGLOBAL      temp_Handle ;    // The variable type is case sensitive
		char*         temp_ptr ;
		OpenClipboard(0);         // 0 means no window
		temp_Handle =  GlobalAlloc (GMEM_MOVEABLE + GMEM_DDESHARE,
					   strlen(szInPercentValue) + 1 );
		temp_ptr = (char*)GlobalLock(temp_Handle);
		memcpy (temp_ptr, szInPercentValue, strlen(szInPercentValue)+1);
		GlobalUnlock(temp_Handle);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, temp_Handle);
		CloseClipboard();
	}
}

void CBVHTweaker::SquareShoulders(bool up)
{
	float fAdjustment = -5.f;
	if(up) fAdjustment = 5.f;
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) {

		if((jointItr->JOINTName ==_T("lCollar"))||
		   (jointItr->JOINTName ==_T("LeftShoulder")))
		{
			MotionDataAdd(&(*jointItr),0.f,0.f,fAdjustment);
		}
		if((jointItr->JOINTName ==_T("lShldr"))||
		   (jointItr->JOINTName ==_T("LeftArm")))
		{
			MotionDataAdd(&(*jointItr),0.f,0.f,-fAdjustment);
		}
		if((jointItr->JOINTName ==_T("rCollar"))||
		   (jointItr->JOINTName ==_T("RightShoulder")))
		{
			MotionDataAdd(&(*jointItr),0.f,0.f,-fAdjustment);
		}
		if((jointItr->JOINTName ==_T("rShldr"))||
		   (jointItr->JOINTName ==_T("RightArm")))
		{
			MotionDataAdd(&(*jointItr),0.f,0.f,fAdjustment);
		}
		jointItr++;
	}
}

void CBVHTweaker::SetBlendPose()
{
	// ask user if they have already added the t-stance
	int iResponse = AfxMessageBox(_T("Have you already added the reference frame (T-Stance)?"),MB_YESNO|MB_ICONQUESTION);
	
	// if no t-stance it won't work, so add it
	if(iResponse==IDNO) 
	{
		iResponse = AfxMessageBox(_T("This will not work properly without the reference frame.\nAdd it now?"),MB_YESNO|MB_ICONQUESTION);
		if(iResponse==IDNO)
		{
			AfxMessageBox(_T("Cannot continue without reference frame. Aborting operation."),MB_ICONEXCLAMATION);
			return;
		}
	}

	// first make sure the animation's hips offset matches the t-stance
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	BVHMotion motionDataLineToCopy = *motionItr++;
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// just copy over the hips offset
		motionItr->data[1] = motionDataLineToCopy.data[1];
		motionItr->data[2] = motionDataLineToCopy.data[2];
		motionItr->data[3] = motionDataLineToCopy.data[3];
		motionItr++;
	}

	// now set the fps to one frame per second
	m_pBVHFile->SetFrameTime(1000);
}

// makes the figure's starting point in centre x,z
void CBVHTweaker::CentreFigure(void)
{
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	float fX = motionItr->data[0];
	//float fY = motionItr->data[1];
	float fZ = motionItr->data[2];

	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		motionItr->data[0]-=fX;
		//motionItr->data[1]-=fY;
		motionItr->data[2]-=fZ;
		motionItr++;
	}
}

void CBVHTweaker::HalfSample(void)
{
	// get original values
	int   nFrames = m_pBVHFile->GetNFrames();
	float fFrameTime = m_pBVHFile->GetFrameTime();
	float fTotalLengthMS = fFrameTime*(float)nFrames;

	// sanity check
	if(nFrames<3)
	{
		AfxMessageBox(_T("There are not enough frames to perform resampling"),MB_ICONEXCLAMATION);
		return;
	}

	// first create a new vector to hold the new data values
	vector<BVHMotion> newMotionValues;

	// iterate through the original data taking every other value
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		newMotionValues.push_back(*motionItr);
		motionItr++;
		if(motionItr!=m_pBVHFile->m_motion.end()) motionItr++;
	}

	// now replace the old one with the new one
	m_pBVHFile->m_motion.clear();
	m_pBVHFile->m_motion = newMotionValues;

	// adjust the frame time and number of frames
	nFrames/=2;
	m_pBVHFile->SetNFrames(nFrames);

	fFrameTime=fTotalLengthMS/(float)nFrames;
	m_pBVHFile->SetFrameTime(fFrameTime);
}

// see: http://en.wikipedia.org/wiki/Rotation_matrix
void CBVHTweaker::MatrixRotateZ(BVHJoint* pJoint, int sign)
{
	// set up rotation matirx
	float fRotationMatrix[9];
	float fXUnit=1.f;
	float fYUnit=0.f;
	float fZUnit=0.f;

	// joint offsets (order checked and OK)
	float fXoff = pJoint->offsetChannels[2];
	float fYoff = pJoint->offsetChannels[1];
	float fZoff = pJoint->offsetChannels[0];

	float fNewXoff = fXoff;
	float fNewYoff = fYoff;
	float fNewZoff = fZoff;



	// transform the joint's offsets 90 degrees about Z 
	fRotationMatrix[0] =  0; fRotationMatrix[3] = -1; fRotationMatrix[6] = 0;
	fRotationMatrix[1] =  1; fRotationMatrix[4] =  0; fRotationMatrix[7] = 0;
	fRotationMatrix[2] =  0; fRotationMatrix[5] =  0; fRotationMatrix[8] = 1;

	// multiply matrix by offset vectors for new offset values
	fNewXoff = (fRotationMatrix[0]*fXoff)+(fRotationMatrix[3]*fYoff)+(fRotationMatrix[6]*fZoff);
	fNewYoff = (fRotationMatrix[1]*fXoff)+(fRotationMatrix[4]*fYoff)+(fRotationMatrix[7]*fZoff);
	fNewZoff = (fRotationMatrix[2]*fXoff)+(fRotationMatrix[5]*fYoff)+(fRotationMatrix[8]*fZoff);

	pJoint->offsetChannels[2] = fNewXoff*sign;
	pJoint->offsetChannels[1] = fNewYoff;
	pJoint->offsetChannels[0] = fNewZoff;

	// Finally, store new joint's offsets
	fXoff = pJoint->offsetChannels[2];
	fYoff = pJoint->offsetChannels[1];
	fZoff = pJoint->offsetChannels[0];

	return;

	// transform the rotation data
	float fDataVector[3];
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// get the current rotation values and apply to a unit vector
		if(pJoint->channelOrder!=NONE)
		{
			fXUnit=1.f;
			fYUnit=0.f;
			fZUnit=0.f;
			switch(pJoint->channelOrder)
			{
				case(ZXY):
				{
					// order checked and OK
					fDataVector[0] = motionItr->data[pJoint->nMotionDataIndex+1];//X
					fDataVector[1] = motionItr->data[pJoint->nMotionDataIndex+2];//Y
					fDataVector[2] = motionItr->data[pJoint->nMotionDataIndex+0];//Z
					break;
				}
				case(ZYX):
				{
					// order has already been changed
					fDataVector[0] = motionItr->data[pJoint->nMotionDataIndex+2];//X
					fDataVector[1] = motionItr->data[pJoint->nMotionDataIndex+1];//Y
					fDataVector[2] = motionItr->data[pJoint->nMotionDataIndex+0];//Z
					break;
				}
				default:
				{
					int stop = true;
				}
			}
		}
		else 
		{
			// probably just an End Site, so no rot data
			fDataVector[0] = 0.f;
			fDataVector[1] = 0.f;
			fDataVector[2] = 0.f;
		}




		//// work out a position vector from the rotation data
		//// Z (2)
		//float fSinTheta = sin((fDataVector[2])*((float)N_PI/180));
		//float fCosTheta = cos((fDataVector[2])*((float)N_PI/180));
		//if(fSinTheta>-0.00001&&fSinTheta<0.00001) fSinTheta=0.f;
		//if(fCosTheta>-0.00001&&fCosTheta<0.00001) fCosTheta=0.f;

		//fRotationMatrix[0] =  fCosTheta; fRotationMatrix[3] = -fSinTheta; fRotationMatrix[6] = 0.f;
		//fRotationMatrix[1] =  fSinTheta; fRotationMatrix[4] =  fCosTheta; fRotationMatrix[7] = 0.f;
		//fRotationMatrix[2] =  0.f      ; fRotationMatrix[5] =  0.f      ; fRotationMatrix[8] = 1.f;

		//fNewXoff = (fRotationMatrix[0]*fXUnit)+(fRotationMatrix[3]*fYUnit)+(fRotationMatrix[6]*fZUnit);
		//fNewYoff = (fRotationMatrix[1]*fXUnit)+(fRotationMatrix[4]*fYUnit)+(fRotationMatrix[7]*fZUnit);
		//fNewZoff = (fRotationMatrix[2]*fXUnit)+(fRotationMatrix[5]*fYUnit)+(fRotationMatrix[8]*fZUnit);

		//if(fNewXoff>-0.00001&&fNewXoff<0.00001) fNewXoff=0.f;
		//if(fNewYoff>-0.00001&&fNewYoff<0.00001) fNewYoff=0.f;
		//if(fNewZoff>-0.00001&&fNewZoff<0.00001) fNewZoff=0.f;

		////fXoff = fNewXoff*fCosTheta-fNewYoff*fSinTheta;
		////fYoff = fNewYoff*fCosTheta+fNewXoff*fSinTheta;
		////fZoff = fNewZoff;

		//fXUnit = fNewXoff*fCosTheta-fNewYoff*fSinTheta;
		//fYUnit = fNewYoff*fCosTheta+fNewXoff*fSinTheta;
		//fZUnit = fNewZoff;

		////fXUnit = fNewXoff;
		////fYUnit = fNewYoff;
		////fZUnit = fNewZoff;

		////fXoff = fNewXoff;
		////fYoff = fNewYoff;
		////fZoff = fNewZoff;



		// work out the position vector after rotation data applied
		// X			 now X (1)->(0)
		float fSinTheta = sin((fDataVector[1])*((float)N_PI/180));
		float fCosTheta = cos((fDataVector[1])*((float)N_PI/180));
		if(fSinTheta>-0.00001&&fSinTheta<0.00001) fSinTheta=0.f;
		if(fCosTheta>-0.00001&&fCosTheta<0.00001) fCosTheta=0.f;

		fRotationMatrix[0] =  1.f      ; fRotationMatrix[3] =  0.f      ; fRotationMatrix[6] =  0.f;
		fRotationMatrix[1] =  0.f      ; fRotationMatrix[4] =  fCosTheta; fRotationMatrix[7] = -fSinTheta;
		fRotationMatrix[2] =  0.f      ; fRotationMatrix[5] =  fSinTheta; fRotationMatrix[8] =  fCosTheta;

		fNewXoff = (fRotationMatrix[0]*fXUnit)+(fRotationMatrix[3]*fYUnit)+(fRotationMatrix[6]*fZUnit);
		fNewYoff = (fRotationMatrix[1]*fXUnit)+(fRotationMatrix[4]*fYUnit)+(fRotationMatrix[7]*fZUnit);
		fNewZoff = (fRotationMatrix[2]*fXUnit)+(fRotationMatrix[5]*fYUnit)+(fRotationMatrix[8]*fZUnit);

		if(fNewXoff>-0.00001&&fNewXoff<0.00001) fNewXoff=0.f;
		if(fNewYoff>-0.00001&&fNewYoff<0.00001) fNewYoff=0.f;
		if(fNewZoff>-0.00001&&fNewZoff<0.00001) fNewZoff=0.f;

		//fXoff = fNewXoff;
		//fYoff = fNewYoff*fCosTheta-fNewZoff*fSinTheta;
		//fZoff = fNewZoff*fCosTheta+fNewYoff*fSinTheta;

		fXUnit = fNewXoff;
		fYUnit = fNewYoff*fCosTheta-fNewZoff*fSinTheta;
		fZUnit = fNewZoff*fCosTheta+fNewYoff*fSinTheta;

		//fXoff = fNewXoff;
		//fYoff = fNewYoff;
		//fZoff = fNewZoff;

		//fXUnit = fNewXoff;
		//fYUnit = fNewYoff;
		//fZUnit = fNewZoff;	


		// work out the position vector after rotation data applied
		//  Y			now Y (0)->(1)
		fSinTheta = sin((fDataVector[0])*((float)N_PI/180));
		fCosTheta = cos((fDataVector[0])*((float)N_PI/180));
		if(fSinTheta>-0.00001&&fSinTheta<0.00001) fSinTheta=0.f;
		if(fCosTheta>-0.00001&&fCosTheta<0.00001) fCosTheta=0.f;

		fRotationMatrix[0] =  fCosTheta; fRotationMatrix[3] =  0.f      ; fRotationMatrix[6] = fSinTheta;
		fRotationMatrix[1] =  0.f      ; fRotationMatrix[4] =  1.f      ; fRotationMatrix[7] = 0.f;
		fRotationMatrix[2] = -fSinTheta; fRotationMatrix[5] =  0.f      ; fRotationMatrix[8] = fCosTheta;

		fNewXoff = (fRotationMatrix[0]*fXUnit)+(fRotationMatrix[3]*fYUnit)+(fRotationMatrix[6]*fZUnit);
		fNewYoff = (fRotationMatrix[1]*fXUnit)+(fRotationMatrix[4]*fYUnit)+(fRotationMatrix[7]*fZUnit);
		fNewZoff = (fRotationMatrix[2]*fXUnit)+(fRotationMatrix[5]*fYUnit)+(fRotationMatrix[8]*fZUnit);

		if(fNewXoff>-0.00001&&fNewXoff<0.00001) fNewXoff=0.f;
		if(fNewYoff>-0.00001&&fNewYoff<0.00001) fNewYoff=0.f;
		if(fNewZoff>-0.00001&&fNewZoff<0.00001) fNewZoff=0.f;

		//fXoff = fNewXoff*fCosTheta+fNewZoff*fSinTheta;
		//fYoff = fNewYoff;
		//fZoff = fNewZoff*fCosTheta-fNewXoff*fSinTheta;

		fXUnit = fNewXoff*fCosTheta+fNewZoff*fSinTheta;
		fYUnit = fNewYoff;
		fZUnit = fNewZoff*fCosTheta-fNewXoff*fSinTheta;

		//fXoff = fNewXoff;
		//fYoff = fNewYoff;
		//fZoff = fNewZoff;

/*		fXUnit = fNewXoff;
		fYUnit = fNewYoff;
		fZUnit = fNewZoff;*/	










		// now work out the rotations given the new postition data
		if(pJoint->channelOrder!=NONE) // don't apply to End Sites
			DoRotations(&(*motionItr), pJoint->nMotionDataIndex, fXUnit, fYUnit, fZUnit);

		motionItr++;
	}
}


// calculate new rotations based on supplied offsets
void CBVHTweaker::DoRotations(BVHMotion* motionData, int nMotionDataIndex, float fXoff, float fYoff, float fZoff)
{
	// asin(1) = 90

	float fXrot = 0.f;
	float fYrot = 0.f;
	float fZrot = 0.f;			

	if(fZoff<0) fXrot = asin(fZoff/1.f)/((float)N_PI/180);
	else if(fZoff>0) fXrot = asin(fZoff/(-1*1.f))/((float)N_PI/180);
	else fXrot = 0.f;	

	if(fXoff<0) fYrot = asin(fXoff/1.f)/((float)N_PI/180);
	else if(fXoff>0) fYrot = asin(fXoff/(-1*1.f))/((float)N_PI/180);
	else fYrot = 0.f;		

	if(fYoff<0) fZrot = asin(fYoff/1.f)/((float)N_PI/180);
	else if(fYoff>0) fZrot = asin(fYoff/(-1*1.f))/((float)N_PI/180);
	else fZrot = 0.f;

	//fXrot+=90.f;// adjust for initial joint adjustment 
	//fYrot+=90.f;// adjust for initial joint adjustment 
	fZrot+=90.f;// adjust for initial joint adjustment 

	// update the rot data. Swap channels here
	motionData->data[nMotionDataIndex+0]=fZrot;
	motionData->data[nMotionDataIndex+1]=fXrot;
	motionData->data[nMotionDataIndex+2]=fYrot;
}


// next two functions are experimental and not working...
void CBVHTweaker::HackArmsAlignmentAxis(BVHJoint* pJoint, float X, float Y, float Z, int signX, int signY, int signZ, int channelOrder)
{
	MotionDataAdd(X,Y,Z,signX,signY,signZ,pJoint->nMotionDataIndex,pJoint->channelOrder);	
	RearrangeMotionChannels(pJoint, channelOrder);// does not set channel order on joint, just changes order of data
}
// nasty hack, better to use CBVHQuaterniser version
void CBVHTweaker::HackArmsAlignmentAxis(void)
{
	if(m_pBVHFile->CalculateArmsAxisAlignment()==X_AXIS_ALIGNED_ARMS)
	{
		AfxMessageBox(_T("The figure's arms are already aligned along the X axis"),MB_ICONEXCLAMATION);
		return;
	}

	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	CString szJointName = _T("");
	int nChestIndex = 0; // for error checking when assigning collars / shoulders / clavicles

	while(jointItr!=m_pBVHFile->m_joints.end()) {

		szJointName = jointItr->JOINTName.GetBuffer();


		// left arm
		if(szJointName== _T("lShldr"))
		{
			// assume ZXY
			MotionDataInvertChannel(jointItr->nMotionDataIndex+2);// invert Y
			for(int i=0;i<m_pBVHFile->GetNFrames();i++)
				MotionFrameAdd(&(*jointItr), 0.f, 0.f, -90.f, i);// adjust Z for new alignment
			MotionDataSwapChannels(jointItr->nMotionDataIndex+1,jointItr->nMotionDataIndex+2);// swap X and Y
			//jointItr->channelOrder=ZYX;
		}

		if(szJointName== _T("lForeArm"))
		{
			// apply correction
			MatrixRotateZ(&(*jointItr));
			
			// assume ZXY
			MotionDataSwapChannels(jointItr->nMotionDataIndex+1,jointItr->nMotionDataIndex+2);// swap X and Y
			MotionDataInvertChannel(jointItr->nMotionDataIndex+2);// invert Y

			//jointItr->channelOrder=ZXY;
		}

		if(szJointName== _T("lHand"))
		{
			// apply correction
			MatrixRotateZ(&(*jointItr));

			// assume ZXY
			MotionDataSwapChannels(jointItr->nMotionDataIndex+1,jointItr->nMotionDataIndex+2);// swap X and Y
			MotionDataInvertChannel(jointItr->nMotionDataIndex+2);// invert Y
			//jointItr->channelOrder=ZXY;
			
			// End Site
			jointItr++;
			MatrixRotateZ(&(*jointItr));
			jointItr--;
		}

		// right arm
		if(szJointName == _T("rShldr"))
		{
			// assume ZXY
			MotionDataInvertChannel(jointItr->nMotionDataIndex+2);// invert Y
			for(int i=0;i<m_pBVHFile->GetNFrames();i++)
				MotionFrameAdd(&(*jointItr), 0.f, 0.f, 90.f, i);// adjust Z for new alignment
			MotionDataSwapChannels(jointItr->nMotionDataIndex+1,jointItr->nMotionDataIndex+2);// swap X and Y
			//jointItr->channelOrder=ZYX;
		}

		if(szJointName == _T("rForeArm"))
		{
			// apply correction
			MatrixRotateZ(&(*jointItr), -1);
			
			// assume ZXY
			MotionDataSwapChannels(jointItr->nMotionDataIndex+1,jointItr->nMotionDataIndex+2);// swap X and Y
			MotionDataInvertChannel(jointItr->nMotionDataIndex+2);// invert Y
			//jointItr->channelOrder=ZXY;
		}

		if(szJointName == _T("rHand"))
		{
			// apply correction
			MatrixRotateZ(&(*jointItr), -1);

			// assume ZXY
			MotionDataSwapChannels(jointItr->nMotionDataIndex+1,jointItr->nMotionDataIndex+2);// swap X and Y
			MotionDataInvertChannel(jointItr->nMotionDataIndex+2);// invert Y
			//jointItr->channelOrder=ZXY;

			// End Site
			jointItr++;
			MatrixRotateZ(&(*jointItr), -1);
			jointItr--;
		}

		jointItr++;
	}

	m_pBVHFile->CalculateArmsAxisAlignment();
}

void CBVHTweaker::CombineChestAbdomen()
{
	BVHJoint* pChestJoint = NULL;
	BVHJoint* pAbdomenJoint = NULL;
	Vector3<float> vChestValues(0.f,0.f,0.f);
	Vector3<float> vAbdomenValues(0.f,0.f,0.f);
	Vector3<float> vCombinedValues(0.f,0.f,0.f);
	Vector3<float> vAbdomenOffsets(0.f,0.f,0.f);

	float fChestStiffness = 0.3f;// i.e. chest gets 30% of total rotation
	
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) 
	{
		if(jointItr->internalName ==_T("chest"))
		{
			pChestJoint = &(*jointItr);		
		}
		if(jointItr->internalName ==_T("abdomen"))
		{
			pAbdomenJoint = &(*jointItr);		
			vAbdomenOffsets.z=pAbdomenJoint->offsetChannels[0];
			vAbdomenOffsets.y=pAbdomenJoint->offsetChannels[1];
			vAbdomenOffsets.x=pAbdomenJoint->offsetChannels[2];
		}
		jointItr++;
	}
	if (pChestJoint==NULL||pAbdomenJoint==NULL)
	{
		AfxMessageBox(_T("bvhacker is unable to find the joints necessary for this operation - I don't recognise the joint naming convention"), MB_ICONSTOP);
		return;//can't find chest or abdomen? then return
	}
	
	// now combine all the data on the chest
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// get values for both joints and combine
		vChestValues.x=motionItr->data[pChestJoint->nMotionDataIndex+0];
		vChestValues.y=motionItr->data[pChestJoint->nMotionDataIndex+1];
		vChestValues.z=motionItr->data[pChestJoint->nMotionDataIndex+2];

		vAbdomenValues.x=motionItr->data[pAbdomenJoint->nMotionDataIndex+0];
		vAbdomenValues.y=motionItr->data[pAbdomenJoint->nMotionDataIndex+1];
		vAbdomenValues.z=motionItr->data[pAbdomenJoint->nMotionDataIndex+2];

		vCombinedValues=vChestValues+vAbdomenValues;

		// write new values
		motionItr->data[pChestJoint->nMotionDataIndex+0]=vCombinedValues.x;
		motionItr->data[pChestJoint->nMotionDataIndex+1]=vCombinedValues.y;
		motionItr->data[pChestJoint->nMotionDataIndex+2]=vCombinedValues.z;

		motionItr++;
	}

	// add the abdomen's offsets to the chest's
	pChestJoint->offsetChannels[0]+=vAbdomenOffsets.z;
	pChestJoint->offsetChannels[1]+=vAbdomenOffsets.y;
	pChestJoint->offsetChannels[2]+=vAbdomenOffsets.x;

	// remove the abdomen
	RemoveJoint(pAbdomenJoint);
	
	pChestJoint = NULL;
	pAbdomenJoint = NULL;
}

void CBVHTweaker::CraneNeck(bool bStraighten)
{
	float fAdjustment = -2.f;
	if(bStraighten) fAdjustment = 2.f;
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) {

		if((jointItr->JOINTName ==_T("Neck"))||
			(jointItr->internalName ==_T("neck")))
		{
			MotionDataAdd(&(*jointItr),fAdjustment,0.f,0.f);
		}
		if((jointItr->JOINTName ==_T("Head"))||
		   (jointItr->JOINTName ==_T("head")))
		{
			MotionDataAdd(&(*jointItr),-fAdjustment,0.f,0.f);
		}
		jointItr++;
	}
}


void CBVHTweaker::Reverse(void)
{
	vector<BVHMotion> vReversedMotion;
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.end();
	motionItr--;
	while(motionItr!=m_pBVHFile->m_motion.begin()) 
	{
		vReversedMotion.push_back(*motionItr);
		motionItr--;
	}
	vReversedMotion.push_back(*motionItr);
	m_pBVHFile->m_motion = vReversedMotion;
}


void CBVHTweaker::DeleteDescendants()//BVHJoint* pJoint)
{
	// get ptr to the currently selected joint
	BVHJoint* pJoint = m_pBVHFile->GetSelectedJoint();
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	bool bSaveEndSite = true;

	// iterate through to the currently selected joint
	while(&(*jointItr)!=pJoint) jointItr++; 
	jointItr++; // don't want to delete the selected joint, just it's descendants
	int nJointIndex = jointItr->index;
	int nJointHeirarchy = jointItr->heirarchy;

	// now delete all ensuing joints with a higher hierarchy
	while(jointItr->heirarchy>pJoint->heirarchy)
	{		
		// delete the joint unless it's the first End Site
		if(jointItr->isEndSite&&bSaveEndSite)
		{
			bSaveEndSite = false;
			nJointIndex++;
			if(nJointIndex>(m_pBVHFile->GetNJoints()-1)) 
				nJointIndex = 0; // we have overrun the last joint, so need to point at ROOT
			jointItr = m_pBVHFile->m_joints.begin()+nJointIndex;
		} 
		else 
		{
			RemoveJoint(&(*jointItr));
			// now our iterator is fucked, so we need to reassign
			if(nJointIndex>(m_pBVHFile->GetNJoints()-1)) 
				nJointIndex = 0; // we have deleted the last joint, so need to point at ROOT
			jointItr = m_pBVHFile->m_joints.begin()+nJointIndex;
			nJointHeirarchy = jointItr->heirarchy;
		}
	}
}


void CBVHTweaker::ZeroJoint(CString szJointName, int mode)
{
	ZeroOutChannel(N_X, mode, szJointName);
	ZeroOutChannel(N_Y, mode, szJointName);
	ZeroOutChannel(N_Z, mode, szJointName);

	// if zeroing the hips in translation, set the waist height
	BVHJoint* pJoint = GetJointByName(szJointName); // m_pBVHFile->GetJoint(szJointName);
	if(pJoint->isROOT&&mode==MODE_TRANSLATING) 
		MotionDataAdd(pJoint,0.f,(float)m_pBVHFile->GetGroundHeight(),0.f, true);
}


void CBVHTweaker::ZeroOutToEnd(CString szJointStart)
{
	// get a ptr to our joint
	BVHJoint* pJoint;
	
	if(szJointStart.Compare(_T("current"))==0) pJoint = m_pBVHFile->GetSelectedJoint();
	else pJoint = GetJointByName(szJointStart);

	// iterate through until we get to our joint of interest
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(&(*jointItr)!=pJoint) jointItr++;
	jointItr++; // command is zero descendants - don't want to zero out selected joint

	// now zero out subsequent joints
	while(jointItr!=m_pBVHFile->m_joints.end())
	{
		if(jointItr->heirarchy <= pJoint->heirarchy) return;// branch drilldown complete

		ZeroJoint(jointItr->JOINTName, MODE_ROTATING);
		jointItr++;
	}
}


BVHJoint* CBVHTweaker::GetJointByName(CString szJointName)
{
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end()) 
	{
		if(jointItr->JOINTName==szJointName) return &(*jointItr);
		if(jointItr->internalName==szJointName) return &(*jointItr);
		jointItr++;
	}
	return NULL;
}


void CBVHTweaker::CombineWithParent(void)
{
	// get pointers to the three joints
	BVHJoint* pCurrentJoint = m_pBVHFile->GetSelectedJoint();
	if(pCurrentJoint->isROOT) 
	{
		pCurrentJoint = NULL;
		AfxMessageBox(_T("The root has no parent - unable to combine"), MB_ICONSTOP);
		return;
	}
	BVHJoint* pParent = m_pBVHFile->GetJoint(pCurrentJoint->parentIndex);
	if (pCurrentJoint->channelOrder!=pParent->channelOrder)
	{
		AfxMessageBox(_T("Problem: the joint and it's parent have different rotation orders - cannot continue"), MB_ICONSTOP);
		pCurrentJoint = NULL;
		pParent = NULL;
		return;
	}
	if (pCurrentJoint==NULL||pParent==NULL)
	{
		AfxMessageBox(_T("Error: bvhacker is unable to find the joints necessary for this operation"), MB_ICONSTOP);
		pCurrentJoint = NULL;
		pParent = NULL;
		return;//can't find joints
	}

	// how not to use the STL:
	Vector3<float> vCurrentJointOffsets(0.f,0.f,0.f);
	Vector3<float> vDescendantJointOffsets(0.f,0.f,0.f);
	Vector3<float> vCurrentJointRotation(0.f,0.f,0.f);
	Vector3<float> vParentJointRotation(0.f,0.f,0.f);
	Vector3<float> vCombinedValues(0.f,0.f,0.f);

	// save the current offsets 
	vCurrentJointOffsets.z=pCurrentJoint->offsetChannels[0];
	vCurrentJointOffsets.y=pCurrentJoint->offsetChannels[1];
	vCurrentJointOffsets.x=pCurrentJoint->offsetChannels[2];
	
	// now combine all the rotation data on the parent
	vector<BVHMotion>::iterator motionItr = m_pBVHFile->m_motion.begin();
	while(motionItr!=m_pBVHFile->m_motion.end()) 
	{
		// get values for both joints and combine
		vCurrentJointRotation.x=motionItr->data[pCurrentJoint->nMotionDataIndex+0];
		vCurrentJointRotation.y=motionItr->data[pCurrentJoint->nMotionDataIndex+1];
		vCurrentJointRotation.z=motionItr->data[pCurrentJoint->nMotionDataIndex+2];

		vParentJointRotation.x=motionItr->data[pParent->nMotionDataIndex+0];
		vParentJointRotation.y=motionItr->data[pParent->nMotionDataIndex+1];
		vParentJointRotation.z=motionItr->data[pParent->nMotionDataIndex+2];

		vCombinedValues=vCurrentJointRotation+vParentJointRotation;

		// write new values
		motionItr->data[pParent->nMotionDataIndex+0]=vCombinedValues.x;
		motionItr->data[pParent->nMotionDataIndex+1]=vCombinedValues.y;
		motionItr->data[pParent->nMotionDataIndex+2]=vCombinedValues.z;

		motionItr++;
	}

	// add the selected joint's offsets to all decendants
	// iterate through until we get to our joint of interest
	vector<BVHJoint>::iterator jointItr = m_pBVHFile->m_joints.begin();
	while(jointItr!=m_pBVHFile->m_joints.end())
	{
		if(jointItr->parentIndex==pCurrentJoint->index)
		{
			jointItr->offsetChannels[0]+=vCurrentJointOffsets.z;
			jointItr->offsetChannels[1]+=vCurrentJointOffsets.y;
			jointItr->offsetChannels[2]+=vCurrentJointOffsets.x;
		}
		jointItr++;
	}

	// 
	//BVHJoint* pDesendant = m_pBVHFile->GetJoint(pCurrentJoint->index+1);


	// 

	// remove the abdomen
	RemoveJoint(pCurrentJoint);
	
	pCurrentJoint = NULL;
	pParent = NULL;
}
