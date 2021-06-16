#include "material.h"

namespace Sogas 
{
	Material::Material(std::shared_ptr<Shader> shader)
		: m_shader(shader), m_isTransparent(false), m_color(glm::vec4(1)), m_roughnessFactor(0.0f),
		m_metallicFactor(0.0f), m_tillingFactor(1.0f), m_emissiveFactor(glm::vec3(0)),
		m_colorTexture(nullptr), m_emissiveTexture(nullptr), m_metallicRoughnessTexture(nullptr),
		m_occlusionTexture(nullptr), m_normalTexture(nullptr)
	{
	}

	Material::Material(std::shared_ptr<Shader> shader, const MaterialProperties& properties)
		: m_shader(shader), m_isTransparent(properties.isTransparent), m_color(properties.color),
		m_roughnessFactor(properties.roughnessFactor), m_metallicFactor(properties.metallicFactor),
		m_tillingFactor(properties.tillingFactor), m_emissiveFactor(properties.emissiveFactor),
		m_colorTexture(properties.colorTexture), m_emissiveTexture(properties.emissiveTexture),
		m_metallicRoughnessTexture(properties.metallicRoughnessTexture), m_occlusionTexture(properties.occlusionTexture),
		m_normalTexture(properties.normalTexture)
	{
	}

	Material::~Material()
	{
	}

	void Material::setMaterialShader(std::shared_ptr<Shader> shader)
	{
		m_shader = shader;
	}

	void Material::setMaterialProperties(const MaterialProperties& properties)
	{
		m_isTransparent = properties.isTransparent;

		m_color = properties.color;
		m_roughnessFactor = properties.roughnessFactor;
		m_metallicFactor = properties.metallicFactor;
		m_tillingFactor = properties.tillingFactor;
		m_emissiveFactor = properties.emissiveFactor;

		m_colorTexture = properties.colorTexture;
		m_emissiveTexture = properties.emissiveTexture;
		m_metallicRoughnessTexture = properties.metallicRoughnessTexture;
		m_occlusionTexture = properties.occlusionTexture;
		m_normalTexture = properties.normalTexture;
	}
}