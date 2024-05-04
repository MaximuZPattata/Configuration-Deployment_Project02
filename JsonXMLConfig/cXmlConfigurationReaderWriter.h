#pragma once
#include "iConfigurationReaderWriter.h"
#include <pugixml/pugixml.hpp>

// This class is created to read from and write to XML files

class cXmlConfigurationReaderWriter : public iConfigurationReaderWriter
{
public: 

	virtual ~cXmlConfigurationReaderWriter();

	virtual bool ReadConfig(const std::string& filePath, sConfigDef& configOut, sPreferencesDef& prefOut,
		std::vector<sLanguageDef>& languageOut);
	
	virtual bool WriteConfig(const std::string& filePath,
		sConfigDef configOut, sPreferencesDef& prefOut, std::string writeTo, std::string overwriteMessage);

private:

		pugi::xml_document ReadFile(std::string fileName);
};

