//
//  CanvasType.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

//Pls dont change the order!
struct CanvasType
{
	enum Enum
	{
		MainMenu,
		PauseMenu,
		DeathMenu,
		Confirm,
		Instructions,
		Credits,
		Options,
		Num
	};
};

TYPEDEF_ENUM(CanvasType);
