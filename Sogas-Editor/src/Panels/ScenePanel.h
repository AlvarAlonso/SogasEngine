#pragma once

#include "defines.h"
#include "scene/scene.h"
#include "scene/entity.h"

namespace Sogas {

	class ScenePanel
	{
	public:
		ScenePanel() = default;
		ScenePanel(const std::shared_ptr<Scene>& scene);

		void setContext(const std::shared_ptr<Scene>& scene);
		void onImGuiRender();

		Entity getSelectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(Entity entity);
	private:
		EntityId drawEntityNode(Entity entity);
		void drawEntityComponents(Entity entity);
	private:
		std::shared_ptr<Scene> m_context;
		Entity m_selectedEntity;
	};
}
