#include <iostream>

#include "defs.h"
#include "cLibrary.h"
#include <string>

#define USE_JSON_CONFIG
//#define USE_XML_CONFIG

#ifdef USE_JSON_CONFIG
#include "cJsonConfigurationReaderWriter.h"
#endif 

#ifdef USE_XML_CONFIG
#include "cXmlConfigurationReaderWriter.h"
#endif 

iConfigurationReaderWriter* configReaderWriter = 0;

// This function is to print the intro statement and user options to the console
void LanguageIntro(std::vector<sLanguageDef> languageDef, std::string defaultLanguage, cLibrary* library, sPreferencesDef preferenceDef)
{
	for (int languageIndex = 0; languageIndex < languageDef.size(); languageIndex++)
	{
		if (languageDef[languageIndex].LanguageName == defaultLanguage)
			library = new cLibrary(&languageDef[languageIndex]);
	}

	std::cout << "<<--------------------------------------------------------------------------------->>" << std::endl;
	std::cout << library->GetValue("Greeting") << std::endl;
	std::cout << library->GetValue("PreferenceMessage") << std::endl << std::endl;
	std::cout << library->GetValue("Preference_1") << preferenceDef.PrefsKeyValues["UserLanguage"] << std::endl;
	std::cout << library->GetValue("Preference_2") << preferenceDef.PrefsKeyValues["Username"] << std::endl;
	std::cout << library->GetValue("Preference_3") << preferenceDef.PrefsKeyValues["FavoriteAnimal"] << std::endl;
	std::cout << library->GetValue("Preference_4") << preferenceDef.PrefsKeyValues["FavoritePersonality"] << std::endl << std::endl;
	std::cout << library->GetValue("ChangePreferences") << std::endl << std::endl;

	delete library;
}

int main()
{
//-----Initialize Objects and variables--------------------------------------------------

#ifdef USE_JSON_CONFIG
	configReaderWriter = new cJsonConfigurationReaderWriter();
	std::string fileName = "Config.json";
#endif

#ifdef USE_XML_CONFIG
	configReaderWriter = new cXmlConfigurationReaderWriter();
	std::string fileName = "Config.xml";
#endif

	sConfigDef configDef;
	sPreferencesDef preferenceDef;
	std::vector<sLanguageDef> languageDef;
	cLibrary* library = NULL;

	char userInput = NULL;
	std::string userOverwrite = "";
	bool exitLoop = false;
	std::string defaultLanguage = "English";
	int languageIndex = 0;

//----------------Loop starts-----------------------------------------------------------

	if (configReaderWriter->ReadConfig(fileName, configDef, preferenceDef, languageDef))
	{
		while (!exitLoop)
		{
			LanguageIntro(languageDef, defaultLanguage, library, preferenceDef);
			
			std::cin >> userInput;
			std::cout << std::endl;

			if (userInput == 'X' || userInput == 'x')
			{
				library = new cLibrary(&languageDef[languageIndex]);

				std::cout << library->GetValue("ExitProgram") << std::endl << std::endl;
				exitLoop = true;

				delete library;
			}
			else if (userInput == '1') // Change Language option
			{
				library = new cLibrary(&languageDef[languageIndex]);

				std::cout << library->GetValue("OverwriteUserLanguage") << std::endl;
				std::cout << library->GetValue("LanguageOption_1") << std::endl;
				std::cout << library->GetValue("LanguageOption_2") << std::endl << std::endl;

				std::cin >> userInput;
				std::cout << std::endl;

				if (userInput == '1') // English Selection
				{
					defaultLanguage = "English";
					languageIndex = 0;

					configReaderWriter->WriteConfig(fileName, configDef, preferenceDef, "UserLanguage", defaultLanguage);
					configReaderWriter->ReadConfig(fileName, configDef, preferenceDef, languageDef);
				}
				else if (userInput == '2') // French Selection
				{
					defaultLanguage = "French";
					languageIndex = 1;

					configReaderWriter->WriteConfig(fileName, configDef, preferenceDef, "UserLanguage", defaultLanguage);
					configReaderWriter->ReadConfig(fileName, configDef, preferenceDef, languageDef);
				}
				else // Wrong input given
				{
					std::cout << library->GetValue("WrongInput") << std::endl << std::endl;
					std::cout << library->GetValue("LanguageOption_1") << std::endl;
					std::cout << library->GetValue("LanguageOption_2") << std::endl << std::endl;
				}

				delete library;
			}
			else if (userInput == '2') // Change Username option
			{
				library = new cLibrary(&languageDef[languageIndex]);

				std::cout << library->GetValue("OverwriteUserName") << std::endl << std::endl;
				std::cin >> userOverwrite;

				configReaderWriter->WriteConfig(fileName, configDef, preferenceDef, "Username", userOverwrite);
				configReaderWriter->ReadConfig(fileName, configDef, preferenceDef, languageDef);

				delete library;
			}
			else if (userInput == '3') // Change Favorite Animal option
			{
				library = new cLibrary(&languageDef[languageIndex]);

				std::cout << library->GetValue("OverwriteAnimal") << std::endl << std::endl;
				std::cin >> userOverwrite;

				configReaderWriter->WriteConfig(fileName, configDef, preferenceDef, "FavoriteAnimal", userOverwrite);
				configReaderWriter->ReadConfig(fileName, configDef, preferenceDef, languageDef);

				delete library;
			}
			else if (userInput == '4') // Change Favorite Personality option
			{
				library = new cLibrary(&languageDef[languageIndex]);

				std::cout << library->GetValue("OverwritePersonality") << std::endl << std::endl;
				std::cin >> userOverwrite;

				configReaderWriter->WriteConfig(fileName, configDef, preferenceDef, "FavoritePersonality", userOverwrite);
				configReaderWriter->ReadConfig(fileName, configDef, preferenceDef, languageDef);

				delete library;
			}
			else // Wrong input given
			{
				library = new cLibrary(&languageDef[languageIndex]);

				std::cout << library->GetValue("WrongInput") << std::endl << std::endl;
				std::cout << library->GetValue("ChangePreferences") << std::endl << std::endl;

				delete library;
			}
		} // Loop ends
	}
	else
	{
		std::cout << "Cannot read file - " << fileName << std::endl;
	}

	return 0;
}