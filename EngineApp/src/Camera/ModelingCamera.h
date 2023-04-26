#pragma once
#include "Hazel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class ModelingCamera
{
public:
	ModelingCamera();
	void Reset();
	void OnUpdate(float timestep, float width, float height);
	void Scroll(float offset);

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_CameraModelMatrix;
	bool b_left;

	double m_horizontalAngle;
	double m_verticalAngle;
	double m_SpeedHori, m_SpeedVerti;

	float m_FoV;
	float m_Distance;
	float m_xposPrevious, m_yposPrevious;
	float m_ScrollSpeed;
	bool b_selected;
};