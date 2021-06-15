#pragma once

//#include "sogas.h"
#include "core/application.h"
#include "core/layer.h"
#include "core/cameraController.h"
#include "actors/mesh.h"

#include "renderer/texture.h"
#include "renderer/framebuffer.h"
#include "renderer/shader.h"

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
	Framebuffer* m_framebuffer;
	std::unique_ptr<CameraController> m_cameraController;
	std::shared_ptr<Camera> m_camera;

	std::shared_ptr<VertexArray> m_vertexArray;
	std::shared_ptr<Texture2D> m_texture;
	std::shared_ptr<Shader> m_shader;

	glm::vec2 m_viewportSize{0, 0};

	bool m_viewportFocused = false;
	bool m_viewportHovered = false;

	Mesh* mesh;
	glm::vec2 mouse_pos;
};