#include "StdAfx.h"
#include "BVHStdioFile.h"

CBVHStdioFile::CBVHStdioFile(void)
{
}

CBVHStdioFile::CBVHStdioFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	CStdioFile(lpszFileName, nOpenFlags);
}

CBVHStdioFile::~CBVHStdioFile(void)
{
}

BOOL CBVHStdioFile::ReadString(CString& rString)
{
    ASSERT_VALID(this);

	rString.Empty();// = &afxChNil;    // empty string without deallocating
    const int nMaxSize = 128;
    LPTSTR lpsz = rString.GetBuffer(nMaxSize);
    LPTSTR lpszResult;
    int nLen;
    for (;;)
    {
         lpszResult = _fgetts(lpsz, nMaxSize+1, m_pStream);
         rString.ReleaseBuffer();

         // handle error/eof case
         if (lpszResult == NULL && !feof(m_pStream))
         {
              clearerr(m_pStream);
              AfxThrowFileException(CFileException::generic, _doserrno,
                   m_strFileName);
         }

         // if string is read completely or EOF
         if (lpszResult == NULL ||
              (nLen = lstrlen(lpsz)) < nMaxSize ||
              lpsz[nLen-1] == '\n')
         {
              break;
         }

         nLen = rString.GetLength();
         lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
    }

    // remove '\n' from end of string if present
    lpsz = rString.GetBuffer(0);
    nLen = rString.GetLength();
    if (nLen != 0 && lpsz[nLen-1] == '\n')
         rString.GetBufferSetLength(nLen-1);

    return lpszResult != NULL;
}