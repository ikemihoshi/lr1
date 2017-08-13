#include "MyString.h"

#ifndef _STATE_FILE_HELPER_H_
#define _STATE_FILE_HELPER_H_

class StateFileHelper
{
public:
	bool BeginWrite(const char* fileName);
	void NewLine(int tabnum = 0);
	void AddString(String str);
	void AddInt(int i);
	void EndWrite();

	bool BeginRead(const char* fileName);
	void GetLine();
	bool GetString(String &str);
	bool GetInt(int &i);
	void EndRead();
private:
	FILE* _pf;
	String _strBuf;
	char *_szBuf;
	unsigned int _bufSize;
	unsigned int _bufIndex;
};
#endif