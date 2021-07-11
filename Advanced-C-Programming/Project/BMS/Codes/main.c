#include "main.h"
#include "gui.h"

int borrowingDays=30;
int borrowingBooks=5;

void Main() 
{
	currentFileStatus=nLoaded;
	isLogin = 0;
	/*如果没有passwd.bin文件，初始密码为123456，建议修改*/ 
	strcpy(mySettings.adminPassward, passwordEncrypt("123456", PASSWD_OFFSET));  
	systemSettings_load(); /*从文件加载系统设定*/ 
	windowsInit();
	registerMouseEvent(mouseHandler);
	registerKeyboardEvent(keyboardHandler);
	registerCharEvent(charHandler);
	registerTimerEvent(timerHandler); /*注册四类事件的回调函数*/ 
	startTimer(timerRefresh, timerInteval); /*启动屏幕刷新定时器*/ 
	if(mySettings.autoSaveSwitch)
		startTimer(timerAutoSave, mySettings.autoSaveTime); /*根据设置启动自动保存定时器*/ 
}
