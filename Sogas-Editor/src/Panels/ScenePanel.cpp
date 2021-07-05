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

		for (auto& entity : m_context.get()->getEntities())
		{
			ImGui::Text(entity->getName().c_str());
			//drawEntityNode(*entity);
		}

		// TODO: Add creation of entities

		// TODO: Add selection of entities

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

	}

	void ScenePanel::drawEntityComponents(Entity entity)
	{
	}
}