/*
 * �ļ���Sort.h
 * ---------------------------
 * ����ļ������˸����������ҳ����ܻ�ӭͼ�顢��������û��ĺ��� 
 * ��дʱ�䣺2020.05.31
 */
 
#ifndef _SORT_H_
#define _SORT_H_  
   
/*ͼ�������� (flag==0Ϊ����flag==1Ϊ����)*/
void book_sort(int flag);

/*�û������� (flag==0Ϊ����flag==1Ϊ����)*/
void user_sort(int flag); 

/*�ҳ����Ĵ��������屾��*/
struct bookInformation *SORT_book(void);

/*�ҳ�����������������*/
struct userInformation *SORT_user(void);

#endif
