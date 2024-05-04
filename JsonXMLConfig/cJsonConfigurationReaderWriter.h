#pragma once
#include "iConfigurationReaderWriter.h"

#include<rapidjson/filereadstream.h>
#include<rapidjson/filewritestream.h>
#include<rapidjson/writer.h>
#include<rapidjson/stringbuffer.h>
#include<rapidjson/document.h>

// This class is created to read from and write to Json files

class cJsonConfigurationReaderWriter : public iConfigurationReaderWriter
{
public:

	virtual ~cJsonConfigurationReaderWriter();

	virtual bool ReadConfig(const std::string& filePath,
		sConfigDef& configOut, sPreferencesDef& prefOut,
		std::vector<sLanguageDef>& languageOut);

	virtual bool WriteConfig(const std::string& filePath,
		sConfigDef configOut, sPreferencesDef& prefOut, std::string writeTo, std::string overwriteMessage);

private:

	bool SetValue(const rapidjson::Value& jsonValue, std::string& stringOut);

	bool ParseConfigDef(const rapidjson::Value& configObject, sConfigDef& configOut);

	bool ParseLanguageDef(const rapidjson::Value& configObject, sLanguageDef& languageDef);

	bool ParsePreferenceDef(const rapidjson::Value& configObject, sPreferencesDef& preferenceDef);

	rapidjson::Document ReadFile(std::string fileName);

};

