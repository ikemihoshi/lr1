#include"BufferIn.h"

BufferIn::BufferIn(const char* szBuf,bool isFilePath)
{
	this->szBuf[0] = this->szBuf[_MAX_BUF_LEN] = '\0';
	this->pf = NULL;
	szDataBuf = NULL;
	Init(szBuf,isFilePath);
	return ;
}
BufferIn::BufferIn()
{
	this->szBuf[0] = this->szBuf[_MAX_BUF_LEN] = '\0';
	this->pf = NULL;
	szDataBuf = NULL;
	this->iBase = this->iMax = 0;
	err = 0;
	iFileSize = 0;
	return ;
}
BufferIn::~BufferIn()
{
	if(pf)
	{
		fclose(pf);
		pf = NULL;
	}
	if(szDataBuf)
	{
		delete[]szDataBuf;
		szDataBuf = NULL;
	}
}
unsigned int BufferIn::GetFileSize()
{
	return iFileSize;
}
bool BufferIn::Init(const char* szBuf,bool isFilePath)
{
	if(szBuf == NULL)
	{
		err = ERROR_INVALID_POINTER;
		return false;
	}
	this->BufferIn::~BufferIn();
	iFileSize = 0;
	this->iBase = this->iMax = 0;
	err = 0;
	isFile = isFilePath;
	if(isFilePath)
	{
		pf = fopen(szBuf , "rb");
		if(pf == NULL)
		{
			err = ERROR_OPEN_FILE_ERROR;
			return false;
		}
		fseek(pf,0,SEEK_END);
		iFileSize = ftell(pf);
		fseek(pf,0,SEEK_SET);
		iMax += fread(this->szBuf,1,_MAX_BUF_LEN,pf);
	}
	else 
	{
		iFileSize = iMax = strlen(szBuf);
		szDataBuf = new char[iMax+1];
		strcpy(szDataBuf,szBuf);
	}
	return true;
}
char& BufferIn::operator[](const unsigned int Index)
{
	if(Index >= iFileSize)
	{
		return szBuf[_MAX_BUF_LEN];
	}
	if(!isFile)
	{
		if(szDataBuf)
		{
			if(Index >= iMax)
			{
				return szBuf[_MAX_BUF_LEN];
			}
			else
			{
				return szDataBuf[Index];
			}
		}
		else
		{
			return szBuf[_MAX_BUF_LEN];
		}
	}
	if(err || !pf)
	{
		return szBuf[_MAX_BUF_LEN];
	}
	if(Index < iBase || (Index > iMax && Index - iMax >= _MAX_BUF_LEN / 2))		//重定位
	{
		int iTmp  = Index >= _MAX_BUF_LEN / 2 ? Index - _MAX_BUF_LEN / 2 : 0 ;
		if(!fseek(pf,iTmp,SEEK_SET))
		{
			iMax = iBase = iTmp;
			iMax += fread(this->szBuf,1,_MAX_BUF_LEN,pf);
			if(iMax > Index)
			{
				return szBuf[Index - iBase];
			}
			return szBuf[_MAX_BUF_LEN];
		}
		return szBuf[_MAX_BUF_LEN];
	}
	else if(Index >= iMax)		//小跳跃,仅仅再读入一次
	{
		if(iMax - iBase < _MAX_BUF_LEN || feof(pf))	//到文件结尾了,已经不能再读了
		{
			return szBuf[_MAX_BUF_LEN];
		}
		memcpy(szBuf , szBuf + _MAX_BUF_LEN / 2 , _MAX_BUF_LEN / 2);
		iBase += _MAX_BUF_LEN / 2;
		iMax += fread(szBuf + _MAX_BUF_LEN / 2 , 1 , _MAX_BUF_LEN / 2 , pf);
		if(iMax > Index)
		{
			return szBuf[Index - iBase];
		}
		return szBuf[_MAX_BUF_LEN];
	}
	return szBuf[Index - iBase];
}
int BufferIn::isBad()
{
	return err;
}
string BufferIn::GetString(const unsigned int index , const unsigned int iLen)
{
	string str = "";
	unsigned int i = 0;
	for(; i < iLen && index + i < iMax; i++)
	{
		str = str + (*this)[i+index];
	}
	return str;
}