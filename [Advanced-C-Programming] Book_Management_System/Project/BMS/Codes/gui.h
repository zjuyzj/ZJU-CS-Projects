/*
 * �ļ���gui.h
 * ---------------------------
 * ͼ�ν����û�����ģ�� 
 * ��дʱ�䣺2020.05.22
 */
 
#ifndef _GUI_H_
#define _GUI_H_

/*����Win32ͼ�ο��LibGraphics/IMGUI���ͷ�ļ� */
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
#pragma comment(lib,"winmm.lib") /*����Windows��ý���*/ 

/*�������ȫ�ֱ������� */
static double windowsWidth,windowsHeight; /*���ڿ��*/
static double fontHeight; /*ȫ������߶�*/ 
 
/*�˵����ݱ����Ķ���*/
static char * menuListFile[] = {"File", "Open  | Ctrl-O", "Save  | Ctrl-S", "Close | Ctrl-D", "Exit    | Ctrl-E"};
static char * menuListView[] = {"View", "Books     | Ctrl-B", "Readers  | Ctrl-R", "Statistic  | Ctrl-I"};
static char * menuListOption[] = {"Option", "System Settings", "Log Out"};
static char * menuListHelp[] = {"Help", "Please read Help.pdf for more information"};

/*��ʱ����ض��� TimerID Definition*/
#define timerRefresh 1 /*1�Ŷ�ʱ��������Ļˢ��*/ 
#define timerAutoSave 2 /*2�Ŷ�ʱ�������ļ����Զ����湦��*/ 
#define timerInteval 2 /*��ʱ������ʱ����*/

/*GUIȫ�ֱ��� */
enum boolean{True, False};
enum mode{
	Login, Readers, Books, Stats, Settings, bookLend, bookReturn
};
enum mode graphicsMode; //�������������ݻ��Ʒ�ʽ 
enum boolean isLogin;
enum fileStatus{Saved, nSaved, nLoaded}currentFileStatus;
bool dateErr; /*���ڸ�ʽ�����־*/ 
bool queryErr; /*ͼ�������������־����������*/
bool inputErr; /*ͼ�����/�޸ĵ����벻�Ϲ淶*/
bool notFound; /*����δ�ҵ��������־���û������ڱ�־*/ 
bool isExist; /*ͼ��/�û��Ѵ��ڱ�־*/ 
bool errPasswd; /*������������־*/ 
bool passwdNotMatch; /*�޸�����ʱ���β�ƥ��ı�־*/
bool passwdChangeOK; /*��������޸ĵı�־*/ 
bool isSucceed; 
bool isSortFin;
bool notEmpty; /*�û�ɾ��ʱ��δ������ı�־*/
bool overLimit; /*���������ı����ı�־*/

/*ͼ����غ������� */
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
void saveAll(void); /*�����������ݿ�����õ��ļ�*/ 
void loadAll(void);

void setPen(void);
void drawHorizontalLine(char *color, double y);

void mouseHandler(int x, int y, int button, int event);
void keyboardHandler(int key, int event);
void charHandler(char ch); 
void timerHandler(int timerID);

#endif
