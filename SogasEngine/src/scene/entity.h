#pragma once

#include "defines.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <memory>

enum class e_LightType 
{
	DIRECTIONAL = 0,
	POINT,
	SPOT
};

class Prefab;

// TODO: This is temporal until we implement an Entity Component System

class SGS Entity
{
public:
	std::string m_name;
	glm::mat4 m_model;

	Entity();
};

class SGS Renderable : public Entity
{
public:
	Renderable();
	Renderable(const std::string& name);

	std::shared_ptr<Prefab> m_prefab;
};

class SGS Light : public Entity
{
public:
	Light();
	Light(const std::string& name);

	glm::vec3 m_color;
	f32 m_maxDist;
	f32 m_intensity;
	e_LightType m_lightType;
};