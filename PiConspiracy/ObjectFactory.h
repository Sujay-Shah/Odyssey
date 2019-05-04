//
//  ObjectFactory.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

class ObjectFactory 
{
public:
	ObjectFactory();
	~ObjectFactory();

	GameObject* CreateGameObject(string a_gameObjectName, bool);
	string m_belongsTo;
};

