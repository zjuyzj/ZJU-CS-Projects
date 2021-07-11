/*
 * �ļ���ds.h
 * ---------------------------
 * ����ļ�������ͼ����û���Ҫ�õ��Ļ��������ݽṹ:
 *	ȫ�ֱ���ͼ����Ϣ�ṹ��struct bookInformation)
 *	ȫ�ֱ����û������¼��struct user_borrowing_record�� 
 *	ȫ�ֱ����û������ߣ���Ϣ�ṹ��struct userInformation)
 *	ȫ�ֱ������û���ϵ�ṹ��struct book_and_userRalation)      
 *	����ص������� 
 * ��дʱ�䣺2020.05.16 
 */
 
#ifndef _DS_H_
#define _DS_H_	
	/*
	 *����: bookInformation
	 * -------------------
	 *��;������bookInformation�ṹ,������ţ�Ψһ��������(������80���ַ����������Ĵ��� 
	 *ͼ�����ͣ�0-4�ֱ��Ӧ��������\ë��˼�롢��ѧ������ѧ����Ȼ��ѧ���ۺ���ͼ�飩 
	 *�ؼ��ʣ����5����ÿһ���ʲ�����30���ַ��������ߣ����3�ˣ�ÿһ������������30���ַ����� 
	 *�����磨���Ʋ�����30���ַ���\�������ڣ���λ��ݣ���λ�·ݣ���λ���ڣ�������0���룩�Լ��ṹָ�� 
	 *��д�ˣ��ŵ���
	 *��д���ڣ�2020.5.12
	 */
	 
	struct bookInformation
	{
		int bookNumber;
		int booktypeFlag;
		int borrowTimes; /*�����Ĵ���*/ 
		char bookName[81];
		char keyword[5][31]; /*������5��������'~'���*/ 
		char authorName[3][31]; /*������3��������'~'���*/
		char publishingHouse[31];
		char publicationDate[11];
		struct bookInformation *next; 
	}; 
	
	
	/*
	 *���ͣ�user_borrowing_record  
	 * --------------------------
	 *��;������struct user_borrowing_record�ṹ�������û��������š������Լ��黹�����1λ�黹��0Ϊδ�黹��
	 *��д�ˣ��ŵ���
	 *��д���ڣ�2020.5.12
	 */
	 
	struct user_borrowing_record
	{
		int bookNumber;
		char bookName[31];
		int returnFlag;
		struct user_borrowing_record *next;
	};
	
	
	/*
	 *���ͣ�userInformation
	 * --------------------
	 *��;������userInformation�ṹ�������û��ţ�Ψһ��������������û�������������30���ַ����� 
	 *�Ա�(0ΪŮ�ԣ�1Ϊ���ԣ���������λ��������30���ַ�)
	 *��д�ˣ��ŵ���
	 *��д���ڣ�2020.5.12
	 */
	 
	struct userInformation
	{
		int userNumber;
		int borrowtimes[5]; /*�������,�±��0��4�ֱ��Ӧͼ�����*/ 
		char userName[31];
		int genderFlag;
		char workUnits[31];	
		struct user_borrowing_record *head;
		struct user_borrowing_record *tail;
		struct userInformation *Next;
	}; 
	  
	  
	/*
	 *���ͣ�book_and_user_relation
	 * ----------------------------------
	 *��;������struct book_and_userRelation�ṹ��������š��û��š����������Լ�Ӧ������
	 *��д�ˣ��ŵ��� 
	 *��д���ڣ�2020.5.12
	 */
	 
	struct book_and_userRelation
	{
		int bookNumber;
		int userNumber;
		int borrowyear;
		int borrowmonth;
		int borrowday;
		int returnyear;
		int returnmonth;
		int returnday; 
		struct book_and_userRelation *next;
	};
	
	/*ͼ���ȫ�ֱ���*/ 
	extern struct bookInformation *bookhead;
	extern struct bookInformation *booktail;
	extern int booktype[5][2];  /*���ÿһ��ͼ��������Լ��������*/ 
	/*��ͼ��������һһ��ӳ,�ֱ�Ϊ��������\ë��˼�롢��ѧ������ѧ����Ȼ��ѧ���ۺ���ͼ��*/	  
	extern const char *booktypeName[5];
	extern struct bookInformation *bookformer; /*formerָ��ǰһ��ָ��*/
	extern struct bookInformation *searchflag; /*����������ָ��*/ 

	/*�û���ȫ�ֱ���*/
	extern struct userInformation *userhead;
	extern struct userInformation *usertail;
	extern struct userInformation *userformer; /*formerָ��ǰһ��ָ��*/

	/*�鼮���û���ϵ��ȫ�ֱ���*/
	extern struct book_and_userRelation *relationhead;
	extern struct book_and_userRelation *relationtail;
	extern struct book_and_userRelation *relationformer;
	  
	/*ͼ���������޸ĺ�����flag=1��������flag=0���޸ģ� */ 
	void bookAddition_Change(int flag, int bookNumber, int bookType, char *bookName, char keywordPoiter[][31],
						     int keywordNumber, char authorPoiter[][31], int authorNumber, char *publishingHouse,
						     char *publicationDate);
				
	/*ͼ��ɾ������*/			  
	void bookDelete(int bookNumber);
	
	/*�û��������޸ĺ�����flag=1��������flag=0���޸�*/
	void userAddition_Change(int flag, int userNumber, char *userName, int gender, char *workUnits);
	
	/*�û�ɾ������*/
	void userDelete(int userNumber);
	
	/*���������ͼ����Ϣ������������ͼ��ĺ���*/
	struct bookInformation* booknumberSearch(int number); 
	
	/*����������û����ļ�¼�������*/ 
	struct user_borrowing_record* user_borrowing_record_search(int number, struct user_borrowing_record *q);	
	
	/*���������ͼ���û���ϵ�����������*/
	struct book_and_userRelation* book_and_userRelation_search(int number);
	
	/*�����û������û���Ϣ�������������û��ĺ���*/
	struct userInformation* usernumberSearch(int number);
	
	/*�ж�һ���ַ����Ƿ�����һ���ַ������У��򷵻�1���ޣ��򷵻�0*/
	int includeJudge(char *name, char *key); 
	
	/*תСд*/ 
	char *lowerChange(char *p);
	
	/*����������ͼ����Ϣ������ģ������*/
	struct bookInformation* bookname_fuzzySearch(struct bookInformation* p, char *key);
	
	/*���ݹؼ�����ͼ����Ϣ������ģ�����������޽�����򷵻�NULL���У��򷵻ظýṹ��*/
	struct bookInformation* keyword_fuzzySearch(struct bookInformation* p, char *key);
	
	/*������������ͼ����Ϣ������ģ�����������޽�����򷵻�NULL���У��򷵻ظýṹ��*/
	struct bookInformation* author_fuzzySearch(struct bookInformation* poiter, char *key);
	
	/*���麯��*/
	void borrowBooks(int bookNumber, int userNumber, int year, int month, int day); 
	
	/*���麯��*/
	void returnBooks(int bookNumber, int userNumber); 
	
	/*�ж����꺯��*/ 
	int judgeleapyear(int year);
	
	/*�������ͣ�����������ַ���Ϊ���ģ�passwordΪ���ģ��ú���ʵ�������ĵ�ת������*/ 
	/*�ַ������ܺ��������ܲ�������򣬺������ܵı��ַ��������������г����Զ�offsetΪƫ�������������Ԫ���±���Ϊƫ����*/  
	char *passwordEncrypt(char p[], int offset);
	
#endif
