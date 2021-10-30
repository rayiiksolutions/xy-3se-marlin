
#pragma once

#define	STM32_IDADDR        STM32F1_IDADDR
#define	POS_BOOTINFO        0x08007800
#define POS_VERSION         0x08007A00
#define POS_INTERRUPT       0x08007D00
#define MCU_NAME            "STM32F103ZET"
#define BOOT_INFO_SIZE      1060
#define INTERRUPT_SIGN      0xA5A5
#define TRONXY_SOFT_VERSION  "V1.22"

#include "2lcd/api/yWin.hpp"
#include "2lcd/api/yScale.hpp"
#include "2lcd/api/yLabel.hpp"
#include "2lcd/api/yNotepad.hpp"
#include "2lcd/api/yVariable.hpp"
#include "2lcd/picdata/winPic.h"
#include "2lcd/picdata/yIco.h"
#include "3rom/romdata.h"
#include "yCompress.h"

extern volatile BYTE globalStop;

void tronxyInit(void);
void tronxyUpdate(void);
void screenPopIndex(int index,VoidFun other = nullptr);
void screenCloseMessageBox(void);
void screenFilament(void);
bool updateFromSdcardStart(void);
void updateAutoLevel(int i,int j);
void screenKillError(const char*errorInfo);
void screenMain(void);
void gobackScreen(VoidFun screen);
void screenReadFailAfter(void);
#if TRONXY_PRINTING_NOWAIT
extern bool printing_no_wait;
#endif
extern BYTE my_lang,my_theme;
extern yWindows *topwin;
extern int messageIndexNow,messageIndexNext;
extern uint32_t printTotalTime, modifyTotalTime;

#define POP_FILAMENT_OUT              1
#define POP_PAUSING                   2
#define POP_WAITNOZZLE_HEAT           3
#define POP_KILL_ERROR                4 
#define POP_LANGUAGE                  5
#define POP_FACTORY_RESET             6
#define POP_CALIBRATE_CONFIRM         7
#define POP_STOP_PRINT                8
#define POP_COMPLETE_PRINT            9
#define POP_DELETE_FILE               10
#define POP_EXTRUDER_COLD             11
#define POP_FILAMENT_INSERT           12
#define POP_RECOVERY_PRINT            13
#define POP_NOSUPPORT                 14
#define POP_PROB_FAIL                 15
#define POP_NORMAL_MODE               16
#define POP_SAVE_HOTENDOFFSET         17
#define POP_OPENFILE_FAIL             18
#define POP_READFILE_FAIL             19
#define POP_INSERT_CARD               20

#define LANG_ENGLISH      0
#define LANG_CHINESE      1
#define LANG_JAPANESE     2
#define LANG_KOREAN       3
#define LANG_RUSSIAN      4
#define LANG_FRENCH       5
#define LANG_GERMAN       6
#define LANG_SPANISH      7
#define LANG_PORTUGUESE   8
enum TronxyPrintStatus
{
	PRINT_IDLE = 0,
	PRINT_RUNNING,//打印中
	PRINT_PAUSE,//暂停
	PRINT_PAUSING,//暂停中
	PRINT_RESUME,//恢复中
	PRINT_RUNOUT,//断料
	PRINT_AUTO_LEVEL,//在自动调平中
	PRINT_START_UPDATE1,//开机更新中,Tronxy
	PRINT_START_UPDATE2,//Update
	PRINT_UPDATE_END,//开机更新结束
};
extern TronxyPrintStatus my_print_status;

