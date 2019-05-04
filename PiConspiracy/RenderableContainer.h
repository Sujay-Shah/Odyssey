//
//  RenderableContainer.h
//  PiEngine
//
//  Created by PiConspiracy on 11/18/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#include "Renderable.h"

class RenderableContainer
{
    friend class Renderer;
public:
                                RenderableContainer();
                                ~RenderableContainer();

    bool                        Initialize();
    bool                        Finalize();
	bool                        IsEmpty() { return m_Renderables.empty(); }

    void                        PushRenderable(Renderable* a_pRenderable);
    Renderable*                 PopRenderable();
private:

    std::deque<Renderable*>     m_Renderables;

	//for safe thread writing
	std::shared_mutex m_mutex;
};

