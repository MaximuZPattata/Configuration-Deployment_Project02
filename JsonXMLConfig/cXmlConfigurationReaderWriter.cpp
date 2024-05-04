#include "cXmlConfigurationReaderWriter.h"
#include <iostream>

cXmlConfigurationReaderWriter::~cXmlConfigurationReaderWriter()
{

}

//-----------------------------------Private functions-----------------------------------------------------------------

pugi::xml_document cXmlConfigurationReaderWriter::ReadFile(std::string fileName) 
{
    using namespace pugi; 
    
    xml_document docReadObj;
    
    xml_parse_result errorCheck = docReadObj.load_file(fileName.c_str());

    if (!errorCheck) {
        std::cerr << "Error while reading the file - " << fileName << std::endl;
    }

    return docReadObj;
}

//-----------------------------------Public functions------------------------------------------------------------------

bool cXmlConfigurationReaderWriter::ReadConfig(const std::string& filePath, sConfigDef& configOut, sPreferencesDef& prefOut,
                std::vector<sLanguageDef>& languageOut) 
{
    using namespace pugi;

    //------------Reading Config Def-----------------------------------------------

    xml_document docReadObj = ReadFile(filePath);

    if (docReadObj.empty()) 
        return false;

    xml_node root = docReadObj.child("Root");

    configOut.PreferenceFileName = root.child_value("PreferenceFile");

    xml_node languageFilesNode = root.child("LanguageFiles");
    
    for (xml_node fileNode = languageFilesNode.child("File"); fileNode; fileNode = fileNode.next_sibling("File")) 
    {
        configOut.LanguageFileNames.push_back(fileNode.child_value());
    }

    //------------Reading Preference Def-----------------------------------------------

    docReadObj = ReadFile(configOut.PreferenceFileName);

    if (docReadObj.empty())
        return false;

    xml_node rootNode = docReadObj.child("UserInfo");

    for (xml_node child = rootNode.first_child(); child; child = child.next_sibling())
    {
        std::string childMapKey = child.name();
        std::string childMapValue = child.child_value();

        prefOut.PrefsKeyValues[childMapKey] = childMapValue;
    }

    //------------Reading Language Def--------------------------------------------------
    
    if (languageOut.size() == 0) // Checking to see if the language array is empty, then read from file 
    {
        for (int index = 0; index < configOut.LanguageFileNames.size(); index++)
        {
            docReadObj = ReadFile(configOut.LanguageFileNames[index]);

            if (docReadObj.empty())
                return false;

            sLanguageDef newLanguageDef;

            xml_node languageDataNode = docReadObj.child("LanguageData");

            newLanguageDef.LanguageName = languageDataNode.child_value("Language");
            
            for (xml_node child = languageDataNode.first_child(); child; child = child.next_sibling())
            {
                std::string childMapKey = child.name();
                std::string childMapValue = child.child_value();

                newLanguageDef.KeyValues[childMapKey] = childMapValue;
            }

            languageOut.push_back(newLanguageDef);
        }
    }

    return true;
}

bool cXmlConfigurationReaderWriter::WriteConfig(const std::string& filePath,
    sConfigDef configOut, sPreferencesDef& prefOut, std::string writeTo, std::string overwriteMessage)
{
    using namespace pugi;
    xml_document docWriteObj;
    
    //------------Reading Config Def-----------------------------------------------

    docWriteObj = ReadFile(filePath);

    if (docWriteObj.empty())
        return false;

    xml_node root = docWriteObj.child("Root");

    configOut.PreferenceFileName = root.child_value("PreferenceFile");

    xml_node languageFilesNode = root.child("LanguageFiles");

    for (xml_node fileNode = languageFilesNode.child("File"); fileNode; fileNode = fileNode.next_sibling("File"))
    {
        configOut.LanguageFileNames.push_back(fileNode.child_value());
    }

    //------------Reading Preference Def-----------------------------------------------

    docWriteObj = ReadFile(configOut.PreferenceFileName);

    if (docWriteObj.empty())
        return false;

    //------------Writing to Preference Def---------------------------------------------

    xml_node rootNode = docWriteObj.child("UserInfo");

    xml_node userLanguageNode = rootNode.child(writeTo.c_str());

    userLanguageNode.text().set(overwriteMessage.c_str());

    if (!docWriteObj.save_file(configOut.PreferenceFileName.c_str()))
    {
        return false;
    }

    return true;
}
