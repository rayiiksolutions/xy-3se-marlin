
#pragma once

constexpr PICinfo homepage_fr = {//主页
	{
		1,//ifERom
		TYPE_BINARY,//type
		0,//typeTf
		0,//black_tf
		0,//ifFrame
		0,//former
	},
	0,//zero
	14,//wNum
	THEME_XPIXEL,//xpixel
	THEME_YPIXEL,//ypixel
	BYTES_PER_LINE,
	THEME_YPIXEL * BYTES_PER_LINE,//vspace
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 669,//value
};
constexpr PICinfo system_fr = {//系统
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 683,//value
};
// constexpr WORD system_fr[] = {0x7cfb,0x7edf,0};

constexpr PICinfo tool_fr = {//工具
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 691,//value
};
// constexpr WORD tool_fr[] = {0x5de5,0x5177,0};

constexpr PICinfo print_fr = {//打印
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 695,//value
};
// constexpr WORD print_fr[] = {0x6253,0x5370,0};

constexpr PICinfo back_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 703,//value
};
constexpr PICinfo status_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 711,//value
};
constexpr PICinfo info_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 716,//value
};
constexpr PICinfo language_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 720,//value
};
constexpr PICinfo factory_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 726,//value
};
constexpr PICinfo theme_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 733,//value
};
constexpr WORD calibration_fr[] = {0x5c4f,0x5e55,0x6821,0x51c6,0};
constexpr PICinfo mode_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 738,//value
};
constexpr PICinfo manual_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 742,//value
};
// constexpr PICinfo manualLevel_fr = {};//manual_fr代替
constexpr PICinfo preheat_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 748,//value
};
constexpr PICinfo filament_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 757,//value
};
constexpr PICinfo level_fr[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 765,//value
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
		THEME_XPIXEL,//xpixel
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 774,//value
	}
};
constexpr PICinfo fan_fr[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 778,//value
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
		THEME_XPIXEL,//xpixel
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 787,//value
	}
};
constexpr PICinfo emergency_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 791,//value
};
constexpr PICinfo offset_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 798,//value
};
// constexpr PICinfo com_fr = {};
const PICinfo cool_fr[3] = {
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
		9,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 806,//value
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
		THEME_XPIXEL,//xpixel
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
		7,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 816,//value
	}
};
constexpr PICinfo throat_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 823,//value
};
constexpr PICinfo board_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 828,//value
};
constexpr PICinfo confirm_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 835,//value
};
constexpr PICinfo save_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 842,//value
};
constexpr PICinfo modify_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 849,//value
};
constexpr PICinfo key_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 857,//value
};
const PICinfo totalControl_fr[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 864,//value
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
		20,//xpixel
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
		5,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 873,//value
	},
};
const PICinfo autoPark_fr[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 878,//value
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
		7,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 883,//value
	},
};
constexpr PICinfo autoLevel_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 883,//value
};
constexpr PICinfo copy_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 890,//value
};
constexpr PICinfo mirror_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 895,//value
};
constexpr PICinfo test_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 901,//value
};
constexpr PICinfo french_fr = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 906,//value
};
// constexpr WORD chinese_fr[] = {0x4e2d,0x6587,0};


