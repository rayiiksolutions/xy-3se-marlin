#ifndef Y_BOARD_H
#define Y_BOARD_H

#if T2_STM32F103
#include "0board/cxy_chitu_v6.h"
#elif T2_STM32F446
#include "0board/tronxy_f446.h"
#endif

#define LCD_USE_EROM
#define LCD_USE_TOUCH

#define EROM_W25QXX           1
#define EROM_AT24CXX          2
#define EROM_CHIP_TYPE        EROM_W25QXX

#ifdef LCD_USE_EROM
    #if EROM_CHIP_TYPE == EROM_W25QXX
    #include "3rom/yW25QxxApi.h"
    #include "3rom/yAt24cxx.hpp"
    #define EROM_Init            W25QxxInit
    #define EROM_ReadStart       W25QxxBatchRead
    #define EROM_ReadOneByte     W25QxxReadOnebyte
    #define EROM_ReadSyn         W25QxxBatchReadSyn
    #define EROM_ReadOk          W25QxxBatchReadOk
    #else
    #error You must define a external rom.
    #endif
#endif

#endif
