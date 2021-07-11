/*
 * 文件：gui.h
 * ---------------------------
 * 图形界面用户交互模块
 * 编写时间：2020.05.22
 */

#include "main.h"
#include "gui.h"
#include "ds.h"
#include "file.h"
#include "search.h"

/*程序界面信息缓冲区*/
struct bookInformation currentBookInformation={0, 0, 0, "", {"~", "~", "~", "~", "~"}, {"~", "~", "~"}, "", "", NULL}; /*初始书籍信息为空*/ 
struct userInformation currentUserInformation={0, {0}, {0}, 1, {0}, NULL, NULL, NULL};
struct bookInformation* newBookInformation = NULL;
struct bookInformation* fuzzySearchPointer = NULL; /*模糊搜索指针，用于搜索下一个直至结束*/ 
struct userInformation currentUserInformation;
struct userInformation* newUserInformation = NULL;
char currentBookNumber[9]={0};
char currentAuthorName[3][31]={{0}, {0}, {0}}; 
char currentKeyword[5][31]={{0}, {0}, {0}}; 
char currentUserNumber[9]={0};
char opUserNumber[9]={0}; /*借书使用的读者ID字符缓冲区*/ 
char fuzzyBookName[81]={0};
char fuzzyAuthorName[31]={0};
char fuzzyKeyword[31]={0};
int authorNumber=0, keywordNumber=0;

void windowsInit(void)
{
	SetWindowTitle("Books Management System");
	SetWindowSize(5, 5);
	InitGraphics();
	/*设定界面颜色*/ 
	usePredefinedMenuColors(4);
	usePredefinedButtonColors(4);
	usePredefinedTexBoxColors(4);
	if(mySettings.musicSwitch)
	{ 
		/*根据系统设置控制背景音乐播放*/ 
		if(mySettings.musicRepeat)
			PlaySound(mySettings.musicPath, NULL, SND_FILENAME|SND_ASYNC|SND_LOOP);
		else
			PlaySound(mySettings.musicPath, NULL, SND_FILENAME|SND_ASYNC);		
	}
}

void drawMenus(void)
{
	double menuButtonWidth, menuListWidth, menuButtonPosX = 0;
	double menuButtonHeight=fontHeight*1.5;
	int menuChoice;
	
	/*绘制菜单栏边框*/ 
	drawMenuBar(0, windowsHeight-menuButtonHeight, windowsWidth, menuButtonHeight); 
	
	/*绘制并处理 文件 菜单*/
	menuButtonWidth=TextStringWidth(menuListFile[0])*1.4;
	menuListWidth=TextStringWidth(menuListFile[1])*1.2;
	menuChoice=menuList(GenUIID(0), menuButtonPosX, windowsHeight-menuButtonHeight,
						menuButtonWidth, menuListWidth, menuButtonHeight, menuListFile, 
						sizeof(menuListFile)/sizeof(menuListFile[0]));
	menuButtonPosX+=menuButtonWidth; 
	switch(menuChoice)
	{
		case 1:
			if(currentFileStatus==nLoaded)
			{
				loadAll();
				currentFileStatus=Saved;
				fuzzySearchPointer=bookhead; /*初始化模糊搜索指针*/ 
			}
			break;
		case 2: 
			saveAll();
			break;
		case 3:
			clearFlags();
			currentFileStatus=nLoaded;
			break;
		case 4:
			exit(0);
		default:
			break;
	}
	
	/*绘制并处理 视图 菜单*/ 
	menuButtonWidth=TextStringWidth(menuListView[0])*1.4;
	menuListWidth=TextStringWidth(menuListView[1])*1.1; 
	menuChoice=menuList(GenUIID(0), menuButtonPosX, windowsHeight-menuButtonHeight,
						menuButtonWidth, menuListWidth, menuButtonHeight, menuListView, 
						sizeof(menuListView)/sizeof(menuListView[0]));
	menuButtonPosX+=menuButtonWidth;
	if(currentFileStatus!=nLoaded)
	{ 
		switch(menuChoice)
		{
			case 1:
				clearFlags();
				clearCurrentBook();
				clearCurrentUser();
				graphicsMode =Books;
				break;
			case 2:
				clearFlags();
				clearCurrentBook();
				clearCurrentUser();
				graphicsMode=Readers;
				break;
			case 3:
				clearFlags();
				clearCurrentBook();
				clearCurrentUser();
				graphicsMode=Stats;
				break;
			default:
				break; 
		}
	}

	//绘制并处理 选项 菜单 
	menuButtonWidth=TextStringWidth(menuListOption[0])*1.3;
	menuListWidth=TextStringWidth(menuListOption[1])*1.1;
	menuChoice=menuList(GenUIID(0), menuButtonPosX, windowsHeight-menuButtonHeight,
						menuButtonWidth, menuListWidth, menuButtonHeight, menuListOption, 
						sizeof(menuListOption)/sizeof(menuListOption[0]));
	menuButtonPosX+=menuButtonWidth; 
	if(currentFileStatus!=nLoaded)
	{
		switch(menuChoice)
		{
			case 1:
				clearFlags();
				clearCurrentBook();
				clearCurrentUser();
				graphicsMode = Settings;
				break;
			default:
				break; 
		}
	}
	if(menuChoice==2){
		currentFileStatus=nLoaded; /*更改文件状态为已卸载*/
		strcpy(currentPassword, ""); /*上次登陆时的密码置空，防止明文泄露*/
		clearFlags();
		clearCurrentBook();
		clearCurrentUser(); 
		isLogin=0; /*退出登陆状态*/ 
	}

	//绘制 帮助 菜单 
	menuButtonWidth = TextStringWidth(menuListHelp[0])*1.3;
	menuListWidth = TextStringWidth(menuListHelp[1])*1.05;
	menuList(GenUIID(0), menuButtonPosX, windowsHeight-menuButtonHeight,
			 menuButtonWidth, menuListWidth, menuButtonHeight, menuListHelp, 
			 sizeof(menuListHelp)/sizeof(menuListHelp[1]));
}

void drawStatusBar(void)
{
	double labelOffset=0.1;
	setPen();
	drawRectangle(0, 0, windowsWidth, fontHeight*1.2, 0);
	drawLabel(labelOffset, fontHeight/3, "Current Mode:");
	labelOffset+=TextStringWidth("Current Mode:");
	if(currentFileStatus!=nLoaded)
	{
		switch(graphicsMode)
		{
		case Readers:
			drawLabel(labelOffset, fontHeight/3, "Readers Management");
			break;
		case Books: 
			drawLabel(labelOffset, fontHeight/3, "Books Management");
			break;
		case Stats:
			drawLabel(labelOffset, fontHeight/3, "Data Analysis");
			break;
		case Settings:
			drawLabel(labelOffset, fontHeight/3, "Global Settings");
			break;
		default:
			break;
		}
	}
	else if(graphicsMode==Login) drawLabel(labelOffset, fontHeight/3, "Please Login!");
	else drawLabel(labelOffset, fontHeight/3, "Home Page");
	if(isLogin==1)
	{
		switch(currentFileStatus)
		{
		case Saved:
			drawLabel(windowsWidth-TextStringWidth("Saved")-0.1, fontHeight/3, "Saved");
			break;
		case nSaved:
			drawLabel(windowsWidth-TextStringWidth("Not Saved!")-0.1, fontHeight/3, "Not Saved!");
			break;
		case nLoaded:
			drawLabel(windowsWidth-TextStringWidth("Please Open File!")-0.1, fontHeight/3, "Please Open File!");
			break;
		default:
			break;
		}
	}
}

void drawInnerContent(void)
{
	switch(graphicsMode)
	{
		case Login:
			funcLogin();
			break;
		case Readers:
			if(currentFileStatus!=nLoaded)
				funcReadersManagement();
			else funcHomepage();
			break;
		case Books:
			if(currentFileStatus!=nLoaded)
				funcBooksManagement();
			else funcHomepage();
			break;
		case Stats:
			if(currentFileStatus!=nLoaded)
				funcStatInfo();
			else funcHomepage();
			break;
		case Settings:
			if(currentFileStatus!=nLoaded)
				funcSettings();
			else funcHomepage();
			break;
		case bookLend:
			funcLendBook();
			break;
		case bookReturn:
			funcReturnBook();
			break;
		default:
			break;
	}
}
	
void funcLogin(void)
{
	SetPenColor("Blue");
	drawLabel(0.2, windowsHeight - 0.5, ctime(&timep));
	SetPenColor("Orange");
	drawLabel(windowsWidth/2-TextStringWidth("Welcome to Book Management System!")/2, windowsHeight-1.2,
			  "Welcome to Book Management System!");
	SetPenColor("Red");
	drawLabel(windowsWidth/2-TextStringWidth("Please Enter Password of Administrator:")/2, windowsHeight-1.8,
			  "Please Enter Password of Administrator:");
	textbox(GenUIID(0), windowsWidth/2-TextStringWidth("xxxxxxxx")/2*1.6, windowsHeight-2.8, 
			TextStringWidth("xxxxxxxx")*1.6, fontHeight*1.5, currentPassword,9);
	if(button(GenUIID(0), windowsWidth/2-1.2, windowsHeight-4, TextStringWidth("Login")+0.2, fontHeight+0.2, "Login"))
	{
		clearFlags();
		chkPasswd();
	}
	if(button(GenUIID(0),windowsWidth/2+0.6, windowsHeight-4, TextStringWidth("Exit")+0.3, fontHeight+0.2, "Exit"))
	{
		exit(0);
	}
} 

void funcBooksManagement(void)
{
	int i; /*循环变量*/ 
	/*绘制左侧提示标签*/ 
	setPen();
	drawLabel(0.2, windowsHeight-0.6, "Book ID:");
	drawLabel(0.2, windowsHeight-1.1, "Name:");
	drawLabel(0.2, windowsHeight-1.55, "Catagory:");
	drawLabel(0.2, windowsHeight-2.1, "Keyword:");
	drawLabel(0.2, windowsHeight-2.6, "Author:");
	drawLabel(0.2, windowsHeight-3.1, "Publisher:");
	drawLabel(0.2, windowsHeight-3.6, "Pub. Date:");
	
	/*处理书号输入框*/ 
	if(currentBookInformation.bookNumber!=0) /*注意：书号不支持以0开头，最多8位*/ 
		itoa(currentBookInformation.bookNumber,currentBookNumber,10); /*将“当前图书”结构中的书号整数转化为字符串显示*/ 
	else currentBookNumber[0]='\0';
	textbox(GenUIID(0), 1, windowsHeight-0.6-0.05, TextStringWidth("01234567")*1.3, fontHeight*1.2, currentBookNumber, 9); 
	if(currentBookNumber[0]!='\0')
		currentBookInformation.bookNumber=atoi(currentBookNumber); /*将字符串书号转化为整数存入“当前图书”结构*/
	else currentBookInformation.bookNumber=0;
	
	/*绘制书名输入框*/ 
	textbox(GenUIID(0), 1, windowsHeight-1.1-0.05, TextStringWidth("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")*1.2, fontHeight*1.2,
			currentBookInformation.bookName,31); /*书名最多三十个字*/ 
	
	/*绘制类别标签和切换按钮*/ 
	if(button(GenUIID(0), 4, windowsHeight-1.65, TextStringWidth("Prev")+0.1, fontHeight+0.2, "Prev"))
		if(currentBookInformation.booktypeFlag>0) currentBookInformation.booktypeFlag--;
	if(button(GenUIID(0), 4.45, windowsHeight-1.65,TextStringWidth("Next")+0.1, fontHeight+0.2, "Next"))
		if(currentBookInformation.booktypeFlag < 4) currentBookInformation.booktypeFlag++;
	SetPenColor("Black");
	drawLabel(1, windowsHeight-1.55, booktypeName[currentBookInformation.booktypeFlag]);
	
	/*绘制关键字输入框*/
	setPen();
	for(i=0; i<5; i++)
	{
		if(!strcmp(currentBookInformation.keyword[i], "~"))
			strcpy(currentKeyword[i], "");
		else strcpy(currentKeyword[i], currentBookInformation.keyword[i]);
	}
	textbox(GenUIID(0), 1, windowsHeight-1.95-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentKeyword[0], 11); //每个关键词最多10个字 
	textbox(GenUIID(0), 2.37, windowsHeight-1.95-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentKeyword[1], 11); 
	textbox(GenUIID(0), 3.75, windowsHeight-1.95-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentKeyword[2], 11); 
	textbox(GenUIID(0), 1, windowsHeight-2.25-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentKeyword[3], 11);
	textbox(GenUIID(0), 2.37, windowsHeight-2.25-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentKeyword[4], 11); 
	
	for(i=0; i<5; i++)
	{
		if(!strcmp(currentKeyword[i], ""))
			strcpy(currentBookInformation.keyword[i], "~");
		else strcpy(currentBookInformation.keyword[i], currentKeyword[i]);
	}	
	
	/*绘制作者输入框*/
	for(i=0; i<3; i++)
	{
		if(!strcmp(currentBookInformation.authorName[i], "~"))
			strcpy(currentAuthorName[i], "");
		else strcpy(currentAuthorName[i], currentBookInformation.authorName[i]);
	} 
	textbox(GenUIID(0), 1, windowsHeight-2.6-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentAuthorName[0], 11); /*作者姓名最多10个字*/ 
	textbox(GenUIID(0), 2.37, windowsHeight-2.6-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentAuthorName[1], 11); 
	textbox(GenUIID(0), 3.75, windowsHeight- 2.6-0.05, TextStringWidth("xxxxxxxxxx")*1.2, fontHeight*1.2, currentAuthorName[2], 11); 
	for(i=0; i<3; i++)
	{
		if(!strcmp(currentAuthorName[i], ""))
			strcpy(currentBookInformation.authorName[i], "~");
		else strcpy(currentBookInformation.authorName[i], currentAuthorName[i]);
	}
	
	 
	/*绘制出版社信息输入框*/ 
	textbox(GenUIID(0), 1, windowsHeight-3.1-0.05, TextStringWidth("xxxxxxxxxxxxxxxxxxxxxxxxxxxxx")*1.2, fontHeight*1.2, currentBookInformation.publishingHouse, 31); /*出版社名称最多29个字*/ 
	textbox(GenUIID(0), 1, windowsHeight-3.6-0.05, TextStringWidth("0000-00-00")*1.2, fontHeight*1.2, currentBookInformation.publicationDate, 11);
	
	//绘制分割线
	drawHorizontalLine("Blue", 0.8);
	drawHorizontalLine("Blue", 1.25);
	drawHorizontalLine("Blue", 1.74);
	drawHorizontalLine("Blue", 2.37);
	drawHorizontalLine("Blue", 2.8);
	drawHorizontalLine("Blue", 3.3);
	drawHorizontalLine("Blue", 3.8);
	
	/*更新可选文本框计数*/ 
	countNumber();
	
	/*绘制功能按钮*/	
	if(button(GenUIID(0), 0.3, windowsHeight-4.5, TextStringWidth("Add")+0.2, fontHeight+0.2, "Add"))
	{
		clearFlags(); 
		isInputValid();
		if(!dateErr && !inputErr && currentBookInformation.bookNumber!=0 && 
	      (booknumberSearch(currentBookInformation.bookNumber))==NULL)/*不对已有的书号重复添加，防止日后查找出错*/ 
		{ 
			bookAddition_Change(1, currentBookInformation.bookNumber, currentBookInformation.booktypeFlag, currentBookInformation.bookName,
								currentBookInformation.keyword, keywordNumber, currentBookInformation.authorName, authorNumber,
								currentBookInformation.publishingHouse, currentBookInformation.publicationDate);
			isSucceed=1; /*提示操作成功*/ 
			currentFileStatus=nSaved; /*文件状态提醒用户及时保存*/ 
		}else if(booknumberSearch(currentBookInformation.bookNumber)!=NULL)
			isExist=1;
	}
	if(button(GenUIID(0), 0.9, windowsHeight - 4.5, TextStringWidth("Search") + 0.2, fontHeight + 0.2, "Search"))
	{
		clearFlags();
		bookSearch();
	}
	if(button(GenUIID(0), 3.95, windowsHeight - 0.68, TextStringWidth("Fuzzy Log") + 0.2, fontHeight + 0.2, "Fuzzy Log"))/*快速恢复上一次模糊搜索的关键词*/
	{  
		fuzzyLog();
	}
	if(button(GenUIID(0), 1.7, windowsHeight - 4.5, TextStringWidth("Modify") + 0.2, fontHeight + 0.2, "Modify"))
	{
		clearFlags(); 
		isInputValid();
		if(!dateErr && !inputErr && currentBookInformation.bookNumber != 0 && 
		  (booknumberSearch(currentBookInformation.bookNumber)) != NULL)
		{
			bookAddition_Change(0, currentBookInformation.bookNumber, currentBookInformation.booktypeFlag, currentBookInformation.bookName,
								currentBookInformation.keyword, keywordNumber, currentBookInformation.authorName, authorNumber,
								currentBookInformation.publishingHouse, currentBookInformation.publicationDate);
			isSucceed = 1;
			currentFileStatus = nSaved;
		}
		else if(booknumberSearch(currentBookInformation.bookNumber)==NULL)
			notFound = 1;
	}
	if(button(GenUIID(0), 2.5, windowsHeight - 4.5, TextStringWidth("Delete") + 0.2, fontHeight + 0.2, "Delete"))
	{
		clearFlags();
		if(booknumberSearch(currentBookInformation.bookNumber) != NULL)
		{
			bookDelete(currentBookInformation.bookNumber);
			clearCurrentBook(); 
			isSucceed = 1;
			currentFileStatus = nSaved;
		} 
		else notFound = 1;
	}
	if(button(GenUIID(0), 3.3, windowsHeight - 4.5, TextStringWidth("Clear") + 0.2, fontHeight + 0.2, "Clear"))
	{
		clearFlags();
		clearCurrentBook();
	}
	if(booknumberSearch(currentBookInformation.bookNumber) != NULL){
		if(book_and_userRelation_search(currentBookInformation.bookNumber) == NULL)/*根据书是否已被借出显示不同的功能按钮*/
		{  
			if(button(GenUIID(0), 4.05, windowsHeight - 4.5, TextStringWidth("Lend") + 0.2,fontHeight + 0.2, "Lend"))
			{
				int tmpBookNumber = currentBookInformation.bookNumber;
				clearCurrentBook();
				currentBookInformation.bookNumber = tmpBookNumber; /*只保留当前输入的书号*/ 
				countNumber(); /*进行一次计数，防止因刷新不及时导致搜索模式判断出错*/  
				bookSearch(); /*进行一次精确搜索，保证数据准确*/ 
				clearFlags(); /*清除可能的错误信息*/ 
				strcpy(opUserNumber,""); /*清空借书人ID的字符缓冲区*/ 
				graphicsMode = bookLend; /*切换到借书界面*/ 
			}	
		}
		else
		{
			if(button(GenUIID(0),4.05,windowsHeight - 4.5,TextStringWidth("Return") + 0.2,fontHeight + 0.2,"Return"))
			{
				int tmpBookNumber = currentBookInformation.bookNumber;
				clearCurrentBook();
				currentBookInformation.bookNumber = tmpBookNumber;
				countNumber(); /*进行一次计数，防止因刷新不及时导致搜索模式判断出错*/ 
				bookSearch();
				clearFlags();
				graphicsMode = bookReturn;
			}
		}
	}
	else
	{
		button(GenUIID(0),4.05,windowsHeight - 4.5,TextStringWidth("N/A") + 0.2,fontHeight + 0.2,"N/A"); /*找不到图书，按钮不可用*/ 
	}	
}

void funcReadersManagement(void)
{	
	/*绘制左侧提示标签*/ 
	setPen();
	drawLabel(0.2, windowsHeight - 0.6, "Reader ID:");
	drawLabel(0.2, windowsHeight - 1.2, "Name:");
	drawLabel(0.2, windowsHeight - 1.8, "Gender:");
	drawLabel(0.2, windowsHeight - 2.4, "Company:");
	drawLabel(0.2, windowsHeight - 2.9, "Books to Return:");
	
	/*绘制读者ID输入框*/ 
	if(currentUserInformation.userNumber != 0)
		itoa(currentUserInformation.userNumber,currentUserNumber,10); 
	else currentUserNumber[0] ='\0';
	if(textbox(GenUIID(0),1,windowsHeight - 0.6 - 0.05,TextStringWidth("01234567") * 1.3,fontHeight * 1.2,currentUserNumber,9))
	{
		clearCurrentUser(); /*读者ID被编辑，则清空屏幕信息*/ 
	}
	if(currentUserNumber[0] != '\0')
		currentUserInformation.userNumber = atoi(currentUserNumber); 
	else currentUserInformation.userNumber = 0;
	
	/*绘制读者姓名输入框*/ 
	textbox(GenUIID(0), 1, windowsHeight - 1.2 - 0.05, TextStringWidth("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx") * 1.3, 
			fontHeight * 1.2, currentUserInformation.userName,31); 
	
	/*绘制性别标签和切换按钮*/ 
	setPen();
	SetPenColor("Black");
	if(currentUserInformation.genderFlag == 0) 
		drawLabel(1.1, windowsHeight - 1.8, "Female");
	else if(currentUserInformation.genderFlag == 1)
		drawLabel(1.1, windowsHeight - 1.8, "Male");
	if(button(GenUIID(0), 2, windowsHeight - 1.8 -0.1, TextStringWidth("Select") + 0.2, fontHeight + 0.2, "Select"))
		currentUserInformation.genderFlag = !currentUserInformation.genderFlag;
	
	SetPenColor("Blue");
	/*绘制读者公司输入框*/ 
	textbox(GenUIID(0), 1, windowsHeight - 2.4 - 0.05, TextStringWidth("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx") * 1.3, fontHeight * 1.2, 
	        currentUserInformation.workUnits,31); 
	
	/*显示在借书目（至多显示3本*/ 
	SetPenColor("Black");
	struct user_borrowing_record *p = currentUserInformation.head;
	char bookItem[81]="";
	char returnDate[11];
	char borrowDate[11];
	int cnt = 0,dcnt = 0; /*总未还条目统计，显示未还条目统计*/ 
	while(p != NULL)
	{
		if(p->returnFlag == 0)
		{
			if(dcnt < 3)
			{
				itoa(p->bookNumber, bookItem, 10);
				strcat(bookItem, "    ");
				strcat(bookItem, p->bookName);
				strcat(bookItem, "    ");
				sprintf(borrowDate, "%4d-%2d-%2d", book_and_userRelation_search(p->bookNumber)->borrowyear,
						book_and_userRelation_search(p->bookNumber)->borrowmonth,
						book_and_userRelation_search(p->bookNumber)->borrowday);
				strcat(bookItem, borrowDate);
				strcat(bookItem, "    ");
				sprintf(returnDate, "%4d-%2d-%2d", book_and_userRelation_search(p->bookNumber)->returnyear,
						book_and_userRelation_search(p->bookNumber)->returnmonth,
						book_and_userRelation_search(p->bookNumber)->returnday);
				strcat(bookItem,returnDate);
				drawLabel(0.5,windowsHeight - 2.9 - 0.2 * (dcnt + 1), bookItem);
				dcnt++;
			}
			cnt++;
		}
		p = p->next;
	}
	if(dcnt < cnt) drawLabel(windowsWidth / 2 - TextStringWidth("----------More Books to Return----------") / 2, 
	    					 windowsHeight - 3.7,"----------More Books to Return----------");
	if(cnt == 0) drawLabel(windowsWidth / 2 - TextStringWidth("----------No Book to Return----------") / 2, 
						   windowsHeight - 3.3,"----------No Book to Return----------");
	if(cnt != 0) drawLabel(0.2 + TextStringWidth("Books to Return:"),windowsHeight - 2.9,
						   "[Book ID,Book Name,Borrow Date,Return Date]");

	/*查询按钮*/ 
	if(button(GenUIID(0), 2.7, windowsHeight - 0.7, TextStringWidth("Query") + 0.2, fontHeight + 0.2, "Query"))
	{
		clearFlags(); 
		if(newUserInformation = usernumberSearch(currentUserInformation.userNumber))
		{
			strcpy(currentUserInformation.userName,newUserInformation->userName);
			strcpy(currentUserInformation.workUnits,newUserInformation->workUnits);
			currentUserInformation.genderFlag = newUserInformation->genderFlag;
			currentUserInformation.head = newUserInformation->head;
			currentUserInformation.tail = newUserInformation->tail;
		}
		else notFound = 1;
	}
	
	//注册按钮 
	if(button(GenUIID(0), 0.6, windowsHeight - 4.5, TextStringWidth("Register") + 0.2, fontHeight + 0.2, "Register"))
	{
		clearFlags(); 
		if(currentUserInformation.userNumber != 0 && usernumberSearch(currentUserInformation.userNumber) == NULL)
		{
			userAddition_Change(1, currentUserInformation.userNumber, currentUserInformation.userName, 
								currentUserInformation.genderFlag, currentUserInformation.workUnits); 
			isSucceed = 1;
			currentFileStatus = nSaved;
		}
		else isExist = 1;
	}
	
	/*修改按钮*/ 
	if(button(GenUIID(0), 2.2, windowsHeight - 4.5, TextStringWidth("Modify") + 0.2, fontHeight + 0.2, "Modify"))
	{
		clearFlags(); 
		if(currentUserInformation.userNumber != 0 && usernumberSearch(currentUserInformation.userNumber) != NULL)
		{
			userAddition_Change(0,currentUserInformation.userNumber,currentUserInformation.userName,
								currentUserInformation.genderFlag,currentUserInformation.workUnits); 	
			isSucceed = 1;
			currentFileStatus = nSaved;
		}
		else notFound = 1;
	}
	
	/*删除按钮*/
	if(button(GenUIID(0), 3.7, windowsHeight - 4.5, TextStringWidth("Delete") + 0.2, fontHeight + 0.2, "Delete"))
	{
		clearFlags();  
		if(usernumberSearch(currentUserInformation.userNumber) != NULL)
		{
			while(p != NULL){
				if(p->returnFlag == 0)
					cnt++;
				p = p->next;
			}
			if(cnt == 0){
				userDelete(currentUserInformation.userNumber);	
				clearCurrentUser();
				isSucceed = 1;
				currentFileStatus = nSaved;
			}else notEmpty = 1; 
		}
		else notFound = 1;
	}
	
	/*绘制功能区分割线*/ 
	drawHorizontalLine("Blue", 2.65);
	drawHorizontalLine("Blue", 3.8);
}

void funcLendBook(void){
	static char ch_bookNumber[9]; /*书号临时字符缓冲区*/ 
	static char ch_borrowingDays[5];
	setPen();
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Basic Information") / 2, windowsHeight - 0.4, "Basic Information");
	
	SetPenColor("Blue");
	drawLabel(0.2, windowsHeight - 1, "Book Number:");
	itoa(currentBookInformation.bookNumber, ch_bookNumber, 10);
	drawLabel(1.2, windowsHeight - 1, ch_bookNumber);
	
	drawLabel(0.2, windowsHeight - 1.6, "Book Name:");
	drawLabel(1.1, windowsHeight - 1.6, currentBookInformation.bookName);

	drawLabel(0.2, windowsHeight - 2.2, "Current Time:");
	drawLabel(1.2, windowsHeight - 2.2, ctime(&timep));
	
	drawLabel(0.2, windowsHeight - 2.8, "Maximum Lease Term:");
	itoa(borrowingDays, ch_borrowingDays, 10);
	drawLabel(1.8, windowsHeight - 2.8, ch_borrowingDays); 
	drawLabel(1.85 + TextStringWidth(ch_borrowingDays), windowsHeight - 2.8, "Day(s)"); 
	
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Please Enter Reader ID") / 2, windowsHeight - 3.3, "Please Enter Reader ID");
	textbox(GenUIID(0), windowsWidth / 2 - TextStringWidth("0123456789") * 1.3 / 2, windowsHeight - 3.7, 
	        TextStringWidth("0123456789") * 1.3,fontHeight * 1.2,opUserNumber,9);

	if(button(GenUIID(0), 1, windowsHeight - 4.5, TextStringWidth("Cancel") + 0.2, fontHeight + 0.2, "Cancel"))
	{
		clearFlags();
		graphicsMode = Books;
		clearCurrentBook();
	}
	
	if(button(GenUIID(0), 3.5, windowsHeight - 4.5,TextStringWidth("Lend") + 0.2, fontHeight + 0.2, "Lend"))
	{
		clearFlags();
		struct userInformation* curUser = usernumberSearch(atoi(opUserNumber));
		if(curUser)
		{	
			int cnt = 0;
			struct user_borrowing_record* p = curUser->head;
			while(p != NULL){
				if(p->returnFlag == 0)
					cnt++;
				p = p->next;
			}
			if(cnt < borrowingBooks){
				borrowBooks(currentBookInformation.bookNumber, atoi(opUserNumber), splitTime(0), splitTime(1), splitTime(2));
				currentFileStatus = nSaved;
				graphicsMode = Books;
				isSucceed = 1;
				clearCurrentBook();
				saveAll();
			}
			else overLimit = 1; 
		}
		else notFound = 1;	
	}
}

void funcReturnBook(void){
	struct book_and_userRelation *infoBookAndUser = book_and_userRelation_search(currentBookInformation.bookNumber);
	static char bookNumber[9], readerNumber[9], borrowDate[12], returnDate[12], currentDate[12];
	itoa(infoBookAndUser->bookNumber, bookNumber, 10);
	itoa(infoBookAndUser->userNumber, readerNumber, 10);
	sprintf(borrowDate, "%d.%d.%d", infoBookAndUser->borrowyear, infoBookAndUser->borrowmonth, infoBookAndUser->borrowday);
	sprintf(returnDate, "%d.%d.%d", infoBookAndUser->returnyear, infoBookAndUser->returnmonth, infoBookAndUser->returnday);
	sprintf(currentDate, "%d.%d.%d", splitTime(0), splitTime(1), splitTime(2));
	
	setPen();
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Book Overview") / 2, windowsHeight - 0.4, "Book Overview");
	
	SetPenColor("Blue");
	drawLabel(1, windowsHeight - 0.7, "Book Number:");
	drawLabel(2.05, windowsHeight - 0.7, bookNumber);
	drawLabel(1, windowsHeight - 1, "Book Name:");
	drawLabel(1.9, windowsHeight - 1, currentBookInformation.bookName);
	drawLabel(1, windowsHeight - 1.3,"Book Category:");
	drawLabel(2.1, windowsHeight - 1.3, booktypeName[currentBookInformation.booktypeFlag]);
	
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Reader Information") / 2, windowsHeight - 1.7, "Reader Information");
	
	SetPenColor("Blue");
	drawLabel(1, windowsHeight - 2, "Reader ID:");
	drawLabel(1.8, windowsHeight - 2, readerNumber);
	drawLabel(1, windowsHeight - 2.3, "Reader Name:");
	drawLabel(2, windowsHeight - 2.3, usernumberSearch(infoBookAndUser->userNumber)->userName);
	drawLabel(1, windowsHeight - 2.6, "Gender:");
	switch(usernumberSearch(infoBookAndUser->userNumber)->genderFlag)
	{
		case 0:
			drawLabel(1.55,windowsHeight - 2.6, "Female");
			break;
		case 1:
			drawLabel(1.55,windowsHeight - 2.6, "Male");
			break;
		default:
			break;
	}
	
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Lease Information") / 2, windowsHeight - 3, "Lease Information");
	
	SetPenColor("Blue");
	drawLabel(1, windowsHeight - 3.3, "Borrow Date:");
	drawLabel(2, windowsHeight - 3.3, borrowDate);
	drawLabel(1, windowsHeight - 3.6, "Date to Return:");
	drawLabel(2.05, windowsHeight - 3.6, returnDate);
	drawLabel(1, windowsHeight - 3.9, "Current Date:");
	drawLabel(2, windowsHeight - 3.9, currentDate);
	
	if(button(GenUIID(0), 1, windowsHeight - 4.6, TextStringWidth("Cancel") + 0.2, fontHeight + 0.2, "Cancel")){
		clearFlags();
		graphicsMode = Books;
		clearCurrentBook();
	}
	
	if(button(GenUIID(0), 3.4, windowsHeight - 4.6, TextStringWidth("Return") + 0.2, fontHeight + 0.2, "Return")){
		clearFlags();
		returnBooks(currentBookInformation.bookNumber, infoBookAndUser->userNumber);
		currentFileStatus = nSaved;
		graphicsMode = Books;
		isSucceed = 1;
		clearCurrentBook();
		saveAll();
	}
}

void funcStatInfo(void){
	int i;
	char *bookNumber[9]; /*图书数量字符缓冲区*/ 
	static bool sortType; /*0-图书排序，1-图书排序*/
	
	setPen();
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Book Category Statistic") / 2, 
			  windowsHeight - 0.5, "Book Category Statistic");
	
	SetPenColor("Blue");
	for(i = 0; i < 5; i++)
		drawLabel(0.2, windowsHeight - 0.8 - 0.4 * i, booktypeName[i]);
	for(i = 0; i < 5; i++)
		drawLabel(1, windowsHeight - 1 - 0.4 * i, "Total:");
	for(i = 0; i < 5; i++)
		drawLabel(2.8, windowsHeight - 1 - 0.4 * i, "Borrowed:");
	SetPenColor("Black");
	for(i = 0;i < 5;i++)
	{
		itoa(booktype[i][0], bookNumber, 10); 
		drawLabel(1.8, windowsHeight - 1 - 0.4 * i, bookNumber);
	}
	for(i = 0; i < 5; i++)
	{
		itoa(booktype[i][1], bookNumber, 10); 
		drawLabel(3.9, windowsHeight - 1 - 0.4 * i, bookNumber);
	}
	
	drawHorizontalLine("Blue", 2.8);
	
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Sort") / 2, windowsHeight - 3, "Sort");
	
	SetPenColor("Blue");
	drawLabel(0.2, windowsHeight - 3.4, "Sort by:");
	if(button(GenUIID(0), 1.8, windowsHeight - 3.5, TextStringWidth("Switch") + 0.2, fontHeight + 0.2, "Switch"))
		sortType = !sortType;
	SetPenColor("Black");
	switch(sortType)
	{
		case 0:
			drawLabel(0.25 + TextStringWidth("Sort by:"), windowsHeight - 3.4, "Book Name");
			break;
		case 1:
			drawLabel(0.25 + TextStringWidth("Sort by:"), windowsHeight - 3.4, "Reader Name");
			break;
		default:
			break;
	}
	char *buttonText[2]={"Ascending", "Descending"};
	for(i = 0;i < 2;i++)
	{
		if(button(GenUIID(i), 2.8 + 1 * i, windowsHeight - 3.5, TextStringWidth(buttonText[i]) + 0.2, 
		   fontHeight + 0.2,buttonText[i]))
		{
			clearFlags();
			switch(i){
				case 0:
					if(sortType)
						user_sort(0);
					else book_sort(0);
					break;
				case 1:
					if(sortType)
						user_sort(1);
					else book_sort(1);
					break;
				default:
					break;
			}
			if(sortType)
				userSort_add();
			else bookSort_add();
			isSortFin = 1;
		}	
	}
	
	drawHorizontalLine("Blue", 3.85);
	
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Recent Top 3") / 2, windowsHeight - 4.01, "Recent Top 3");
	drawLabel(1,0.85, "Books");
	drawLabel(3.7,0.85, "Readers");
	struct bookInformation *bestFiveBook = SORT_book();
	struct userInformation *bestFiveUser = SORT_user();
	
	SetPenColor("Black");
	char bookInfo1[20];
	char bookInfo2[50];
	int cnt = 0;
	while(bestFiveBook != NULL && cnt < 3)
	{
		sprintf(bookInfo1, "(BID:%d)", bestFiveBook->bookNumber);
		strcpy(bookInfo2, bestFiveBook->bookName);
		strcat(bookInfo2, bookInfo1);
		drawLabel(0.2, windowsHeight - 4.3 - cnt * 0.2, bookInfo2);
		cnt++;
		bestFiveBook = bestFiveBook->next;
	}
	
	char userInfo[46];
	cnt = 0;
	while(bestFiveUser != NULL && cnt < 3)
	{
		sprintf(userInfo, "%s(UID:%d)", bestFiveUser->userName, bestFiveUser->userNumber);
		drawLabel(3.6, windowsHeight - 4.3 - cnt * 0.2, userInfo);
		cnt++;
		bestFiveUser = bestFiveUser->Next;
	}
		
}

void funcSettings(void)
{
	static char autoSaveInterval[13]; /*自动保存时间的字符缓冲区*/ 
	static char oldPasswd[9];
	static char newPasswd[9];
	static char confirmPasswd[9]; /*密码修改部分的字符缓冲区*/ 
	double offset = 0.5;
	
	setPen();
	drawLabel(0.2 + offset, windowsHeight - 0.7, "Auto-Save ON/OFF:");
	if(mySettings.autoSaveSwitch) drawLabel(2 + offset, windowsHeight - 0.7, "ON");
	else drawLabel(2 + offset, windowsHeight - 0.7, "OFF");
	if(button(GenUIID(0), 3 + offset, windowsHeight - 0.7 - 0.1, TextStringWidth("ON/OFF") + 0.2, fontHeight + 0.2, "ON/OFF"))
	{
		mySettings.autoSaveSwitch = !mySettings.autoSaveSwitch;
		currentFileStatus = nSaved;
	}
	
	setPen();
	drawLabel(0.2 + offset, windowsHeight - 1.2, "Auto-Save Interval:");
	itoa(mySettings.autoSaveTime, autoSaveInterval, 10);
	if(textbox(GenUIID(0), 1.7 + offset, windowsHeight - 1.2 - 0.05, TextStringWidth("xxxxxxxxxxxxxxxxx") * 1.2,
	   fontHeight * 1.2, autoSaveInterval, 13)) /*音乐文件路径最长35个字符,不得出现中文*/
		currentFileStatus = nSaved; /*如果修改，提示保存*/ 
	mySettings.autoSaveTime = atoi(autoSaveInterval); /*字符缓冲区与整型数据的互相转化*/ 
	setPen();
	drawLabel(3.5 + offset, windowsHeight - 1.2, "ms"); /*提示时间单位*/ 
	
	drawLabel(0.2 + offset, windowsHeight - 1.7, "Music ON/OFF:");
	if(mySettings.musicSwitch) drawLabel(2 + offset, windowsHeight - 1.7, "ON");
	else drawLabel(2 + offset, windowsHeight - 1.7, "OFF");
	if(button(GenUIID(0), 3 + offset, windowsHeight - 1.7 - 0.1, TextStringWidth("ON/OFF") + 0.2, fontHeight + 0.2, "ON/OFF"))
	{
		mySettings.musicSwitch = !mySettings.musicSwitch;
		currentFileStatus = nSaved;
	}
	
	setPen();
	drawLabel(0.2 + offset, windowsHeight - 2.2, "Music Repetition:");
	if(mySettings.musicRepeat) drawLabel(2 + offset, windowsHeight - 2.2, "ON");
	else drawLabel(2 + offset,windowsHeight - 2.2, "OFF");
	if(button(GenUIID(0), 3 + offset, windowsHeight - 2.2 - 0.1, TextStringWidth("ON/OFF") + 0.2, fontHeight + 0.2, "ON/OFF"))
	{
		mySettings.musicRepeat = !mySettings.musicRepeat;
		currentFileStatus = nSaved;
	}
	
	setPen();
	drawLabel(0.2 + offset, windowsHeight - 2.7, "Music Path:"); 
	if(textbox(GenUIID(0), 1.1 + offset, windowsHeight - 2.7 - 0.05, TextStringWidth("xxxxxxxxxxxxxxxxxxxxxxxxxx") * 1.2,
	   fontHeight * 1.2,mySettings.musicPath,30)) /*音乐文件路径最长30个字符,不得出现中文*/
		currentFileStatus = nSaved;
	SetPenColor("Red");
	drawLabel(0.2 + offset, windowsHeight - 3.05, "Note:Absolute Path Only!"); //提示仅绝对路径是有效的 
	
	drawHorizontalLine("Blue",3.35); /*功能设置与密码设置的分割线*/
	 
	double offset_n=0.5;
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 -TextStringWidth("Change Administrator Password") / 2, windowsHeight - 3.55, "Change Administrator Password");
	SetPenColor("Blue");
	drawLabel(0.2 + offset_n, windowsHeight - 3.8, "Old Password:");
	drawLabel(0.2 + offset_n, windowsHeight - 4.2, "New Password:");
	drawLabel(0.2 + offset_n, windowsHeight - 4.6, "Retype to Confirm:");
	textbox(GenUIID(0), 1.9 + offset_n, windowsHeight - 3.8 - 0.05, TextStringWidth("xxxxxxxx") * 1.2, fontHeight * 1.2, oldPasswd, 9);
	textbox(GenUIID(0), 1.9 + offset_n, windowsHeight - 4.2 - 0.05, TextStringWidth("xxxxxxxx") * 1.2, fontHeight * 1.2, newPasswd, 9);
	textbox(GenUIID(0), 1.9 + offset_n, windowsHeight - 4.6 - 0.05, TextStringWidth("xxxxxxxx") * 1.2, fontHeight * 1.2, confirmPasswd, 9);
	if(button(GenUIID(0), 3.5 + offset_n - 0.25, windowsHeight - 4 - 0.1, TextStringWidth("Change") + 0.2, fontHeight + 0.2, "Change"))
	{
		clearFlags();
		if(strcmp(passwordEncrypt(oldPasswd, PASSWD_OFFSET), mySettings.adminPassward) != 0)
		{
			errPasswd = 1; /*原密码错误*/ 
			strcpy(oldPasswd, "");
			strcpy(newPasswd, "");
			strcpy(confirmPasswd, ""); /*清空密码框，方便下次输入并防止密码泄露*/ 
		}
		else if(strcmp(newPasswd,confirmPasswd) != 0)
		{
			passwdNotMatch = 1; /*两次密码输入不一致*/ 
			strcpy(oldPasswd, "");
			strcpy(newPasswd, "");
			strcpy(confirmPasswd, "");
		}
		else/*满足修改密码的条件*/ 
		{ 
			passwdChangeOK = 1;
			strcpy(mySettings.adminPassward, passwordEncrypt(newPasswd, PASSWD_OFFSET));
			strcpy(oldPasswd, "");
			strcpy(newPasswd, "");
			strcpy(confirmPasswd, "");
			currentFileStatus = nSaved;
		} 
	}
}

void funcHomepage(void)
{
	setPen();
	drawLabel(0.2, windowsHeight - 0.6, ctime(&timep));
	SetPenColor("Red");
	drawLabel(windowsWidth / 2 - TextStringWidth("Welcome!") / 2, windowsHeight - 0.9, "Welcome!");
	drawLabel(windowsWidth / 2 - TextStringWidth("Please Load the Database First!") / 2, windowsHeight - 1.3, "Please Load the Database First!");
	SetPenColor("Orange");
	drawLabel(0.2, windowsHeight - 1.7, "Overall Operation Instructions:");
	SetPenColor("Black");
	drawLabel(0.2, windowsHeight - 2.1, "Step1:File-Open to load the database.");
	drawLabel(0.2 + TextStringWidth("Step0:"), windowsHeight - 2.4, "Books Management Page will automatically pop out.");
	drawLabel(0.2, windowsHeight - 2.8, "Step2:Use View/Option Menu to switch Page Modes.");
	drawLabel(0.2 + TextStringWidth("Step0:"), windowsHeight - 3.1, "Available Pages:Books/Readers/Statistics/Settings/Password.");
	drawLabel(0.2, windowsHeight - 3.5, "Step3:File-Save to update database,then File-Close.");
	SetPenColor("Red");
	drawLabel(0.2 + TextStringWidth("Step0:"), windowsHeight - 3.8, "Any change without a saving WILL NOT come into your disk!");
	SetPenColor("Orange");
	drawLabel(0.2, windowsHeight - 4.2, "Note:More detailed Operation Manual is in README.md");
	drawLabel(0.2 + TextStringWidth("Note:"), windowsHeight - 4.5, "You can find this page at any time after File-Close.");
}

void saveAll(void)
{
	if(currentFileStatus != nLoaded && currentFileStatus == nSaved)
	{
		bookDocument_add();
		userDocument_add();
		relationDocument_add(); /*保存数据库*/ 
		systemSettings_add(); /*保存系统设定*/ 
		currentFileStatus = Saved;
		loadAll();
	} 
}

void loadAll(void)
{
	bookhead=NULL;
	booktail=NULL;					   		
	bookformer=NULL;
	userhead=NULL;
	usertail=NULL;
	userformer=NULL; 
	relationhead=NULL;
	relationtail=NULL;
	relationformer=NULL;
	bookDocument_load();
	userDocument_load();
	relationDocument_load();
}

void displayErrMessages(void)
{
	setPen();
	if(graphicsMode == Books)
	{
		if(dateErr && !isExist && !inputErr) /*输出日期格式错误信息（如果有），日期错误优先级低于添加冲突错误优先级*/ 
			drawLabel(windowsWidth / 2 - TextStringWidth("Pub.Date Invalid!Format:YYYY-MM-DD") / 2, 1, "Pub.Date Invalid! Format:YYYY-MM-DD");
		if(queryErr) /*输出检索输入错误信息（如果有）*/ 
			drawLabel(windowsWidth / 2 - TextStringWidth("Wrong Input of Searching!Book ID/Book Name/Keyword/Author Only!") / 2,1,"Wrong Input of Searching!Book ID/Book Name/Author Name Only!");
		if(notFound && !inputErr) /*输出检索结束信息*/ 
			drawLabel(windowsWidth / 2 - TextStringWidth("Target Not Found or Search Finished!") / 2, 1, "Target Not Found or Search Finished!");	
		if(isExist) /*输出添加冲突信息*/ 
			drawLabel(windowsWidth / 2 - TextStringWidth("Book Already Exists and Could Not Added Again!") / 2, 1, "Book Already Exists and Could Not Added Again!");		
		if(inputErr) /*输出信息缺失提示*/ 
			drawLabel(windowsWidth / 2 - TextStringWidth("Book Name and Publisher CAN NOT be Empty!") / 2, 1, "Book Name and Publisher CAN NOT be Empty!");		
	}
	if(graphicsMode == Readers)
	{
		if(notFound)
			drawLabel(windowsWidth / 2 - TextStringWidth("Reader Not Found!") / 2, 1, "Reader Not Found!");	
		if(isExist)
			drawLabel(windowsWidth / 2 - TextStringWidth("Reader Already Exists!") / 2, 1, "Reader Already Exists!");	
		if(notEmpty)
			drawLabel(windowsWidth / 2 - TextStringWidth("Please Return All Books before Delete Reader!") / 2, 1, "Please Return All Books before Delete Reader!");	
	}
	if(isSucceed) drawLabel(windowsWidth / 2 - TextStringWidth("Succeed!") / 2, 1, "Succeed!");
	if(graphicsMode == Login && errPasswd)
		drawLabel(windowsWidth / 2 - TextStringWidth("Wrong Password!") / 2, 1.8, "Wrong Password!");	
	if(graphicsMode == Settings)/*设置界面的错误信息提示*/
	{  
		if(errPasswd)
			drawLabel(3.4, 0.6, "Wrong Old Password!");
		if(passwdNotMatch)
			drawLabel(3.4, 0.6, "Passwords Dismatch!");
		if(passwdChangeOK)
			drawLabel(3.4, 0.6, "Password Change OK!");
	}
	if(graphicsMode == bookLend)/*设置界面的错误信息提示*/
	{  
		if(notFound)
			drawLabel(windowsWidth / 2 - TextStringWidth("Reader Not Found!") / 2, 1, "Reader Not Found!");	
		if(overLimit)
			drawLabel(windowsWidth / 2 - TextStringWidth("Exceed Maximum Amount of Borrowing Books!") / 2, 1, "Exceed Maximum Amount of Borrowing Books!");	
	}
	if(graphicsMode == Stats)/*统计界面排序成功提示*/
	{  
		if(isSortFin)
			drawLabel(windowsWidth / 2 - TextStringWidth("Sort Completed! Result Saved in sort_result.txt") / 2, 1.25, 
			"Sort Completed! Result Saved in sort_result.txt");	
	}
}

void isInputValid(void)
{
	if(currentBookInformation.publicationDate[4] != '-' || currentBookInformation.publicationDate[7] != '-'){
		setPen();
		dateErr = 1;
	}
	else
	{
		dateErr = 0;
	}
	if(strcmp(currentBookInformation.bookName, "") == 0 || strcmp(currentBookInformation.publishingHouse, "") == 0)
	{
		inputErr = 1;
	}
	else inputErr = 0;
}

/*检查检索功能输入有效性并判断是精确搜索还是模糊搜索 
  返回值：0-输入有误；1-按书号精确检索；2-按书名模糊搜索；3-按关键词模糊搜索；4-按作者模糊搜索*/
int isQueryValid(void){ 
	bool haveBookName = (strcmp(currentBookInformation.bookName, "") != 0);
	bool havePubDate = (strcmp(currentBookInformation.publicationDate, "") != 0);
	bool havePubHouse = (strcmp(currentBookInformation.publishingHouse, "") != 0);
	if(havePubDate || havePubHouse)
	{
		clearCurrentBook();
		queryErr = 1;
		return 0; 
	} 
	if(currentBookInformation.bookNumber != 0 && !haveBookName && !keywordNumber && !authorNumber) /*按书号精确查找*/
	{  
		return 1;
	}
	else if(currentBookInformation.bookNumber == 0 && haveBookName && !keywordNumber && !authorNumber) /*按书名模糊查找*/
	{  
		return 2;
	}
	else if(currentBookInformation.bookNumber == 0 && !haveBookName && keywordNumber == 1 && !authorNumber) /*按关键词模糊查找*/
	{  
		return 3;
	}
	else if(currentBookInformation.bookNumber == 0 && !haveBookName && !keywordNumber && authorNumber == 1) /*按作者模糊查找*/
	{  
		return 4;
	}
	else /*其余输入均为无效输入*/ 
	{ 
		clearCurrentBook();
		queryErr = 1;
		return 0; 
	}
}

void chkPasswd(void)
{
	if(strcmp(passwordEncrypt(currentPassword,PASSWD_OFFSET), mySettings.adminPassward) == 0) /*检查密码*/ 
	{ 
		isLogin=1;
		graphicsMode = Books;
	}
	else
	{
		errPasswd = 1;
	} 
}

void bookSearch(void)
{
	clearFlags();
	switch(isQueryValid())
	{
		case 1:
			if(newBookInformation = booknumberSearch(currentBookInformation.bookNumber))
				copyBookInformation();
			else notFound = 1; 
			break;
		case 2:
			if(newBookInformation = bookname_fuzzySearch(fuzzySearchPointer, currentBookInformation.bookName))
			{
				strcpy(fuzzyBookName,currentBookInformation.bookName); /*将当前输入的关键词保存，便于下次查找快速恢复*/ 
				strcpy(fuzzyKeyword, "");
				strcpy(fuzzyAuthorName, "");
				copyBookInformation();
				fuzzySearchPointer = newBookInformation -> next; /*模糊搜索指针前移准备继续搜索*/
				if(fuzzySearchPointer == NULL) fuzzySearchPointer = bookhead; /*如果搜完了就从头开始*/ 
			}
			else
			{
				notFound = 1;
				fuzzySearchPointer = bookhead; /*如果后面找不到也从头开始*/ 
			}
			break;
		case 3:
			if(newBookInformation = keyword_fuzzySearch(fuzzySearchPointer, currentBookInformation.keyword[0]))/*根据第一作者模糊搜索*/ 
			{ 
				strcpy(fuzzyBookName, "");
				strcpy(fuzzyKeyword,currentBookInformation.keyword[0]);
				strcpy(fuzzyAuthorName, "");
				copyBookInformation();
				fuzzySearchPointer = newBookInformation -> next;
				if(fuzzySearchPointer == NULL) fuzzySearchPointer = bookhead;
			}
			else
			{
				notFound = 1;
				fuzzySearchPointer = bookhead; 
			}
			break;
		case 4:
			if(newBookInformation = author_fuzzySearch(fuzzySearchPointer,currentBookInformation.authorName[0]))/*根据第一作者模糊搜索*/ 
			{ 
				strcpy(fuzzyBookName, "");
				strcpy(fuzzyKeyword, "");
				strcpy(fuzzyAuthorName, currentBookInformation.authorName[0]);
				copyBookInformation();
				fuzzySearchPointer = newBookInformation -> next;
				if(fuzzySearchPointer == NULL) fuzzySearchPointer = bookhead;
			}
			else
			{
				notFound = 1;
				fuzzySearchPointer = bookhead; 
			}
			break;
		default:  /*输入错误，什么也不做*/ 
			break;
	}
}

void fuzzyLog(void)
{
	clearCurrentBook();
	strcpy(currentBookInformation.bookName, fuzzyBookName);
	strcpy(currentBookInformation.keyword[0], fuzzyKeyword);
	strcpy(currentBookInformation.authorName[0], fuzzyAuthorName);
}

void clearFlags(void) /*复位屏幕提示相关标志变量*/
{ 
	dateErr = 0;
	queryErr = 0;
	notFound = 0;
	isExist = 0;
	errPasswd = 0;
	passwdNotMatch = 0; 
	passwdChangeOK = 0;
	isSucceed = 0;
	isSortFin = 0;
	inputErr = 0;
	notEmpty = 0;
	overLimit = 0;
}

void setPen(void)/*快速设置默认画笔*/ 
{ 
	SetPenSize(2);
	SetPenColor("Blue");
}

void drawHorizontalLine(char *color,double y) /*绘制水平分割线*/ 
{
	SetPenColor(color);
	MovePen(0, windowsHeight - y);
	DrawLine(windowsWidth, 0);
}

void countNumber(void)
{
	int i;
	authorNumber = 3;
	keywordNumber = 5;
	for(i = 0; i < 3; i++)
	{ 
		if(!strcmp(currentBookInformation.authorName[i], "~"))
			authorNumber--;
	}
	for(i = 0; i < 5; i++)
	{
		if(!strcmp(currentBookInformation.keyword[i], "~"))
			keywordNumber--;
	}
	printf("%d %d\n", authorNumber, keywordNumber);
}

/*转换当前的年月日为整型数据，type=0-年，type=1-月，type=2-日*/ 
int splitTime(int type)
{
	char *curTime = ctime(&timep);
	char curMonth[4] = "";
	curMonth[0] = curTime[4];
	curMonth[1] = curTime[5];
	curMonth[2] = curTime[6];
	curMonth[3] = '\0';
	int month = 0, i;
	char *monthTable[12] ={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
	for(i = 0;i < 12;i++)
	{
		if(strcmp(monthTable[i],curMonth) == 0)
		{
			month = i + 1;
			break;
		}
	}
	switch(type)
	{
		case 0:
			return (*(curTime + 20) - '0') * 1000 + (*(curTime + 21) -'0') * 100 + (*(curTime + 22) - '0') * 10 + (*(curTime + 23) -'0');
			break; /*可有可无*/ 
		case 1:
			return month;
			break;
		case 2:
			return (*(curTime + 8) - '0') * 10 + (*(curTime + 9) - '0');
			break;
		default:
			break;
	}
}

void clearCurrentBook(void)
{
	int i;
	currentBookInformation.bookNumber=0;
	strcpy(currentBookInformation.bookName, "");
	currentBookInformation.booktypeFlag=0;
	for(i = 0; i < 5; i++)
		strcpy(currentBookInformation.keyword[i], "~");
	for(i = 0;i < 3;i++)
		strcpy(currentBookInformation.authorName[i], "~");
	strcpy(currentBookInformation.publishingHouse, "");
	strcpy(currentBookInformation.publicationDate, "");
}

void clearCurrentUser(void)
{
	strcpy(currentUserInformation.userName, "");
	strcpy(currentUserInformation.workUnits, "");
	currentUserInformation.genderFlag = 1;
	currentUserInformation.head = NULL;
	currentUserInformation.tail = NULL;
	currentUserInformation.userNumber = 0;
}

void copyBookInformation(void)
{
	int i;
	currentBookInformation.bookNumber=newBookInformation->bookNumber;
	strcpy(currentBookInformation.bookName, newBookInformation->bookName);
	currentBookInformation.booktypeFlag=newBookInformation->booktypeFlag;
	for(i = 0; i < 5; i++)
		strcpy(currentBookInformation.keyword[i], newBookInformation->keyword[i]);
	for(i = 0; i < 3; i++)
		strcpy(currentBookInformation.authorName[i], newBookInformation->authorName[i]);
	strcpy(currentBookInformation.publishingHouse, newBookInformation->publishingHouse);
	strcpy(currentBookInformation.publicationDate, newBookInformation->publicationDate);
}

void refreshScreen(void)
{
	DisplayClear(); /*刷新屏幕信息前清空屏幕内容，防止留下残像*/ 
	time(&timep); /*更新系统时间*/ 
	windowsWidth = GetWindowWidth();
	windowsHeight = GetWindowHeight(); /*更新窗口宽高变量*/ 
	fontHeight = GetFontHeight(); /*更新字体高度变量*/ 
	if(isLogin == 0) graphicsMode = Login; /*将登陆状态同步至绘图模式*/
	drawInnerContent(); /*重新绘制界面主体内容*/ 
	if(graphicsMode != bookLend && graphicsMode != bookReturn) drawStatusBar(); /*重新绘制状态栏；借还书界面下隐藏状态栏*/ 
	displayErrMessages(); /*重绘错误信息*/ 
	if(isLogin == 1 && graphicsMode != bookLend && graphicsMode != bookReturn) drawMenus(); /*如果已经登陆，重新绘制菜单；借还书界面下隐藏菜单*/ 
}

void mouseHandler(int x, int y, int button, int event)
{
	uiGetMouse(x,y,button,event); /*调用imgui提供的函数处理鼠标事件，对应UIID的控件状态被更新*/ 
	refreshScreen(); /*在屏幕刷新定时器到来前强制重绘所有控件以更新屏幕内容*/ 
}

void keyboardHandler(int key,int event)
{
	uiGetKeyboard(key,event); /*同上*/ 
	switch(event) /*一些自定义快捷键*/ 
	{ 
		case KEY_DOWN:
			switch(key){
				case VK_RETURN: /*登录页下回车键登录*/ 
					if(graphicsMode == Login)
						chkPasswd();
					break;
				case VK_F1: /*图书管理页下F1键清除屏幕内容*/ 
					if(graphicsMode == Books)
						clearCurrentBook();
					break; 
				case VK_F2: /*图书管理页下F2键借还书*/ 
					if(graphicsMode == Books) /*与funcBookManagement()中的借还书按钮逻辑一致*/ 
					{
						if(booknumberSearch(currentBookInformation.bookNumber) != NULL)
						{
							if(book_and_userRelation_search(currentBookInformation.bookNumber) == NULL)
							{
								int tmpBookNumber = currentBookInformation.bookNumber;
								clearCurrentBook();
								currentBookInformation.bookNumber = tmpBookNumber; /*只保留当前输入的书号*/ 
								bookSearch(); /*进行一次精确搜索，保证数据准确*/ 
								clearFlags(); /*清除可能的错误信息*/ 
								strcpy(opUserNumber, ""); /*清空借书人ID的字符缓冲区*/ 
								graphicsMode = bookLend; /*切换到借书界面*/ 
							}
							else
							{
								int tmpBookNumber = currentBookInformation.bookNumber;
								clearCurrentBook();
								currentBookInformation.bookNumber = tmpBookNumber;
								bookSearch();
								clearFlags();
								graphicsMode = bookReturn;
							}
						}
					}
					break; 
				case VK_F3: /*图书管理页下F3键回填模糊搜索历史*/ 
					if(graphicsMode == Books)
						fuzzyLog();
					break;
				case VK_F4: /*图书管理页面下F4键继续搜索*/ 
					if(graphicsMode == Books)
						bookSearch();
					break; 
			} 
			break;
		case KEY_UP: /*按键按下时响应，抬起时不响应*/ 
			break;
		default:
			break;
	}
	refreshScreen(); 
}

void charHandler(char ch)
{
	uiGetChar(ch); /*同上*/ 
	refreshScreen();
}

void timerHandler(int timerID)
{
	if(timerID == timerRefresh)
	{
		refreshScreen();
	}
	else if(timerID ==timerAutoSave)
	{
		saveAll();
	}
}
