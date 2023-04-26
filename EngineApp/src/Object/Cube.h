#pragma once
#include "Hazel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class Cube
{
public:
	Cube(int id);

	float xLoc;
	float yLoc;
	float zLoc;

	float xScale;
	float yScale;
	float zScale;

	float xRotate;
	float yRotate;
	float zRotate;

	bool m_Selected;

	std::vector<float> m_Colors;

	int m_id;
	std::string m_Name;

	void Init();
	void ResetLoc();
	void ResetScale();
	void ResetRot();
	void Draw(std::shared_ptr<Hazel::Shader> shader, glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::mat4 CameraModelMatrix, bool left);
	
};