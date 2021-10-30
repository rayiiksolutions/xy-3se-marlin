#ifndef Y_TYPE_SZ
#define Y_TYPE_SZ

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef uint32_t
typedef unsigned long uint32_t;
#endif

typedef const char* CPSTR;
typedef char* PSTR;
#define	SEX(x,y,z)			(x) |= (uint32_t)(y) << (z)
#define	CLX(x,y,z)			(x) &= ~((uint32_t)(y) << (z))

#define Y(X)  *((volatile uint32_t *)(X))
#define	__YSZ_0(...)
#define	__YSZ_1(a1,...)	#a1
#define	__YSZ_2(a1,a2,...)	a1##a2
#define	__YSZ_3(a1,a2,a3,...)	a1##a2##a3
#define	__YSZ_4(a1,a2,a3,a4,...)	a1##a2##a3##a4
#define	__YSZ_5(a1,a2,a3,a4,a5,...)	a1##a2##a3##a4##a5
#define	__YSZ_6(a1,a2,a3,a4,a5,a6,...)	a1##a2##a3##a4##a5##a6
#define	_YSZ(x,...)				__YSZ_##x(__VA_ARGS__)
#define	__YSZ(n,...)			_YSZ(n,__VA_ARGS__)

#define	I16TOB(i16,byte2,byte1)							do{byte2=(uint8_t)((i16)>>8);byte1=(uint8_t)(i16);}while(0)
#define	I32TOB(i32,byte4,byte3,byte2,byte1)				do{byte4=(uint8_t)((i32)>>24);byte3=(uint8_t)((i32)>>16);I16TOB((i32),byte2,byte1);}while(0)
#define	BTOI16(i16,byte2,byte1)							i16=((uint16_t)(byte2)<<8)|(uint16_t)(byte1)
#define	BTOI32(i32,byte4,byte3,byte2,byte1)				i32=((uint32_t)(byte4)<<24)|((uint32_t)(byte3)<<16)|((uint32_t)(byte2)<<8)|(uint32_t)(byte1)
//小端模式
#define	S_I16TOA(i,b,index)		I16TOB(i,b[(index) + 1],b[index])
#define	S_I32TOA(i,b,index)		I32TOB(i,b[(index) + 3],b[(index) + 2],b[(index) + 1],b[index])
#define	S_ATOI16(i,b,index)		BTOI16(i,b[(index) + 1],b[index])
#define	S_ATOI32(i,b,index)		BTOI32(i,b[(index) + 3],b[(index) + 2],b[(index) + 1],b[index])
//大端模式
#define	B_I16TOA(i,b,index)		I16TOB(i,b[index],b[(index) + 1])
#define	B_I32TOA(i,b,index)		I32TOB(i,b[index],b[(index) + 1],b[(index) + 2],b[(index) + 3])
#define	B_ATOI16(i,b,index)		BTOI16(i,b[index],b[(index) + 1])
#define	B_ATOI32(i,b,index)		BTOI32(i,b[index],b[(index) + 1],b[(index) + 2],b[(index) + 3])

#endif
