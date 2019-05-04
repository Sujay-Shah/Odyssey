//
//  JsonReader.h
//  PiEngine
//
//  Created by PiConspiracy on 11/20/2018
//  Copyright 2018 DigiPen Institute of Technology. All rights reserved.
//

#pragma once
#include "nlohmann/json_fwd.hpp"
namespace JsonReader
{

using json = nlohmann::json;
typedef std::string String;

	float ParseFloat(const json& j, const String&);
	float ParseFloat(const json& j, const String&, const String&);
	float ParseFloat(const json& j, const String&, const String&, const String&);

	double ParseDouble(const json& j, const String&);
	double ParseDouble(const json& j, const String&, const String&);
	double ParseDouble(const json& j, const String&, const String&, const String&);

	int ParseInt(const json& j, const String&);
	int ParseInt(const json& j, const String&, const String&);
	int ParseInt(const json& j, const String&, const String&, const String&);

	String ParseString(const json& j, const String&);
	String ParseString(const json& j, const String&, const String&);
	String ParseString(const json& j, const String&, const String&, const String&);

	bool ParseBool(const json& j, const String&);
	bool ParseBool(const json& j, const String&, const String&);
	bool ParseBool(const json& j, const String&, const String&, const String&);

	std::vector<String> ParseStringList(const json& j, const String&);
	std::vector<String> ParseStringList(const json& j, const String&, const String&);
	std::vector<String> ParseStringList(const json& j, const String&, const String&, const String&);

	bool ValueExists(const json& j, const String&);
	bool ValueExists(const json& j, const String&, const String&);
	bool ValueExists(const json& j, const String&, const String&, const String&);
	
	json OpenJsonFile(const String& fileName);

	int GetSceneIndex(const json& j, const String&);
}
