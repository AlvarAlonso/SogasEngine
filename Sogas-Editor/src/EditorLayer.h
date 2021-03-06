#pragma once

//#include "sogas.h"
#include "core/application.h"
#include "core/layer.h"
#include "scene/types.h"
#include "Panels/ScenePanel.h"
#include "Panels/AssetsPanel.h"
#include "glm/glm.hpp"

#define SOGAS_EDITOR

namespace Sogas 
{
	class CameraController;
	class Scene;
	class EntityFactory;
	class Framebuffer;
	class Camera;
	class ScenePanel;
	class Mesh;

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
		void saveScene();
		void saveSceneAs();

	private:
		Framebuffer* m_framebuffer = nullptr;
		std::unique_ptr<CameraController> m_cameraController;
		std::shared_ptr<Camera> m_pCamera;
		std::shared_ptr<Camera> m_oldCamera; // [TEMPORAL]: To be removed

		std::shared_ptr<Scene> m_pScene;
		std::shared_ptr<Mesh> m_pGrid;	// Should grid be defined in editor?

		glm::vec2 m_viewportSize{ 0, 0 };

		glm::vec2 m_viewportBounds[2];

		bool m_viewportFocused = false;
		bool m_viewportHovered = false;
		std::string m_savePath;

		i32 m_gizmoType = -1;

		glm::vec2 mouse_pos{ 0, 0 };

		// Panels
		ScenePanel m_scenePanel;
		AssetsPanel m_assetsPanel;

		std::weak_ptr<Entity> m_hoveredEntity;
		i32 m_entityIdHovered;
	};
}