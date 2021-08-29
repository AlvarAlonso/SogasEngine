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

		virtual bool init() = 0;
		virtual void shutdown() = 0;

		virtual void clear() = 0;
		virtual void setClearColor(const glm::vec4& colour) = 0;
		virtual void enableDepthBuffer(const bool& depth) = 0;
		virtual void setDepthFunc(const DepthTypes factor) = 0;
		virtual void enableDepthMask(const bool& mask) = 0;
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