/*
	bvhacker - a program for animation previewing and editing

	bvhacker Copyright © 2006 to 2010 David Wooldridge  

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
 
/********************************************************************


		rsettings.h: Easy Load/Save settings from/to registry
		Copyright (C) 2002 Magomed G. Abdurakhmanov, maq@mail.ru

		History:

		7.10.2002 13:53
			+ OnBeforeSave, OnAfterLoad virtual methods
			- bugs fixed

		25.09.2002 1:39
			+ REG_ITEM_SUBKEY:
				store data in sub keys

			+ REG_ITEM_BINARY,REG_ITEM_BINARY_SIZE:
				store binary data (void*, structs, etc.)
				
			+ REG_ITEM_SZ, REG_ITEM_SZ_REQUIRE, REG_ITEM_SZ_LEN, 
				REG_ITEM_SZ_REQUIRE_LEN: 
				store C strings (TCHAR*)

			+ REG_ITEM and REG_ITEM_REQUIRED enhanced to support:
				bool, int, char and wchar_t

		19.09.2002 
			Created


********************************************************************/
#include <atlbase.h>
#include <atlstr.h>

#define BEGIN_REG_MAP(CLASS_NAME) \
	CLASS_NAME() \
	{m_hKey = 0;}\
    CLASS_NAME(HKEY hKey, LPCTSTR szKey, ...) \
	{ \
		m_hKey = hKey; \
		if(szKey != NULL) { \
		va_list marker; \
		va_start(marker, szKey); \
		m_sKey.FormatV(szKey, marker); \
		va_end(marker); }\
		InternalSaveLoad(0, false, true); \
	} \
	virtual DWORD InternalSaveLoad(HKEY hKey, bool bSave, bool bOnlyInit) { \
	DWORD dwResult = ERROR_SUCCESS; \
	if(bSave && !bOnlyInit) OnBeforeSave(); \

#define END_REG_MAP() if(!bSave && !bOnlyInit) OnAfterLoad(); return dwResult; }

#define REG_ITEM(variable, DEFAULT_VALUE) \
			if(bOnlyInit) variable = DEFAULT_VALUE; else {\
			dwResult = SaveLoadItem(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult; else variable = DEFAULT_VALUE; }

#define REG_ITEM_REQUIRE(variable) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItem(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			return dwResult; }

#define REG_ITEM_SUBKEY(variable) \
			if(bOnlyInit) variable.InternalSaveLoad(0, false, true); else {\
			dwResult = SaveLoadItem(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			return dwResult; }

#define REG_ITEM_SIMPLE_ARRAY(variable) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItemATL(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult;}

#define REG_ITEM_VECTOR(variable) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItemSTL(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult;}

#define REG_ITEM_LIST(variable) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItemSTL(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult;}

#define REG_ITEM_SZ(variable, DEFAULT_VALUE) \
			if(bOnlyInit) _tcscpy(variable, DEFAULT_VALUE); else {\
			dwResult = SaveLoadItem(hKey, _T(#variable), variable, sizeof(variable)/sizeof(TCHAR), bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult; else _tcscpy(variable, DEFAULT_VALUE); }

#define REG_ITEM_SZ_REQUIRE(variable) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItem(hKey, _T(#variable), variable, sizeof(variable), bSave); \
			if(dwResult != ERROR_SUCCESS) \
			return dwResult; }

#define REG_ITEM_SZ_LEN(variable, DEFAULT_VALUE, variable_len) \
			if(bOnlyInit) _tcsncpy(variable, DEFAULT_VALUE, variable_len); else {\
			dwResult = SaveLoadItem(hKey, _T(#variable), variable, variable_len, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult; else _tcsncpy(variable, DEFAULT_VALUE, variable_len); }

#define REG_ITEM_SZ_REQUIRE_LEN(variable, variable_len) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItem(hKey, _T(#variable), variable, variable_len, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			return dwResult; }

#define REG_ITEM_BINARY(variable) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItem(hKey, _T(#variable), (void*)&variable, sizeof(variable), bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult;}

#define REG_ITEM_BINARY_SIZE(variable, variable_size) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItem(hKey, _T(#variable), (void*)&variable, variable_size, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult;}

#define REG_ITEM_STL(variable, DEFAULT_VALUE) \
			if(bOnlyInit) variable = DEFAULT_VALUE; else {\
			dwResult = SaveLoadItemSTLstring(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			if(bSave) return dwResult; else variable = DEFAULT_VALUE; }

#define REG_ITEM_STL_REQUIRE(variable) \
			if(!bOnlyInit){\
			dwResult = SaveLoadItemSTLstring(hKey, _T(#variable), variable, bSave); \
			if(dwResult != ERROR_SUCCESS) \
			return dwResult; }

class CRegSettings
{
protected:

	// DWORD 
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, DWORD& Variable, bool bSave)
	{
		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(DWORD);
		if(bSave)
			return ::RegSetValueEx(hKey, szName, NULL, dwType, (const BYTE*)&Variable, dwSize);
		else
			return ::RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)&Variable, &dwSize);
	}

	// int
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, int& Variable, bool bSave)
	{
		return SaveLoadItem(hKey, szName, (DWORD&)Variable, bSave);
	}

	// long
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, long& Variable, bool bSave)
	{
		return SaveLoadItem(hKey, szName, (DWORD&)Variable, bSave);
	}

	// bool
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, bool& Variable, bool bSave)
	{
		DWORD dwValue = Variable ? 1 : 0;
		DWORD dwResult = SaveLoadItem(hKey, szName, dwValue, bSave);
		if(!bSave && dwResult == ERROR_SUCCESS)
			Variable = dwValue != 0;
		return dwResult;
	}

	// char
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, char& Variable, bool bSave)
	{
		DWORD dwValue = (DWORD)Variable;
		DWORD dwResult = SaveLoadItem(hKey, szName, dwValue, bSave);
		if(!bSave && dwResult == ERROR_SUCCESS)
			Variable = (char)LOBYTE(LOWORD(dwValue));
		return dwResult;
	}

	// unsigned char
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, unsigned char& Variable, bool bSave)
	{
		DWORD dwValue = (DWORD)Variable;
		DWORD dwResult = SaveLoadItem(hKey, szName, dwValue, bSave);
		if(!bSave && dwResult == ERROR_SUCCESS)
			Variable = (unsigned char)LOBYTE(LOWORD(dwValue));
		return dwResult;
	}

	// wchar_t
#ifdef _WCHAR_T_DEFINED
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, wchar_t& Variable, bool bSave)
	{
		DWORD dwValue = (DWORD)Variable;
		DWORD dwResult = SaveLoadItem(hKey, szName, dwValue, bSave);
		if(!bSave && dwResult == ERROR_SUCCESS)
			Variable = (wchar_t)LOWORD(dwValue);
		return dwResult;
	}
#endif

	// TCHAR*
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, TCHAR* Variable, size_t size, bool bSave)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = (DWORD)size;
		DWORD dwResult = ERROR_SUCCESS;
		if(bSave)
			return ::RegSetValueEx(hKey, szName, NULL, dwType, (LPBYTE)Variable, dwSize);
		else
			return ::RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)Variable, &dwSize);
	}

	// void*
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, void* Variable, size_t size, bool bSave)
	{
		DWORD dwType = REG_BINARY;
		DWORD dwSize = (DWORD)size;
		if(bSave)
			return ::RegSetValueEx(hKey, szName, NULL, dwType, (LPBYTE)Variable, dwSize);
		else
			return ::RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)Variable, &dwSize);
	}

	// CRegSettings (sub keys)
	// std::string
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, CRegSettings& Variable, bool bSave)
	{
		DWORD dwResult = ERROR_SUCCESS;
		HKEY hKeySub;
		if(bSave)
			dwResult = ::RegCreateKeyEx(hKey, szName, NULL, NULL, REG_OPTION_NON_VOLATILE,
				KEY_WRITE, NULL, &hKeySub, NULL);
		else
			dwResult = ::RegOpenKeyEx(hKey, szName, 0, KEY_READ, &hKeySub);

		if(dwResult != ERROR_SUCCESS)
			return dwResult;

		dwResult = Variable.InternalSaveLoad(hKeySub, bSave, false);

		::RegCloseKey(hKeySub);
		return dwResult;
	}

	// CString
#if defined(_MFC_VER) || defined(__ATLSTR_H__)
	inline DWORD SaveLoadItem(HKEY hKey, LPCTSTR szName, CString& Variable, bool bSave)
	{
		if(bSave)
			return ::RegSetValueEx(hKey, szName, NULL, REG_SZ, (LPBYTE)(LPCTSTR)Variable, (Variable.GetLength() + 1)*sizeof(TCHAR));
		else
		{
			TCHAR buf[100];
			TCHAR* pBuf = buf;
			DWORD dwType = REG_SZ;
			DWORD dwSize = sizeof(buf);
			DWORD dwResult = ERROR_SUCCESS;

			dwResult = ::RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)pBuf, &dwSize);
			if(dwResult == ERROR_MORE_DATA)
			{
				pBuf = (TCHAR*)_alloca(dwSize);
				dwResult = ::RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)pBuf, &dwSize);
			}
			
			if(dwResult == ERROR_SUCCESS)
				Variable = pBuf;

			return dwResult;
		}
	}
#endif

	// std::string
	template <class T>
	inline DWORD SaveLoadItemSTLstring(HKEY hKey, LPCTSTR szName, T& Variable, bool bSave)
	{
		if(bSave)
		{
			DWORD dwLen = (DWORD)Variable.size();
			LPCTSTR sz = Variable.c_str();
			return ::RegSetValueEx(hKey, szName, NULL, REG_SZ, (LPBYTE)sz, dwLen);
		}
		else
		{
			TCHAR buf[100];
			TCHAR* pBuf = buf;
			DWORD dwType = REG_SZ;
			DWORD dwSize = sizeof(buf);
			DWORD dwResult = ERROR_SUCCESS;

			dwResult = ::RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)pBuf, &dwSize);
			if(dwResult == ERROR_MORE_DATA)
			{
				pBuf = (TCHAR*)_alloca(dwSize);
				dwResult = ::RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)pBuf, &dwSize);
			}
			
			if(dwResult == ERROR_SUCCESS)
				Variable = pBuf;

			return dwResult;
		}
	}

	// CSimpleArray<T>
	template <class TArray> 
	DWORD SaveLoadItemATL(HKEY hKey, LPCTSTR szName, TArray& Variable, bool bSave)
	{
		if(!bSave)
			Variable.RemoveAll();
		DWORD dwResult = ERROR_SUCCESS;
		HKEY hKeySub;
		if(bSave)
			dwResult = ::RegCreateKeyEx(hKey, szName, NULL, NULL, REG_OPTION_NON_VOLATILE,
				KEY_WRITE, NULL, &hKeySub, NULL);
		else
			dwResult = ::RegOpenKeyEx(hKey, szName, 0, KEY_READ, &hKeySub);

		if(dwResult != ERROR_SUCCESS)
			return dwResult;

		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(DWORD);
		DWORD dwItemCount = Variable.GetSize();

		if(bSave)
			::RegSetValueEx(hKeySub, _T("Count"), NULL, dwType, (const LPBYTE)&dwItemCount, dwSize);			
		else
			::RegQueryValueEx(hKeySub, _T("Count"), NULL, &dwType, (LPBYTE)&dwItemCount, &dwSize);


		if(bSave)
		{
			for(DWORD i = 0; i < dwItemCount; i++)
			{
				CString sItemKey;
				sItemKey.Format(_T("%i"), i);
				HKEY hKeyItem;
				dwResult = ::RegCreateKeyEx(hKeySub, sItemKey, NULL, NULL, REG_OPTION_NON_VOLATILE,
					KEY_WRITE, NULL, &hKeyItem, NULL);

				if(dwResult != ERROR_SUCCESS)
					goto ret;

				dwResult = Variable[i].InternalSaveLoad(hKeyItem, bSave, false);
				::RegCloseKey(hKeyItem);
				if(dwResult != ERROR_SUCCESS)
					goto ret;
			}
		}
		else
		{
			for(DWORD i = 0; i < dwItemCount; i++)
			{
				CString sItemKey;
				sItemKey.Format(_T("%i"), i);
				HKEY hKeyItem;
				dwResult = ::RegOpenKeyEx(hKeySub, sItemKey, 0, KEY_READ, &hKeyItem);

				if(dwResult != ERROR_SUCCESS)
					goto ret;

				TArray::_ArrayElementType item;
				dwResult = item.InternalSaveLoad(hKeyItem, bSave, false);
				Variable.Add(item);
				::RegCloseKey(hKeyItem);
			}
		}

ret:
		::RegCloseKey(hKeySub);
		return dwResult;
	}

	// std::vector<T>, std::list<T>
	template <class TSTLArray> 
	DWORD SaveLoadItemSTL(HKEY hKey, LPCTSTR szName, TSTLArray& Variable, bool bSave)
	{
		if(!bSave)
			Variable.clear();
		DWORD dwResult = ERROR_SUCCESS;
		HKEY hKeySub;
		if(bSave)
			dwResult = ::RegCreateKeyEx(hKey, szName, NULL, NULL, REG_OPTION_NON_VOLATILE,
				KEY_WRITE, NULL, &hKeySub, NULL);
		else
			dwResult = ::RegOpenKeyEx(hKey, szName, 0, KEY_READ, &hKeySub);

		if(dwResult != ERROR_SUCCESS)
			return dwResult;

		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(DWORD);
		DWORD dwItemCount = (DWORD)Variable.size();

		if(bSave)
			::RegSetValueEx(hKeySub, _T("Count"), NULL, dwType, (const LPBYTE)&dwItemCount, dwSize);			
		else
			::RegQueryValueEx(hKeySub, _T("Count"), NULL, &dwType, (LPBYTE)&dwItemCount, &dwSize);


		if(bSave)
		{
			DWORD i = 0;
			for(TSTLArray::iterator it = Variable.begin(); it != Variable.end(); it++)
			{
				CString sItemKey;
				sItemKey.Format(_T("%i"), i);
				HKEY hKeyItem;
				dwResult = ::RegCreateKeyEx(hKeySub, sItemKey, NULL, NULL, REG_OPTION_NON_VOLATILE,
					KEY_WRITE, NULL, &hKeyItem, NULL);

				if(dwResult != ERROR_SUCCESS)
					goto ret;

				dwResult = it->InternalSaveLoad(hKeyItem, bSave, false);
				::RegCloseKey(hKeyItem);
				if(dwResult != ERROR_SUCCESS)
					goto ret;
				i++;
			}
		}
		else
		{
			for(DWORD i = 0; i < dwItemCount; i++)
			{
				CString sItemKey;
				sItemKey.Format(_T("%i"), i);
				HKEY hKeyItem;
				dwResult = ::RegOpenKeyEx(hKeySub, sItemKey, 0, KEY_READ, &hKeyItem);

				if(dwResult != ERROR_SUCCESS)
					goto ret;

				TSTLArray::value_type item;
				dwResult = item.InternalSaveLoad(hKeyItem, bSave, false);
				Variable.push_back(item);
				::RegCloseKey(hKeyItem);
			}
		}

ret:
		::RegCloseKey(hKeySub);
		return dwResult;
	}

	CString m_sKey;
	HKEY m_hKey;
	virtual DWORD InternalSaveLoad(HKEY hKey, bool bSave, bool bOnlyInit) = 0;

	virtual void OnBeforeSave()
	{
		//override if needed
	}

	virtual void OnAfterLoad()
	{
		//override if needed
	}

public:
	DWORD Save()
	{
		CRegKey reg;
		DWORD dwResult = 0;
		dwResult = reg.Create(m_hKey, m_sKey, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE);

		if(dwResult != ERROR_SUCCESS)
			return dwResult;

		dwResult = InternalSaveLoad(reg.m_hKey, true, false);
		reg.Close();
		return dwResult;
	}

	DWORD Load()
	{
		CRegKey reg;
		DWORD dwResult = 0;
		dwResult = reg.Open(m_hKey, m_sKey, KEY_READ);

		if(dwResult != ERROR_SUCCESS)
		{
			InternalSaveLoad(0, false, true);
			return dwResult;
		}

		dwResult = InternalSaveLoad(reg.m_hKey, false, false);
		reg.Close();
		return dwResult;
	}
};
