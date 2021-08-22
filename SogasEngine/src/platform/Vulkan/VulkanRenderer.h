#pragma once

#include "VulkanTypes.h"
#include "renderer/rendererAPI.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderpass.h"
#include "VulkanCommandbuffer.h"
#include "VulkanFence.h"

namespace Sogas
{
	class SGS VulkanRenderer : public RendererAPI
	{
	public:
		VkInstance							m_instance;
		std::unique_ptr<VulkanDevice>		m_device;
		std::unique_ptr<VulkanSwapchain>	m_swapchain;
		std::unique_ptr<VulkanRenderpass>	m_renderpass;
		VkSurfaceKHR						m_surface;

		u32 m_currentFrame, m_imageIndex;
		u32 m_framebufferWidth, m_framebufferHeight;

		std::vector<VulkanCommandbuffer>	m_graphicsCommandBuffer;
		std::vector<VkSemaphore>			m_imageAvailableSemaphore;
		std::vector<VkSemaphore>			m_queueCompleteSemaphore;
		std::vector<VulkanFence>			m_inFlightFences;
		std::vector<VulkanFence*>			m_imagesInFlightFences;

		VkDescriptorPool m_descriptorPool;

#ifdef _DEBUG
		VkDebugUtilsMessengerEXT debugMessenger;
#endif
	private:
		static VulkanRenderer* m_singletonInstance;
	public:
		VulkanRenderer(void);

		static VulkanRenderer* get(void);

		virtual bool init() override;
		virtual void shutdown() override;

		virtual void clear() override {};
		virtual void setClearColor(const glm::vec4& colour) override {};
		virtual void enableDepthBuffer(const bool& depth) override {};
		virtual void setDepthFunc(const DepthTypes factor) override {};
		virtual void enableDepthMask(const bool& mask) override {};
		virtual void draw(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES) override {};
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES) override {};
		virtual void setBlendFunc(const BlendTypes sfactor, const BlendTypes dfactor) override {};
		virtual void enableBlend(const bool blend) override {};
		virtual void setLineWidth(const f32 width) override {};

		// Vulkan things
		virtual bool beginFrame() override;
		virtual bool endFrame() override;
	};
}