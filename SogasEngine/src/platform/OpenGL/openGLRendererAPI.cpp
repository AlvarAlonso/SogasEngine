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

	void OpenGLRendererAPI::draw(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive)
	{
		for (std::shared_ptr<VertexBuffer> vertexBuffer : vertexArray->getVertexBuffers())
		{
			glDrawArrays((GLenum)primitive, 0, vertexBuffer.get()->getSize());
		}
	}

	void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive)
	{
		glDrawElements((GLenum)primitive, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::setBlendFunc(const BlendTypes sfactor, const BlendTypes dfactor)
	{
		GLenum sf = GL_ZERO, df = GL_ZERO;
		switch (sfactor)
		{
		case BlendTypes::ZERO:
			sf = GL_ZERO; break;
		case BlendTypes::ONE:
			sf = GL_ONE; break;
		case BlendTypes::SRC_COLOR:
			sf = GL_SRC_COLOR; break;
		case BlendTypes::ONE_MINUS_SRC_COLOR:
			sf = GL_ONE_MINUS_SRC_COLOR; break;
		case BlendTypes::DST_COLOR:
			sf = GL_DST_COLOR; break;
		case BlendTypes::ONE_MINUS_DST_COLOR:
			sf = GL_ONE_MINUS_DST_COLOR; break;
		case BlendTypes::SRC_ALPHA:
			sf = GL_SRC_ALPHA; break;
		case BlendTypes::ONE_MINUS_SRC_ALPHA:
			sf = GL_ONE_MINUS_SRC_ALPHA; break;
		case BlendTypes::DST_ALPHA:
			sf = GL_DST_ALPHA; break;
		case BlendTypes::ONE_MINUS_DST_ALPHA:
			sf = GL_ONE_MINUS_DST_ALPHA; break;
		case BlendTypes::CONSTANT_COLOR:
			sf = GL_ONE_MINUS_CONSTANT_COLOR; break;
		case BlendTypes::ONE_MINUS_CONSTANT_COLOR:
			sf = GL_ONE_MINUS_CONSTANT_COLOR; break;
		case BlendTypes::CONSTANT_ALPHA:
			sf = GL_CONSTANT_ALPHA; break;
		case BlendTypes::ONE_MINUS_CONSTANT_ALPHA:
			sf = GL_ONE_MINUS_CONSTANT_ALPHA; break;
		}

		switch (dfactor)
		{
		case BlendTypes::ZERO:
			df = GL_ZERO; break;
		case BlendTypes::ONE:
			df = GL_ONE; break;
		case BlendTypes::SRC_COLOR:
			df = GL_SRC_COLOR; break;
		case BlendTypes::ONE_MINUS_SRC_COLOR:
			df = GL_ONE_MINUS_SRC_COLOR; break;
		case BlendTypes::DST_COLOR:
			df = GL_DST_COLOR; break;
		case BlendTypes::ONE_MINUS_DST_COLOR:
			df = GL_ONE_MINUS_DST_COLOR; break;
		case BlendTypes::SRC_ALPHA:
			df = GL_SRC_ALPHA; break;
		case BlendTypes::ONE_MINUS_SRC_ALPHA:
			df = GL_ONE_MINUS_SRC_ALPHA; break;
		case BlendTypes::DST_ALPHA:
			df = GL_DST_ALPHA; break;
		case BlendTypes::ONE_MINUS_DST_ALPHA:
			df = GL_ONE_MINUS_DST_ALPHA; break;
		case BlendTypes::CONSTANT_COLOR:
			df = GL_ONE_MINUS_CONSTANT_COLOR; break;
		case BlendTypes::ONE_MINUS_CONSTANT_COLOR:
			df = GL_ONE_MINUS_CONSTANT_COLOR; break;
		case BlendTypes::CONSTANT_ALPHA:
			df = GL_CONSTANT_ALPHA; break;
		case BlendTypes::ONE_MINUS_CONSTANT_ALPHA:
			df = GL_ONE_MINUS_CONSTANT_ALPHA; break;
		}
		glBlendFunc(sf, df);
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