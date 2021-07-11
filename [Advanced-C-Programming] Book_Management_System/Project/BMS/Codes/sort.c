/*
 * 文件：Sort.c
 * ---------------------------
 * 各排序函数及找出最受欢迎图书、借书最多用户的函数 
 * 编写时间：2020.05.31
 */	
	  
#include "main.h"
#include "ds.h"
#include "sort.h"

/*图书排序函数 (flag==0为升序，flag==1为降序)*/ 
void book_sort(int flag)
{
	if(flag==0)
	{
		if((bookhead==NULL)||(bookhead->next==NULL))
		{
			;
		}
		else
		{
			struct bookInformation *head, *pre, *cur, *next, *eof, *temp;
			head=(struct bookInformation *)malloc(sizeof(struct bookInformation));
            head->next=bookhead;
            eof=NULL;
            while(head->next!=eof)
            {
                for(pre=head, cur=pre->next, next=cur->next; next!=eof; pre=pre->next, cur=cur->next, next=next->next)
                {
                    if(strcmp(cur->bookName, next->bookName)>0)
                    {
                        cur->next=next->next;
                        pre->next=next;
                        next->next=cur;
                        temp=next;
                        next=cur;
                        cur=temp;
                    }
                }
                eof=cur;
            }
            bookhead=head->next;
		}
	}
	else if(flag==1)
	{
		if((bookhead==NULL)||(bookhead->next==NULL))
		{
			;
		} 
		else
		{
			struct bookInformation *head, *pre, *cur, *next, *eof, *temp;
			head=(struct bookInformation *)malloc(sizeof(struct bookInformation));
            head->next=bookhead;
            eof=NULL;
            while(head->next!=eof)
            {
                for(pre=head, cur=pre->next, next=cur->next; next!=eof; pre=pre->next, cur=cur->next, next=next->next)
                {
                    if(strcmp(cur->bookName, next->bookName)<0)
                    {
                        cur->next=next->next;
                        pre->next=next;
                        next->next=cur;
                        temp=next;
                        next=cur;
                        cur=temp;
                    }
                }
                eof=cur;
            }
            bookhead=head->next;
		}
	}
}

/*用户排序函数 (flag==0为升序，flag==1为降序)*/
void user_sort(int flag)
{
	if(flag==0)
	{
		if((userhead==NULL)||(userhead->Next==NULL))
		{
			;
		} 
		else
		{
			struct userInformation *head, *pre, *cur, *next, *eof, *temp;
			head=(struct userInformation*)malloc(sizeof(struct userInformation));
            head->Next=userhead;
            eof=NULL;
            while(head->Next!=eof)
            {
                for(pre=head, cur=pre->Next, next=cur->Next; next!=eof; pre=pre->Next, cur=cur->Next, next=next->Next)
                {
                    if(strcmp(cur->userName, next->userName)>0)
                    {
                        cur->Next=next->Next;
                        pre->Next=next;
                        next->Next=cur;
                        temp=next;
                        next=cur;
                        cur=temp;
                    }
                }
                eof=cur;
            }
            userhead=head->Next;
		}
	}
	else if(flag==1)
	{
		if((userhead==NULL)||(userhead->Next==NULL))
		{
			;
		}
		else
		{
			struct userInformation *head, *pre, *cur, *next, *eof, *temp;
			head=(struct userInformation *)malloc(sizeof(struct userInformation));
            head->Next=userhead;
            eof=NULL;
            while(head->Next!=eof)
            {
                for(pre=head, cur=pre->Next, next=cur->Next; next!=eof; pre=pre->Next, cur=cur->Next, next=next->Next)
                {
                    if(strcmp(cur->userName, next->userName)<0)
                    {
                        cur->Next=next->Next;
                        pre->Next=next;
                        next->Next=cur;
                        temp=next;
                        next=cur;
                        cur=temp;
                    }
                }
                eof=cur;
            }
            userhead=head->Next;
		}
	}
}

/*找出借阅次数最多的五本书,对于有并列的全部输出*/
struct bookInformation *SORT_book(void)
{
	struct bookInformation *p=NULL, *searchhead=NULL, *searchtail=NULL, *q=NULL;
	int count=0,i,item;
	p=bookhead;
	while(p!=NULL)
	{
		q=(struct bookInformation *)malloc(sizeof(struct bookInformation));
		q->bookNumber=p->bookNumber;
		q->booktypeFlag=p->booktypeFlag;
		q->borrowTimes=p->borrowTimes;
		for(i=0; i<3; i++)
		{
			strcpy(q->authorName[i], p->authorName[i]);
		}
		strcpy(q->bookName, p->bookName);
		for(i=0; i<5; i++)
		{
			strcpy(q->keyword[i], p->keyword[i]);
		}
		strcpy(q->publicationDate, p->publicationDate);
		strcpy(q->publishingHouse, p->publishingHouse);
		q->next=p->next;
		if(searchhead==NULL)
		{
			searchhead=q;
		}
		else
		{
			searchtail->next=q;
		}searchtail=q;
		count++;
		p=p->next; 
	}
	if((searchhead==NULL)||(searchhead->next==NULL))
	{
		;
	}
	else
	{
		struct bookInformation *head, *pre, *cur, *next, *eof, *temp;
		head=(struct bookInformation *)malloc(sizeof(struct bookInformation));
        head->next=searchhead;
        eof=NULL;
        while(head->next!=eof)
        {
            for(pre=head, cur=pre->next, next=cur->next; next!=eof; pre=pre->next, cur=cur->next, next=next->next)
            {
                if(cur->borrowTimes < next->borrowTimes)
                {
                    cur->next=next->next;
                    pre->next=next;
                    next->next=cur;
                    temp=next;
                    next=cur;                      
					cur=temp;
                }
	        }
            eof=cur;
        }
        searchhead=head->next;
	}
	if(count<=5)
	{
		return searchhead;
	}
	else
	{
		p=searchhead;
		for(i=0; i<4; i++)
		{
			p=p->next;
		}
		while(p->next!=NULL&&p->borrowTimes==p->next->borrowTimes)
		{
			p=p->next;
		}
		while(p->next!=NULL)
		{
			if(p->next->next==NULL)
			{
				free(p->next);
				p->next=NULL;
				return searchhead;
			}
			else
			{
				q=p->next->next;
				free(p->next);
				p->next=q;
			}
		}
		return searchhead;
	}
}


/*找出借书最多的五名读者,对于并列的全部输出*/ 
struct userInformation *SORT_user(void)
{
	struct userInformation *p=NULL, *searchhead=NULL, *searchtail=NULL, *q=NULL;
	struct user_borrowing_record *op=NULL, *oq=NULL;
	int count=0, i, item;
	int curborrowtimes=0, nextborrowtimes=0;
	p=userhead;
	while(p!=NULL)
	{
		q=(struct userInformation*)malloc(sizeof(struct userInformation));
		for(i=0; i<5; i++)
		{
			q->borrowtimes[i]=p->borrowtimes[i];
		}
		q->genderFlag=p->genderFlag;
		q->userNumber=p->userNumber;
		strcpy(q->userName, p->userName);
		strcpy(q->workUnits, p->workUnits);
		op=p->head;
		q->head=NULL;
		q->tail=NULL;
		while(op!=NULL)
		{
			oq=(struct user_borrowing_record *)malloc(sizeof(struct user_borrowing_record)); //Is Correct?
			oq->bookNumber=op->bookNumber;
			oq->returnFlag=op->returnFlag;
			strcpy(oq->bookName, op->bookName);
			oq->next=op->next;
			if(q->head==NULL)
			{
				q->head=oq;
			}
			else
			{
				q->tail->next=oq;
			}q->tail=oq;
			op=op->next;
		} 
		q->Next=p->Next;
		if(searchhead==NULL)
		{
			searchhead=q;
		}
		else
		{
			searchtail->Next=q;
		}searchtail=q;
		count++;
		p=p->Next; 
	}
	if((userhead==NULL)||(userhead->Next==NULL))
	{
		;
	} 
	else
	{
		struct userInformation *head, *pre, *cur, *next, *eof, *temp;
		head=(struct userInformation *)malloc(sizeof(struct userInformation));
        head->Next=searchhead;
        eof=NULL;
        while(head->Next!=eof)
        {
            for(pre=head, cur=pre->Next, next=cur->Next; next!=eof; pre=pre->Next, cur=cur->Next, next=next->Next)
            {
            	curborrowtimes=0;
				nextborrowtimes=0;
            	for(i=0; i<5; i++)
				{
					curborrowtimes=curborrowtimes+cur->borrowtimes[i];
				}
	          	for(i=0; i<5; i++)
				{
					nextborrowtimes=nextborrowtimes+next->borrowtimes[i];
				}
                if(curborrowtimes<nextborrowtimes)
                {
                    cur->Next=next->Next;
                    pre->Next=next;
                    next->Next=cur;
                    temp=next;
                    next=cur;
                    cur=temp;
                }
            }
            eof=cur;
        }
        searchhead=head->Next;
	}
	if(count<=5)
	{
		return searchhead;
	}
	else
	{
		p=searchhead;
		for(i=0;i<4;i++)
		{
			p=p->Next;
		}
		while(p->Next!=NULL&&(p->borrowtimes[0]+p->borrowtimes[1]+p->borrowtimes[2]+p->borrowtimes[3]+p->borrowtimes[4])==
		                     (p->Next->borrowtimes[0]+p->Next->borrowtimes[1]+p->Next->borrowtimes[2]+p->Next->borrowtimes[3]+p->Next->borrowtimes[4]))
		{
			p=p->Next;
		}
		while(p->Next!=NULL)
		{
			if(p->Next->Next==NULL)
			{
				free(p->Next);
				p->Next=NULL;
				return searchhead;
			}
			else
			{
				q=p->Next->Next;
				free(p->Next);
				p->Next=q;
			}
		}
		return searchhead;
	}
}
