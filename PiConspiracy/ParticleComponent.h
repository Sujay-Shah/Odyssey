//
//  ParticleComponent.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class ParticleRenderer;
class Particle;
class RandomNumberGenerator;
class ParticleTexture;

class ParticleComponent :
	public Component
{
	COMPONENT_DECLARE(ParticleComponent);
public:
	~ParticleComponent();

	// Inherited via Component
	virtual void Initialize() override;
	virtual void Update(float a_dTime) override;
	virtual void Serialize(const json &) override;
	virtual void HandleEvent(Event * a_pEvent) override;
	virtual void Deactivate(void) override;
	virtual void OverrideParams(const json &, float) override;

	void addParticle(Particle* p);

	virtual void ImguiUpdate(float a_dTime) override;

	bool m_isGlobalSpace;
	bool m_startSimulation;
private:
	//helper functions
	void _emitParticle(void);
	void _generateParticles(float);
	float _generateValue(float&,float&);
	float _generateRotation(void);
	glm::vec3 _generateRandomUnitVectorWithinCone(glm::vec3& coneDirection, float angle);
	glm::vec3 _generateRandomUnitVector(void);



	ParticleTexture* m_particleTexture;
	ParticleRenderer* m_particleRender;
	std::vector<Particle*> m_particleList;
	RandomNumberGenerator* rng_;
	
	string m_shaderName;
	int m_particleCount;
	
	float m_paricleLifeSpan;
	glm::vec3 m_systemPos;
	glm::vec3 m_particleVel;
	float m_particleRot;
	float m_particleScale;
	float m_gravityEffect;

	//particle config
	//bet 0 - 1 
	float m_velError;
	float m_scaleError;
	float m_lifeError;

	float m_avgSpeed;
	float m_particlesPerSecond;
	bool m_randomRotation;
	glm::vec3 m_direction;
	float m_directionDeviation;
	bool m_haveDirection;
};

