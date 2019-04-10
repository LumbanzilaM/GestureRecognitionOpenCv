#pragma once
#include "stdio.h"
#include <string>
#include <windows.h>
#include <KnownFolders.h>
#include <shlobj.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>

class Communication
{
public:
	Communication();
	~Communication();

	void Send(std::string message);
private:
	std::wstring	gesturePath;
	std::wstring	tmpPath;
	std::ofstream	myfile;
	const char*		gestureFile = "gestures.txt";
};

