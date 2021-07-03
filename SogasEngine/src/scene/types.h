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
	// TODO: should be moved to a utils file
	template<typename T>
	std::shared_ptr<T> makeStrongPtr(std::weak_ptr<T> pWeakPtr)
	{
		if (!pWeakPtr.expired())
			return std::shared_ptr<T>(pWeakPtr);
		else
			return std::shared_ptr<T>();
	}

	class Entity;
	class EntityComponent;

	typedef u32 EntityId;
	typedef u32 ComponentId;

	const u32 MAX_COMPONENTS = 5;
	typedef std::bitset<MAX_COMPONENTS> Signature;

	typedef std::shared_ptr<Entity> StrongEntityPtr;
	typedef std::weak_ptr<Entity> WeakEntityPtr;
	typedef std::shared_ptr<EntityComponent> StrongEntityComponentPtr;
	typedef std::weak_ptr<EntityComponent> WeakEntityComponentPtr;
}