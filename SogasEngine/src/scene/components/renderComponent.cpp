
#include "sgspch.h"
#include "renderComponent.h"
#include "renderer/shader.h"
#include "renderer/resources/texture.h"

namespace Sogas
{
	const char* RenderComponent::s_name = "RenderComponent";

	/*
	* @brief constructor to default parameters. Cube.obj and basic.shader at the moment
	* @param void
	* @return none
	*/
	RenderComponent::RenderComponent()
	{
		m_pMesh = nullptr; // Mesh::GET("../Assets/cube.obj");
		//m_pMaterial = nullptr; // std::make_shared<Material>(Shader::GET("../SogasEngine/shaders/phong.shader"));
	}

	bool RenderComponent::init()
	{
		return true;
	}

	/*
	* Serialize RenderComponent to json data
	* @param json&
	* @retun void
	*/
	void RenderComponent::to_json(json& j)
	{
		json material;
		getMesh()->getMaterial()->to_json(material);
		j = json{
			{"Mesh", getMesh()->getMeshName() },
			{"Material", }
		};
	}

	/*
	* @brief Deserialize RenderComponent from json data
	* @param const json&
	* @return void
	*/
	void RenderComponent::from_json(const json& jsonComponent)
	{
		// -----------------------
		// Check for the mesh data - it should contain a Mesh, otherwise the render component loses its purpose
		// TODO: Maybe should include an error handling for when loading a render component without a mesh or add a default one
		// such as a cube
		// -----------------------
		if (jsonComponent.contains("Mesh") && !jsonComponent["Mesh"].is_null())
		{
			setMesh(jsonComponent["Mesh"].get<std::string>().c_str());
		}
		else {
			SGSWARN("Loading a render component with no valid Mesh!");
		}

		// -----------------------
		// Check for the material data
		// For the material data we create a default material if none is given. The default material should load the basic shader too
		// -----------------------
		if (jsonComponent.contains("Material"))
		{
			// Create the standard material, this will be inflated with the data parsed if any
			std::shared_ptr<Material> material = std::make_shared<Material>();

			if (!jsonComponent["Material"].is_null())
			{
				auto jsonMaterial = jsonComponent["Material"];

				material->from_json(jsonMaterial);
			}
			else {
				material->setMaterialShader(Shader::GET("../SogasEngine/shaders/basic.shader"));
				auto& properties = material->getMaterialProperties();
				properties.colorTexture = Sogas::Texture2D::GET(1, 1, "Default white");
				u32 defaultTextureData = 0xffffffff;
				properties.colorTexture->setData(&defaultTextureData, sizeof(u32));
				material->setMaterialProperties(properties);
			}
			setMaterial(material);
		}
		else
		{
			SGSWARN("Render component with no Material defined!");
		}
	}
}