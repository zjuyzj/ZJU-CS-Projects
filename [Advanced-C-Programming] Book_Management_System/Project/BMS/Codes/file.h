/*
 * �ļ���file.h
 * ---------------------------
 * �����ļ��������������ͳ���������뱣�漰��ȡ����
 * ��дʱ�䣺2020.05.22��2020.05.25 
 */
 
#ifndef _FILE_H_
#define _FILE_H_  

/*ͼ����Ϣ���ļ� "book_information.db"  �������ݴ����ļ�����*/ 
int bookDocument_add(void);

/*�û���Ϣ���ļ� "user_information.db"  �������ݴ����ļ�����*/ 
int userDocument_add(void);

/*ͼ���û���ϵ�� "book_user_information.db" �������ݴ����ļ�����*/
int relationDocument_add(void);

/*ͼ����Ϣ���ļ� "book_information.db"  ���ݼ���������ĺ���*/ 
int bookDocument_load(void);

/*�û���Ϣ���ļ� "user_information.db"  ���ݼ���������ĺ���*/  
int userDocument_load(void);

/*ͼ���û���ϵ�� "book_user_information.db"  ���ݼ���������ĺ���*/  
int relationDocument_load(void);

void systemSettings_add(void);

void systemSettings_load(void);

int bookSort_add(void);

int userSort_add(void);

#endif
