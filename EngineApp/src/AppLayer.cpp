#include "AppLayer.h"

namespace Hazel {
	AppLayer::AppLayer()
	{
	}

	void AppLayer::OnAttach()
	{
		m_Scene = std::make_shared<Scene>();
		m_ModelingCamera = std::make_shared<ModelingCamera>();
		m_Viewports = std::vector<ViewportData>(3);

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_SceneHierarchyPanel.m_Scene = m_Scene;

		m_Selected = 0;
		//m_Color = &m_Scene->m_ObjectArray[m_Scene->m_Selected].m_Colors[0];
	}

	void AppLayer::OnDetach()
	{
	}

	void AppLayer::OnUpdate(float timestep)
	{
		m_ModelingCamera->OnUpdate(timestep, m_Viewports[0].width, m_Viewports[0].height);
		

	}

	void AppLayer::OnImGuiRender()
	{
		
		// Create the docking environment
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
		ImGui::PopStyleVar(3);
		ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"));

				if (ImGui::MenuItem("Exit"));

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();


		bool b_GlobalWinDebug = true;
		bool b_WinDebug = true;
		ImGuiWindowClass window_class;


		b_WinDebug = true;
		if (b_WinDebug && b_GlobalWinDebug)
		{
			window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
			ImGui::SetNextWindowClass(&window_class);
		}
		m_SceneHierarchyPanel.OnImGuiRender();



		b_WinDebug = true;
		if (b_WinDebug && b_GlobalWinDebug)
		{
			window_class.DockNodeFlagsOverrideSet = 0 | ImGuiDockNodeFlags_NoTabBar;
			ImGui::SetNextWindowClass(&window_class);
		}
		ImGui::Begin("TOOLBAR2");
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Text("Toolbar2");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::GetWindowDrawList();
		ImGui::End();



		b_WinDebug = true;
		if (b_WinDebug && b_GlobalWinDebug)
		{
			window_class.DockNodeFlagsOverrideSet = 0 | ImGuiDockNodeFlags_NoTabBar;
			ImGui::SetNextWindowClass(&window_class);
		}
		ImGui::Begin("TOOLBAR3");
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		if (m_Scene->m_Selected >= 0)
			ImGui::Text(m_Scene->m_ObjectArray[m_Scene->m_Selected].m_Name.c_str());
		else
			ImGui::Text("Scene");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		DrawTransform();
		DrawColorEditor();
		//glm::vec3 c3 = glm::vec3(0, 0, 0);
		
	

		ImGui::End();
		


		b_WinDebug = true;
		if (b_WinDebug && b_GlobalWinDebug)
		{
			window_class.DockNodeFlagsOverrideSet = 0 | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_NoDockingSplitOther;
			ImGui::SetNextWindowClass(&window_class);
		}
		ImGui::Begin("Main Viewport");
		UpdateViewport(0);
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_Viewports[0].width > 0.0f && m_Viewports[0].height > 0.0f && 
			(spec.Width != m_Viewports[0].width || spec.Height != m_Viewports[0].height))
		{
			m_Framebuffer->Resize((uint32_t)m_Viewports[0].width, (uint32_t)m_Viewports[0].height);
		}
		m_Framebuffer->Bind();
		m_Scene->OnUpdate(m_ModelingCamera->m_ProjectionMatrix, m_ModelingCamera->m_ViewMatrix, m_ModelingCamera->m_CameraModelMatrix, m_ModelingCamera->b_left);
		m_Scene->Draw();
		m_Framebuffer->Unbind();
		int64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_Viewports[0].width, m_Viewports[0].height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		bool m_ViewportFocused = ImGui::IsWindowFocused();
		bool m_ViewportHovered = ImGui::IsWindowHovered();

		ImGui::End();



		/*
		b_WinDebug = true;
		if (b_WinDebug && b_GlobalWinDebug)
		{
			window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
			ImGui::SetNextWindowClass(&window_class);
		}
		ImGui::Begin("Viewport1");
		ImGui::Text("Viewport 1");
		UpdateViewport(1);
		
		ImGui::End();
		*/


		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 200.0f;
		style.WindowMinSize.y = 200.0f;
	

	}

	void AppLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(AppLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(AppLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(AppLayer::OnMouseScrolled));
	}

	bool AppLayer::OnMouseScrolled(Hazel::MouseScrolledEvent& event)
	{
		float offset = event.GetYOffset();
		auto pos = ImGui::GetMousePos();
		if (pos.x > m_Viewports[0].x1 && pos.x < m_Viewports[0].x2 && pos.y > m_Viewports[0].y1 && pos.y < m_Viewports[0].y2)
		{
			m_ModelingCamera->Scroll(offset);
		}

		return false;
	}

	bool AppLayer::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == (int)Hazel::Mouse::ButtonMiddle)
		{
			auto pos = ImGui::GetMousePos();
			if (pos.x > m_Viewports[0].x1 && pos.x < m_Viewports[0].x2 && pos.y > m_Viewports[0].y1 && pos.y < m_Viewports[0].y2)
			{
				m_ModelingCamera->b_selected = true;
			}
		}

		return false;
	}

	bool AppLayer::OnMouseButtonReleased(Hazel::MouseButtonReleasedEvent& event)
	{
		if (event.GetMouseButton() == (int)Hazel::Mouse::ButtonMiddle)
		{
			m_ModelingCamera->b_selected = false;
		}

		return false;
	}

	inline void AppLayer::UpdateViewport(int index)
	{
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGuiWindow* ViewportWindow = GImGui->CurrentWindow;
		ImVec2 max = ViewportWindow->ContentRegionRect.Max;
		m_Viewports[index].x1 = max.x - viewportPanelSize.x;
		m_Viewports[index].y1 = max.y - viewportPanelSize.y;
		m_Viewports[index].x2 = max.x;
		m_Viewports[index].y2 = max.y;
		m_Viewports[index].width = viewportPanelSize.x;
		m_Viewports[index].height = viewportPanelSize.y;
	}

	inline void AppLayer::DrawTransform()
	{
		if (m_Scene->m_Selected >= 0) 
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("\t\tLocation\t");
				ImGui::SameLine();
				if (ImGui::Button("reset", ImVec2(40, 25))) m_Scene->m_ObjectArray[m_Scene->m_Selected].ResetLoc();
				ImGui::Text("\t\t\t\tX");
				ImGui::SameLine();
				ImGui::DragFloat("\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].xLoc, 0.01f, 0, 0, "%.3f m");
				ImGui::Text("\t\t\t\tY");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].yLoc, 0.01f, 0, 0, "%.3f m");
				ImGui::Text("\t\t\t\tZ");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].zLoc, 0.01f, 0, 0, "%.3f m");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(0.0f, 20.0f));

				ImGui::Text("\t\tRotation\t");
				ImGui::SameLine();
				if (ImGui::Button("reset  ", ImVec2(40, 25))) m_Scene->m_ObjectArray[m_Scene->m_Selected].ResetRot();
				ImGui::Text("\t\t\t\tX");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].xRotate, 0.1f, 0, 0, "%.3f '");
				ImGui::Text("\t\t\t\tY");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t\t\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].yRotate, 0.1f, 0, 0, "%.3f '");
				ImGui::Text("\t\t\t\tZ");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t\t\t\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].zRotate, 0.1f, 0, 0, "%.3f '");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(0.0f, 20.0f));

				ImGui::Text("\t\tScale  \t\t");
				ImGui::SameLine();
				if (ImGui::Button("reset ", ImVec2(40, 25))) m_Scene->m_ObjectArray[m_Scene->m_Selected].ResetScale();
				ImGui::Text("\t\t\t\tX");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t\t\t\t\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].xScale, 0.01f, 0, 0, "%.3f x");
				ImGui::Text("\t\t\t\tY");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t\t\t\t\t\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].yScale, 0.01f, 0, 0, "%.3f x");
				ImGui::Text("\t\t\t\tZ");
				ImGui::SameLine();
				ImGui::DragFloat("\t\t\t\t\t\t\t\t\t", &m_Scene->m_ObjectArray[m_Scene->m_Selected].zScale, 0.01f, 0, 0, "%.3f x");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(0.0f, 60.0f));
			}
	}

	inline void AppLayer::DrawColorEditor()
	{
		if (m_Scene->m_Selected >= 0)
			if (ImGui::CollapsingHeader("Vertex Color", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorPicker3("Color", &m_Scene->m_ObjectArray[m_Scene->m_Selected].m_Colors[m_Selected * 3], ImGuiColorEditFlags_DefaultOptions_);

				ImGui::Dummy(ImVec2(0.0f, 15.0f));
				ImGui::BulletText("Vertices");
				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				const char* names[8] = { "\tVertex1", "\tVertex2", "\tVertex3", "\tVertex4", "\tVertex5", "\tVertex6", "\tVertex7", "\tVertex8" };
				for (int n = 0; n < 8; n++)
				{
					if (ImGui::Selectable(names[n], m_Selected == n))
					{
						m_Selected = n;
					}
				}		
			}
	}
}