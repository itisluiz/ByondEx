#include <hooks/windows.hh>
#include <gui/imguiboiler.hh>
#include <state.hh>

namespace hooks::windows
{
	namespace wndproc
	{
		LRESULT CALLBACK hook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (GetCapture() == hwnd)
			{
				LRESULT ogResult{ CallWindowProcW(originalProc, hwnd, msg, wParam, lParam) };
				HWND newCapture{ GetCapture() };

				if (newCapture != hwnd)
					state::hDragWindow = newCapture;

				return ogResult;
			}

			return CallWindowProcW(originalProc, hwnd, msg, wParam, lParam);
		}

		WNDPROC originalProc;
	}

	namespace getmsgproc
	{
		LRESULT CALLBACK hook(int nCode, WPARAM wParam, LPARAM lParam)
		{
			if (nCode < 0)
				return CallNextHookEx(NULL, nCode, wParam, lParam);

			MSG* lpMsg{ reinterpret_cast<MSG*>(lParam) };
			MSG msgCopy{ *lpMsg };

			if (msgCopy.hwnd == state::hDragWindow)
			{
				if (msgCopy.message >= WM_MOUSEFIRST && msgCopy.message <= WM_MOUSELAST)
				{
					POINT pt;
					pt.x = msgCopy.lParam & 0xFFFF;
					pt.y = (msgCopy.lParam >> 16) & 0xFFFF;

					ClientToScreen(msgCopy.hwnd, &pt);
					ScreenToClient(state::hRenderWindow, &pt);
					
					msgCopy.lParam = (pt.x & 0xFFFF) | ((pt.y & 0xFFFF) << 16);
				}

			}

			if (msgCopy.hwnd == state::hRenderWindow)
			{
				static DWORD lastKeyTime{ 0 };

				if (msgCopy.message == WM_KEYDOWN && msgCopy.time != lastKeyTime)
				{
					lastKeyTime = msgCopy.time;

					BYTE keys[256]{ };
					keys[VK_SHIFT] = (GetKeyState(VK_SHIFT) & 0x8000) ? 0x80 : 0;
					keys[VK_CONTROL] = (GetKeyState(VK_CONTROL) & 0x8000) ? 0x80 : 0;
					keys[VK_MENU] = (GetKeyState(VK_MENU) & 0x8000) ? 0x80 : 0;

					WCHAR charBuffer[4]{ };
					INT32 copiedChars{ ToUnicodeEx(msgCopy.wParam, msgCopy.lParam & 0xFFFF, keys, charBuffer, 4, 0, NULL) };
					for (INT32 i{ 0 }; i < copiedChars; i++)
						gui::imguiboiler::handleMessage(state::hRenderWindow, WM_CHAR, charBuffer[i], NULL);
				}
				else if (msgCopy.message == WM_MOUSELEAVE && GetCapture() == state::hDragWindow)
					msgCopy.message = WM_NULL;

			}

			if (msgCopy.hwnd == state::hRenderWindow || msgCopy.hwnd == state::hDragWindow)
			{
				if (gui::imguiboiler::handleMessage(state::hRenderWindow, msgCopy.message, msgCopy.wParam, msgCopy.lParam))
					lpMsg->message = WM_NULL;
			}

			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}

		HHOOK hookHandle;
	}

	bool setup()
	{
		wndproc::originalProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(state::hRenderWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc::hook)));
		if (!wndproc::originalProc)
			return false;

		if (!(getmsgproc::hookHandle = SetWindowsHookExW(WH_GETMESSAGE, getmsgproc::hook, state::hModule, NULL)))
			return false;

		return true;
	}

	void deinit()
	{
		if (getmsgproc::hookHandle)
			UnhookWindowsHookEx(getmsgproc::hookHandle);

		if (wndproc::originalProc)
			SetWindowLongPtrW(state::hRenderWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc::originalProc));
	}
}

