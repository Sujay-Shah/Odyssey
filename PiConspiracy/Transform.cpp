//
//  Transform.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"

Transform::Transform(void)
	: Component(STRINGIFY(Transform))
	, m_rotation(glm::quat(m_eulerAngles))
	, m_scale(1.0f)
	, m_axis(vec3(1.0f, 0.0f, 0.0f))
	, m_eulerAngles(vec3(0.0))

{
}

Transform::~Transform(void)
{

}

// parameterized constructor
Transform::Transform(vec3 a_position, glm::quat a_rotation, float a_scale, float a_angle, vec3 a_axis, vec3 a_EulerAngles)
	: Component(STRINGIFY(Transform))
	, m_rotation(a_rotation)
	, m_position(a_position)
	, m_scale(a_scale)
	, m_axis(a_axis)
	, m_eulerAngles(a_EulerAngles)
{

}

void Transform::Initialize()
{

}

void Transform::Deactivate()
{
	//reset all the values
	m_owner = nullptr;
	m_activeFlag = false;
	m_scale = vec3(0);
	m_axis = m_position = vec3(0);
	m_modelingMatrix = mat4(0);
	m_rotation = glm::quat(0, 0, 0, 0);
	m_eulerAngles = vec3(0);
}

void Transform::OverrideParams(const json& j,float offset)
{
	if (ValueExists(j, "position")) {
		m_position.x = ValueExists(j["position"], "x") ? ParseFloat(j["position"], "x") : m_position.x;
		m_position.y = ValueExists(j["position"], "y") ? ParseFloat(j["position"], "y") : m_position.y;
		m_position.z = ValueExists(j["position"], "z") ? ParseFloat(j["position"], "z") : m_position.z;
		
	}
	if (ValueExists(j, "scale")) {
		m_scale.x = ValueExists(j["scale"], "x") ? ParseFloat(j["scale"], "x") : m_scale.x;
		m_scale.y = ValueExists(j["scale"], "y") ? ParseFloat(j["scale"], "y") : m_scale.y;
		m_scale.z = ValueExists(j["scale"], "z") ? ParseFloat(j["scale"], "z") : m_scale.z;
	}
	if (ValueExists(j, "angle")) {
		m_eulerAngles.x = ValueExists(j["angle"], "x") ? ParseFloat(j["angle"], "x") * PI / 180 : m_eulerAngles.x;
		m_eulerAngles.y = ValueExists(j["angle"], "y") ? ParseFloat(j["angle"], "y") * PI / 180 : m_eulerAngles.y;
		m_eulerAngles.z = ValueExists(j["angle"], "z") ? ParseFloat(j["angle"], "z") * PI / 180 : m_eulerAngles.z;

		EulerToQuat(m_rotation, m_eulerAngles);
	}
	m_position.z = m_position.z + offset;
}

void Transform::Update(float a_dt)
{
	mat4 m(1.0f);
	m_rotation = glm::quat(m_eulerAngles);
	//	m_rotation = glm::transpose(glm::toMat4(m_rotation));
	mat4 mat = glm::translate(m, m_position) * (glm::toMat4(m_rotation)) * glm::scale(mat4(1.0f), m_scale);
	m_modelingMatrix = mat;

}

void Transform::Serialize(const json & a_j)
{
	//TODO : check if value present,
	m_position.x = ParseFloat(a_j["position"], "x");
	m_position.y = ParseFloat(a_j["position"], "y");
	m_position.z = ParseFloat(a_j["position"], "z");

	m_eulerAngles.x = ParseFloat(a_j, "angle", "x") * PI / 180;
	m_eulerAngles.y = ParseFloat(a_j, "angle", "y") * PI / 180;
	m_eulerAngles.z = ParseFloat(a_j, "angle", "z") * PI / 180;

	m_scale.x = ParseFloat(a_j, "scale", "x");
	m_scale.y = ParseFloat(a_j, "scale", "y");
	m_scale.z = ParseFloat(a_j, "scale", "z");

	EulerToQuat(m_rotation, m_eulerAngles);
}

void Transform::HandleEvent(Event * a_pEvent)
{
}

void Transform::SetRotation(glm::quat& a_q)
{
	m_rotation = a_q;
}

void Transform::SetScale(vec3 a_scale)
{
	m_scale = a_scale;
}

void Transform::Scale(vec3 a_scale)
{
	m_scale = a_scale;
	glm::scale(m_modelingMatrix, a_scale);
}

void Transform::Rotate(float a_angle, vec3 a_axis) //change to quaternion
{
	glm::quat q;
	m_rotation = glm::rotate(q, a_angle, a_axis);
}

void Transform::Rotate(float a_angle)
{
	glm::quat q;
	m_rotation = glm::rotate(q, a_angle, m_axis);
}

void Transform::EulerToQuat(glm::quat & q1, vec3 euler)
{
	/*euler.x = euler.x *  PI / 180.0;
	euler.y = euler.y * PI / 180.0;
	euler.z = euler.z * PI / 180.0;*/

	float x = euler.x, y = euler.y, z = euler.z;
	float c1 = cos(x / 2);
	float c2 = cos(y / 2);
	float c3 = cos(z / 2);

	float s1 = sin(x / 2);
	float s2 = sin(y / 2);
	float s3 = sin(z / 2);

	q1.x = s1 * c2 * c3 + c1 * s2 * s3;
	q1.y = c1 * s2 * c3 - s1 * c2 * s3;
	q1.z = c1 * c2 * s3 + s1 * s2 * c3;
	q1.w = c1 * c2 * c3 - s1 * s2 * s3;
}

void Transform::Rotate(float a_angle, Axes::Enum a_axis)
{
	vec3 axis;
	glm::quat q;
	if (a_axis == Axes::xAxis) axis = vec3(1.0f, 0.0f, 0.0f);
	else
		if (a_axis == Axes::yAxis) axis = vec3(0.0f, 1.0f, 0.0f);
		else
			if (a_axis == Axes::xAxis) axis = vec3(0.0f, 0.0f, 1.0f);

	m_rotation = glm::rotate(q, a_angle, axis);
}

void Transform::Translate(vec3 a_translation)
{
	this->m_position += a_translation;
}

void Transform::Translate(float a_x, float a_y, float a_z)
{
	this->m_position.x += a_x;
	this->m_position.y += a_y;
	this->m_position.z += a_z;
}

void Transform::Translate(float a_xyz)
{
	this->m_position.x = this->m_position.y = this->m_position.z = a_xyz;
}


