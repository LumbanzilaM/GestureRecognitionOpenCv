#include "stdafx.h"
#include "Communication.h"


Communication::Communication()
{
	wchar_t* path = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &path);
	std::wstringstream ss;
	ss << path << "\\" << gestureFile;
	gesturePath = ss.str();
	CoTaskMemFree(static_cast<void*>(path));
}


Communication::~Communication()
{
}

void Communication::Send(std::string message)
{
	myfile.open(gesturePath, std::ofstream::out | std::ofstream::trunc);
	time_t result = time(NULL);
	char str[26];
	ctime_s(str, sizeof str, &result);
	std::string newMessage = message + "-" + str;
	myfile << newMessage << std::endl;
	myfile.close();
}

