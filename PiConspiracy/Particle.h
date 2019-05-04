//
//  Particle.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
class ParticleTexture;
class Particle
{
public:
	Particle();
	~Particle();
	
	void Initialize(ParticleTexture* pTex, glm::vec3 a_pos, glm::vec3 a_vel, float a_gravity, float a_lifetime, float a_rot, float a_scale);
	float GetScale(void) const;
	glm::vec3 GetPosition(void) const;
	float GetRotation(void) const;
	bool GetLiveStatus(void) const;
	ParticleTexture* GetParticleTexture(void)const;
	void Update(float dt);

	// for memory manager use
	inline void SetActiveFlag(bool a_activeFlag) { m_activeFlag = a_activeFlag; }
	inline bool GetActiveFlag(void) { return m_activeFlag; }

private:
	glm::vec3 position;
	glm::vec3 velocity;
	float gravityEffect; // <= 1.0
	float lifeLength;
	float rotation;
	float scale;
	float elapsedTime = 0.0f;
	float distance;
	bool isAlive;
	ParticleTexture* m_particleTex;

	bool m_activeFlag;

};

