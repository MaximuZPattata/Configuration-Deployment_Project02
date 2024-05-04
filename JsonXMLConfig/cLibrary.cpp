#include "cLibrary.h"

cLibrary::cLibrary(sLanguageDef* language) : mLanguage(language)
{

}

cLibrary::~cLibrary()
{

}

const std::string cLibrary::GetValue(const std::string& key)
{
	//std::map<std::string, std::string>::iterator itKeyValue = std::find(mLanguage->KeyValues.begin(), mLanguage->KeyValues.end(), key);
	//return itKeyValue->second;

	if (mLanguage->KeyValues.find(key) != mLanguage->KeyValues.end())
		return mLanguage->KeyValues[key];
	else
	{
		std::string errorMessage = "Value pair not found for the key - " + key;
		return errorMessage;
	}
}
