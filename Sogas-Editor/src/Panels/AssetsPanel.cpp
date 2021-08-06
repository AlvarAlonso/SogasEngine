#include "AssetsPanel.h"
#include "imgui.h"
#include "defines.h"

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

		static f32 itemRightPadding = 16.0f;
		static f32 thumbnailSize = 128.0f;
		f32 cellSize = thumbnailSize + itemRightPadding;

		f32 panelWidth = ImGui::GetContentRegionAvail().x;
		i32 columnCount = (i32)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for(auto& directoryItem : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryItem.path();
			auto relativePath = std::filesystem::relative(path, s_assetsDirectory);
			std::string relativeFilenameString = relativePath.filename().string();

			ImGui::Button(relativeFilenameString.c_str(), {thumbnailSize, thumbnailSize});
			if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryItem.is_directory())
					m_currentDirectory /= path.filename();
			}
			ImGui::Text(relativeFilenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &itemRightPadding, 0, 32);

		ImGui::End();
	}
}
