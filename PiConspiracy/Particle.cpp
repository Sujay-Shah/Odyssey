//
//  Particle.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "Particle.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::Initialize(ParticleTexture * pTex, glm::vec3 a_pos, glm::vec3 a_vel, float a_gravity, float a_lifetime, float a_rot, float a_scale)
{
	m_particleTex = pTex;
	position = a_pos;
	velocity = a_vel;
	gravityEffect = a_gravity;
	lifeLength = a_lifetime;
	rotation = a_rot;
	scale = a_scale;
	elapsedTime = 0.0f;
	isAlive = true;
}

float Particle::GetScale(void) const
{
	return scale;
}

glm::vec3 Particle::GetPosition(void) const
{
	return position;
}

float Particle::GetRotation(void) const
{
	return rotation;
}

bool Particle::GetLiveStatus(void) const
{
	return isAlive;
}

ParticleTexture * Particle::GetParticleTexture(void) const
{
	return m_particleTex;
}

void Particle::Update(float dt)
{
	velocity.y += -9.8f * gravityEffect * dt;
	vec3 change = velocity ;
	position += change;
	elapsedTime += dt;

	if (elapsedTime > lifeLength)
	{
		isAlive = false;
	}
	return;
}
