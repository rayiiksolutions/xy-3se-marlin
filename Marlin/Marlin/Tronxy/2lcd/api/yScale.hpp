
#pragma once

#include "yVariable.hpp"
class yScale : public yVariable //带范围底色的控件
{
	public:
		yScale(int xStart = 0,int yStart = 0,int width = 0,int height = 0,
		const PICinfo*picIlde = 0,const PICinfo*picFocus = 0,bool enfocus = true);
		virtual bool update(bool force = false);
		inline void setScaleColor(color_t idle_color,color_t focus_color){scale_idle_color = idle_color;scale_focus_color = focus_color;}
		
	private:
		color_t scale_idle_color,scale_focus_color;
};
/*
连续显示pic的wNum数量的二位图
当pic的wNum > 16时,它有特殊含义:
17:换行,xpixel表示换行后,字体离wgRect.xStart + wxOffset的距离,ypixel表示换行y增加量
显示不等宽字体的控件,每个字体对应一个pic,pic数组决定字体的顺序,setChars中num决定这个数组的长度
*/
class yMulti : public yVariable
{
	public:
		yMulti(int xStart = 0,int yStart = 0,int width = 0,int height = 0,
		const PICinfo*picIlde = 0,const PICinfo*picFocus = 0,bool enfocus = true);
		virtual bool update(bool force = false);
		void setMulti(const PICinfo* pic, int num = 1);
		WORD head_char,div_char;//头字符以及分隔符
	private:
		int pnum;
		const PICinfo *wMulPic;
};
