#include "ScenePanel.h"

#include <imgui.h>

namespace Sogas
{
	ScenePanel::ScenePanel(const std::shared_ptr<Scene>& scene)
	{
		setContext(scene);
	}

	void ScenePanel::setContext(const std::shared_ptr<Scene>& scene)
	{
		m_context = scene;
		m_selectedEntity = {};
	}

	void ScenePanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//SGSINFO("Entity Selected %i", m_selectedEntity.getId());

		for (auto& entity : m_context.get()->getEntities())
		{
			//ImGui::Text(entity->getName().c_str());
			drawEntityNode(*entity);
		}

		// TODO: Add creation of entities

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectedEntity = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if(m_selectedEntity)
		{
			drawEntityComponents(m_selectedEntity);
		}

		ImGui::End();
	}
	void ScenePanel::setSelectedEntity(Entity entity)
	{
		m_selectedEntity = entity;
	}

	void ScenePanel::drawEntityNode(Entity entity)
	{
		ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity.getId(), flags, entity.getName().c_str());
		if(ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}

		if(opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, entity.getName().c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	void ScenePanel::drawEntityComponents(Entity entity)
	{
	}
}