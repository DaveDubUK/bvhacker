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
 
// bvhTweak.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "bvhTweak.h"
#include "bvhTweakDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CbvhTweakApp

BEGIN_MESSAGE_MAP(CbvhTweakApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CbvhTweakApp construction

CbvhTweakApp::CbvhTweakApp()
{
	m_pRegistrySettings = new CBVHRegistrySettings(HKEY_CURRENT_USER,"Software\\Davedub\\bvhacker\\1.8");
    m_pRegistrySettings->Load(); // Load configuration from registry    
}

CbvhTweakApp::~CbvhTweakApp()
{
	// Save configuration to registry
	m_pRegistrySettings->Save(); 
	
	// clean up in the right order
	if(m_pRegistrySettings) {
		delete m_pRegistrySettings;
	}
	if(m_pBVHQuaterniser) {
		delete m_pBVHQuaterniser;
	}
	m_pBVHQuaterniser = NULL;
	if(m_pBVHFile) {
		delete m_pBVHFile;
	}
	m_pBVHFile = NULL;
	if(m_pBVHTweaker) {
		delete m_pBVHTweaker;
	}
	m_pBVHTweaker = NULL;
}


// The one and only CbvhTweakApp object

CbvhTweakApp theApp;


// CbvhTweakApp initialization

BOOL CbvhTweakApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Davedub's bvhacker application"));

	// app specific
	m_pBVHFile = new CBVHFile();
	m_IsLocked = false;
	CbvhTweakDlg dlg;
	m_pMainWnd = &dlg;
	//LPSTR lpCmdLine = GetCommandLine();
	dlg.SetCmdLine(m_lpCmdLine);
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
