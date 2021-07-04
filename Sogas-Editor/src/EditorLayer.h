#pragma once

//#include "sogas.h"
#include "core/application.h"
#include "core/layer.h"

#include "../external/glm/glm/glm.hpp"

#include "scene/types.h"

namespace Sogas 
{
	class CameraController;
	class Texture2D;
	class Shader;
	class Scene;
	class EntityFactory;
	class Framebuffer;
	class Camera;

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
		Framebuffer* m_framebuffer = nullptr;
		std::unique_ptr<CameraController> m_cameraController;
		std::shared_ptr<Camera> m_pCamera;
		std::shared_ptr<Texture2D> m_texture;

		// ECS
		std::shared_ptr<Scene> m_pScene;

		StrongEntityPtr m_cameraEntity;

		glm::vec2 m_viewportSize{ 0, 0 };

		bool m_viewportFocused = false;
		bool m_viewportHovered = false;

		glm::vec2 mouse_pos{ 0, 0 };
	};
}