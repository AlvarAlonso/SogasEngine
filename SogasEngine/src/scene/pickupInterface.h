#pragma once

#include "defines.h"
#include "types.h"

#include "entityComponent.h"

namespace Sogas
{
	class PickupInterface : public EntityComponent
	{
	public:

		//const static ComponentId COMPONENT_ID;

		virtual void apply(WeakEntityPtr pEntity) = 0;
	};
}