#pragma once

#include "vertexArray.h"
#include "scene/types.h"
#include "glm/glm.hpp"

namespace Sogas 
{
	class SGS RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};
	public:
		virtual void clear() = 0;

		virtual void setClearColor(const glm::vec4& colour) = 0;

		virtual void setDepthBuffer(const bool& setOn) = 0;

		virtual void draw(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive) = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive) = 0;

		virtual void setBlendFunc(const BlendTypes sfactor, const BlendTypes dfactor) = 0;

		virtual void enableBlend(bool blend) = 0;

		virtual void setLineWidth(const f32 width) = 0;

		inline static API getAPI() { return s_API; }
	private:
		static API s_API;
	};
}