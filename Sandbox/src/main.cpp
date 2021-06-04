
#include <iostream>
#include "sogas.h"

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		// renderer example primitive usage
		m_vertexArray.reset(VertexArray::create());

		f32 positions[9] = {
			-0.5f, -0.5f, 1.0f,
			 0.0f,  0.5f, 1.0f,
			 0.5f, -0.5f, 1.0f
		};

		u32 indices[3] = {
			0, 1, 2
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(positions, sizeof(positions)));

		VertexBufferLayout layout = { {ShaderDataType::Float3, "a_position"} };

		vertexBuffer->setLayout(layout);
		m_vertexArray->addVertexBuffer(vertexBuffer);

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(u32)));
		m_vertexArray->setIndexBuffer(indexBuffer);

		m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

		m_camera = new Camera();
	}

	void onUpdate(f32 dt) override
	{
		Renderer::setClearColor(glm::vec4( 0.2 ));
		Renderer::clear();

		if (x < -1.0f || x > 1.0f) {
			inc *= -1;
		}

		x += inc;

		// Should dt be stored as a class variable and used in the events through the dispatcher???
		if (Input::isKeyPressed(SGS_KEY_A)){
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(-1.0, 0.0, 0.0) * dt);
		}
		else if (Input::isKeyPressed(SGS_KEY_D)){
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(1.0, 0.0, 0.0) * dt);
		}

		if (Input::isKeyPressed(SGS_KEY_W)){
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(0.0, -1.0, 0.0) * dt);
		}
		else if (Input::isKeyPressed(SGS_KEY_S)){
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(0.0, 1.0, 0.0) * dt);
		}

		m_shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_color", 1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("view", m_camera->getView());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("projection", m_camera->getProjection());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("offset", x);

		//Renderer::draw(m_vertexArray);
		Renderer::drawIndexed(m_vertexArray);
	}

	void onEvent(Event& event) override
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(ExampleLayer::onKeyPressed));
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

private:
	Camera* m_camera;
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<VertexArray> m_vertexArray;

	float x = 0.0f;
	float inc = 0.01f;
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