#pragma once

#include "../api/yType.hpp"

#define POS_MAIN_EN         0 //01-首页EN.bmp
#define POS_MAIN_CN         1 //02-首页CN.bmp
#define POS_SYSTEM_EN       2 //03-系统EN.bmp
#define POS_SYSTEM_CN       3 //04-系统CN.bmp
#define POS_TOOL_EN         4 //05-工具EN.bmp
#define POS_TOOL_CN         5 //06-工具CN.bmp
#define POS_FILE_MANU       6 //07-文件操作.bmp
#define POS_FILE_CHOOSE     7 //08-文件选择.bmp
#define POS_MANUAL          8 //09-手动.bmp
#define POS_INFO_EN         9 //10-系统信息EN.bmp
#define POS_INFO_CN         10 //11-系统信息CN.bmp
#define POS_PREHEAT         11 //12-预热.bmp
#define POS_FIXMATERIAL     12 //13-装卸耗材.bmp
#define POS_FIXMATERIAL2    13 //14-装卸耗材.bmp
#define POS_FIXMATERIAL3    14 //15-装卸耗材.bmp
#define POS_SYSTEM_STATUS   15 //16-状态.bmp
#define POS_KEYBOARD        16 //17-数字键盘.bmp
#define POS_PRINT_TOOL_EN   17 //18-打印工具EN.bmp
#define POS_PRINTING_RESUME 18 //19-打印界面.bmp
#define POS_PRINTING        19 //20-打印界面_暂停.bmp
#define POS_FAN_EN          20 //21-风扇EN.bmp
#define POS_FAN_CN          21 //22-风扇CN.bmp
#define POS_MANUALLEVEL_EN  22 //23-平台调平EN.bmp
#define POS_MANUALLEVEL_CN  23 //24-平台调平CN.bmp
#define POS_WIFI_ON         24 //25-WIFI_ON.bmp
#define POS_WIFI_OFF        25 //26-WIFI_OFF.bmp
#define POS_ZOFFSET_EN      26 //27-Z_offset_EN.bmp
#define POS_ZOFFSET_CN      27 //28-Z_offset_CN.bmp
#define POS_AUTOLEVEL_EN    28 //29-自动平台调平EN.bmp
#define POS_AUTOLEVEL_CN    29 //30-自动平台调平CN.bmp
#define POS_PRINTMODE_EN    30 //31-打印模式EN.bmp
#define POS_PRINTMODE_CN    31 //32-打印模式CN.bmp
#define POS_EOFFSET         32 //33-E偏移调整界面
#define POS_ICO_DIR         38 //39-dir.bmp
#define POS_ICO_FILE        39 //40-file.bmp
#define POS_ICO_LOGO        40 //41-logo.bmp

#define COLOR_LIGHTBLUE     0xCEBE
#define COLOR_LIGHTBLUE_T   0x8433
#define POPWIN_COLOR        0x9f7d

#define MCU_FONTU16_ADDR    0x00111000
#define MCU_LOG_ADDR        0x001B5000
#define MCU_UI_ADDR         0x0807F000 //图片地址表,512K,128K:0x0801F000,64K:0x0800F000
#define MCU_PIC_ADDR(v)     *(uint32_t*)((v) * 4 + MCU_UI_ADDR)
#define MCU_TIP_ADDR(v)     (const WORD*)(MCU_PIC_ADDR(v) + MCU_UI_ADDR)

extern PICinfo pic_page;
extern PICinfo pic_self;

extern const PICinfo picMain_focus;
extern const PICinfo pic_popwin_background;
extern const PICinfo pic_button_background_idle;
extern const PICinfo pic_button_background_focus;
extern const PICinfo pic_white;
extern PICinfo pic_base,pic_title;
extern PICinfo pic_value_bg,pic_value_bgfc;
extern PICinfo pic_button_idle,pic_button_focus;
extern const PICinfo pic_log;
extern PICinfo picIco_dir;
extern PICinfo picIco_file;

