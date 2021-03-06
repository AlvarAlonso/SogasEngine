#include <iostream>
#include "sogas.h"

#include "../external/glm/glm/gtc/matrix_transform.hpp"
#include "renderer/resources/mesh.h"
#include "scene/entity.h"

class ExampleLayer : public Sogas::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		// load texture
		m_texture = Sogas::Texture2D::create("../Assets/texture.png");
		m_texture = Sogas::Texture2D::create("../Assets/brick_wall_2k.jpg");
		m_defaultTexture = Sogas::Texture2D::create(1, 1);
		uint32_t defaultTextureData = 0xffffffff;
		m_defaultTexture->setData(&defaultTextureData, sizeof(uint32_t));

		//mesh = new Sogas::Mesh();
		//mesh->load("../Assets/viking_room.obj");

		Sogas::Renderable* renderable = new Sogas::Renderable();
		renderable->m_prefab.get()->get("../Assets/cube.obj");

		m_scene.get()->getEntities().push_back(renderable);

		m_shader.reset(Sogas::Shader::create("../SogasEngine/shaders/basic.shader"));

		m_camera = new Sogas::Camera();
		m_camera->setPosition({ 0, 15, -50 });

		mouse_pos = { Sogas::Application::getInstance()->getWindow().getWidth(), Sogas::Application::getInstance()->getWindow().getHeight() };

	}

	void onAttach()
	{
		Sogas::FramebufferSpecs specs;
		specs.width = 1280; // Application::getInstance()->getWindow().getWidth();
		specs.height = 720; // Application::getInstance()->getWindow().getHeight();

		m_framebuffer = Sogas::Framebuffer::create(specs);
	}

	void onUpdate(f32 dt) override
	{
		m_framebuffer->bind();

		Sogas::Renderer::setClearColor(glm::vec4( 0.2 ));
		Sogas::Renderer::setDepthBuffer(true);
		Sogas::Renderer::clear();
		
		if (x < -1.0f || x > 1.0f) {
			inc *= -1;
		}

		x += inc;
		
		// Should dt be stored as a class variable and used in the events through the dispatcher???
		if (Sogas::Input::isKeyPressed(SGS_KEY_A)){
			m_camera->move(Sogas::LEFT, dt);
		}
		else if (Sogas::Input::isKeyPressed(SGS_KEY_D)){
			m_camera->move(Sogas::RIGHT, dt);
		}

		if (Sogas::Input::isKeyPressed(SGS_KEY_W)){
			m_camera->move(Sogas::FORWARD, dt);
		}
		else if (Sogas::Input::isKeyPressed(SGS_KEY_S)){
			m_camera->move(Sogas::BACKWARD, dt);
		}

		m_shader->bind();
		m_texture->bind();
		for (auto& renderable : m_scene.get()->getEntities())
		{
			std::dynamic_pointer_cast<Sogas::OpenGLShader>(m_shader)->setUniform("u_projection", m_camera->getProjection());
			std::dynamic_pointer_cast<Sogas::OpenGLShader>(m_shader)->setUniform("u_view", m_camera->getView());
			std::dynamic_pointer_cast<Sogas::OpenGLShader>(m_shader)->setUniform("u_offset", x);
			std::dynamic_pointer_cast<Sogas::OpenGLShader>(m_shader)->setUniform("u_model", renderable->m_model);
			std::dynamic_pointer_cast<Sogas::OpenGLShader>(m_shader)->setUniform("u_texture", 0);

			renderable->m_prefab.get()->render();
		}

		
		m_framebuffer->unbind();
	}

	void onImguiRender()
	{
		ImGui::SetCurrentContext(Sogas::Application::getImguiContext());

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				//ImGui::MenuItem("Padding", NULL, &opt_padding);
				//ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false))
					Sogas::Application::getInstance()->close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
		u32 textureId = m_framebuffer->getColorAttachment();

		ImGui::Begin("Stats");
		ImGui::Text("Frame Count: %i", ImGui::GetFrameCount());
		ImGui::Text("Delta time: %f ms", io.DeltaTime);
		ImGui::Text("Framerate %.2f fps", io.Framerate);
		ImGui::Image((ImTextureID)textureId, ImVec2(1280.0f, 720.0f));
		ImGui::End();


		ImGui::End();
	}

	void onEvent(Sogas::Event& event) override
	{
		Sogas::EventDispatcher dispatcher(event);
		dispatcher.dispatch<Sogas::KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::onKeyPressed));
		dispatcher.dispatch<Sogas::MouseButtonPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseButtonPressed));
		dispatcher.dispatch<Sogas::MouseButtonReleasedEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseButtonReleased));
		dispatcher.dispatch<Sogas::MouseMoveEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseMoved));
	}

	bool onKeyPressed(Sogas::KeyPressedEvent& event)
	{
		return false;
	}

	bool onMouseButtonPressed(Sogas::MouseButtonPressedEvent& event) {
		return false;
	}

	bool onMouseButtonReleased(Sogas::MouseButtonReleasedEvent& event)
	{
		// Hide the cursor if using the camera
		if (event.getMouseButton() == SGS_MOUSE_BUTTON_MIDDLE)
		{
			m_camera->m_locked = !m_camera->m_locked;
			if (m_camera->m_locked)
				Sogas::Application::getInstance()->hideCursor();
			else
				Sogas::Application::getInstance()->showCursor();
			return false;
		}
	}

	bool onMouseMoved(Sogas::MouseMoveEvent& event)
	{
		glm::vec2 deltaMouse = mouse_pos - Sogas::Input::getMousePosition();
		if (m_camera->m_locked) {
			Sogas::Input::centerMouse();
			m_camera->rotate(deltaMouse.x, deltaMouse.y);
		}
		mouse_pos = Sogas::Input::getMousePosition();
		return false;
	}

private:
	Sogas::Camera* m_camera;
	std::shared_ptr<Sogas::Shader> m_shader;
	std::shared_ptr<Sogas::VertexArray> m_vertexArray;
	std::shared_ptr<Sogas::Texture2D> m_texture;
	std::shared_ptr<Sogas::Texture2D> m_defaultTexture;

	// TODO: erase them, provisional at the moment
	f32 x = 0.0f;
	f32 inc = 0.01f;
	glm::vec2 mouse_pos;
	std::unique_ptr<Sogas::Scene> m_scene;
	Sogas::Mesh* mesh;
	Sogas::Framebuffer* m_framebuffer;
};

class Sandbox : public Sogas::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Sogas::Application* Sogas::createApplication()
{
	return new Sandbox();
}