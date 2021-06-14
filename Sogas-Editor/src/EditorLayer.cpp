#include "EditorLayer.h"

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
{
}

void EditorLayer::onAttach()
{
	//FramebufferSpecs specs;
	//specs.width = 1280; // Application::getInstance()->getWindow().getWidth();
	//specs.height = 720; // Application::getInstance()->getWindow().getHeight();

	//m_framebuffer = Framebuffer::create(specs);

	//// load texture
	//m_texture = Texture2D::create("../Assets/texture.png");

	//mesh = new Mesh();
	//mesh->load("data/viking-room.obj");

	//m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

	//m_camera = new Camera();
	//m_camera->setPosition({ 0, 15, -50 });

	//mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };

}

void EditorLayer::onDetach()
{
}

void EditorLayer::onUpdate(f32 dt)
{
	//m_framebuffer->bind();

	//Renderer::setClearColor(glm::vec4(0.2));
	//Renderer::setDepthBuffer(true);
	//Renderer::clear();

	//// Should dt be stored as a class variable and used in the events through the dispatcher???
	//if (Input::isKeyPressed(SGS_KEY_A)) {
	//	m_camera->move(LEFT, dt);
	//}
	//else if (Input::isKeyPressed(SGS_KEY_D)) {
	//	m_camera->move(RIGHT, dt);
	//}

	//if (Input::isKeyPressed(SGS_KEY_W)) {
	//	m_camera->move(FORWARD, dt);
	//}
	//else if (Input::isKeyPressed(SGS_KEY_S)) {
	//	m_camera->move(BACKWARD, dt);
	//}

	//glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 5.0f));
	//model = glm::rotate(glm::mat4(model), glm::radians(0.0f), glm::vec3(1, 0, 0));
	////model = glm::scale(glm::mat4(model), glm::vec3(1.0f, 2.0f, 1.0f));

	////m_shader->bind();
	//m_texture->bind();
	//std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_projection", m_camera->getProjection());
	//std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_view", m_camera->getView());
	//std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_model", model);
	//std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_texture", 0);

	//Renderer::drawIndexed(mesh->m_vertexArray);

	//m_framebuffer->unbind();
}

void EditorLayer::onImguiRender()
{
	//ImGui::SetCurrentContext(Application::getImguiContext());

	//static bool dockspaceOpen = true;
	//static bool opt_fullscreen = true;
	//static bool opt_padding = false;
	//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	//// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	//// because it would be confusing to have two docking targets within each others.
	//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	//if (opt_fullscreen)
	//{
	//	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	//	ImGui::SetNextWindowPos(viewport->WorkPos);
	//	ImGui::SetNextWindowSize(viewport->WorkSize);
	//	ImGui::SetNextWindowViewport(viewport->ID);
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	//	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	//	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	//}
	//else
	//{
	//	dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	//}

	//// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	//// and handle the pass-thru hole, so we ask Begin() to not render a background.
	//if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	//	window_flags |= ImGuiWindowFlags_NoBackground;

	//// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	//// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	//// all active windows docked into it will lose their parent and become undocked.
	//// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	//// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	//if (!opt_padding)
	//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	//if (!opt_padding)
	//	ImGui::PopStyleVar();

	//if (opt_fullscreen)
	//	ImGui::PopStyleVar(2);

	//// Submit the DockSpace
	//ImGuiIO& io = ImGui::GetIO();
	//if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	//{
	//	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	//}

	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("Options"))
	//	{
	//		// Disabling fullscreen would allow the window to be moved to the front of other windows,
	//		// which we can't undo at the moment without finer window depth/z control.
	//		//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
	//		//ImGui::MenuItem("Padding", NULL, &opt_padding);
	//		//ImGui::Separator();

	//		if (ImGui::MenuItem("Close", NULL, false))
	//			Application::getInstance()->close();
	//		ImGui::EndMenu();
	//	}

	//	ImGui::EndMenuBar();
	//}

	////static bool show = true;
	////ImGui::ShowDemoWindow(&show);
	//u32 textureId = m_framebuffer->getColorAttachment();

	//ImGui::Begin("Stats");
	//ImGui::Text("Frame Count: %i", ImGui::GetFrameCount());
	//ImGui::Text("Delta time: %f ms", io.DeltaTime);
	//ImGui::Text("Framerate %.2f fps", io.Framerate);
	//ImGui::Image((ImTextureID)textureId, ImVec2(1280.0f, 720.0f));
	//ImGui::End();

	//ImGui::End();
}

void EditorLayer::onEvent(Event& event)
{
}
