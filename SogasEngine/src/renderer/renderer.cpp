#include "renderer.h"
#include "platform/OpenGL/openGLRendererAPI.h"

Renderer::API Renderer::s_API = Renderer::API::OpenGL;
RendererAPI* Renderer::s_RendererAPI = new OpenGLRendererAPI;