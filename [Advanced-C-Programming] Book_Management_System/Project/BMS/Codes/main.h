/****************************************************************
��Ŀ���ƣ�ͼ�����ϵͳ
****************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_

/*���볣�ñ�׼ͷ�ļ�*/
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <process.h>
#include <time.h>

/*ȫ���ִ��������� */
#define MAXLEN 100

/*���ݿ��ļ���С���ƣ�ȡ����ͼ��ݹ�ģ */
#define APPRANGE 30000000

/*�������ƫ�Ʋ��� */
#define PASSWD_OFFSET 3

/*ϵͳ���ýṹ�弰���� */
struct settings
{
	char adminPassward[21]; /*������������*/ 
	int autoSaveSwitch;
	int autoSaveTime;
	int musicSwitch;
	int musicRepeat;
	char musicPath[41]; /*��������·��*/ 
}mySettings; 

char currentPassword[9];

/*ϵͳ����*/
extern int borrowingDays; /*���������*/

extern int borrowingBooks; /*�����ı���*/


time_t timep;/*��̬ˢ�µ�ϵͳʱ��*/

#endif
