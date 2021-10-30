#pragma once

#include "yLabel.hpp"
class yVariable: public yLabel
{
	public:
		yVariable() = delete;
		yVariable(int xStart,int yStart,int width,int height,const PICinfo*picIdle,const PICinfo*picFocus,bool enfocus = true);
		virtual bool update(bool force = false);//窗口控件更新
		inline virtual void setfocus(bool tf) {
			if(enFocus){
				if(tf && !focus){
					pic_color1 = pic_focus;
					pic_color2 = pic_focus2;
				}
				else if(!tf && focus){
					pic_color1 = pic_idle;
					pic_color2 = pic_idle2;
				}
				yLabel::setfocus(tf);
			}
		}
		void changeBorder(int v);//fixedEdge决定固定边,这里变化的是与之对应的可变的边
		inline void setPicColor(color_t first_idle_color,color_t second_idle_color,color_t first_focus_color = 0,color_t second_focus_color = 0)
		{
			pic_color1 = pic_idle = first_idle_color;
			pic_color2 = pic_idle2 = second_idle_color;
			pic_focus = first_focus_color;
			pic_focus2 = second_focus_color;
		}
		bool horizon;//true:horizon,false:vertical
		bool fixedEdge;//true-当horizon为true时,左边,否则上边;false-当horizon为true时,右边,否则下边;
		WORD tail_char;

	private:
		color_t pic_idle,pic_focus;
		color_t pic_idle2,pic_focus2;
		color_t pic_color1,pic_color2;
		void horizonRect();//填充水平类型区域
		void verticalRect();//填充垂直类型区域
		void horizonIBinary();//显示水平类二元图
		void verticalIBinary();//显示垂直类二元图
};
