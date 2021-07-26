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

		static std::unordered_map<std::string, std::shared_ptr<Mesh>> s_loadedMeshes;

		Mesh();

		inline std::string& getMeshName() { return m_filename; };

		static std::shared_ptr<Mesh> GET(const std::string& filename);
		void createGrid(f32 dist);

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<IndexBuffer> m_indexBuffer;

	private:
		void load(const std::string& filename);

		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::string m_filename;
	};
}

