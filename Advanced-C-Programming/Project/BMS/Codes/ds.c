/*
 * 文件：ds.c
 * ---------------------------
 * 各个结构体和链表所用的函数 
 * 编写时间：2020.05.16
 */
	 
#include "main.h"
#include "ds.h" 
#include "search.h"

/*图书的全局变量*/ 
struct bookInformation *bookhead=NULL;

struct bookInformation *booktail=NULL;

int booktype[5][2]={0}; /*存放每一类图书的总数以及外借数量*/ 
/*和图书类类型一一对映,分别为马列主义\毛泽东思想、哲学、社会科学、自然科学和综合性图书*/	
  
const char *booktypeName[5]={"Marxism-Leninism & Mao Zedong Thought",
					   		"Philosophy", "Social Science", "Natural Science",
					   		"Comprehensive book"};
					   		
struct bookInformation *bookformer=NULL; /*former指向前一个指针*/

struct bookInformation *searchflag; /*用于搜索的指针*/ 

/*用户的全局变量*/
struct userInformation *userhead=NULL;

struct userInformation *usertail=NULL;

struct userInformation *userformer=NULL; /*former指向前一个指针*/

/*书籍和用户关系的全局变量*/
struct book_and_userRelation *relationhead=NULL;

struct book_and_userRelation *relationtail=NULL;

struct book_and_userRelation *relationformer=NULL;

/*图书新增和修改函数（flag=1，新增；flag=0，修改）修改时，若int=-1则不用修改，若指针为NULL，则不用修改 */ 
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
			booktype[bookType][0]++; /*将此书属于的类别的书的数量加一*/ 
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
			booktype[p->booktypeFlag][0]--; /*原先的该类总数减一，新提供的类别总数加一*/ 
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

/*图书删除函数*/			  
void bookDelete(int bookNumber)
{
	struct bookInformation *p=NULL;
	int type;
	p=booknumberSearch(bookNumber);
	type=p->booktypeFlag;
	booktype[type][0]--; /*该类图书总数减一*/ 
	if(bookformer==NULL)
	{
		bookhead=p->next;
	}
	else bookformer->next=p->next;
	free(p);
}
 
/*用户新增和修改函数（flag=1，新增；flag=0，修改*/
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

/*用户删除函数*/
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

/*借书函数,包括增加用户借书记录,增加该书的借阅次数，增加用户借阅次数,增加该类书籍的外借数量，以及新增用户和书籍的关系*/ 
void borrowBooks(int bookNumber, int userNumber, int year, int month, int day)
{
	/*增加用户借书记录*/ 
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
	
	/* 增加该书的借阅次数*/ 
	struct bookInformation *s=NULL;
	s=booknumberSearch(bookNumber);
	s->borrowTimes=s->borrowTimes+1;
	
	/*增加用户借阅次数*/
	q->borrowtimes[s->booktypeFlag]++;
		
	/*增加该类书籍的外借数量*/
	booktype[s->booktypeFlag][1]++;
	
	/*新增用户和书籍的关系*/ 
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

/*还书函数,包括改用户借阅记录中该书籍的状态，减少该类书籍的外借数量，以及删除该图书用户的关系*/
void returnBooks(int bookNumber, int userNumber)
{
	/*改状态*/ 
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
	
	/*减少该类书籍的外借数量*/
	struct bookInformation *s;
	s=booknumberSearch(bookNumber);
	booktype[s->booktypeFlag][1]--; 

	/*删除图书用户关系*/ 
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


/*判断闰年*/ 
int judgeleapyear(int year)
{
	if(year%4==0 && year%100!=0) return 1;
	if(year%400==0) return 1;
	return 0;	
}

/*字符串加密函数，加密采用先异或，后凯撒加密的变种方法，凯撒加密中除了自定offset为偏移量，还添加了元素下标作为偏移量*/ 
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
		c=c^90;/*将8位二进制中的第2、4、5、7位(从左往右数)翻转*/ 
		sum=c+off+i;
		c=sum%128;
		q[i]=c;
	}
	q[i] = '\0'; 
	return q;
}
