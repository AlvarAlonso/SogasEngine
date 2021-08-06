#include "AssetsPanel.h"
#include "imgui.h"

namespace Sogas {

	static const std::filesystem::path s_assetsDirectory = "../Assets";

	AssetsPanel::AssetsPanel()
		: m_currentDirectory(s_assetsDirectory)
	{
	}

	void AssetsPanel::onImGuiRender()
	{
		ImGui::Begin("Assets");

		if(m_currentDirectory != std::filesystem::path(s_assetsDirectory))
		{
			if(ImGui::Button("Back"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		for(auto& directoryItem : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryItem.path();
			auto relativePath = std::filesystem::relative(path, s_assetsDirectory);
			std::string relativeFilenameString = relativePath.filename().string();

			if(directoryItem.is_directory())
			{
				if(ImGui::Button(relativeFilenameString.c_str()))
				{
					m_currentDirectory /= path.filename();
				}
			}
			else
			{
				if(ImGui::Button(relativeFilenameString.c_str()))
				{
				}
			}
		}

		ImGui::End();
	}
}
