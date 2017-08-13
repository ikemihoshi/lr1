#include<stdio.h>
#include<stdarg.h>

#ifndef _My_String_H_
#define _My_String_H_

#include<string>
using namespace std;

class String : public string
{
public:
	String(const char * szBuf = "");
	String(string &str);
	int Format(const char* szFormat , ...);
	int AppendFormat(const char* szFormat , ...);
	int replace(const char* szsi , const char* szdi , int iCount = -1);
	int indexOf(String subStr);
	bool isEqual(const string & str);
	int getHashCode();
	const unsigned char at(unsigned int uiPOs);

	static String format(const char* szFormat , ...);
	
	String& operator +=(char chToAdd);
	String& operator +=(unsigned char uchToAdd);
	String& operator +=(int iToAdd);
	String& operator +=(unsigned int iToAdd);
	String& operator +=(double dbToAdd);
	String& operator +=(__int64 i64ToAdd);
	String& operator +=(string & str);
	String& operator +=(String & str);
 	String& operator +=(const char *str);
	
	String operator +(int iToAdd);
	String operator +(unsigned int iToAdd);
	String operator +(char chToAdd);
	String operator +(unsigned char uchToAdd);
	String operator +(double dbToAdd);
	String operator +(const char*str);
	String operator +(string str);
	String operator +(__int64 i64ToAdd);
	String operator +(String str);
	
	const char operator [](int index);
	const char operator [](unsigned int index);
	const char operator [](short index);
	const char operator [](unsigned short index);
	const char operator [](char index);
	const char operator [](unsigned char);
	const char operator [](long index);
	const char operator [](unsigned long index);
	const char operator [](double index);
	const char operator [](float index);
	operator const char*();
		const static int BasicBufLen;
};

#endif