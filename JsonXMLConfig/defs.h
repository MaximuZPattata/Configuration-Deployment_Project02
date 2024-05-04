#pragma once
#include <string>
#include <vector>
#include <map>

// Structure Definitions

struct sConfigDef
{
	std::string PreferenceFileName;
	std::vector<std::string> LanguageFileNames;
};

struct sLanguageDef
{
	std::string LanguageName;
	std::map<std::string, std::string> KeyValues;
};

struct sPreferencesDef
{
	std::map<std::string, std::string> PrefsKeyValues;
};