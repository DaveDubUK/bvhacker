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
 
// bvhTweak.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "BVHFile.h"
#include "BVHTweaker.h"
#include "BVHRegistrySettings.h"
#include "BVHQuaterniser.h"

// CbvhTweakApp:
// See bvhTweak.cpp for the implementation of this class
//

class CbvhTweakApp : public CWinApp
{
public:
	CbvhTweakApp();
	virtual CbvhTweakApp::~CbvhTweakApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	CBVHFile* m_pBVHFile;
	CBVHTweaker* m_pBVHTweaker;
	CBVHQuaterniser* m_pBVHQuaterniser;
	bool m_IsLocked; // simple mutex
	CBVHRegistrySettings* m_pRegistrySettings;
};

extern CbvhTweakApp theApp;