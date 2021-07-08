#include "EditorLayer.h"

#include "core/input.h"
#include "core/keyCodes.h"
#include "core/mouseButtonCodes.h"
#include "core/logger.h"
#include "core/cameraController.h"
#include "platform/openGL/openGLShader.h"

#include <ImGuizmo.h>
#include <../external/glm/glm/gtc/type_ptr.hpp>

/*
#include "../external/glm/glm/gtc/matrix_transform.hpp"
#include "scene/entity.h"
#include "scene/prefab.h"
*/

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

		m_pScene = std::make_shared<Scene>();

		auto entity = m_pScene->createEntity("Cube");
		m_pScene->addComponent(entity, RenderComponent::s_name);
		makeStrongPtr(entity->getComponent<RenderComponent>(RenderComponent::s_name))->setMesh("../Assets/cube.obj");

		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 10.0f);
		makeStrongPtr(entity->getComponent<TransformComponent>(TransformComponent::s_name))->setTranslation(translate);

		auto entity2 = m_pScene->createEntity("Cube");
		m_pScene->addComponent(entity2, RenderComponent::s_name);
		makeStrongPtr(entity2->getComponent<RenderComponent>(RenderComponent::s_name))->setMesh("../Assets/cube.obj");

		glm::vec3 translate2 = glm::vec3(5.0f, 0.0f, 10.0f);
		makeStrongPtr(entity2->getComponent<TransformComponent>(TransformComponent::s_name))->setTranslation(translate2);

		auto light = m_pScene->createEntity("Light");
		m_pScene->addComponent(light, LightComponent::s_name);
		makeStrongPtr(light->getComponent<LightComponent>(LightComponent::s_name))->setColor(glm::vec3{ 1.0f, 0.0f, 1.0f });
		glm::vec3 lightTransform = glm::vec3(0.0f, 10.0f, 0.0f);
		makeStrongPtr(light->getComponent<TransformComponent>(TransformComponent::s_name))->setTranslation(lightTransform);

		// load texture
		m_texture = Texture2D::create("../Assets/texture.png");

		m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

		m_cameraEntity = m_pScene->createEntity("Camera");
		m_pScene->addComponent(m_cameraEntity, CameraComponent::s_name);

		// TODO: add scripting for camera movement/behavior
		m_cameraController.reset(new CameraController(makeStrongPtr(m_cameraEntity->getComponent<CameraComponent>(CameraComponent::s_name))->camera));

		mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };
	
		m_scenePanel.setContext(m_pScene);
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

		Renderer::setClearColor(glm::vec4(0.2f));
		Renderer::setDepthBuffer(true);
		Renderer::clear();

		m_shader->bind();
		m_texture->bind();

		std::shared_ptr<CameraComponent> camera = makeStrongPtr(m_cameraEntity->getComponent<CameraComponent>(CameraComponent::s_name));

		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_projection", camera->camera->getProjection());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_view", camera->camera->getView());

		std::vector<StrongEntityPtr> renderables = m_pScene->getByComponent(RenderComponent::s_name);
		std::vector<StrongEntityPtr> lights = m_pScene->getByComponent(LightComponent::s_name);

		for (const auto& renderable : renderables)
		{
			glm::mat4 model = makeStrongPtr(renderable->getComponent<TransformComponent>(TransformComponent::s_name))->getTransform();
			std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_model", model);
			std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_texture", 0);

			for (const auto& light : lights)
			{
				// Set light position
				glm::mat4 lightModel = makeStrongPtr(light->getComponent<TransformComponent>(TransformComponent::s_name))->getTransform();
				std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("lightPosition", (glm::vec3)lightModel[3]);

				// Set light colour
				auto lightComponent = makeStrongPtr(light->getComponent<LightComponent>(LightComponent::s_name));
				std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("lightColor", lightComponent->getColor());

				Renderer::drawIndexed(makeStrongPtr(renderable->getComponent<RenderComponent>(RenderComponent::s_name))->getMesh()->m_vertexArray);
			}
		}

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

		m_scenePanel.onImGuiRender();

		// Stats panel
		ImGui::Begin("Stats");
		ImGui::Text("Frame Count: %i", ImGui::GetFrameCount());
		ImGui::Text("Delta time: %f ms", io.DeltaTime);
		ImGui::Text("Framerate %.2f fps", io.Framerate);
		ImGui::End();

		// TODO: Redo scene hierarchy
		/*
		// Scene Hierarchy
		ImGui::Separator;
		ImGui::Text("Scene Hierarchy");
		for(auto& entity : m_currentScene.get()->getEntities())
		{
			entity->OnImguiRender();
		}
		ImGui::Separator;
		ImGui::End();
		*/
		/*
		* // Example:
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
		*/
		// Viewport panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0, 0.0));
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::getInstance()->getImGuiLayer()->blockEvents(!m_viewportFocused || !m_viewportHovered);

		//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		//m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

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
		
		// Gizmos
		Entity* selectedEntity = Application::m_guizmoEntity;
		if(selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);
		
			// Editor camera
			const glm::mat4& cameraProjection = m_cameraEntity->getComponent<CameraComponent>(CameraComponent::s_name).lock()->camera->getProjection();
			glm::mat4 cameraView = m_cameraEntity->getComponent<CameraComponent>(CameraComponent::s_name).lock()->camera->getView();

			// Entity transform
			glm::mat4 transform = selectedEntity->getComponent<TransformComponent>(TransformComponent::s_name).lock()->getTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, nullptr);

			if(ImGuizmo::IsUsing())
			{
				f32 matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), matrixTranslation, matrixRotation, matrixScale);

				// apply transformation here

				ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(transform));
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& event)
	{
		if (m_viewportFocused) {
			m_cameraController.get()->onEvent(event);
		}

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(EditorLayer::onKeyPressed));
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		if (e.getRepeatCount() > 0)
			return false;

		switch(e.getKeyCode())
		{
			// Gizmos
			case SGS_KEY_Q:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = -1;
				break;
			}
			case SGS_KEY_W:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case SGS_KEY_E:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case SGS_KEY_R:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
	}

	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	void EditorLayer::onImGuizmoRender()
	{
		if (Application::m_guizmoEntity == nullptr)
			return;

		glm::mat4 transform = Application::m_guizmoEntity->getComponent<TransformComponent>(TransformComponent::s_name).lock()->getTransform();

		static ImGuizmo::OPERATION sCurrentGuizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE sCurrentGuizmoMode(ImGuizmo::WORLD);

		// TODO: Abstract the input
		if (ImGui::IsKeyPressed(90))
			sCurrentGuizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			sCurrentGuizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82))
			sCurrentGuizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", sCurrentGuizmoOperation == ImGuizmo::TRANSLATE))
			sCurrentGuizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", sCurrentGuizmoOperation == ImGuizmo::ROTATE))
			sCurrentGuizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", sCurrentGuizmoOperation == ImGuizmo::SCALE))
			sCurrentGuizmoOperation = ImGuizmo::SCALE;

		f32 matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation, "%.3f");
		ImGui::InputFloat3("Rt", matrixRotation, "%.3f");
		ImGui::InputFloat3("Sc", matrixScale, "%.3f");
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(transform));

		if(sCurrentGuizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", sCurrentGuizmoMode == ImGuizmo::LOCAL))
				sCurrentGuizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", sCurrentGuizmoMode == ImGuizmo::WORLD))
				sCurrentGuizmoMode = ImGuizmo::WORLD;
		}

		static bool useSnap(false);
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("", &useSnap);
		ImGui::SameLine();
		static glm::vec3 snap;
		switch (sCurrentGuizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap.x);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap.x);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap.x);
			break;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(glm::value_ptr(m_cameraEntity->getComponent<CameraComponent>(CameraComponent::s_name).lock()->camera->getView()), 
			glm::value_ptr(m_cameraEntity->getComponent<CameraComponent>(CameraComponent::s_name).lock()->camera->getProjection()), sCurrentGuizmoOperation,
			sCurrentGuizmoMode, glm::value_ptr(transform), nullptr, useSnap ? &snap.x : nullptr);
	}
}