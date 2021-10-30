#pragma once

#include "yLabel.hpp"
typedef struct
{
	int lineHighest;
	WORD* word;
}PADS;
class yNotepad:public yLabel
{
	public:
		yNotepad(int xStart = 0,int yStart = 0,int width = 0,int height = 0,
		const PICinfo*picIlde = 0,const PICinfo*picFocus = 0,bool enfocus = true);
		virtual ~yNotepad();
		virtual bool update(bool force = false);
		inline virtual void setRectOffset(int x,int y){
			w_xstart += x;
			w_xend += x;
			w_ystart += y;
			w_yend += y;
			curXPixel += x;
		}
		void addChars(const char*str);//向Notepad加入字符串
		void addChars(const WORD*tar);
		void setBuffSpace(int row,int column);
	
	private:
		BYTE Column,Row;//一共有多少列多少行
		BYTE curRow,curColumn;//光标位置:所在行和列
		int curXPixel;//光标列像素位置(用以计算是否越边界)
		PADS *Pads;
		void freePad();
		void allocPad();//此函数应该在设置utf16是否为true后调用
		void lineFeed();//换行
};
