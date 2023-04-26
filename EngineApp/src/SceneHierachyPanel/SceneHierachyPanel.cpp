#include"SceneHierachyPanel.h"

namespace Hazel {

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierachy");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::Text("Scene Hierachy");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
		{
			ImGui::EndTable();
		}

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create New Entity"))
				m_Scene->CreateCube();

			ImGui::EndPopup();
		}
		
		for (int i = 0; i < m_Scene->m_ObjectArray.size(); i++)
		{
			DrawEntityNode(i);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_Scene->m_Selected = -1;
		}
		
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(int index)
	{
		ImGui::PushID(index);

		ImGuiTreeNodeFlags flags = ((m_Scene->m_Selected == index) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		std::string tag = "Cube";
		bool opened = ImGui::TreeNodeEx((void*)index, flags, m_Scene->m_ObjectArray[index].m_Name.c_str());
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
		{
			m_Scene->m_Selected = index;
		}
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				m_Scene->DeleteCube(index);
			ImGui::EndPopup();
		}
		if (opened)
		{
			ImGui::TreePop();
		}
		
		
		ImGui::PopID();

	}

}