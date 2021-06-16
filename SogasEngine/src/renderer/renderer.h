#pragma once

#include "rendererAPI.h"

namespace Sogas 
{
	class SGS Renderer
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};
	public:
		inline static API getAPI() { return s_API; }

		inline static void clear()
		{
			s_RendererAPI->clear();
		}

		inline static void setClearColor(const glm::vec4& color)
		{
			s_RendererAPI->setClearColor(color);
		}

		inline static void setDepthBuffer(const bool& setOn)
		{
			s_RendererAPI->setDepthBuffer(setOn);
		}

		inline static void draw(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->draw(vertexArray);
		}

		inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->drawIndexed(vertexArray);
		}

	private:
		static API s_API; // TODO: Hide the API to the rest of the application
		static RendererAPI* s_RendererAPI;
	};
}