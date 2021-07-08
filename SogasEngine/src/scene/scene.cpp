
#include "sgspch.h"
#include "scene.h"

#include "renderer/shader.h"
#include "transformComponent.h"
#include "renderComponent.h"
#include "glm/gtc/quaternion.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Sogas
{
	Scene::Scene()
	{
		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();
	}

	Scene::Scene(const char* filename)
	{

		std::ifstream file(filename);
		json j;
		file >> j;
		
		std::cout << j;

		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();

		if (j.contains("Scene"))
		{
			auto scene = j["Scene"];
			if (scene.contains("Entities")) {
				auto entities = scene["Entities"];

				for (auto entity : entities)
				{
					std::string name = entity["Name"].get<std::string>();
					// TODO: pass the entity json to the function
					auto ent = m_pEntityFactory->createEntity(name.c_str());

					if (entity.contains(TransformComponent::s_name) && !entity[TransformComponent::s_name].is_null())
					{
						// TODO: add transform
						auto jsonTransform = entity[TransformComponent::s_name];
						auto transformComponent = makeStrongPtr<TransformComponent>(ent->getComponent<TransformComponent>(TransformComponent::s_name));
						glm::mat4 model = glm::mat4(1);

						if (jsonTransform.contains("Position") && !jsonTransform["Position"].is_null())
						{
							auto jsonPosition = jsonTransform["Position"];
							glm::vec3 position = glm::vec3(jsonPosition["x"], jsonPosition["y"], jsonPosition["z"]);
							model = glm::translate(model, position);
						}
						if (jsonTransform.contains("Rotation") && !jsonTransform["Rotation"].is_null())
						{
							// TODO: rotation
							auto jsonRotation = jsonTransform["Rotation"];
							glm::quat quaternion = glm::quat(jsonRotation["x"], jsonRotation["y"], jsonRotation["z"], jsonRotation["w"]);
							glm::mat4 rotationMatrix = glm::mat4_cast(quaternion);
							model = model * rotationMatrix;
						}
						if (jsonTransform.contains("Scale") && !jsonTransform["Scale"].is_null())
						{
							auto jsonScale = jsonTransform["Scale"];
							glm::vec3 scale = glm::vec3(jsonScale["x"], jsonScale["y"], jsonScale["z"]);
							model = glm::scale(model, scale);
						}

						transformComponent->setTransform(model);
					}
					if (entity.contains(RenderComponent::s_name) && !entity[RenderComponent::s_name].is_null())
					{

						ent->addComponent(m_pEntityFactory->createComponent(RenderComponent::s_name));

						auto jsonComponent = entity[RenderComponent::s_name];
						std::shared_ptr<RenderComponent> renderComponent = makeStrongPtr(ent->getComponent<RenderComponent>(RenderComponent::s_name));

						// -----------------------
						// Check for the mesh data - it should contain a Mesh, otherwise the render component loses its purpose
						// TODO: Maybe should include an error handling for when loading a render component without a mesh or add a default one
						// such as a cube
						// -----------------------
						if (jsonComponent.contains("Mesh") && !jsonComponent["Mesh"].is_null())
						{
							renderComponent->setMesh(jsonComponent["Mesh"].get<std::string>().c_str());
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
							
								// Check if a valid shader is given
								if (jsonMaterial.contains("Shader") && 
									!jsonMaterial["Shader"].is_null() && 
									jsonMaterial["Shader"].is_string())
								{
									material->setMaterialShader(Shader::create(jsonMaterial["Shader"].get<std::string>()));
								}
								else {
									material->setMaterialShader(Shader::create("../SogasEngine/shaders/basic.shader"));
								}

								// Get the material properties
							}
							else {
								material->setMaterialShader(Shader::create("../SogasEngine/shaders/basic.shader"));
							}
							renderComponent->setMaterial(material);
						}
						else
						{
							SGSWARN("Render component with no Material defined!");
						}

					}
					m_entities.push_back(ent);
				}
			}
		}
	}

	Scene::~Scene()
	{
		
	}

	StrongEntityPtr Scene::createEntity(const std::string& name)
	{
		StrongEntityPtr entity = m_pEntityFactory->createEntity(name.c_str());
		m_entities.push_back(entity);
		return entity;
	}

	void Scene::DestroyEntity(StrongEntityPtr entity)
	{
		entity->destroy();
	}

	void Scene::destroy()
	{
		for (auto& entity : m_entities)
		{
			DestroyEntity(entity);
		}
	}

	// TODO: scene onUpdate should update and submit commands to the renderer, thus the renderer needs to have access to the shader
	// and textures to bind them when updating/rendering
	void Scene::onUpdate(f32 dt)
	{

		//for (auto& entity : m_entities)
		//{
		//	auto tComponent = entity->getComponent<TransformComponent>(TransformComponent::s_name);
		//}

	}

	void Scene::addComponent(StrongEntityPtr entity, const char* componentName)
	{
		if (!componentName)
		{
			SGSWARN("No valid component name given.");
			return;
		}

		// TODO: name from type may not be the same as stored in static s_name
		// Should find a more robust way to create a specific component type
		entity->addComponent(m_pEntityFactory->createComponent(componentName));
	}

	std::vector<StrongEntityPtr> Scene::getByComponent(const char* name)
	{
		std::vector<StrongEntityPtr> returnVector{};

		for (auto& entity : m_entities)
		{
			if (entity->has(EntityComponent::getIdFromName(name)))
			{
				returnVector.push_back(entity);
			}
		}
		
		return returnVector;
	}
}