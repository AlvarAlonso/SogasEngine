#pragma once
#include "defines.h"

#include "core/logger.h"
#include "core/assertions.h"

#include <queue>
#include <bitset>
#include <array>
#include <memory>

namespace Sogas
{
	class Entity;
	class EntityComponent;

	// TODO: should be moved to a utils file
	template<typename T>
	std::shared_ptr<T> makeStrongPtr(std::weak_ptr<T> pWeakPtr)
	{
		if (!pWeakPtr.expired())
			return std::shared_ptr<T>(pWeakPtr);
		else
			return std::shared_ptr<T>();
	}

	enum class Primitive { POINTS = 0, LINES, LINE_STRIP, LINE_LOOP, TRIANGLES, TRIANGLES_STRIP, TRIANGLE_FAN };
	enum class BlendTypes { ZERO = 0, ONE, SRC_COLOR, ONE_MINUS_SRC_COLOR, DST_COLOR, ONE_MINUS_DST_COLOR, 
		SRC_ALPHA, ONE_MINUS_SRC_ALPHA, DST_ALPHA, ONE_MINUS_DST_ALPHA, CONSTANT_COLOR, ONE_MINUS_CONSTANT_COLOR, 
		CONSTANT_ALPHA, ONE_MINUS_CONSTANT_ALPHA, SRC_ALPHA_SATURATE, SRC1_COLOR, ONE_MINUS_SRC1_COLOR, SRC1_ALPHA, ONE_MINUS_SRC1_ALPHA};
	enum class DepthTypes {};


	typedef u32 EntityId;
	typedef u32 ComponentId;

	const u32 MAX_COMPONENTS = 5;
	typedef std::bitset<MAX_COMPONENTS> Signature;

	typedef std::shared_ptr<Entity> StrongEntityPtr;
	typedef std::weak_ptr<Entity> WeakEntityPtr;
	typedef std::shared_ptr<EntityComponent> StrongEntityComponentPtr;
	typedef std::weak_ptr<EntityComponent> WeakEntityComponentPtr;
}