#include "main.h"
#include "stm32f4xx_ll_spi.h"
#define M_PI 3.14159265358979323846
//https://github.com/yaroslav8765
/***************************************CONFIG PINS, SPI and DMA*************************************************/


#define LCD_RST_1 LL_GPIO_SetOutputPin	(	RES_GPIO_Port,RES_Pin	)			// LCD_RST = 1 , LCD RESET pin
#define LCD_RST_0 LL_GPIO_ResetOutputPin(	RES_GPIO_Port,RES_Pin	)			// LCD_RST = 0 , LCD RESET pin

#define LCD_CS_1 LL_GPIO_SetOutputPin		(	CS_GPIO_Port,CS_Pin		)			// LCD_CS = 1, LCD select pin
#define LCD_CS_0 LL_GPIO_ResetOutputPin	(	CS_GPIO_Port,CS_Pin		)			// LCD_CS = 0, LCD select pin

#define LCD_DC_1 LL_GPIO_SetOutputPin		(	DC_GPIO_Port,DC_Pin		)			// LCD_DC = 1, LCD Data/Command pin
#define LCD_DC_0 LL_GPIO_ResetOutputPin	(	DC_GPIO_Port,DC_Pin		)			// LCD_DC = 0,LCD Data/Command pin

#define SPI_NO SPI1																										//Which SPI do you use
#define DMA_NO DMA2																										//Which DMA do you use
#define DMA_STREAM LL_DMA_STREAM_3																		//Which DMA stream do you use

#define LL_DMA_ClearFlag_TC LL_DMA_ClearFlag_TC3											//Replace 3 with DMA stream you use
#define LL_DMA_ClearFlag_TE LL_DMA_ClearFlag_TE3
#define LL_DMA_IsActiveFlag_TC LL_DMA_IsActiveFlag_TC3
#define LL_DMA_IsActiveFlag_TE LL_DMA_IsActiveFlag_TE3
/****************************************************************************************************************/





#define swap(a,b) {int16_t t=a;a=b;b=t;}

#define LCD_W 240								//With of LCD
#define LCD_H 240								//Height of LCD
#define USE_HORIZONTAL 1  //Set the display direction 0,1,2,3	four directions


/*******************************************INIT DISPLAY DATA****************************************************/
#define GC9A01_RST_DELAY 120    ///< delay ms wait for reset finish
#define GC9A01_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define GC9A01_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define GC9A01_NOP 0x00
#define GC9A01_SWRESET 0x01
#define GC9A01_RDDID 0x04
#define GC9A01_RDDST 0x09

#define GC9A01_SLPIN 0x10
#define GC9A01_SLPOUT 0x11
#define GC9A01_PTLON 0x12
#define GC9A01_NORON 0x13

#define GC9A01_INVOFF 0x20
#define GC9A01_INVON 0x21
#define GC9A01_DISPOFF 0x28
#define GC9A01_DISPON 0x29

#define GC9A01_CASET 0x2A
#define GC9A01_RASET 0x2B
#define GC9A01_RAMWR 0x2C
#define GC9A01_RAMRD 0x2E

#define GC9A01_PTLAR 0x30
#define GC9A01_COLMOD 0x3A
#define GC9A01_MADCTL 0x36

#define GC9A01_MADCTL_MY 0x80
#define GC9A01_MADCTL_MX 0x40
#define GC9A01_MADCTL_MV 0x20
#define GC9A01_MADCTL_ML 0x10
#define GC9A01_MADCTL_RGB 0x00

#define GC9A01_RDID1 0xDA
#define GC9A01_RDID2 0xDB
#define GC9A01_RDID3 0xDC
#define GC9A01_RDID4 0xDD
/*******************************************INIT DISPLAY DATA END*************************************************/




/***************************************************COLORS********************************************************/
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0xF81F
#define GRED             0xFFE0
#define GBLUE            0x07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0xBC40
#define BRRED            0xFC07
#define GRAY             0x8430
#define DARKBLUE         0x01CF
#define LIGHTBLUE        0x7D7C
#define GRAYBLUE         0x5458
#define LIGHTGREEN       0x841F
#define LGRAY            0xC618
#define LGRAYBLUE        0xA651
#define LBBLUE           0x2B12
// Colors of the rainbow
#define VIOLET           0x8010
#define INDIGO           0x4810
#define RAINBOW_BLUE     0x001F
#define RAINBOW_GREEN    0x07E0
#define RAINBOW_YELLOW   0xFEC0
#define RAINBOW_ORANGE   0xFD20
#define RAINBOW_RED      0xF800
// Additional colors
#define ORANGE           0xFD20
#define PINK             0xF97F
#define GOLD             0xFEA0
#define SILVER           0xC618
#define PURPLE           0x780F
#define TURQUOISE        0x471A
#define MINT             0xB7E0
#define PEACH            0xF9E1
#define BEIGE            0xF7BB
#define OLIVE            0x8400
#define CORAL            0xFBEA
#define AQUA             0x07FF

/********************TEXT STYLE**********************/
#define default_text_color BLACK
#define default_background_color WHITE
#define default_active_color GREEN
#define default_font &Font16
/****************************************************/

/********************MENU PARAMETRS******************/
#define X_POS 50
#define Y_POS 25
#define X_END_POS 240
#define STEP 6
#define AMOUNT_OF_MENU_MEMBERS 5
/****************************************************/

/********************TEXT POSITION*******************/
#define OUT_TEXT_LEFT_INDENTATION 35
#define OUT_TEXT_RIGHT_INDENTATION 35
/****************************************************/



typedef enum
{
	active,
	unactive
}states;

struct MenuMember {
    char text[50];
    uint8_t number;
		states state;
};

typedef enum
{
	red, 
	orange, 
	yellow, 
	green,
	blue, 
	pink,
	violet,
	indigo
}colors;

typedef struct
{
	uint16_t TextColor;
	uint16_t BackColor;
	sFONT *pFont;
}GC9A01_DrawPropTypeDef;

void SPI_write(uint8_t data);
void GC9A01_Write_Cmd_Data (uint8_t CMDP);
void GC9A01_Write_Cmd(uint8_t CMD);
void GC9A01_Write_Data_U16(uint16_t y);
void GC9A01_Write_Data(uint8_t DH,uint8_t DL);
void GC9A01_Write_Bytes(uint8_t * pbuff, uint16_t size);
void GC9A01_Initial(void);
void GC9A01_ClearScreen(uint16_t bColor);
void GC9A01_ClearWindow(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint16_t bColor);
void GC9A01_SetPos(uint8_t Xstart, uint8_t Ystart, uint8_t Xend, uint8_t Yend);
void GC9A01_show_picture(uint16_t *picture, uint16_t x,uint16_t y, uint8_t widht, uint8_t height);
void GC9A01_DrawPixel(uint8_t x, uint8_t y, uint16_t color);
void GC9A01_draw_line(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t thickness);
void GC9A01_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void GC9A01_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

void DrawArrow(int16_t angle, uint8_t lineLen, uint8_t thick, uint16_t color);
void DrawLineAroundTheCircle(int16_t angle, uint8_t r1,uint8_t r2, uint8_t thick, uint16_t color);
void GC9A01A_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void GC9A01_Draw_Triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint16_t color);
void GC9A01_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);

void DrawTriangleArrow(	int16_t 	angle, 								uint8_t lineLen, 			uint8_t thick,													\
												uint16_t 	color, 								uint8_t base_lenght, 	uint16_t back_tail_angle,								\
												uint8_t 	imaginable_circle_R);

void GC9A01_FillCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

void GC9A01_DrawCircleArountTheCircle(uint16_t start_pos,	int16_t angle, 																							\
																			uint8_t r,					uint8_t distance_to_center, uint16_t color);

void GC9A01_GradientScale(uint32_t min_value,uint32_t max_value, uint32_t value);
void GC9A01_FillCircleFast(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

void GC9A01_SetTextColor(uint16_t color);
void GC9A01_SetBackColor(uint16_t color);
void GC9A01_SetFont(sFONT *pFonts);
void GC9A01_DrawChar(uint16_t x, uint16_t y, uint8_t c);
void GC9A01_String(uint16_t x,uint16_t y, char *str);
void GC9A01_Text(char *str,uint8_t page);
void GC9A01_Rainbow_String(uint16_t x,uint16_t y, char *str);
void ShowMenu(struct MenuMember Members[AMOUNT_OF_MENU_MEMBERS - 1],uint8_t page_num);
void refresh_menu_member(struct MenuMember Members, uint8_t pos);
uint8_t get_active_menu_member(struct MenuMember Members[AMOUNT_OF_MENU_MEMBERS]);
uint8_t chek_menu_member_for_the_file_type(struct MenuMember Member, char *str);
void menu_active_member_running_text_animation(struct MenuMember Members[AMOUNT_OF_MENU_MEMBERS], uint8_t pos);

