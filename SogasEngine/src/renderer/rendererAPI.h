#pragma once

#include "vertexArray.h"
#include "../external/glm/glm/glm.hpp"

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

	virtual void draw(const std::shared_ptr<VertexArray>& vertexArray) = 0;

	virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

	inline static API getAPI() { return s_API; }
private:
	static API s_API;
};