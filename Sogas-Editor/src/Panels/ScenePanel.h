#pragma once

#include "defines.h"
#include "scene/scene.h"

namespace Sogas {

	class ScenePanel
	{
	public:
		ScenePanel(const std::shared_ptr<Scene>& scene);

		void setContext(const std::shared_ptr<Scene>& scene);
		void onImGuiRender();
	private:

		std::shared_ptr<Scene> m_context;
	};
}
