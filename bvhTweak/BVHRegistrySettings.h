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
#include "rsettings.h"
#include "BVHFileDialog.h"

// From:
//http://www.codeproject.com/KB/system/regsettings.aspx


class CBVHRegistrySettings :
	public CRegSettings
{
public:

    DWORD EditMode; 
    DWORD DisplayColour; 
    DWORD DisplayGroundPlane;
	DWORD DisplayHeadMesh;
	DWORD DisplayFirstFrame;
	DWORD SliderRange;
	DWORD DisplayShading;
	DWORD DefaultFileView;

    BEGIN_REG_MAP(CBVHRegistrySettings)
        REG_ITEM(EditMode, 1)
        REG_ITEM(DisplayColour, 1)
        REG_ITEM(DisplayGroundPlane, 1)
		REG_ITEM(DisplayHeadMesh, 1)
		REG_ITEM(DisplayFirstFrame, 1)
		REG_ITEM(SliderRange,1200);
		REG_ITEM(DisplayShading,1);
		REG_ITEM(DefaultFileView,CBVHFileDialog::ODM_VIEW_DETAIL);
    END_REG_MAP()

};
