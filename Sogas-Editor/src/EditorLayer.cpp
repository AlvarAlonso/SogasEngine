#include "EditorLayer.h"

#include "core/input.h"
#include "core/keyCodes.h"
#include "core/logger.h"
#include "core/cameraController.h"
#include "platform/openGL/openGLShader.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "renderer/renderer.h"
#include "renderer/shader.h"
#include "renderer/resources/texture.h"
#include "renderer/framebuffer.h"

// TODO: find a better place to define components
#include "scene/entityFactory.h"
#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/transformComponent.h"
#include "scene/renderComponent.h"
#include "scene/cameraComponent.h"
#include "scene/lightComponent.h"

namespace Sogas 
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::onAttach()
	{
		Sogas::FramebufferSpecs specs;
		specs.width = 1280; // Application::getInstance()->getWindow().getWidth();
		specs.height = 720; // Application::getInstance()->getWindow().getHeight();

		m_framebuffer = Framebuffer::create(specs);

		m_pCamera = std::make_shared<Camera>();

		m_pScene = std::make_shared<Scene>();

		auto entity = m_pScene->createEntity("Cube");
		m_pScene->addComponent(entity, RenderComponent::s_name);
		makeStrongPtr(entity->getComponent<RenderComponent>(RenderComponent::s_name))->setMesh("../Assets/cube.obj");
		//makeStrongPtr(entity->getComponent<RenderComponent>(RenderComponent::s_name))->setMaterial(mat);

		glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 10.0f));
		makeStrongPtr(entity->getComponent<TransformComponent>(TransformComponent::s_name))->setTransform(translate);

		auto entity2 = m_pScene->createEntity("Cube");
		m_pScene->addComponent(entity2, RenderComponent::s_name);
		makeStrongPtr(entity2->getComponent<RenderComponent>(RenderComponent::s_name))->setMesh("../Assets/plane.obj");

		glm::mat4 translate2 = glm::translate(glm::mat4(1), glm::vec3(0.0f, -5.0f, 0.0f)) * glm::scale(glm::mat4(1), glm::vec3(10.0f));
		makeStrongPtr(entity2->getComponent<TransformComponent>(TransformComponent::s_name))->setTransform(translate2);

		auto light = m_pScene->createEntity("Light");
		m_pScene->addComponent(light, LightComponent::s_name);
		makeStrongPtr(light->getComponent<LightComponent>(LightComponent::s_name))->setColor(glm::vec3{ 1.0f, 1.0f, 1.0f });
		glm::mat4 lightTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
		makeStrongPtr(light->getComponent<TransformComponent>(TransformComponent::s_name))->setTransform(lightTransform);

		auto light2 = m_pScene->createEntity("Light");
		m_pScene->addComponent(light2, LightComponent::s_name);
		makeStrongPtr(light2->getComponent<LightComponent>(LightComponent::s_name))->setColor(glm::vec3{ 1.0f, 0.0f, 1.0f });

		// load texture
		// TODO: textures should be handled by material
		m_texture = Texture2D::create("../Assets/texture.png");

		// TODO: add scripting for camera movement/behavior
		m_cameraController.reset(new CameraController(m_pCamera));

		mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };
	}

	void EditorLayer::onDetach()
	{
		m_pScene->destroy();
	}

	void EditorLayer::onUpdate(f32 dt)
	{

		if (m_viewportFocused)
		{
			m_cameraController->onUpdate(dt);
		}

		// TODO: implement onUpdate func
		m_pScene->onUpdate(dt);

		m_framebuffer->bind();

		Renderer::beginScene(m_pScene, m_cameraEntity);
		m_texture->bind();

		std::vector<StrongEntityPtr> renderables = m_pScene->getByComponent(RenderComponent::s_name);
		
		for (const auto& renderable : renderables)
		{
			auto renderComponent = makeStrongPtr(renderable->getComponent<RenderComponent>(RenderComponent::s_name));
			glm::mat4& model = makeStrongPtr(renderable->getComponent<TransformComponent>(TransformComponent::s_name))->getTransform();

			Renderer::submit(renderComponent->getShader(), renderComponent->getMesh()->m_vertexArray, model);
		}

		Renderer::endScene();

		m_framebuffer->unbind();
	}

	void EditorLayer::onImguiRender()
	{
		ImGui::SetCurrentContext(Application::getImguiContext());

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Close", NULL, false))
					Application::getInstance()->close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Stats panel
		ImGui::Begin("Stats");
		ImGui::Text("Frame Count: %i", ImGui::GetFrameCount());
		ImGui::Text("Delta time: %f ms", io.DeltaTime);
		ImGui::Text("Framerate %.2f fps", io.Framerate);
		ImGui::End();

		ImGui::Begin("Components");
		for (const auto& entity : m_pScene->getEntities())
		{
			ImGui::Text("Entity");
			if (entity->has(TransformComponent::s_name)) 
			{
				bool changed = false;
				auto transformComponent = makeStrongPtr(entity->getComponent<TransformComponent>(TransformComponent::s_name));
				auto& matrix = transformComponent->getTransform();
				changed |= ImGui::SliderFloat3("Transform", &((glm::vec3)matrix[3])[0], -1000.0f, 1000.0f);
				if(changed)
					transformComponent->setTransform(matrix);
			}
		}
		ImGui::End();

		// Viewport panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0, 0.0));
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::getInstance()->getImGuiLayer()->blockEvents(!m_viewportFocused || !m_viewportHovered);

		// TODO: Should also resize the aspect ratio of the camera
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_viewportSize != *((glm::vec2*) & viewportPanelSize))
		{
			m_framebuffer->resize((u32)viewportPanelSize.x, (u32)viewportPanelSize.y);
			m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_cameraController.get()->setViewportSize(m_viewportSize.x, m_viewportSize.y);
		}
		u64 textureId = m_framebuffer->getColorAttachment();
		ImGui::Image((ImTextureID)textureId, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& event)
	{
		if (m_viewportFocused) {
			m_cameraController->onEvent(event);
		}
	}
}
