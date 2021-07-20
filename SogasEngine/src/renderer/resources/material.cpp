#include "sgspch.h"

#include "material.h"

namespace Sogas 
{
	Material::Material(std::shared_ptr<Shader> shader)
		: m_pShader(shader)
	{
		m_materialProperties = MaterialProperties();
	}

	Material::Material(std::shared_ptr<Shader> shader, const MaterialProperties& properties)
	{
	}

	Material::~Material()
	{
	}

	void Material::setMaterialShader(std::shared_ptr<Shader> shader)
	{
		m_pShader = shader;
	}

	void Material::setMaterialProperties(const MaterialProperties& properties)
	{
		m_materialProperties = properties;
	}
}