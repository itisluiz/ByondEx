#include <crash.hh>
#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <chrono>

namespace crash
{
	static void createMinidump(EXCEPTION_POINTERS* pExceptionInfo)
	{
		std::string dumpFilename = "crash_dump_" + std::to_string(std::time(nullptr)) + ".dmp";

		HANDLE hFile = CreateFileA
		(
			dumpFilename.c_str(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION dumpInfo{ };
			dumpInfo.ThreadId = GetCurrentThreadId();
			dumpInfo.ExceptionPointers = pExceptionInfo;
			dumpInfo.ClientPointers = FALSE;

			MiniDumpWriteDump
			(
				GetCurrentProcess(),
				GetCurrentProcessId(),
				hFile,
				MiniDumpWithDataSegs,
				&dumpInfo,
				NULL,
				NULL
			);

			CloseHandle(hFile);

			MessageBoxA
			(
				NULL,
				("Crash dump saved to \"" + dumpFilename + '\"').c_str(),
				"Crash",
				MB_OK | MB_ICONERROR
			);
		}
	}

	static LONG WINAPI customUnhandledExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo)
	{
		createMinidump(pExceptionInfo);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void setupHandler()
	{
		SetUnhandledExceptionFilter(customUnhandledExceptionFilter);
	}
}
