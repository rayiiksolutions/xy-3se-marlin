
#pragma once

constexpr PICinfo homepage_rs = {//主页
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	8,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 390,//value
};
constexpr PICinfo system_rs = {//系统
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	7,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 398,//value
};
// constexpr WORD system_rs[] = {0x30b7,0x30b9,0x30c6,0x30e0,0};

constexpr PICinfo tool_rs = {//工具
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	10,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 405,//value
};
// constexpr WORD tool_rs[] = {0x5de5,0x5177,0};

constexpr PICinfo print_rs = {//打印
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	10,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 415//value
};
// constexpr WORD print_rs[] = {0x30d7,0x30ea,0x30f3,0x30c8,0};

constexpr PICinfo back_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	7,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 425,//value
};
constexpr PICinfo status_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 432,//value
};
const PICinfo info_rs[3] = {
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
		6,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 438,//value
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
		5,//xpixel
		THEME_YPIXEL,//ypixel
		2,//bytesPerLine
		40,//vspace
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
		5,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 445,//value
	},
};
constexpr PICinfo language_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 450,//value
};
const PICinfo factory_rs[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 454,//value
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
		-15,//xpixel
		THEME_YPIXEL,//ypixel
		2,//bytesPerLine
		40,//vspace
		Font_En10x20.value,//value 空格
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
		8,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 460,//value
	}
};
constexpr PICinfo theme_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 468,//value
};
constexpr WORD calibration_rs[] = {0x30ad,0x30e3,0x30ea,0x30d6,0x30ec,0x30fc,0x30b7,0x30e7,0x30f3,0};
constexpr PICinfo mode_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 472,//value
};
constexpr PICinfo manual_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 477,//value
};
constexpr PICinfo preheat_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	9,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 483,//value
};
constexpr PICinfo filament_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 492,//value
};
constexpr PICinfo level_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	7,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 496,//value
};
constexpr PICinfo fan_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	9,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 503,//value
};
constexpr PICinfo emergency_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	9,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 512,//value
};
constexpr PICinfo offset_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	10,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 521,//value
};
// constexpr PICinfo com_rs = {};
constexpr PICinfo cool_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	10,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 531,//value
};
constexpr PICinfo throat_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 541,//value
};
constexpr PICinfo board_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 546,//value
};
constexpr PICinfo confirm_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	8,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 551,//value
};
constexpr PICinfo save_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 559,//value
};
constexpr PICinfo modify_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 565,//value
};
constexpr PICinfo key_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	10,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 571,//value
};
const PICinfo totalControl_rs[3] = {
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
		6,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 581,//value
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
		-10,//xpixel
		THEME_YPIXEL,//ypixel
		2,//bytesPerLine
		40,//vspace
		(DWORD)Font_En10x20.value,//value
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
		8,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 588,//value
	}
};
constexpr PICinfo autoPark_rs[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 596,//value
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
		4,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 601,//value
	}
};
constexpr PICinfo autoLevel_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 596,//value
};
// constexpr PICinfo manualLevel_rs[2] = {};//使用manual_rs
constexpr PICinfo copy_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	10,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 605,//value
};
constexpr PICinfo mirror_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	7,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 615,//value
};
constexpr PICinfo test_rs = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 622,//value
};
constexpr PICinfo russian_rs = {
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	7,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 626,//value
};

