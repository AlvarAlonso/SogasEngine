
#include "sgspch.h"
#include "serializer.h"

#include "core/assertions.h"
#include "core/logger.h"

#include "scene.h"
#include <nlohmann/json.hpp>

#include "components/transformComponent.h"
#include "components/renderComponent.h"
#include "components/lightComponent.h"
#include "components/cameraComponent.h"
#include "serializerUtils.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

namespace Sogas
{
	//-----------------------
	// 	   TRANSFORM COMPONENT
	//-----------------------
	void to_json(json& j, const std::shared_ptr<TransformComponent>& transformComponent)
	{
		json translation, rotation, scale;
		to_json(translation, transformComponent->getTranslation());
		to_json(rotation, transformComponent->getRotation());
		to_json(scale, transformComponent->getScale());

		j = json{
			{"Translation", translation},
			{"Rotation", rotation},
			{"Scale", scale}
		};
	}

	void from_json(const json& j, std::shared_ptr<TransformComponent>& transformComponent)
	{

		glm::vec3 translation, scale, rot;
		glm::quat rotation;
		from_json(j.at("Translation"), translation);
		from_json(j.at("Rotation"), rotation);
		from_json(j.at("Scale"), scale);
		
		rot = glm::eulerAngles(rotation);

		transformComponent->setTranslation(translation);
		transformComponent->setRotation(rot);
		transformComponent->setScale(scale);
	}

	//-----------------------
	// 	   RENDER COMPONENT
	//-----------------------
	void to_json(json& j, const std::shared_ptr<RenderComponent>& renderComponent)
	{
		j = json{
			{"Mesh", renderComponent->getMesh()->getMeshName() },
			{"Material", nullptr}
		};
	};

	void from_json(const json& j, std::shared_ptr<RenderComponent>& renderComponent)
	{
		//renderComponent->setMesh(j["Mesh"].get<std::string>().c_str());
		//json jMaterial;
		//std::shared_ptr<Material> material;
		//from_json(jMaterial, material);
		//renderComponent->setMaterial(material);
	}

	//-----------------------
	// 	   LIGHT COMPONENT
	//-----------------------
	void to_json(json& j, const std::shared_ptr<LightComponent>& lightComponent)
	{
		json color;
		to_json(color, lightComponent->getColor());

		j = json
		{
			{ "Color", color },
			{ "MaxDistance", lightComponent->getMaxDistance() },
			{ "Intensity", lightComponent->getIntensity() }
		};
	}

	void from_json(const json& j, std::shared_ptr<LightComponent>& lightComponent)
	{

	}


	//-----------------------
	// 	   MATERIAL
	//-----------------------

	void to_json(json& j, const Material& material)
	{
		j = json
		{
			// TODO: Material to json func
		};
	}

	void from_json(const json& j, std::shared_ptr<Material>& material)
	{
		
	}

	void to_json(json& j, const StrongEntityPtr& entity)
	{
		json transform, render, light;
		to_json(transform, makeStrongPtr(entity->getComponent<TransformComponent>()));
		
		if (entity->has<RenderComponent>())
			to_json(render, makeStrongPtr(entity->getComponent<RenderComponent>()));

		if (entity->has<LightComponent>())
			to_json(light, makeStrongPtr(entity->getComponent<LightComponent>()));

		j = json
		{
			{"Name", !entity->getName().empty() ? entity->getName() : "Unknown name" },
			{TransformComponent::s_name, transform},
			{RenderComponent::s_name, render},
			{LightComponent::s_name, light}
		};
	}

	void to_json(json& j, const std::shared_ptr<Scene>& scene)
	{
		json entArray = json::array();
		for (auto& entity : scene->getEntities())
		{
			json ent;
			to_json(ent, entity);
			entArray.push_back(ent);
		}
		j = json{ {"Entities", entArray} };
	}

	Serializer::Serializer(std::shared_ptr<Scene>& scene)
		: m_pScene(scene)
	{
	}

	bool Serializer::serialize(const std::string& filename)
	{

		json scene;
		to_json(scene, m_pScene);

		json output = json
		{
			{"Scene", scene}
		};

		std::ofstream o(filename);
		o << output << std::endl;

		return true;
	}

	bool Serializer::deserialize(const std::string& filename)
	{
		SGSASSERT_MSG(false, "Function not implemented yet.");
		return false;
	}
}