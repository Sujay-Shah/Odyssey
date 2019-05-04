//
//  JsonReader.cpp
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#include "PiEnginePreCompiled.h"
#include "JsonReader.h"
#include <nlohmann/json.hpp>
#include <fstream>


namespace JsonReader
{


	float ParseFloat(const json& j, const String& first)
	{
		if (j.find(first) != j.end())
				return j[first];

		return 0.0f;
	}

	float ParseFloat(const json& j, const String& first, const String& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0.0f;
	}

	float ParseFloat(const json& j, const String& first, const String& second, const String& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0.0f;
	}

	double ParseDouble(const json& j, const String& first)
	{
		if (j.find(first) != j.end()) return j[first];
		return 0.0;
	}

	double ParseDouble(const json& j, const String& first, const String& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0.0;
	}

	double ParseDouble(const json& j, const String& first, const String& second, const String& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0.0;
	}

	int ParseInt(const json& j, const String& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return 0;
	}

	int ParseInt(const json& j, const String& first, const String& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0;
	}

	int ParseInt(const json& j, const String& first, const String& second, const String& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0;
	}

	String ParseString(const json& j, const String& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return "";
	}

	String ParseString(const json& j, const String& first, const String& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return "";
	}

	String ParseString(const json& j, const String& first, const String& second, const String& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return "";
	}

	bool ParseBool(const json& j, const String& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return false;
	}

	bool ParseBool(const json& j, const String& first, const String& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return false;
	}

	bool ParseBool(const json& j, const String& first, const String& second, const String& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return false;
	}

	std::vector<String> ParseStringList(const json& j, const String& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return std::vector<String>();
	}

	std::vector<String> ParseStringList(const json& j, const String& first, const String& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return std::vector<String>();
	}

	std::vector<String> ParseStringList(const json& j, const String& first, const String& second, const String& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return std::vector<String>();
	}



	bool ValueExists(const json& j, const String& first)
	{
		return j.find(first) != j.end();
	}

	bool ValueExists(const json& j, const String& first, const String& second)
	{
		if (j.find(first) != j.end()) {
			return j[first].find(second) != j[first].end();
		}
		return false;
	}

	bool ValueExists(const json& j, const String& first, const String& second, const String& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return true;
			}
		}
		return false;
	}

	json OpenJsonFile(const String& fileName)
	{
		try {
			std::ifstream i(fileName);
			json j;
			i >> j;
			i.close();
			return j;
		}
		catch (nlohmann::json::parse_error &e) {
			std::cerr << e.what() << " in " + fileName << std::endl;
			return 0;
		}
	}

	int GetSceneIndex(const json & j, const String &str)
	{
		int i = 0;
		for (auto it = j.begin(); it != j.end(); ++it)
		{
			std::cout << /*it.key() <<*/ i<< " value : " << it.value() << std::endl;
		
			if (str==ParseString(it.value(), "sceneName"))
			{
				return i;
			}
			++i;
		}
		i = -1;
		return 0;
	}

}

	


