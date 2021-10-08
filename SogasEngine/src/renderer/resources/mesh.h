#pragma once

#include "defines.h"
#include "renderer/VertexArray.h"
#include "renderer/resources/material.h"
#include <string>

namespace Sogas 
{
	class CameraComponent;

	class SGS Mesh
	{
	public:

		static std::unordered_map<std::string, std::shared_ptr<Mesh>> s_loadedMeshes;

		Mesh();

		inline std::string& getMeshName() { return m_filename; };
		inline std::shared_ptr<Material>& getMaterial() { return m_pMaterial; }
		inline void setMaterial(std::shared_ptr<Material>& material) { m_pMaterial = material; }

		static std::shared_ptr<Mesh> GET(const std::string& filename);
		void createGrid(f32 dist = 5.0f, i32 n_lines = 20);
		void createCameraMesh(std::weak_ptr<CameraComponent>& cameraComponent);
		void getFrustrumFromCamera(std::weak_ptr<CameraComponent>& cameraComponent);

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<IndexBuffer> m_indexBuffer;

	private:
		void load(const std::string& filename);

		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::string m_filename;
		std::shared_ptr<Material> m_pMaterial;
	};
}

