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

							//���ļ������ַ�����Ӧ��ĳһ������,ʹ�÷������Ǿ���ͳһ�Ľӿ�(�������),���Ҷ����ļ�,���������������ڴ�,ע��
							//��������ṩ���ķ���,д�ķ��������ᷴӳ���������ַ��������ļ�
class BufferIn
{
public:
	enum ErrorCode
	{
		ERROR_INVALID_POINTER = 10001,
		ERROR_OPEN_FILE_ERROR = 10002
	};
	BufferIn(const char* szBuf,bool isFilePath = false);		//��ʼ��
	BufferIn();
	~BufferIn();
	bool Init(const char* szBuf,bool isFilePath = false);
	char& operator[](const unsigned int Index);
	int isBad();
	unsigned int GetFileSize();
	string GetString(const unsigned int index , const unsigned int iLen);
private:
	char szBuf[_MAX_BUF_LEN+1];	//�ļ�����,���һ���ֽڲ���
	char *szDataBuf;			//������ַ���,��ά��һ�ο���
	int err;					//������
	unsigned int iBase;			//��ǰ���ڻ�ַ��Ӧ���ļ���ǰƫ��
	unsigned int iMax;			//��󻺴�
	bool isFile;				//��¼�Ƿ����ļ�
	FILE *pf;					//�ļ�ָ��(���)
	unsigned int iFileSize;		//��¼�ļ���С
};
#endif
