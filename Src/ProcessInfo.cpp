///
/// Copyright (C) 2013 - All Rights Reserved
/// All rights reserved. http://www.equals-forty-two.com
///

#include "Precompiled.h"
#include "ProcessInfo.h"

#ifdef WIN32

#define _WIN32_DCOM

#include <comdef.h>
#include <Wbemidl.h>
#include <winternl.h>

# pragma comment(lib, "wbemuuid.lib")

#include <windows.h>
#include <stdio.h>

typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(
	HANDLE ProcessHandle,
	DWORD ProcessInformationClass,
	PVOID ProcessInformation,
	DWORD ProcessInformationLength,
	PDWORD ReturnLength
	);

	struct __RTL_DRIVE_LETTER_CURDIR {


		USHORT                  Flags;
		USHORT                  Length;
		ULONG                   TimeStamp;
		UNICODE_STRING          DosPath;




	};

	typedef struct ___RTL_USER_PROCESS_PARAMETERS {


		ULONG                   MaximumLength;
		ULONG                   Length;
		ULONG                   Flags;
		ULONG                   DebugFlags;
		PVOID                   ConsoleHandle;
		ULONG                   ConsoleFlags;
		HANDLE                  StdInputHandle;
		HANDLE                  StdOutputHandle;
		HANDLE                  StdErrorHandle;
		UNICODE_STRING          CurrentDirectoryPath;
		HANDLE                  CurrentDirectoryHandle;
		UNICODE_STRING          DllPath;
		UNICODE_STRING          ImagePathName;
		UNICODE_STRING          CommandLine;
		PVOID                   Environment;
		ULONG                   StartingPositionLeft;
		ULONG                   StartingPositionTop;
		ULONG                   Width;
		ULONG                   Height;
		ULONG                   CharWidth;
		ULONG                   CharHeight;
		ULONG                   ConsoleTextAttributes;
		ULONG                   WindowFlags;
		ULONG                   ShowWindowFlags;
		UNICODE_STRING          WindowTitle;
		UNICODE_STRING          DesktopName;
		UNICODE_STRING          ShellInfo;
		UNICODE_STRING          RuntimeData;
		__RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];

	} __RTL_USER_PROCESS_PARAMETERS;


PVOID GetPebAddress(HANDLE ProcessHandle)
{
	_NtQueryInformationProcess NtQueryInformationProcess =
		(_NtQueryInformationProcess)GetProcAddress(
		GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	PROCESS_BASIC_INFORMATION pbi;

	NtQueryInformationProcess(ProcessHandle, 0, &pbi, sizeof(pbi), NULL);

	return pbi.PebBaseAddress;
}

QString GetWorkingDir(quint32 pid)
{
	HANDLE processHandle;
	PVOID pebAddress;
	PVOID rtlUserProcParamsAddress;
	UNICODE_STRING currentDir;
	WCHAR* currentDirContents;

	if ((processHandle = OpenProcess(
		PROCESS_QUERY_INFORMATION | /* required for NtQueryInformationProcess */
		PROCESS_VM_READ, /* required for ReadProcessMemory */
		FALSE, pid)) == 0)
	{
		printf("Could not open process!\n");
		return "";
	}

	pebAddress = GetPebAddress(processHandle);

	/* get the address of ProcessParameters */
	if (!ReadProcessMemory(processHandle, (PCHAR)pebAddress + offsetof(PEB,ProcessParameters),
		&rtlUserProcParamsAddress, sizeof(PVOID), NULL))
	{
		printf("Could not read the address of ProcessParameters!\n");
		return "";
	}

	if ( rtlUserProcParamsAddress == 0 ) 
		return "";

	/* read the CommandLine UNICODE_STRING structure */
	if (!ReadProcessMemory(processHandle, (PCHAR)rtlUserProcParamsAddress + offsetof(__RTL_USER_PROCESS_PARAMETERS, CurrentDirectoryPath),
		&currentDir, sizeof(currentDir), NULL))
	{
		printf("Could not read CommandLine!\n");
		return "";
	}

	/* allocate memory to hold the command line */
	currentDirContents = (WCHAR *)malloc(currentDir.Length+2);
	ZeroMemory(currentDirContents, currentDir.Length+2);

	/* read the current directory */
	if (!ReadProcessMemory(processHandle, currentDir.Buffer,
		currentDirContents, currentDir.Length, NULL))
	{
		printf("Could not read the command line string!\n");
		return "";
	}

	/* print it */
	/* the length specifier is in characters, but commandLine.Length is in bytes */
	/* a WCHAR is 2 bytes */
	QString currentDirQStr = QString::fromWCharArray(currentDirContents);

	CloseHandle(processHandle);
	free(currentDirContents);

	return currentDirQStr;
}

QList<ProcessInfo> GetProcessInfoList(const QString& query)
{
	QList<ProcessInfo> infos;
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------

	hres =  CoInitializeSecurity(
		NULL, 
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);


	if (FAILED(hres))
	{
		qDebug() << "Failed to initialize security. Error code = 0x" 
			<<  QString("%1").arg(hres, 0, 16) << endl;
		CoUninitialize();
		return QList<ProcessInfo>();                    // Program has failed.
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *) &pLoc);

	if (FAILED(hres))
	{
		qDebug() << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<<  QString("%1").arg(hres, 0, 16) << endl;
		CoUninitialize();
		return QList<ProcessInfo>();                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		qDebug() << "Could not connect. Error code = 0x" 
			<<  QString("%1").arg(hres, 0, 16) << endl;
		pLoc->Release();     
		CoUninitialize();
		return QList<ProcessInfo>();                // Program has failed.
	}

	qDebug() << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		qDebug() << "Could not set proxy blanket. Error code = 0x" 
			<<  QString("%1").arg(hres, 0, 16) << endl;
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return QList<ProcessInfo>();               // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t((const char*)query.toAscii()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		qDebug() << "Query for operating system name failed."
			<< " Error code = 0x" 
			<<  QString("%1").arg(hres, 0, 16) << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return QList<ProcessInfo>();               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		ProcessInfo pi;

		VARIANT vtProp;

		// Get process PID
		hr = pclsObj->Get(L"ProcessId", 0, &vtProp, 0, 0);
		pi.pid = vtProp.uintVal;
		VariantClear(&vtProp);

		// Get the name of the process
		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		pi.name = QString::fromWCharArray(vtProp.bstrVal);
		VariantClear(&vtProp);

		// Get the command line info
		hr = pclsObj->Get(L"CommandLine", 0, &vtProp, 0, 0);
		pi.commandLine = QString::fromWCharArray(vtProp.bstrVal);
		VariantClear(&vtProp);

		// Get process working directory
		hr = pclsObj->Get(L"ExecutablePath", 0, &vtProp, 0, 0);
		pi.workingDir = QString::fromWCharArray(vtProp.bstrVal);
		VariantClear(&vtProp);

		pi.workingDir = GetWorkingDir( pi.pid );

		// Split the command line into arguments
		LPWSTR* szArglist = nullptr;
		int nArgs = 0;

		const size_t cmdLineBufSize = pi.commandLine.count() + 1;
		LPWSTR szCmdLine = new WCHAR[ cmdLineBufSize ];
		ZeroMemory(szCmdLine, sizeof(WCHAR) * cmdLineBufSize );
		pi.commandLine.toWCharArray(szCmdLine);
		szArglist = CommandLineToArgvW(szCmdLine, &nArgs);
		if( NULL == szArglist )
		{
			qDebug() << "CommandLineToArgv failed";
		}
		else 
		{
			for(int i=0; i<nArgs; i++) 
				pi.args << QString::fromWCharArray(szArglist[i]);
		}
		delete [] szCmdLine;

		// Free memory allocated for CommandLineToArgvW arguments.
		LocalFree(szArglist);
		
		pclsObj->Release();

		infos.push_back( pi );
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();

	return infos;
}

#else

QList<ProcessInfo> GetProcessInfoList(const QString& query)
{
	return QList<ProcessInfo>();
}

#endif

bool GetProcessInfo(quint32 processId, ProcessInfo& out_Info)
{
#ifdef WIN32

	QList<ProcessInfo> infos = GetProcessInfoList(QString("SELECT * FROM Win32_Process WHERE ProcessId = %1").arg(processId));
	if ( infos.empty() )
		return false;

	out_Info = infos.front();
	return true;

#else

	return false;

#endif
}

QList<ProcessInfo> GetProcessInfos( const QString& processName /*= ""*/ )
{
#ifdef WIN32

	return GetProcessInfoList(QString("SELECT * FROM Win32_Process WHERE Name = '%1'").arg(processName));

#else

	return QList<ProcessInfo>();

#endif
}

QList<ProcessInfo> GetAllProcessInfo()
{
#ifdef WIN32

	return GetProcessInfoList(QString("SELECT * FROM Win32_Process "));

#else

	return QList<ProcessInfo>();

#endif
}

