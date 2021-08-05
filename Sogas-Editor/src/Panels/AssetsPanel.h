#pragma once

#include <filesystem>

namespace Sogas {

	class AssetsPanel
	{
	public:
		AssetsPanel();

		void onImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;
	};
}
