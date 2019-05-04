//
//  MathDefs.h
//  PiEngine
//
//  Created by PiConspiracy on 10/27/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
//#pragma once

#ifndef MATH_DEFINES_H
#define MATH_DEFINES_H

#define TEST_MODE 1
#define PASS "PASS"
#define FAIL "!!!!! FAIL !!!!!"
#define EPSILON 0.0001f
#define PI       3.14159265358979323846f   // pi
#define PI_2     1.57079632679489661923f   // pi/2
#define PI_4     0.785398163397448309616f  // pi/4

#define DEG_TO_RAD (3.14159265358979323846f / 180.0f)
#define RAD_TO_DEG (180.0f / 3.14159265358979323846f)

#define INFECT_RAND_MAX 0x7FFF
#define COLLISIONFLAF_MAX 0xffffff
#define DOUBLE_PRECISION
typedef float real;
typedef unsigned collision_flag;
#define REAL_MAX FLT_MAX
#define real_sqrt sqrt
#define real_abs fabs
#define real_sin sin
#define real_cos cos
#define real_exp exp
#define real_pow pow
#define real_fmod fmod
#define real_epsilon DBL_EPSILON

struct Point {
	float x, y;
};

typedef std::vector<Point> ControlPoints;

struct InterpolationItem {
	ControlPoints points;	
	float amplitude;	
};

static real cubeface[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                              {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

#endif
