#include "Cube.h"

static const float Positions[] = {
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f
};

static const float VertexColors[] = {
	1.0f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 0.f, 1.f,
	1.0f,1.0f, 0.f,
	1.0f, 0.f, 1.0f,
	0.f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, 0.0f, 0.5f,
};

static const float SelectedColors[] = {
	1.0f, 1.f, 1.f,
	1.0f, 1.f, 1.f,
	1.0f, 1.f, 1.f,
	1.0f, 1.f, 1.f,
	1.0f, 1.f, 1.f,
	1.0f, 1.f, 1.f,
	1.0f, 1.f, 1.f,
	1.0f, 1.f, 1.f

};

unsigned int MeshIndice[]
{
	1, 0, 2,
	0, 2, 3,
	4, 5, 6,
	6, 7, 4,
	2, 1, 5,
	2, 5, 6,
	0, 3, 4,
	3, 4, 7,
	0, 1, 4,
	1, 4, 5,
	2, 3, 6,
	3, 6, 7
};


unsigned int EdgeIndice[] = {
	0, 1,
	1, 2,
	2, 3,
	0, 3,
	0, 4,
	1, 5,
	2, 6,
	3, 7,
	4, 5,
	5, 6,
	6, 7,
	7, 4
};


Cube::Cube(int id)
{
	m_id = id;
	m_Name = id? std::string("Cube") + std::to_string(id) : std::string("Cube");
	Init();
}

void Cube::Init()
{
	xLoc = 0.f;
	yLoc = 0.f;
	zLoc = 0.f;
	xScale = 1.f;
	yScale = 1.f;
	zScale = 1.f;
	xRotate = 0.f;
	yRotate = 0.f;
	zRotate = 0.f;
	m_Colors.clear();
	for (int i = 0; i < 24; ++i)
	{
		m_Colors.push_back(VertexColors[i]);
	}
	m_Selected = false;
}

void Cube::ResetLoc()
{
	xLoc = 0.0f;
	yLoc = 0.0f;
	zLoc = 0.0f;
}

void Cube::ResetScale()
{
	xScale = 1.0f;
	yScale = 1.0f;
	zScale = 1.0f;
}

void Cube::ResetRot()
{
	xRotate = 1.0f;
	yRotate = 1.0f;
	zRotate = 1.0f;
}



void Cube::Draw(std::shared_ptr<Hazel::Shader> shader, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::mat4 CameraModelMatrix, bool left)
{
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(xLoc, yLoc, zLoc));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)xRotate), glm::vec3(1, 0, 0));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)yRotate), glm::vec3(0, 1, 0));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)zRotate), glm::vec3(0, 0, 1));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(xScale, yScale, zScale));
	
	ModelMatrix = left ? CameraModelMatrix * ModelMatrix : ModelMatrix * CameraModelMatrix;
	glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
	shader->SetUniformMatrix4fv("MVP", &mvp[0][0]);

	Hazel::VertexArray vertexArray;
	Hazel::VertexBuffer vb_Positions(Positions, 3 * 8 * sizeof(float));
	Hazel::VertexBuffer vb_Colors(&m_Colors[0], 3 * 8 * sizeof(float));
	Hazel::VertexBufferLayout layout_Vertices;
	Hazel::VertexBufferLayout layout_Colors;
	Hazel::IndexBuffer indexBuffer(MeshIndice, 3 * 12);
	layout_Vertices.Push<float>(3);
	layout_Colors.Push<float>(3);
	vertexArray.AddBuffer(vb_Positions, layout_Vertices);
	vertexArray.AddBuffer(vb_Colors, layout_Colors);
	Hazel::Renderer::Draw(vertexArray, indexBuffer, *shader);

	if (m_Selected)
	{ 
		Hazel::VertexArray vertexArray1;
		Hazel::VertexBuffer vb_Positions1(Positions, 3 * 8 * sizeof(float));
		Hazel::VertexBuffer vb_Colors1(SelectedColors, 3 * 8 * sizeof(float));
		Hazel::VertexBufferLayout layout_Vertices1;
		Hazel::VertexBufferLayout layout_Colors1;
		Hazel::IndexBuffer indexBuffer1(EdgeIndice, 2 * 12);
		layout_Vertices1.Push<float>(3);
		layout_Colors1.Push<float>(3);
		vertexArray1.AddBuffer(vb_Positions1, layout_Vertices1);
		vertexArray1.AddBuffer(vb_Colors1, layout_Colors1);
		Hazel::Renderer::DrawLine(vertexArray1, indexBuffer1, *shader, 3);
	}
}

