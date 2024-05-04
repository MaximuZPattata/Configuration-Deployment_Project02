#pragma once
#include "defs.h"

class cLibrary
{
public:
	cLibrary(sLanguageDef* language);
	~cLibrary();

	const std::string GetValue(const std::string& key);

private:
	sLanguageDef* mLanguage;
};

