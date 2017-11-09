// MessageBox定义

#include "kgl_lib_pch.h"
#include "kgl_message_box.h"

namespace kgl
{
#if defined(WIN32) || defined(_WIN32)
	
	void MessageBox(const std::wstring& title, const std::wstring& content)
	{
		::ShowCursor(true);

		int ret = ::MessageBoxW(NULL, content.c_str(), title.c_str(), MB_YESNO | MB_ICONERROR);

		if (ret == IDYES)
		{
			ret = ::MessageBoxW(NULL, L"Terminate now (exit(1)) ?", L"Eject! Eject!", MB_YESNO | MB_ICONQUESTION);

			if (ret == IDYES)
			{
				exit(1);
			}
			else
			{
				::PostQuitMessage(0);
			}
		}
	}
#endif
}