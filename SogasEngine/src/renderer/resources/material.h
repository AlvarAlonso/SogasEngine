#pragma once

#include "core/assertions.h"

#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace Sogas 
{
	class Texture2D;
	class Shader;

	struct MaterialProperties
	{
		bool isTransparent{ false };

		// material property factors
		glm::vec4	color{ 1.0 };
		f32			roughnessFactor{ 0.0f };
		f32			metallicFactor{ 0.0f };
		f32			tillingFactor{ 1.0f };
		glm::vec3	emissiveFactor{ 0.0f };

		// material textures
		std::shared_ptr<Texture2D> colorTexture;
		std::shared_ptr<Texture2D> emissiveTexture;
		std::shared_ptr<Texture2D> metallicRoughnessTexture;
		std::shared_ptr<Texture2D> occlusionTexture;
		std::shared_ptr<Texture2D> normalTexture;
	};

	class SGS Material
	{
	public:
		Material() = default;
		Material(std::shared_ptr<Shader> shader);
		Material(std::shared_ptr<Shader> shader, const MaterialProperties& properties);
		~Material();

		void setMaterialShader(std::shared_ptr<Shader> shader);
		void setMaterialProperties(const MaterialProperties& properties);

		inline std::shared_ptr<Shader>& getShader() { return m_pShader; };
		inline MaterialProperties& getMaterialProperties() { return m_materialProperties; }

		inline std::shared_ptr<Texture2D>& getColorTexture() { return m_materialProperties.colorTexture; }
	private:

		std::shared_ptr<Shader> m_pShader;
		MaterialProperties m_materialProperties;
	};
}