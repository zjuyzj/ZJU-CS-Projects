/*
 * 文件：search.h
 * ---------------------------
 * 各个搜索所用的函数 
 * 编写时间：2020.05.22
 */
 
#ifndef _SEARCH_H_
#define _SEARCH_H_

/*根据书号在图书信息链表里面搜索图书的函数,former指向搜索出的前一个指针*/
struct bookInformation* booknumberSearch(int number);

/*根据书号在用户借阅记录链表里面查找*/ 
struct user_borrowing_record* user_borrowing_record_search(int number, struct user_borrowing_record *q);

/*根据书号在图书用户关系链表里面查找*/
struct book_and_userRelation* book_and_userRelation_search(int number);

/*根据用户号在用户信息链表里面搜索用户的函数,former指向搜索出的前一个指针*/
struct userInformation* usernumberSearch(int number);

/*判断一个字符串是否含有另一个字符串，有，则返回1；无，则返回0*/
int includeJudge(char *string, char *key);

/*转小写*/ 
char *lowerChange(char *p);

/*根据书名在图书信息链表里模糊搜索,若无结果，则返回NULL；有，则返回该结构体*/
struct bookInformation* bookname_fuzzySearch(struct bookInformation* poiter, char *key);

/*根据作者名在图书信息链表里模糊搜索，若无结果，则返回NULL；有，则返回该结构体*/
struct bookInformation* author_fuzzySearch(struct bookInformation* poiter, char *key);

#endif
