#include "sgspch.h"

#include <Windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

//#include "windowsUtils.h"
#include "platform/utils/platformUtils.h"
#include "core/application.h"

namespace Sogas
{
	inline std::string FileDialog::openFile(const char* filter)
	{
		OPENFILENAME ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize		= sizeof(OPENFILENAME);
		ofn.hwndOwner		= glfwGetWin32Window((GLFWwindow*)Application::getInstance()->getWindow().getNativeWindow());
		ofn.lpstrFile		= szFile; 
		ofn.nMaxFile		= sizeof(szFile);
		ofn.lpstrFilter		= filter;
		ofn.nFilterIndex	= 1;
		ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}

	inline std::string FileDialog::saveFile(const char* filter)
	{
		OPENFILENAME ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize		= sizeof(OPENFILENAME);
		ofn.hwndOwner		= glfwGetWin32Window((GLFWwindow*)Application::getInstance()->getWindow().getNativeWindow());
		ofn.lpstrFile		= szFile;
		ofn.nMaxFile		= sizeof(szFile);
		ofn.lpstrFilter		= filter;
		ofn.nFilterIndex	= 1;
		ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileName(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
}