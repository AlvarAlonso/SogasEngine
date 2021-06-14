//#pragma once
//
//#include "sogas.h"
//
//class Shader;
//
//class EditorLayer : public Layer
//{
//public:
//	EditorLayer();
//	virtual ~EditorLayer() = default;
//
//	virtual void onAttach() override;
//	virtual void onDetach() override;
//
//	virtual void onUpdate(f32 dt) override;
//	virtual void onImguiRender() override;
//	virtual void onEvent(Event& event) override;
//
//private:
//	Framebuffer* m_framebuffer;
//	Camera* m_camera;
//
//	std::shared_ptr<VertexArray> m_vertexArray;
//	std::shared_ptr<Texture2D> m_texture;
//	std::shared_ptr<Shader> m_shader;
//
//	Mesh* mesh;
//	glm::vec2 mouse_pos;
//};