#include "buffer.h"

#include "renderer.h"
#include "platform/OpenGL/openGLBuffer.h"

#include "core/assertions.h"

VertexBuffer* VertexBuffer::create(f32* vertices, u32 size)
{
	switch(Renderer::getAPI())
	{
		case Renderer::API::None: SGSASSERT_MSG(false, "No graphics API selected");
		case Renderer::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
	}

SGSASSERT_MSG(false, "Unknown renderer API!");
	return nullptr;
}

IndexBuffer* IndexBuffer::create(u32* indices, u32 count)
{
	switch(Renderer::getAPI())
	{
	case Renderer::API::None:  SGSASSERT_MSG(false, "No graphics API selected");
	case Renderer::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
	}

	SGSASSERT_MSG(false, "Unknown renderer API!");
	return nullptr;
}