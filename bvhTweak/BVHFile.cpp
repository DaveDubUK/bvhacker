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
#include "BVHFile.h"
//#include <math.h>
#include "math.h"


CBVHFile::CBVHFile(void)
: m_nFrames(0)
, m_fFrameTime(0)
, m_pCurrentMotionFrame(NULL)
, m_nCurrentFrameIdx(0)
, m_fGroundHeight(0)
, m_nJoints(0)
, m_FileName(_T(""))
, m_nParameters(0)
, m_nFigureHeightMax(0)
, m_nFigureHeightMin(0)
, m_Skeleton(NULL)
{
}

CBVHFile::~CBVHFile(void)
{
}

bool CBVHFile::LoadBVH(CString FilePath)
{
	//char szFilePath[MAX_PATH];
	//strcpy(szFilePath, (LPCTSTR) FilePath );
	CStdioFile* pFile = new CStdioFile((LPCTSTR)FilePath, CFile::modeRead | CFile::typeText);
	CString line = NULL;
	CString lastChar = NULL; // used for curlies counting
	int jointNo = 0;
	int motionNo = 0;
	int nHeirarchyLevel = 0;
	int nParent = 0; 
	int nJointsSinceCloseCurlies = 0;
	m_nParameters = 0; // is each join's motion data offset member counter during load

	// store away the filename
	TCHAR* dblSlashToken = _T("\\");
	CString tcs;
	m_FileName = _T("");
	int rf = FilePath.ReverseFind(*dblSlashToken);
	dblSlashToken = NULL;
	int len = FilePath.GetLength();
	for(int i=rf+1;i<len;i++) {
		tcs = FilePath.GetAt(i);
		m_FileName.Append(tcs);
	}

	// read the first line
	bool success = ReadLine(pFile, &line);
	if(!success) return false;

	while(!feof(pFile->m_pStream)) {
		
		// for referring back to previous joints
		vector<BVHJoint>::iterator jointsItr = m_joints.begin();

		// Read in the bone structure
		if(line.Compare(_T("HIERARCHY"))==0) {

			bool success = ReadLine(pFile, &line);
			if(!success) return false;

			while(line.Compare(_T("MOTION"))!=0) {

				BVHJoint newJoint;
				newJoint.channelOrder = NONE;
				newJoint.offsetChannelOrder = NONE;
				newJoint.scaleChannelOrder = NONE;
				newJoint.nMotionDataIndex = 0;
				newJoint.nOffsetMotionDataIndex = 0;
				newJoint.nScaleMotionDataIndex = 0;
				CString word = _T("");
				CString tcs;
				int f = Delimit(line);
				for(int i = 0;i<f;i++) {
					tcs = line.GetAt(i);
					word.Append(tcs);
				}
				word.TrimLeft();
				word.TrimRight();

				if(word.Compare(_T("ROOT"))==0) {
					newJoint.isROOT = true;
					newJoint.isEndSite = false;
					newJoint.parentIndex = 0;
					newJoint.heirarchy = 0;
					newJoint.nMotionDataIndex = 0;
					newJoint.JOINTName = ProcessNameLine(line, jointNo);
					newJoint.internalName = AssignInternalName(newJoint.JOINTName);
					//m_nParameters+=3;
					m_nJoints++;
				}

				else if(word.Compare(_T("JOINT"))==0) {
					newJoint.isROOT = false;
					newJoint.isEndSite = false;
					newJoint.nMotionDataIndex = m_nParameters;
					newJoint.nOffsetMotionDataIndex = m_nParameters;
					newJoint.nScaleMotionDataIndex = m_nParameters;
					newJoint.JOINTName = ProcessNameLine(line, jointNo);
					newJoint.internalName = AssignInternalName(newJoint.JOINTName);
					if((1<jointNo)&&nJointsSinceCloseCurlies==1) {
						nParent = jointNo-1;
						nJointsSinceCloseCurlies=0;
					}
					if(nJointsSinceCloseCurlies==2) nJointsSinceCloseCurlies=1;
					m_nJoints++;
				}
				else if(word.Compare(_T("End"))==0) {
					newJoint.isROOT = false;
					newJoint.isEndSite = true;
					newJoint.JOINTName = _T("End Site");
					newJoint.nMotionDataIndex = m_nParameters-3;
					newJoint.nOffsetMotionDataIndex = m_nParameters-3;
					newJoint.nScaleMotionDataIndex = m_nParameters-3;
					if((1<jointNo)&&nJointsSinceCloseCurlies==1) {
						nParent = jointNo-1;
						nJointsSinceCloseCurlies=0;
					}
					if(nJointsSinceCloseCurlies==2) nJointsSinceCloseCurlies=1;
					m_nJoints++;
				}
				else return false;

				// read past the curlies (use to keep track of heirarchy)
				bool success = ReadLine(pFile, &line);
				if(!success) return false;
				lastChar = line.GetAt(line.GetLength()-1);

				int currentParentsHeirarchy = 0;
				if((nParent>-1)&&!(newJoint.isROOT))
					currentParentsHeirarchy = GetJoint(nParent)->heirarchy;

				while( (lastChar.Compare(_T("{"))==0) ||
					   (currentParentsHeirarchy>nHeirarchyLevel)) {

					if(!newJoint.isROOT) nHeirarchyLevel++;
					success = ReadLine(pFile, &line);
					if(!success) return false;
					lastChar = line.GetAt(line.GetLength()-1);
				} 
				// read OFFSET
				ProcessOffsetLine(&newJoint, line, jointNo);

				// read CHANNELS (if not end site)
				if(!newJoint.isEndSite) {
					bool success = ReadLine(pFile, &line);
					if(!success) return false;
					newJoint.CHANNELSLine=line;
					m_nParameters+=ProcessChannelLine(&newJoint,line,jointNo);
				} else {
					newJoint.nChannels = 0;
					newJoint.channelOrder = NONE;
					newJoint.nMotionDataIndex = 0;
				}
		
				// set parents & heirarchy
				if(!newJoint.isROOT) {
					newJoint.heirarchy = nHeirarchyLevel;
					newJoint.parentIndex = nParent;
				}

				// set first transform as the identity matrix
				for(int i=0;i<16;i++) newJoint.fTransformMatrix[i] = 0.0f;
				newJoint.fTransformMatrix[0] = 1.0f;
				newJoint.fTransformMatrix[5] = 1.0f;
				newJoint.fTransformMatrix[10] = 1.0f;
				newJoint.fTransformMatrix[15] = 1.0f;

				// oh, and take note of the index
				newJoint.index = jointNo;

				// then add the new joint to the joints vector
				m_joints.push_back(newJoint);
				
				///////// done parsing this joint \\\\\\\\\\\\\

				// increment the joint counter
				jointNo++;

				// read next line (skip curlies) and set next parent
				success = ReadLine(pFile, &line);
				if(!success) return false;
				lastChar = line.GetAt(line.GetLength()-1);
				bool checkHeirarchy = false;
				while(lastChar.Compare(_T("}"))==0){
					nHeirarchyLevel--;
					nParent--;
					success = ReadLine(pFile, &line);
					if(!success) return false;
					lastChar = line.GetAt(line.GetLength()-1);
					checkHeirarchy = true;
					nJointsSinceCloseCurlies=2;
				} 
				if(!GetJoint(jointNo-1)->isROOT) {
					nParent++;
					if(checkHeirarchy&&(nParent>0)) {
						while((GetJoint(nParent)->heirarchy>nHeirarchyLevel)&&(nParent>0)) {
							nParent--;
						}
					}
				}
			}
		}

		// read in the motion data header
		if(line.Compare(_T("MOTION"))==0) {

			// get the number of frames
			bool success = ReadLine(pFile, &line);
			if(!success) return false;
			CString word = _T("");
			CString tcs;
			int rf = DelimitR(line);
			for(int i = rf;i<line.GetLength();i++) {
				tcs = line.GetAt(i);
				word.Append(tcs);
			}	
			word.TrimLeft();
			word.TrimRight();
			_stscanf(word.GetBuffer(), _T("%d"), &m_nFrames);

			// now get the Frame Time
			success = ReadLine(pFile, &line);
			if(!success) return false;
			word = _T("");
			rf = DelimitR(line);
			for(int i = rf;i<line.GetLength();i++) {
				tcs = line.GetAt(i);
				word.Append(tcs);
			}	
			word.TrimLeft();
			word.TrimRight();
			_stscanf(word.GetBuffer(), _T("%f"), &m_fFrameTime);
		}

		// else it must be the actual motion data 
		else {
			bool success = ReadLine(pFile, &line);
			if(success) {
				BVHMotion newLine;
				for(int j=0;j<m_nParameters;j++) {
					
					CString word = _T("");
					CString tcs;
					int fPos1 = 0;
					int fPos2 = Delimit(line);
					if(fPos2==0xFF) fPos2 = line.GetLength();
					for(int i = fPos1;i<fPos2;i++) {
						tcs = line.GetAt(i);
						word.Append(tcs);
					}
					line.Delete(fPos1,fPos2+1);
					line.TrimLeft();//because some bvh files have more than one space in-between values
					line.TrimRight();
					float value = 0;
					_stscanf(word.GetBuffer(), _T("%f"), &value);
					if(j==1&&m_fGroundHeight<1)m_fGroundHeight=value;// grab the first y hip offset as the ground height
					newLine.data.push_back(value);	
				}
				m_motion.insert(m_motion.begin()+motionNo, newLine);
				motionNo++;
			}
		}
	}
	m_pCurrentMotionFrame = &(*m_motion.begin());
	CalculateFigureHeight();
	m_armsAlignment = CalculateArmsAxisAlignment();
	pFile->Close();
	delete pFile;
	pFile=NULL;

	if(GetNFrames()==1) 
	{
		// need to make sure display first frame is checked - TODO - DRW
		//m_pRegistrySettings->DisplayFirstFrame = TRUE;
		//GetMenu()->CheckMenuItem(ID_VIEW_DISPLAYFIRSTFRAME, MF_CHECKED);
	}

	SaveState();

	return true;
}

bool CBVHFile::SaveBVH(CString FilePath)
{
	CStdioFile* pFile = new CStdioFile(FilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
	vector<BVHJoint>::iterator jointsItr = m_joints.begin();
	CString line = _T("");
	int currentTreeLevel = 0;
	int previousTreeLevel = 0;

	// store away the filename
	TCHAR* dblSlashToken = _T("\\");
	m_FileName = _T("");
	CString tcs;
	int rf = FilePath.ReverseFind(*dblSlashToken);
	dblSlashToken = NULL;
	int len = FilePath.GetLength();
	for(int i=rf+1;i<len;i++) {
		tcs = FilePath.GetAt(i);
		m_FileName.Append(tcs);
	}

	// set the header
	line = _T("HIERARCHY");
	WriteLine(pFile, &line);

	// add the joints
	for(int j=0;j<m_nJoints;j++) {
		
		// start of joint
		currentTreeLevel = jointsItr->heirarchy;
		if(jointsItr->isROOT) {
			line.Append(_T("ROOT"));
			SetTabs(&line, currentTreeLevel);
			AppendSpace(&line);
			line.Append(jointsItr->JOINTName);
		}
		else if(jointsItr->isEndSite) {
			line.Append(_T("End Site"));
			SetTabs(&line, previousTreeLevel+1);
		}
		else {
			line.Append(_T("JOINT"));
			SetTabs(&line, currentTreeLevel);
			AppendSpace(&line);
			line.Append(jointsItr->JOINTName);
		}
		WriteLine(pFile, &line);

		// Read curlie
		if(jointsItr->isEndSite) SetTabs(&line, previousTreeLevel+1);
		else SetTabs(&line, currentTreeLevel);
		line.Append(_T("{"));
		WriteLine(pFile, &line);
		
		// OFFSET line
		if(jointsItr->isEndSite) SetTabs(&line, previousTreeLevel+2);
		else SetTabs(&line, currentTreeLevel+1);
		line.Append(_T("OFFSET"));
		AppendSpace(&line);
		char szTempString[256]; 
		vector<float>::iterator offsetItr = jointsItr->offsetChannels.end();
		sprintf(szTempString, "%f", *--offsetItr); 
		CString temp(szTempString);
		line.Append(temp);
		AppendSpace(&line);
		sprintf(szTempString, "%f", *--offsetItr); 
		CString temp1(szTempString);
		line.Append(temp1);
		AppendSpace(&line);
		sprintf(szTempString, "%f", *--offsetItr); 
		CString temp2(szTempString);
		line.Append(temp2);
		AppendSpace(&line);
		WriteLine(pFile, &line);

		// CHANNELS line
		if(!jointsItr->isEndSite) {
			SetTabs(&line, currentTreeLevel+1);
			line.Append(jointsItr->CHANNELSLine);
			WriteLine(pFile, &line);
		}
		else {	// set  curlies
			int nextJointTabs = 0;
			if(j<m_nJoints-1) nextJointTabs = m_joints[j+1].heirarchy;//GetJointTabs(m_joints[j+1].JOINTName);
			else nextJointTabs = 0;
			int curlyTabs = previousTreeLevel+1;
			while(curlyTabs>=nextJointTabs) {
				SetTabs(&line, curlyTabs);
				line.Append(_T("}"));
				WriteLine(pFile, &line);
				curlyTabs--;
			}
		}
		previousTreeLevel = currentTreeLevel;
		jointsItr++;
	}

	// now add the motion data header
	line = _T("MOTION");
	WriteLine(pFile, &line);
	line = _T("Frames: ");
	char szTempString[256]; 
	sprintf(szTempString, "%d", m_nFrames); 
	CString temp(szTempString);
	line.Append(temp);
	WriteLine(pFile, &line);
	line = _T("Frame Time: ");
	sprintf(szTempString, "%f", m_fFrameTime); 
	CString temp1(szTempString);
	line.Append(temp1);
	WriteLine(pFile, &line);

	// and finally our motion data
	vector<BVHMotion>::iterator motionItr = m_motion.begin();
	for(int i=0;i<m_nFrames;i++) {
		for(int j=0;j<m_nParameters;j++) {
			
			sprintf(szTempString, "%f", motionItr->data[j]); // 
			//sprintf(szTempString, "%0.*f", 3, motionItr->data[j]);
			CString temp(szTempString);
			AppendSpace(&temp);
			line.Append(temp);
		}
		WriteLine(pFile, &line);
		motionItr++;
	}

	pFile->Close();
	delete pFile;
	pFile=NULL;
	return true;
}

CString CBVHFile::ProcessNameLine(CString line, int jointNo)
{
	CString word = NULL;
	TCHAR* token = _T(" ");
	CString tcs;

	int rf = line.ReverseFind(*token);
	for(int i = (rf+1);i<(line.GetLength());i++) {
		tcs = line.GetAt(i);
		word.Append(tcs);
	}

	token = NULL;
	return word;
}

CString CBVHFile::AssignInternalName(CString sJointName)
{
	if(sJointName.Compare("Hips")==0
	 ||sJointName.Compare("Hip")==0
	 ||sJointName.Compare("hips")==0
	 ||sJointName.Compare("hip")==0)
		return "hip";

	if(sJointName.Compare("Abdomen")==0
	 ||sJointName.Compare("Spine")==0
	 ||sJointName.Compare("abdomen")==0
	 ||sJointName.Compare("Waist")==0
	 ||sJointName.Compare("Torso")==0)
		return "abdomen";

	if(sJointName.Compare("Chest2")==0
	 ||sJointName.Compare("chest2")==0
	 ||sJointName.Compare("Spine1")==0
	 ||sJointName.Compare("chest")==0
	 ||sJointName.Compare("Chest")==0)
		return "chest";

	if(sJointName.Compare("LeftHip")==0
	 ||sJointName.Compare("LeftUpLeg")==0
	 ||sJointName.Compare("LeftUpperLeg")==0
	 ||sJointName.Compare("lThigh")==0)
		return "lThigh";

	if(sJointName.Compare("LeftKnee")==0
	 ||sJointName.Compare("LeftLowLeg")==0
	 ||sJointName.Compare("LeftLowerLeg")==0
	 ||sJointName.Compare("LeftLeg")==0
	 ||sJointName.Compare("lShin")==0)
		return "lShin";

	if(sJointName.Compare("LeftFoot")==0
	 ||sJointName.Compare("lFoot")==0)
		return "lFoot";

	if(sJointName.Compare("RightHip")==0
	 ||sJointName.Compare("RightUpLeg")==0
	 ||sJointName.Compare("RightUpperLeg")==0
	 ||sJointName.Compare("rThigh")==0)
		return "rThigh";

	if(sJointName.Compare("RightKnee")==0
	 ||sJointName.Compare("RightLowLeg")==0
	 ||sJointName.Compare("RightLowerLeg")==0
	 ||sJointName.Compare("RightLeg")==0
	 ||sJointName.Compare("rShin")==0)
		return "rShin";

	if(sJointName.Compare("RightFoot")==0
	 ||sJointName.Compare("rFoot")==0)
		return "rFoot";

	if(sJointName.Compare("LeftCollar")==0
	 ||sJointName.Compare("lCollar")==0
	 ||sJointName.Compare("LeftShoulder")==0)
		return "lCollar";

	if(sJointName.Compare("LeftUpArm")==0
	 ||sJointName.Compare("LeftArm")==0
	 ||sJointName.Compare("lShldr")==0
	 ||sJointName.Compare("LeftUpperArm")==0)
		return "lShldr";

	if(sJointName.Compare("LeftElbow")==0
	 ||sJointName.Compare("LeftLowArm")==0
	 ||sJointName.Compare("LeftForeArm")==0
	 ||sJointName.Compare("lForeArm")==0
	 ||sJointName.Compare("LeftLowerArm")==0)
		return "lForeArm";

	if(sJointName.Compare("LeftWrist")==0
	 ||sJointName.Compare("LeftHand")==0
	 ||sJointName.Compare("lHand")==0)
		return "lHand";

	if(sJointName.Compare("RightCollar")==0
	 ||sJointName.Compare("rCollar")==0
	 ||sJointName.Compare("RightShoulder")==0)
		return "rCollar";

	if(sJointName.Compare("RightUpArm")==0
	 ||sJointName.Compare("RightArm")==0
	 ||sJointName.Compare("rShldr")==0
	 ||sJointName.Compare("RightUpperArm")==0)
		return "rShldr";

	if(sJointName.Compare("RightElbow")==0
	 ||sJointName.Compare("RightLowArm")==0
	 ||sJointName.Compare("RightForeArm")==0
	 ||sJointName.Compare("rForeArm")==0
	 ||sJointName.Compare("RightLowerArm")==0)
		return "rForeArm";

	if(sJointName.Compare("RightWrist")==0
	 ||sJointName.Compare("LeftHand")==0
	 ||sJointName.Compare("rHand")==0)
		return "rHand";

	if(sJointName.Compare("Neck")==0
	 ||sJointName.Compare("neck")==0)
		return "neck";

	if(sJointName.Compare("Head")==0
	 ||sJointName.Compare("head")==0)
		return "head";

	return "Unknown";
}

void CBVHFile::ProcessOffsetLine(BVHJoint* joint, CString line, int jointNo)
{
	CString word = _T("");
	CString tcs(_T(""));
	float value = 0;
	float multiplier = 1.f;//40.5633f;

	int rf1 = DelimitR(line);
	int len = line.GetLength();

	// usually X
	for(int i = rf1;i<len;i++) {
		tcs = line.GetAt(i);
		word.Append(tcs);
	}	
	_stscanf(word.GetBuffer(), _T("%f"), &value);
	value*=multiplier;
	joint->offsetChannels.push_back(value);
	line.Delete(rf1,(len-rf1));
	line.TrimLeft();// because some bvh files have got more than one space inbetween values - bug fix 09.08
	line.TrimRight();
	len = line.GetLength();
	word = _T("");

	// usually Y
	int rf2 = DelimitR(line);
	for(int i = rf2;i<len;i++) {
		tcs = line.GetAt(i);
		word.Append(tcs);
	}
	_stscanf(word.GetBuffer(), _T("%f"), &value);
	value*=multiplier;
	joint->offsetChannels.push_back(value);
	line.Delete(rf2,(len-rf2));
	line.TrimRight();
	line.TrimLeft();// because some bvh files have got more than one space inbetween values - bug fix 09.08
	len = line.GetLength();
	word = _T("");

	// usually Z
	int rf3 = DelimitR(line);
	for(int i = rf3;i<len;i++) {
		tcs = line.GetAt(i);
		word.Append(tcs);
	}
	_stscanf(word.GetBuffer(), _T("%f"), &value);
	value*=multiplier;
	joint->offsetChannels.push_back(value);
}

int CBVHFile::ProcessChannelLine(BVHJoint* joint, CString line, int jointNo)
{
	// get n channles
	int order = 0;
	int currentChannelType = 0;
	CString word = _T("");
	TCHAR* token = _T("CHANNELS");
	int rf = line.Find(*token);
	TCHAR wch = line.GetAt(rf+9);
	joint->nChannels = atoi(&wch);

	int channelItr = joint->nChannels/3;

	for(channelItr=joint->nChannels/3;channelItr>0;channelItr--)
	{
		// locals
		TCHAR* token1 = _T(" ");
		CString tcs(_T(""));
		int rf1 = line.ReverseFind(*token1);
		int len = line.GetLength();

		// work out which kind of data (TRS) for this channel
		rf1 = line.ReverseFind(*token1);
		for(int i = (rf1+1);i<(line.GetLength());i++) {
			tcs = line.GetAt(i);
			if (tcs.Compare(_T("p"))==0) currentChannelType = TRANSLATION;
			if (tcs.Compare(_T("r"))==0) currentChannelType = ROTATION;
			if (tcs.Compare(_T("c"))==0) currentChannelType = SCALE;
		}
		

		// ascertain order
		for(int i = (rf1+1);i<len;i++) {
			tcs = line.GetAt(i);
			if(tcs.Compare(_T("X"))==0) {
				order+=4;
			} else if (tcs.Compare(_T("Y"))==0) {
				order+=2;
			} else if (tcs.Compare(_T("Z"))==0) {
				order+=1;
			}		
		}
		line.Delete(rf1, (line.GetLength()-rf1));
		line.TrimLeft();// because some bvh files have got more than one space inbetween values - bug fix 09.08
		line.TrimRight();
		rf1 = line.ReverseFind(*token1);
		for(int i = (rf1+1);i<(line.GetLength());i++) {
			tcs = line.GetAt(i);
			if(tcs.Compare(_T("X"))==0) {
				order+=32;
			} else if (tcs.Compare(_T("Y"))==0) {
				order+=16;
			} else if (tcs.Compare(_T("Z"))==0) {
				order+=8;
			}
		}
		line.Delete(rf1, (line.GetLength()-rf1));
		line.TrimLeft();// because some bvh files have got more than one space inbetween values - bug fix 09.08
		line.TrimRight();
		rf1 = line.ReverseFind(*token1);
		for(int i = (rf1+1);i<(line.GetLength());i++) {
			tcs = line.GetAt(i);
			if(tcs.Compare(_T("X"))==0) {
				order+=256;
			} else if (tcs.Compare(_T("Y"))==0) {
				order+=128;
			} else if (tcs.Compare(_T("Z"))==0) {
				order+=64;
			}
		}
		line.Delete(rf1, (line.GetLength()-rf1));
		line.TrimLeft();// because some bvh files have got more than one space inbetween values - bug fix 09.08
		line.TrimRight();

		switch(currentChannelType)
		{
		case(TRANSLATION):
			{
				joint->offsetChannelOrder=order;
				joint->nOffsetMotionDataIndex+=(channelItr-1)*3;
				break;
			}
		case(ROTATION):
			{
				joint->channelOrder=order;
				joint->nMotionDataIndex+=(channelItr-1)*3;
				break;
			}
		case(SCALE):
			{
				joint->scaleChannelOrder=order;
				joint->nScaleMotionDataIndex+=(channelItr-1)*3;
				break;
			}
		}

		order = 0;
		token1 = NULL;
	}
	
	//	return number of channels found
	return joint->nChannels;
}

bool CBVHFile::ReadLine(CStdioFile* pFile, CString* line) 
{
	line->Empty();
	//int len = 0;
	
	//while(len==0) {
		BOOL success = pFile->ReadString(*line);
		if(!success) return false;
		*line->TrimRight();
		*line->TrimLeft();
		if(line->GetLength()>2048)
		{
			AfxMessageBox(_T("Problem with file: was the file saved using Mac line endings?"),MB_ICONSTOP);
			return false;
		}
	//}
	return true;
}

bool CBVHFile::WriteLine(CStdioFile* pFile, CString* line)
{
	line->Append(_T("\n"));
	pFile->WriteString(*line);
	line->Empty();
	return true;
}

void CBVHFile::SetTabs(CString* line, int nTabs)
{
	for(int i=0;i<nTabs;i++) {
		line->Insert(0,_T("	"));
	}
}

void CBVHFile::AppendSpace(CString* line)
{
	line->Append(_T(" "));
}

BVHJoint* CBVHFile::GetJoint(int index)
{
	vector<BVHJoint>::iterator jointsItr = m_joints.begin();
	while(jointsItr!=m_joints.end()) {
		if(jointsItr->index==index) return &(*jointsItr);
		jointsItr++;
	}
	return NULL;
}

// gets currently selected joint - why have joint name at all?
BVHJoint* CBVHFile::GetJoint(CString JointName)
{
	CString szParent = m_Skeleton->GetCurrentSelParent();
	vector<BVHJoint>::iterator jointsItr = m_joints.begin();

	if(szParent==_T("")) return &(*jointsItr);// must be the ROOT (hips).

	while(jointsItr!=m_joints.end()) {
		CString nextName(jointsItr->JOINTName);
		if(nextName==JointName) {
			BVHJoint* pParent = GetJoint(jointsItr->parentIndex);
			CString szJointParent = pParent->JOINTName;
			pParent=NULL;
			if(szJointParent==szParent) // some joints will have the same name, but not the same parent
				return &(*jointsItr);
		}
		jointsItr++;
	}
	return NULL;
}

BVHJoint* CBVHFile::GetSelectedJoint()
{
	CString szJointName = m_Skeleton->GetCurrentSelection();
	CString szParent = m_Skeleton->GetCurrentSelParent();
	vector<BVHJoint>::iterator jointsItr = m_joints.begin();

	if(szParent==_T("")) return &(*jointsItr);// must be the ROOT hips...
	
	while(jointsItr!=m_joints.end()) {
		CString nextName(jointsItr->JOINTName);
		if(nextName==szJointName) {
			BVHJoint* pParent = GetJoint(jointsItr->parentIndex);
			CString szJointParent = pParent->JOINTName;
			pParent=NULL;
			if(szJointParent==szParent) // some joints will have the same name, but not the same parent
				return &(*jointsItr);
		}
		jointsItr++;
	}
	return NULL;
}

BVHMotion* CBVHFile::GetCurrentMotionFrame(void)
{
	return m_pCurrentMotionFrame;
}

int CBVHFile::GetCurrentFrameIdx()
{
	return m_nCurrentFrameIdx;
}

BVHMotion* CBVHFile::MotionStepFwd(void)
{
	m_nCurrentFrameIdx++;
	if(m_nCurrentFrameIdx>m_nFrames-1) m_nCurrentFrameIdx=0;
	vector<BVHMotion>::iterator motionItr = m_motion.begin();
	motionItr+=m_nCurrentFrameIdx;
	m_pCurrentMotionFrame = &(*motionItr);  
	return m_pCurrentMotionFrame;
}

void CBVHFile::MotionStepRev(void)
{
	m_nCurrentFrameIdx--;
	if(m_nCurrentFrameIdx<0) m_nCurrentFrameIdx=m_nFrames-1;
	vector<BVHMotion>::iterator motionItr = m_motion.begin();
	motionItr+=m_nCurrentFrameIdx;
	m_pCurrentMotionFrame = &(*motionItr);
}
float CBVHFile::GetFrameTime(void)
{
	return m_fFrameTime*1000.0f;
}

GLfloat CBVHFile::GetGroundHeight(void)
{
	return m_fGroundHeight;
}

int CBVHFile::GetNFrames(void)
{
	return m_nFrames;
}

void CBVHFile::SetNFrames(int nFrames)
{
	m_nFrames = nFrames;
	return;
}

void CBVHFile::SetCurrentMotionFrame(int index)
{
	vector<BVHMotion>::iterator motionItr = m_motion.begin();
	motionItr+=index;
	m_pCurrentMotionFrame = &(*motionItr);
	m_nCurrentFrameIdx=index;
}

void CBVHFile::SetFrameTime(float NewFrameTime)
{
	float fNewFrameTime = (float)NewFrameTime/1000.f;
	m_fFrameTime = fNewFrameTime;
}

CString CBVHFile::GetFileName()
{
	return m_FileName;
}

int CBVHFile::GetNJoints()
{
	return m_nJoints;
}

int CBVHFile::GetNParameters()
{
	return m_nParameters;
}

int CBVHFile::DelimitR(CString line)
{
	TCHAR* tabToken = _T("	");//  TAB
	TCHAR* spcToken = _T(" ");//  SPACE

	int nPos1 = line.ReverseFind(*tabToken);
	int nPos2 = line.ReverseFind(*spcToken);
	tabToken = NULL; spcToken = NULL;
	if(nPos1<0)nPos1=0;
	if(nPos2<0)nPos2=0;
	if(nPos1<nPos2)return nPos2;
	else return nPos1;
} 

int CBVHFile::Delimit(CString line)
{
	TCHAR* tabToken = _T("	");//  TAB
	TCHAR* spcToken = _T(" ");//  SPACE

	int nPos1 = line.Find(*tabToken);
	int nPos2 = line.Find(*spcToken);
	tabToken = NULL; spcToken = NULL;
	if(nPos1<0)nPos1=0xFF;
	if(nPos2<0)nPos2=0xFF;
	if(nPos1<nPos2)return nPos1;
	else return nPos2;
} 

GLfloat CBVHFile::GetFigureHeight(void)
{
	return m_nFigureHeightMax-m_nFigureHeightMin;
}

void CBVHFile::SetAssociatedTreeControl(CBVHTreeCtrl* skeleton)
{
	m_Skeleton = skeleton;
}

void CBVHFile::CalculateFigureHeight()
{
	vector<BVHJoint>::iterator jointItr = m_joints.begin();
	m_nFigureHeightMax = 0;
	m_nFigureHeightMin = 0;

	while(jointItr!=m_joints.end()) {

		if(jointItr->isEndSite) {

			float nEndSiteY = jointItr->offsetChannels[1];
			
			// gather a vector of pointers to all transformations back to ROOT
			BVHJoint* tempJoint = &(*jointItr);
			vector<BVHJoint*> path;
			int nParentIndex = jointItr->parentIndex;

			while(!tempJoint->isROOT) { 
				tempJoint = GetJoint(nParentIndex);
				nParentIndex = tempJoint->parentIndex;
				path.push_back(tempJoint);
			}

			// now iterate through path summing all the transformations
			vector<BVHJoint*>::iterator jointsPath = path.end();
			while(jointsPath!=path.begin()) {
				jointsPath--;
				tempJoint = *jointsPath;
				vector<float> offSets = tempJoint->offsetChannels;
				vector<float>::iterator offsetItr = offSets.begin();
				offsetItr++;
				float nJointYOff = *offsetItr;
				nEndSiteY+=nJointYOff;
			}
			tempJoint = NULL;

			if(nEndSiteY>m_nFigureHeightMax) m_nFigureHeightMax = nEndSiteY;
			if(nEndSiteY<m_nFigureHeightMin) m_nFigureHeightMin = nEndSiteY;
		}
		jointItr++;
	}

	// now set the new ground height
	GLfloat oldGroundHeight = m_fGroundHeight;
	GLfloat newGroundHeight = -m_nFigureHeightMin;
	GLfloat difGroundHeight = newGroundHeight - oldGroundHeight;
	m_fGroundHeight += difGroundHeight;

	// motion
	//vector<BVHMotion>::iterator motionItr = m_motion.begin();
	//while(motionItr<m_motion.end()) {
	//	vector<GLfloat>::iterator dataItr = motionItr->data.begin()+1;
	//	*dataItr += difGroundHeight;
	//	motionItr++;
	//}
}

int CBVHFile::GetArmsAlignment()
{
	return m_armsAlignment;
}

int CBVHFile::CalculateArmsAxisAlignment()
{
	vector<BVHJoint>::iterator jointItr = m_joints.begin();
	CString szJointName = _T("");
	float fExtentX = 0.f;
	float fExtentY = 0.f;
	float fJointX = 0.f;
	float fJointY = 0.f;

	while(jointItr!=m_joints.end()) {

		szJointName = jointItr->JOINTName.GetBuffer();

		fJointX = jointItr->offsetChannels[2];
		fJointY = jointItr->offsetChannels[1];

		// work out from left arm's offsets	
		if((szJointName==_T("LeftCollar"))||(szJointName == _T("lCollar"))||(szJointName == _T("lColr"))) { // due to bug in Poser 6
			fExtentX += fabs(fJointX);
			fExtentY += fabs(fJointY);
		}

		if((szJointName==_T("LeftShoulder"))||(szJointName==_T("LeftUpArm"))||(szJointName == _T("lShldr"))) {
			fExtentX += fabs(fJointX);
			fExtentY += fabs(fJointY);
		}

		if((szJointName==_T("LeftElbow"))||(szJointName==_T("LeftLowArm"))||(szJointName == _T("lForeArm"))) {
			fExtentX += fabs(fJointX);
			fExtentY += fabs(fJointY);;
		}

		if((szJointName==_T("LeftWrist"))||(szJointName==_T("LeftHand"))||(szJointName == _T("lHand"))) {
			fExtentX += fabs(fJointX);
			fExtentY += fabs(fJointY);
			
			// and End Site
			jointItr++;
			szJointName = jointItr->JOINTName.GetBuffer();
			fJointX = jointItr->offsetChannels[0];
			fJointY = jointItr->offsetChannels[1];
			fExtentX += fabs(fJointX);
			fExtentY += fabs(fJointY);
			jointItr = m_joints.end(); // we're done, no need to search the remaining joints
			jointItr--;
		}
		jointItr++;
	}

	

	if(fExtentX>fExtentY) 
	{
		m_armsAlignment = X_AXIS_ALIGNED_ARMS;
		return X_AXIS_ALIGNED_ARMS;
	}
	else if(fExtentY>fExtentX) 
	{
		m_armsAlignment = Y_AXIS_ALIGNED_ARMS;
		return Y_AXIS_ALIGNED_ARMS;
	}
	else 
	{
		m_armsAlignment = UNKNOWN_ALIGNMENT;
		return UNKNOWN_ALIGNMENT;
	}
}

void CBVHFile::SaveState()
{
	// copy the joint data
	m_jointsZ.clear();
	vector<BVHJoint>::iterator jointItr = m_joints.begin();
	while(jointItr!=m_joints.end()) 
	{
		m_jointsZ.push_back(*jointItr);
		jointItr++;
	}

	// copy the motion data
	m_motionZ.clear();
	vector<BVHMotion>::iterator motionItr = m_motion.begin();
	while(motionItr!=m_motion.end())
	{
		m_motionZ.push_back(*motionItr);
		motionItr++;
	}
	
	m_nJointsZ = m_nJoints;
	m_nFramesZ = m_nFrames;
	m_fFrameTimeZ = m_fFrameTime;
	m_armsAlignmentZ = m_armsAlignment;
	m_nParametersZ = m_nParameters;
	m_fGroundHeightZ = m_fGroundHeight;	
	m_nFigureHeightMaxZ = m_nFigureHeightMax;// hips to head 
	m_nFigureHeightMinZ = m_nFigureHeightMin;// hips to feet
}

void CBVHFile::SwitchState()
{
	// make a local copy 
	vector<BVHJoint> jointsT;
	vector<BVHMotion> motionT;

	// copy the joints data
	vector<BVHJoint>::iterator jointItr = m_jointsZ.begin();
	while(jointItr!=m_jointsZ.end()) 
	{
		jointsT.push_back(*jointItr);
		jointItr++;
	}

	// copy the motion data
	vector<BVHMotion>::iterator motionItr = m_motionZ.begin();
	while(motionItr!=m_motionZ.end())
	{
		motionT.push_back(*motionItr);
		motionItr++;
	}

	int nJointsT = m_nJointsZ;
	int nFramesT = m_nFramesZ;
	float fFrameTimeT = m_fFrameTimeZ;
	int armsAlignmentT = m_armsAlignmentZ;
	int nParametersT = m_nParametersZ;
	GLfloat fGroundHeightT = m_fGroundHeightZ;	
	GLfloat nFigureHeightMaxT = m_nFigureHeightMaxZ;// hips to head 
	GLfloat nFigureHeightMinT = m_nFigureHeightMinZ;// hips to feet

	SaveState();

	// copy the joint data
	m_joints.clear();
	vector<BVHJoint>::iterator jointItrT = jointsT.begin();
	while(jointItrT!=jointsT.end()) 
	{
		m_joints.push_back(*jointItrT);
		jointItrT++;
	}

	// copy the motion data
	m_motion.clear();
	vector<BVHMotion>::iterator motionItrT = motionT.begin();
	while(motionItrT!=motionT.end())
	{
		m_motion.push_back(*motionItrT);
		motionItrT++;
	}
	
	m_nJoints = nJointsT;
	m_nFrames = nFramesT;
	m_fFrameTime = fFrameTimeT;
	m_armsAlignment = armsAlignmentT;
	m_nParameters = nParametersT;
	m_fGroundHeight = fGroundHeightT;	
	m_nFigureHeightMax = nFigureHeightMaxT;// hips to head 
	m_nFigureHeightMin = nFigureHeightMinT;// hips to feet
}
