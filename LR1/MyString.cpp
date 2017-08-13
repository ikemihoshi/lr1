#include"MyString.h"
const int String::BasicBufLen = 32;
String::String(const char *szBuf):string(szBuf == NULL ? "":szBuf)
{
}
String::String(string &str):string(str)
{
}
String::operator const char*()
{
	return this->c_str();
}
bool String::isEqual(const string & str)
{
	return *this == str;
}
String& String::operator +=(int iToAdd)
{
	this->AppendFormat("%d",iToAdd);
	return *this;
}
String & String::operator+=(unsigned int iToAdd)
{
	this->AppendFormat("%u", iToAdd);
	return *this;
}
String& String::operator +=(double dbToAdd)
{
	this->AppendFormat("%.13g",dbToAdd);
	return *this;
}
String& String::operator +=(__int64 i64ToAdd)
{
	this->AppendFormat("%I64d",i64ToAdd);
	return *this;
}
int String::getHashCode()
{
	int i = 0, j = 1, hashCode = 0;
	for(; i < length(); i++)
	{
		hashCode += j * this->at(i);
		j += 2;
	}
	hashCode |= (length() << 16) & 0x7fffffff;
	return hashCode;
}
int String::Format(const char* szFormat , ...)
{
	if(szFormat == NULL)
	{
		return -1;
	}
	this[0] = "";
	va_list vl;
	va_start(vl , szFormat);
	int iLenBase = BasicBufLen;
	char* szBuf = new char[iLenBase+1];
	int iRet = 0;
	while(1)
	{
		if((iRet = _vsnprintf(szBuf,iLenBase,szFormat,vl)) >= 0)
		{
			this->append(szBuf);
			break;
		}
		else
		{
			char *szBufTmp;
			try
			{
				szBufTmp = new char[iLenBase*2+1];
			}
			catch(...)
			{
				iRet = -1;
				break;
			}
			iLenBase *= 2;
			delete[]szBuf;
			szBuf = szBufTmp;
		}
	}
	delete[]szBuf;
	va_end(vl);
	return iRet;
}
String String::format(const char* szFormat , ...)
{
	String res = "";
	if(szFormat == NULL)
	{
		return res;
	}
	va_list vl;
	va_start(vl , szFormat);
	int iLenBase = BasicBufLen;
	char* szBuf = new char[iLenBase+1];
	int iRet = 0;
	while(1)
	{
		if((iRet = _vsnprintf(szBuf,iLenBase,szFormat,vl)) >= 0 && iRet < iLenBase)
		{
			res.append(szBuf);
			break;
		}
		else
		{
			char *szBufTmp;
			try
			{
				szBufTmp = new char[iLenBase*2+1];
			}
			catch(...)
			{
				break;
			}
			iLenBase *= 2;
			delete[]szBuf;
			szBuf = szBufTmp;
		}
	}
	delete[]szBuf;
	va_end(vl);
	return res;
}
int String::AppendFormat(const char* szFormat , ...)
{
	if(szFormat == NULL)
	{
		return -1;
	}
	va_list vl;
	va_start(vl , szFormat);
	int iLenBase = BasicBufLen;
	char* szBuf = new char[iLenBase+1];
	int iRet = 0;
	while(1)
	{
		if((iRet = _vsnprintf(szBuf,iLenBase,szFormat,vl)) >= 0 && iRet < iLenBase)
		{
			this->append(szBuf);
			break;
		}
		else
		{
			char *szBufTmp;
			try
			{
				szBufTmp = new char[iLenBase*2+1];
			}
			catch(...)
			{
				iRet = -1;
				break;
			}
			iLenBase *= 2;
			delete[]szBuf;
			szBuf = szBufTmp;
		}
	}
	delete[]szBuf;
	va_end(vl);
	return iRet;
}
String& String::operator +=(string & str)
{
	this->AppendFormat("%s" , str.c_str());
	return *this;
}
String& String::operator +=(String & str)
{
	this->AppendFormat("%s" , str.c_str());
	return *this;
}
String& String::operator +=(const char *str)
{
	if(str)
	{
		this->AppendFormat("%s",str);
	}
	return *this;
}

int String::replace(const char* szsi , const char* szdi , int iCount )
{
	if(szsi == NULL || szdi == NULL)
	{
		return 0;
	}
	String strNew = "";
	if(iCount < 0)
	{
		iCount = this->length();
	}
	int iStrLen = strlen(szsi);
	string::size_type iPos = 0;
	for(int i = 0 ; i < iCount ; i ++)
	{
		string::size_type iNewPos = this->find(szsi , iPos);
		if(iNewPos != this->npos)
		{
			if(iNewPos > iPos)
			{
				strNew += this->substr(iPos , iNewPos - iPos);
			}
			strNew += szdi;
			iPos = iNewPos + iStrLen;
		}
		else
		{
			if(this->length() > iPos)
			{
				strNew += this->substr(iPos , this->length() - iPos);
			}
			*this = String(strNew);
			return i;
		}
	}
	strNew += this->substr(iPos , this->length() - iPos);
	*this = String(strNew);
	return iCount;
}

int String::indexOf(String subStr)
{
	const char*p = strstr(c_str(), subStr.c_str());
	if (p == NULL) {
		return -1;
	}
	return p - c_str();
}

String& String::operator +=(char chToAdd)
{
	this->AppendFormat("%c" , chToAdd);
	return *this;
}
String& String::operator +=(unsigned char uchToAdd)
{
	this->AppendFormat("%c" , uchToAdd);
	return *this;
}
const unsigned char String::at(unsigned int uiPOs)
{
	if(uiPOs >= this->length())
	{
		return '\0';
	}
	return this->c_str()[uiPOs];
}
const char String::operator [](int index)
{
	return this->at(index);
}
const char String::operator [](unsigned int index)
{
	return this->at(index);
}
const char String::operator [](short index)
{
	return this->at(index);
}
const char String::operator [](unsigned short index)
{
	return this->at(index);
}
const char String::operator [](char index)
{
	return this->at(index);
}
const char String::operator [](unsigned char index)
{
	return this->at(index);
}
const char String::operator [](long index)
{
	return this->at(index);
}
const char String::operator [](unsigned long index)
{
	return this->at(index);
}
const char String::operator [](double index)
{
	return this->at(index);
}
const char String::operator [](float index)
{
	return this->at(index);
}
String String::operator +(int iToAdd)
{
	String str = *this;
	str += iToAdd;
	return str;
}
String String::operator+(unsigned int iToAdd)
{
	String str = *this;
	str += iToAdd;
	return str;
}
String String::operator +(char chToAdd)
{
	String str = *this;
	str += chToAdd;
	return str;
}
String String::operator +(unsigned char uchToAdd)
{
	String str = *this;
	str += uchToAdd;
	return str;
}
String String::operator +(double dbToAdd)
{
	String str = *this;
	str += dbToAdd;
	return str;
}
String String::operator +(const char*str)
{
	String strt = *this;
	if(str != NULL && str[0] != '\0')
	{
		strt += str;
	}
	return strt;
}
String String::operator +(string str)
{
	String strt = *this;
	if(str.length() > 0)
	{
		strt += str;
	}
	return strt;
}
String String::operator +(__int64 i64ToAdd)
{
	String str = *this;
	str += i64ToAdd;
	return str;
}
String String::operator +(String str)
{
	String strt = *this;
	if(str.length() > 0)
	{
		strt += str;
	}
	return strt;
}