


#include <shlobj.h>
#include <tchar.h>

#include <windows.h>
#include <iostream>
#include <io.h>
#include <direct.h>
#include <Windows.h>  

#include <stdio.h>  
#include <tchar.h>  
#include <conio.h> 
using namespace std;

//��װ����

wstring multiByteToWideChar(const string& pKey);
int GetSystemBits();
void Lnk(string &SourceFile, string &destFile, string &WorkDirectory);

/*******�������32λ��******/
void main() {
	
	if (GetSystemBits() != 32);
	char PWD[MAX_PATH];
	_getcwd(PWD, MAX_PATH);

	string p(PWD);
	p += "/vs2015���п�/vc_redist.VS2015.x64.exe";
	system(p.c_str());

	char path[255];
	SHGetSpecialFolderPath(0, path, CSIDL_DESKTOPDIRECTORY, 0);
	printf("%s", path);

	string lnk(path);
	lnk += "/ѧ���ɼ�����ϵͳ.lnk";
	Lnk(PWD +string("/ѧ���ɼ�����ϵͳ.exe"),lnk, string(PWD));
}


void Lnk(string &SourceFile, string &destFile, string &WorkDirectory) {
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink *pisl;
		hr = CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
		if (SUCCEEDED(hr))
		{
			IPersistFile* pIPF;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//����������Ҫ������ݷ�ʽ��ԭʼ�ļ���ַ
			pisl->SetPath(SourceFile.c_str());
			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);

			pisl->SetWorkingDirectory(WorkDirectory.c_str());
			if (SUCCEEDED(hr))
			{

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//����������Ҫ������ݷ�ʽ��Ŀ���ַ

				pIPF->Save(multiByteToWideChar(destFile.c_str()).c_str(), FALSE);
				pIPF->Release();
			}
			pisl->Release();
		}
		CoUninitialize();
	}

}

wstring multiByteToWideChar(const string& pKey)
{
	const char* pCStrKey = pKey.c_str();
	//��һ�ε��÷���ת������ַ������ȣ�����ȷ��Ϊwchar_t*���ٶ����ڴ�ռ�
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);

	wchar_t *pWCStrKey = new wchar_t[pSize];
	//�ڶ��ε��ý����ֽ��ַ���ת����˫�ֽ��ַ���
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
	wstring w(pWCStrKey, pSize);
	delete[] pWCStrKey;
	return w;
}




VOID SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo)
{
	if (NULL == lpSystemInfo)    return;
	typedef VOID(WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");;
	if (NULL != fnGetNativeSystemInfo)
	{
		fnGetNativeSystemInfo(lpSystemInfo);
	}
	else
	{
		GetSystemInfo(lpSystemInfo);
	}
}
// ��ȡ����ϵͳλ��  
int GetSystemBits()
{
	SYSTEM_INFO si;
	SafeGetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		return 64;
	}
	return 32;
}
