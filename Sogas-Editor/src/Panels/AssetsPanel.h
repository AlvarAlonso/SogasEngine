#pragma once

#include "renderer/resources/texture.h"

#include <filesystem>
#include <memory>

namespace Sogas {

	class AssetsPanel
	{
	public:
		AssetsPanel();

		void onImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;

		// Resource Icons
		std::shared_ptr<Texture2D> m_folderIcon;
		std::shared_ptr<Texture2D> m_fileIcon;
	};
}
