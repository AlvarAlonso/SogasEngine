#include "vertexArray.h"
#include "renderer.h"
#include "platform/OpenGL/openGLVertexArray.h"

namespace Sogas 
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
		case Renderer::API::None:  SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return new OpenGLVertexArray();
		}

		SGSASSERT_MSG(false, "Unknown renderer API!");
		return nullptr;
	}
}