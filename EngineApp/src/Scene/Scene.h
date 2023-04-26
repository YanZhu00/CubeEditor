#pragma once
#include "Hazel.h"
#include "Object/Cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Scene
{
public:
	Scene();
	void Draw();
	void DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
	void DrawGrid(int range = 100);
	void OnUpdate(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::mat4 CameraModelMatrix, bool left);

	void CreateCube();
	void DeleteCube(int index);

	std::shared_ptr<Hazel::Shader> m_Shader;
	std::vector<Cube> m_ObjectArray;
	int m_Selected;
	int m_LastSelected;
	//Hazel::Shader* m_Shader;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_CameraModelMatrix;
	bool b_left;

	std::vector<float> m_CorVertex;
	std::vector<float> m_CorColor;
	std::vector<unsigned int> m_CorIndice;

};