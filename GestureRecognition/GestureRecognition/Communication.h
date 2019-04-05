#pragma once
#include "stdio.h"
#include <string>
#include <windows.h>

class Communication
{
public:
	Communication();
	~Communication();

	void Send(std::string message);
private:
	////Pipe Init Data
	//char buf[100];

	//HANDLE hPipe1, hPipe2;
	//LPTSTR lpszPipename1 = TEXT("\\\\.\\pipe\\myNamedPipe1");
	//LPTSTR lpszPipename2 = TEXT("\\\\.\\pipe\\myNamedPipe2");

	//DWORD cbWritten;
	//DWORD dwBytesToWrite = (DWORD)strlen(buf);

	////Thread Init Data
	//DWORD threadId;
	//HANDLE hThread = NULL;

	//BOOL Write_St = TRUE;
};

