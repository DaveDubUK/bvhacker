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

class CQCFile
{
public:
	CQCFile(void);
	~CQCFile(void);
	void Initialise(void);

private:
	CStdioFile* pFile;
public:
	void LogProblem(CString szProblem, int nFrameNumber, float fValue);
	void LogProblem(CString szProblem);
	void LogWarning(CString szWarning);
	void Log(CString szEntry);
	void Close(void);
private:
	int m_nProblems;
	int m_nWarnings;
};
