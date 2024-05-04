#include "cJsonConfigurationReaderWriter.h"
#include <iostream>

//-----------------------------------Private functions-----------------------------------------------------------------

rapidjson::Document cJsonConfigurationReaderWriter::ReadFile(std::string fileName)
{
	using namespace rapidjson;

	Document docReadObj;
	char readBuffer[65536];
	FILE* filePointer = 0; // Initializing filePointer to read from opened file
	errno_t errorChecker;

	docReadObj.SetObject(); // docReadObj initialized to parse JSON data

	errorChecker = fopen_s(&filePointer, fileName.c_str(), "rb");

	if (errorChecker != 0)
	{
		std::cout << "Error while reading the file - " << fileName << std::endl;
		return NULL;
	}

	FileReadStream inputStream(filePointer, readBuffer, sizeof(readBuffer));

	// Parses JSON data from inputStream onto docReadObj 
	docReadObj.ParseStream(inputStream);

	fclose(filePointer);

	return docReadObj;
}

bool cJsonConfigurationReaderWriter::SetValue(const rapidjson::Value& jsonValue, std::string& stringOut)
{
	if (!jsonValue.IsString())
	{
		return false;
	}

	stringOut = jsonValue.GetString();

	return true;
}

bool cJsonConfigurationReaderWriter::ParseConfigDef(const rapidjson::Value& configObject, sConfigDef& configOut)
{
	using namespace rapidjson;

	bool result = true;

	if (!configObject.IsObject())
	{
		return false;
	}

	if (!SetValue(configObject["PreferenceFile"], configOut.PreferenceFileName))
	{
		result = false;
	}

	const Value& languageFileValue = configObject["LanguageFiles"];

	if (languageFileValue.IsArray())
	{
		for (int index = 0; index < languageFileValue.Size(); index++)
		{
			if (languageFileValue[index].IsString())
			{
				configOut.LanguageFileNames.push_back(languageFileValue[index].GetString());
			}
		}
	}

	return result;
}

bool cJsonConfigurationReaderWriter::ParseLanguageDef(const rapidjson::Value& configObject, sLanguageDef& languageDef)
{
	using namespace rapidjson;

	bool result = true;

	if (!configObject.IsObject())
	{
		return false;
	}

	std::vector<std::string> languageMapKeys = {"Greeting", "PreferenceMessage", "ChangePreferences",
		"Preference_1", "Preference_2", "Preference_3", "Preference_4", "WrongInput", "ExitProgram",
	"OverwriteUserLanguage", "OverwriteUserName", "OverwriteAnimal", "OverwritePersonality", "LanguageOption_1", "LanguageOption_2"};

	if (!SetValue(configObject["Language"], languageDef.LanguageName))
	{
		std::cout << "Json object to string conversion error" << std::endl;
		result = false;
	}

	for (int index = 0; index < languageMapKeys.size(); index++)
	{
		std::string indexString = languageMapKeys[index];

		if (!SetValue(configObject[indexString.c_str()], languageDef.KeyValues[indexString]))
		{
			std::cout << "Json object to string conversion error - LanguageDef" << std::endl;
			result = false;
		}
	}

	return result;
}

bool cJsonConfigurationReaderWriter::ParsePreferenceDef(const rapidjson::Value& configObject, sPreferencesDef& preferenceDef)
{
	using namespace rapidjson;

	bool result = true;

	if (!configObject.IsObject())
	{
		return false;
	}

	std::vector<std::string> preferenceMapKeys = { "UserLanguage", "Username", "FavoriteAnimal", "FavoritePersonality"};


	for (int index = 0; index < preferenceMapKeys.size(); index++)
	{
		std::string indexString = preferenceMapKeys[index];

		if (!SetValue(configObject[indexString.c_str()], preferenceDef.PrefsKeyValues[indexString]))
		{
			std::cout << "Json object to string conversion error - PreferenceDef" << std::endl;
			result = false;
		}
	}

	return result;
}


//-----------------------------------Public functions------------------------------------------------------------------

cJsonConfigurationReaderWriter::~cJsonConfigurationReaderWriter()
{

}

bool cJsonConfigurationReaderWriter::ReadConfig(const std::string& filePath,
	sConfigDef& configOut, sPreferencesDef& prefOut,
	std::vector<sLanguageDef>& languageOut)
{
	using namespace rapidjson;

	Document docObj;

	//--------Reading Config File-----------------------------------

	docObj.SetObject();
	docObj = ReadFile(filePath);

	if (docObj.HasParseError())
	{
		std::cerr << "Failed to parse JSON." << std::endl;
		return false;
	}

	ParseConfigDef(docObj, configOut);

	//--------Reading Preference File--------------------------------

	docObj.SetObject();
	docObj = ReadFile(configOut.PreferenceFileName);

	ParsePreferenceDef(docObj, prefOut);

	//--------Reading Language Files-----------------------------------

	if (languageOut.size() == 0) // Checking to see if the language array is empty, then read from file 
	{
		for (int index = 0; index < configOut.LanguageFileNames.size(); index++)
		{
			docObj.SetObject();
			docObj = ReadFile(configOut.LanguageFileNames[index]);

			sLanguageDef newLanguageDef;

			ParseLanguageDef(docObj, newLanguageDef);

			languageOut.push_back(newLanguageDef);
		}
	}

	return true;
}

bool cJsonConfigurationReaderWriter::WriteConfig(const std::string& filePath,
	sConfigDef configOut, sPreferencesDef& prefOut, std::string writeTo, std::string overwriteMessage)
{
	using namespace rapidjson;

	Document docWriteObj;

	//--------Reading Config File-----------------------------------

	docWriteObj = ReadFile(filePath);

	if (docWriteObj.HasParseError())
	{
		std::cerr << "Failed to parse JSON." << std::endl;
		return false;
	}

	ParseConfigDef(docWriteObj, configOut);

	//--------Reading Preference File--------------------------------

	docWriteObj.SetObject();
	docWriteObj = ReadFile(configOut.PreferenceFileName);

	ParsePreferenceDef(docWriteObj, prefOut);

	if (docWriteObj.HasParseError()) {
		std::cerr << "Failed to parse JSON." << std::endl;
		return false;
	}

	//--------Writing to Preference File--------------------------------

	Value& userLanguage = docWriteObj[writeTo.c_str()];
	userLanguage.SetString(overwriteMessage.c_str(), docWriteObj.GetAllocator());
	errno_t errorChecker;
	FILE* filePointer = 0;

	errorChecker = fopen_s(&filePointer, configOut.PreferenceFileName.c_str(), "wb");

	if (errorChecker != 0)
	{
		std::cout << "Error while writing to file - " << configOut.PreferenceFileName.c_str() << std::endl;
		return NULL;
	}

	char writeBuffer[65536];

	FileWriteStream outputStream(filePointer, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(outputStream);
	docWriteObj.Accept(writer);

	fclose(filePointer);

	return 0;
}