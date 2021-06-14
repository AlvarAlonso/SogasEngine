#include "imguiLayer.h"

#include "core/application.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

ImGuiLayer::ImGuiLayer()
	: Layer("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::onAttach()
{
	IMGUI_CHECKVERSION();
	Application::s_imguiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	Application* app = Application::getInstance();

	GLFWwindow* window = static_cast<GLFWwindow*>(app->getWindow().getNativeWindow());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void ImGuiLayer::onDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::onImguiRender()
{
 //   static bool dockspaceOpen = true;
 //   static bool opt_fullscreen = true;
 //   static bool opt_padding = false;
 //   static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

 //   // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
 //   // because it would be confusing to have two docking targets within each others.
 //   ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
 //   if (opt_fullscreen)
 //   {
 //       const ImGuiViewport* viewport = ImGui::GetMainViewport();
 //       ImGui::SetNextWindowPos(viewport->WorkPos);
 //       ImGui::SetNextWindowSize(viewport->WorkSize);
 //       ImGui::SetNextWindowViewport(viewport->ID);
 //       ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
 //       ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
 //       window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
 //       window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
 //   }
 //   else
 //   {
 //       dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
 //   }

 //   // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
 //   // and handle the pass-thru hole, so we ask Begin() to not render a background.
 //   if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
 //       window_flags |= ImGuiWindowFlags_NoBackground;

 //   // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
 //   // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
 //   // all active windows docked into it will lose their parent and become undocked.
 //   // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
 //   // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
 //   if (!opt_padding)
 //       ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
 //   ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
 //   if (!opt_padding)
 //       ImGui::PopStyleVar();

 //   if (opt_fullscreen)
 //       ImGui::PopStyleVar(2);

 //   // Submit the DockSpace
 //   ImGuiIO& io = ImGui::GetIO();
 //   if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
 //   {
 //       ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
 //       ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
 //   }

 //   if (ImGui::BeginMenuBar())
 //   {
 //       if (ImGui::BeginMenu("Options"))
 //       {
 //           // Disabling fullscreen would allow the window to be moved to the front of other windows,
 //           // which we can't undo at the moment without finer window depth/z control.
 //           //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
 //           //ImGui::MenuItem("Padding", NULL, &opt_padding);
 //           //ImGui::Separator();

 //           if (ImGui::MenuItem("Close", NULL, false))
 //               Application::getInstance()->close();
 //           ImGui::EndMenu();
 //       }

 //       ImGui::EndMenuBar();
 //   }

	////static bool show = true;
	////ImGui::ShowDemoWindow(&show);
 //   ImGui::Begin("Stats");
 //   ImGui::Text("Frame Count: %i", ImGui::GetFrameCount());
 //   ImGui::Text("Delta time: %f ms", io.DeltaTime);
 //   ImGui::Text("Framerate %.2f fps", io.Framerate);
 //   ImGui::End();

 //   //ImGui::Image();

 //   ImGui::End();
}

void ImGuiLayer::begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::end()
{
	ImGuiIO& io = ImGui::GetIO();
	Application* app = Application::getInstance();
	io.DisplaySize = ImVec2((f32)app->getWindow().getWidth(), (f32)app->getWindow().getHeight());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}