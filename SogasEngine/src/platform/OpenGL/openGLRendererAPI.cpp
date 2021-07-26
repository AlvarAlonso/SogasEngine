#include "sgspch.h"

#include "openGLRendererAPI.h"
#include "renderer/shader.h"

#include "GL/glew.h"
#include <glm/ext/matrix_transform.hpp>

namespace Sogas 
{
	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLRendererAPI::setDepthBuffer(const bool& setOn)
	{
		setOn ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::draw(const std::shared_ptr<VertexArray>& vertexArray)
	{
		for (std::shared_ptr<VertexBuffer> vertexBuffer : vertexArray->getVertexBuffers())
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.get()->getSize());
		}
	}

	void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::drawLines(const std::shared_ptr<VertexArray>& vertexArray)
	{
		for (std::shared_ptr<VertexBuffer> vertexBuffer : vertexArray->getVertexBuffers())
		{
			//glPointSize(10);
			glDrawArrays(GL_LINES, 0, vertexBuffer.get()->getSize());
		}
	}

	void OpenGLRendererAPI::setBlendFunc(bool blend)
	{
		if (blend)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void OpenGLRendererAPI::enableBlend(bool blend)
	{
		blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	}

	void OpenGLRendererAPI::setLineWidth(const f32 width)
	{
		glLineWidth(width);
	}
}