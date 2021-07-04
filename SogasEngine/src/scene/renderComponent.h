#pragma once

#include "defines.h"
#include "entityComponent.h"

#include "renderer/resources/mesh.h"
#include "renderer/resources/material.h"

namespace Sogas
{
	class Material;

	class SGS RenderComponent : public EntityComponent
	{
	public:
		static const char* s_name;

		RenderComponent();
		virtual bool init() override;
		virtual const char* getName() const override { return s_name; }

		Mesh* getMesh(void) const { return m_pMesh; }
		void setMesh(const char* filename) { m_pMesh = new Mesh; m_pMesh->load(filename); }

		//inline void setMaterial(Material* material) { m_pMaterial = std::make_shared<Material>(material); }
		inline std::shared_ptr<Material>& getMaterial() { return m_pMaterial; }
		inline std::shared_ptr<Shader>& getShader() { return m_pMaterial->getShader(); }

	private:
		Mesh* m_pMesh = nullptr;
		std::shared_ptr<Material> m_pMaterial;
	};
}