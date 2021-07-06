#pragma once

#include "defines.h"
#include "scene/scene.h"
#include "scene/entity.h"

namespace ImGui
{
	void PushMultiItemsWidths(int components, float width_full);
}

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

		template<typename T, typename UIFunction>
		void drawComponent(const std::string& name, Entity entity, UIFunction uiFuntion);
	private:
		std::shared_ptr<Scene> m_context;
		Entity m_selectedEntity;
	};
}
