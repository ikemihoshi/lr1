#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include<conio.h>
#include<math.h>
#include<string>
using namespace std;

#ifndef _BufferIn_H_
#define _BufferIn_H_

#define _MAX_BUF_LEN 4096

							//将文件或者字符串对应到某一个窗口,使得访问他们具有统一的接口(随机访问),并且对于文件,仅仅消耗少量的内存,注意
							//本类仅仅提供读的方法,写的方法并不会反映到真正的字符串或者文件
class BufferIn
{
public:
	enum ErrorCode
	{
		ERROR_INVALID_POINTER = 10001,
		ERROR_OPEN_FILE_ERROR = 10002
	};
	BufferIn(const char* szBuf,bool isFilePath = false);		//初始化
	BufferIn();
	~BufferIn();
	bool Init(const char* szBuf,bool isFilePath = false);
	char& operator[](const unsigned int Index);
	int isBad();
	unsigned int GetFileSize();
	string GetString(const unsigned int index , const unsigned int iLen);
private:
	char szBuf[_MAX_BUF_LEN+1];	//文件窗口,最后一个字节不用
	char *szDataBuf;			//如果是字符串,则维护一段拷贝
	int err;					//错误码
	unsigned int iBase;			//当前窗口基址对应的文件当前偏移
	unsigned int iMax;			//最大缓存
	bool isFile;				//记录是否是文件
	FILE *pf;					//文件指针(句柄)
	unsigned int iFileSize;		//记录文件大小
};
#endif
