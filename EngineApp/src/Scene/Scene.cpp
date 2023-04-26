#include "Scene.h"

//Hazel::Shader m_Shader("assets/shaders/Basic.glsl");

Scene::Scene()
{
	m_Shader = std::make_shared<Hazel::Shader>("assets/shaders/Basic.glsl");
	//m_Shader = new Hazel::Shader("assets/shaders/Basic.glsl");

	m_ObjectArray.push_back(Cube(0));

	m_Selected = 0;
	m_LastSelected = 0;

	int range = 100;
	for (int i = -range; i <= range; ++i)
	{
	
		m_CorVertex.push_back(-range);
		m_CorVertex.push_back(0.);
		m_CorVertex.push_back(i);
		m_CorVertex.push_back(range);
		m_CorVertex.push_back(0.);
		m_CorVertex.push_back(i);

		m_CorVertex.push_back(i);
		m_CorVertex.push_back(0.);
		m_CorVertex.push_back((float)-range);
		m_CorVertex.push_back(i);
		m_CorVertex.push_back(0.);
		m_CorVertex.push_back((float)range);

		for (int j = 0; j < 12; ++j)
			m_CorColor.push_back(0.3);

		
	}
	for (int i = 0; i < m_CorVertex.size() / 3; i++)
		m_CorIndice.push_back(i);

}

void Scene::OnUpdate(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::mat4 CameraModelMatrix, bool left)
{
	m_ProjectionMatrix = ProjectionMatrix;
	m_ViewMatrix = ViewMatrix;
	m_CameraModelMatrix = CameraModelMatrix;
	b_left = left;
}

void Scene::CreateCube()
{
	auto iter = m_ObjectArray.begin();
	int count = 0;
	while (iter != m_ObjectArray.end())
	{
		if (iter->m_id != count)
		{
			break;
		}
		count++;
		iter++;
	}
	m_ObjectArray.emplace(iter, Cube(count));
	m_Selected = count;
}

void Scene::DeleteCube(int index)
{
	m_ObjectArray.erase(m_ObjectArray.begin() + index);
	m_Selected = -1;
}

void Scene::Draw()
{	
	DrawLine(-100, 0, 0, 100, 0, 0, 1, 0, 0);
	DrawLine(0, 0, 100, 0, 0, -100, 0, 1, 0);
	DrawGrid();
	
	for (int i = 0; i < m_ObjectArray.size(); i++)
	{
		m_ObjectArray[i].m_Selected = (i == m_Selected);
		m_ObjectArray[i].Draw(m_Shader, m_ProjectionMatrix, m_ViewMatrix, m_CameraModelMatrix, b_left);
	}
}

void Scene::DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
{
	std::vector<float> LineVertex;
	std::vector<float> LineColor;
	std::vector<unsigned int> LineIndice;
	int range = 100;
	LineVertex.push_back(x1);
	LineVertex.push_back(y1);
	LineVertex.push_back(z1);
	LineVertex.push_back(x2);
	LineVertex.push_back(y2);
	LineVertex.push_back(z2);

	LineColor.push_back(r);
	LineColor.push_back(g);
	LineColor.push_back(b);
	LineColor.push_back(r);
	LineColor.push_back(g);
	LineColor.push_back(b);

	LineIndice.push_back(0);
	LineIndice.push_back(1);

	Hazel::VertexArray vaLine;
	Hazel::VertexBuffer vbLineVertex(&LineVertex[0], 6 * sizeof(float));
	Hazel::VertexBuffer vbLineColor(&LineColor[0], 6 * sizeof(float));
	Hazel::VertexBufferLayout LineVertexLayout;
	Hazel::VertexBufferLayout LineColorLayout;
	Hazel::IndexBuffer idLine(&LineIndice[0], 2);
	LineVertexLayout.Push<float>(3);
	LineColorLayout.Push<float>(3);
	vaLine.AddBuffer(vbLineVertex, LineVertexLayout);
	vaLine.AddBuffer(vbLineColor, LineColorLayout);

	glm::mat4 ModelMatrix = b_left ? m_CameraModelMatrix * glm::mat4(1.0) : glm::mat4(1.0) * m_CameraModelMatrix;
	glm::mat4 mvp = m_ProjectionMatrix * m_ViewMatrix * ModelMatrix;
	m_Shader->SetUniformMatrix4fv("MVP", &mvp[0][0]);

	Hazel::Renderer::DrawLine(vaLine, idLine, *m_Shader);
}


void Scene::DrawGrid(int range)
{

	Hazel::VertexArray vaLine;
	Hazel::VertexBuffer vbLineVertex(&m_CorVertex[0], m_CorVertex.size() * sizeof(float));
	Hazel::VertexBuffer vbLineColor(&m_CorColor[0], m_CorColor.size() * sizeof(float));
	Hazel::VertexBufferLayout LineVertexLayout;
	Hazel::VertexBufferLayout LineColorLayout;
	Hazel::IndexBuffer idLine(&m_CorIndice[0], m_CorIndice.size());
	LineVertexLayout.Push<float>(3);
	LineColorLayout.Push<float>(3);
	vaLine.AddBuffer(vbLineVertex, LineVertexLayout);
	vaLine.AddBuffer(vbLineColor, LineColorLayout); 

	glm::mat4 ModelMatrix = b_left ? m_CameraModelMatrix * glm::mat4(1.0) : glm::mat4(1.0) * m_CameraModelMatrix;
	glm::mat4 mvp = m_ProjectionMatrix * m_ViewMatrix * ModelMatrix;
	m_Shader->SetUniformMatrix4fv("MVP", &mvp[0][0]);

	Hazel::Renderer::DrawLine(vaLine, idLine, *m_Shader);


}