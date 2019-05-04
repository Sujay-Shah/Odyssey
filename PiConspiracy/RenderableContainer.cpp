//
//  RenderableContainer.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/18/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "RenderableContainer.h"

RenderableContainer::RenderableContainer()
{

}


RenderableContainer::~RenderableContainer()
{

}

bool RenderableContainer::Initialize()
{
    
    return true;
}

bool RenderableContainer::Finalize()
{

    return true;
}

void RenderableContainer::PushRenderable(Renderable* a_pRenderable)
{
	if (!a_pRenderable->IsRenderable())
	{
		return;
	}
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
    if (a_pRenderable)
    {
        m_Renderables.push_front(/*std::move(*/a_pRenderable/*)*/);
    }
}

Renderable* RenderableContainer::PopRenderable()
{
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
    if (m_Renderables.empty())
    {
        return nullptr;
    }
    Renderable* pRenderable = /*std::move(*/m_Renderables.back()/*)*/;
    m_Renderables.pop_back();
    
	//pRenderable->SetRenderable(false);
    return pRenderable;
}
