//
//  ParticleComponent.cpp
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "ParticleComponent.h"
#include "ParticleRenderer.h"
#include "Particle.h"
#include "Renderer.h"
#include "RenderableContainer.h"
#include "ParticleTexture.h"
#include "ResourceManager.h"
#include "TextureData.h"
#include "MemoryManager.h"

ParticleComponent::ParticleComponent():Component(STRINGIFY(ParticleComponent)), m_gravityEffect(0.0f)
{
}


ParticleComponent::~ParticleComponent()
{
	//delete particle list
	/*SAFE_DELETE (rng_);
	SAFE_DELETE( m_particleTexture);
	SAFE_DELETE (m_particleRender);

	for (auto it : m_particleList)
	{
		SAFE_DELETE( it);
	}
	m_particleList.clear();*/
	Deactivate();
}

void ParticleComponent::Initialize()
{

	/*for (int i=0;i<m_particleCount;++i)
	{
		Particle* p = new Particle(m_systemPos,m_particleVel,m_gravityEffect,m_paricleLifeSpan,m_particleRot,m_particleScale);
		m_particleList.push_back(p);
	}*/
	rng_ = new RandomNumberGenerator();
	rng_->Initialize();
	m_startSimulation = false;
}

void ParticleComponent::Update(float a_dTime)
{
	if (!m_startSimulation)
	{
		return;
	}
	m_systemPos = m_owner->GetComponent<Transform>(STRINGIFY(Transform))->GetPosition();
	if (m_particleList.size()< m_particleCount)
	{
	//	return;
		_generateParticles(a_dTime);
	}
	
	for (int i=0;i<m_particleList.size();++i)
	{
		if (m_particleList[i]->GetLiveStatus())
		{
			m_particleList[i]->Update(a_dTime);
			//m_particleRender->Draw();
		}
		else
		{
			//remove particle from list, 
			//TODO:dangerous find alternative
			PiMemory->SetParticleBackInMemory(m_particleList[i]);
			m_particleList.erase(m_particleList.begin()+i);
			m_particleRender->m_Particles.erase(m_particleRender->m_Particles.begin()+i);
			//delete m_particleRender->m_Particles[i];
		}
	}
	//m_particleRender->SetRenderable(true);
	if (m_particleRender->IsRenderable())
	{
		RenderableContainer* pContainer = PiRenderer->GetRenderableContainer();
		pContainer->PushRenderable(m_particleRender);
	}
	m_particleRender->parentTransformation = glm::translate(mat4(1), m_systemPos);
}

void ParticleComponent::Serialize(const json &j)
{
	m_particleCount = ParseInt(j, "particleCount");
	m_paricleLifeSpan = ParseFloat(j, "particleLifeSpan");
	m_particleScale = ParseFloat(j, "particleScale");
	m_particleRot = ParseFloat(j, "particleRotation");
	m_gravityEffect = ParseFloat(j, "gravityEffect");
	m_shaderName = ParseString(j, "shaderName");
	//pos,vel

	//particle emission config
	m_particlesPerSecond = ParseFloat(j, "particlesPerSecond");
	m_randomRotation = ParseBool(j,"randomRotation");
	m_haveDirection = ParseBool(j, "haveDirection");
	m_isGlobalSpace = ParseBool(j, "isGlobalSpace");
	m_lifeError = ParseFloat(j, "lifeError");
	m_scaleError = ParseFloat(j, "scaleError");
	m_velError = ParseFloat(j, "velocityError");
	m_directionDeviation = ParseFloat(j, "directionDeviation");
	m_avgSpeed = ParseFloat(j, "speed");

	if (ValueExists(j,"direction"))
	{
		m_direction.x = ParseFloat(j["direction"],"x");
		m_direction.y = ParseFloat(j["direction"],"y");
		m_direction.z = ParseFloat(j["direction"],"z");
	}

	string textureName = ParseString(j, "textureName");
	m_particleTexture = new ParticleTexture;
	m_particleTexture->SetTextureID(PiResource->GetTexture(textureName)->GetTexData()->textureID);


	m_particleRender = new ParticleRenderer(); 
	m_particleRender->m_ownerSystem = this;
	//m_particleRender->SetRenderable(true);
	m_particleRender->Initialize(m_shaderName.c_str(), m_particleList);
}

void ParticleComponent::HandleEvent(Event * a_pEvent)
{
	if (a_pEvent->GetEventType() == EEventType::OnDeath)
	{
		m_startSimulation = true;
	}
	else if (a_pEvent->GetEventType() == EEventType::OnRestart || a_pEvent->GetEventType() == EEventType::Exit2Main)
	{
		m_startSimulation = false;
	}
}

void ParticleComponent::Deactivate(void)
{
	SAFE_DELETE(rng_);
	SAFE_DELETE(m_particleTexture);
	SAFE_DELETE(m_particleRender);
	
	m_particleList.clear();
	m_startSimulation = false;
}

void ParticleComponent::OverrideParams(const json &, float)
{
}

void ParticleComponent::addParticle(Particle * p)
{
	m_particleList.push_back(p);
	//TODO: remove this
	m_particleRender->m_Particles.push_back(p);
}

void ParticleComponent::ImguiUpdate(float a_dTime)
{
	START_WINDOW("Particle Params");
	PiImgui->AddSliderFloat("pps", 0, 10000.0f, m_particlesPerSecond);
	PiImgui->AddSliderInt("maxParticle", 0, 10000, m_particleCount);
	//PiImgui->AddSliderFloat("yOffset", -30.0f, 50.0f, m_offset.y);
	//PiImgui->AddSliderFloat("zOffset", 0.0f, 100.0f, m_offset.z);
	END_WINDOW;
}

void ParticleComponent::_emitParticle(void)
{
	glm::vec3 velocity;
	if (m_haveDirection) {
		velocity = _generateRandomUnitVectorWithinCone(m_direction, m_directionDeviation);
	}
	else {
		velocity = _generateRandomUnitVector();
	}
	velocity = glm::normalize(velocity);

	float speed = _generateValue(m_avgSpeed, m_velError);
	velocity *= speed;
	float scale = _generateValue(m_particleScale, m_scaleError);
	float lifeLength = _generateValue(m_paricleLifeSpan, m_lifeError);

	Particle* particle = PiMemory->GetParticleFromMemory();
	if (m_isGlobalSpace)
	{
		particle->Initialize(m_particleTexture, m_systemPos, velocity, m_gravityEffect, lifeLength, _generateRotation(), scale);
	}
	else
	{
		particle->Initialize(m_particleTexture,glm::vec3(0,0,0), velocity, m_gravityEffect, lifeLength, _generateRotation(), scale);
	}
	m_particleList.push_back(particle);
	m_particleRender->m_Particles.push_back(particle);
}

void ParticleComponent::_generateParticles(float dt)
{
	float particlesToCreate = m_particlesPerSecond * dt;
	float count = floor(particlesToCreate);
	for (int i = 0; i < count; i++) {
		_emitParticle();
	}
}

float ParticleComponent::_generateValue(float &val, float &errorMargin)
{
	float offset = (rng_->GetRandomNumberBetween(0.0f,1.0f) - 0.5f) * 2.0f * errorMargin;
	return val + offset;
}

float ParticleComponent::_generateRotation(void)
{
	if (m_randomRotation) {
		return rng_->GetRandomNumberBetween(0.0f, 1.0f) * 360.0f;
	}
	else {
		return 0;
	}
}

//credits :https://math.stackexchange.com/questions/56784/generate-a-random-direction-within-a-cone
glm::vec3 ParticleComponent::_generateRandomUnitVectorWithinCone(glm::vec3 & coneDirection, float angle)
{
	//pick random diection val on a uniformly distributed sphere
	// for point to be truly random, project sphere's area onto a cylinder
	float cosAngle = cos(angle);
	float theta = rng_->GetRandomNumberBetween(0.0f, 1.0f) * 2.0f * PI;
	float z = cosAngle + rng_->GetRandomNumberBetween(0.0f, 1.0f) * (1 - cosAngle);
	float rootOneMinusZSquared = sqrt(1 - z * z);
	float x = rootOneMinusZSquared * cos(theta);
	float y = rootOneMinusZSquared * sin(theta);

	//once we get direction,align north pole and cone axis
	glm::vec4 direction(x, y, z, 1);

	if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
		glm::vec3 k(0, 0, 1);
		glm::vec3 rotateAxis = glm::cross(coneDirection, k);
		rotateAxis = glm::normalize(rotateAxis);
		float value = glm::dot(coneDirection, k);
		float rotateAngle = acos(value);
		glm::mat4 unit = glm::mat4(1.0f);
		glm::mat4 rotationMatrix = glm::rotate(unit, -rotateAngle, rotateAxis);
		direction = rotationMatrix * direction;
	}
	else if (coneDirection.z == -1) {
		direction.z *= -1;
	}

	return glm::vec3(direction.x, direction.y, direction.z);
}

glm::vec3 ParticleComponent::_generateRandomUnitVector(void)
{
	float theta = rng_->GetRandomNumberBetween(0.0f, 1.0f) * 2.0f * PI;
	float z = rng_->GetRandomNumberBetween(0.0f, 1.0f) * 2.0f - 1.0f;
	float rootOneMinusZSquared = sqrtf(1.0f - z * z);
	float x = rootOneMinusZSquared * cos(theta);
	float y = rootOneMinusZSquared * sin(theta);
	return glm::vec3(x, y, z);
}
