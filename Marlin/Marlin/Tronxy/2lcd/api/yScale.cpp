#include "yScale.hpp"
yScale::yScale(int xStart,int yStart,int width,int height,const PICinfo*picIdle,const PICinfo*picFocus,bool enfocus)
	:yVariable(xStart,yStart,width,height,picIdle,picFocus,enfocus)
{
	sign.content.scale = 1;
}
bool yScale::update(bool force)
{
	if(force || sign.content.scale) {
		if(focus)rect(scale_focus_color,w_xstart,w_ystart,w_xend,w_yend);
		else rect(scale_idle_color,w_xstart,w_ystart,w_xend,w_yend);
	}
	return yVariable::update(force);
}
yMulti::yMulti(int xStart,int yStart,int width,int height,const PICinfo*picIdle,const PICinfo*picFocus,bool enfocus)
	:yVariable(xStart,yStart,width,height,picIdle,picFocus,enfocus)
{
	wMulPic = nullptr;
	pnum = 0;
	head_char = 0;
	div_char = 0;
}
bool yMulti::update(bool force)
{
	bool ret = yVariable::update(force);
	if((sign.content.text || force) && u16_mode == 2 && wMulPic) {
		bool tf;
		int xStart,xEnd,yStart,yEnd;
		uint32_t addr;
		binaryInfo.x = w_xstart + wxOffset;
		yStart = binaryInfo.y = w_ystart + wyOffset;
		putc(head_char);
		putc(div_char);
		for(int j = 0; j < pnum; j++) {
			switch(wMulPic[j].wNum) {
			case 17:
				binaryInfo.x = w_xstart + wxOffset + wMulPic[j].xPixel;
				yStart = binaryInfo.y + wMulPic[j].yPixel;
				binaryInfo.y = yStart;
				break;
			default:
				binaryInfo.pic = &wMulPic[j];
				addr = wMulPic[j].value;
				yEnd = binaryInfo.y + wMulPic[j].yPixel - 1;
				for(int i = 0; i < wMulPic[j].wNum; i++,addr += wMulPic[j].vSpace) {
					if(textDistance < 100) {
						if(wMulPic[j].attr.ifERom)getPureED8bScale(xStart,xEnd,addr);
						else getPureID8bScale(xStart,xEnd,(const BYTE*)addr);
						xEnd += textDistance;
					}
					else {
						xStart = binaryInfo.x;
						xEnd = binaryInfo.x + wMulPic[j].xPixel - 1;
					}
					w_xend--;
					tf = boundIntersect3(&xStart,&yStart,&xEnd,&yEnd);
					w_xend++;
					if(tf) {
						binaryInfo.x = xEnd + 1;
						continue;//求本图像范围与外可视范围及本widget三方的交集
					}
					if(wMulPic[j].attr.ifERom)showED8b(xStart,yStart,xEnd,yEnd,addr);
					else showID8b(xStart,yStart,xEnd,yEnd,(const BYTE*)addr);
				}
				break;
			}
		}
		putc(tail_char);
		fillMargin();
	}
	return ret;
}
void yMulti::setMulti(const PICinfo* pic, int num)
{
	sign.content.text = 1;
	wMulPic = pic;
	pnum = num;
	u16_mode = 2;
}

