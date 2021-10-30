
#include "yCompress.h"
#include <string.h>
#define MINOFFSET 2
#define UNCONDITIONAL_MATCHLEN 6
#define UNCOMPRESSED_END 4
#define CWORD_LEN 4
#define COMPRESS_POINTERS 16
#define COMPRESS_HASH_VALUES 4096

typedef struct 
{
	const unsigned char *offset[COMPRESS_POINTERS];
}CompressHash;
typedef struct
{
	CompressHash hash[COMPRESS_HASH_VALUES];
	unsigned char hash_counter[COMPRESS_HASH_VALUES];
}CompressSt;
 
inline unsigned int hash_func(unsigned int i)
{
	return ((i >> 12) ^ i) & (COMPRESS_HASH_VALUES - 1);
}
 
static inline unsigned int hashat(const unsigned char *src)
{
	unsigned int fetch, hash;
	fetch = buffToInt(src, 3);
	hash = hash_func(fetch);
	return hash;
}

unsigned int _decompress(const void *source, void *destination,int decompressSize)
{
	unsigned int cword_val = 1,fetch;
	const unsigned char *src = (const unsigned char*)source;
	unsigned char *dst = (unsigned char *)destination;
	const unsigned char *last_destination_byte = (unsigned char *)destination + decompressSize - 1;
	const unsigned char *last_matchstart = last_destination_byte - UNCONDITIONAL_MATCHLEN - UNCOMPRESSED_END;
	static const unsigned int bitlut[16] = {4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};
	while(1)
	{
		if (cword_val == 1)
		{
			cword_val = buffToInt(src, CWORD_LEN);
			src += CWORD_LEN;
		}
		fetch = buffToInt(src, CWORD_LEN);
 
		if ((cword_val & 1) == 1)
		{
			unsigned int matchlen,offset;
			const unsigned char *offset2;
			cword_val >>= 1;
			if ((fetch & 3) == 0)
			{
				offset = (fetch & 0xff) >> 2;
				matchlen = 3;
				src++;
			}
			else if ((fetch & 2) == 0)
			{
				offset = (fetch & 0xffff) >> 2;
				matchlen = 3;
				src += 2;
			}
			else if ((fetch & 1) == 0)
			{
				offset = (fetch & 0xffff) >> 6;
				matchlen = ((fetch >> 2) & 15) + 3;
				src += 2;
			}
			else if ((fetch & 127) != 3)
			{
				offset = (fetch >> 7) & 0x1ffff;
				matchlen = ((fetch >> 2) & 0x1f) + 2;
				src += 3;
			}
			else
			{
				offset = (fetch >> 15);
				matchlen = ((fetch >> 7) & 255) + 3;
				src += 4;
			}
 
			offset2 = dst - offset;
			for(unsigned int i = 0;i < matchlen; i++)dst[i] = offset2[i];
			dst += matchlen;
		}
		else
		{
			if (dst < last_matchstart)
			{
				unsigned int n = bitlut[cword_val & 0xf];
				for(int i = 0;i < 4; i++)dst[i] = src[i];
				cword_val >>= n;
				dst += n;
				src += n;
			}
			else
			{			
				while(dst <= last_destination_byte)
				{
					if (cword_val == 1)
					{
						src += CWORD_LEN;
						cword_val = 1U << 31;
					}
					*dst = *src;
					dst++;
					src++;
					cword_val >>= 1;
				}
				break;
			}
		}
	}
	return (unsigned int)dst - (unsigned int)destination;
}
