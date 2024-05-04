#pragma once
#include "defs.h"

// This is an interface class to mediate between XML and JSON 

class iConfigurationReaderWriter
{
public:
	virtual ~iConfigurationReaderWriter() {}
	
	virtual bool ReadConfig(const std::string& filePath, 
		sConfigDef& configOut, sPreferencesDef& prefOut, 
		std::vector<sLanguageDef>& languageOut) = 0;
	
	virtual bool WriteConfig(const std::string& filePath,
		sConfigDef configOut, sPreferencesDef& prefOut, std::string writeTo, std::string overwriteMessage) = 0;
};

