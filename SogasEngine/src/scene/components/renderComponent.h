#pragma once

#include "defines.h"
#include "entityComponent.h"
#include "scene/types.h"
#include "renderer/resources/mesh.h"
#include "renderer/resources/material.h"

namespace Sogas
{
	class SGS RenderComponent : public EntityComponent
	{
	public:
		static const char* s_name;

		RenderComponent();
		virtual bool init() override;
		virtual const char* getName() const override { return s_name; }
		virtual void to_json(json& j) override;
		virtual void from_json(const json& j) override;
		virtual LuaPlus::LuaObject toLuaObject(LuaPlus::LuaObject self) const override { return LuaPlus::LuaObject(); };
		virtual void fromLuaObject(LuaPlus::LuaObject) override {};

		std::shared_ptr<Mesh> getMesh(void) const { return m_pMesh; }
		void setMesh(const char* filename) { m_pMesh = Mesh::GET(filename); }
		void setMesh(std::shared_ptr<Mesh>& mesh) { m_pMesh = mesh; }
		void setMaterial(std::shared_ptr<Material> material) { m_pMesh->setMaterial(material); }
		void setPrimitive(const Primitive primitive) { m_primitive = primitive; }

		//inline void setMaterial(Material* material) { m_pMaterial = std::make_shared<Material>(material); }
		inline std::shared_ptr<Material>& getMaterial() { return m_pMesh->getMaterial(); }
		inline std::shared_ptr<Shader>& getShader() { return m_pMesh->getMaterial()->getShader(); }
		inline const Primitive getPrimitive() { return m_primitive; }

	private:
		std::shared_ptr<Mesh> m_pMesh;
		//std::shared_ptr<Material> m_pMaterial;
		Primitive m_primitive = Primitive::TRIANGLES;
	};
}