#pragma once

//#include "sogas.h"
#include "core/application.h"
#include "core/layer.h"
//#include "core/cameraController.h"

#include "../external/glm/glm/glm.hpp"

//#include "renderer/resources/texture.h"
//#include "renderer/framebuffer.h"
//#include "renderer/shader.h"
//#include "scene/scene.h"
#include "Panels/ScenePanel.h"

#include "scene/types.h"

namespace Sogas 
{
	class CameraController;
	class Texture2D;
	class Shader;
	class Scene;
	class EntityFactory;
	class Framebuffer;

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate(f32 dt) override;
		virtual void onImguiRender() override;
		virtual void onEvent(Event& event) override;
	private:
		bool onKeyPressed(KeyPressedEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		
		void newScene();
		void openScene();
		void saveSceneAs();
	private:
		Framebuffer* m_framebuffer = nullptr;
		std::unique_ptr<CameraController> m_cameraController;

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<Shader> m_shader;

		std::unique_ptr<Scene> m_currentScene;

		// ECS
		std::shared_ptr<Scene> m_pScene;

		StrongEntityPtr m_cameraEntity;

		glm::vec2 m_viewportSize{ 0, 0 };

		glm::vec2 m_viewportBounds[2];

		bool m_viewportFocused = false;
		bool m_viewportHovered = false;

		i32 m_gizmoType = -1;

		glm::vec2 mouse_pos{ 0, 0 };

		// Panels
		ScenePanel m_scenePanel;
	};
}