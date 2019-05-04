//
//  ParticleRenderer.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "Renderable.h"

class Particle;

class ParticleRenderer : public Renderable
{
public:
	ParticleRenderer();
	~ParticleRenderer();

	virtual void SetModelMatrix(const mat4 a_mModelMatrix) override;
	virtual mat4 GetModelMatrix() const override;
	virtual void Draw(Material* a_pMaterial) override;

	bool Initialize(const GLchar* a_strShaderName, std::vector<Particle*>& particleList);
	void Update(float a_dt);
	std::vector<Particle*> m_Particles;
	mat4 parentTransformation = mat4(1);
	ParticleComponent* m_ownerSystem;
private:
	ShaderManager* m_particleShader;
	void _initQuad();
	void _updateModelViewMatrix(vec3 pos,float rotation,float scale, const mat4& viewMatrix);
	uint32 m_uVAO, m_uVBO, m_uIBO;
	mat4 m_projectionMatrix;
};

