#pragma once

#include "rendererAPI.h"
#include "renderCommand.h"

// TODO: add statistics to print in Sogas-Editor
struct statistics
{
	u32 drawCalls = 0;
};

namespace Sogas 
{
	class SGS Renderer
	{
	public:

		enum class API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};

		inline static API getAPI() { return s_API; }

		static void beginScene();
		static void endScene();
		static void submit(const std::shared_ptr<VertexArray>& vertexArray);

	private:
		static API s_API; // TODO: Hide the API to the rest of the application
		static RendererAPI* s_RendererAPI;
	};
}