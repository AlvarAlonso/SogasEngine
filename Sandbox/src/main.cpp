
#include <iostream>
#include "sogas.h"

#include "../external/glm/glm/gtc/matrix_transform.hpp"
#include "actors/mesh.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		// renderer example primitive usage
		m_vertexArray.reset(VertexArray::create());

		mesh = new Mesh();
		mesh->load("data/viking-room.obj");

		m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

		m_camera = new Camera();
		m_camera->setPosition({ 0, 0, -2 });

		mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };
	}

	void onUpdate(f32 dt) override
	{
		Renderer::setClearColor(glm::vec4( 0.2 ));
		Renderer::setDepthBuffer(true);
		Renderer::clear();
		
		if (x < -1.0f || x > 1.0f) {
			inc *= -1;
		}

		x += inc;
		
		// Should dt be stored as a class variable and used in the events through the dispatcher???
		if (Input::isKeyPressed(SGS_KEY_A)){
			m_camera->move(LEFT, dt);
		}
		else if (Input::isKeyPressed(SGS_KEY_D)){
			m_camera->move(RIGHT, dt);
		}

		if (Input::isKeyPressed(SGS_KEY_W)){
			m_camera->move(FORWARD, dt);
		}
		else if (Input::isKeyPressed(SGS_KEY_S)){
			m_camera->move(BACKWARD, dt);
		}

		glm::mat4 model = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 1, 0));
		
		m_shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_color", 1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_view", m_camera->getView());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_projection", m_camera->getProjection());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_offset", x);
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_model", model);

		Renderer::drawIndexed(mesh->m_vertexArray);
	}

	void onEvent(Event& event) override
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::onKeyPressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseButtonPressed));
		dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseButtonReleased));
		dispatcher.dispatch<MouseMoveEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseMoved));
	}

	bool onKeyPressed(KeyPressedEvent& event)
	{
		return false;
	}

	bool onMouseButtonPressed(MouseButtonPressedEvent& event) {
		return false;
	}

	bool onMouseButtonReleased(MouseButtonReleasedEvent& event)
	{
		// Hide the cursor if using the camera
		if (event.getMouseButton() == SGS_MOUSE_BUTTON_MIDDLE)
		{
			m_camera->m_locked = !m_camera->m_locked;
			if (m_camera->m_locked)
				Application::getInstance()->hideCursor();
			else
				Application::getInstance()->showCursor();
			return false;
		}
	}

	bool onMouseMoved(MouseMoveEvent& event)
	{
		glm::vec2 deltaMouse = mouse_pos - Input::getMousePosition();
		if (m_camera->m_locked) {
			Input::centerMouse();
			m_camera->rotate(deltaMouse.x, deltaMouse.y);
		}
		mouse_pos = Input::getMousePosition();
		return false;
	}

private:
	Camera* m_camera;
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<VertexArray> m_vertexArray;

	// TODO: erase them, provisional at the moment
	float x = 0.0f;
	float inc = 0.01f;
	glm::vec2 mouse_pos;
	Mesh* mesh;
};

class Sandbox : public Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Application* createApplication()
{
	return new Sandbox();
}