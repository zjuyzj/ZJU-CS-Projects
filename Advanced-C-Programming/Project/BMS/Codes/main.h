/****************************************************************
项目名称：图书管理系统
****************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_

/*引入常用标准头文件*/
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <process.h>
#include <time.h>

/*全局字串长度限制 */
#define MAXLEN 100

/*数据库文件大小限制，取决于图书馆规模 */
#define APPRANGE 30000000

/*密码加密偏移参数 */
#define PASSWD_OFFSET 3

/*系统设置结构体及变量 */
struct settings
{
	char adminPassward[21]; /*存放密码的密文*/ 
	int autoSaveSwitch;
	int autoSaveTime;
	int musicSwitch;
	int musicRepeat;
	char musicPath[41]; /*背景音乐路径*/ 
}mySettings; 

char currentPassword[9];

/*系统参数*/
extern int borrowingDays; /*最长借阅天数*/

extern int borrowingBooks; /*最大借阅本数*/


time_t timep;/*动态刷新的系统时间*/

#endif
