#include "stdafx.h"
#include "Communication.h"


Communication::Communication()
{
}


Communication::~Communication()
{
}

void Communication::Send(std::string message)
{
	char buf[100];
	HANDLE hPipe1;
	LPTSTR lpszPipename1 = TEXT("\\\\.\\pipe\\myNamedPipe1");
	DWORD cbWritten;
	DWORD dwBytesToWrite = (DWORD)strlen(buf);
	//Thread Init Data
	BOOL Write_St = TRUE;

	hPipe1 = CreateFile(lpszPipename1, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (message != "NG")
	{
		strcpy_s(buf, message.c_str());	// or pass &s[0]
		WriteFile(hPipe1, buf, message.size(), &cbWritten, NULL);
		memset(buf, 0xCC, 100);
	}
}

