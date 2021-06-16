#pragma once

#include "core/assertions.h"

#include "../external/glm/glm/gtc/matrix_transform.hpp"

#include <memory>

namespace Sogas 
{
	class Texture2D;
	class Shader;

	struct MaterialProperties
	{
		bool isTransparent;

		// material property factors
		glm::vec4 color;
		f32 roughnessFactor;
		f32 metallicFactor;
		f32 tillingFactor;
		glm::vec3 emissiveFactor;

		// material textures
		std::shared_ptr<Texture2D> colorTexture;
		std::shared_ptr<Texture2D> emissiveTexture;
		std::shared_ptr<Texture2D> metallicRoughnessTexture;
		std::shared_ptr<Texture2D> occlusionTexture;
		std::shared_ptr<Texture2D> normalTexture;
	};

	class Material
	{
	public:
		Material() = default;
		Material(std::shared_ptr<Shader> shader);
		Material(std::shared_ptr<Shader> shader, const MaterialProperties& properties);
		~Material();

		void setMaterialShader(std::shared_ptr<Shader> shader);
		void setMaterialProperties(const MaterialProperties& properties);
	private:

		std::shared_ptr<Shader> m_shader;

		// material properties

		bool m_isTransparent;

		// material property factors
		glm::vec4 m_color;
		f32 m_roughnessFactor;
		f32 m_metallicFactor;
		f32 m_tillingFactor;
		glm::vec3 m_emissiveFactor;

		// material textures
		std::shared_ptr<Texture2D> m_colorTexture;
		std::shared_ptr<Texture2D> m_emissiveTexture;
		std::shared_ptr<Texture2D> m_metallicRoughnessTexture;
		std::shared_ptr<Texture2D> m_occlusionTexture;
		std::shared_ptr<Texture2D> m_normalTexture;
	};
}