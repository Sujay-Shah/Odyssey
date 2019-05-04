//
//  RandomNumberGenerator.h
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//
typedef std::chrono::high_resolution_clock myclock;
#pragma once
class RandomNumberGenerator
{
public:
	RandomNumberGenerator();
	~RandomNumberGenerator();

	/*static float GetRandomNumber();
	static float GetRandomNumber1();
	static float GetRandomNumber2();
	static int GetIntRandomNumber3();
	static float GetRandomNumber4();
	static float GetRandomNumber5();
	static float GetRandomNumber6();
	static float GetRandomNumber7();
	static float GetRandomNumber8();
	static float GetRandomNumber9();
	static float GetRandomNumber10();*/
	float GetRandomNumberBetween(const float &min , const float &max);
	void Initialize();
private:
	myclock::time_point beginning;
	std::mt19937_64 RNGen;
	std::seed_seq seq;
	std::uniform_real_distribution<float> myrandom;
};

