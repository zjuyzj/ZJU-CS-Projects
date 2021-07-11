/*
 * 文件：Sort.h
 * ---------------------------
 * 这个文件申明了各排序函数及找出最受欢迎图书、借书最多用户的函数 
 * 编写时间：2020.05.31
 */
 
#ifndef _SORT_H_
#define _SORT_H_  
   
/*图书排序函数 (flag==0为升序，flag==1为降序)*/
void book_sort(int flag);

/*用户排序函数 (flag==0为升序，flag==1为降序)*/
void user_sort(int flag); 

/*找出借阅次数最多的五本书*/
struct bookInformation *SORT_book(void);

/*找出借书最多的五名读者*/
struct userInformation *SORT_user(void);

#endif
