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
#include "QCFile.h"

CQCFile::CQCFile(void)
: m_nProblems(0)
, m_nWarnings(0)
{
}

CQCFile::~CQCFile(void)
{
}

void CQCFile::Initialise(void)
{
	char desktopPath[MAX_PATH];
	char* pHomeDrive = getenv("HOMEDRIVE");
	char* pHomePath = getenv("HOMEPATH"); 
	strcpy(desktopPath, pHomeDrive);
	strcat(desktopPath, pHomePath);
	strcat(desktopPath, "\\Desktop\\bvhacker quality report.txt");
	pFile = new CStdioFile(desktopPath, CFile::modeWrite | CFile::modeCreate | CFile::typeText);
}

void CQCFile::LogProblem(CString szProblem, int nFrameNumber, float fValue)
{
	if(pFile)
	{
		CString szDetails;
		szDetails.Format(" Value = %f at frame number %d \n", fValue, nFrameNumber);
		szProblem.Append(szDetails);
		pFile->Write(szProblem, szProblem.GetLength());
	}
	else
		AfxMessageBox(_T("Error: You tried to use the QCFile class without initialising it."),MB_ICONSTOP);
	
	m_nProblems++;

	return;
}
void CQCFile::LogProblem(CString szProblem)
{
	if(pFile)
		pFile->Write(szProblem, szProblem.GetLength());

	else
		AfxMessageBox(_T("Error: You tried to use the QCFile class without initialising it."),MB_ICONSTOP);
	
	m_nProblems++;

	return;
}

void CQCFile::LogWarning(CString szWarning)
{
	if(pFile)
		pFile->Write(szWarning, szWarning.GetLength());

	else
		AfxMessageBox(_T("Error: You tried to use the QCFile class without initialising it."),MB_ICONSTOP);
	
	m_nWarnings++;

	return;
}

void CQCFile::Log(CString szEntry)
{
	if(pFile)
		pFile->Write(szEntry, szEntry.GetLength());

	else
		AfxMessageBox(_T("Error: You tried to use the QCFile class without initialising it."),MB_ICONSTOP);

	return;
}


void CQCFile::Close(void)
{
	CString szLogEntry;
	if(m_nProblems>1) szLogEntry.Format("\n\n%d human limits problems were found with this animation.\n\n", m_nProblems);
	else if(m_nProblems==1) szLogEntry.Format("\nOne human limits problem was found with this animation.\n\n");
	else szLogEntry.Format("\nNo human limits problems were found with this animation (or the bone naming convention did not match that of SL).\n\n");
	pFile->Write(szLogEntry, szLogEntry.GetLength());
	if(m_nWarnings>1) szLogEntry.Format("This animation produced %d warnings.\n\n", m_nWarnings);
	else if(m_nWarnings==1) szLogEntry.Format("This animation produced one warning.\n\n");
	else szLogEntry.Format("This animation produced no warnings.\n\n\n");
	pFile->Write(szLogEntry, szLogEntry.GetLength());
	szLogEntry.Format("End of report.");
	pFile->Write(szLogEntry, szLogEntry.GetLength());
	pFile->Close();
	delete pFile;
	pFile=NULL;

	// now open the file for the user
	char desktopPath[MAX_PATH];
	char reportLocation[MAX_PATH];
	char* pHomeDrive = getenv("HOMEDRIVE");
	char* pHomePath = getenv("HOMEPATH"); 
	strcpy(desktopPath, pHomeDrive);
	strcat(desktopPath, pHomePath);
	strcat(desktopPath, "\\Desktop");
	strcpy(reportLocation, desktopPath);
	strcat(reportLocation, "\\bvhacker quality report.txt");
	ShellExecute(AfxGetMainWnd()->m_hWnd,"open", reportLocation,"",desktopPath,1);
}

