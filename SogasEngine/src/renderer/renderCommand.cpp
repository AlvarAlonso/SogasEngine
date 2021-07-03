#include "sgspch.h"
#include "renderCommand.h"
#include "platform/OpenGL/openGLRendererAPI.h"

namespace Sogas
{
	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}