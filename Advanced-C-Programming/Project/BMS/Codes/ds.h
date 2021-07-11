/*
 * 文件：ds.h
 * ---------------------------
 * 这个文件申明了图书和用户所要用到的基本的数据结构:
 *	全局变量图书信息结构（struct bookInformation)
 *	全局变量用户借书记录（struct user_borrowing_record） 
 *	全局变量用户（读者）信息结构（struct userInformation)
 *	全局变量书用户关系结构（struct book_and_userRalation)      
 *	和相关的链表函数 
 * 编写时间：2020.05.16 
 */
 
#ifndef _DS_H_
#define _DS_H_	
	/*
	 *类型: bookInformation
	 * -------------------
	 *用途：申明bookInformation结构,包含书号（唯一）、书名(不超过80个字符）、被借阅次数 
	 *图书类型（0-4分别对应马列主义\毛泽东思想、哲学、社会科学、自然科学和综合性图书） 
	 *关键词（最多5个，每一个词不超过30个字符）、作者（最多3人，每一个姓名不超过30个字符）、 
	 *出版社（名称不超过30个字符）\出版日期（四位年份，两位月份，两位日期，不足用0补齐）以及结构指针 
	 *编写人：闻丹丽
	 *编写日期：2020.5.12
	 */
	 
	struct bookInformation
	{
		int bookNumber;
		int booktypeFlag;
		int borrowTimes; /*被借阅次数*/ 
		char bookName[81];
		char keyword[5][31]; /*若不足5个，则用'~'填充*/ 
		char authorName[3][31]; /*若不足3个，则用'~'填充*/
		char publishingHouse[31];
		char publicationDate[11];
		struct bookInformation *next; 
	}; 
	
	
	/*
	 *类型：user_borrowing_record  
	 * --------------------------
	 *用途：申明struct user_borrowing_record结构，包含用户借过的书号、书名以及归还情况（1位归还，0为未归还）
	 *编写人：闻丹丽
	 *编写日期：2020.5.12
	 */
	 
	struct user_borrowing_record
	{
		int bookNumber;
		char bookName[31];
		int returnFlag;
		struct user_borrowing_record *next;
	};
	
	
	/*
	 *类型：userInformation
	 * --------------------
	 *用途：申明userInformation结构，包含用户号（唯一）、借书次数、用户姓名（不超过30个字符）、 
	 *性别(0为女性，1为男性）、工作单位（不超过30个字符)
	 *编写人：闻丹丽
	 *编写日期：2020.5.12
	 */
	 
	struct userInformation
	{
		int userNumber;
		int borrowtimes[5]; /*借书次数,下标从0到4分别对应图书类别*/ 
		char userName[31];
		int genderFlag;
		char workUnits[31];	
		struct user_borrowing_record *head;
		struct user_borrowing_record *tail;
		struct userInformation *Next;
	}; 
	  
	  
	/*
	 *类型：book_and_user_relation
	 * ----------------------------------
	 *用途：申明struct book_and_userRelation结构，包含书号、用户号、借阅日期以及应还日期
	 *编写人：闻丹丽 
	 *编写日期：2020.5.12
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
	
	/*图书的全局变量*/ 
	extern struct bookInformation *bookhead;
	extern struct bookInformation *booktail;
	extern int booktype[5][2];  /*存放每一类图书的总数以及外借数量*/ 
	/*和图书类类型一一对映,分别为马列主义\毛泽东思想、哲学、社会科学、自然科学和综合性图书*/	  
	extern const char *booktypeName[5];
	extern struct bookInformation *bookformer; /*former指向前一个指针*/
	extern struct bookInformation *searchflag; /*用于搜索的指针*/ 

	/*用户的全局变量*/
	extern struct userInformation *userhead;
	extern struct userInformation *usertail;
	extern struct userInformation *userformer; /*former指向前一个指针*/

	/*书籍和用户关系的全局变量*/
	extern struct book_and_userRelation *relationhead;
	extern struct book_and_userRelation *relationtail;
	extern struct book_and_userRelation *relationformer;
	  
	/*图书新增和修改函数（flag=1，新增；flag=0，修改） */ 
	void bookAddition_Change(int flag, int bookNumber, int bookType, char *bookName, char keywordPoiter[][31],
						     int keywordNumber, char authorPoiter[][31], int authorNumber, char *publishingHouse,
						     char *publicationDate);
				
	/*图书删除函数*/			  
	void bookDelete(int bookNumber);
	
	/*用户新增和修改函数（flag=1，新增；flag=0，修改*/
	void userAddition_Change(int flag, int userNumber, char *userName, int gender, char *workUnits);
	
	/*用户删除函数*/
	void userDelete(int userNumber);
	
	/*根据书号在图书信息链表里面搜索图书的函数*/
	struct bookInformation* booknumberSearch(int number); 
	
	/*根据书号在用户借阅记录里面查找*/ 
	struct user_borrowing_record* user_borrowing_record_search(int number, struct user_borrowing_record *q);	
	
	/*根据书号在图书用户关系链表里面查找*/
	struct book_and_userRelation* book_and_userRelation_search(int number);
	
	/*根据用户号在用户信息链表里面搜索用户的函数*/
	struct userInformation* usernumberSearch(int number);
	
	/*判断一个字符串是否含有另一个字符串，有，则返回1；无，则返回0*/
	int includeJudge(char *name, char *key); 
	
	/*转小写*/ 
	char *lowerChange(char *p);
	
	/*根据书名在图书信息链表里模糊搜索*/
	struct bookInformation* bookname_fuzzySearch(struct bookInformation* p, char *key);
	
	/*根据关键字在图书信息链表里模糊搜索，若无结果，则返回NULL；有，则返回该结构体*/
	struct bookInformation* keyword_fuzzySearch(struct bookInformation* p, char *key);
	
	/*根据作者名在图书信息链表里模糊搜索，若无结果，则返回NULL；有，则返回该结构体*/
	struct bookInformation* author_fuzzySearch(struct bookInformation* poiter, char *key);
	
	/*借书函数*/
	void borrowBooks(int bookNumber, int userNumber, int year, int month, int day); 
	
	/*还书函数*/
	void returnBooks(int bookNumber, int userNumber); 
	
	/*判断闰年函数*/ 
	int judgeleapyear(int year);
	
	/*稍作解释：界面输入的字符串为明文，password为密文，该函数实现了明文的转换功能*/ 
	/*字符串加密函数，加密采用先异或，后凯撒加密的变种方法，凯撒加密中除了自定offset为偏移量，还添加了元素下标作为偏移量*/  
	char *passwordEncrypt(char p[], int offset);
	
#endif
