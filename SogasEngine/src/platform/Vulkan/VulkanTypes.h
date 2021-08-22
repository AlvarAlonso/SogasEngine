#pragma once

#include "vulkan/vulkan.hpp"
#include "defines.h"
#include "core/logger.h"
#include "core/assertions.h"

namespace Sogas
{
	typedef enum VulkanRenderpassState {
		READY,
		RECORDING,
		IN_RENDER_PASS,
		RECORDING_ENDED,
		SUBMITTED,
		NOT_ALLOCATED
	}VulkanRenderpassState;
}

#define VK_CHECK(x) do { VkResult err = x; if(err){ SGSERROR("Vulkan Error: %s", err); abort(); }} while(0)