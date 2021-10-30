#include "Tronxy.h"
#include "src/sd/cardreader.h"
#include "src/module/temperature.h"
#include "src/gcode/queue.h"
#include "src/module/planner.h"
#include "src/module/stepper.h"
#include "src/module/settings.h"
#include "src/module/probe.h"
#include "src/feature/bedlevel/bedlevel.h"
#include "src/module/printcounter.h"
#include "src/gcode/gcode.h"
#include "src/libs/buzzer.h"
#include "src/feature/controllerfan.h"
#if HAS_FILAMENT_SENSOR
#include "src/feature/runout.h"
#endif
#include "src/feature/powerloss.h"
#if ENABLED(BABYSTEPPING)
#include "src/feature/babystep.h"
#include "src/libs/numtostr.h"
#endif
#define	STM32F0_IDADDR	    0x1FFFF7AC
#define	STM32F1_IDADDR	    0x1FFFF7E8
#define	STM32F2_IDADDR	    0x1FFF7A10
#define	STM32F3_IDADDR	    0x1FFFF7AC
#define	STM32F4_IDADDR	    0x1FFF7A10
#define	STM32F7_IDADDR	    0x1FF0F420
#define	STM32L0_IDADDR	    0x1FF80050
#define	STM32L1_IDADDR	    0x1FF80050
#define	STM32L4_IDADDR	    0x1FFF7590
#define	STM32H7_IDADDR	    0x1FF0F420
#if MY_PROJ == 1
#define TRONXY_MACHINE_NAME  "XY3SE"
#elif MY_PROJ == 2
#define TRONXY_MACHINE_NAME  "XY3SE-2T1"
#endif
#define INDEX_LANG          104
#define INDEX_THEME         105
#define INDEX_CALIBRATION   106
#define INDEX_Z_OFFSET      118
#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
#define INDEX_Z_VALUES      148
#define INDEX_HOTEND_OFFSET   212
#else
#define INDEX_HOTEND_OFFSET   184
#endif
#define WIN_YSTART 0
#define WIN_XBASE  480 //下面界面基于480x320 信息栏24
#define WIN_YBASE  296 //320-24
#define WIN_YMAX  (LCD_WMAX - WIN_YSTART) 
#define FONT_NUM  Font_En10x20
#define ICO_XSIZE  100
#define ICO_YSIZE  108

#if TRONXY_PRINTING_NOWAIT
 bool printing_no_wait = false;
#endif
volatile bool myui_tf = false,myui_sw = false;
BYTE my_lang = LANG_ENGLISH,my_theme = 0;
BYTE distanceIndex = 4,winStep = 0;
volatile BYTE globalStop = 0;
TronxyPrintStatus my_print_status = PRINT_IDLE;
uint16_t topline = 0,fileCnt = 0;
yWidget *currentWidget;
yWindows *topwin = nullptr;
VoidFun currentScreen = nullptr,lastScreen = nullptr;
static VoidFun funAfter = nullptr;
yWidget* topwinComponents[24] = {0};
yLabel* lbComponents[24] = {0};
float fvalue = 0,fvalue2;
uint32_t logTime = 0, printTotalTime = 0, modifyTotalTime = 0;
int messageIndexNow = 0,messageIndexNext = 0;
const char* error_info = nullptr;
static uint32_t last = 0;

#include "2lcd/language/mylang.h"
#define WORD_COLOR_IDLE       COLOR_BLUE //图标文字idle颜色,也是不可触摸的控件文字基本颜色
#define BASIC_BG_COLOR        COLOR_WHITE //大背景颜色
#define VALUE_BG_COLOR        COLOR_GREY //显示值背景颜色
#define VALUE_FR_COLOR        COLOR_WHITE //显示值前景常态颜色

#define QUICK_STOP()   do{globalStop = 2;queue.clear();quickstop_stepper();}while(0) //大循环两次,保证全部停下来
WIN_Attribute winAttr = {
	.x = 0,
	.y = 0,
	.width = LCD_LMAX,
	.height = LCD_WMAX,
	.components = topwinComponents,
	.componentNum = 0,
};
void closeTopWin()
{
	if(topwin) {
		yWindows* t = topwin;
		topwin = topwin->Parent;
		delete t;
	}
}
inline void closeAllWin(){while(topwin)closeTopWin();}

int tx = 0,ty = 0;
#define CALIBRATE_DIS   32
struct{
	BYTE index;
	int touchX[4];
	int touchY[4];
}Calibrate_P;

char cmd[MAX_CMD_SIZE + 16];
static void screenResumeOpenFile(void)
{
	void screenPrinting(void);
	sprintf_P(cmd, M23_STR, PrintJobRecovery::info.sd_filename);
	gcode.process_subcommands_now(cmd);
	if(card.isFileOpen() && messageIndexNext == 0) {
		queue.inject_P(PSTR("M1000"));
		my_print_status = PRINT_RESUME;
		currentScreen = screenPrinting;
	}
}
static void screenWaitStop(void)
{
	if(globalStop == 0) {
		gobackScreen(screenMain);
		card.flag.abort_sd_printing = true;
	}
}
void screenTool(void);
void updateFileList();
static void funGeneral(yWidget*w,WidgetEvent evt){
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		currentScreen = w->funNext;
		currentWidget = w;
		messageIndexNow = 0;
		break;
	default:break;
	}
}
#define BUTTON_CLOSE_POPWIN     1
#define BUTTON_RESET            2
#define BUTTON_ERR_CONFIRM      3
#define BUTTON_CALI_SAVE        4
#define BUTTON_STOP_PRINT       5
#define BUTTON_FILE_UP          6
#define BUTTON_FILE_DOWN        7
#define BUTTON_LEVEL_STOP       8
#define BUTTON_NEXT             9
#define BUTTON_CONFIRM_ZOFFSET  10
static void funGeneralButton(yWidget*w,WidgetEvent evt) {
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch(w->funParam) {
		case BUTTON_CLOSE_POPWIN:
			currentScreen = screenCloseMessageBox;
			break;
		case BUTTON_RESET:
			#if T2_STM32F446
			NVIC_SystemReset();
			#elif T2_STM32F103
			nvic_sys_reset();
			#endif
			break;
		case BUTTON_ERR_CONFIRM:
			if(globalStop == 0){
				if(PrintJobRecovery::enabled)PrintJobRecovery::check();
				my_print_status = PRINT_IDLE;
				wait_for_user = false;
				gobackScreen(screenMain);
			}
			break;
		case BUTTON_CALI_SAVE:
			messageIndexNow = 0;
			currentScreen = w->funNext;
			yWindows::para_calibration.x = ((LCD_LMAX -  2 * CALIBRATE_DIS) << 17) / (Calibrate_P.touchX[1] + Calibrate_P.touchX[2] - Calibrate_P.touchX[0] - Calibrate_P.touchX[3]);
			yWindows::para_calibration.y = ((LCD_WMAX -  2 * CALIBRATE_DIS) << 17) / (Calibrate_P.touchY[2] + Calibrate_P.touchY[3] - Calibrate_P.touchY[0] - Calibrate_P.touchY[1]);
			yWindows::para_calibration.offset_x = CALIBRATE_DIS - int16_t(((Calibrate_P.touchX[0] + Calibrate_P.touchX[3]) * yWindows::para_calibration.x) >> 17);
			yWindows::para_calibration.offset_y = CALIBRATE_DIS - int16_t(((Calibrate_P.touchY[0] + Calibrate_P.touchY[1]) * yWindows::para_calibration.y) >> 17);
			break;
		case BUTTON_STOP_PRINT:
			currentScreen = screenWaitStop;
			globalStop = 8;
			HOTEND_LOOP() thermalManager.reset_hotend_idle_timer(e);
			my_print_status = PRINT_IDLE;
			break;
		case BUTTON_FILE_UP:
			if(topline > 0){
				topline -= 4;
				updateFileList();
			}
			break;
		case BUTTON_FILE_DOWN:
			if(topline + 4 < fileCnt) {
				topline += 4;
				updateFileList();
			}
			break;
		case BUTTON_LEVEL_STOP:
			QUICK_STOP();//stop run
			currentScreen = screenTool;
			break;
		case BUTTON_NEXT:
			currentScreen = w->funNext;
			break;
		#ifdef AUTO_BED_LEVELING_BILINEAR
		case BUTTON_CONFIRM_ZOFFSET:
			if(currentScreen || planner.has_blocks_queued())break;
			fvalue = destination.z - current_position.z;
			break;
		#endif
		default:
			break;
		}
		break;
	default:break;
	}
}
static void funPopScreen(yWidget*w,WidgetEvent evt){
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		screenPopIndex(w->funParam);
		break;
	default:break;
	}
}

//SECTOR1:****************************系统****************************
static void showBed(void) {
	int16_t c,t;
	c = thermalManager.degBed();
	t = thermalManager.degTargetBed();
	if(t > c + 2)lbComponents[0]->setBinaryFrColor(COLOR_RED,0);
	else lbComponents[0]->setBinaryFrColor(COLOR_BLACK,0);
	sprintf(lbComponents[0]->char_tar,"%3d/%-3d",c,t);
	lbComponents[0]->sign.content.text = 1;
}
static void showHotend(void) {
	int16_t c,t;
	c = thermalManager.degHotend(0);
	t = thermalManager.degTargetHotend(0);
	if(t > c + 2)lbComponents[1]->setBinaryFrColor(COLOR_RED,0);
	else lbComponents[1]->setBinaryFrColor(COLOR_BLACK,0);
	sprintf(lbComponents[1]->char_tar,"%3d/%-3d",c,t);
	lbComponents[1]->sign.content.text = 1;
	#if HOTENDS > 1
	c = thermalManager.degHotend(1);
	t = thermalManager.degTargetHotend(1);
	if(t > c + 2)lbComponents[2]->setBinaryFrColor(COLOR_RED,0);
	else lbComponents[2]->setBinaryFrColor(COLOR_BLACK,0);
	sprintf(lbComponents[2]->char_tar,"%3d/%-3d",c,t);
	lbComponents[2]->sign.content.text = 1;
	#endif
}

static void screenUpdateStatus(void)
{
	uint32_t tnow = millis();
	if(tnow >= last){
		last = tnow + 500;
		showBed();
		showHotend();
		yString::doubleToStr(current_position.x,lbComponents[3]->char_tar,-7,2);
		lbComponents[3]->sign.content.text = 1;
		yString::doubleToStr(current_position.y,lbComponents[4]->char_tar,-7,2);
		lbComponents[4]->sign.content.text = 1;
		yString::doubleToStr(current_position.z,lbComponents[5]->char_tar,-7,2);
		lbComponents[5]->sign.content.text = 1;
	}
}

void screenSystem(void);
static void screenStatus(void) {
	closeAllWin();
	currentScreen = screenUpdateStatus;
	pic_page.value = MCU_PIC_ADDR(POS_SYSTEM_STATUS);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(376,254,100,62,nullptr,&picMain_focus));
	int width = 8 * FONT_NUM.xPixel;
	_NEW_W(lbComponents[3],yLabel(154,72 - FONT_NUM.yPixel / 2,width,FONT_NUM.yPixel,nullptr,nullptr));//XPos
	topwinComponents[2] = lbComponents[3];
	_NEW_W(lbComponents[4],yLabel(354,74 - FONT_NUM.yPixel / 2,width,FONT_NUM.yPixel,nullptr,nullptr));//YPos
	topwinComponents[3] = lbComponents[4];
	_NEW_W(lbComponents[5],yLabel(154,134 - FONT_NUM.yPixel / 2,width,FONT_NUM.yPixel,nullptr,nullptr));//ZPos
	topwinComponents[4] = lbComponents[5];
	_NEW_W(lbComponents[1],yLabel(154,191 - FONT_NUM.yPixel / 2,width,FONT_NUM.yPixel,nullptr,nullptr));//喷头1
	topwinComponents[5] = lbComponents[1];
	_NEW_W(lbComponents[2],yLabel(354,190 - FONT_NUM.yPixel / 2,width,FONT_NUM.yPixel,nullptr,nullptr));//喷头2
	topwinComponents[6] = lbComponents[2];
	_NEW_W(lbComponents[0],yLabel(154,260 - FONT_NUM.yPixel / 2,width,FONT_NUM.yPixel,nullptr,nullptr));//热床
	topwinComponents[7] = lbComponents[0];
	winAttr.componentNum = 8;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenSystem;
	for(int i = 0; i < 6; i++) {
		lbComponents[i]->setCBuffer(7);
		lbComponents[i]->setBinaryOpaque(true);
		lbComponents[i]->wEnPic = &FONT_NUM;
	}
	#if HOTENDS < 2
	lbComponents[2]->setChars("---");
	#endif
}

static void screenInfo(void) {
	closeAllWin();
	currentScreen = nullptr;

	if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_INFO_CN);
	else pic_page.value = MCU_PIC_ADDR(POS_INFO_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(326,234,80,60,nullptr,&picMain_focus));
	_NEW_W(topwinComponents[2],yWidget(5,5,80,60,nullptr,&picMain_focus));
	_NEW_W(lbComponents[0],yLabel(156,100 - DEFAULT_FONT_YPIXEL / 2,24 * DEFAULT_FONT_XPIXEL,DEFAULT_FONT_YPIXEL,nullptr,nullptr));//mcu_id
	topwinComponents[3] = lbComponents[0];
	_NEW_W(lbComponents[1],yLabel(156,148 - FONT_NUM.yPixel / 2,300,FONT_NUM.yPixel,nullptr,nullptr));//version
	lbComponents[1]->wEnPic = &FONT_NUM;
	topwinComponents[4] = lbComponents[1];
	winAttr.componentNum = 5;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenSystem;
	topwinComponents[2]->fun = funGeneralButton;
	topwinComponents[2]->funParam = BUTTON_RESET;
	BYTE t;
	char str[28];
	const volatile uint8_t* mcu_id = (const volatile uint8_t*)STM32_IDADDR;//12字节
	for(int i = 0; i < 12; i++){
		t = mcu_id[i] >> 4;
		str[i << 1] = t > 9 ? t - 10 + 'A' : t + '0';
		t = mcu_id[i] & 0xF;
		str[(i << 1) + 1] = t > 9 ? t - 10 + 'A' : t + '0';
	}
	str[24] = 0;
	lbComponents[0]->setCBuffer(0,str);
	lbComponents[1]->setCBuffer(0,TRONXY_MACHINE_NAME "-" TRONXY_SOFT_VERSION);
	lbComponents[1]->setBinaryOpaque(true);
}
static void exchangeLang(yWidget*w,WidgetEvent evt){
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		gobackScreen(screenSystem);
		if(my_lang == LANG_CHINESE)my_lang = LANG_ENGLISH;
		else my_lang = LANG_CHINESE;
		do{
			int index = INDEX_LANG;
			AT24CXX_Write(index,&my_lang,sizeof(my_lang));
		}while(0);
		break;
	default:break;
	}
}
void screenFileChoose(void);
yWidget *popComponents[6];
void screenCloseMessageBox(void) {
	if(messageIndexNow && topwin && messageIndexNow != POP_KILL_ERROR) {
		if(topwin->Parent) closeTopWin();
		currentScreen = lastScreen;
		lastScreen = nullptr;
		messageIndexNow = 0;
	}
}
void screenDeleteFile()
{
	screenCloseMessageBox();
	yLabel *lb = (yLabel*)topwin->getComponent(0);
	card.removeFile(lb->char_tar);
	currentScreen = screenFileChoose;
}
static void screenResumePrint(void)
{
	screenCloseMessageBox();
	HOTEND_LOOP() thermalManager.reset_hotend_idle_timer(e);
	marlin_state = MF_RUNNING;
	#if ENABLED(FILAMENT_RUNOUT_SENSOR)
	runout.reset();
	#endif
	currentScreen = screenResumeOpenFile;
}
static void screenInserCard(void)
{
	screenCloseMessageBox();
	card.release();
	card.mount();
	if(card.isMounted())screenPopIndex(POP_RECOVERY_PRINT);
	else screenPopIndex(POP_INSERT_CARD);
}
void screenReadFailAfter(void)
{
	if(globalStop == 0){
		print_job_timer.stop();
		current_position.z += 5;
		line_to_current_position();
		endstops.enable(true);
		#if ENABLED(DUAL_X_CARRIAGE)

			// Always home the 2nd (right) extruder first
			active_extruder = 1;
			homeaxis(X_AXIS);

			// Remember this extruder's position for later tool change
			inactive_extruder_x_pos = current_position.x;

			// Home the 1st (left) extruder
			active_extruder = 0;
			homeaxis(X_AXIS);

			// Consider the active extruder to be parked
			raised_parked_position = current_position;
			delayed_move_time = 0;
			active_extruder_parked = true;

		#else

			homeaxis(X_AXIS);

		#endif
		homeaxis(Y_AXIS);
		endstops.not_homing();
		#if TRONXY_PRINTING_NOWAIT
		printing_no_wait = false;
		#endif
		card.release();
		card.mount();
		currentScreen = nullptr;
	}
}
static void screenReadFail(void)
{
	void screenMessageBox(void);
	currentScreen = screenMain;
	lastScreen = screenMessageBox;
	messageIndexNow = 0;
	if(!card.isMounted())card.mount();
	if(card.isMounted()) messageIndexNext = POP_RECOVERY_PRINT;
	else messageIndexNext = POP_INSERT_CARD;
	PrintJobRecovery::load();
}
static void screenCancelPrint(void)
{
	PrintJobRecovery::cancel();
	card.endFilePrint();
	HOTEND_LOOP() thermalManager.reset_hotend_idle_timer(e);
	marlin_state = MF_RUNNING;
	#if ENABLED(FILAMENT_RUNOUT_SENSOR)
	runout.reset();
	#endif
	queue.inject_P(PSTR("M84"));
	my_print_status = PRINT_IDLE;
	currentScreen = screenMain;
	lastScreen = nullptr;
	messageIndexNow = 0;
}
const WORD ok_cn[] = {0x786e,0x5b9a,0};
const WORD cancel_cn[] = {0x53d6,0x6d88,0};
const char *ok_en = "OK";
const char *cancel_en = "Cancel";
static void popUpdateHotendTemperature(void){
	uint32_t tnow = millis();
	if(tnow >= last) {
		yLabel *lb = (yLabel*)popComponents[2];
		#if HOTENDS > 1
		sprintf_P(lb->char_tar,"%-3d|%-3d %-3d|%-3d %-3d|%-3d",(int)thermalManager.degBed(),
		(int)thermalManager.degTargetBed(),
		(int)thermalManager.degHotend(0),(int)thermalManager.degTargetHotend(0),
		(int)thermalManager.degHotend(1),(int)thermalManager.degTargetHotend(1));
		#else
		sprintf_P(lb->char_tar,"%-3d|%-3d %-3d|%-3d",(int)thermalManager.degBed(),
		(int)thermalManager.degTargetBed(),
		(int)thermalManager.degHotend(0),(int)thermalManager.degTargetHotend(0));
		#endif
		lb->sign.content.text = 1;
		last = tnow + 100;
	}
}
static void screenFactoryReset(void)
{
	int eeprom_index = 100; //EEPROM_OFFSET
	const char version[4] = "xxx";
	AT24CXX_Write(eeprom_index,version,sizeof(version)/sizeof(version[0]));
	#if T2_STM32F446
	NVIC_SystemReset();
	#elif T2_STM32F103
	nvic_sys_reset();
	#endif
}
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
static uint8_t out_extruder = 0;
#endif

void screenMessageBox(void)
{
	screenCloseMessageBox();
	if(messageIndexNow) {
		currentScreen = lastScreen;
		funAfter = nullptr;
		messageIndexNext = 0;
		return;
	}
	messageIndexNow = messageIndexNext;
	messageIndexNext = 0;
	yLabel *lbOK = nullptr;
	yLabel *lbCancel = nullptr;
	if(funAfter) {
		currentScreen = funAfter;
		funAfter = nullptr;
	}
	else currentScreen = nullptr;
	#if ENABLED(DUAL_X_CARRIAGE)
	void funConfirmHotendOffset(yWidget*w,WidgetEvent evt);
	#endif
	yNotepad* npInfo;
	constexpr int length = LCD_LMAX * 3 / 4;
	constexpr int width = LCD_WMAX / 2;
	constexpr int bl = LCD_LMAX * 5 / 24;
	constexpr int bw = LCD_WMAX / 8;
	_NEW_W(npInfo,yNotepad(0,0,length,width,&pic_popwin_background,nullptr,false));
	popComponents[0] = npInfo;
	WIN_Attribute winAttr = {
		.x = (LCD_LMAX - length) / 2,
		.y = (LCD_WMAX + WIN_YSTART - width) / 2,
		.width = length + 1,
		.height = width + 1,
		.components = popComponents,
	};
	topwin->update();

	npInfo->setBuffSpace(8,63);
	npInfo->setBinaryFrColor(COLOR_BLUE,COLOR_BLUE);
	npInfo->setBinaryBgColor(COLOR_BLACK,COLOR_BLACK);

	switch(messageIndexNow) {
	case POP_FILAMENT_OUT://断料停靠
	case POP_PAUSING://暂停中
		winAttr.componentNum = 1;
		break;
	case POP_LANGUAGE://语言切换
	case POP_CALIBRATE_CONFIRM://校准屏幕确认
	case POP_FACTORY_RESET://恢复出厂设置
	case POP_STOP_PRINT://是否停止打印
	case POP_DELETE_FILE: //删除一个文件
	case POP_FILAMENT_INSERT://按键确认进料
	case POP_RECOVERY_PRINT://是否继续打印
	case POP_OPENFILE_FAIL://打开文件失败,是否重试
	case POP_INSERT_CARD://请插入SD卡
	#if ENABLED(DUAL_X_CARRIAGE)
	case POP_SAVE_HOTENDOFFSET://是否保存喷嘴偏移
	#endif
		_NEW_W(lbOK,yLabel(length / 2 - 8 - bl,width - bw - 16,bl,bw,&pic_button_background_idle,&pic_button_background_focus));
		lbOK->setBinaryFrColor(COLOR_BLUE,COLOR_WHITE);
		lbOK->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLUE);
		_NEW_W(lbCancel,yLabel(length / 2 + 8,width - bw - 16,bl,bw,&pic_button_background_idle,&pic_button_background_focus));
		lbCancel->setBinaryFrColor(COLOR_BLUE,COLOR_WHITE);
		lbCancel->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLUE);
		lbOK->updateTextIfChange = true;
		lbCancel->updateTextIfChange = true;
		popComponents[1] = lbOK;
		popComponents[2] = lbCancel;
		winAttr.componentNum = 3;
		break;
	case POP_KILL_ERROR://出错确认
	case POP_COMPLETE_PRINT://打印完成
	case POP_EXTRUDER_COLD://喷头过冷
	case POP_NOSUPPORT://暂不支持
	case POP_PROB_FAIL://自动调平探头探底失败
	case POP_NORMAL_MODE://提示当前处于正常打印模式
	case POP_READFILE_FAIL:
		_NEW_W(lbOK,yLabel((length - bl) / 2,width - bw - 16,bl,bw,&pic_button_background_idle,&pic_button_background_focus));
		lbOK->setBinaryFrColor(COLOR_BLUE,COLOR_WHITE);
		lbOK->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLUE);
		lbOK->updateTextIfChange = true;
		popComponents[1] = lbOK;
		winAttr.componentNum = 2;
		break;
	case POP_WAITNOZZLE_HEAT://等待喷头加热
		_NEW_W(lbOK,yLabel((length - bl) / 2,width - bw - 10,bl,bw,&pic_button_background_idle,&pic_button_background_focus));//关闭界面
		lbOK->setBinaryFrColor(COLOR_BLUE,COLOR_WHITE);
		lbOK->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLUE);
		lbOK->updateTextIfChange = true;
		popComponents[1] = lbOK;
		{
			yLabel *lb;
			#if HOTENDS > 1
			const int wlen = FONT_NUM.xPixel * 24;
			_NEW_W(lb,yLabel((length - wlen) / 2,width - bw - FONT_NUM.yPixel - 18,wlen,FONT_NUM.yPixel,nullptr,nullptr,false));//温度
			lb->setCBuffer(27);
			#else
			const int wlen = FONT_NUM.xPixel * 16;
			_NEW_W(lb,yLabel((length - wlen) / 2,width - bw - FONT_NUM.yPixel - 18,wlen,FONT_NUM.yPixel,nullptr,nullptr,false));//温度
			lb->setCBuffer(15);
			#endif
			lb->wEnPic = &FONT_NUM;
			lb->setBinaryFrColor(COLOR_RED,0);
			lb->setBinaryBgColor(pic_popwin_background.value,0);
			lb->setBinaryOpaque(true);
			popComponents[2] = lb;
		}
		winAttr.componentNum = 3;
		currentScreen = popUpdateHotendTemperature;
		break;
	}
	_NEW_W(topwin,yWindows(&winAttr,topwin));
	
	if(my_lang) {
		if(lbOK)lbOK->textDistance = 1;
		if(lbCancel)lbCancel->textDistance = 1;
	}
	switch(my_lang) {
	case LANG_CHINESE:
		if(messageIndexNow == POP_OPENFILE_FAIL){
			const WORD retry_cn[] = {0x91cd,0x8bd5,0};
			lbOK->setWPos(34,14);
			lbOK->setWBuffer(3,retry_cn);
			lbCancel->setWPos(34,14);
			lbCancel->setWBuffer(3,cancel_cn);
		}
		else {
			if(lbOK) {
				lbOK->setWPos(34,14);
				lbOK->setWBuffer(0,ok_cn);
			}
			if(lbCancel) {
				lbCancel->setWPos(34,14);
				lbCancel->setWBuffer(0,cancel_cn);
			}
		}
		break;
	default:
		if(messageIndexNow == POP_OPENFILE_FAIL) {
			lbOK->setWPos((bl - DEFAULT_FONT_XPIXEL * 5) / 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
			lbOK->setCBuffer(5,"Retry");
			lbCancel->setWPos((bl - DEFAULT_FONT_XPIXEL * 6) / 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
			lbCancel->setCBuffer(7,cancel_en);
		}
		else {
			if(lbOK){
				lbOK->setWPos((bl - DEFAULT_FONT_XPIXEL * 2) / 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
				lbOK->setCBuffer(3,ok_en);
			}
			if(lbCancel) {
				lbCancel->setWPos((bl - DEFAULT_FONT_XPIXEL * 6) / 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
				lbCancel->setCBuffer(7,cancel_en);
			}
		}
		break;
	}

	if(my_lang){
		switch(messageIndexNow) {
		case POP_KILL_ERROR:
			npInfo->addChars(tip_err_cn);
			npInfo->addChars(error_info);
			npInfo->addChars(tip_system_reset_cn);
			break;
		case POP_FILAMENT_OUT:
			npInfo->addChars(tip_filamentout_cn);
			break;
		case POP_PAUSING:
			npInfo->addChars(tip_pausing_cn);
			break;
		case POP_WAITNOZZLE_HEAT:
			npInfo->addChars(tip_waitforheating_cn);
			break;
		case POP_LANGUAGE:
			npInfo->addChars(tip_language_cn);
			break;
		case POP_FACTORY_RESET:
			npInfo->addChars(tip_factoryreset_cn);
			break;
		case POP_CALIBRATE_CONFIRM:
			npInfo->addChars(tip_calibrate_cn);
			break;
		case POP_STOP_PRINT:
			npInfo->addChars(tip_stopprint_cn);
			break;
		case POP_RECOVERY_PRINT:
			npInfo->addChars(tip_recoveryprint_cn);
			break;
		case POP_COMPLETE_PRINT:
			do{
				char str[32];
				int h = printTotalTime / 3600; 
				int m = (printTotalTime % 3600) / 60;
				sprintf(str,"%d:%d",h,m);
				WORD wd[32];
				for(h = 0; str[h]; h++)wd[h] = str[h];
				wd[h] = 0;
				npInfo->addChars(" \n ");
				npInfo->addChars(wd);
				npInfo->addChars(tip_completeprint_cn);
			}while(false);
			break;
		case POP_DELETE_FILE:
			npInfo->addChars(tip_deletefile_cn);
			break;
		case POP_EXTRUDER_COLD:
			npInfo->addChars(tip_coodextruder_cn);
			break;
		#if ENABLED(FILAMENT_RUNOUT_SENSOR)
		case POP_FILAMENT_INSERT:
			npInfo->addChars(tip_filamentinsert_cn);
			npInfo->addChars(tip_extruder_cn);
			if(out_extruder){
				if(out_extruder == 1)npInfo->addChars("1");
				else npInfo->addChars("2");
				out_extruder = 0;
			}
			else {
				if (TERN0(DUAL_X_CARRIAGE, dxc_is_duplicating())
				|| TERN0(MULTI_NOZZLE_DUPLICATION, extruder_duplication_enabled))
				{
					if(READ(FIL_RUNOUT_PIN))npInfo->addChars("1");
					else npInfo->addChars("2");
				}
				else {
					if(active_extruder == 0)npInfo->addChars("1");
					else npInfo->addChars("2");
				}
			}
			break;
		#endif
		case POP_NOSUPPORT://暂不支持
			npInfo->addChars(tip_nosupport_cn);
			break;
		case POP_PROB_FAIL:
			npInfo->addChars(tip_probe_fail_cn);
			break;
		case POP_NORMAL_MODE:
			npInfo->addChars(tip_normal_mode_cn);
			break;
		#if ENABLED(DUAL_X_CARRIAGE)
		case POP_SAVE_HOTENDOFFSET:
			npInfo->addChars(tip_modify_save_cn);
			break;
		#endif
		case POP_OPENFILE_FAIL://打开文件失败,是否重试
			npInfo->addChars(tip_open_fail_cn);
			npInfo->addChars(PrintJobRecovery::info.sd_longname);
			break;
		case POP_READFILE_FAIL:
			npInfo->addChars(tip_read_fail_cn);
			break;
		case POP_INSERT_CARD:
			npInfo->addChars(tip_insert_sdcard_cn);
			break;
		}
	}
	else {
		switch(messageIndexNow){
		case POP_KILL_ERROR:
			npInfo->addChars(tip_err_en);
			npInfo->addChars(error_info);
			npInfo->addChars(tip_system_reset_en);
			break;
		case POP_FILAMENT_OUT:
			npInfo->addChars(tip_filamentout_en);
			break;
		case POP_PAUSING:
			npInfo->addChars(tip_pausing_en);
			break;
		case POP_WAITNOZZLE_HEAT:
			npInfo->addChars(tip_waitforheating_en);
			break;
		case POP_LANGUAGE:
			npInfo->addChars(tip_language_en);
			break;
		case POP_FACTORY_RESET:
			npInfo->addChars(tip_factoryreset_en);
			break;
		case POP_CALIBRATE_CONFIRM:
			npInfo->addChars(tip_calibrate_en);
			break;
		case POP_STOP_PRINT:
			npInfo->addChars(tip_stopprint_en);
			break;
		case POP_RECOVERY_PRINT:
			npInfo->addChars(tip_recoveryprint_en);
			break;
		case POP_COMPLETE_PRINT:
			do{
				char str[32];
				int h = printTotalTime / 3600; 
				int m = (printTotalTime % 3600) / 60;
				sprintf(str,"%d:%d",h,m);
				npInfo->addChars(" \n Print time:");
				npInfo->addChars(str);
				npInfo->addChars(tip_completeprint_en);
			}while(false);
			break;
		case POP_DELETE_FILE:
			npInfo->addChars(tip_deletefile_en);
			break;
		case POP_EXTRUDER_COLD:
			npInfo->addChars(tip_coodextruder_en);
			break;
		#if ENABLED(FILAMENT_RUNOUT_SENSOR)
		case POP_FILAMENT_INSERT:
			npInfo->addChars(tip_filamentinsert_en);
			npInfo->addChars(tip_extruder_en);
			if(out_extruder){
				if(out_extruder == 1)npInfo->addChars("1");
				else npInfo->addChars("2");
				out_extruder = 0;//此生缘,此生结,来生无缘路人嫌
			}
			else {
				if (TERN0(DUAL_X_CARRIAGE, dxc_is_duplicating())
				|| TERN0(MULTI_NOZZLE_DUPLICATION, extruder_duplication_enabled))
				{
					if(READ(FIL_RUNOUT_PIN))npInfo->addChars("1");
					else npInfo->addChars("2");
				}
				else {
					if(active_extruder == 0)npInfo->addChars("1");
					else npInfo->addChars("2");
				}
			}
			break;
		#endif
		case POP_NOSUPPORT://暂不支持
			npInfo->addChars(tip_nosupport_en);
			break;
		case POP_PROB_FAIL:
			npInfo->addChars(tip_probe_fail_en);
			break;
		case POP_NORMAL_MODE:
			npInfo->addChars(tip_normal_mode_en);
			break;
		#if ENABLED(DUAL_X_CARRIAGE)
		case POP_SAVE_HOTENDOFFSET:
			npInfo->addChars(tip_modify_save_en);
			break;
		#endif
		case POP_OPENFILE_FAIL://打开文件失败,是否重试
			npInfo->addChars(tip_open_fail_en);
			npInfo->addChars(PrintJobRecovery::info.sd_longname);
			break;
		case POP_READFILE_FAIL:
			npInfo->addChars(tip_read_fail_en);
			break;
		case POP_INSERT_CARD:
			npInfo->addChars(tip_insert_sdcard_en);
			break;
		}
	}

	/***********操作指定***********/
	switch(messageIndexNow){
	case POP_KILL_ERROR://出错
		lbOK->fun = funGeneralButton;
		lbOK->funParam = BUTTON_ERR_CONFIRM;
		break;
	case POP_LANGUAGE://语言切换确认
		lbOK->fun = exchangeLang;
		lbCancel->fun = funGeneralButton;
		lbCancel->funParam = BUTTON_CLOSE_POPWIN;
		break;
	case POP_FACTORY_RESET://恢复出厂设置确认
		lbOK->fun = funGeneral;
		lbOK->funNext = screenFactoryReset;
		lbCancel->fun = funGeneralButton;
		lbCancel->funParam = BUTTON_CLOSE_POPWIN;
		break;
	case POP_CALIBRATE_CONFIRM://校准确认
		lbOK->fun = funGeneralButton;
		lbOK->funParam = BUTTON_CALI_SAVE;
		lbCancel->fun = funGeneral;
		lbOK->funNext = screenSystem;
		lbCancel->funNext = screenSystem;
		break;
	case POP_STOP_PRINT://是否停止打印
		lbOK->fun = funGeneralButton;
		lbOK->funParam = BUTTON_STOP_PRINT;
		lbCancel->fun = funGeneralButton;
		lbCancel->funParam = BUTTON_CLOSE_POPWIN;
		break;
	case POP_RECOVERY_PRINT://是否恢复打印
		lbOK->fun = funGeneralButton;
		lbOK->funNext = screenResumePrint;
		lbOK->funParam = BUTTON_NEXT;
		lbCancel->fun = funGeneral;
		lbCancel->funNext = screenCancelPrint;
		break;
	case POP_COMPLETE_PRINT://打印完成
		lbOK->fun = funGeneral;
		lbOK->funNext = screenMain;
		lastScreen = nullptr;
		break;
	case POP_DELETE_FILE: //删除一个文件
		lbOK->fun = funGeneralButton;
		lbOK->funNext = screenDeleteFile;
		lbOK->funParam = BUTTON_NEXT;
		lbCancel->fun = funGeneralButton;
		lbCancel->funParam = BUTTON_CLOSE_POPWIN;
		break;
	case POP_EXTRUDER_COLD://喷头过冷
	case POP_NOSUPPORT://暂不支持
	case POP_NORMAL_MODE:
	case POP_WAITNOZZLE_HEAT:
		lbOK->fun = funGeneralButton;
		lbOK->funParam = BUTTON_CLOSE_POPWIN;
		break;
	case POP_PROB_FAIL:
		lbOK->fun = funGeneral;
		lbOK->funNext = screenTool;
		break;
	case POP_FILAMENT_INSERT:
		lbOK->fun = funGeneralButton;
		lbOK->funParam = BUTTON_CLOSE_POPWIN;
		lbCancel->fun = funPopScreen;
		lbCancel->funParam = POP_STOP_PRINT;
		break;
	#if ENABLED(DUAL_X_CARRIAGE)
	case POP_SAVE_HOTENDOFFSET:
		lbOK->fun = funConfirmHotendOffset;
		lbOK->funParam = 1;
		lbCancel->fun = funConfirmHotendOffset;
		lbCancel->funParam = 2;
		break;
	#endif
	case POP_OPENFILE_FAIL://打开文件失败,是否重试
		lbOK->fun = funGeneralButton;
		lbOK->funParam = BUTTON_CLOSE_POPWIN;
		lbCancel->fun = funGeneral;
		lbCancel->funNext = screenCancelPrint;
		break;
	case POP_READFILE_FAIL:
		lbOK->fun = funGeneralButton;
		lbOK->funNext = screenReadFail;
		lbOK->funParam = BUTTON_NEXT;
		break;
	case POP_INSERT_CARD:
		lbOK->fun = funGeneralButton;
		lbOK->funNext = screenInserCard;
		lbOK->funParam = BUTTON_NEXT;
		lbCancel->fun = funGeneral;
		lbCancel->funNext = screenCancelPrint;
		break;
	default:break;
	}
}
void screenPopIndex(int index,VoidFun other)
{
	funAfter = other;
	messageIndexNext = index;
	if(logTime == 0) {
		lastScreen = currentScreen;//保留当前
		currentScreen = screenMessageBox;
	}
	else { //断电续打时,处于logscreen阶段,不能弹窗,因此,保存起来,到主界面时,screenMain会执行lastScreen弹出来
		lastScreen = screenMessageBox;
	}
}
void turntoMainscreen(void)
{
	logTime = 0;
	topwin->touchReset();
	#if MULTI_LANGUAGE
	refreshTheme();
	screenGlobalStatus();//YSZ-GOGO:这里需要验证断电续打弹窗功能
	#endif
	currentScreen = screenMain;
	if(lastScreen == nullptr)SERIAL_ECHOLN("Last screen is null.");//YSZ-GOGO
	constexpr tone_t music[7] = {{200,420},{200,0},{100,520},{20,0},{300,620},{20,0},{300,1020}};
	for(int i = 0; i < 7; i++)buzzer.tone(music[i].duration,music[i].frequency);
}
static void cross(color_t color)
{
	#define CLOSS_R   8
	int xStart,yStart,xEnd,yEnd,x,y;
	switch(Calibrate_P.index){
	case 0:
		x = CALIBRATE_DIS;
		y = CALIBRATE_DIS;
		break;
	case 1:
		x = LCD_LMAX - CALIBRATE_DIS;
		y = CALIBRATE_DIS;
		break;
	case 2:
		x = LCD_LMAX - CALIBRATE_DIS;
		y = LCD_WMAX - CALIBRATE_DIS;
		break;
	default:
		x = CALIBRATE_DIS;
		y = LCD_WMAX - CALIBRATE_DIS;
		break;
	}
	xStart = x - CLOSS_R;
	xEnd = x + CLOSS_R;
	yStart = y - CLOSS_R;
	yEnd = y + CLOSS_R;
	HORIZONTAL_LINE(color,y,xStart,xEnd);
	VERTICAL_LINE(color,x,yStart,yEnd);
}
static void crossFun(yWidget*w,WidgetEvent evt)
{
	if(evt != wevt_untouch)return;
	cross(COLOR_WHITE);
	Calibrate_P.index++;
	switch(Calibrate_P.index){
	case 1:
		Calibrate_P.touchX[0] = tx;
		Calibrate_P.touchY[0] = ty;
		break;
	case 2:
		Calibrate_P.touchX[1] = tx;
		Calibrate_P.touchY[1] = ty;
		break;
	case 3:
		Calibrate_P.touchX[2] = tx;
		Calibrate_P.touchY[2] = ty;
		break;
	default:
		screenPopIndex(POP_CALIBRATE_CONFIRM);
		Calibrate_P.touchX[3] = tx;
		Calibrate_P.touchY[3] = ty;
		return;
	}
	while(!LCD_BatchOk()){} //先绘制完前面的十字,不然赋值颜色会影响前面的十字

	cross(COLOR_BLUE);
	char str[2] = {(char)(Calibrate_P.index + '1'),0};
	lbComponents[0]->setChars(str);
}
static void screenAdjust(void)
{
	closeAllWin();
	currentScreen = nullptr;
	_NEW_W(lbComponents[0],yLabel(0,0,LCD_LMAX,LCD_WMAX,&pic_white,nullptr,false));
	topwinComponents[0] = lbComponents[0];
	if(my_lang == LANG_CHINESE) {
		WORD adjust_cn[] = {0x5c4f,0x5e55,0x6821,0x51c6,0};//3
		lbComponents[0]->setWBuffer(0,adjust_cn);
		lbComponents[0]->setWPos(LCD_LMAX / 2 - DEFAULT_UFONT.xPixel * 2,LCD_WMAX / 2);
	}
	else {
		char adjust_en[] = "Calibrate screen";//15
		lbComponents[0]->setCBuffer(0,adjust_en);
		lbComponents[0]->setWPos(LCD_LMAX / 2 - 8 * DEFAULT_FONT_XPIXEL,LCD_WMAX / 2);
	}
	_NEW_W(topwinComponents[1],yWidget(0,0,0,0,nullptr,nullptr));
	_NEW_W(lbComponents[0],yLabel((LCD_LMAX - FONT_NUM.xPixel) / 2 - 1,
	LCD_WMAX / 2 - FONT_NUM.yPixel,
	FONT_NUM.xPixel + 2,FONT_NUM.yPixel,&pic_white,nullptr,false));
	topwinComponents[2] = lbComponents[0];

	winAttr.componentNum = 3;
	_NEW_W(topwin,yWindows(&winAttr,topwin));
	Calibrate_P.index = 0;
	lbComponents[0]->wEnPic = &Font_En10x20;
	lbComponents[0]->setCBuffer(1,"1");
	lbComponents[0]->setBinaryOpaque(true);
	topwinComponents[1]->fun = crossFun;
	topwinComponents[1]->alwaysExecute = true;
	topwin->update();
	topwin->clrUpdateSign();
	cross(COLOR_BLUE);
}

void screenSystem(void)
{
	closeAllWin();
	currentScreen = nullptr;
		if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_SYSTEM_CN);
		else pic_page.value = MCU_PIC_ADDR(POS_SYSTEM_EN);
		_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
		_NEW_W(topwinComponents[1],yWidget(376,178,100,116,nullptr,&picMain_focus));//返回
		_NEW_W(topwinComponents[2],yWidget(18,32,100,116,nullptr,&picMain_focus));//状态
		_NEW_W(topwinComponents[3],yWidget(136,32,100,116,nullptr,&picMain_focus));//信息
		_NEW_W(topwinComponents[4],yWidget(256,32,100,116,nullptr,&picMain_focus));//语言切换
		_NEW_W(topwinComponents[5],yWidget(376,32,100,116,nullptr,&picMain_focus));//恢复出厂设置
		_NEW_W(topwinComponents[6],yWidget(18,178,100,116,nullptr,&picMain_focus));//校准
		_NEW_W(topwinComponents[7],yWidget(136,178,100,116,nullptr,&picMain_focus));
		winAttr.componentNum = 8;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenMain;
	topwinComponents[2]->fun = funGeneral;
	topwinComponents[2]->funNext = screenStatus;
	topwinComponents[3]->fun = funGeneral;
	topwinComponents[3]->funNext = screenInfo;
	topwinComponents[4]->fun = funPopScreen;
	topwinComponents[4]->funParam = POP_LANGUAGE;
	topwinComponents[5]->fun = funPopScreen;
	topwinComponents[5]->funParam = POP_FACTORY_RESET;
	topwinComponents[6]->fun = funGeneral;
	topwinComponents[6]->funNext = screenAdjust;
	topwinComponents[7]->fun = funPopScreen;
	topwinComponents[7]->funParam = POP_NORMAL_MODE;
}

//SECTOR2:****************************工具****************************
void distanceChoose(yWidget*w,WidgetEvent evt)
{
	switch(evt){
	case wevt_away:if(w->funParam != distanceIndex)w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		if(distanceIndex != w->funParam){
			switch(distanceIndex) {
			case 1:topwinComponents[2]->setfocus(false);break;
			case 2:topwinComponents[3]->setfocus(false);break;
			case 3:topwinComponents[4]->setfocus(false);break;
			case 4:topwinComponents[5]->setfocus(false);break;//active_extruder
			}
			distanceIndex = w->funParam;
		}
		break;
	default:break;
	}
}
void extruderExchange(yWidget*w,WidgetEvent evt)
{
	switch(evt){
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		#if HAS_MULTI_EXTRUDER
		if ( !TERN0(DUAL_X_CARRIAGE, dxc_is_duplicating())
			&& !TERN0(MULTI_NOZZLE_DUPLICATION, extruder_duplication_enabled)
			){
				yLabel *lb = (yLabel*)w;
				if(active_extruder == 0) {
					lb->setChars("E2");
					lb->updateTextIfChange = true;
					queue.inject_P("T1");
				}
				else {
					lb->setChars("E1");
					lb->updateTextIfChange = true;
					queue.inject_P("T0");
				}
			}
		#endif
		break;
	default:break;
	}
}
void axisMotion(yWidget*w,WidgetEvent evt)
{
	double dis = 0;
	char str[32],tstr[32];
	sync_plan_position();
	switch(evt){
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		switch(distanceIndex){
		case 1:dis = 0.01;break;
		case 2:dis = 0.1;break;
		case 3:dis = 1.0;break;
		case 4:dis = 10.0;break;
		}
		w->setfocus(false);
		switch(w->funParam) {
		case 10://HOME
			if(!planner.has_blocks_queued())queue.inject_P("G28");
			break;
		case 7://Y-
			// if(!planner.has_blocks_queued())
			{
				#if 0
				if(current_position.y < Y_MAX_POS) {
					dis = current_position.y + dis;
					if(dis > Y_MAX_POS)dis = Y_MAX_POS;
					sprintf_P(str,"G0 Y%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#else
				if(current_position.y > 0) {
					dis = current_position.y - dis;
					if(dis < 0)dis = 0;
					sprintf_P(str,"G0 Y%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#endif
			}
			break;
		case 9://X-
			// if(!planner.has_blocks_queued())
			{
				#if 0
				dis = planner.get_axis_position_mm(X_AXIS) - dis;
				planner.buffer_line(dis, planner.get_axis_position_mm(Y_AXIS),planner.get_axis_position_mm(Z_AXIS),
					planner.get_axis_position_mm(E_AXIS),MMS_SCALED(feedrate_mm_s), X_AXIS,0.0);
				#else
				if(current_position.x > 0) {
					dis = current_position.x - dis;
					if(dis < 0)dis = 0;
					sprintf_P(str,"G0 X%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#endif
			}
			break;
		case 11://X+
			// if(!planner.has_blocks_queued())
			{
				#if 0
				dis += planner.get_axis_position_mm(X_AXIS);
				planner.buffer_line(dis, planner.get_axis_position_mm(Y_AXIS),planner.get_axis_position_mm(Z_AXIS),
					planner.get_axis_position_mm(E_AXIS),MMS_SCALED(feedrate_mm_s), X_AXIS,0.0);
				#else
				if(current_position.x < X_MAX_POS) {
					dis = current_position.x + dis;
					if(dis > X_MAX_POS)dis = X_MAX_POS;
					sprintf_P(str,"G0 X%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#endif
			}
			break;
		case 13://Y+
			// if(!planner.has_blocks_queued())
			{
				#if 0
				if(current_position.y > 0) {
					dis = current_position.y - dis;
					if(dis < 0)dis = 0;
					sprintf_P(str,"G0 Y%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#else
				if(current_position.y < Y_MAX_POS) {
					dis = current_position.y + dis;
					if(dis > Y_MAX_POS)dis = Y_MAX_POS;
					sprintf_P(str,"G0 Y%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#endif
			}
			break;
		case 8://Z+
			// if(!planner.has_blocks_queued())
			{
				#if 0
				if(current_position.z > 0){
					dis = current_position.z - dis;
					if(dis < 0)dis = 0;
					sprintf_P(str,"G0 Z%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#else
				if(current_position.z < Z_MAX_POS){
					dis = current_position.z + dis;
					if(dis > Z_MAX_POS)dis = Z_MAX_POS;
					sprintf_P(str,"G0 Z%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#endif
			}
			break;
		case 14://Z-
			// if(!planner.has_blocks_queued()) 
			{
				#if 0
				if(current_position.z < Z_MAX_POS){
					dis = current_position.z + dis;
					if(dis > Z_MAX_POS)dis = Z_MAX_POS;
					sprintf_P(str,"G0 Z%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#else
				if(current_position.z > 0){
					dis = current_position.z - dis;
					if(dis < 0)dis = 0;
					sprintf_P(str,"G0 Z%s",dtostrf(dis,4,2,tstr));
					queue.enqueue_one_P(str);
				}
				#endif
			}
			break;
		case 12://E-
			if(thermalManager.tooColdToExtrude(active_extruder)){
				screenPopIndex(POP_EXTRUDER_COLD);
			}
			else{
				dis = current_position.e - dis;
				sprintf_P(str,"G0 E%s",dtostrf(dis,4,2,tstr));
				queue.enqueue_one_P(str);
			}
			break;
		case 6://E+
			if(thermalManager.tooColdToExtrude(active_extruder)){
				screenPopIndex(POP_EXTRUDER_COLD);
			}
			else {
				dis = current_position.e + dis;
				sprintf_P(str,"G0 E%s",dtostrf(dis,4,2,tstr));
				queue.enqueue_one_P(str);
			}
			break;
		default:break;
		}
		break;
	default:break;
	}
}
void screenEmergency(void)
{
	QUICK_STOP();//stop run
	disable_all_steppers();
	wait_for_heatup = false;
	thermalManager.disable_all_heaters();
	thermalManager.set_fan_speed(0, 0);
	#if FAN_COUNT > 1
	thermalManager.set_fan_speed(1, 0);
	#endif
	#if FAN_COUNT > 2
	thermalManager.set_fan_speed(2, 0);
	#endif
	#if FAN_COUNT > 3
	thermalManager.set_fan_speed(3, 0);
	#endif
	#if ENABLED(SDSUPPORT)
	card.fileHasFinished();
	#endif
	currentScreen = nullptr;
}
void screenManual(void)
{
	closeAllWin();
	currentScreen = nullptr;
	pic_page.value = MCU_PIC_ADDR(POS_MANUAL);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(408,259,60,60,nullptr,&picMain_focus));
	_NEW_W(topwinComponents[2],yWidget(68,259,80,45,nullptr,&picMain_focus));//0.01
	_NEW_W(topwinComponents[3],yWidget(148,259,80,45,nullptr,&picMain_focus));//0.1
	_NEW_W(topwinComponents[4],yWidget(228,259,77,45,nullptr,&picMain_focus));//1.0
	_NEW_W(topwinComponents[5],yWidget(305,259,79,45,nullptr,&picMain_focus));//10.0
	_NEW_W(topwinComponents[6],yWidget(30,17,100,64,nullptr,&picMain_focus));//E+
	_NEW_W(topwinComponents[7],yWidget(183,17,100,64,nullptr,&picMain_focus));//Y-
	_NEW_W(topwinComponents[8],yWidget(342,17,80,64,nullptr,&picMain_focus));//Z+
	_NEW_W(topwinComponents[9],yWidget(110,90,66,92,nullptr,&picMain_focus));//X-
	_NEW_W(topwinComponents[10],yWidget(186,90,94,92,nullptr,&picMain_focus));//HOME
	_NEW_W(topwinComponents[11],yWidget(292,90,66,92,nullptr,&picMain_focus));//X+
	_NEW_W(topwinComponents[12],yWidget(30,186,100,64,nullptr,&picMain_focus));//E-
	_NEW_W(topwinComponents[13],yWidget(183,186,100,64,nullptr,&picMain_focus));//Y+
	_NEW_W(topwinComponents[14],yWidget(342,186,80,64,nullptr,&picMain_focus));//Z-
	_NEW_W(lbComponents[0],yLabel(6,260,58,44,nullptr,&picMain_focus));//extruders exchange
	topwinComponents[15] = lbComponents[0];
	lbComponents[0]->setBinaryFrColor(COLOR_BLACK,COLOR_WHITE);
	lbComponents[0]->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLACK);
	lbComponents[0]->setWPos(29 - FONT_NUM.xPixel,22 - FONT_NUM.yPixel / 2);
	lbComponents[0]->wEnPic = &FONT_NUM;
	if(active_extruder == 0)lbComponents[0]->setCBuffer(3,"E1");
	else lbComponents[0]->setCBuffer(3,"E2");
	lbComponents[0]->updateTextIfChange = true;

	winAttr.componentNum = 16;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenTool;
	topwinComponents[2]->funParam = 1;
	topwinComponents[2]->fun = distanceChoose;
	topwinComponents[3]->funParam = 2;
	topwinComponents[3]->fun = distanceChoose;
	topwinComponents[4]->funParam = 3;
	topwinComponents[4]->fun = distanceChoose;
	topwinComponents[5]->funParam = 4;
	topwinComponents[5]->fun = distanceChoose;
	distanceIndex = 4;
	topwinComponents[5]->setfocus(true);
	
	topwinComponents[6]->fun = axisMotion;//E+
	topwinComponents[6]->funParam = 6;
	topwinComponents[7]->fun = axisMotion;//Y-
	topwinComponents[7]->funParam = 7;
	topwinComponents[8]->fun = axisMotion;//Z+
	topwinComponents[8]->funParam = 8;
	topwinComponents[9]->fun = axisMotion;//X-
	topwinComponents[9]->funParam = 9;
	topwinComponents[10]->fun = axisMotion;//HOME
	topwinComponents[10]->funParam = 10;
	topwinComponents[11]->fun = axisMotion;//X+
	topwinComponents[11]->funParam = 11;
	topwinComponents[12]->fun = axisMotion;//E-
	topwinComponents[12]->funParam = 12;
	topwinComponents[13]->fun = axisMotion;//Y+
	topwinComponents[13]->funParam = 13;
	topwinComponents[14]->fun = axisMotion;//Z-
	topwinComponents[14]->funParam = 14;

	topwinComponents[15]->fun = extruderExchange;//extruders exchange
}
void preheatFun(yWidget*w,WidgetEvent evt)
{
	int16_t t;
	switch(evt){
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch (w->funParam)
		{
		case 1://热床-
			t = thermalManager.degTargetBed();
			if(t > 0){
				t -= 4;
				if(t < 0)t = 0;
				thermalManager.setTargetBed(t);
			}
			break;
		case 2://热床+
			t = thermalManager.degTargetBed() + 4;
			if(t > BED_MAXTEMP)t = BED_MAXTEMP;
			thermalManager.setTargetBed(t);
			break;
		case 3://热床切换
			t = thermalManager.degTargetBed();
			if(t > 0){
				thermalManager.setTargetBed(0);
			}
			else {
				thermalManager.setTargetBed(PREHEAT_1_TEMP_BED);
			}
			break;
		case 4://喷头1-
			t = thermalManager.degTargetHotend(0);
			if(t > 0){
				t -= 4;
				if(t < 0)t = 0;
				thermalManager.setTargetHotend(t,0);
			}
			break;
		case 5://喷头1+
			t = thermalManager.degTargetHotend(0) + 4;
			if(t > Temperature::heater_maxtemp[0])t = Temperature::heater_maxtemp[0];
			thermalManager.setTargetHotend(t,0);
			break;
		case 6://喷头1切换
			t = thermalManager.degTargetHotend(0);
			if(t > 0){
				thermalManager.setTargetHotend(0,0);
			}
			else {
				thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND,0);
			}
			break;
		#if HOTENDS > 1
		case 7://喷头2-
			t = thermalManager.degTargetHotend(1);
			if(t > 0) {
				t -= 4;
				if(t < 0)t = 0;
				thermalManager.setTargetHotend(t,1);
			}
			break;
		case 8://喷头2+
			t = thermalManager.degTargetHotend(1) + 4;
			if(t > Temperature::heater_maxtemp[1])t = Temperature::heater_maxtemp[1];
			thermalManager.setTargetHotend(t,1);
			break;
		case 9://喷头2切换
			t = thermalManager.degTargetHotend(1);
			if(t > 0){
				thermalManager.setTargetHotend(0,1);
			}
			else {
				thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND,1);
			}
			break;
		#endif
		default:break;
		}
		break;
	default:break;
	}
}
void screenUpdateTemperature(void)
{
	uint32_t tnow = millis();
	if(tnow >= last){
		last = tnow + 500;
		showHotend();
		showBed();
	}
}
void screenPreheat(void)
{
	closeAllWin();
	currentScreen = screenUpdateTemperature;
	pic_page.value = MCU_PIC_ADDR(POS_PREHEAT);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(400,256,60,62,nullptr,&picMain_focus));//返回
	_NEW_W(topwinComponents[2],yWidget(120,32,52,60,nullptr,&picMain_focus));//热床-
	_NEW_W(lbComponents[0],yLabel(188,36,170,48,nullptr,&picMain_focus));//热床切换
	topwinComponents[3] = lbComponents[0];
	_NEW_W(topwinComponents[4],yWidget(376,32,52,60,nullptr,&picMain_focus));//热床+
	_NEW_W(topwinComponents[5],yWidget(120,107,52,60,nullptr,&picMain_focus));//喷头1-
	_NEW_W(lbComponents[1],yLabel(188,112,170,48,nullptr,&picMain_focus));//喷头1切换
	topwinComponents[6] = lbComponents[1];
	_NEW_W(topwinComponents[7],yWidget(376,107,52,60,nullptr,&picMain_focus));//喷头1+
	#if HOTENDS > 1
	_NEW_W(topwinComponents[8],yWidget(120,183,52,60,nullptr,&picMain_focus));//喷头2-
	_NEW_W(lbComponents[2],yLabel(188,188,170,48,nullptr,&picMain_focus));//喷头2切换
	topwinComponents[9] = lbComponents[2];
	_NEW_W(topwinComponents[10],yWidget(376,183,52,60,nullptr,&picMain_focus));//喷头2+
	winAttr.componentNum = 11;
	#else
	_NEW_W(lbComponents[2],yLabel(188,188,170,48,nullptr,&picMain_focus));//喷头2切换
	topwinComponents[8] = lbComponents[2];
	winAttr.componentNum = 9;
	#endif
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	lbComponents[0]->setBinaryFrColor(COLOR_BLACK,0);
	lbComponents[0]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
	lbComponents[0]->setBinaryOpaque(true);
	lbComponents[0]->setCBuffer(7);
	lbComponents[0]->setWPos(77 - 3 * FONT_NUM.xPixel,24 - FONT_NUM.yPixel / 2);
	lbComponents[0]->wEnPic = &FONT_NUM;

	lbComponents[1]->setBinaryFrColor(COLOR_BLACK,0);
	lbComponents[1]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
	lbComponents[1]->setBinaryOpaque(true);
	lbComponents[1]->setCBuffer(7);
	lbComponents[1]->setWPos(77 - 3 * FONT_NUM.xPixel,24 - FONT_NUM.yPixel / 2);
	lbComponents[1]->wEnPic = &FONT_NUM;

	lbComponents[2]->setBinaryFrColor(COLOR_BLACK,0);
	lbComponents[2]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
	lbComponents[2]->setBinaryOpaque(true);
	lbComponents[2]->wEnPic = &FONT_NUM;

	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenTool;
	topwinComponents[2]->fun = preheatFun;
	topwinComponents[2]->funParam = 1;
	topwinComponents[4]->fun = preheatFun;
	topwinComponents[4]->funParam = 2;
	topwinComponents[3]->fun = preheatFun;
	topwinComponents[3]->funParam = 3;
	topwinComponents[5]->fun = preheatFun;
	topwinComponents[5]->funParam = 4;
	topwinComponents[7]->fun = preheatFun;
	topwinComponents[7]->funParam = 5;
	topwinComponents[6]->fun = preheatFun;
	topwinComponents[6]->funParam = 6;
	#if HOTENDS > 1
	lbComponents[2]->setCBuffer(7);
	lbComponents[2]->setWPos(77 - 3 * FONT_NUM.xPixel,24 - FONT_NUM.yPixel / 2);
	topwinComponents[8]->fun = preheatFun;
	topwinComponents[8]->funParam = 7;
	topwinComponents[10]->fun = preheatFun;
	topwinComponents[10]->funParam = 8;
	topwinComponents[9]->fun = preheatFun;
	topwinComponents[9]->funParam = 9;
	#else
	lbComponents[2]->setCBuffer(3,"---");
	lbComponents[2]->setWPos(77 - FONT_NUM.xPixel,24 - FONT_NUM.yPixel / 2);
	#endif
}
static inline void moveEAxis(uint8_t e,int dis,bool dir)
{
	#if HAS_MULTI_EXTRUDER
	uint8_t t = active_extruder;
	active_extruder = e;
	#endif
	if(dir)current_position.e += dis / planner.e_factor[e];
	else current_position.e -= dis / planner.e_factor[e];
	line_to_current_position(FILAMENT_CHANGE_FAST_LOAD_FEEDRATE);
	#if HAS_MULTI_EXTRUDER
	active_extruder = t;
	#endif
}
void fixmaterialFun(yWidget*w,WidgetEvent evt)
{
	int16_t t;
	switch(evt){
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch(w->funParam){
		case 3://E0温度预热
			thermalManager.reset_hotend_idle_timer(0);//有可能断料时长超过PAUSE_PARK_NOZZLE_TIMEOUT,加不上温度,必须复位后才能加
			t = thermalManager.degTargetHotend(0);
			if(t > 0) {
				thermalManager.setTargetHotend(0,0);
			}
			else {
				thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND,0);
			}
			break;
		case 4://Stop
			quickstop_stepper();
			break;
		case 5://E1+
			if(thermalManager.tooColdToExtrude(0)){
				screenPopIndex(POP_EXTRUDER_COLD);
			}
			else if(!planner.has_blocks_queued())moveEAxis(0,1999,true);
			break;
		#if EXTRUDERS > 1
		case 7://E2+
			if(thermalManager.tooColdToExtrude(1)){
				screenPopIndex(POP_EXTRUDER_COLD);
			}
			else if(!planner.has_blocks_queued())moveEAxis(1,1999,true);
			break;
		#endif
		case 6://E1-
			if(thermalManager.tooColdToExtrude(0)){
				screenPopIndex(POP_EXTRUDER_COLD);
			}
			else if(!planner.has_blocks_queued())moveEAxis(0,1999,false);
			break;
		#if EXTRUDERS > 1
		case 8://E2-
			if(thermalManager.tooColdToExtrude(1)){
				screenPopIndex(POP_EXTRUDER_COLD);
			}
			else if(!planner.has_blocks_queued())moveEAxis(1,1999,false);
			break;
		#endif
		#if HOTENDS > 1
		case 9://E1温度预热
			thermalManager.reset_hotend_idle_timer(1);//有可能断料时长超过PAUSE_PARK_NOZZLE_TIMEOUT,加不上温度,必须复位后才能加
			t = thermalManager.degTargetHotend(1);
			if(t > 0){
				lbComponents[2]->setBinaryFrColor(COLOR_BLACK,0);
				lbComponents[2]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
				thermalManager.setTargetHotend(0,1);
			}
			else {
				lbComponents[2]->setBinaryFrColor(COLOR_RED,0);
				lbComponents[2]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
				thermalManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND,1);
			}
			break;
		#endif
		default:break;
		}
		break;
	default:break;
	}
}
void screenUpdateHotend(void)
{
	uint32_t tnow = millis();
	if(tnow >= last){
		last = tnow + 500;
		showHotend();
	}
}

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
static void funBackPrint(yWidget*w,WidgetEvent evt){
	void screenPrinting(void);
	#if HAS_FILAMENT_SENSOR
	bool runout = false;
	#endif
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		#if HAS_FILAMENT_SENSOR
			if (TERN0(DUAL_X_CARRIAGE, dxc_is_duplicating())
			|| TERN0(MULTI_NOZZLE_DUPLICATION, extruder_duplication_enabled) //复制模式
			){
				runout = READ(FIL_RUNOUT_PIN) | READ(FIL_RUNOUT2_PIN);
			}
			else {
				if(active_extruder == 0)runout = READ(FIL_RUNOUT_PIN);
				else runout = READ(FIL_RUNOUT2_PIN);
			}
			if(runout)
		#else
			if(1)
		#endif
		{//判断有料
			screenPopIndex(POP_FILAMENT_INSERT);
		}
		else {
			wait_for_user = false;
			quickstop_stepper();
			currentScreen = screenPrinting;
			my_print_status = PRINT_RESUME;
		}
		break;
	default:break;
	}
}
void screenFilament(void)
{
	closeAllWin();
	currentScreen = screenUpdateHotend;
	#if HOTENDS > 1 //独立双头
	pic_page.value = MCU_PIC_ADDR(POS_FIXMATERIAL3);
	#elif EXTRUDERS > 1 //二进一
	pic_page.value = MCU_PIC_ADDR(POS_FIXMATERIAL2);
	#else
	pic_page.value = MCU_PIC_ADDR(POS_FIXMATERIAL);
	#endif
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(398,229,60,64,nullptr,&picMain_focus));//back
	#if HOTENDS > 1 //独立双头
		_NEW_W(lbComponents[1],yLabel(140,44,200,56,&pic_value_bg,&pic_value_bgfc));//E1 temperature
		lbComponents[1]->setCBuffer(9);
		topwinComponents[2] = lbComponents[1];
		_NEW_W(lbComponents[2],yLabel(140,135,200,56,&pic_value_bg,&pic_value_bgfc));//E2 temperature
		lbComponents[2]->setCBuffer(9);
		topwinComponents[8] = lbComponents[2];
		_NEW_W(topwinComponents[3],yWidget(22,40,90,60,nullptr,&picMain_focus));//E1+
		_NEW_W(topwinComponents[4],yWidget(366,40,90,60,nullptr,&picMain_focus));//E1-
		_NEW_W(topwinComponents[5],yWidget(22,130,90,60,nullptr,&picMain_focus));//E2+
		_NEW_W(topwinComponents[6],yWidget(366,130,90,60,nullptr,&picMain_focus));//E2-
		_NEW_W(topwinComponents[7],yWidget(200,225,80,72,nullptr,&picMain_focus));//stop
		winAttr.componentNum = 9;
	#elif EXTRUDERS > 1 //二进一
		_NEW_W(lbComponents[1],yLabel(140,95,200,56,nullptr,&picMain_focus));//E1 temperature
		lbComponents[1]->setCBuffer(9);
		topwinComponents[2] = lbComponents[1];
		_NEW_W(topwinComponents[3],yWidget(22,40,90,60,nullptr,&picMain_focus));//E1+
		_NEW_W(topwinComponents[4],yWidget(366,40,90,60,nullptr,&picMain_focus));//E1-
		_NEW_W(topwinComponents[5],yWidget(22,130,90,60,nullptr,&picMain_focus));//E2+
		_NEW_W(topwinComponents[6],yWidget(366,130,90,60,nullptr,&picMain_focus));//E2-
		_NEW_W(topwinComponents[7],yWidget(200,225,80,72,nullptr,&picMain_focus));//stop
		winAttr.componentNum = 8;
	#else
		_NEW_W(lbComponents[1],yLabel(140,95,200,56,nullptr,&picMain_focus));//E1 temperature
		lbComponents[1]->setCBuffer(9);
		topwinComponents[2] = lbComponents[1];
		_NEW_W(topwinComponents[3],yWidget(22,92,90,60,nullptr,&picMain_focus));//E+
		_NEW_W(topwinComponents[4],yWidget(366,92,90,60,nullptr,&picMain_focus));//E-
		_NEW_W(topwinComponents[5],yWidget(200,225,80,72,nullptr,&picMain_focus));//stop
		winAttr.componentNum = 6;
	#endif
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	if(my_print_status == PRINT_RUNOUT){
		out_extruder = runout.outWhere();
		topwinComponents[1]->fun = funBackPrint;
	}
	else {
		topwinComponents[1]->fun = funGeneral;
		topwinComponents[1]->funNext = screenTool;
	}
	topwinComponents[3]->fun = fixmaterialFun;
	topwinComponents[3]->funParam = 5;//E+
	topwinComponents[4]->fun = fixmaterialFun;
	topwinComponents[4]->funParam = 6;//E-
	#if EXTRUDERS > 1
	topwinComponents[5]->fun = fixmaterialFun;
	topwinComponents[5]->funParam = 7;//E2+
	topwinComponents[6]->fun = fixmaterialFun;
	topwinComponents[6]->funParam = 8;//E2-
	#endif

	lbComponents[1]->setBinaryOpaque(true);//E1 temperature
	lbComponents[1]->wEnPic = &FONT_NUM;
	lbComponents[1]->setWPos(92 - 3 * FONT_NUM.xPixel,28 - FONT_NUM.yPixel / 2);
	lbComponents[1]->setBinaryFrColor(COLOR_BLACK,COLOR_BLACK);
	lbComponents[1]->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_LIGHTBLUE_T);
	lbComponents[1]->updateTextIfChange = true;
	lbComponents[1]->fun = fixmaterialFun;
	lbComponents[1]->funParam = 3;

	#if HOTENDS > 1 //独立双头
		lbComponents[2]->setBinaryOpaque(true);//E2 temperature
		lbComponents[2]->setWPos(92 - 3 * FONT_NUM.xPixel,28 - FONT_NUM.yPixel / 2);
		lbComponents[2]->wEnPic = &FONT_NUM;
		lbComponents[2]->setBinaryFrColor(VALUE_FR_COLOR,VALUE_FR_COLOR);
		lbComponents[2]->setBinaryBgColor(VALUE_BG_COLOR,VALUE_BGFC_COLOR);
		lbComponents[2]->updateTextIfChange = true;
		lbComponents[2]->fun = fixmaterialFun;
		lbComponents[2]->funParam = 9;
	#endif
	#if EXTRUDERS > 1
		topwinComponents[7]->fun = fixmaterialFun;
		topwinComponents[7]->funParam = 4;
	#else
		topwinComponents[5]->fun = fixmaterialFun;
		topwinComponents[5]->funParam = 4;
	#endif
}
#endif

void drawFrame(yWidget* w,int xStart,int yStart,int xEnd,int yEnd)
{
	w->rect(COLOR_BLACK,xStart,yStart,xEnd,yStart + 1);
	w->rect(COLOR_BLACK,xEnd - 1,yStart,xEnd,yEnd);
	w->rect(COLOR_BLACK,xStart,yEnd - 1,xEnd,yEnd);
	w->rect(COLOR_BLACK,xStart,yStart,xStart + 1,yEnd);
}
#ifdef AUTO_BED_LEVELING_BILINEAR
void screenWaitLevelling(void)
{
	if(my_print_status == PRINT_IDLE){
		current_position.z += 10;
		line_to_current_position();
		watchdog_refresh();
		int index = INDEX_Z_VALUES;
		AT24CXX_Write(index,&z_values,sizeof(z_values));
		queue.enqueue_one_P("G28 XY");
		currentScreen = nullptr;
	}
}
void updateAutoLevel(int i,int j)
{
	if(my_print_status != PRINT_AUTO_LEVEL)return;
	yLabel* lb = (yLabel*)topwinComponents[2 + i + j * GRID_MAX_POINTS_Y];
	yString::doubleToStr(z_values[i][j],lb->char_tar,-6,3);
	lb->sign.content.text = 1;
}
void screenAutoLevel(void)
{
	closeAllWin();
	if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_AUTOLEVEL_CN);
	else pic_page.value = MCU_PIC_ADDR(POS_AUTOLEVEL_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[14],yLabel(36,25,94,42,nullptr,nullptr));
	_NEW_W(topwinComponents[15], yLabel(146,30,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[16],yLabel(247,30,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[17],yLabel(348,25,94,42,nullptr,nullptr));

	_NEW_W(topwinComponents[10],yLabel(44,74,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[11],yLabel(146,74,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[12],yLabel(247,74,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[13],yLabel(348,74,86,36,nullptr,nullptr));

	_NEW_W(topwinComponents[6],yLabel(44,119,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[7],yLabel(146,119,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[8],yLabel(247,119,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[9],yLabel(348,119,86,36,nullptr,nullptr));

	_NEW_W(topwinComponents[2],yLabel(36,163,94,42,nullptr,nullptr));
	_NEW_W(topwinComponents[3],yLabel(146,164,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[4],yLabel(247,164,86,36,nullptr,nullptr));
	_NEW_W(topwinComponents[5],yLabel(348,163,94,42,nullptr,nullptr));
	winAttr.componentNum = 18;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	queue.enqueue_one_P("G28");
	queue.enqueue_one_P("G29");
	my_print_status = PRINT_AUTO_LEVEL;
	currentScreen = screenWaitLevelling;

	topwinComponents[1]->fun = funGeneralButton;
	topwinComponents[1]->funParam = BUTTON_LEVEL_STOP;
	yLabel *lb;
	int y = 18 - FONT_NUM.yPixel / 2;
	for(int i = 2; i < 18; i++){
		lb = (yLabel*)topwinComponents[i];
		lb->setBinaryOpaque(true);
		lb->setCBuffer(7,"0");
		lb->setBinaryFrColor(COLOR_BLACK,0);
		lb->setBinaryBgColor(COLOR_LIGHTBLUE,0);
		lb->setWPos(43 - FONT_NUM.xPixel * 3,y);
		lb->wEnPic = &FONT_NUM;
	}
}
#endif

static void funGotoLevelPos(yWidget*w,WidgetEvent evt){
	const float x_min = 20, x_max = X_BED_SIZE - 20;
	const float y_min = 20, y_max = Y_BED_SIZE - 20;
	char str[64],tstr1[16],tstr2[16];
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		queue.enqueue_one_P("G0 Z10 F6000");
		switch(w->funParam) {
		case 1://左上
			sprintf_P(str,"G0 X%s Y%s",dtostrf(x_min,4,2,tstr1),dtostrf(y_max,4,2,tstr2));
			queue.enqueue_one_P(str);
			break;
		case 2://左下
			sprintf_P(str,"G0 X%s Y%s",dtostrf(x_min,4,2,tstr1),dtostrf(y_min,4,2,tstr2));
			queue.enqueue_one_P(str);
			break;
		case 3://中
			sprintf_P(str,"G0 X%s Y%s",dtostrf((x_min + x_max) / 2,4,2,tstr1),dtostrf((y_min + y_max) / 2,4,2,tstr2));
			queue.enqueue_one_P(str);
			break;
		case 4://右上
			sprintf_P(str,"G0 X%s Y%s",dtostrf(x_max,4,2,tstr1),dtostrf(y_max,4,2,tstr2));
			queue.enqueue_one_P(str);
			break;
		case 5://右下
			sprintf_P(str,"G0 X%s Y%s",dtostrf(x_max,4,2,tstr1),dtostrf(y_min,4,2,tstr2));
			queue.enqueue_one_P(str);
			break;
		}
		queue.enqueue_one_P("G0 Z0");
		break;
	default:break;
	}
}
void screenManualLevel(void)
{
	closeAllWin();
	currentScreen = nullptr;
	if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_MANUALLEVEL_CN);
	else pic_page.value = MCU_PIC_ADDR(POS_MANUALLEVEL_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(404,250,60,64,nullptr,&picMain_focus));

	_NEW_W(topwinComponents[2],yWidget(28,46,94,88,nullptr,&picMain_focus));//左上
	_NEW_W(topwinComponents[5],yWidget(28,150,94,88,nullptr,&picMain_focus));//左下
	_NEW_W(topwinComponents[3],yWidget(164,98,94,88,nullptr,&picMain_focus));//中
	_NEW_W(topwinComponents[4],yWidget(297,46,94,88,nullptr,&picMain_focus));//右上
	_NEW_W(topwinComponents[6],yWidget(297,147,94,88,nullptr,&picMain_focus));//右下
	winAttr.componentNum = 7;
	_NEW_W(topwin,yWindows(&winAttr,topwin));
	queue.enqueue_one_P("T0");//切换到E0
	queue.enqueue_one_P("G28");

	topwinComponents[1]->fun = funGeneralButton;
	topwinComponents[1]->funParam = BUTTON_LEVEL_STOP;
	topwinComponents[2]->fun = funGotoLevelPos;
	topwinComponents[2]->funParam = 1;
	topwinComponents[5]->fun = funGotoLevelPos;
	topwinComponents[5]->funParam = 2;
	topwinComponents[3]->fun = funGotoLevelPos;
	topwinComponents[3]->funParam = 3;
	topwinComponents[4]->fun = funGotoLevelPos;
	topwinComponents[4]->funParam = 4;
	topwinComponents[6]->fun = funGotoLevelPos;
	topwinComponents[6]->funParam = 5;
}
void screenLevelPopConfirm(void)
{
	currentScreen = nullptr;
	lastScreen = nullptr;
	messageIndexNow = 1;//cancel需要用关闭弹窗,而弹窗需要一个不为0的窗号,这里置1
	#ifdef AUTO_BED_LEVELING_BILINEAR
	const WORD auto_cn[] = {0x81ea,0x52a8,0};
	#endif
	const WORD manual_cn[] = {0x624b,0x52a8,0};
	yLabel* lbTip,*lbManual,*lbCancel;
	constexpr int length = LCD_LMAX * 3 / 4;
	constexpr int width = LCD_WMAX * 5 / 12;
	constexpr int bl = LCD_LMAX * 5 / 24;
	constexpr int bw = LCD_WMAX / 8;
	_NEW_W(lbTip,yLabel(0,0,length,width,&pic_popwin_background,nullptr,false));
	#ifdef AUTO_BED_LEVELING_BILINEAR
	yLabel* lbAuto;
	_NEW_W(lbAuto,yLabel(length / 2 - bl - bl / 2 - 8,width / 2,bl,bw,&pic_button_background_idle,&pic_button_background_focus));//自动
	_NEW_W(lbManual,yLabel((length - bl) / 2,width / 2,bl,bw,&pic_button_background_idle,&pic_button_background_focus));//手动
	_NEW_W(lbCancel,yLabel(length / 2 + bl / 2 + 8,width / 2,bl,bw,&pic_button_background_idle,&pic_button_background_focus));//取消
	#else
	_NEW_W(lbManual,yLabel(length / 2 - bl - 4,width / 2,bl,bw,&pic_button_background_idle,&pic_button_background_focus));
	_NEW_W(lbCancel,yLabel(length / 2 + 4,width / 2,bl,bw,&pic_button_background_idle,&pic_button_background_focus));
	#endif
	lbTip->setBinaryBgColor(0,0);
	lbTip->setBinaryFrColor(COLOR_BLUE,0);
	#ifdef AUTO_BED_LEVELING_BILINEAR
	lbAuto->setBinaryFrColor(COLOR_BLUE,COLOR_WHITE);
	lbAuto->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLUE);
	#endif
	lbManual->setBinaryFrColor(COLOR_BLUE,COLOR_WHITE);
	lbManual->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLUE);
	lbCancel->setBinaryFrColor(COLOR_BLUE,COLOR_WHITE);
	lbCancel->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_BLUE);
	lbTip->setWPos(16,20);
	if(my_lang) {
		lbTip->setWBuffer(0,tip_level_cn);
	}
	else {
		lbTip->setCBuffer(0,tip_level_en);
	}
	switch(my_lang) {
	case LANG_CHINESE:
		#ifdef AUTO_BED_LEVELING_BILINEAR
			lbAuto->setWPos((bl - DEFAULT_FONT_XPIXEL * 4) / 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
			lbAuto->setWBuffer(0,auto_cn);
		#endif
		lbManual->setWPos((bl - DEFAULT_FONT_XPIXEL * 4) / 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
		lbManual->setWBuffer(0,manual_cn);
		lbCancel->setWPos((bl - DEFAULT_FONT_XPIXEL * 4) / 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
		lbCancel->setWBuffer(0,cancel_cn);
		break;
	default:
		#ifdef AUTO_BED_LEVELING_BILINEAR
		lbAuto->setWPos(bl / 2 - DEFAULT_FONT_XPIXEL * 2,(bw - DEFAULT_FONT_YPIXEL) / 2);
		lbAuto->setCBuffer(5,"Auto");
		#endif
		lbManual->setWPos(bl / 2 - DEFAULT_FONT_XPIXEL * 3,(bw - DEFAULT_FONT_YPIXEL) / 2);
		lbManual->setCBuffer(7,"Manual");
		lbCancel->setWPos(bl / 2 - DEFAULT_FONT_XPIXEL * 3,(bw - DEFAULT_FONT_YPIXEL) / 2);
		lbCancel->setCBuffer(7,cancel_en);
		break;
	}
	lbManual->updateTextIfChange = true;
	lbCancel->updateTextIfChange = true;
	popComponents[0] = lbTip;
	#ifdef AUTO_BED_LEVELING_BILINEAR
	lbAuto->updateTextIfChange = true;
	popComponents[1] = lbAuto;
	popComponents[2] = lbManual;
	popComponents[3] = lbCancel;
	#else
	popComponents[1] = lbManual;
	popComponents[2] = lbCancel;
	#endif
	WIN_Attribute winAttr = {
		.x = (LCD_LMAX - length) / 2,
		.y = (LCD_WMAX + WIN_YSTART - width) / 2,
		.width = length + 1,
		.height = width + 1,
		.components = popComponents,
		#ifdef AUTO_BED_LEVELING_BILINEAR
		.componentNum = 4,
		#else
		.componentNum = 3,
		#endif
	};
	topwin->update();
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	#ifdef AUTO_BED_LEVELING_BILINEAR
	lbAuto->fun = funGeneral;
	lbAuto->funNext = screenAutoLevel;
	lbManual->fun = funGeneral;
	lbManual->funNext = screenManualLevel;
	lbCancel->fun = funGeneralButton;
	lbCancel->funParam = BUTTON_CLOSE_POPWIN;
	#else
	lbManual->fun = funGeneral;
	lbManual->funNext = screenManualLevel;
	lbCancel->fun = funGeneralButton;
	lbCancel->funParam = BUTTON_CLOSE_POPWIN;
	#endif
}
void fanChange(yWidget*w,WidgetEvent evt) {
	int v,n,index = w->funParam > 3 ? w->funParam - 4 : w->funParam - 1;
	yLabel *lb = lbComponents[index];
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		#ifdef USE_CONTROLLER_FAN
		if(w->funParam == 3) controllerFan.change(0);
		else if(w->funParam == 6)controllerFan.change(255);
		else 
		#endif
		{
			n = v = yString::strToInt(lb->char_tar,NUM10);
			if(w->funParam < 4){//123左侧减少
				if(v >= 4)n = v - 4;
				else n = 0;
			}
			else {//456右侧增加
				if(v <= 96)n = v + 4;
				else n = 100;
			}
			if(n != v){
				n = n * 255 / 100;
				#ifdef THROAT_FAN
				if(index){ //喉管风扇
					#if ENABLED(SINGLENOZZLE) //多进一的时候,风扇与加热头只有一个
					thermalManager.set_fan_speed(THROAT_FAN, n);
					#else
					thermalManager.set_fan_speed(THROAT_FAN + active_extruder, n);
					#endif
				}
				else //冷却风扇
				#endif
				{
					#if ENABLED(SINGLENOZZLE) //多进一的时候,风扇与加热头只有一个
					thermalManager.set_fan_speed(0, n);
					#else
					thermalManager.set_fan_speed(active_extruder, n);
					#endif
				}
			}
		}
		break;
	default:break;
	}
}
void fanOnOff(yWidget*w,WidgetEvent evt) {
	yLabel *lb = (yLabel*)w;
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch(w->funParam){
		case 1:
			do{
				int v = yString::strToInt(lb->char_tar,NUM10);
				if(v >= 50){
					#if ENABLED(SINGLENOZZLE)
					thermalManager.set_fan_speed(0, 0);
					#else
					thermalManager.set_fan_speed(active_extruder, 0);
					#endif
				}
				else {
					#if ENABLED(SINGLENOZZLE)
					thermalManager.set_fan_speed(0, 255);
					#else
					thermalManager.set_fan_speed(active_extruder, 255);
					#endif
				}
			}while(0);
			break;
		#ifdef THROAT_FAN
		case 2:
			do{
				int v = yString::strToInt(lb->char_tar,NUM10);
				if(v >= 50){
					#if ENABLED(SINGLENOZZLE)
					thermalManager.set_fan_speed(THROAT_FAN, 0);
					#else
					thermalManager.set_fan_speed(THROAT_FAN + active_extruder, 0);
					#endif
				}
				else {
					#if ENABLED(SINGLENOZZLE)
					thermalManager.set_fan_speed(THROAT_FAN, 255);
					#else
					thermalManager.set_fan_speed(THROAT_FAN + active_extruder, 255);
					#endif
				}
			}while(0);
			break;
		#endif
		#if ENABLED(USE_CONTROLLER_FAN)
		case 3:
			if(lb->char_tar[1] == 'N'){
				controllerFan.change(0);
			}
			else {
				controllerFan.change(255);
			}
			break;
		#endif
		}
		break;
	default:break;
	}
}
void screenUpdateFan(void)
{
	uint32_t tnow = millis();
	if(tnow > last)
	{
		last = tnow + 200;
		#if ENABLED(SINGLENOZZLE) //多进一的时候,风扇与加热头只有一个
		int v = thermalManager.fan_speed[0];
		#else
		int v = thermalManager.fan_speed[active_extruder];
		#endif
		v = v * 100 / 255;
		sprintf(lbComponents[0]->char_tar,"%-3d",v);
		lbComponents[0]->sign.content.text = 1;
		#if THROAT_FAN
			#if ENABLED(SINGLENOZZLE) //多进一的时候,风扇与加热头只有一个
			v = thermalManager.fan_speed[THROAT_FAN] * (int)100 / 255;
			#else 
			v = thermalManager.fan_speed[THROAT_FAN + active_extruder] * (int)100 / 255;
			#endif
			sprintf(lbComponents[1]->char_tar,"%-3d",v);
			lbComponents[1]->sign.content.text = 1;
		#endif
		#if ENABLED(USE_CONTROLLER_FAN)
			if(controllerFan.state())yString::cpy(lbComponents[2]->char_tar,"ON ");
			else yString::cpy(lbComponents[2]->char_tar,"OFF");
			lbComponents[2]->sign.content.text = 1;
		#endif
	}
}
void screenFan(void)
{
	closeAllWin();
	last = 0;
	currentScreen = screenUpdateFan;
	if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_FAN_CN);
	else pic_page.value = MCU_PIC_ADDR(POS_FAN_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(413,256,62,62,nullptr,&picMain_focus));//返回

	_NEW_W(topwinComponents[2],yWidget(111,25,60,64,nullptr,&picMain_focus));//冷却风扇
	_NEW_W(topwinComponents[3],yWidget(317,25,60,64,nullptr,&picMain_focus));
	_NEW_W(lbComponents[0],yLabel(178,30,130,51,nullptr,&picMain_focus));//213->178
	lbComponents[0]->setCBuffer(3);
	topwinComponents[4] = lbComponents[0];
	_NEW_W(topwinComponents[5],yWidget(111,110,60,64,nullptr,&picMain_focus));//喉管风扇
	_NEW_W(topwinComponents[6],yWidget(317,110,60,64,nullptr,&picMain_focus));
	_NEW_W(lbComponents[1],yLabel(178,116,130,51,nullptr,&picMain_focus));
	lbComponents[1]->setCBuffer(3);
	topwinComponents[7] = lbComponents[1];
	_NEW_W(topwinComponents[8],yWidget(111,195,60,64,nullptr,&picMain_focus));//主板风扇
	_NEW_W(topwinComponents[9],yWidget(317,195,60,64,nullptr,&picMain_focus));
	_NEW_W(lbComponents[2],yLabel(178,202,130,51,nullptr,&picMain_focus));
	lbComponents[2]->setCBuffer(3);
	topwinComponents[10] = lbComponents[2];
	_NEW_W(lbComponents[3],yLabel(413,180,60,51,nullptr,&picMain_focus));//切换头
	lbComponents[3]->setWPos(30 - FONT_NUM.xPixel,26 - FONT_NUM.yPixel / 2);
	lbComponents[3]->setBinaryFrColor(COLOR_WHITE,COLOR_WHITE);
	lbComponents[3]->setBinaryBgColor(0xD0A3,0xD0A3);
	topwinComponents[11] = lbComponents[3];
	winAttr.componentNum = 12;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenTool;
	//中间
	lbComponents[0]->wEnPic = &FONT_NUM;
	int xpos = 60 - FONT_NUM.xPixel;
	int ypos = 26 - FONT_NUM.yPixel / 2;
	lbComponents[0]->setBinaryFrColor(COLOR_BLACK,COLOR_WHITE);
	lbComponents[0]->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_GREY);
	lbComponents[0]->setWPos(xpos,ypos);
	lbComponents[0]->setBinaryOpaque(true);
	lbComponents[0]->funParam = 1;
	lbComponents[0]->fun = fanOnOff;
	#ifdef THROAT_FAN
		lbComponents[1]->wEnPic = &FONT_NUM;
		lbComponents[1]->setWPos(xpos,ypos);
		lbComponents[1]->setBinaryFrColor(COLOR_BLACK,COLOR_WHITE);
		lbComponents[1]->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_GREY);
		lbComponents[1]->setBinaryOpaque(true);
		lbComponents[1]->funParam = 2;
		lbComponents[1]->fun = fanOnOff;
	#endif
	#ifdef USE_CONTROLLER_FAN
		lbComponents[2]->wEnPic = &FONT_NUM;
		lbComponents[2]->setWPos(xpos,ypos);
		lbComponents[2]->setBinaryFrColor(COLOR_BLACK,COLOR_WHITE);
		lbComponents[2]->setBinaryBgColor(COLOR_LIGHTBLUE,COLOR_GREY);
		lbComponents[2]->setBinaryOpaque(true);
		lbComponents[2]->funParam = 3;	
		lbComponents[2]->fun = fanOnOff;
	#endif

	lbComponents[3]->wEnPic = &FONT_NUM;
	lbComponents[3]->setBinaryOpaque(true);
	lbComponents[3]->fun = extruderExchange;
	if(active_extruder == 0){
		lbComponents[3]->setCBuffer(3,"E1");
		lbComponents[3]->updateTextIfChange = true;
	}
	else{
		lbComponents[3]->setCBuffer(3,"E2");
		lbComponents[3]->updateTextIfChange = true;
	}

	//左侧258
	topwinComponents[2]->fun = fanChange;//fan1-
	topwinComponents[2]->funParam = 1;
	#ifdef THROAT_FAN
	topwinComponents[5]->fun = fanChange;
	topwinComponents[5]->funParam = 2;
	#endif
	#ifdef USE_CONTROLLER_FAN
	topwinComponents[8]->fun = fanChange;
	topwinComponents[8]->funParam = 3;
	#endif
	//右侧369
	topwinComponents[3]->fun = fanChange;//fan1+
	topwinComponents[3]->funParam = 4;
	#ifdef THROAT_FAN
	topwinComponents[6]->fun = fanChange;
	topwinComponents[6]->funParam = 5;
	#endif
	#ifdef USE_CONTROLLER_FAN
	topwinComponents[9]->fun = fanChange;
	topwinComponents[9]->funParam = 6;
	#endif
}
#ifdef AUTO_BED_LEVELING_BILINEAR
void funZOffset(yWidget*w,WidgetEvent evt)
{
	float dis = 0;
	switch(evt){
	case wevt_away:if(w->funParam != distanceIndex)w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch(distanceIndex) {
		case 1:dis = 0.01;break;
		case 2:dis = 0.1;break;
		case 3:dis = 1.0;break;
		case 4:dis = 10.0;break;
		}
		switch(w->funParam) {
		case 1:
			destination.z += dis;
			planner.buffer_line(destination, feedrate_mm_s, active_extruder);
			break;
		case 2:
			destination.z -= dis;
			planner.buffer_line(destination, feedrate_mm_s, active_extruder);
			break;
		}
		break;
	default:break;
	}
}
void funSaveZOffset(yWidget*w,WidgetEvent evt)
{
	switch(evt){
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		if(queue.has_commands_queued() || planner.has_blocks_queued() || stepper.axis_is_moving(Z_AXIS)){
			QUICK_STOP();
			break;
		}
		else {
			if(fvalue != probe.offset.z){
				int index = INDEX_Z_OFFSET;
				watchdog_refresh();
				probe.offset.z = fvalue;
				const xyz_pos_t &zpo = probe.offset;
				AT24CXX_Write(index,&zpo,sizeof(zpo));//保存参数
			}
			else {
				probe.offset.z = fvalue;
			}
			current_position.z += 5;
			line_to_current_position();
		}
		currentScreen = screenTool;
		break;
	default:break;
	}
}
void screenZOffsetCenter(void)
{
	if(queue.has_commands_queued() || planner.has_blocks_queued() || stepper.axis_is_moving(Z_AXIS))return;
	currentScreen = nullptr;
	set_axis_is_at_home(Z_AXIS);
	planner.set_position_mm(current_position);
}
void screenGotoCenter(void)
{
	if(queue.has_commands_queued() || planner.has_blocks_queued() || stepper.axis_is_moving(Z_AXIS))return;//等待复位完成
	currentScreen = screenZOffsetCenter;
	current_position.x = X_CENTER;
	current_position.y = Y_CENTER;
	// current_position.z = current_position.z;//复位之后,Z轴的位置会被标记会Z_HOME_POS - probe.offset.z,所以为了不改变这个高度,就依此高度移动
	destination = current_position;
	line_to_current_position();
}
void screenZOffset(void)
{
	closeAllWin();
	if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_ZOFFSET_CN);
	else pic_page.value = MCU_PIC_ADDR(POS_ZOFFSET_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(410,256,62,62,nullptr,&picMain_focus));//back
	_NEW_W(topwinComponents[2],yWidget(68,259,80,45,nullptr,&picMain_focus));//0.01
	_NEW_W(topwinComponents[3],yWidget(148,259,80,45,nullptr,&picMain_focus));//0.1
	_NEW_W(topwinComponents[4],yWidget(228,259,77,45,nullptr,&picMain_focus));//1.0
	_NEW_W(topwinComponents[5],yWidget(305,259,79,45,nullptr,&picMain_focus));//10.0
	_NEW_W(topwinComponents[6],yWidget(90,28,80,80,nullptr,&picMain_focus));//Z+
	_NEW_W(topwinComponents[7],yWidget(92,158,82,80,nullptr,&picMain_focus));//Z-
	_NEW_W(topwinComponents[8],yLabel(240,98,128,88,nullptr,&picMain_focus));//set
	winAttr.componentNum = 9;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funSaveZOffset;
	topwinComponents[2]->funParam = 1;
	topwinComponents[2]->fun = distanceChoose;
	topwinComponents[3]->funParam = 2;
	topwinComponents[3]->fun = distanceChoose;
	topwinComponents[4]->funParam = 3;
	topwinComponents[4]->fun = distanceChoose;
	topwinComponents[5]->funParam = 4;
	topwinComponents[5]->fun = distanceChoose;
	distanceIndex = 3;
	topwinComponents[distanceIndex + 1]->setfocus(true);

	topwinComponents[6]->fun = funZOffset;//Z+
	#if 0
	topwinComponents[6]->funParam = 2;
	#else
	topwinComponents[6]->funParam = 1;
	#endif
	topwinComponents[7]->fun = funZOffset;//Z-
	#if 0
	topwinComponents[7]->funParam = 1;
	#else
	topwinComponents[7]->funParam = 2;
	#endif
	topwinComponents[8]->fun = funGeneralButton;//set
	topwinComponents[8]->funParam = BUTTON_CONFIRM_ZOFFSET;
	fvalue = probe.offset.z;
	probe.offset.z = 0;
	queue.enqueue_one_P("G28");
	currentScreen = screenGotoCenter;
}
#endif
void screenTool(void)
{
	closeAllWin();
	currentScreen = nullptr;
	if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_TOOL_CN);
	else pic_page.value = MCU_PIC_ADDR(POS_TOOL_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(368,178,100,120,nullptr,&picMain_focus));//返回
	_NEW_W(topwinComponents[2],yWidget(18,32,100,120,nullptr,&picMain_focus));//手动
	_NEW_W(topwinComponents[3],yWidget(132,32,100,120,nullptr,&picMain_focus));//预热
	_NEW_W(topwinComponents[4],yWidget(252,32,100,120,nullptr,&picMain_focus));//装材料
	_NEW_W(topwinComponents[5],yWidget(368,32,100,120,nullptr,&picMain_focus));//调平
	_NEW_W(topwinComponents[6],yWidget(18,178,100,120,nullptr,&picMain_focus));//风扇
	_NEW_W(topwinComponents[7],yWidget(132,178,100,120,nullptr,&picMain_focus));//紧急停止
	_NEW_W(topwinComponents[8],yWidget(252,178,100,120,nullptr,&picMain_focus));//Z偏移
	winAttr.componentNum = 9;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenMain;
	topwinComponents[2]->fun = funGeneral;
	topwinComponents[2]->funNext = screenManual;
	topwinComponents[3]->fun = funGeneral;
	topwinComponents[3]->funNext = screenPreheat;
	topwinComponents[4]->fun = funGeneral;
	#if ENABLED(FILAMENT_RUNOUT_SENSOR)
	topwinComponents[4]->funNext = screenFilament;
	#endif
	topwinComponents[5]->fun = funGeneral;
	topwinComponents[5]->funNext = screenLevelPopConfirm;
	topwinComponents[6]->fun = funGeneral;
	topwinComponents[6]->funNext = screenFan;
	topwinComponents[7]->fun = funGeneral;
	topwinComponents[7]->funNext = screenEmergency;
	#ifdef AUTO_BED_LEVELING_BILINEAR
	topwinComponents[8]->fun = funGeneral;
	topwinComponents[8]->funNext = screenZOffset;
	#else
	topwinComponents[8]->fun = funPopScreen;
	topwinComponents[8]->funParam = POP_NOSUPPORT;
	#endif
}

//SECTOR3:****************************打印****************************
void funPrint(yWidget*w,WidgetEvent evt)
{
	switch(evt){
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch(w->funParam){
		case 1:
			if(wait_for_heatup){ //如果遇到暂停后恢复打印,正在加热中,则直接弹出加热等待界面,而不继续暂停
				screenPopIndex(POP_WAITNOZZLE_HEAT);
				break;
			}
			if(my_print_status == PRINT_RUNNING) {
				my_print_status = PRINT_PAUSING;
				queue.inject_P(PSTR("M125 P1"));
			}
			else if(my_print_status == PRINT_PAUSE){
				my_print_status = PRINT_RESUME;
				wait_for_user = false;
			}
			currentScreen = w->funNext;
			break;
		case 2:
			screenPopIndex(POP_STOP_PRINT);
			break;
		}
		break;
	default:break;
	}
}
yVariable *lvPrinting[2];
int progress = 0;
void screenUpdatePrinting(void)
{
	uint32_t t = millis();
	int16_t temp;
	int h,m,i;
	if(t >= last) {
		last = t + 500;

		temp = thermalManager.degBed();
		sprintf(lbComponents[0]->char_tar,"%-3d/%-3d",temp,thermalManager.degTargetBed());
		lbComponents[0]->sign.content.text = 1;
		temp = thermalManager.degHotend(0);
		sprintf(lbComponents[1]->char_tar,"%-3d/%-3d",temp,thermalManager.degTargetHotend(0));
		lbComponents[1]->sign.content.text = 1;
		#if HOTENDS > 1
		temp = thermalManager.degHotend(1);
		sprintf(lbComponents[2]->char_tar,"%-3d/%-3d",temp,thermalManager.degTargetHotend(1));
		lbComponents[2]->sign.content.text = 1;
		#endif

		t = print_job_timer.duration();
		h = t / 3600;
		m = (t % 3600) / 60;
		char *str = lbComponents[3]->char_tar;
		lbComponents[3]->sign.content.text = 1;
		sprintf(str,"%d:%d",h,m);
		for(i = 0; str[i]; i++);
		for(;i < 6;i++)str[i] = ' ';
		str[i] = 0;
		
		str = lbComponents[4]->char_tar;
		lbComponents[4]->sign.content.text = 1;
		if(printTotalTime > t)t = printTotalTime - t;
		else t = 0;
		h = t / 3600;
		m = (t % 3600) / 60;
		sprintf(str,"%d:%d",h,m);
		for(i = 0;str[i];i++);
		for(;i < 6;i++)str[i] = ' ';
		str[i] = 0;
		
		str = lbComponents[5]->char_tar;
		lbComponents[5]->sign.content.text = 1;
		
		if(stepper.current_block){
			h = stepper.current_block->nominal_rate;
			h = h * 1.0f /planner.settings.axis_steps_per_mm[X_AXIS] + 0.5;
		}
		else h = 0;
		sprintf(str,"%-3d",h);
		
		if(marlin_state == MF_SD_COMPLETE || my_print_status == PRINT_IDLE){
			h = 100;
			printTotalTime = print_job_timer.duration();
			screenPopIndex(POP_COMPLETE_PRINT);
		}
		else h = card.percentDone();
		if(progress != h && h <= 100) {
			char tempstr[8];
			sprintf(tempstr,"%3d%%",h);
			lvPrinting[0]->setChars(tempstr);
			lvPrinting[1]->setChars(tempstr);
			m = h - progress;
			m <<= 2;
			lvPrinting[0]->changeBorder(m);
			lvPrinting[1]->changeBorder(m);
			progress = h;
		}
	}
}
void screenPrintTool(void);
void funKey(yWidget*w,WidgetEvent evt)
{
	int i,t;
	switch(evt){
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch(w->funParam){
		case 2://确认
			t = atoi(lbComponents[0]->char_tar);
			switch(lbComponents[0]->funParam){
			case 1://速率
				if(t < 10)t = 10;
				else if(t > 999)t = 999;
				feedrate_percentage = t;
				break;
			case 2://热床温度
				if(t < 0)t = 0;
				else if(t > BED_MAXTEMP)t = BED_MAXTEMP;
				thermalManager.setTargetBed(t);
				break;
			case 3://喷头1温度
				if(t < 0)t = 0;
				else if(t > Temperature::heater_maxtemp[0])t = Temperature::heater_maxtemp[0];
				thermalManager.setTargetHotend(t,0);
				break;
			#if HOTENDS > 1
			case 4://喷头2温度
				if(t < 0)t = 0;
				else if(t > Temperature::heater_maxtemp[1])t = Temperature::heater_maxtemp[1];
				thermalManager.setTargetHotend(t,1);
				break;
			#endif
			case 5://冷却风扇
				if(t < 0)t = 0;
				else if(t > 100)t = 100;
				t = t * 255 / 100;
				thermalManager.set_fan_speed(0,t);
				#if HOTENDS > 1 && HAS_FAN1
				thermalManager.set_fan_speed(1,t);
				#endif
				break;
			#if THROAT_FAN
			case 6://喉管风扇
				if(t < 0)t = 0;
				else if(t > 100)t = 100;
				t = t * 255 / 100;
				thermalManager.set_fan_speed(THROAT_FAN,t);
				#if HOTENDS > 1
				thermalManager.set_fan_speed(THROAT_FAN + 1,t);
				#endif
				break;
			#endif
			case 7://挤丝流量
				if(t < 10)t = 10;
				else if(t > 999)t = 999;
				planner.flow_percentage[active_extruder] = t;
				break;
			default:break;
			}
		case 1://返回
			currentScreen = screenPrintTool;
			break;
		case 3://退格
			if(lbComponents[0]->char_tar[9] == '0' && lbComponents[0]->char_tar[8] == ' ')break;
			for(i = 9;lbComponents[0]->char_tar[i - 1] != ' ' && i > 0;i--)lbComponents[0]->char_tar[i] = lbComponents[0]->char_tar[i - 1];
			if(i == 9)lbComponents[0]->char_tar[i] = '0';
			else lbComponents[0]->char_tar[i] = ' ';
			lbComponents[0]->sign.content.text = true;
			break;
		case 4://.
			if(lbComponents[0]->char_tar[0] != ' ')break;
			for(i = 0;lbComponents[0]->char_tar[i + 1] == ' '; i++);//找到下一个不是空格的起点
			for(t = i + 1;t < 10;t++)if(lbComponents[0]->char_tar[t] == '.')break;//不能有两个以上的.
			if(t < 10)break;
			for(; i < 9; i++)lbComponents[0]->char_tar[i] = lbComponents[0]->char_tar[i + 1];
			lbComponents[0]->char_tar[9] = '.';
			lbComponents[0]->sign.content.text = true;
			break;
		case 5://-
			if(lbComponents[0]->char_tar[0] != ' ')break;
			for(i = 0;lbComponents[0]->char_tar[i + 1] == ' '; i++);//找到下一个不是空格的起点
			if(lbComponents[0]->char_tar[i + 1] == '-')lbComponents[0]->char_tar[i + 1] = ' ';
			else lbComponents[0]->char_tar[i] = '-';
			lbComponents[0]->sign.content.text = true;
			break;
		default://0--9
			if(lbComponents[0]->char_tar[0] != ' ' || w->funParam > 15)break;
			if(!(lbComponents[0]->char_tar[9] == '0' && lbComponents[0]->char_tar[8] == ' ')){
				for(i = 0;lbComponents[0]->char_tar[i + 1] == ' '; i++);//找到下一个不是空格的起点
				for(; i < 9; i++)lbComponents[0]->char_tar[i] = lbComponents[0]->char_tar[i + 1];
			}
			lbComponents[0]->char_tar[9] = w->funParam - 6 + '0';
			lbComponents[0]->sign.content.text = true;
			break;
		}
	default:break;
	}
}
void screenKeyboard(void)
{
	currentScreen = nullptr;
	last = 0;//返回后,立即刷新值
	// BYTE index = currentWidget->funParam;
	yLabel *lb = (yLabel*)currentWidget;
	pic_page.value = MCU_PIC_ADDR(POS_KEYBOARD);
	_NEW_W(lbComponents[0],yLabel(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	char str[16];
	sprintf(str,"%10s",lb->char_tar);
	lbComponents[0]->wEnPic = &FONT_NUM;
	lbComponents[0]->setCBuffer(15,str);//量
	lbComponents[0]->funParam = lb->funParam;
	closeAllWin();
	topwinComponents[0] = lbComponents[0];
	lbComponents[0]->setWPos(324,42);
	_NEW_W(topwinComponents[1],yWidget(338,241,96,64,nullptr,&picMain_focus));//取消,返回
	_NEW_W(topwinComponents[2],yWidget(338,170,96,64,nullptr,&picMain_focus));//确认
	topwinComponents[2]->fun = funKey;
	topwinComponents[2]->funParam = 2;
	_NEW_W(topwinComponents[3],yWidget(338,96,96,64,nullptr,&picMain_focus));//退格
	topwinComponents[3]->fun = funKey;
	topwinComponents[3]->funParam = 3;
	_NEW_W(topwinComponents[4],yWidget(146,256,93,50,nullptr,&picMain_focus));//.
	_NEW_W(topwinComponents[5],yWidget(241,256,93,50,nullptr,&picMain_focus));//-
	_NEW_W(topwinComponents[6],yWidget(49,256,93,50,nullptr,&picMain_focus));//0
	_NEW_W(topwinComponents[7],yWidget(49,96,93,50,nullptr,&picMain_focus));//1
	_NEW_W(topwinComponents[8],yWidget(146,96,93,50,nullptr,&picMain_focus));//2
	_NEW_W(topwinComponents[9],yWidget(241,96,93,50,nullptr,&picMain_focus));//3
	_NEW_W(topwinComponents[10],yWidget(49,149,93,50,nullptr,&picMain_focus));//4
	_NEW_W(topwinComponents[11],yWidget(146,149,93,50,nullptr,&picMain_focus));//5
	_NEW_W(topwinComponents[12],yWidget(241,149,93,50,nullptr,&picMain_focus));//6
	_NEW_W(topwinComponents[13],yWidget(49,202,93,50,nullptr,&picMain_focus));//7
	_NEW_W(topwinComponents[14],yWidget(146,202,93,50,nullptr,&picMain_focus));//8
	_NEW_W(topwinComponents[15],yWidget(241,202,93,50,nullptr,&picMain_focus));//9
	winAttr.componentNum = 16;
	for(int i = 4; i < 16; i++) {
		topwinComponents[i]->fun = funKey;
		topwinComponents[i]->funParam = i;
	}
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	lbComponents[0]->setBinaryOpaque(true);
	lbComponents[0]->setBinaryFrColor(COLOR_BLACK,0);
	lbComponents[0]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenPrintTool;
}
void screenUpdateTool(void)
{
	int t;
	uint32_t now = millis();
	if(now >= last) {
		last = now + 500;
		t = feedrate_percentage;
		sprintf(lbComponents[0]->char_tar,"%3d",t);
		lbComponents[0]->sign.content.text = 1;
		t = thermalManager.temp_bed.target;
		sprintf(lbComponents[1]->char_tar,"%3d",t);
		lbComponents[1]->sign.content.text = 1;
		t = thermalManager.temp_hotend[0].target;
		sprintf(lbComponents[2]->char_tar,"%3d",t);
		lbComponents[2]->sign.content.text = 1;
		#if HOTENDS > 1
		t = thermalManager.temp_hotend[1].target;
		sprintf(lbComponents[3]->char_tar,"%3d",t);
		lbComponents[3]->sign.content.text = 1;
		#else
		lbComponents[3]->setChars("  -");
		#endif
		t = thermalManager.fan_speed[0] * (int)100 / 255;
		sprintf(lbComponents[4]->char_tar,"%3d",t);
		lbComponents[4]->sign.content.text = 1;

		#if THROAT_FAN
		t = thermalManager.fan_speed[THROAT_FAN] * (int)100 / 255;
		sprintf(lbComponents[5]->char_tar,"%3d",t);
		lbComponents[5]->sign.content.text = 1;
		#endif

		t = planner.flow_percentage[active_extruder];
		sprintf(lbComponents[6]->char_tar,"%3d",t);
		lbComponents[6]->sign.content.text = 1;

		#if ENABLED(USE_CONTROLLER_FAN)
		if(controllerFan.state()){
			if(lbComponents[7]->char_tar[1] != 'N')lbComponents[6]->setChars("ON ");
		}
		else {
			if(lbComponents[7]->char_tar[1] != 'F')lbComponents[6]->setChars("OFF");
		}
		#endif
	}
}
void screenPrintTool(void)
{
	void screenPrinting(void);
	closeAllWin();
	last = 0;
	currentScreen = screenUpdateTool;
	pic_page.value = MCU_PIC_ADDR(POS_PRINT_TOOL_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(400,250,60,64,nullptr,&picMain_focus));
	_NEW_W(lbComponents[0],yLabel(120,28,64,40,nullptr,&picMain_focus));//速度控制率
	lbComponents[0]->setCBuffer(3);
	topwinComponents[2] = lbComponents[0];
	_NEW_W(lbComponents[1],yLabel(120,90,64,40,nullptr,&picMain_focus));//热床温度
	lbComponents[1]->setCBuffer(3);
	topwinComponents[3] = lbComponents[1];
	_NEW_W(lbComponents[2],yLabel(120,148,64,40,nullptr,&picMain_focus));//喷头1温度
	lbComponents[2]->setCBuffer(3);
	topwinComponents[4] = lbComponents[2];
	_NEW_W(lbComponents[3],yLabel(120,210,64,40,nullptr,&picMain_focus));//喷头2温度
	lbComponents[3]->setCBuffer(3);
	topwinComponents[5] = lbComponents[3];
	_NEW_W(lbComponents[4],yLabel(388,28,64,40,nullptr,&picMain_focus));//冷却风扇
	lbComponents[4]->setCBuffer(3);
	topwinComponents[6] = lbComponents[4];
	_NEW_W(lbComponents[5],yLabel(388,90,64,40,nullptr,&picMain_focus));//喉管风扇
	lbComponents[5]->setCBuffer(3);
	topwinComponents[7] = lbComponents[5];
	_NEW_W(lbComponents[7],yLabel(388,148,64,40,nullptr,&picMain_focus));//主板风扇
	lbComponents[7]->setCBuffer(3);
	lbComponents[7]->fun = fanOnOff;
	lbComponents[7]->funParam = 3;
	topwinComponents[8] = lbComponents[7];
	_NEW_W(lbComponents[6],yLabel(324,210,64,40,nullptr,&picMain_focus));//挤丝流量
	lbComponents[6]->setCBuffer(3);
	topwinComponents[9] = lbComponents[6];
	winAttr.componentNum = 10;
	_NEW_W(topwin,yWindows(&winAttr,topwin));
	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenPrinting;
	int t;
	for(t = 0;t < 7;t++) {
		lbComponents[t]->setWPos(4,20 - FONT_NUM.yPixel / 2);
		lbComponents[t]->wEnPic = &FONT_NUM;
		lbComponents[t]->setBinaryOpaque(true);
		lbComponents[t]->funParam = t + 1;
		lbComponents[t]->fun = funGeneral;
		lbComponents[t]->funNext = screenKeyboard;
	}
	#if HOTENDS < 2
	lbComponents[3]->fun = nullptr;
	#endif
}
void screenPrinting(void) {
	closeAllWin();
	currentScreen = screenUpdatePrinting;
	yLabel *tlb;
	if(my_print_status == PRINT_RUNNING || my_print_status == PRINT_RESUME)pic_page.value = MCU_PIC_ADDR(POS_PRINTING);
	else pic_page.value = MCU_PIC_ADDR(POS_PRINTING_RESUME);
	_NEW_W(tlb,yLabel(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));//文件名
	tlb->setWPos(216,128);
	tlb->setWBuffer(0,PrintJobRecovery::info.sd_longname);
	topwinComponents[0] = tlb;

	_NEW_W(topwinComponents[1],yWidget(35,214,136,96,nullptr,&picMain_focus));//暂停
	_NEW_W(topwinComponents[2],yWidget(173,214,134,96,nullptr,&picMain_focus));//停止
	_NEW_W(topwinComponents[3],yWidget(309,214,136,96,nullptr,&picMain_focus));//设置工具
	_NEW_W(lbComponents[0],yLabel(216,24,64,32,nullptr,nullptr));//热床温度
	_NEW_W(lbComponents[1],yLabel(305,24,64,32,nullptr,nullptr));//喷头1温度
	_NEW_W(lbComponents[2],yLabel(402,24,64,32,nullptr,nullptr));//喷头2温度
	_NEW_W(lbComponents[3],yLabel(216,76,64,32,nullptr,nullptr));//已打印时间
	_NEW_W(lbComponents[4],yLabel(305,76,64,32,nullptr,nullptr));//未打印时间
	_NEW_W(lbComponents[5],yLabel(402,76,64,32,nullptr,nullptr));//速度
	for(int i = 0; i < 6; i++) {
		topwinComponents[4 + i] = lbComponents[i];
		lbComponents[i]->setCBuffer(11);
		lbComponents[i]->setBinaryOpaque(true);
		lbComponents[i]->setBinaryFrColor(COLOR_BLACK,0);
		lbComponents[i]->setBinaryBgColor(COLOR_WHITE,0);
	}
	#if HOTENDS < 2
	lbComponents[2]->setChars("---");
	#endif
	_NEW_W(lvPrinting[0],yVariable(40,172,0,32,&pic_title,nullptr));//进度
	lvPrinting[0]->setPicColor(COLOR_BLUE,COLOR_GREEN);
	lvPrinting[0]->setBinaryFrColor(COLOR_YELLOW,0);
	lvPrinting[0]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
	lvPrinting[0]->setWPos(206 - 2 * FONT_NUM.xPixel,16 - FONT_NUM.yPixel / 2);
	topwinComponents[10] = lvPrinting[0];
	_NEW_W(lvPrinting[1],yVariable(40,172,400,32,&pic_title,nullptr));//余度
	lvPrinting[1]->setPicColor(COLOR_BLACK,COLOR_BLACK);
	lvPrinting[1]->setBinaryFrColor(COLOR_YELLOW,0);
	lvPrinting[1]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
	lvPrinting[1]->setWPos(206 - 2 * FONT_NUM.xPixel,16 - FONT_NUM.yPixel / 2);
	topwinComponents[11] = lvPrinting[1];
	winAttr.componentNum = 12;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	lvPrinting[0]->fixedEdge = false;//左边固定
	lvPrinting[0]->setCBuffer(7);
	lvPrinting[0]->wEnPic = &FONT_NUM;
	lvPrinting[1]->fixedEdge = true;//右边固定
	lvPrinting[1]->setCBuffer(7,"  0%");
	lvPrinting[1]->wEnPic = &FONT_NUM;
	progress = 0;

	topwinComponents[1]->fun = funPrint;
	topwinComponents[1]->funParam = 1;
	topwinComponents[1]->funNext = screenPrinting;
	topwinComponents[2]->fun = funPrint;
	topwinComponents[2]->funParam = 2;
	topwinComponents[3]->fun = funGeneral;
	topwinComponents[3]->funNext = screenPrintTool;
}
static void screenOpenFile(void)
{
	// char cmd[MAX_CMD_SIZE + 16];
	sprintf_P(cmd, M23_STR, PrintJobRecovery::info.sd_filename);
	gcode.process_subcommands_now(cmd);
	if(card.isFileOpen() && messageIndexNext == 0) {
		gcode.process_subcommands_now_P("T0 S");
		runout.reset();
		gcode.process_subcommands_now_P("M24");
		last = 0;
		currentScreen = screenPrinting;
		lastScreen = nullptr;
	}
}
static void funSdfile(yWidget*w,WidgetEvent evt)
{
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		switch(w->funParam){
		case 1://删除
			screenPopIndex(POP_DELETE_FILE);
			break;
		case 2://开始打印一个文件
			{
				yLabel *lb = (yLabel*)topwinComponents[0];
				//保存长文件名
				int i = 0;
				for(; i < LONG_FILENAME_LENGTH - 1 && lb->word_tar[i]; i++)PrintJobRecovery::info.sd_longname[i] = lb->word_tar[i];
				PrintJobRecovery::info.sd_longname[i] = 0;
				//保存短文件名(在M24里面,会再次保存一次)
				for(i = 0; i < FILENAME_LENGTH - 1 && lb->char_tar[i]; i++)PrintJobRecovery::info.sd_filename[i] = lb->char_tar[i];
				PrintJobRecovery::info.sd_filename[i] = 0;
				currentScreen = screenOpenFile;
			}
			break;
		}
		break;
	default:break;
	}
}
void screenFileManu(void)
{
	yLabel *lb;
	pic_page.value = MCU_PIC_ADDR(POS_FILE_MANU);
	_NEW_W(lb,yLabel(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	lb->setBinaryFrColor(WORD_COLOR_IDLE,0);
	lb->setBinaryBgColor(BASIC_BG_COLOR,0);
	lb->setWPos(16,16);
	lb->setCBuffer(FILENAME_LENGTH,lbComponents[currentWidget->funParam]->char_tar);
	lb->setWBuffer(LONG_FILENAME_LENGTH,lbComponents[currentWidget->funParam]->word_tar);//这里在后面,可以更新u16_mode至1
	closeAllWin();//注意,得先把参数保存在新控件中,才能关闭
	currentScreen = nullptr;
	topwinComponents[0] = lb;
	_NEW_W(topwinComponents[1],yWidget(364,226,102,85,nullptr,&picMain_focus));//返回
	_NEW_W(topwinComponents[2],yWidget(364,42,102,90,nullptr,&picMain_focus));//删除
	_NEW_W(topwinComponents[3],yWidget(364,136,102,87,nullptr,&picMain_focus));//打印
	winAttr.componentNum = 4;
	_NEW_W(topwin,yWindows(&winAttr,topwin));
	
	topwinComponents[1]->fun = funGeneral;
	topwinComponents[1]->funNext = screenFileChoose;
	topwinComponents[2]->fun = funSdfile;
	topwinComponents[2]->funParam = 1;
	topwinComponents[3]->fun = funSdfile;
	topwinComponents[3]->funParam = 2;//之所以写4,是因为要对应弹窗编号,以提示信息
}
void funDir(yWidget*w,WidgetEvent evt){
	yLabel *lb = (yLabel*)w;
	switch(evt)
	{
	case wevt_away:w->setfocus(false);break;
	case wevt_touch:w->setfocus(true);break;
	case wevt_untouch:
		w->setfocus(false);
		if(w->funParam == 0xFF){
			#if defined(SDSUPPORT)
			if(card.flag.workDirIsRoot)currentScreen = screenMain;
			else {
				card.cdup();
				currentScreen = w->funNext;
			}
			#endif
		}
		else {
			#if defined(SDSUPPORT)
			card.cd(lb->char_tar);
			#endif
			currentScreen = w->funNext;
		}
		currentWidget = w;
		break;
	default:break;
	}
}
void updateFileList()
{
	// const WORD tbuf[2] = {' ',0};
	int i = 0;
	if(card.isMounted()) {
		uint16_t curline = topline;
		for(; i < 4 && curline < fileCnt; i++)
		{
			card.getfilename_sorted(SD_ORDER(curline, fileCnt));//先显示最新的文件
			lbComponents[i]->setCBuffer(0,card.filename);
			lbComponents[i]->setWBuffer(0,card.longest_filename());//这里在后,置u16_mode为1
			if (card.flag.filenameIsDir) {
				lbComponents[i]->fun = funDir;
				lbComponents[i]->wgPic = &picIco_dir;
				lbComponents[i]->wgPic_idle = &picIco_dir;
				lbComponents[i]->funNext = screenFileChoose;
			}
			else {
				lbComponents[i]->fun = funGeneral;
				lbComponents[i]->funNext = screenFileManu;
				lbComponents[i]->wgPic = &picIco_file;
				lbComponents[i]->wgPic_idle = &picIco_file;
			}
			lbComponents[i]->sign.content.widget = 1;
			curline++;
		}
	}
	for(;i < 4; i++) {
		lbComponents[i]->wgPic = nullptr;
		lbComponents[i]->setCBuffer(0);
		lbComponents[i]->setWBuffer(0);
		lbComponents[i]->fun = nullptr;
		lbComponents[i]->sign.content.widget = 1;
	}
}
void screenFileChoose(void)
{
	closeAllWin();
	currentScreen = nullptr;
	pic_page.value = MCU_PIC_ADDR(POS_FILE_CHOOSE);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(364,252,100,66,nullptr,&picMain_focus));//返回
	_NEW_W(lbComponents[0],yLabel(28, 30,292,56,nullptr,&picMain_focus)); //文件列表-文件1
	topwinComponents[2] = lbComponents[0];
	_NEW_W(lbComponents[1],yLabel(28,100,292,56,nullptr,&picMain_focus)); //文件列表-文件2
	topwinComponents[3] = lbComponents[1];
	_NEW_W(lbComponents[2],yLabel(28,166,292,56,nullptr,&picMain_focus)); //文件列表-文件3
	topwinComponents[4] = lbComponents[2];
	_NEW_W(lbComponents[3],yLabel(28,230,292,56,nullptr,&picMain_focus)); //文件列表-文件4
	topwinComponents[5] = lbComponents[3];
	_NEW_W(topwinComponents[6],yWidget(364,12,106,113,nullptr,&picMain_focus));//上
	_NEW_W(topwinComponents[7],yWidget(364,126,106,114,nullptr,&picMain_focus));//下
	winAttr.componentNum = 8;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->fun = funDir;//返回
	topwinComponents[1]->funParam = 0xFF;
	topwinComponents[1]->funNext = screenFileChoose;
	topwinComponents[6]->fun = funGeneralButton;
	topwinComponents[6]->funParam = BUTTON_FILE_UP;
	topwinComponents[7]->fun = funGeneralButton;
	topwinComponents[7]->funParam = BUTTON_FILE_DOWN;
	for(int i = 0; i < 4; i++) {
		lbComponents[i]->_fillMargin = true;
		lbComponents[i]->setBinaryFrColor(COLOR_BLUE,0);
		lbComponents[i]->setBinaryBgColor(COLOR_LIGHTBLUE,0);
		lbComponents[i]->setBinaryOpaque(true);
		lbComponents[i]->setWPos(52,28 - FONT_NUM.yPixel / 2);
		lbComponents[i]->funParam = i;
	}
	#if defined(SDSUPPORT)
	if(card.isMounted())fileCnt = card.get_num_Files();
	else fileCnt = 0;
	#endif
	topline = 0;
	updateFileList();
}
void screenToFileChoose(void){
	watchdog_refresh();
	topline = 0;
	card.release();//由于没有检测SD卡插入的PIN,所以,每次进入文件界面,预防SD卡拔出过,都先释放,重新mount
	card.mount();
	screenFileChoose();
}

//SECTOR4:****************************主页****************************
// __attribute__((section(".bss")))
void screenMain(void)
{
	closeAllWin();
	currentScreen = lastScreen;
	lastScreen = nullptr;
	if(my_lang == LANG_CHINESE)pic_page.value = MCU_PIC_ADDR(POS_MAIN_CN);
	else pic_page.value = MCU_PIC_ADDR(POS_MAIN_EN);
	_NEW_W(topwinComponents[0],yWidget(0,0,LCD_LMAX,LCD_WMAX,&pic_page,nullptr,false));
	_NEW_W(topwinComponents[1],yWidget(36,90,128,148,nullptr,&picMain_focus));
	_NEW_W(topwinComponents[2],yWidget(176,90,128,148,nullptr,&picMain_focus));
	_NEW_W(topwinComponents[3],yWidget(316,90,128,148,nullptr,&picMain_focus));

	winAttr.componentNum = 4;
	_NEW_W(topwin,yWindows(&winAttr,topwin));

	topwinComponents[1]->funNext = screenSystem;
	topwinComponents[1]->fun = funGeneral;
	topwinComponents[2]->fun = funGeneral;
	topwinComponents[2]->funNext = screenTool;
	topwinComponents[3]->fun = funGeneral;
	topwinComponents[3]->funNext = screenToFileChoose;
}
bool updateFromSdcardStart(void) {
	int i;
    SdFile dir, root = card.getroot();
	if (dir.open(&root, "Tronxy", O_RDONLY))my_print_status = PRINT_START_UPDATE1;
	else if (dir.open(&root, "Update", O_RDONLY))my_print_status = PRINT_START_UPDATE2;
	else return false;
	bool ifexist = false;
	dir_t d;
	char str[32];
	while (dir.readDir(&d, card.longFilename) > 0) {
		watchdog_refresh();
		// If we dont get a long name, but gets a short one, try it
		if (card.longFilename[0] == 0){
			strcpy(str,(const char*)d.name);
		}
		else {
			for(i = 0;card.longFilename[i] && i < 30;i++)str[i] = card.longFilename[i];
			str[i] = 0;
		}
		if((str[0] == 'c' || str[0] == 'C') &&
		(str[1] == 'f' || str[1] == 'F') &&
		(str[2] == 'g' || str[2] == 'G') &&
		str[3] == '_'){
			createFilename(card.filename,d);//在cardread.h中声明,cardreader.cpp中定义
			if(my_print_status == PRINT_START_UPDATE2)card.cd("Update");
			else card.cd("Tronxy");
			card.openFileRead(card.filename);
			if(card.isFileOpen()){
				card.startFileprint();            // SD card will now be read for commands
				startOrResumeJob();               // Start (or resume) the print job timer
				ifexist = true;
			}
			break;
		}
	}
	if(!ifexist)my_print_status = PRINT_IDLE;
	dir.close();
	return ifexist;
}
void updateFromSdcardEnd(void){
	if(my_print_status == PRINT_UPDATE_END){
		my_print_status = PRINT_IDLE;
		card.cdup();
		if(my_print_status == PRINT_START_UPDATE2) {
			SdFile dir, root = card.getroot();
			if(dir.open(&root, "Update", O_RDONLY)){
				dir.rename(&root, "_Update");
				dir.close();
			}
		}
		TERN(POWER_LOSS_RECOVERY, //YSZ-COMMENT:开机后检测掉电文件
          PrintJobRecovery::check(),           // Check for PLR file. (If not there it will beginautostart)
          card.beginautostart()            // Look for auto0.g on the next loop
        );
	}
}
void showLog(void) {
	const char* ver = (const char*)POS_VERSION;
	int len;
	char str[128];
	if(ver[0] && ver[0] != 0xFF && ver[1] && ver[1] != 0xFF)sprintf(str,"Boot:%s Firmware:%s",ver,TRONXY_MACHINE_NAME "-" TRONXY_SOFT_VERSION);
	else sprintf(str,"Boot:unknown Firmware:%s",TRONXY_MACHINE_NAME "-" TRONXY_SOFT_VERSION);
	yLabel *wLog;
	_NEW_W(wLog,yLabel(0,0,LCD_LMAX,LCD_WMAX,&pic_log,nullptr,false));
	topwinComponents[0] = wLog;
	wLog->setCBuffer(0,str);
	len = yString::len(str);
	wLog->setBinaryFrColor(WORD_COLOR_IDLE,BASIC_BG_COLOR);
	wLog->setBinaryOpaque(true);
	wLog->setWgpOffset(((LCD_LMAX - pic_log.xPixel) >> 1),((LCD_WMAX - pic_log.yPixel) >> 1));
	len *= wLog->wEnPic->xPixel;
	wLog->setWPos(0,LCD_WMAX - DEFAULT_FONT_YPIXEL);
	logTime = millis() + BOOTSCREEN_TIMEOUT;

	winAttr.componentNum = 1;
	_NEW_W(topwin,yWindows(&winAttr,topwin));
	topwin->update();
}
void logScreen(void)
{
	updateFromSdcardEnd();
	if(millis() >= logTime)turntoMainscreen();
	if(millis() > 100)if(READ(POWER_LOSS_PIN) == HIGH){
		#if T2_STM32F103
		nvic_sys_reset();
		#elif T2_STM32F446
		NVIC_SystemReset();
		#else
		while(1){}
		#endif
	}
	int r = topwin->continueTouchTimes();
	if(r > 2) {
		currentScreen = screenAdjust;
		topwin->touchReset();
	}
}
void screenKillError(const char*errorInfo) //注意,kill会在中断中被调用(温度的定时器中断就调用过)
{
	error_info = errorInfo;
	if (my_print_status == PRINT_RUNNING //打印中出错
		|| my_print_status == PRINT_PAUSE //暂停后出错
		|| my_print_status == PRINT_RUNOUT //断料时出错
		){
		if(my_print_status == PRINT_RUNNING && PrintJobRecovery::enabled){
			// cli();
			PrintJobRecovery::save(true,current_position.z);
			// sei();
		}
		card.endFilePrint(TERN_(SD_RESORT, true));
		print_job_timer.stop();
	}
	else ;//未打印时出错
	queue.clear();
	globalStop = 2;
	buzzer.tone(300,1200);
	buzzer.tick();
	wait_for_heatup = false;
	wait_for_user = false;
	screenPopIndex(POP_KILL_ERROR,quickstop_stepper);
}
//SECTOR5:*************************断电续打*************************
extern "C" {
	#if T2_STM32F103
	void __irq_exti2(void)
	{
		EXTI_BASE->PR = 1 << 2;//清除line2上的中断标志
	#elif T2_STM32F446
	//C:\Users\Tronxy_3\.platformio\packages\framework-arduinoststm32\libraries\SrcWrapper\src\stm32\interrupt.cpp
	//需要在上面文件中注释掉EXTI1_IRQHandler
	void EXTI1_IRQHandler(void)
	{
		EXTI->PR = 1 << 1;//清除line1上的中断标志
	#else
	#error You must appoint one tip board.
	#endif
		if(millis() < 5000)return;
		DISABLE_TEMPERATURE_INTERRUPT();
		WRITE_HEATER_BED(false);//必须把加热部件关掉,否则电量会被快速耗尽
		WRITE_HEATER_0(false);
		#if HOTENDS > 1
			WRITE_HEATER_1(false);
			#if HOTENDS > 2
				WRITE_HEATER_2(false);
			#endif
		#endif
		#ifdef USE_CONTROLLER_FAN
		bool controlFan = READ(CONTROLLER_FAN_PIN);
		WRITE(CONTROLLER_FAN_PIN, 0);
		#endif
		#if FAN_COUNT > 0
		bool fan0 = READ(FAN0_PIN);
		WRITE_FAN(0,0);
		#endif
		#if FAN_COUNT > 1 && HOTENDS > 1
		bool fan1 = READ(FAN1_PIN);
		WRITE_FAN(1,0);
		#endif
		for(uint32_t t = 0; READ(POWER_LOSS_PIN) == HIGH; t++){
			#if T2_STM32F103
			if(t > 552){ //消抖0.1ms,1000000次约181ms
			#elif T2_STM32F446
			if(t > 1449){ //消抖0.1ms,1000000次约69ms
			#else
			{
			#endif
				if(PrintJobRecovery::enabled &&
				(my_print_status == PRINT_RUNNING //打印中掉电
				// || my_print_status == PRINT_PAUSE //暂停后掉电,本身M125已经保存,这里不重复
				// || my_print_status == PRINT_RUNOUT //断料时掉电
				)){ 
					disable_all_steppers();
					PrintJobRecovery::save(true,current_position.z);
				}
				globalStop = 100;
				buzzer.tone(300,1200);
				buzzer.tick();
				break;
			}
		}
		#ifdef USE_CONTROLLER_FAN
		if(controlFan)WRITE(CONTROLLER_FAN_PIN, 255);
		#endif
		#if FAN_COUNT > 0
		if(fan0)WRITE_FAN(0,1);
		#endif
		#if FAN_COUNT > 1 && HOTENDS > 1
		if(fan1)WRITE_FAN(1,1);
		#endif
		ENABLE_TEMPERATURE_INTERRUPT();
	}
}
void powerLossInit(void)
{
	OUT_WRITE(POWER_LM393_PIN,HIGH);
	SET_INPUT_PULLUP(POWER_LOSS_PIN);

	#if T2_STM32F103

	//外中断配置
	RCC_BASE->APB2ENR |= 0x01;//使能AFIO时钟
	AFIO_BASE->EXTICR1 &= ~(0x000F << 8);//每条线占4位,PG2是2线,所以是第2 * 4 = 8位开始
	AFIO_BASE->EXTICR1 |= (0x0006 << 8); //将EXTI.bit映射到GPIO.bit,其中从PA为0开始,PB为1...PG为6

	EXTI_BASE->IMR |= (1 << 2);//开启line2上的中断
	EXTI_BASE->EMR |= (1 << 2);//屏蔽line2上的事件
	// EXTI_BASE->FTSR |= (1 << 2);//开启line2下降沿触发
	EXTI_BASE->RTSR |= (1 << 2);//开启line2上升沿触发

	NVIC_BASE->ISER[NVIC_EXTI2 / 32] |= ( 1 << NVIC_EXTI2 % 32 );//使能中断位(要清除的话,相反操作就OK) 
	NVIC_BASE->IP[NVIC_EXTI2] = 2 << 4;//设置响应优先级和抢断优先级,0最高,15最低,marlin使用的电机定时器优先级是2,温度定时器优先级是3

	#elif T2_STM32F446
	
	RCC->APB2ENR |= 1 << 14; //使能SYSCFG时钟  
	SYSCFG->EXTICR[0] &= ~(0x000F << 4); //清除原来设置！！！
	SYSCFG->EXTICR[0] |= 4 << 4; //EXTI1映射到PE1
	//中断设置
	EXTI->IMR |= 1 << 1; //开启line 1上的中断(如果要禁止中断，则反操作即可)
	EXTI->RTSR |= 1 << 1; //line 1上事件上升降沿触发
	uint32_t temp;
	temp = 1 << (4 - 2); //枪占优先级为1
	temp |= 2 & (0x0f >> 2); //响应优先级为2
	temp &= 0xf;//取低四位 
	NVIC->ISER[EXTI1_IRQn / 32] |= 1 << EXTI1_IRQn % 32;//使能中断位
	NVIC->IP[EXTI1_IRQn] |= temp;//设置中断级别

	#endif
}
void gobackScreen(VoidFun screen) {
	lastScreen = nullptr;
	messageIndexNow = 0;
	currentScreen = screen;
}

//SECTOR6:*************************固件总接口*************************
void tronxyInit(void)
{
	EROM_Init();
	LCD_Init();
	AT24CXX_Init();
	#if defined(LCD_USE_TOUCH)
	XPT2046::Init();
	#endif
	powerLossInit();

	picIco_dir.value = MCU_PIC_ADDR(POS_ICO_DIR);
	picIco_file.value = MCU_PIC_ADDR(POS_ICO_FILE);

	showLog();
	currentScreen = logScreen;
}

void tronxyUpdate(void)
{
	if(myui_tf)return; //防quickstop_stepper调用idle重新进来
	myui_tf = true;
	if(currentScreen)currentScreen();
	if(topwin) {
		topwin->update();
		topwin->clrUpdateSign();
		#if defined(LCD_USE_TOUCH)
		// bool tf = false;//YSZ_WORK:这里如果不这样处理,topwin->touch()后,touchStep会与touch()里面执行后的结果不一样.
		// uint32_t addr = (uint32_t)topwin;
		// if(topwin->touchStep == 11)tf = true;
		if(logTime == 0)topwin->touch(tx,ty);//在logScreen期间,不执行触摸
		// if(tf) {
		// 	uint32_t t = (uint32_t)topwin;
		// 	if(t != addr)SERIAL_CHAR('#');
		// }
		#endif
	}
	myui_tf = false;
}
