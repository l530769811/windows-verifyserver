#include <WinSock2.h>
#include <MSWSock.h>
#include <MSTcpIP.h>
#include "StdAfx.h"
#include "GlobalFunc.h"
#include "Shlwapi.h"
#include <stdio.h>

MyString GetModuleFilePath()
{
	TCHAR str[MAX_PATH] = {0};
	MyString csBasePath;
	::GetModuleFileName(NULL, str, MAX_PATH);
	csBasePath = str;

	int nPos = -1;
	nPos = csBasePath.rfind(_T('\\'));
	csBasePath = csBasePath.substr(0, nPos);
	return csBasePath;
}

int GlobalUnicodeToUtf8(const TCHAR *unicode, char *uft8, int len)
{
	int utf8size = 0;
#ifdef _UNICODE
	utf8size = ::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	if (utf8size > len)
	{
		return NULL;
	}
	

	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)(LPCWSTR)unicode, -1, uft8, sizeof(char *)*utf8size, NULL, NULL);
#else
	strcpy(uft8, unicode);
	utf8size = len;
#endif //_UNICODE
	return utf8size;
}

TCHAR* GlobalUtf8ToUnicode(const char *uft8, TCHAR *unicode, int len)
{
#ifdef _UNICODE
	
	int nLen = 0;
	nLen = MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, NULL, 0);
	if (len < nLen)
	{
		return NULL;
	}
	::memset(unicode, 0, len * sizeof(TCHAR));
	::MultiByteToWideChar(CP_UTF8, 0, (char*)uft8, -1, unicode, nLen);

#else
	strcpy(unicode, uft8);
#endif //_UNICODE
	return unicode;
}

TCHAR* GetLocalIP(TCHAR* pIp)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char Name[255] = {0};//�������ڴ�Ż�õ��������ı��� 
	char *IP = 0;//����IP��ַ���� 
	char chIP[MAX_PATH]  = {0};
	PHOSTENT hostinfo;
	if(gethostname (Name, sizeof(Name)) == 0)
	{ 
		//����ɹ��ؽ������������������name����ָ���Ļ������� 
		if((hostinfo = gethostbyname(Name)) != NULL) 
		{ 
			//���ǻ�ȡ���������������������ɹ��Ļ���������һ��ָ�룬ָ��hostinfo��hostinfo 
			//ΪPHOSTENT�͵ı��������漴���õ�����ṹ�� 
			IP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			int nIpLen = ::strlen(IP);

			::memcpy(chIP, IP, nIpLen);
			DWORD dwMun = ::MultiByteToWideChar(CP_ACP, 
				0, 
				chIP,
				-1, 
				NULL,
				0);
			dwMun = dwMun * sizeof(TCHAR);
			::MultiByteToWideChar(CP_ACP, 0,
				chIP, -1, pIp, dwMun);
		}
		else
		{
			//WRITE_LOG("gethostname Fail");
		}
	}
	else
	{
		long err = ::GetLastError();
		int i = 0;
	}

	::WSACleanup();
	return pIp;
}

BOOL CreateDirectoryMulti(TCHAR* path)
{
	if (path == NULL)
	{
		return FALSE;
	}
	::PathRemoveBackslash(path);
	::PathAddBackslash(path);
	TCHAR *tmp = (TCHAR*)path;
	while(*tmp)
	{
		if (*tmp == _T('\\') || *tmp == _T('/'))
		{
			*tmp = _T('\0');
			DWORD fileAttr = GetFileAttributes(path);
			if (fileAttr == 0xFFFFFFFF || !(fileAttr & FILE_ATTRIBUTE_DIRECTORY))
			{
				if(PathFileExists(path) == FALSE)
				{
					if (!::CreateDirectory((LPCTSTR) path, NULL))
					{
						return FALSE;
					}
				}
			}
			*tmp = _T('\\');
		}
		++tmp;
	}
	return TRUE;
}
