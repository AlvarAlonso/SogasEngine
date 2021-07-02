#include "EditorLayer.h"

#include "renderer/renderer.h"
#include "core/input.h"
#include "core/keyCodes.h"
#include "core/mouseButtonCodes.h"
#include "core/logger.h"
#include "platform/openGL/openGLShader.h"
#include "../external/glm/glm/gtc/matrix_transform.hpp"
#include "scene/entity.h"
#include "scene/prefab.h"
#include <ImGuizmo.h>
#include <../external/glm/glm/gtc/type_ptr.hpp>

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

		// load texture
		m_texture = Texture2D::create("../Assets/texture.png");

		Mesh* mesh = new Mesh();
		mesh->load("../Assets/viking_room.obj");

		m_shader.reset(Shader::create("../SogasEngine/shaders/basic.shader"));

		m_camera.reset(new Camera());
		m_camera->setPosition({ 0, 15, -50 });

		m_cameraController.reset(new CameraController(m_camera));

		mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };

		m_currentScene = std::make_unique<Scene>();

		m_prefab.reset(new Prefab());
		Node* node = new Node();
		node->m_mesh.reset(mesh);
		m_prefab.get()->m_roots.push_back(node);

		Renderable* renderable = new Renderable();

		renderable->m_prefab = m_prefab;

		m_currentScene.get()->entities.push_back(renderable);
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate(f32 dt)
	{
		if (m_viewportFocused)
		{
			m_cameraController->onUpdate(dt);
		}

		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 5.0f));
		model = glm::rotate(glm::mat4(model), glm::radians(0.0f), glm::vec3(1, 0, 0));

		m_framebuffer->bind();

		Renderer::setClearColor(glm::vec4(0.2));
		Renderer::setDepthBuffer(true);
		Renderer::clear();

		m_shader->bind();
		m_texture->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_projection", m_camera->getProjection());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_view", m_camera->getView());
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_model", model);
		std::dynamic_pointer_cast<OpenGLShader>(m_shader)->setUniform("u_texture", 0);

		// TODO: clean up the prefab and entity API, pass the scene to be rendered to the renderer
		Renderer::drawIndexed(m_prefab.get()->m_roots[0]->m_mesh->m_vertexArray);

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

		// Scene Hierarchy
		ImGui::Separator;
		ImGui::Text("Scene Hierarchy");
		for(auto& entity : m_currentScene.get()->entities)
		{
			entity->OnImguiRender();
		}
		ImGui::Separator;
		ImGui::End();

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
		u32 textureId = m_framebuffer->getColorAttachment();
		ImGui::Image((ImTextureID)textureId, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		// Gizmos
		Entity* selectedEntity = Application::m_guizmoEntity;
		if(selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);
		
			// Editor camera
			const glm::mat4& cameraProjection = m_camera.get()->getProjection();
			glm::mat4 cameraView = m_camera.get()->getView();

			// Entity transform
			glm::mat4& transform = selectedEntity->m_model;

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

		glm::mat4& transform = Application::m_guizmoEntity->m_model;

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
		ImGuizmo::Manipulate(glm::value_ptr(m_camera.get()->getView()), glm::value_ptr(m_camera.get()->getProjection()), sCurrentGuizmoOperation,
			sCurrentGuizmoMode, glm::value_ptr(transform), nullptr, useSnap ? &snap.x : nullptr);
	}
}
