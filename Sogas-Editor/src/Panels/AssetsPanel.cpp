#include "AssetsPanel.h"
#include "imgui.h"
#include "defines.h"

namespace Sogas {

	extern const std::filesystem::path g_assetsDirectory = "../Assets";

	AssetsPanel::AssetsPanel()
		: m_currentDirectory(g_assetsDirectory)
	{
		m_folderIcon = Texture2D::create("resources/folder-icon.png");
		m_fileIcon = Texture2D::create("resources/file-icon.png");
	}

	void AssetsPanel::onImGuiRender()
	{
		ImGui::Begin("Assets");

		if(m_currentDirectory != std::filesystem::path(g_assetsDirectory))
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
			auto relativePath = std::filesystem::relative(path, g_assetsDirectory);
			std::string relativeFilenameString = relativePath.filename().string();

			ImGui::PushID(relativeFilenameString.c_str());

			if(directoryItem.is_directory())
			{
				ImGui::ImageButton((ImTextureID)m_folderIcon->getID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			else
			{
				ImGui::ImageButton((ImTextureID)m_fileIcon->getID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}

			if(ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("ASSETS_PANEL_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryItem.is_directory())
					m_currentDirectory /= path.filename();
			}
			ImGui::Text(relativeFilenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &itemRightPadding, 0, 32);

		ImGui::End();
	}
}
