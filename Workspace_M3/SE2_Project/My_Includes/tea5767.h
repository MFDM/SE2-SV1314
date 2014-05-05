#ifndef tea5767
#define tea5767

#define MASK_MUTE 0x80
#define MASK_SEARCH_MODE 0x40
#define MASK_PLL_HIGH 0x3F
#define MASK_PLL_LOW 0xFF
#define MASK_SSL 0x20
#define MASK_SEARCH_UP 0x80
#define MASK_SEARCH_DOWN 0x00
#define MASK_XTAL 0x10
#define MASK_PLL_REF 0x80
#define MASK_READY_FLAG 0x80
#define MASK_BAND_LIMIT_FLAG 0x40
#define MASK_READY_FLAG 0x80
#define MASK_STEREO 0x80
#define MASK_IF 0x7F
#define TEA_SIZE_OF_ARRAY 5
#define ADDR_RADIO 0x60
#define READ 1
#define WRITE 0
#define XTAL 32768
#define EUROPE_FREQUENCY_MAX 108
#define EUROPE_FREQUENCY_MIN 87.5

void TEA5767_Init();
void TEA5767_SearchUp(int freq);
void TEA5767_SearchDown(int freq);
unsigned int TEA5767_GetFrequency();
void TEA5767_SetFrequency(unsigned int RFrequency,unsigned int UPnDOWN);
#endif
