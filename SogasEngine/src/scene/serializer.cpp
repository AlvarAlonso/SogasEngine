
#include "sgspch.h"
#include "serializer.h"

#include "core/assertions.h"
#include "core/logger.h"

#include "scene.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

namespace Sogas
{
	Serializer::Serializer(std::shared_ptr<Scene>& scene)
		: m_pScene(scene)
	{
	}

	/*
	* @brief Serialize the scene and store to the given path
	* @param const std::string&
	* @retun bool
	*/
	bool Serializer::serialize(const std::string& filename)
	{

		json scene;
		m_pScene->to_json(scene);

		json output = json
		{
			{"Scene", scene}
		};

		std::ofstream o(filename);
		o << output << std::endl;

		return true;
	}

	/*
	* @brief Deserialize the scene stored in the json file in the given path
	* @param const std::string&
	* @return bool
	*/
	bool Serializer::deserialize(const std::string& filename)
	{
		SGSASSERT_MSG(false, "Function not implemented yet.");
		return false;
	}
}