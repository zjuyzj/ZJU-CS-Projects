/*
 * �ļ���ds.c
 * ---------------------------
 * �����ṹ����������õĺ��� 
 * ��дʱ�䣺2020.05.16
 */
	 
#include "main.h"
#include "ds.h" 
#include "search.h"

/*ͼ���ȫ�ֱ���*/ 
struct bookInformation *bookhead=NULL;

struct bookInformation *booktail=NULL;

int booktype[5][2]={0}; /*���ÿһ��ͼ��������Լ��������*/ 
/*��ͼ��������һһ��ӳ,�ֱ�Ϊ��������\ë��˼�롢��ѧ������ѧ����Ȼ��ѧ���ۺ���ͼ��*/	
  
const char *booktypeName[5]={"Marxism-Leninism & Mao Zedong Thought",
					   		"Philosophy", "Social Science", "Natural Science",
					   		"Comprehensive book"};
					   		
struct bookInformation *bookformer=NULL; /*formerָ��ǰһ��ָ��*/

struct bookInformation *searchflag; /*����������ָ��*/ 

/*�û���ȫ�ֱ���*/
struct userInformation *userhead=NULL;

struct userInformation *usertail=NULL;

struct userInformation *userformer=NULL; /*formerָ��ǰһ��ָ��*/

/*�鼮���û���ϵ��ȫ�ֱ���*/
struct book_and_userRelation *relationhead=NULL;

struct book_and_userRelation *relationtail=NULL;

struct book_and_userRelation *relationformer=NULL;

/*ͼ���������޸ĺ�����flag=1��������flag=0���޸ģ��޸�ʱ����int=-1�����޸ģ���ָ��ΪNULL�������޸� */ 
void bookAddition_Change(int flag, int bookNumber, int bookType, char *bookName, char keywordPoiter[][31],
						 int keywordNumber, char authorPoiter[][31], int authorNumber, char *publishingHouse,
						 char *publicationDate)
{
	struct bookInformation *p=NULL;
	int i;
	if(flag==1)
	{
		p=(struct bookInformation *)malloc(sizeof(struct bookInformation));
		if(p!=NULL)
		{
			p->bookNumber=bookNumber;
			p->booktypeFlag=bookType;
			p->borrowTimes=0;
			booktype[bookType][0]++; /*���������ڵ��������������һ*/ 
			strcpy(p->bookName, bookName);
			for(i=0; i<keywordNumber; i++)
			{
				strcpy(p->keyword[i], keywordPoiter[i]);
			}
			for(i=keywordNumber; i<5; i++)
			{
				strcpy(p->keyword[i], "~");
			}
			for(i=0; i<authorNumber; i++)
			{
				strcpy(p->authorName[i], authorPoiter[i]);
			}
			for(i=authorNumber; i<3; i++)
			{
				strcpy(p->authorName[i], "~");
			}
			strcpy(p->publishingHouse, publishingHouse);
			strcpy(p->publicationDate, publicationDate);
			p->next=NULL;
		}
		if(bookhead==NULL)
		{
			bookhead=p;
			searchflag=(struct bookInformation *)malloc(sizeof(struct bookInformation));
			searchflag->next=bookhead;
		}
		else
		{
			booktail->next=p;
		}
		booktail=p;
	}
	else
	{
		p=booknumberSearch(bookNumber);
		if(bookType!=-1)
		{
			booktype[p->booktypeFlag][0]--; /*ԭ�ȵĸ���������һ�����ṩ�����������һ*/ 
			booktype[bookType][0]++;
			p->booktypeFlag=bookType;
		} 
		if(bookName!=NULL) strcpy(p->bookName, bookName);
		if(keywordNumber!=-1)
		{
			for(i=0; i<keywordNumber; i++){
				strcpy(p->keyword[i], keywordPoiter[i]);
			}
		}
		if(authorNumber!=-1)
		{
			for(i=0; i<authorNumber; i++)
			{
				strcpy(p->authorName[i], authorPoiter[i]);
			}
		}
			
		if(publishingHouse!=NULL) strcpy(p->publishingHouse, publishingHouse);
		if(publicationDate!=NULL) strcpy(p->publicationDate, publicationDate);
	}	 
}

/*ͼ��ɾ������*/			  
void bookDelete(int bookNumber)
{
	struct bookInformation *p=NULL;
	int type;
	p=booknumberSearch(bookNumber);
	type=p->booktypeFlag;
	booktype[type][0]--; /*����ͼ��������һ*/ 
	if(bookformer==NULL)
	{
		bookhead=p->next;
	}
	else bookformer->next=p->next;
	free(p);
}
 
/*�û��������޸ĺ�����flag=1��������flag=0���޸�*/
void userAddition_Change(int flag, int userNumber, char *userName, int gender, char *workUnits)
{
	struct userInformation *p=NULL;
	int i;
	if(flag==1)
	{
		p=(struct userInformation *)malloc(sizeof(struct userInformation));
		if(p!=NULL)
		{
			p->userNumber=userNumber;
			p->genderFlag=gender;
			p->borrowtimes[0]=0;
			p->borrowtimes[1]=0;
			p->borrowtimes[2]=0;
			p->borrowtimes[3]=0;
			p->borrowtimes[4]=0;
			strcpy(p->userName, userName);
			strcpy(p->workUnits, workUnits);
			p->head=NULL; 
			p->tail=NULL;
			p->Next=NULL;
		}
		if(userhead==NULL)
		{
			userhead=p;
		}
		else
		{
			usertail->Next=p;
		}
		usertail=p;
	}
	else
	{
		p=usernumberSearch(userNumber);
		if(userName!=NULL) strcpy(p->userName, userName);	
		if(gender!=-1) p->genderFlag=gender;
		if(workUnits!=NULL) strcpy(p->workUnits, workUnits);
	}	
}

/*�û�ɾ������*/
void userDelete(int userNumber)
{
	struct userInformation *p=NULL;
	p=usernumberSearch(userNumber);
	if(userformer==NULL)
	{
		userhead=p->Next;
	}
	else userformer->Next=p->Next;
	free(p);
} 

/*���麯��,���������û������¼,���Ӹ���Ľ��Ĵ����������û����Ĵ���,���Ӹ����鼮������������Լ������û����鼮�Ĺ�ϵ*/ 
void borrowBooks(int bookNumber, int userNumber, int year, int month, int day)
{
	/*�����û������¼*/ 
	struct userInformation *q=NULL;
	q=usernumberSearch(userNumber);	
	struct user_borrowing_record *p=NULL;
	p=(struct user_borrowing_record *)malloc(sizeof(struct user_borrowing_record));
	if(p!=NULL)
	{
		p->bookNumber=bookNumber;
		p->returnFlag=0;
		strcpy(p->bookName,booknumberSearch(bookNumber)->bookName);
		p->next=NULL;
		if(q->head==NULL)
		{
			q->head=p;
		}
		else
		{
			q->tail->next=p; 
		}
		q->tail=p;
	}
	
	/* ���Ӹ���Ľ��Ĵ���*/ 
	struct bookInformation *s=NULL;
	s=booknumberSearch(bookNumber);
	s->borrowTimes=s->borrowTimes+1;
	
	/*�����û����Ĵ���*/
	q->borrowtimes[s->booktypeFlag]++;
		
	/*���Ӹ����鼮���������*/
	booktype[s->booktypeFlag][1]++;
	
	/*�����û����鼮�Ĺ�ϵ*/ 
	struct book_and_userRelation *r;
	r=(struct book_and_userRelation *)malloc(sizeof(struct book_and_userRelation));
	r->bookNumber=bookNumber;
	r->userNumber=userNumber;
	r->borrowyear=year;
	r->borrowmonth=month;
	r->borrowday=day;
	int flag;
	int i,days;
	days=0;
	int monthday[2][12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
						 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int yearday[2]={365, 366};
	flag=judgeleapyear(year);
	for(i=0; i<month-1; i++)
	{
		days+=monthday[flag][i];
	}
	days=days+day+borrowingDays;
	i=0;
	if(days<=yearday[flag])
	{
		r->returnyear=year;
		while(days>0)
		{
			days=days-monthday[flag][i];
			i++;
			
		}
		r->returnmonth=i;
		r->returnday=days+monthday[flag][i-1];
	}
	else
	{
		r->returnyear=year+1;
		days=days-yearday[flag];
		while(days>0)
		{
			days=days-monthday[flag][i];
			i++;
		}
		r->returnmonth=i;
		r->returnday=days+monthday[flag][i-1];
	}
	r->next=NULL;
	if(relationhead==NULL)
	{
		relationhead=r;
		
	}
	else{
		relationtail->next=r;
	}
	relationtail=r;
}

/*���麯��,�������û����ļ�¼�и��鼮��״̬�����ٸ����鼮������������Լ�ɾ����ͼ���û��Ĺ�ϵ*/
void returnBooks(int bookNumber, int userNumber)
{
	/*��״̬*/ 
	struct userInformation *p;
	struct user_borrowing_record *q;
	p=usernumberSearch(userNumber);
	q=p->head;
	while(q != NULL){
		q=user_borrowing_record_search(bookNumber, q);
		if(q->returnFlag == 0) break;
		q = q->next;
	}
	q->returnFlag=1;
	
	/*���ٸ����鼮���������*/
	struct bookInformation *s;
	s=booknumberSearch(bookNumber);
	booktype[s->booktypeFlag][1]--; 

	/*ɾ��ͼ���û���ϵ*/ 
	struct book_and_userRelation *r;
	r=book_and_userRelation_search(bookNumber);
	if(relationtail==r)
		relationtail=relationformer;
	if(relationformer==NULL)
	{
		relationhead=r->next;
		
	}
	else
	{
		relationformer->next=r->next;
	}
	free(r);
}


/*�ж�����*/ 
int judgeleapyear(int year)
{
	if(year%4==0 && year%100!=0) return 1;
	if(year%400==0) return 1;
	return 0;	
}

/*�ַ������ܺ��������ܲ�������򣬺������ܵı��ַ��������������г����Զ�offsetΪƫ�������������Ԫ���±���Ϊƫ����*/ 
char *passwordEncrypt(char p[],int offset)
{
	int i,len,off,sum;
	char *q;
	char c;
	len=strlen(p);
	off=abs(offset);
	q=(char *)malloc((len+1)*sizeof(char));
	for(i=0; i<len; i++)
	{
		sum=0;
		c=p[i];
		c=c^90;/*��8λ�������еĵ�2��4��5��7λ(����������)��ת*/ 
		sum=c+off+i;
		c=sum%128;
		q[i]=c;
	}
	q[i] = '\0'; 
	return q;
}
