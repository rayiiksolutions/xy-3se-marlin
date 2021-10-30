
#pragma once

constexpr PICinfo homepage_jp = {//主页
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	6,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 109,//value
};
constexpr PICinfo system_jp = {//系统
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 115,//value
};
// constexpr WORD system_jp[] = {0x30b7,0x30b9,0x30c6,0x30e0,0};

constexpr PICinfo tool_jp = {//工具
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 119,//value
};
// constexpr WORD tool_jp[] = {0x5de5,0x5177,0};

constexpr PICinfo print_jp = {//打印
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 121//value
};
// constexpr WORD print_jp[] = {0x30d7,0x30ea,0x30f3,0x30c8,0};

constexpr PICinfo back_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 125,//value
};
constexpr PICinfo status_jp = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	5,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 127,//value
};
constexpr PICinfo info_jp = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	5,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 132,//value
};
constexpr PICinfo language_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 137,//value
};
constexpr PICinfo factory_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 139,//value
};
constexpr PICinfo theme_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 143,//value
};
constexpr WORD calibration_jp[] = {0x30ad,0x30e3,0x30ea,0x30d6,0x30ec,0x30fc,0x30b7,0x30e7,0x30f3,0};
constexpr PICinfo mode_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 146,//value
};
constexpr PICinfo manual_jp = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	5,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 149,//value
};
constexpr PICinfo preheat_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 154,//value
};
constexpr PICinfo filament_jp[3] = {
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
		5,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 158,//value
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
		17,//wNum
		30,//xpixel
		THEME_YPIXEL,//ypixel
		1,//bytesPerLine
		0,//vspace
		0,//value
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 163,//value
	},
};
constexpr PICinfo level_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 165,//value
};
constexpr PICinfo fan_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 168,//value
};
constexpr PICinfo emergency_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 171,//value
};
constexpr PICinfo offset_jp = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
	0,//former
	},
	0,//zero
	5,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 175,//value
};
constexpr PICinfo com_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 180,//value
};
constexpr PICinfo cool_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 183,//value
};
constexpr PICinfo throat_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 185,//value
};
constexpr PICinfo board_jp = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	6,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 188,//value
};
constexpr PICinfo confirm_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 194,//value
};
constexpr PICinfo save_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 196,//value
};
constexpr PICinfo modify_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 198,//value
};
constexpr PICinfo key_jp = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	5,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 200,//value
};
constexpr PICinfo totalControl_jp[3] = {
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
		4,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 205,//value
	},
	{
		{
			0,//ifERom
			TYPE_BINARY,//type
			0,//typeTf
			0,//black_tf
			0,//ifFrame
			0,//former
		},
		0,//zero
		17,//wNum
		0,//xpixel
		THEME_YPIXEL,//ypixel
		2,//bytesPerLine
		40,//vspace
		0,//value 空格
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
		4,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 209,//value
	},
};
constexpr PICinfo autoPark_jp[3] = { //自动和停靠各3个字
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
		3,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 213,//value
	},
	{
		{
			0,//ifERom
			TYPE_BINARY,//type
			0,//typeTf
			0,//black_tf
			0,//ifFrame
			0,//former
		},
		0,//zero
		17,//wNum
		0,//xpixel
		THEME_YPIXEL,//ypixel
		2,//bytesPerLine
		40,//vspace
		0,//value 空格
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
		3,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 216,//value
	},
};
constexpr PICinfo autoLevel_jp[2] = {
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
		3,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 213,//value
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
		3,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 165,//value
	}
};
constexpr PICinfo manualLevel_jp[2] = { //手动+调平
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
		5,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 149,//value
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
		3,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 165,//value
	}
};
constexpr PICinfo copy_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 219,//value
};
constexpr PICinfo mirror_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 222,//value
};
constexpr PICinfo test_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 225,//value
};
constexpr PICinfo japanese_jp = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 228,//value
};

