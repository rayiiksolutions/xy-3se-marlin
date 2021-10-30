
#pragma once

const WORD tip_language_cn[] = {' ','\n',' ',0xFCFC,0x5f53,0x524d,0x662f,0x4e2d,0x6587,0x002c,0x662f,0x5426,0x5207,0x6362,0x81f3,0x82f1,0x6587,0x003F,0};
const WORD tip_factoryreset_cn[] = {' ','\n',' ',0x4f60,0x786e,0x5b9a,0x6062,0x590d,0x51fa,0x5382,0x8bbe,0x7f6e,0x5417,0x003F,
'\n',' ',0x786e,0x8ba4,0x540e,0x002c,0x4f1a,0x91cd,0x542f,0x0021,0};
const WORD tip_calibrate_cn[] = {' ','\n',' ',0x6821,0x51c6,0x5b8c,0x6210,0x002c,0x662f,0x5426,0x4fdd,0x5b58,0};
const WORD tip_coodextruder_cn[] = {' ','\n',' ',0x55b7,0x5934,0x8fc7,0x51b7,0x002c,0x4e0d,0x53ef,0x79fb,0x6599,0};
const WORD tip_stopprint_cn[] = {' ','\n',' ',0x662f,0x5426,0x505c,0x6b62,0x6253,0x5370,0x003f,0};
const WORD tip_completeprint_cn[] = {'\n',' ',0x6253,0x5370,0x5b8c,0x6210,0};
const WORD tip_deletefile_cn[] = {' ','\n',' ',0x786e,0x5b9a,0x8981,0x5220,0x9664,0x8be5,0x6587,0x4ef6,0x003f,0};
const WORD tip_filamentout_cn[] = {' ','\n',' ','\n',' ',0x65ad,0x6599,0x68c0,0x6d4b,0x5230,0x002c,0x505c,0x9760,0x4e2d,0};
const WORD tip_pausing_cn[] = {' ','\n',' ','\n',' ',0x6682,0x505c,0x52a8,0x4f5c,0x4e2d,0};
const WORD tip_filamentinsert_cn[] = {' ','\n',' ',0x63d2,0x5165,0x65b0,0x7269,0x6599,0x002c,0x6309,0x786e,0x8ba4,0x7ee7,0x7eed,0};
const WORD tip_extruder_cn[] = {' ','\n',' ',0x6324,0x51fa,0x673a,':'};
const WORD tip_waitforheating_cn[] = {' ','\n',' ','\n',' ',0x55b7,0x5934,0x52a0,0x70ed,0x4e2d,0x002c,0x8bf7,0x7b49,0x5f85,0x002e,0x002e,0x002e,0};
const WORD tip_recoveryprint_cn[] = {' ','\n',' ','\n',' ',0x6253,0x5370,0x4e2d,0x65ad,0x002c,0x662f,0x5426,0x6062,0x590d,0x003f,0};
const WORD tip_nosupport_cn[] = {' ','\n',' ',0x4e0a,0x6b21,0x6253,0x5370,0x672a,0x5b8c,0x6210,'\n',' ',0x662f,0x5426,0x7ee7,0x7eed,'?',0};
const WORD tip_err_cn[] = {' ','\n',' ','\n',' ',0x9519,0x8bef,':',0};
const WORD tip_system_reset_cn[] = {' ','\n',' ',0x7cfb,0x7edf,0x505c,0x6b62,0};
const WORD tip_probe_fail_cn[] = {' ','\n',' ',0x63a2,0x5e95,0x5931,0x8d25,'\n',' ',0x786e,0x4fdd,0x9ad8,0x5ea6,0x5dee,0x5c0f,0x4e8e,'2','m','m',0};
const WORD tip_normal_mode_cn[] = {' ','\n',' ',0x5f53,0x524d,0x6253,0x5370,0x6a21,0x5f0f,0x662f,0x666e,0x901a,0x6a21,0x5f0f,0};
const WORD tip_modify_save_cn[]  = {' ','\n',' ',0x662f,0x5426,0x4fdd,0x5b58,0x5f53,0x524d,0x4fee,0x6539,'?',0};
const WORD tip_level_cn[] = {0x8bf7,0x9009,0x62e9,0x8c03,0x5e73,0x6a21,0x5f0f,0};
const WORD tip_open_fail_cn[] = {' ','\n',' ',0x6253,0x5f00,0x6587,0x4ef6,0x5931,0x8d25,':','\n',' ',0};
const WORD tip_read_fail_cn[] = {' ','\n',' ',0x6587,0x4ef6,0x8bfb,0x53d6,0x9519,0x8bef,0};
const WORD tip_insert_sdcard_cn[] = {' ','\n',' ',0x8bf7,0x68c0,0x67e5,'S','D',0x5361,',',0x5e76,0x91cd,0x65b0,0x63d2,0x5165,0};

const char *tip_language_en = " \n Current language:ENGLISH!\n Are you sure to exchange language\n to chinese?";
const char *tip_open_fail_en = " \n Fail to open file:\n ";
const char *tip_read_fail_en = " \n File read error.";
const char *tip_insert_sdcard_en = " \n Please check the SD card and re-insert it.";
const char *tip_normal_mode_en = " \n The current printing mode is normal\n mode.";
const char *tip_modify_save_en = " \n Do you want to save the current\n changes?";
const char *tip_probe_fail_en = " \n Probe failed.\n Make sure that the height difference\n is less than 2mm";
const char *tip_calibrate_en = " \n Calibrate complete.\n Are you sure to save?";
const char *tip_factoryreset_en = " \n Are you sure to factory reset?\n Sure will restart!";
const char *tip_coodextruder_en = " \n Cold extrusion prevented.";
const char *tip_stopprint_en = " \n Are you sure to stop printing?";
const char *tip_completeprint_en = "\n Complete print!";
const char *tip_deletefile_en = " \n Are you sure to delete the file?";
const char *tip_filamentout_en = " \n \n Filament run out,docking aside...";
const char *tip_pausing_en = " \n \n Please wait...";
const char *tip_filamentinsert_en = " \n Insert new material and press OK to\n continue.";
const char *tip_extruder_en = " \n Extruder:";
const char *tip_waitforheating_en = " \n \n Heating now,please wait...";
const char *tip_recoveryprint_en = " \n There is unfinished printing\n Do you want to continue?";
const char *tip_nosupport_en = " \n \n Don't support.";
const char *tip_err_en =  " \n \n Error:";
const char *tip_system_reset_en = " \n System stop!";
const char *tip_level_en = "Please choose level mode";

const char *tip_autoLevel_en = "If the absolute value of the difference\nis greater than 0.5,adjust the screw,do\nit again.";
const char *tip_manualLevel_en = "After the movement, adjust the screw under the platform to make the distance between the extrusion head and the platform equal";

