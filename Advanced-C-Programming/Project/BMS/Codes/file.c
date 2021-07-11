/*
 * �ļ���file.c
 * ---------------------------
 * �����ļ��������������ͳ���������뱣�漰��ȡ���� 
 * ��дʱ�䣺2020.05.22��2020.05.25 
 */
 
#include "main.h"
#include "file.h"
#include "ds.h" /*��������������������ļ�*/

/*ͼ����Ϣ���ļ� "book_information.db"*/
/*�û���Ϣ���ļ� "user_information.db"*/
/*ͼ���û���ϵ�� "book_user_information.db"*/
/*ͼ����Ϣ���ļ� "book_information.db"  ���ݼ���������ĺ���*/ 


void systemSettings_add(void)
{
	FILE *fp;
	if(fp=fopen("settings.ini", "w+"))
	{
		fprintf(fp, "%d %d\n", borrowingDays, borrowingBooks);
		fprintf(fp, "%d %d\n", mySettings.autoSaveSwitch, mySettings.autoSaveTime);
		fprintf(fp, "%d %d\n", mySettings.musicSwitch, mySettings.musicRepeat);
		fprintf(fp, "%s",mySettings.musicPath);
		fclose(fp);
	}
	if(fp=fopen("passwd.bin", "wb+"))
	{
		fprintf(fp, "%s;", mySettings.adminPassward);
		fclose(fp);
	}
}

void systemSettings_load(void)
{
	FILE *fp;
	if(fp=fopen("settings.ini", "r"))
	{
		fscanf(fp, "%d %d\n", &borrowingDays, &borrowingBooks);
		fscanf(fp, "%d %d\n", &(mySettings.autoSaveSwitch), &(mySettings.autoSaveTime));
		fscanf(fp, "%d %d\n", &(mySettings.musicSwitch), &(mySettings.musicRepeat));
		fscanf(fp, "%s", mySettings.musicPath);
		fclose(fp);
	}
	else
	{
		systemSettings_add(); /*�����ʧ�ܣ�û���ҵ������ļ�������ô�ʹ���һ��*/ 
		fclose(fp);
	}
	if(fp=fopen("passwd.bin", "rb"))
	{ 
		fscanf(fp, "%s", mySettings.adminPassward);
		mySettings.adminPassward[strlen(mySettings.adminPassward) - 1] ='\0';
		fclose(fp);
	}
	else
	{
		systemSettings_add();
		fclose(fp);
	}
}

/*ͼ����Ϣ���ļ� "book_information.db"  ���ݼ���������ĺ���*/ 
int bookDocument_load(void)
{
	FILE *fp;
	int i, j;
	int flag, bookNumber, bookType, borrowTimes, keywordNumber=0, authorNumber=0;
	char *bookName=(char *)malloc(sizeof(char)*81), keywordPoiter[5][31], authorPoiter[3][31];
	char *publishingHouse=(char *)malloc(sizeof(char)*31), *publicationDate=(char *)malloc(sizeof(char)*11);
	char *s=(char *)malloc(sizeof(char)*280);
    struct bookInformation *p=NULL;
    for(i = 0;i < 5;i++) 
			for(j = 0;j < 2;j++)
				booktype[i][j] = 0;	/*���ͳ�����ݣ������ļ������߼�������*/
	if((fp=fopen("book_information.db", "r+"))==NULL) /*��ͼ����Ϣ���ļ�*/ 
	{
		return 1;
	}
	rewind(fp);
	fgets(s, APPRANGE, fp);
   	if(strlen(s)<5)  ;
   	else
	{
   		rewind(fp);
     	while(!feof(fp))
    	{
    		keywordNumber=0,authorNumber=0;
    	   	fscanf(fp, "%d %d %d %s ", &bookNumber, &bookType, &borrowTimes, bookName);
            for(i=0; i<5; i++)
     		{
	    		fscanf(fp, "%s ", keywordPoiter[i]);
 	    		if(strcmp(keywordPoiter[i], "~")!=0)
	    		{
	    			keywordNumber++;
	    		}
	    	}
	    	for(i=0; i<3; i++)
	    	{
	    		fscanf(fp, "%s ", authorPoiter[i]);
	    		if(strcmp(authorPoiter[i], "~")!=0)
		    	{
			    	authorNumber++;
		    	}
	    	}
	    	fscanf(fp, "%s %s\n", publishingHouse, publicationDate);
	    	for(i=0; i<strlen(bookName); i++)
	    	{
		    	if(*(bookName+i)=='_')
		    	{
		    		*(bookName+i)=' ';
		    	}
	    	}
            for(j=0; j<keywordNumber; j++)
	    	{
	    		for(i=0; i<strlen(keywordPoiter[j]); i++)
	    		{
	    			if(keywordPoiter[j][i]=='_')
	    			{
		    			keywordPoiter[j][i]=' ';
	    			}
	    		}
	    	}
            for(j=0; j<authorNumber; j++)
	    	{
	    		for(i=0; i<strlen(authorPoiter[j]); i++)
	    		{
	    			if(authorPoiter[j][i]=='_')
	    			{
		    			authorPoiter[j][i]=' ';
		    		}
		    	}
	    	}
	    	for(i=0; i<strlen(publishingHouse); i++)
	    	{
		    	if(*(publishingHouse+i)=='_')
		    	{
		    		*(publishingHouse+i)=' ';
		    	}
	    	}
	    	for(i=0; i<strlen(publicationDate); i++)
	    	{
	    		if(*(publicationDate+i)=='_')
		    	{
		    		*(publicationDate+i)=' ';
		    	}
	    	}
     		p=(struct bookInformation *)malloc(sizeof(struct bookInformation));
    		p->bookNumber=bookNumber;
    		p->booktypeFlag=bookType;
    		p->borrowTimes=borrowTimes;
    		booktype[bookType][0]++; /*���������ڵ��������������һ*/ 
  	    	strcpy(p->bookName, bookName);
    		for(i=0;i<5;i++)
			{
  				strcpy(p->keyword[i], keywordPoiter[i]);
 			}
			for(i=0;i<3;i++)
			{
				strcpy(p->authorName[i], authorPoiter[i]);
			}
			strcpy(p->publishingHouse, publishingHouse);
			strcpy(p->publicationDate, publicationDate);
			p->next=NULL;
	    	if(bookhead==NULL)
			{	
     	   		bookhead=p;
	    	}
	    	else
			{
     			booktail->next=p;
     		}
     		booktail=p; 
    	}
    }
	if(fclose(fp)) /*�ر��ļ�*/ 
	{
		return 1;
	}
	return 0;	
}

/*ͼ����Ϣ���ļ� "book_information.db"  �������ݴ����ļ�����*/ 
int bookDocument_add(void)
{
	FILE *fp;
	int i,j;
	struct bookInformation *p=bookhead;
	if((fp=fopen("book_information.db", "w+"))==NULL) /*��ͼ����Ϣ���ļ�*/ 
	{
		return 1;
	}
    while(p!=NULL)
	{
		for(i=0; i<strlen(p->bookName); i++)
		{
			if(*(p->bookName+i)==' ')
			{
				*(p->bookName+i)='_';
			}
		}
        for(j=0; strcmp(p->keyword[j], "~")!=0&&j<5; j++)
		{
			for(i=0; i<strlen(p->keyword[j]); i++)
			{
				if(p->keyword[j][i]==' ')
				{
					p->keyword[j][i]='_';
				}
			}
		}
        for(j=0; strcmp(p->authorName[j], "~")!=0&&j<3; j++)
		{
			for(i=0; i<strlen(p->authorName[j]); i++)
			{
				if(p->authorName[j][i]==' ')
				{
					p->authorName[j][i]='_';
				}
			}
		}
		for(i=0; i<strlen(p->publishingHouse); i++)
		{
			if(*(p->publishingHouse+i)==' ')
			{
				*(p->publishingHouse+i)='_';
			}
		}
		for(i=0; i<strlen(p->publicationDate); i++)
		{
			if(*(p->publicationDate+i)==' ')
			{
				*(p->publicationDate+i)='_';
			}
		}
		fprintf(fp, "%d %d %d %s ", p->bookNumber, p->booktypeFlag, p->borrowTimes, p->bookName);
		for(i=0; i<5; i++)
		{
			fprintf(fp,"%s ", p->keyword[i]);
		}
		for(i=0; i<3; i++)
		{
			fprintf(fp, "%s ", p->authorName[i]);
		}
		fprintf(fp, "%s %s\n", p->publishingHouse, p->publicationDate);
		p=p->next;
		
	}
   	if(fclose(fp)) /*�ر��ļ�*/ 
	{
		return 1;
	}
	return 0;
} 

/*�û���Ϣ���ļ� "user_information.db"  ���ݼ���������ĺ���*/  
int userDocument_load(void)
{
	FILE *fp;
	int i, j, count=0, temp;
    int userNumber, gender, borrowtimes[5], returnFlag, bookNumber;  
	char *userName=(char *)malloc(sizeof(char)*31), *workUnits=(char *)malloc(sizeof(char)*31), *bookName=(char *)malloc(sizeof(char)*31);
	char *s=(char *)malloc(sizeof(char)*APPRANGE), ch, *str=(char *)malloc(sizeof(char)*50);
	struct userInformation *p=NULL; 
	struct user_borrowing_record *q=NULL;
	if((fp=fopen("user_information.db", "r+"))==NULL) /*���û���Ϣ���ļ�*/
	{
		return 1;
	}
	rewind(fp);
	fgets(s, APPRANGE, fp);
   	if(strlen(s)<5)  ;
   	else
	{
		rewind(fp);
   	    while(!feof(fp))
     	{
     		fscanf(fp, "%d ", &userNumber);
   	    	for(i=0; i<5; i++)
    		{
	    	   	fscanf(fp, "%d ", &borrowtimes[i]);
    		} 
	     	fscanf(fp, "%s %d %s", userName, &gender, workUnits);
	     	for(i=0; i<strlen(userName); i++)
     		{
      			if(*(userName+i)=='_')
     			{
     				*(userName+i)=' ';
     			}
     		}
           	for(i=0; i<strlen(workUnits); i++)
    		{
    			if(*(workUnits+i)=='_')
	    		{
	    			*(workUnits+i)=' ';
	    		}
	    	}
     		p=(struct userInformation *)malloc(sizeof(struct userInformation));
	    	p->userNumber=userNumber;
		   	p->genderFlag=gender;
            for(i=0; i<5; i++)
			{
				p->borrowtimes[i]=borrowtimes[i];
			}
			strcpy(p->userName, userName);
			strcpy(p->workUnits, workUnits);
			p->head=NULL;
			p->tail=NULL;
			p->Next=NULL;
            fgets(s, APPRANGE, fp);
            if(!feof(fp)==0)
			{
				break;
			}
			i=1;
			while(*(s+i)!='\n')
			{
				count=0;
				q=(struct user_borrowing_record *)malloc(sizeof(struct user_borrowing_record));
                while(count<3)
				{
					j=0;
					while(*(s+i)!=' ')
					{
						*(str+j)=*(s+i);
						i++;	
						j++;
					}
					*(str+j)='\0';
					count++;
					if(count==1)
					{
						j=0;
						temp=0;
						while(*(str+j)!='\0')
						{
							temp=temp*10+(*(str+j)-'0');
							j++;
						}
						q->bookNumber=temp;
					}
					else if(count==2)
					{
						strcpy(q->bookName,str);
					}
					else if(count==3)
					{
						j=0;
						temp=0;
						while(*(str+j)!='\0')
						{
							temp=temp*10+(*(str+j)-'0');
							j++;
						}
						q->returnFlag=temp;
					}
					i++;
				}
				q->next=NULL;
				if(p->head==NULL)
				{
					p->head=q;
				}
				else
				{
					p->tail->next=q;
				}p->tail=q;
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
    }
   	if(fclose(fp)) /*�ر��ļ�*/ 
	{
     	return 1;
	}
	return 0;
}

/*�û���Ϣ���ļ� "user_information.db"  �������ݴ����ļ�����*/
int userDocument_add(void)
{
	FILE *fp;
	int i;
	char ch;
	struct userInformation *p=userhead;
	struct user_borrowing_record *q=NULL;
	if((fp=fopen("user_information.db", "w+"))==NULL) /*��ͼ����Ϣ���ļ�*/ 
	{
		return 1;
	}
    while(p!=NULL)
	{
		fprintf(fp, "%d ", p->userNumber);
		for(i=0;i<5;i++)
		{
			fprintf(fp, "%d ", p->borrowtimes[i]);
		}
		for(i=0; i<strlen(p->userName); i++)
		{
			if(*(p->userName+i)==' ')
			{
				*(p->userName+i)='_';
			}
		}
		for(i=0; i<strlen(p->workUnits); i++)
		{
			if(*(p->workUnits+i)==' ')
			{
				*(p->workUnits+i)='_';
			}
		}
        fprintf(fp, "%s %d %s ", p->userName, p->genderFlag, p->workUnits);
        q=p->head;
        while(q!=NULL)
		{
			for(i=0; i<strlen(q->bookName); i++)
	    	{
		    	if(*(q->bookName+i)==' ')
		    	{
		    		*(q->bookName+i)='_';
		    	}
     		}
			fprintf(fp, "%d %s %d ", q->bookNumber, q->bookName, q->returnFlag);
			q=q->next;
		}
		p=p->Next;
		fputc('\n', fp);
	}
   	if(fclose(fp)) /*�ر��ļ�*/
	{
		return 1;
	}
	return 0;
} 

/*ͼ���û���ϵ�� "book_user_information.db"  ���ݼ���������ĺ���*/  
int relationDocument_load(void)
{
	FILE *fp;
	int i;
	int bookNumber, userNumber, borrowyear, borrowmonth, borrowday, returnyear, returnmonth, returnday;
	char *s=(char *)malloc(sizeof(char)*APPRANGE);
	if((fp=fopen("book_user_information.db", "r+"))==NULL) /*���û���Ϣ���ļ�*/
	{
		return 1;
	}
	rewind(fp);
	fgets(s, APPRANGE, fp);
   	if(strlen(s)<5)  ;
   	else
	{
		rewind(fp);
   	    while(!feof(fp))
     	{
     		fscanf(fp, "%d %d %d %d %d %d %d %d ", &bookNumber, &userNumber, &borrowyear, &borrowmonth, &borrowday, &returnyear, &returnmonth, &returnday);
         	booktype[booknumberSearch(bookNumber)->booktypeFlag][1]++;
			struct book_and_userRelation *r;
        	r=(struct book_and_userRelation *)malloc(sizeof(struct book_and_userRelation));
        	r->bookNumber=bookNumber;
         	r->userNumber=userNumber;
        	r->borrowyear=borrowyear;
        	r->borrowmonth=borrowmonth;
        	r->borrowday=borrowday;
        	int flag, days;
        	days=0;
         	int monthday[2][12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
			       			     31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        	int yearday[2]={365, 366};
        	flag=judgeleapyear(borrowyear);
        	for(i=0; i<borrowmonth-1; i++)
			{
         		days+=monthday[flag][i];
         	}
         	days=days+borrowday+borrowingDays;
        	i=0;
         	if(days<=yearday[flag])
		    {
         		r->returnyear=borrowyear;
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
        		r->returnyear=borrowyear+1;
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
         	else
			{
         		relationtail->next=r;
         	}
         	relationtail=r;
    	} 	
    }
	if(fclose(fp)) /*�ر��ļ�*/ 
	{
		return 1;
	}
	return 0;
}

/*ͼ���û���ϵ�� "book_user_information.db" �������ݴ����ļ�����*/
int relationDocument_add(void)
{
	FILE *fp;
	int i;
	struct book_and_userRelation *p=relationhead;
	if((fp=fopen("book_user_information.db", "w+"))==NULL) /*��ͼ����Ϣ���ļ�*/
	{
		return 1;
	}
    while(p!=NULL)
	{
		fprintf(fp, "%d %d %d %d %d ", p->bookNumber, p->userNumber, p->borrowyear, p->borrowmonth, p->borrowday);
		fprintf(fp, "%d %d %d\n", p->returnyear, p->returnmonth, p->returnday);
		p=p->next;
	}
   	if(fclose(fp)) /*�ر��ļ�*/
	{
		return 1;
	}
	return 0;
} 

int bookSort_add(void)
{
	FILE *fp;
	int i, j;
	struct bookInformation *p=bookhead;
	if((fp=fopen("sort_result.txt", "w+"))==NULL) /*��ͼ����Ϣ���ļ�*/
	{
		return 1;
	}
	rewind(fp);
    while(p!=NULL)
	{
		fprintf(fp, "Name:%s\nID:%d\nCategory:%s\n", p->bookName, p->bookNumber, booktypeName[p->booktypeFlag]);
		fprintf(fp, "Keyword:");
		for(i=0; i<5; i++)
		{
			if(strcmp(p->keyword[i], "~") != 0)
				fprintf(fp, "%s ", p->keyword[i]);
		}
		fprintf(fp, "\nAuthor:");
		for(i=0; i<3; i++)
		{
			if(strcmp(p->authorName[i], "~") != 0)
				fprintf(fp, "%s ", p->authorName[i]);
		}
		fprintf(fp, "\nPublishing House:%s\nPublication Date:%s\n\n", p->publishingHouse, p->publicationDate);
		p=p->next;
		
	}
   	if(fclose(fp))
	{
		return 1;
	}
	return 0;
} 

int userSort_add(void)
{
	FILE *fp;
	int i;
	char ch;
	struct userInformation *p=userhead;
	if((fp=fopen("sort_result.txt", "w+"))==NULL)
	{
		return 1;
	}
	rewind(fp);
    while(p!=NULL)
	{
		fprintf(fp, "Name:%s\nID:%d\nGender:%d\nCompany:%s\n", p->userName, p->userNumber, p->genderFlag, p->workUnits);
		p=p->Next;
		fputc('\n', fp);
	}
   	if(fclose(fp))
	{
		return 1;
	}
	return 0;
} 


