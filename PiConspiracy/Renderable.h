//
//  Renderable.h
//  PiEngine
//
//  Created by PiConspiracy on 11/18/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
#pragma once

class Renderable
{
public:
                                Renderable();
    virtual                     ~Renderable();

    virtual inline void         SetModelMatrix(const mat4 a_mModelMatrix);
	virtual inline mat4			GetModelMatrix() const;
    
	virtual void                Draw(Material* a_pMaterial) = 0;

	inline bool					IsRenderable();
	inline void					SetRenderable(bool a_bIsRenderable);

	// for memory manager use, 
	inline void SetActiveFlag(bool a_activeFlag) { m_activeFlag = a_activeFlag; }
	inline bool GetActiveFlag(void) { return m_activeFlag; }

protected:

	bool						m_bIsRenderable;
    mat4                        m_ModelMatrix;
	bool						m_activeFlag;
};

inline void Renderable::SetModelMatrix(const mat4 a_mModelMatrix)
{
    m_ModelMatrix = a_mModelMatrix;
}

inline mat4 Renderable::GetModelMatrix() const
{
	return m_ModelMatrix;
}

inline bool Renderable::IsRenderable()
{
	return m_bIsRenderable;
}

inline void Renderable::SetRenderable(bool a_bIsRenderable)
{
	m_bIsRenderable = a_bIsRenderable;
}