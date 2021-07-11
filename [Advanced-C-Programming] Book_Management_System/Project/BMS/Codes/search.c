/*
 * 文件：search.c
 * ---------------------------
 * 各个搜索所用的函数 
 * 编写时间：2020.05.22
 */
	 
#include "main.h"
#include "ds.h"
#include "search.h"

/*根据书号在图书信息链表里面搜索图书的函数,former指向搜索出的前一个指针*/
struct bookInformation* booknumberSearch(int number)
{
	bookformer=NULL;
	struct bookInformation *p;
	p=bookhead;
	while(p!=NULL)
	{
		if(p->bookNumber==number) break;
		else
		{
			bookformer=p;
			p=p->next;
		}
	}
	return p;
}

/*根据书号在用户借阅记录链表里面查找*/ 
struct user_borrowing_record* user_borrowing_record_search(int number, struct user_borrowing_record *q)
{
	struct user_borrowing_record *p;
	p=q;
	while(p!=NULL)
	{
		if(p->bookNumber==number) break;
		else
		{
			p=p->next;
		}
	}
	return p;
}

/*根据书号在图书用户关系链表里面查找*/
struct book_and_userRelation* book_and_userRelation_search(int number)
{
	relationformer=NULL;
	struct book_and_userRelation *p;
	p=relationhead;
	while(p!=NULL)
	{
		if(p->bookNumber==number) break;
		else
		{
			relationformer=p;
			p=p->next;
		}
	}
	return p;
}

/*根据用户号在用户信息链表里面搜索用户的函数,former指向搜索出的前一个指针*/
struct userInformation* usernumberSearch(int number)
{
	userformer=NULL;
	struct userInformation *p;
	p=userhead;
	while(p!=NULL)
	{
		if(p->userNumber==number) break;
		else
		{
			userformer=p;
			p=p->Next;
		}
	}
	return p;
}

/*判断一个字符串是否含有另一个字符串，有，则返回1；无，则返回0*/
int includeJudge(char *string, char *key)
{
	int stringlen, keylen;
	int i, j, k, f, flag;
	k=f=0;
	char p[31], q[31];
	strcpy(p, lowerChange(string));
	strcpy(q, lowerChange(key));
	stringlen=strlen(p);
	keylen=strlen(q);
	for(i=0; i<=stringlen-keylen; i++)
	{
		flag=1;
		k=0;
		for(j=i; j<i+keylen; j++)
		{
			if(p[j]!=q[k++])
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
		{
			f=1;
			break;
		}
	}
	if(f==1) return 1;
	else return 0;
}

/*转小写*/ 
char *lowerChange(char *p)
{
	int len, i;
	len=strlen(p);
	for(i=0; i<len; i++)
	{
		if(p[i]>='A'&&p[i]<='Z')
		{
			p[i]=p[i]+32;
		}
	}
	return p;
}

/*根据书名在图书信息链表里模糊搜索,若无结果，则返回NULL；有，则返回该结构体*/
struct bookInformation* bookname_fuzzySearch(struct bookInformation* poiter, char *key)
{
	int flag=0;
	char name[31];
	struct bookInformation* p;
	p=poiter; 
	while(1)
	{
		strcpy(name, p->bookName);
		flag=includeJudge(name, key);
		if(flag==0) p=p->next;
		else break;
		if(p==NULL) break;
	}
	if(flag==0) return NULL;
	else
	{
		searchflag=p;
		return p;
	}
}

/*根据关键字在图书信息链表里模糊搜索，若无结果，则返回NULL；有，则返回该结构体*/
struct bookInformation* keyword_fuzzySearch(struct bookInformation* poiter, char *key)
{
	int flag=0;
	int f=0;
	int i;
	char keyword[31];
	struct bookInformation* p;
	p=poiter;
	while(1)
	{
		for(i=0; i<5; i++)
		{
			strcpy(keyword, p->keyword[i]);
			flag=includeJudge(keyword, key);
			if(flag==1)
			{
				f=1;
				break;
			}
		}
		if(f==1) break;
		else p=p->next;
		if(p==NULL) break;
	}
	if(f==0) return NULL;
	else
	{
		searchflag=p;
		return p;
	}
}

/*根据作者名在图书信息链表里模糊搜索，若无结果，则返回NULL；有，则返回该结构体*/
struct bookInformation* author_fuzzySearch(struct bookInformation* poiter, char *key)
{
	int flag=0;
	int f=0;
	int i;
	char author[31];
	struct bookInformation* p;
	p=poiter;
	while(1)
	{
		for(i=0;i<3;i++)
		{
			strcpy(author, p->authorName[i]);
			flag=includeJudge(author, key);
			if(flag==1)
			{
				f=1;
				break;
			}
		}
		if(f==1) break;
		else p=p->next;
		if(p==NULL) break;
	}
	if(f==0) return NULL;
	else
	{
		searchflag=p;
		return p;
	}
}
