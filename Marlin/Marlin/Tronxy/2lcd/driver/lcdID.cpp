#include "lcdID.h"

uint32_t getLcdID(void)
{
    LCD_WReg(0x00);//大多数屏的ID都在0号寄存器上
    uint32_t id,t;
    LCD_RData(id);
    if(id < 0xFF || id == 0xFFFF || id == 0X9300)//读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300
    {
        const uint16_t regaddr[] = {0x00D3,0x00D3,0x00BF,0x00D4,0x0004,};
        const uint16_t tarid[] = {0x9341,0x9488,0x6804,0x5310,0x8066,};
        const int num = sizeof(regaddr) / sizeof(regaddr[0]);
        for(int i = 0; i < num; i++){
            LCD_WReg(regaddr[i]);
            LCD_RData(id);//dummy read
            LCD_RData(id);//读到0x0000
            LCD_RData(id);//高8位
            LCD_RData(t);//低8位
            id = (id << 8) | t;
            if(id == tarid[i])return id;
        }
        LCD_WReg(0xDA00);
        LCD_RData(id);//读到0x0000
        LCD_WReg(0xDB00);
        LCD_RData(id);//读到高8位0x80
        LCD_WReg(0xDC00);
        LCD_RData(t);//低8位0x00
        id = (id << 8) | (t & 0xFF);
        if(id==0x8000)id=0x5510;
    }
    return id;
}
