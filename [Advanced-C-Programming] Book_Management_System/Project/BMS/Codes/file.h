/*
 * 文件：file.h
 * ---------------------------
 * 各个文件库的新增函数和统参数和密码保存及读取函数
 * 编写时间：2020.05.22、2020.05.25 
 */
 
#ifndef _FILE_H_
#define _FILE_H_  

/*图书信息库文件 "book_information.db"  链表数据存入文件函数*/ 
int bookDocument_add(void);

/*用户信息库文件 "user_information.db"  链表数据存入文件函数*/ 
int userDocument_add(void);

/*图书用户关系表 "book_user_information.db" 链表数据存入文件函数*/
int relationDocument_add(void);

/*图书信息库文件 "book_information.db"  数据加载入链表的函数*/ 
int bookDocument_load(void);

/*用户信息库文件 "user_information.db"  数据加载入链表的函数*/  
int userDocument_load(void);

/*图书用户关系表 "book_user_information.db"  数据加载入链表的函数*/  
int relationDocument_load(void);

void systemSettings_add(void);

void systemSettings_load(void);

int bookSort_add(void);

int userSort_add(void);

#endif
