#include "imguiLayer.h"

#include "core/application.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

ImGuiLayer::ImGuiLayer()
	: Layer("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::onAttach()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// TODO: maybe own keys??
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	ImGui_ImplOpenGL3_Init();
}

void ImGuiLayer::onDetach()
{
}

void ImGuiLayer::onUpdate()
{
	ImGuiIO& io = ImGui::GetIO();
	Application* app = Application::getInstance();
	io.DisplaySize = ImVec2(app->getWindow().getWidth(), app->getWindow().getHeight());

	f32 time = (f32)glfwGetTime();
	io.DeltaTime = m_time < 0.0 ? (time - m_time) : (1.0f / 60.0f);
	m_time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::onEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNC(ImGuiLayer::onMouseButtonReleasedEvent));
	dispatcher.dispatch<MouseMoveEvent>(BIND_EVENT_FUNC(ImGuiLayer::onMouseMovedEvent));
	dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(ImGuiLayer::onMouseScrolledEvent));
	dispatcher.dispatch<windowResizeEvent>(BIND_EVENT_FUNC(ImGuiLayer::onWindowResized));
	dispatcher.dispatch<KeyTypedEvent>(BIND_EVENT_FUNC(ImGuiLayer::onKeyTypedEvent));
	dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT_FUNC(ImGuiLayer::onKeyReleased));
	dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(ImGuiLayer::onKeyPressed));
}

bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.getMouseButton()] = true;

	return false;
}

bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.getMouseButton()] = false;
	return false;
}

bool ImGuiLayer::onMouseMovedEvent(MouseMoveEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e.getX(), e.getY());
	return false;
}

bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += e.getOffsetX();
	io.MouseWheel += e.getOffsetY();
	return false;
}

bool ImGuiLayer::onWindowResized(windowResizeEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	glViewport(0, 0, e.getWidth(), e.getHeight());

	return false;
}

bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	i32 keycode = e.getKeyCode();
	if (keycode > 0 && keycode < 0x10000) {
		io.AddInputCharacter((u32)keycode);
	}
	return false;
}

bool ImGuiLayer::onKeyReleased(KeyReleasedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[e.getKeyCode()] = false;
	return false;
}

bool ImGuiLayer::onKeyPressed(KeyPressedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();

	io.KeyCtrl	= io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt	= io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];

	io.KeysDown[e.getKeyCode()] = true;
	return false;
}


