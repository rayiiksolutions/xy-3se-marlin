
#pragma once

constexpr PICinfo homepage_pt = {//主页
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1522,//value
};
constexpr PICinfo system_pt = {//系统
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1536,//value
};
// constexpr WORD system_pt[] = {0x7cfb,0x7edf,0};

constexpr PICinfo tool_pt = {//工具
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1543,//value
};
// constexpr WORD tool_pt[] = {0x5de5,0x5177,0};

constexpr PICinfo print_pt = {//打印
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1553,//value
};
// constexpr WORD print_pt[] = {0x6253,0x5370,0};

constexpr PICinfo back_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1562,//value
};
constexpr PICinfo status_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1569,//value
};
constexpr PICinfo info_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1575,//value
};
constexpr PICinfo language_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1579,//value
};
constexpr PICinfo factory_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1585,//value
};
constexpr PICinfo theme_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1594,//value
};
constexpr WORD calibration_pt[] = {0x5c4f,0x5e55,0x6821,0x51c6,0};
constexpr PICinfo mode_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1598,//value
};
constexpr PICinfo manual_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1602,//value
};
// constexpr PICinfo manualLevel_pt = {};//manual_pt代替
constexpr PICinfo preheat_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1608,//value
};
constexpr PICinfo filament_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1615,//value
};
const PICinfo level_pt[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1624,//value
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
		10,//xpixel
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1632,//value
	},
};
constexpr PICinfo fan_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1637,//value
};
constexpr PICinfo emergency_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1647,//value
};
constexpr PICinfo offset_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE *1657,//value
};
// constexpr PICinfo com_pt = {};
constexpr PICinfo cool_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1663,//value
};
constexpr PICinfo throat_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1670,//value
};
constexpr PICinfo board_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1678,//value
};
constexpr PICinfo confirm_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1683,//value
};
constexpr PICinfo save_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1691,//value
};
constexpr PICinfo modify_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1696,//value
};
constexpr PICinfo key_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1702,//value
};
const PICinfo totalControl_pt[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1709,//value
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
		15,//xpixel
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1718,//value
	},
};
const PICinfo autoPark_pt[3] = {
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
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1723,//value
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
		4,//wNum
		THEME_XPIXEL,//xpixel
		THEME_YPIXEL,//ypixel
		BYTES_PER_LINE,
		THEME_YPIXEL * BYTES_PER_LINE,//vspace
		MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1732,//value
	},
};
constexpr PICinfo autoLevel_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1732,//value
};
constexpr PICinfo copy_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1736,//value
};
constexpr PICinfo mirror_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1741,//value
};
constexpr PICinfo test_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1748,//value
};
constexpr PICinfo portuguese_pt = {
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
	MCU_THEME_ADDR + THEME_YPIXEL * BYTES_PER_LINE * 1754,//value
};
// constexpr WORD chinese_pt[] = {0x4e2d,0x6587,0};
