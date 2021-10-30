
#pragma once

constexpr PICinfo homepage_cn = {//主页
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR,//value
};
constexpr PICinfo system_cn = {//系统
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 2,//value
};
// constexpr WORD system_cn[] = {0x7cfb,0x7edf,0};

constexpr PICinfo tool_cn = {//工具
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 4,//value
};
// constexpr WORD tool_cn[] = {0x5de5,0x5177,0};

constexpr PICinfo print_cn = {//打印
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 6,//value
};
// constexpr WORD print_cn[] = {0x6253,0x5370,0};

constexpr PICinfo back_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 8,//value
};
constexpr PICinfo status_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 10,//value
};
constexpr PICinfo info_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 12,//value
};
constexpr PICinfo language_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 14,//value
};
constexpr PICinfo factory_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 16,//value
};
constexpr PICinfo theme_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 18,//value
};
constexpr WORD calibration_cn[] = {0x5c4f,0x5e55,0x6821,0x51c6,0};
constexpr PICinfo mode_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 20,//value
};
constexpr PICinfo manual_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 22,//value
};
constexpr PICinfo manualLevel_cn[2] = {
	{
		{
			1,//ifERom
			TYPE_BINARY,//type
			0,//typeTf
			0,//black_tf
			0,//ifFrame
		0,//former
		},
		0,//zero
		2,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 22,//value
	},
	{
		{
			1,//ifERom
			TYPE_BINARY,//type
			0,//typeTf
			0,//black_tf
			0,//ifFrame
		0,//former
		},
		0,//zero
		2,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 30,//value
	}
};
constexpr PICinfo preheat_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 24,//value
};
constexpr PICinfo filament_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	4,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 26,//value
};
constexpr PICinfo level_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 30,//value
};
constexpr PICinfo fan_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 32,//value
};
constexpr PICinfo emergency_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	4,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 34,//value
};
constexpr PICinfo offset_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 38,//value
};
constexpr PICinfo com_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	3,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 40,//value
};
constexpr PICinfo cool_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 43,//value
};
constexpr PICinfo throat_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 45,//value
};
constexpr PICinfo board_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 47,//value
};
constexpr PICinfo confirm_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 49,//value
};
constexpr PICinfo save_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 51,//value
};
constexpr PICinfo modify_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 53,//value
};
constexpr PICinfo key_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 55,//value
};
constexpr PICinfo totalControl_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	4,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 57,//value
};
constexpr PICinfo autoPark_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	4,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 61,//value
};
constexpr PICinfo autoLevel_cn[2] = {
	{
		{
			1,//ifERom
			TYPE_BINARY,//type
			0,//typeTf
			0,//black_tf
			0,//ifFrame
		0,//former
		},
		0,//zero
		2,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 61,//value
	},
	{
		{
			1,//ifERom
			TYPE_BINARY,//type
			0,//typeTf
			0,//black_tf
			0,//ifFrame
		0,//former
		},
		0,//zero
		2,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 30,//value
	}
};
constexpr PICinfo copy_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 65,//value
};
constexpr PICinfo mirror_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 67,//value
};
constexpr PICinfo test_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 69,//value
};
constexpr PICinfo chinese_cn = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	2,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 71,//value
};
// constexpr WORD chinese_cn[] = {0x4e2d,0x6587,0};


