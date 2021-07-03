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
	}

	void ScenePanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//for (auto& entity : m_context.get()->getEntities())
		//{
		//
		//}

		ImGui::End();
	}
}