/*
 * 文件：gui.h
 * ---------------------------
 * 图形界面用户交互模块 
 * 编写时间：2020.05.22
 */
 
#ifndef _GUI_H_
#define _GUI_H_

/*包含Win32图形库和LibGraphics/IMGUI相关头文件 */
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"
#pragma comment(lib,"winmm.lib") /*引用Windows多媒体库*/ 

/*窗口相关全局变量定义 */
static double windowsWidth,windowsHeight; /*窗口宽高*/
static double fontHeight; /*全局字体高度*/ 
 
/*菜单内容变量的定义*/
static char * menuListFile[] = {"File", "Open  | Ctrl-O", "Save  | Ctrl-S", "Close | Ctrl-D", "Exit    | Ctrl-E"};
static char * menuListView[] = {"View", "Books     | Ctrl-B", "Readers  | Ctrl-R", "Statistic  | Ctrl-I"};
static char * menuListOption[] = {"Option", "System Settings", "Log Out"};
static char * menuListHelp[] = {"Help", "Please read Help.pdf for more information"};

/*定时器相关定义 TimerID Definition*/
#define timerRefresh 1 /*1号定时器用作屏幕刷新*/ 
#define timerAutoSave 2 /*2号定时器用作文件的自动保存功能*/ 
#define timerInteval 2 /*定时器触发时间间隔*/

/*GUI全局变量 */
enum boolean{True, False};
enum mode{
	Login, Readers, Books, Stats, Settings, bookLend, bookReturn
};
enum mode graphicsMode; //决定主界面内容绘制方式 
enum boolean isLogin;
enum fileStatus{Saved, nSaved, nLoaded}currentFileStatus;
bool dateErr; /*日期格式错误标志*/ 
bool queryErr; /*图书检索输入错误标志（多重输入*/
bool inputErr; /*图书添加/修改的输入不合规范*/
bool notFound; /*搜索未找到或结束标志或用户不存在标志*/ 
bool isExist; /*图书/用户已存在标志*/ 
bool errPasswd; /*密码输入错误标志*/ 
bool passwdNotMatch; /*修改密码时两次不匹配的标志*/
bool passwdChangeOK; /*完成密码修改的标志*/ 
bool isSucceed; 
bool isSortFin;
bool notEmpty; /*用户删除时有未还的书的标志*/
bool overLimit; /*超过最大借阅本数的标志*/

/*图形相关函数声明 */
void windowsInit(void);
void refreshScreen(void);

void drawMenus(void);
void drawStatusBar(void);
void drawInnerContent(void);

void funcLogin(void);
void funcBooksManagement(void);
void funcReadersManagement(void);
void funcStatInfo(void);
void funcSettings(void);
void funcHomepage(void);
void funcLendBook(void);
void funcReturnBook(void);
void isInputValid(void);
int isQueryValid(void);
void chkPasswd(void);
void bookSearch(void);
void fuzzyLog(void);
void countNumber(void);
int splitTime(int type);
void clearCurrentBook(void);
void clearCurrentUser(void);
void clearFlags(void);
void displayErrMessages(void); 
void saveAll(void); /*保存所有数据库和设置到文件*/ 
void loadAll(void);

void setPen(void);
void drawHorizontalLine(char *color, double y);

void mouseHandler(int x, int y, int button, int event);
void keyboardHandler(int key, int event);
void charHandler(char ch); 
void timerHandler(int timerID);

#endif
