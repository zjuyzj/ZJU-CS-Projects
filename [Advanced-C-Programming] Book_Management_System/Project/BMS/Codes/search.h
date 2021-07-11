/*
 * �ļ���search.h
 * ---------------------------
 * �����������õĺ��� 
 * ��дʱ�䣺2020.05.22
 */
 
#ifndef _SEARCH_H_
#define _SEARCH_H_

/*���������ͼ����Ϣ������������ͼ��ĺ���,formerָ����������ǰһ��ָ��*/
struct bookInformation* booknumberSearch(int number);

/*����������û����ļ�¼�����������*/ 
struct user_borrowing_record* user_borrowing_record_search(int number, struct user_borrowing_record *q);

/*���������ͼ���û���ϵ�����������*/
struct book_and_userRelation* book_and_userRelation_search(int number);

/*�����û������û���Ϣ�������������û��ĺ���,formerָ����������ǰһ��ָ��*/
struct userInformation* usernumberSearch(int number);

/*�ж�һ���ַ����Ƿ�����һ���ַ������У��򷵻�1���ޣ��򷵻�0*/
int includeJudge(char *string, char *key);

/*תСд*/ 
char *lowerChange(char *p);

/*����������ͼ����Ϣ������ģ������,���޽�����򷵻�NULL���У��򷵻ظýṹ��*/
struct bookInformation* bookname_fuzzySearch(struct bookInformation* poiter, char *key);

/*������������ͼ����Ϣ������ģ�����������޽�����򷵻�NULL���У��򷵻ظýṹ��*/
struct bookInformation* author_fuzzySearch(struct bookInformation* poiter, char *key);

#endif
