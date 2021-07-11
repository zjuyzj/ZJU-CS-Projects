/*
 * �ļ���search.c
 * ---------------------------
 * �����������õĺ��� 
 * ��дʱ�䣺2020.05.22
 */
	 
#include "main.h"
#include "ds.h"
#include "search.h"

/*���������ͼ����Ϣ������������ͼ��ĺ���,formerָ����������ǰһ��ָ��*/
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

/*����������û����ļ�¼�����������*/ 
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

/*���������ͼ���û���ϵ�����������*/
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

/*�����û������û���Ϣ�������������û��ĺ���,formerָ����������ǰһ��ָ��*/
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

/*�ж�һ���ַ����Ƿ�����һ���ַ������У��򷵻�1���ޣ��򷵻�0*/
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

/*תСд*/ 
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

/*����������ͼ����Ϣ������ģ������,���޽�����򷵻�NULL���У��򷵻ظýṹ��*/
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

/*���ݹؼ�����ͼ����Ϣ������ģ�����������޽�����򷵻�NULL���У��򷵻ظýṹ��*/
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

/*������������ͼ����Ϣ������ģ�����������޽�����򷵻�NULL���У��򷵻ظýṹ��*/
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
