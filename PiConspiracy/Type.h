//
//  Type.h
//  PiEngine
//
//  Created by PiConspiracy on 10/25/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <functional>


using uint8     = unsigned char;
using uint16    = unsigned short;
using uint32    = unsigned int;
using uint64    = unsigned long long;
using int8      = signed char;
using int16     = short;
using int32     = int;
using int64     = long long;


#ifdef _WIN64
using ulong = unsigned long long;
#else 
using ulong = unsigned long;
#endif

using vec2	= glm::vec2;
using vec3	= glm::vec3;
using vec4	= glm::vec4;
using mat3	= glm::mat3;
using mat4	= glm::mat4;

// For Color
using Color3 = vec3;
using Color4 = vec4;

using string = std::string;

using std::cout;

template <class Type, class Allocator = std::allocator<Type> >
using vector = std::vector<Type, Allocator>;

template < class Type, class Allocator = std::allocator<Type> >
using list = std::list<Type, Allocator>;

template <
	class Key,
	class Type,
	class Hash = std::hash<Key>,
	class KeyEqual = std::equal_to<Key>,
	class Allocator = std::allocator<std::pair<const Key, Type> > >
	using unordered_map = std::unordered_map<Key, Type, Hash, KeyEqual, Allocator>;

static void InterpolateTowards(float& a_fX, float& a_fVelocity, float a_fTargetX, float a_fDamping, float a_fAngularFrequency, float a_fDeltaTime)
{
	const float f = 1.0f + 2.0f * a_fDeltaTime * a_fDamping * a_fAngularFrequency;
	const float freq_sqrd_dt = a_fAngularFrequency * a_fAngularFrequency * a_fDeltaTime;
	const float freq_sqrd_dt_sqrd = freq_sqrd_dt * a_fDeltaTime;

	const float det_inv = 1.0f / (f + freq_sqrd_dt_sqrd);
	const float det_x = f * +a_fDeltaTime * a_fVelocity + freq_sqrd_dt_sqrd * a_fTargetX;
	const float det_v = a_fVelocity + freq_sqrd_dt * (a_fTargetX - a_fX);

	a_fX = det_x * det_inv;
	a_fVelocity = det_v * det_inv;
}

static void InterpolateTowards(vec3& a_vPosition, vec3& a_vVelocity, glm::vec3& target, float damping, float angular_freq, float a_fDeltaTime)
{
	InterpolateTowards(a_vPosition.x, a_vVelocity.x, target.x, damping, angular_freq, a_fDeltaTime);
	InterpolateTowards(a_vPosition.y, a_vVelocity.y, target.y, damping, angular_freq, a_fDeltaTime);
	InterpolateTowards(a_vPosition.z, a_vVelocity.z, target.z, damping, angular_freq, a_fDeltaTime);
}