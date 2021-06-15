#include "EditorLayer.h"

#include "renderer/renderer.h"
#include "core/input.h"
#include "core/keyCodes.h"
#include "core/logger.h"
#include "platform/openGL/openGLShader.h"
#include "../external/glm/glm/gtc/matrix_transform.hpp"

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
{
}

void EditorLayer::onAttach()
{
	FramebufferSpecs specs;
	specs.width = 1280; // Application::getInstance()->getWindow().getWidth();
	specs.height = 720; // Application::getInstance()->getWindow().getHeight();

	m_framebuffer = Framebuffer::create(specs);

	// load texture
	m_texture = Texture2D::create("../Assets/texture.png");

	mesh = new Mesh();
	mesh->load("../Assets/viking-room.obj");

	m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

	m_camera.reset(new Camera());
	m_camera->setPosition({ 0, 15, -50 });

	m_cameraController.reset(new CameraController(m_camera));

	mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };

}

void EditorLayer::onDetach()
{
}

void EditorLayer::onUpdate(f32 dt)
{

	if (m_viewportFocused)
	{
		m_cameraController->onUpdate(dt);
	}

	glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 5.0f));
	model = glm::rotate(glm::mat4(model), glm::radians(0.0f), glm::vec3(1, 0, 0));

	m_framebuffer->bind();

	Renderer::setClearColor(glm::vec4(0.2));
	Renderer::setDepthBuffer(true);
	Renderer::clear();

	m_shader->bind();
	m_texture->bind();
	std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_projection", m_camera->getProjection());
	std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_view", m_camera->getView());
	std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_model", model);
	std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_texture", 0);

	Renderer::drawIndexed(mesh->m_vertexArray);

	m_framebuffer->unbind();
}

void EditorLayer::onImguiRender()
{
	ImGui::SetCurrentContext(Application::getImguiContext());

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
			if (ImGui::MenuItem("Close", NULL, false))
				Application::getInstance()->close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	// Stats panel
	ImGui::Begin("Stats");
	ImGui::Text("Frame Count: %i", ImGui::GetFrameCount());
	ImGui::Text("Delta time: %f ms", io.DeltaTime);
	ImGui::Text("Framerate %.2f fps", io.Framerate);
	ImGui::End();

	// Viewport panel
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0, 0.0));
	ImGui::Begin("Viewport");

	m_viewportFocused = ImGui::IsWindowFocused();
	m_viewportHovered = ImGui::IsWindowHovered();
	Application::getInstance()->getImGuiLayer()->blockEvents(!m_viewportFocused || !m_viewportHovered);

	// TODO: Should also resize the aspect ratio of the camera
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if (m_viewportSize != *((glm::vec2*)&viewportPanelSize))
	{
		m_framebuffer->resize((u32)viewportPanelSize.x, (u32)viewportPanelSize.y);
		m_viewportSize = {viewportPanelSize.x, viewportPanelSize.y};
		m_cameraController.get()->setViewportSize(m_viewportSize.x, m_viewportSize.y);
	}
	u32 textureId = m_framebuffer->getColorAttachment();
	ImGui::Image((ImTextureID)textureId, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}

void EditorLayer::onEvent(Event& event)
{
	if (m_viewportFocused) {
		m_cameraController.get()->onEvent(event);
	}
}
