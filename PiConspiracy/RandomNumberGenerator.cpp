//
//  RandomNumberGenerator.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "RandomNumberGenerator.h"
#include "PiEnginePreCompiled.h"
#include <random>


RandomNumberGenerator::RandomNumberGenerator()
{
	beginning = myclock::now();
	//myrandom(RNGen);
}


RandomNumberGenerator::~RandomNumberGenerator()
{
}

float RandomNumberGenerator::GetRandomNumberBetween(const float &min, const float &max) {

	std::uniform_real_distribution<float> myrandom(min, max);
	return myrandom(RNGen);
}

void RandomNumberGenerator::Initialize()
{
	myclock::duration d = myclock::now() - beginning;
	unsigned seedVal = d.count();
	LOG_DEBUG_ARG("seed {} ",seedVal);
	RNGen.seed(seedVal);
}

