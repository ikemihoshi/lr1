#include "StateFileHelper.h"
#include "CStruct.h"

bool StateFileHelper::BeginWrite(const char* fileName)
{
	_pf = fopen(fileName,"w");
	if(_pf == NULL){
		return false;
	}
	return true;
}
void StateFileHelper::NewLine(int tabnum)
{
	fputs(_strBuf.c_str(),_pf);
	fputs("\n",_pf);
	_strBuf = "";
	for(int i = 0; i < tabnum; i++)
	{
		_strBuf+='\t';
	}
}
void StateFileHelper::AddString(String str)
{
//	_strBuf = _strBuf + str + " ";
	_strBuf += str + " ";
}
void StateFileHelper::AddInt(int i)
{
	_strBuf = _strBuf + i + " ";
}
void StateFileHelper::EndWrite()
{
	if(_pf != NULL)
	{
		NewLine();
		fclose(_pf);
		_pf = NULL;
	}
}

bool StateFileHelper::BeginRead(const char* fileName)
{
	_pf = fopen(fileName,"r");
	if(_pf == NULL){
		return false;
	}
	_bufSize = 128;
	_szBuf = new char[_bufSize+1];
	return true;
}
void StateFileHelper::GetLine()
{
	_bufIndex = 0;
	int i = 0;
	_szBuf[i] = '\0';
	i = 0;
	bool isFirstRead = true;
	while(1)
	{
		int ch = fgetc(_pf);
		if(isFirstRead)
		{
			if(ch == '\t')
			{
				continue;
			}
			else
			{
				isFirstRead = false;
			}
		}

		if(ch == EOF)
		{
			_szBuf[i] = '\0';
			break;
		}
		if(ch == '\n')
		{
			if(_szBuf[i-1] == '\r')
			{
				_szBuf[i-1] = '\0';
				break;
			}
			_szBuf[i] = '\0';
			break;
		}
		_szBuf[i] = ch;
		i++;
		if(i >= _bufSize)
		{
			_bufSize = resize(&_szBuf, _bufSize);
		}
	}
}
bool StateFileHelper::GetString(String &str)
{
	int oldIndex = _bufIndex;
	int i = oldIndex;
	while(_szBuf[i] != ' ')
	{
		i++;
	}
	_szBuf[i] = '\0';
	str = _szBuf + oldIndex;
	_bufIndex = i + 1;
	return true;
}
bool StateFileHelper::GetInt(int &iout)
{
	int oldIndex = _bufIndex;
	int i = oldIndex;
	while(_szBuf[i] != ' ')
	{
		i++;
	}
	_szBuf[i] = '\0';
	sscanf(_szBuf + oldIndex, "%d", &iout);
	_bufIndex = i + 1;
	return true;
}
void StateFileHelper::EndRead()
{
	if(_pf != NULL)
	{
		delete[] _szBuf;
		_szBuf = NULL;
		fclose(_pf);
		_pf = NULL;
	}
}