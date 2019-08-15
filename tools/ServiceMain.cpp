
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include "mainmonitor.h"
#include <strsafe.h>

SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;
FILE* fp;

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler (DWORD);
DWORD WINAPI ServiceWorkerThread (LPVOID lpParam);
int Main_Start();

#define SERVICE_NAME  _T("EMM Service")

#if IS_WINDOWS_SERVERICE
int _tmain(int argc, TCHAR *argv[])
#else
int _tmain12(int argc, TCHAR *argv[])
#endif
{
    OutputDebugString(_T("EMM Service: Main: Entry"));

    SERVICE_TABLE_ENTRY ServiceTable[] = 
    {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
        {NULL, NULL}
    };

    if (StartServiceCtrlDispatcher (ServiceTable) == FALSE)
    {
       OutputDebugString(_T("EMM Service: Main: StartServiceCtrlDispatcher returned error"));
       return GetLastError ();
    }

    OutputDebugString(_T("EMM Service: Main: Exit"));
    return 0;
}


VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	DWORD Status = E_FAIL;

	OutputDebugString(_T("EMM Service: ServiceMain: Entry"));

	g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
	do
	{

		if (g_StatusHandle == NULL)
		{
			OutputDebugString(_T("EMM Service: ServiceMain: RegisterServiceCtrlHandler returned error"));

			break;
		}

		// Tell the service controller we are starting
		ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
		g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwServiceSpecificExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 0;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			OutputDebugString(_T("EMM Service: ServiceMain: SetServiceStatus returned error"));
		}

		/*
		 * Perform tasks neccesary to start the service here
		 */
		OutputDebugString(_T("EMM Service: ServiceMain: Performing Service Start Operations"));

		// Create stop event to wait on later.
		g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (g_ServiceStopEvent == NULL)
		{
			OutputDebugString(_T("EMM Service: ServiceMain: CreateEvent(g_ServiceStopEvent) returned error"));

			g_ServiceStatus.dwControlsAccepted = 0;
			g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			g_ServiceStatus.dwWin32ExitCode = GetLastError();
			g_ServiceStatus.dwCheckPoint = 1;

			if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
			{
				OutputDebugString(_T("EMM Service: ServiceMain: SetServiceStatus returned error"));
			}
			break;
		}

		// Tell the service controller we are started
		g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
		g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 0;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			OutputDebugString(_T("EMM Service: ServiceMain: SetServiceStatus returned error"));
		}

		// Start the thread that will perform the main task of the service
	    HANDLE hThread = CreateThread (NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

		OutputDebugString(_T("EMM Service: ServiceMain: Waiting for Worker Thread to complete"));

		// Wait until our worker thread exits effectively signaling that the service needs to stop
		 WaitForSingleObject (hThread, INFINITE);



		OutputDebugString(_T("EMM Service: ServiceMain: Worker Thread Stop Event signaled"));


		/*
		 * Perform any cleanup tasks
		 */
		OutputDebugString(_T("EMM Service: ServiceMain: Performing Cleanup Operations"));

		CloseHandle(g_ServiceStopEvent);

		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 3;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			OutputDebugString(_T("EMM Service: ServiceMain: SetServiceStatus returned error"));
		}
		if (dbhelper != NULL)
		{
			delete dbhelper;
		}

	} while (0);

	OutputDebugString(_T("EMM Service: ServiceMain: Exit"));

	return;
}



//
// Purpose:
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation,
//     in milliseconds
//
// Return value:
//   None
//
VOID ReportSvcStatus(DWORD dwCurrentState,DWORD dwWin32ExitCode,DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	// Fill in the SERVICE_STATUS structure.
	g_ServiceStatus.dwCurrentState = dwCurrentState;
	g_ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
	g_ServiceStatus.dwWaitHint = dwWaitHint;
	if (dwCurrentState == SERVICE_START_PENDING)
	{
		g_ServiceStatus.dwControlsAccepted = 0;
	}
	else
	{
		g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	}
	if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
	{
		g_ServiceStatus.dwCheckPoint = 0;
	}
	else
	{
		g_ServiceStatus.dwCheckPoint = dwCheckPoint++;
	}
	// Report the status of the service to the SCM.
	SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}



VOID WINAPI ServiceCtrlHandler (DWORD CtrlCode)
{
    OutputDebugString(_T("EMM Service: ServiceCtrlHandler: Entry"));

	switch (CtrlCode)
	{
	case SERVICE_CONTROL_STOP:
	{
		OutputDebugString(_T("EMM Service: ServiceCtrlHandler: SERVICE_CONTROL_STOP Request"));
		IsStoped = true;
		if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
		{
			break;
		}
		/*
		* Perform tasks neccesary to stop the service here
		*/
		LogScreen("recv SERVICE_CONTROL_STOP signal ready exit");
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		g_ServiceStatus.dwWin32ExitCode = 0;
		g_ServiceStatus.dwCheckPoint = 4;

		if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
		{
			OutputDebugString(_T("EMM Service: ServiceCtrlHandler: SetServiceStatus returned error"));
		}

		// This will signal the worker thread to start shutting down
		SetEvent(g_ServiceStopEvent);
		
		break;
	}

	case SERVICE_CONTROL_CONTINUE:
	{
		// Do whatever it takes to continue here. 
		g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		IsStoped = false;
		IsPause = false;
		LogScreen("service recv SERVICE_CONTROL_CONTINUE signal\n");
		break;
	}

	case SERVICE_CONTROL_INTERROGATE:
	{
		LogScreen("service recv SERVICE_CONTROL_INTERROGATE signal\n");
		break;
	}
	/*
	#define SERVICE_STOPPED                        0x00000001     已停止
	#define SERVICE_START_PENDING                  0x00000002     启动（正在启动）
	#define SERVICE_STOP_PENDING                   0x00000003     停止（正在停止）
	#define SERVICE_RUNNING                        0x00000004     已启动
	#define SERVICE_CONTINUE_PENDING               0x00000005     恢复（正在恢复-由已暂停至已启动）
	#define SERVICE_PAUSE_PENDING                  0x00000006     暂停（正在暂停）
	#define SERVICE_PAUSED                         0x00000007     已暂停
	*/

	case SERVICE_CONTROL_PAUSE:
	{
		LogScreen("service recv SERVICE_CONTROL_PAUSE signal\n");
		IsStoped = true;
		break;
	}
	default:
	{
		LogScreen("service recv Unknow signal\n");
		IsStoped = true;
		break;
	}
	}
	LogScreen("EMM Service: ServiceCtrlHandler: Exit\n");
    OutputDebugString(_T("EMM Service: ServiceCtrlHandler: Exit"));
	ReportSvcStatus(g_ServiceStatus.dwCurrentState, NO_ERROR, 0);
}

DWORD WINAPI ThreadFunc1(LPVOID lpParameter)
{
	while (WaitForSingleObject(g_ServiceStopEvent, (1000 * 5)) != WAIT_OBJECT_0)
	{
		char buff[64] = {0};
		FormattTime(time(NULL), buff, sizeof(buff), "%Y-%m-%d %H:%M:%S");
		char log[128] = { 0 };
		sprintf(log, "%s %s line %d \n", buff,  __FUNCTION__, __LINE__);
		LogScreen(log);
		//std::chrono::milliseconds dura((long)5000);
		//std::this_thread::sleep_for(dura);
	}
	LogScreen("ThreadFunc1 executed over---------------");
	return NULL;
}

void ThreadFunc2()
{
	while (!IsStoped)
	{
		char buff[64] = { 0 };
		FormattTime(time(NULL), buff, sizeof(buff), "%Y-%m-%d %H:%M:%S");
		char log[128] = { 0 };
		sprintf(log, "%s thread %s line %d \n", buff, std::this_thread::get_id(), __FUNCTION__, __LINE__);
		LogScreen(log);
		std::chrono::milliseconds dura((long)3000);
		std::this_thread::sleep_for(dura);
	}
	LogScreen("ThreadFunc1 executed over---------------");
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	INT nThreadNum = (INT)lpParameter;
	TCHAR szOutput[25];

	while (WaitForSingleObject(g_ServiceStopEvent, (1000*(nThreadNum+1))) != WAIT_OBJECT_0)
	{
		// Just to have something to do, it will beep every second.
		//Sleep(1000);
		wsprintf(szOutput, TEXT(" Thread %d says Beep "), nThreadNum);
		char tbuf[64] = { 0 };
		FormattTime(time(NULL), tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S");
		fprintf(fp, "%s now thread num %ld \n", tbuf, nThreadNum);
		fflush(fp);
		LogScreen(tbuf);
		OutputDebugString(szOutput); //Send visual to debugger.
	}

	return 0;
}

HANDLE hThreads[3] = { NULL, NULL, NULL };

int Main_Start()
{

	//std::thread t1(ThreadFunc1,(VOID*)NULL);
	//t1.detach();
	//t1.join();
	//std::thread t2(ThreadFunc2);
	//t2.detach();

	/*
	int t;
	for (t = 0; t<1; t++)
	{
		hThreads[t] = CreateThread(NULL, 0, ThreadProc,(LPVOID)t, 0, NULL);
		if (hThreads[t] == INVALID_HANDLE_VALUE)
		{

		}
	}
	*/


	HANDLE hThreads1 = CreateThread(NULL, 0, ThreadFunc1, (LPVOID)1, 0, NULL);

	return 0;
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
	OutputDebugString(_T("EMM Service: ServiceWorkerThread: Entry"));
	int i = 0;

	char chpath[MAX_PATH];
	GetModuleFileNameA(NULL, chpath, sizeof(chpath));
	int ret = 0;
	ret=main_monitor(1, (char**)chpath);
	//ret = Main_Start();
	if (ret != 0)
	{
		LogScreen("ServiceWorkerThread main_monitor return not is zero");
		return ERROR_SUCCESS;
	}
	/*
	fp = fopen("c:\\tservice.txt", "ab+");
	char tbuf[64] = { 0 };
	FormattTime(time(NULL), tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S");
	fprintf(fp, "%s main thread ready wait for stop signal \n", tbuf);
	fflush(fp);
	*/
	
	//  Periodically check if the service has been requested to stop
	while (WaitForSingleObject(g_ServiceStopEvent, 1000) != WAIT_OBJECT_0)
	{
		/*
		 * Perform main service function here
		 */
		
		//fprintf(fp, "%s %dst  \n", tbuf, i);
		//fflush(fp);
		++i;
		// Simulate some work by sleeping
		//Sleep(2000);
	
	}
	//fprintf(fp, "main thread recv stop signal ready over.... \n");
	//fclose(fp);
	IsStoped = true;
	fclose(logfp);
	OutputDebugString(_T("EMM Service: ServiceWorkerThread: Exit"));

	return ERROR_SUCCESS;
}

//
// Purpose:
//   Installs a service in the SCM database
//
// Parameters:
//   None
//
// Return value:
//   None
//
VOID SvcInstall()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR szPath[MAX_PATH];
	if (!GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		printf("Cannot install service (%d)\n", GetLastError());
		return;
	}
	// Get a handle to the SCM database.

	schSCManager = OpenSCManager(
		NULL,                    // local computer
		NULL,                    // ServicesActive database
		SC_MANAGER_ALL_ACCESS);  // full access rights

	if (NULL == schSCManager)
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return;
	}
	// Create the service
	schService = CreateService(
		schSCManager,              // SCM database
		SERVICE_NAME,                   // name of service
		SERVICE_NAME,                   // service name to display
		SERVICE_ALL_ACCESS,        // desired access
		SERVICE_WIN32_OWN_PROCESS, // service type
		SERVICE_DEMAND_START,      // start type
		SERVICE_ERROR_NORMAL,      // error control type
		szPath,                    // path to service's binary
		NULL,                      // no load ordering group
		NULL,                      // no tag identifier
		NULL,                      // no dependencies
		NULL,                      // LocalSystem account
		NULL);                     // no password

	if (schService == NULL)
	{
		printf("CreateService failed (%d)\n", GetLastError());
		CloseServiceHandle(schSCManager);
		return;
	}
	else printf("Service installed successfully\n");
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
}