#include "sgspch.h"

#include "material.h"

namespace Sogas 
{
	Material::Material(std::shared_ptr<Shader> shader)
		: m_pShader(shader)
	{
		m_materialProperties = MaterialProperties();
		m_name = "Default";
	}

	Material::Material(std::shared_ptr<Shader> shader, const MaterialProperties& properties)
		: m_pShader(shader), m_materialProperties(properties)
	{
		m_name = "Default";
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