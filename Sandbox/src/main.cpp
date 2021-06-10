
#include <iostream>
#include "sogas.h"

#include "../external/glm/glm/gtc/matrix_transform.hpp"

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		// renderer example primitive usage
		m_vertexArray.reset(VertexArray::create());

		f32 quadVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		
		u32 quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		/*
		f32 positions[72] = {
			 1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,

			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			-1.0, -1.0,  1.0,
			 1.0, -1.0,  1.0,

			 1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			-1.0,  1.0, -1.0,
			 1.0,  1.0, -1.0,

			 1.0, -1.0,  1.0,
			-1.0, -1.0,  1.0,
			-1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,

			-1.0,  1.0,  1.0,
			-1.0, -1.0,  1.0,
			-1.0, -1.0, -1.0,
			-1.0,  1.0, -1.0,

			 1.0,  1.0,  1.0,
			 1.0, -1.0,  1.0,
			 1.0, -1.0, -1.0,
			 1.0,  1.0, -1.0
		};

		u32 indices[36] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};
		*/
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(quadVertices, sizeof(quadVertices)));

		VertexBufferLayout layout = { 
			{ShaderDataType::Float3, "a_position"},
			{ShaderDataType::Float2, "a_texCoord"}
		};

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(quadIndices, sizeof(quadIndices) / sizeof(u32)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

		m_camera = new Camera();
		m_camera->setPosition({ 0, 0, -2 });

		mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };
	}

	void onUpdate(f32 dt) override
	{
		Renderer::setClearColor(glm::vec4( 0.2 ));
		Renderer::clear();
		/*
		if (x < -1.0f || x > 1.0f) {
			inc *= -1;
		}

		x += inc;
		*/
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

		glm::vec2 mousePosition = Input::getMousePosition();
		glm::vec2 deltaMouse = mouse_pos - mousePosition;
		mouse_pos = mousePosition;
		if (m_camera->m_locked) {
			m_camera->rotate(deltaMouse.x, deltaMouse.y);
		}

		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 5.0f));
		model = glm::rotate(glm::mat4(model), glm::radians(0.0f), glm::vec3(0, 1, 0));
		
		m_shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_color", 1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_view", m_camera->getView());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_projection", m_camera->getProjection());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_offset", x);
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_model", model);

		//Renderer::draw(m_vertexArray);
		Renderer::drawIndexed(m_vertexArray);
	}

	void onEvent(Event& event) override
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::onKeyPressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseButtonPressed));
		dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FUNC(ExampleLayer::onMouseButtonReleased));
	}

	bool onKeyPressed(KeyPressedEvent& event)
	{
		/*
		if (event.getKeyCode() == SGS_KEY_A)
		{
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(-1.0, 0.0, 0.0));
		}
		if (event.getKeyCode() == SGS_KEY_D)
		{
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(1.0, 0.0, 0.0));
		}
		if (event.getKeyCode() == SGS_KEY_W)
		{
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(0.0, -1.0, 0.0));
		}
		if (event.getKeyCode() == SGS_KEY_S)
		{
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(0.0, 1.0, 0.0));
		}
		*/
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

private:
	Camera* m_camera;
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<VertexArray> m_vertexArray;

	// TODO: erase them, provisional at the moment
	float x = 0.0f;
	float inc = 0.01f;
	glm::vec2 mouse_pos;
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