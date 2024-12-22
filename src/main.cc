#include <main.hh>
#include <state.hh>
#include <process/identity.hh>
#include <iostream>

static DWORD WINAPI entryThread(LPVOID lpParameter)
{
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    std::cout << "Hello from PID " << GetCurrentProcessId() << '\n';

    state::setup();
    return 0;
}

static DWORD WINAPI exitThread(LPVOID lpParameter)
{
    state::deinit();
    FreeLibraryAndExitThread(state::hModule, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
			if (process::identity::getByondIdentity() != process::identity::ByondIdentity::kDreamSeeker)
				return FALSE;

            DisableThreadLibraryCalls(hinstDLL);
            state::hModule = hinstDLL;
            HANDLE hEntryThread{ CreateThread(NULL, 0, entryThread, NULL, 0, NULL) };

            if (hEntryThread)
                CloseHandle(hEntryThread);
            else
                return FALSE;

            break;
        }
        case DLL_PROCESS_DETACH:
        {
            if (!state::unloading)
                state::deinit();

            break;
        }
    }

    return TRUE;
}

void unload()
{
    HANDLE hExitThread{ CreateThread(NULL, 0, exitThread, NULL, 0, NULL) };

    if (hExitThread)
        CloseHandle(hExitThread);
}
