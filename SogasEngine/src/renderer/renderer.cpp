#include "sgspch.h"

#include "renderer.h"
#include "platform/OpenGL/openGLRendererAPI.h"

namespace Sogas 
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	RendererAPI* Renderer::s_RendererAPI = new OpenGLRendererAPI;

	void Renderer::beginScene()
	{
		RenderCommand::setClearColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		RenderCommand::clear();
		RenderCommand::setDepthBuffer(true);
	}

	void Renderer::endScene()
	{

	}

	void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}