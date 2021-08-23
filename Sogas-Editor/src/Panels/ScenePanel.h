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

		WeakEntityPtr getSelectedEntity() const { return m_selectedEntity; }
		void setSelectedEntity(StrongEntityPtr entity);
	private:
		EntityId drawEntityNode(StrongEntityPtr entity);
		void drawEntityComponents(WeakEntityPtr entity);

		template<typename T, typename UIFunction>
		void drawComponent(const std::string& name, StrongEntityPtr entity, UIFunction uiFuntion);
	private:
		std::shared_ptr<Scene> m_context;
		WeakEntityPtr m_selectedEntity;
	};
}
