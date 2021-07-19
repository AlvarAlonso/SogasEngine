#pragma once

#include <nlohmann/json.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Sogas
{
	using json = nlohmann::json;
	using ordered_json = nlohmann::ordered_json;

	/*
	* Convert a glm::vec3 to json vector
	* @param json&
	* @param const glm::vec3&
	* @return void
	*/
	void to_json(json& j, const glm::vec3& vector)
	{
		j = json{
			{ "x", vector.x },
			{ "y", vector.y },
			{ "z", vector.z }
		};
	}

	/*
	* Convert a json vector with 3 components to a glm::vec3&
	* @param const json&
	* @param glm::vec3&
	* @return void
	*/
	void from_json(const json& j, glm::vec3& vector)
	{
		j.at("x").get_to(vector.x);
		j.at("y").get_to(vector.y);
		j.at("z").get_to(vector.z);
	}

	/*
	* Convert a glm::vec4 to json vector
	* @param json&
	* @param const glm::vec4&
	* @return void
	*/
	void to_json(json& j, const glm::vec4& vector)
	{
		j = ordered_json{
			{ "x", vector.x },
			{ "y", vector.y },
			{ "z", vector.z },
			{ "w", vector.w }
		};
	}

	/*
	* Convert a json vector with 4 components to a glm::vec4&
	* @param const json&
	* @param glm::vec3&
	* @return void
	*/
	void from_json(const json& j, glm::vec4& vector)
	{
		j.at("x").get_to(vector.x);
		j.at("y").get_to(vector.y);
		j.at("z").get_to(vector.z);
		j.at("w").get_to(vector.w);
	}

	/*
	* Convert a quaternion to json data
	* @param json&
	* @param const glm::quat&
	* @return void
	*/
	void to_json(json& j, const glm::quat& quat)
	{
		j = ordered_json
		{
			{ "x", quat.x },
			{ "y", quat.y },
			{ "z", quat.z },
			{ "w", quat.w },
		};
	}

	/*
	* Convert json data to a quaternion
	* @param const json&
	* @param glm::quat&
	* @return void
	*/
	void from_json(const json& j, glm::quat& quat)
	{
		j.at("x").get_to(quat.x);
		j.at("y").get_to(quat.y);
		j.at("z").get_to(quat.z);
		j.at("w").get_to(quat.w);
	}
}