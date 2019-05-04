//
//  Tags.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
struct GameObjectType
{
	enum Enum
	{
		None,
		Player,
		Obstacle,
		Enemy,
		Projectile,
		Ground,
		PickUp,
		UI,
		Menu,
		Button,
		Trigger,
		Breakable,
		HUD,
		SUN,
		SLOWMOTEXTURE,
		CheckBox,
		Num
	};
};

TYPEDEF_ENUM(GameObjectType);

static std::unordered_map<string, EGameObjectType> TAG_NAMES =
{
	{ "Player", EGameObjectType::Player },
	{"Obstacle", EGameObjectType::Obstacle},
	{"Enemy", EGameObjectType::Enemy},
	{"Projectile", EGameObjectType::Projectile},
	{"Ground", EGameObjectType::Ground},
	{"PickUp",EGameObjectType::PickUp},
	{"UI",EGameObjectType::UI},
	{"Trigger",EGameObjectType::Trigger},
	{"Button",EGameObjectType::Button},
	{"Menu",EGameObjectType::Menu},
	{"Breakable",EGameObjectType::Breakable},
	{"HUD",EGameObjectType::HUD},
	{"SUN",EGameObjectType::SUN},
	{"SLOWMOTEXTURE",EGameObjectType::SLOWMOTEXTURE},
	{"CheckBox",EGameObjectType::CheckBox},
	{"None", EGameObjectType::None}
};