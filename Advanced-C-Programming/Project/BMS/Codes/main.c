#include "main.h"
#include "gui.h"

int borrowingDays=30;
int borrowingBooks=5;

void Main() 
{
	currentFileStatus=nLoaded;
	isLogin = 0;
	/*���û��passwd.bin�ļ�����ʼ����Ϊ123456�������޸�*/ 
	strcpy(mySettings.adminPassward, passwordEncrypt("123456", PASSWD_OFFSET));  
	systemSettings_load(); /*���ļ�����ϵͳ�趨*/ 
	windowsInit();
	registerMouseEvent(mouseHandler);
	registerKeyboardEvent(keyboardHandler);
	registerCharEvent(charHandler);
	registerTimerEvent(timerHandler); /*ע�������¼��Ļص�����*/ 
	startTimer(timerRefresh, timerInteval); /*������Ļˢ�¶�ʱ��*/ 
	if(mySettings.autoSaveSwitch)
		startTimer(timerAutoSave, mySettings.autoSaveTime); /*�������������Զ����涨ʱ��*/ 
}
