//
//  InputEnums.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "Macro.h"

struct MouseButton
{
	enum Enum
	{
		None = -1,

		Left,
		Right,
		Middle,

		NUM
	};
}; TYPEDEF_ENUM(MouseButton);

struct ControllerButton
{
	enum Enum
	{
		NONE = -1,

		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,

		ButtonBack,
		ButtonGuide,
		ButtonStart,

		LeftStick,
		RightStick,

		LeftShoulder,
		RightShoulder,

		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,

		NUM
	};
}; TYPEDEF_ENUM(ControllerButton);


struct ControllerAxis
{
	enum Enum
	{
		NONE = -1,

		AxisLeftX,
		AxisLeftY,

		AxisRightX,
		AxisRightY,

		AxisTriggerLeft,
		AxisTriggerRight,

		NUM
	};
}; TYPEDEF_ENUM(ControllerAxis);