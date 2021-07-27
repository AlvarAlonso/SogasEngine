#include "sgspch.h"

#include "material.h"
#include "renderer/shader.h"
#include "renderer/resources/texture.h"
#include "scene/serializerUtils.h"

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

	void Material::to_json(json& jsonMaterial)
	{
		json color, emissive;
		Sogas::to_json(color, m_materialProperties.color);
		Sogas::to_json(color, m_materialProperties.emissiveFactor);

		//std::string colorTexture;
		//if (m_materialProperties.colorTexture)
		//	colorTexture = m_materialProperties.colorTexture->getName();

		jsonMaterial = {
			{"Shader", getShader()->getName()},
			{"Color", color},
			{"RoughnessFactor", m_materialProperties.roughnessFactor},
			{"MetallicFactor", m_materialProperties.metallicFactor},
			{"TillingFactor", m_materialProperties.tillingFactor},
			{"EmissiveFactor", emissive}
			//{"colorTexture", !colorTexture.empty() ? colorTexture : "null"}
			//{"ColorTexture", m_materialProperties.colorTexture ? m_materialProperties.colorTexture->getName() : nullptr}
			//{"NormalTexture", m_materialProperties.normalTexture ? m_materialProperties.normalTexture->getName() : nullptr},
			//{"EmissiveTexture", m_materialProperties.emissiveTexture ? m_materialProperties.emissiveTexture->getName() : nullptr},
			//{"MetallicRoughnessTexture", m_materialProperties.metallicRoughnessTexture ? m_materialProperties.metallicRoughnessTexture->getName() : nullptr},
			//{"OcclusionTexture", m_materialProperties.occlusionTexture ? m_materialProperties.occlusionTexture->getName() : nullptr}
		};
	}

	void Material::from_json(const json& jsonMaterial)
	{
		// Check if a valid shader is given
		if (jsonMaterial.contains("Shader") &&
			!jsonMaterial["Shader"].is_null() &&
			jsonMaterial["Shader"].is_string())
		{
			setMaterialShader(Shader::GET(jsonMaterial["Shader"].get<std::string>()));
		}
		else
		{
			setMaterialShader(Shader::GET("../SogasEngine/shaders/basic.shader"));
		}

		// Get the material properties
		if (jsonMaterial.contains("ColorTexture") && jsonMaterial["ColorTexture"].is_string())
		{
			auto& properties = getMaterialProperties();
			properties.colorTexture = Texture2D::create(jsonMaterial["ColorTexture"].get<std::string>());
			setMaterialProperties(properties);
		}
	}
}