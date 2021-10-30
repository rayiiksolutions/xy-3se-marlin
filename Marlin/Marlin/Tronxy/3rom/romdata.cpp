#include "romdata.h"
namespace yString
{
	//#define m_SIZE(type) ((sizeof(type)+sizeof(int)-1)&~(sizeof(int) - 1))
	#define m_SIZE(type) sizeof(type)
	#define m_NEXT(p,type) *(type*)(p += m_SIZE(type),p - m_SIZE(type))
	#define m_START(n,type) (char*)&n + m_SIZE(type)
	int intToStr(int num, char*str, NUM_MODE mode)
	{
		int tempv = num, n = 0;
		uint32_t temp,t;
		if (!num){
			str[0] = '0';
			str[1] = 0;
			return 1;
		}
		if (mode == 10 && tempv < 0){
			str[0] = '-';
			n = 1;
			temp = -tempv;
		}
			else temp  = tempv;
		for (t = temp; t; n++)t /= mode;
		str[n] = 0;
		for (t = n - 1; temp > 0; t--){
			str[t] = temp % mode;
					if(str[t] > 9)str[t] = str[t] + 'A' - 10;
					else str[t] += '0';
			temp /= mode;
		}
		return n;
	}
	int alignWidth(char* str, const char*src, int srcLen, int width)
	{
		int i = 0;
		if(width < 0){
			width = -width;
			for(; i < width && i < srcLen; i++)str[i] = src[i];
			for(; i < width; i++)str[i] = ' ';
		}
		else {
			int t = width - srcLen;
			for(; i < t; i++)str[i] = ' ';
			for(; i < width; i++)str[i] = src[i - t];
		}
		str[i] = 0;
		return i;
	}
	int intToStr(int num, char*str, NUM_MODE mode,int width)
	{
		if(width == 0){
			str[0] = 0;
			return 0;
		}
		char tstr[32];
		int r = intToStr(num,tstr,mode);
		return alignWidth(str,tstr,r,width);
	}
	int strToInt(const char*str, NUM_MODE mode)
	{
		int n = 0,sign = 1;
		switch(mode)
		{
			case NUM2:
				while(*str == '0' || *str == '1'){
					n = (n << 1) + *str - '0';
					str ++;
				}
				break;
			case NUM8:
				while(*str >= '0' && *str <= '7'){
					n = (n << 3) + *str - '0';
					str ++;
				}
				break;
			case NUM10:
				if(*str == '-'){
					str ++;
					sign = -1;
				}
				while(*str >= '0' && *str <= '9'){
					n = n * 10 + *str - '0';
					str ++;
				}
				n *= sign;
				break;
			case NUM16:
				while(1){
					if(*str >= '0' && *str <= '9')n = (n << 4) + *str - '0';
					else if(*str >= 'A' && *str <= 'F')n = (n << 4) + *str - 'A' + 10;
					else if(*str >= 'a' && *str <= 'f')n = (n << 4) + *str - 'a' + 10;
					else break;
					str ++;
				}
				break;
		}
		return n;
	}

	int doubleToStr(double num, char*str, int width,int dot_num)
	{
		if(width == 0){
			str[0] = 0;
			return 0;
		}
		uint8_t i;
		char tstr[32];
		int n, tv;
		double t = 0.5;
		for(i = 0;i < dot_num;i ++)t /= 10;
		if(num < 0) {
			num -= t;
			num = -num;
			n = (int)num;
			tstr[0] = '-';
			tv = intToStr(n, tstr + 1,NUM10) + 1;
		}
		else {
			num += t;
			n = (int)num;
			tv = intToStr(n, tstr,NUM10);
		}
		num -= n;
		tstr[tv ++] = '.';
		for (i = 0; i < dot_num; i++,tv ++) {
			num *= 10;
			n = (int)num;
			num -= n;
			tstr[tv] = n + '0';
		}
		return alignWidth(str,tstr,tv,width);
	}

	double strToDouble(const char*str)
	{
		int sign = 1,dot = 0,pos = 1;
		double d = 0.0;
		if(*str == '-'){
			sign = -1;
			str ++;
		}
		while(1){
			if(*str == '.' && dot == 0){
				dot = 1;
			}else if(*str >= '0' && *str <= '9'){
				if(dot == 1)pos *= 10;
				d = d * 10 + *str - '0';
			}
			else break;
			str ++;
		}
		d /= pos * sign;
		return d;
	}
	int hasElement(const char* sce,const char* tar,uint8_t end)
	{
		int i, j, k;
		for(i = 0; sce[i]; i++){
			if(*tar == *sce){
				for(j = 0,k = i; tar[j] && sce[k]; j++){
					if(tar[j] != sce[k])break;
					k++;
				}
				if(tar[j] == '\0'){
					if(sce[k] == '\0' || end == 0xFF || isseparate(sce[k],end))return i;
				}
			}
		}
		return -1;
	}
	int len(const char*tar)
	{
		int n = 0;
		if(tar == 0)return 0;
		while(*tar ++)n ++;
		return n; 
	}
	uint8_t startWith(const char*sce,const char*tar)
	{
		for(;*sce && *sce == *tar;sce ++,tar ++);
		return *tar == '\0';
	}
	uint8_t endWith(const char*sce,const char*tar)
	{
		int t = len(tar),s = len(sce);
		if(t > s)return 0;
		sce += s - t;
		for(;*sce && *sce == *tar;sce ++,tar ++);
		return *tar == '\0';
	}
	int cmp(const char*sce, const char*tar)
	{
		if (tar == 0 && sce == 0)return 0;
		if (tar == 0)return 1;
		if (sce == 0)return -1;
		while (*sce&&*tar){
			if (*sce > *tar)return 1;
			else if (*sce < *tar)return -1;
			sce++;
			tar++;
		}
		if (*sce > *tar)return 1;
		else if (*sce < *tar)return -1;
		return 0;
	}
	void cpy(char* tar,const char*sce)
	{
		while (*sce)*tar++ = *sce++; 
		*tar = 0;
	}
	void cat(char*tar,const char*source)
	{
		while(*tar)tar++;
		cpy(tar,source);
	}
	void memzero(void*target,int size)
	{
		uint8_t* tar = (uint8_t*)target;
		while(size > 0)
		{
			*tar = 0;
			tar ++;
			size --;
		}
	}
}

uint8_t useRomBuf = 0;
uint8_t rombuf1St,rombuf2St;
uint32_t romdataAddr = 0;
#define MAX_ROM_READ  512
uint8_t rombuf1[MAX_ROM_READ];
uint8_t rombuf2[MAX_ROM_READ];
int romdataPos = 0;
void romdataUpdate(void)
{
	if(useRomBuf == 0) {
		useRomBuf = 1;
		EROM_ReadStart(romdataAddr,rombuf1,MAX_ROM_READ);
		rombuf1St = 1;
		rombuf2St = 0;
		romdataAddr += MAX_ROM_READ;
	}
	else {
		if(rombuf1St == 1){
			if(EROM_ReadOk()){
				rombuf1St = 2;
			}
		}
		else if(rombuf1St == 0) {
			if(rombuf2St != 1) {
				EROM_ReadStart(romdataAddr,rombuf1,MAX_ROM_READ);
				romdataAddr += MAX_ROM_READ;
				rombuf1St = 1;
			}
		}
		if(rombuf2St == 1){
			if(EROM_ReadOk()){
				rombuf2St = 2;
			}
		}
		else if(rombuf2St == 0){
			if(rombuf1St != 1){
				EROM_ReadStart(romdataAddr,rombuf2,MAX_ROM_READ);
				romdataAddr += MAX_ROM_READ;
				rombuf2St = 1;
			}
		}
	}
}
uint8_t romdataGetOnebyte(void)
{
	if(useRomBuf == 1){
		do {
			romdataUpdate();
		}
		while(rombuf1St != 2);
		if(romdataPos >= MAX_ROM_READ){
			romdataPos = 0;
			useRomBuf = 2;
			while(rombuf2St == 1){
				romdataUpdate();
			}
			rombuf1St = 0;
			romdataUpdate();
			return rombuf2[romdataPos++];
		}
		else 
			return rombuf1[romdataPos++];
	}
	else{
		do {
			romdataUpdate();
		}while(rombuf2St != 2);
		if(romdataPos >= MAX_ROM_READ){
			romdataPos = 0;
			useRomBuf = 1;
			while(rombuf1St == 1){
				romdataUpdate();
			}
			rombuf2St = 0;
			romdataUpdate();
			return rombuf1[romdataPos++];
		}
		else 
			return rombuf2[romdataPos++];
	}
}
void romdataReset(uint32_t taddr)
{
	while(!EROM_ReadOk()){}
	romdataPos = 0;
	useRomBuf = 0;
	romdataAddr = taddr;
}

