#pragma once

#include "defines.h"
#include "renderer/VertexArray.h"
#include <string>
#include "scene/entity.h"

namespace Sogas 
{
	class SGS Mesh
	{
	public:

		Mesh();

		void load(const std::string& filename);
		inline std::string& getMeshName() { return m_filename; };

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<IndexBuffer> m_indexBuffer;

	private:
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::string m_filename;
	};
}

