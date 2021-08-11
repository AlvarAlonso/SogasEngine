#include "sgspch.h"

#include "material.h"
#include "renderer/shader.h"
#include "renderer/resources/texture.h"
#include "scene/serializerUtils.h"
#include "core/logger.h"

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
		Sogas::to_json(emissive, m_materialProperties.emissiveFactor);
		
		jsonMaterial["Shader"]			= getShader()->getName();
		jsonMaterial["Color"]			= color;
		jsonMaterial["RoughnessFactor"] = m_materialProperties.roughnessFactor;
		jsonMaterial["MetallicFactor"]	= m_materialProperties.metallicFactor;
		jsonMaterial["TillingFactor"]	= m_materialProperties.tillingFactor;
		jsonMaterial["EmissiveFactor"]	= emissive;

		m_materialProperties.colorTexture ? jsonMaterial["ColorTexture"] = m_materialProperties.colorTexture->getName() : jsonMaterial["ColorTexture"] = nullptr;
		m_materialProperties.normalTexture ? jsonMaterial["NormalTexture"] = m_materialProperties.normalTexture->getName() : jsonMaterial["NormalTexture"] = nullptr;
		m_materialProperties.emissiveTexture ? jsonMaterial["EmissiveTexture"] = m_materialProperties.emissiveTexture->getName() : jsonMaterial["EmissiveTexture"] = nullptr;
		m_materialProperties.metallicRoughnessTexture ? jsonMaterial["MetallicRoughnessTexture"] = m_materialProperties.metallicRoughnessTexture->getName() : jsonMaterial["MetallicRoughnessTexture"] = nullptr;
		m_materialProperties.occlusionTexture ? jsonMaterial["OcclusionTexture"] = m_materialProperties.occlusionTexture->getName() : jsonMaterial["OcclusionTexture"] = nullptr;
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
			setMaterialShader(Shader::GET("basic.shader"));
		}

		// Get the material properties
		if (jsonMaterial.contains("Color") && !jsonMaterial["Color"].is_null()) {
			Sogas::from_json(jsonMaterial["Color"], getMaterialProperties().color);
		}

		if (jsonMaterial.contains("EmissiveFactor") && !jsonMaterial["EmissiveFactor"].is_null()) {
			Sogas::from_json(jsonMaterial["EmissiveFactor"], getMaterialProperties().emissiveFactor);
		}

		if (jsonMaterial.contains("MetallicFactor") && !jsonMaterial["MetallicFactor"].is_null()) {
			getMaterialProperties().metallicFactor = jsonMaterial["MetallicFactor"].get<f32>();
		}

		if (jsonMaterial.contains("RoughnessFactor") && !jsonMaterial["RoughnessFactor"].is_null()) {
			getMaterialProperties().roughnessFactor = jsonMaterial["RoughnessFactor"].get<f32>();
		}

		if (jsonMaterial.contains("TillingFactor") && !jsonMaterial["TillingFactor"].is_null()) {
			getMaterialProperties().tillingFactor = jsonMaterial["TillingFactor"].get<f32>();
		}


		// Textures
		if (jsonMaterial.contains("ColorTexture") && jsonMaterial["ColorTexture"].is_string())
		{
			auto& properties = getMaterialProperties();
			properties.colorTexture = Texture2D::GET(jsonMaterial["ColorTexture"].get<std::string>());
			setMaterialProperties(properties);
		}

		if (jsonMaterial.contains("NormalTexture") && jsonMaterial["NormalTexture"].is_string())
		{
			auto& properties = getMaterialProperties();
			properties.colorTexture = Texture2D::GET(jsonMaterial["NormalTexture"].get<std::string>());
			setMaterialProperties(properties);
		}

		if (jsonMaterial.contains("EmissiveTexture") && jsonMaterial["EmissiveTexture"].is_string())
		{
			auto& properties = getMaterialProperties();
			properties.colorTexture = Texture2D::GET(jsonMaterial["EmissiveTexture"].get<std::string>());
			setMaterialProperties(properties);
		}

		if (jsonMaterial.contains("MetallicRoughnessTexture") && jsonMaterial["MetallicRoughnessTexture"].is_string())
		{
			auto& properties = getMaterialProperties();
			properties.colorTexture = Texture2D::GET(jsonMaterial["MetallicRoughnessTexture"].get<std::string>());
			setMaterialProperties(properties);
		}

		if (jsonMaterial.contains("OcclusionTexture") && jsonMaterial["OcclusionTexture"].is_string())
		{
			auto& properties = getMaterialProperties();
			properties.colorTexture = Texture2D::GET(jsonMaterial["OcclusionTexture"].get<std::string>());
			setMaterialProperties(properties);
		}
	}
}