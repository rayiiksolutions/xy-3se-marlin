#include "winPic.h"

PICinfo pic_page = {
	{
		1,//ifERom
		TYPE_CPIC,//type
		1,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	480,//xpixel
	320,//ypixel
	960,//bytesPerLine
	307200,//vspace
	0x0000,//value
};
PICinfo pic_self = {
	{
		0,//ifERom
		TYPE_SELFFUN,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	0,//wNum
	0,//xpixel
	0,//ypixel
	0,//bytesPerLine
	0,//vspace
	0x0000,//value
};
const PICinfo picMain_focus = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		1,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	160,//zero
	1,//wNum
	0,//xpixel
	0,//ypixel
	0,//bytesPerLine
	0,//vspace
	COLOR_BLACK,//value
};
const PICinfo pic_popwin_background={
	{
		0,//ifERom
		TYPE_COLOR,//type
		1,//typeTf
		0,//black_tf
		1,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	0,//xpixel
	0,//ypixel
	0,//bytesPerLine
	0,//vspace
	POPWIN_COLOR,//value
};
const PICinfo pic_button_background_idle={
	{
		0,//ifERom
		TYPE_COLOR,//type
		1,//typeTf
		0,//black_tf
		1,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	0,//xpixel
	0,//ypixel
	0,//bytesPerLine
	0,//vspace
	0x0d7b,//value
};
const PICinfo pic_button_background_focus={
	{
		0,//ifERom
		TYPE_COLOR,//type
		1,//typeTf
		0,//black_tf
		1,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	0,//xpixel
	0,//ypixel
	0,//bytesPerLine
	0,//vspace
	0x1111,//value
};

const PICinfo pic_white = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	480,//xpixel
	320,//ypixel
	0,//bytesPerLine
	0,//vspace
	0xFFFF,//value
};

PICinfo pic_title = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		1,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	480,//xpixel
	320,//ypixel
	0,//bytesPerLine
	0,//vspace
	COLOR_BLUE,//value
};
PICinfo pic_base = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	480,//xpixel
	320,//ypixel
	0,//bytesPerLine
	0,//vspace
	0,//value
};
PICinfo pic_value_bg = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		0,//typeTf
		0,//black_tf
		1,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	480,//xpixel
	320,//ypixel
	0,//bytesPerLine
	0,//vspace
	0,//?????????????????????
};
PICinfo pic_value_bgfc = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		0,//typeTf
		0,//black_tf
		1,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	480,//xpixel
	320,//ypixel
	0,//bytesPerLine
	0,//vspace
	0,//?????????????????????
};
PICinfo pic_button_idle = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		1,//typeTf
		0,//black_tf
		1,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	160,//xpixel
	20,//ypixel
	0,//bytesPerLine
	0,//vspace
	COLOR_BLUE,//value
};
PICinfo pic_button_focus = {
	{
		0,//ifERom
		TYPE_COLOR,//type
		1,//typeTf
		0,//black_tf
		1,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	160,//xpixel
	20,//ypixel
	0,//bytesPerLine
	0,//vspace
	COLOR_BLACK,//value
};
PICinfo picIco_dir = {
	{
		1,//ifERom
		TYPE_CPIC,//type
		1,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	32,//xpixel
	48,//ypixel
	64,//bytesPerLine
	3072,//vspace
	0,//value
};
PICinfo picIco_file = {
	{
		1,//ifERom
		TYPE_CPIC,//type
		1,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	32,//xpixel
	48,//ypixel
	64,//bytesPerLine
	3072,//vspace
	0,//value
};
const PICinfo pic_log = {
	{
		1,//ifERom
		TYPE_CPIC,//type
		1,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	1,//wNum
	480,//xpixel
	320,//ypixel
	960,//bytesPerLine
	307200,//vspace
	MCU_LOG_ADDR,//value
};

