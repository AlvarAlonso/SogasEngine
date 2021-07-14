#pragma once

#include "nlohmann/json.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Sogas
{
	using json = nlohmann::json;
	using ordered_json = nlohmann::ordered_json;

	//-----------------------
	// 	   VEC3
	//-----------------------
	void to_json(json& j, const glm::vec3& vector)
	{
		j = json{
			{ "x", vector.x },
			{ "y", vector.y },
			{ "z", vector.z }
		};
	}

	void from_json(const json& j, glm::vec3& vector)
	{
		j.at("x").get_to(vector.x);
		j.at("y").get_to(vector.y);
		j.at("z").get_to(vector.z);
	}

	//-----------------------
	// 	   VEC4
	//-----------------------
	void to_json(json& j, const glm::vec4& vector)
	{
		j = ordered_json{
			{ "x", vector.x },
			{ "y", vector.y },
			{ "z", vector.z },
			{ "w", vector.w }
		};
	}

	void from_json(const json& j, glm::vec4& vector)
	{
		j.at("x").get_to(vector.x);
		j.at("y").get_to(vector.y);
		j.at("z").get_to(vector.z);
		j.at("w").get_to(vector.w);
	}

	//-----------------------
	// 	   QUATERNION
	//-----------------------
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

	void from_json(const json& j, glm::quat& quat)
	{
		j.at("x").get_to(quat.x);
		j.at("y").get_to(quat.y);
		j.at("z").get_to(quat.z);
		j.at("w").get_to(quat.w);
	}
}