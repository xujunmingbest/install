


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

//安装程序

wstring multiByteToWideChar(const string& pKey);
int GetSystemBits();
void Lnk(string &SourceFile, string &destFile, string &WorkDirectory);

/*******打包程序32位的******/
void main() {
	
	if (GetSystemBits() != 32);
	char PWD[MAX_PATH];
	_getcwd(PWD, MAX_PATH);

	string p(PWD);
	p += "/vs2015运行库/vc_redist.VS2015.x64.exe";
	system(p.c_str());

	char path[255];
	SHGetSpecialFolderPath(0, path, CSIDL_DESKTOPDIRECTORY, 0);
	printf("%s", path);

	string lnk(path);
	lnk += "/学生成绩管理系统.lnk";
	Lnk(PWD +string("/学生成绩管理系统.exe"),lnk, string(PWD));
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

			//这里是我们要创建快捷方式的原始文件地址
			pisl->SetPath(SourceFile.c_str());
			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);

			pisl->SetWorkingDirectory(WorkDirectory.c_str());
			if (SUCCEEDED(hr))
			{

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//这里是我们要创建快捷方式的目标地址

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
	//第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);

	wchar_t *pWCStrKey = new wchar_t[pSize];
	//第二次调用将单字节字符串转换成双字节字符串
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
// 获取操作系统位数  
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
