#include "main.h"


//https://github.com/yaroslav8765
volatile GC9A01_DrawPropTypeDef lcdprop;
extern volatile uint8_t dma_spi_fl1;
volatile uint8_t shift = 0;
volatile colors current_text_color;
uint16_t gradient_scale_current_pos = 0;
/************************************basic display fuctions start************************************/
#pragma push
#pragma O0

void SPI_write(uint8_t data) {
	while(!LL_SPI_IsActiveFlag_TXE(SPI_NO)) {}
  LL_SPI_TransmitData8 (SPI_NO, data);
  while(!LL_SPI_IsActiveFlag_RXNE(SPI_NO)) {}
  LL_SPI_ReceiveData8(SPI_NO);
}


void GC9A01_Write_Cmd_Data (uint8_t CMDP)
{
	LCD_CS_0;
	LCD_DC_1;
	
	SPI_write(CMDP);

	LCD_CS_1;
}

void GC9A01_Write_Cmd(uint8_t CMD)
{
	LCD_CS_0;
	LCD_DC_0;

	SPI_write(CMD);

	LCD_CS_1;
}
#pragma pop

void GC9A01_Write_Data_U16(uint16_t y)
{
	volatile uint16_t m,n;
	m=y>>8;
	n=y;
	
	GC9A01_Write_Data(m,n);
}

void GC9A01_Write_Data(uint8_t DH,uint8_t DL)
{
  LCD_CS_0;
  LCD_DC_1;

  SPI_write(DH);
  SPI_write(DL);

	LCD_CS_1;
}


void GC9A01_Write_Bytes(uint8_t * pbuff, uint16_t size)
{
  LCD_CS_0;
 	LCD_DC_1;
	dma_spi_fl1=0;
	LL_DMA_DisableStream(DMA_NO, DMA_STREAM);
	LL_DMA_SetDataLength(DMA_NO, DMA_STREAM, size);
	LL_DMA_EnableIT_TC(DMA_NO, DMA_STREAM);
	LL_DMA_ClearFlag_TC(DMA_NO);
	LL_DMA_ClearFlag_TE(DMA_NO);
	LL_DMA_ConfigAddresses(	DMA_NO, DMA_STREAM, 																	\
													(uint32_t)pbuff, 																				\
													LL_SPI_DMA_GetRegAddr(SPI_NO),														\
													LL_DMA_GetDataTransferDirection(DMA_NO, DMA_STREAM)	\
												);	
	LL_DMA_EnableStream(DMA_NO, DMA_STREAM);
	while (!dma_spi_fl1) {  __asm__ volatile("nop");};
	LL_DMA_DisableStream(DMA_NO, DMA_STREAM);
			
	dma_spi_fl1 = 0;

	LCD_CS_1;
}


void GC9A01_Initial(void)
{
  LCD_CS_1;
	LCD_RST_0;
	LCD_RST_1;

	GC9A01_Write_Cmd(0xEF);

	GC9A01_Write_Cmd(0xEB);
	GC9A01_Write_Cmd_Data(0x14);

  GC9A01_Write_Cmd(0xFE);
	GC9A01_Write_Cmd(0xEF);

	GC9A01_Write_Cmd(0xEB);
	GC9A01_Write_Cmd_Data(0x14);

	GC9A01_Write_Cmd(0x84);
	GC9A01_Write_Cmd_Data(0x40);

	GC9A01_Write_Cmd(0x85);
	GC9A01_Write_Cmd_Data(0xFF);

	GC9A01_Write_Cmd(0x86);
	GC9A01_Write_Cmd_Data(0xFF);

	GC9A01_Write_Cmd(0x87);
	GC9A01_Write_Cmd_Data(0xFF);

	GC9A01_Write_Cmd(0x88);
	GC9A01_Write_Cmd_Data(0x0A);

	GC9A01_Write_Cmd(0x89);
	GC9A01_Write_Cmd_Data(0x21);

	GC9A01_Write_Cmd(0x8A);
	GC9A01_Write_Cmd_Data(0x00);

	GC9A01_Write_Cmd(0x8B);
	GC9A01_Write_Cmd_Data(0x80);

	GC9A01_Write_Cmd(0x8C);
	GC9A01_Write_Cmd_Data(0x01);

	GC9A01_Write_Cmd(0x8D);
	GC9A01_Write_Cmd_Data(0x01);

	GC9A01_Write_Cmd(0x8E);
	GC9A01_Write_Cmd_Data(0xFF);

	GC9A01_Write_Cmd(0x8F);
	GC9A01_Write_Cmd_Data(0xFF);


	GC9A01_Write_Cmd(0xB6);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x00);

	GC9A01_Write_Cmd(0x36);

	if(USE_HORIZONTAL==0)GC9A01_Write_Cmd_Data(0x18);
	else if(USE_HORIZONTAL==1)GC9A01_Write_Cmd_Data(0x28);
	else if(USE_HORIZONTAL==2)GC9A01_Write_Cmd_Data(0x48);
	else GC9A01_Write_Cmd_Data(0x88);

	GC9A01_Write_Cmd(0x3A);
	GC9A01_Write_Cmd_Data(0x05);


	GC9A01_Write_Cmd(0x90);
	GC9A01_Write_Cmd_Data(0x08);
	GC9A01_Write_Cmd_Data(0x08);
	GC9A01_Write_Cmd_Data(0x08);
	GC9A01_Write_Cmd_Data(0x08);

	GC9A01_Write_Cmd(0xBD);
	GC9A01_Write_Cmd_Data(0x06);

	GC9A01_Write_Cmd(0xBC);
	GC9A01_Write_Cmd_Data(0x00);

	GC9A01_Write_Cmd(0xFF);
	GC9A01_Write_Cmd_Data(0x60);
	GC9A01_Write_Cmd_Data(0x01);
	GC9A01_Write_Cmd_Data(0x04);

	GC9A01_Write_Cmd(0xC3);
	GC9A01_Write_Cmd_Data(0x13);
	GC9A01_Write_Cmd(0xC4);
	GC9A01_Write_Cmd_Data(0x13);

	GC9A01_Write_Cmd(0xC9);
	GC9A01_Write_Cmd_Data(0x22);

	GC9A01_Write_Cmd(0xBE);
	GC9A01_Write_Cmd_Data(0x11);

	GC9A01_Write_Cmd(0xE1);
	GC9A01_Write_Cmd_Data(0x10);
	GC9A01_Write_Cmd_Data(0x0E);

	GC9A01_Write_Cmd(0xDF);
	GC9A01_Write_Cmd_Data(0x21);
	GC9A01_Write_Cmd_Data(0x0c);
	GC9A01_Write_Cmd_Data(0x02);

	GC9A01_Write_Cmd(0xF0);
  GC9A01_Write_Cmd_Data(0x45);
  GC9A01_Write_Cmd_Data(0x09);
 	GC9A01_Write_Cmd_Data(0x08);
	GC9A01_Write_Cmd_Data(0x08);
 	GC9A01_Write_Cmd_Data(0x26);
 	GC9A01_Write_Cmd_Data(0x2A);

 	GC9A01_Write_Cmd(0xF1);
 	GC9A01_Write_Cmd_Data(0x43);
 	GC9A01_Write_Cmd_Data(0x70);
 	GC9A01_Write_Cmd_Data(0x72);
 	GC9A01_Write_Cmd_Data(0x36);
 	GC9A01_Write_Cmd_Data(0x37);
 	GC9A01_Write_Cmd_Data(0x6F);


 	GC9A01_Write_Cmd(0xF2);
 	GC9A01_Write_Cmd_Data(0x45);
 	GC9A01_Write_Cmd_Data(0x09);
 	GC9A01_Write_Cmd_Data(0x08);
 	GC9A01_Write_Cmd_Data(0x08);
 	GC9A01_Write_Cmd_Data(0x26);
 	GC9A01_Write_Cmd_Data(0x2A);

 	GC9A01_Write_Cmd(0xF3);
 	GC9A01_Write_Cmd_Data(0x43);
 	GC9A01_Write_Cmd_Data(0x70);
 	GC9A01_Write_Cmd_Data(0x72);
 	GC9A01_Write_Cmd_Data(0x36);
 	GC9A01_Write_Cmd_Data(0x37);
 	GC9A01_Write_Cmd_Data(0x6F);

	GC9A01_Write_Cmd(0xED);
	GC9A01_Write_Cmd_Data(0x1B);
	GC9A01_Write_Cmd_Data(0x0B);

	GC9A01_Write_Cmd(0xAE);
	GC9A01_Write_Cmd_Data(0x77);

	GC9A01_Write_Cmd(0xCD);
	GC9A01_Write_Cmd_Data(0x63);


	GC9A01_Write_Cmd(0x70);
	GC9A01_Write_Cmd_Data(0x07);
	GC9A01_Write_Cmd_Data(0x07);
	GC9A01_Write_Cmd_Data(0x04);
	GC9A01_Write_Cmd_Data(0x0E);
	GC9A01_Write_Cmd_Data(0x0F);
	GC9A01_Write_Cmd_Data(0x09);
	GC9A01_Write_Cmd_Data(0x07);
	GC9A01_Write_Cmd_Data(0x08);
	GC9A01_Write_Cmd_Data(0x03);

	GC9A01_Write_Cmd(0xE8);
	GC9A01_Write_Cmd_Data(0x34);

	GC9A01_Write_Cmd(0x62);
	GC9A01_Write_Cmd_Data(0x18);
	GC9A01_Write_Cmd_Data(0x0D);
	GC9A01_Write_Cmd_Data(0x71);
	GC9A01_Write_Cmd_Data(0xED);
	GC9A01_Write_Cmd_Data(0x70);
	GC9A01_Write_Cmd_Data(0x70);
	GC9A01_Write_Cmd_Data(0x18);
	GC9A01_Write_Cmd_Data(0x0F);
	GC9A01_Write_Cmd_Data(0x71);
	GC9A01_Write_Cmd_Data(0xEF);
	GC9A01_Write_Cmd_Data(0x70);
	GC9A01_Write_Cmd_Data(0x70);

	GC9A01_Write_Cmd(0x63);
	GC9A01_Write_Cmd_Data(0x18);
	GC9A01_Write_Cmd_Data(0x11);
	GC9A01_Write_Cmd_Data(0x71);
	GC9A01_Write_Cmd_Data(0xF1);
	GC9A01_Write_Cmd_Data(0x70);
	GC9A01_Write_Cmd_Data(0x70);
	GC9A01_Write_Cmd_Data(0x18);
	GC9A01_Write_Cmd_Data(0x13);
	GC9A01_Write_Cmd_Data(0x71);
	GC9A01_Write_Cmd_Data(0xF3);
	GC9A01_Write_Cmd_Data(0x70);
	GC9A01_Write_Cmd_Data(0x70);

	GC9A01_Write_Cmd(0x64);
	GC9A01_Write_Cmd_Data(0x28);
	GC9A01_Write_Cmd_Data(0x29);
	GC9A01_Write_Cmd_Data(0xF1);
	GC9A01_Write_Cmd_Data(0x01);
	GC9A01_Write_Cmd_Data(0xF1);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x07);

	GC9A01_Write_Cmd(0x66);
	GC9A01_Write_Cmd_Data(0x3C);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0xCD);
	GC9A01_Write_Cmd_Data(0x67);
	GC9A01_Write_Cmd_Data(0x45);
	GC9A01_Write_Cmd_Data(0x45);
	GC9A01_Write_Cmd_Data(0x10);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x00);

	GC9A01_Write_Cmd(0x67);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x3C);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x01);
	GC9A01_Write_Cmd_Data(0x54);
	GC9A01_Write_Cmd_Data(0x10);
	GC9A01_Write_Cmd_Data(0x32);
	GC9A01_Write_Cmd_Data(0x98);

	GC9A01_Write_Cmd(0x74);
	GC9A01_Write_Cmd_Data(0x10);
	GC9A01_Write_Cmd_Data(0x85);
	GC9A01_Write_Cmd_Data(0x80);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x00);
	GC9A01_Write_Cmd_Data(0x4E);
	GC9A01_Write_Cmd_Data(0x00);

  GC9A01_Write_Cmd(0x98);
	GC9A01_Write_Cmd_Data(0x3e);
	GC9A01_Write_Cmd_Data(0x07);

	GC9A01_Write_Cmd(0x35);
	GC9A01_Write_Cmd(0x21);

	GC9A01_Write_Cmd(0x11);
	GC9A01_Write_Cmd(0x29);
}

void GC9A01_SetPos(uint8_t Xstart, uint8_t Ystart, uint8_t Xend, uint8_t Yend)
{
	GC9A01_Write_Cmd(0x2A);  
	GC9A01_Write_Cmd_Data((Xstart >> 8) & 0xFF);
	GC9A01_Write_Cmd_Data(Xstart & 0xFF);
  GC9A01_Write_Cmd_Data((Xend >> 8) & 0xFF);
  GC9A01_Write_Cmd_Data(Xend & 0xFF);

  GC9A01_Write_Cmd(0x2B);  
  GC9A01_Write_Cmd_Data((Ystart >> 8) & 0xFF);
  GC9A01_Write_Cmd_Data(Ystart & 0xFF);
  GC9A01_Write_Cmd_Data((Yend >> 8) & 0xFF);
  GC9A01_Write_Cmd_Data(Yend & 0xFF);

  GC9A01_Write_Cmd(0x2C);
}

/*****************************basic display functions end****************************************/






void GC9A01_ClearScreen(uint16_t bColor)
{
	GC9A01_ClearWindow(0,0,LCD_W,LCD_H,bColor);
}


void GC9A01_ClearWindow(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint16_t bColor)
{
    uint32_t i;

    uint8_t hb = (bColor & 0xFFFF) >> 8;
    uint8_t lb = bColor & 0xFF;
    uint16_t tempColor = lb * 256 + hb;

    uint32_t totalSize = (endX - startX) * (endY - startY) * 2;
    uint32_t bufSize = 256;
    uint32_t loopNum = (totalSize - (totalSize % bufSize)) / bufSize;
    uint32_t modNum = totalSize % bufSize;

    uint16_t tempBuf[bufSize];
    uint8_t *ptempBuf;

    for (i = 0; i < bufSize; i++) {
        tempBuf[i] = tempColor;
    }

    GC9A01_SetPos(startX, startY, endX - 1, endY - 1);

    ptempBuf = (uint8_t *)tempBuf;
    for (i = 0; i < loopNum; i++) {
        GC9A01_Write_Bytes(ptempBuf, bufSize);
    }

    if (modNum > 0) {
        GC9A01_Write_Bytes(ptempBuf, modNum);
    }
}


 void GC9A01_inversPicData(uint16_t *picture, uint8_t x, uint8_t y){
 	uint16_t i;
 	uint8_t hb, lb;

 	for(i=0; i<y*x; i++){
 			hb = picture[i] >> 8;
 			lb = picture[i] & 0xFF;
 			picture[i] = lb * 256 + hb;
 	}
 }
 
void GC9A01_show_picture(uint16_t *picture, uint16_t x, uint16_t y, uint8_t width, uint8_t height)
{
    int32_t i;
    uint8_t *pPic;
    uint32_t totalSize = width * height * 2;
    uint32_t bufSize = 512;

    uint32_t loopNum = (totalSize - (totalSize % bufSize)) / bufSize;
    uint32_t modNum = totalSize % bufSize;

    GC9A01_SetPos(x, y, (x + width) - 1, (y + height) - 1);
    GC9A01_inversPicData(picture, width, height);

    pPic = (uint8_t *)picture;

    for (i = 0; i < loopNum; i++) {
        GC9A01_Write_Bytes(pPic + i * bufSize, bufSize);
    }
    GC9A01_Write_Bytes(pPic + i * bufSize, modNum);
    GC9A01_inversPicData(picture, width, height);

    return;
}

 
void GC9A01_DrawPixel(uint8_t x, uint8_t y, uint16_t color)
{
    uint16_t i;
    uint8_t hb = (color & 0xFFFF) >> 8;
    uint8_t lb = color & 0xFF;
    uint16_t tempColor = lb * 256 + hb;

    uint32_t bufSize = 4;

    uint16_t tempBuf[bufSize];
    uint8_t *ptempBuf;

    for (i = 0; i < bufSize; i++) {
        tempBuf[i] = tempColor;
    }

    GC9A01_SetPos(x, y, x , y );

    ptempBuf = (uint8_t *)tempBuf;
    GC9A01_Write_Bytes(ptempBuf, 8);
}


void GC9A01_draw_line(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t thickness) {
    int16_t steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        swap(x1, y1);
        swap(x2, y2);
    }

    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    int16_t dx = x2 - x1;
    int16_t dy = abs(y2 - y1);
    int16_t err = dx - dy / 2;
    int16_t ystep = (y1 < y2) ? 1 : -1;

    for (; x1 <= x2; x1++) {
        for (int i = -thickness / 2; i <= thickness / 2; i++) {
            if (steep)
                GC9A01_DrawPixel(y1 + i, x1, color);
            else
                GC9A01_DrawPixel(x1, y1 + i, color);
        }

        err -= dy;
        if (err < 0) {
            y1 += ystep;
            err += dx;
        }
    }
}




void GC9A01_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	GC9A01_draw_line(color,x1,y1,x2,y1,1);
	GC9A01_draw_line(color,x2,y1,x2,y2,1);
	GC9A01_draw_line(color,x1,y1,x1,y2,1);
	GC9A01_draw_line(color,x1,y2,x2,y2,1);
}

void GC9A01_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int16_t f = 1-r;
	int16_t ddF_x=1;
	int16_t ddF_y=-2*r;
	int16_t x = 0;
	int16_t y = r;
	GC9A01_DrawPixel(x0,y0+r,color);
	GC9A01_DrawPixel(x0,y0-r,color);
	GC9A01_DrawPixel(x0+r,y0,color);
	GC9A01_DrawPixel(x0-r,y0,color);
	while (x<y)
	{
		if (f>=0)
		{
			y--;
			ddF_y+=2;
			f+=ddF_y;
		}
		x++;
		ddF_x+=2;
		f+=ddF_x;
		GC9A01_DrawPixel(x0+x,y0+y,color);
		GC9A01_DrawPixel(x0-x,y0+y,color);
		GC9A01_DrawPixel(x0+x,y0-y,color);
		GC9A01_DrawPixel(x0-x,y0-y,color);
		GC9A01_DrawPixel(x0+y,y0+x,color);
		GC9A01_DrawPixel(x0-y,y0+x,color);
		GC9A01_DrawPixel(x0+y,y0-x,color);
		GC9A01_DrawPixel(x0-y,y0-x,color);
	}
}

void GC9A01_FillCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int16_t f = 1-r;
	int16_t ddF_x=1;
	int16_t ddF_y=-2*r;
	int16_t x = 0;
	int16_t y = r;
	GC9A01_DrawPixel(x0,y0+r,color);
	GC9A01_DrawPixel(x0,y0-r,color);
	GC9A01_DrawPixel(x0+r,y0,color);
	GC9A01_DrawPixel(x0-r,y0,color);
	GC9A01_draw_line(color,x0+r,y0,x0-r,y0,1);

	while (x<y)
	{
		if (f>=0)
		{
			y--;
			ddF_y+=2;
			f+=ddF_y;
		}
		x++;
		ddF_x+=2;
		f+=ddF_x;
		GC9A01_DrawPixel(x0+x,y0+y,color);	//DOWN RIGHT
		GC9A01_DrawPixel(x0-x,y0+y,color);	//DOWN LEFT
		GC9A01_draw_line(color,x0+x,y0+y,x0-x,y0+y,1);

		GC9A01_DrawPixel(x0+x,y0-y,color);	//TOP RIGHT
		GC9A01_DrawPixel(x0-x,y0-y,color);	//TOP lEFT
		GC9A01_draw_line(color,x0+x,y0-y,x0-x,y0-y,1);
		
		GC9A01_DrawPixel(x0+y,y0+x,color);	//RIGHT DOWN
		GC9A01_DrawPixel(x0-y,y0+x,color);	//LEFT DOWN
		GC9A01_draw_line(color,x0+y,y0+x,x0-y,y0+x,1);

		GC9A01_DrawPixel(x0+y,y0-x,color);	//TOP RIGHT
		GC9A01_DrawPixel(x0-y,y0-x,color);	//TOP LEFT
		GC9A01_draw_line(color,x0+y,y0-x,x0-y,y0-x,1);

	}
}

void GC9A01_FillCircleFast(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int16_t f = 1-r;
	int16_t ddF_x=1;
	int16_t ddF_y=-2*r;
	int16_t x = 0;
	int16_t y = r;
	GC9A01_ClearWindow(x0-r,y0,x0+r+1,y0+1,color);
	while (x<y)
	{
		if (f>=0)
		{
			y--;
			ddF_y+=2;
			f+=ddF_y;
		}
		x++;
		ddF_x+=2;
		f+=ddF_x;
		GC9A01_ClearWindow(x0-x,y0+y,x0+x+1,y0+y+1,	color);
		GC9A01_ClearWindow(x0-x,y0-y,x0+x+1,(y0-y)+1 ,color);
		GC9A01_ClearWindow(x0-y,y0+x,x0+y+1,y0+x+1,color);
		GC9A01_ClearWindow(x0-y,y0-x,x0+y+1,(y0-x)+1,color);
	}
}

void GC9A01_Draw_Triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint16_t color){
	GC9A01_draw_line(color, x1,y1,x2,y2,1);
	GC9A01_draw_line(color, x1,y1,x3,y3,1);
	GC9A01_draw_line(color, x3,y3,x2,y2,1);

}

void GC9A01A_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	if ((w <= 0) || (h <= 0) || (x >= LCD_W) || (y >= LCD_H))
		return;

	if (x < 0) {
		w += x;
		x = 0;
	}
	if (y < 0) {
		h += y;
		y = 0;
	}

	if ((w <= 0) || (h <= 0))
		return;

	if ((x + w) > LCD_W)
		w = LCD_W - x;
	if ((y + h) > LCD_H)
		h = LCD_H - y;

	for (uint16_t row = 0; row < h; row++) {
		for (uint16_t col = 0; col < w; col++)
		GC9A01_DrawPixel(x+col, y+row, color);
	}
}

void SwapInt16Values(int16_t *pValue1, int16_t *pValue2) {
	int16_t TempValue = *pValue1;
	*pValue1 = *pValue2;
	*pValue2 = TempValue;
}

void GC9A01_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	int16_t x2, int16_t y2, uint16_t color) {
	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		SwapInt16Values(&y0, &y1);
		SwapInt16Values(&x0, &x1);
	}
	if (y1 > y2) {
		SwapInt16Values(&y2, &y1);
		SwapInt16Values(&x2, &x1);
	}
	if (y0 > y1) {
		SwapInt16Values(&y0, &y1);
		SwapInt16Values(&x0, &x1);
	}

	if (y0 == y2)	  // Handle awkward all-on-same-line case as its own thing
			{
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;

		GC9A01A_FillRect(a, y0, b - a + 1, 1, color);
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
			dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if (a > b)
			SwapInt16Values(&a, &b);

		GC9A01A_FillRect(a, y, b - a + 1, 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t) dx12 * (y - y1);
	sb = (int32_t) dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if (a > b)
			SwapInt16Values(&a, &b);

		GC9A01A_FillRect(a, y, b - a + 1, 1, color);
	}
}

void DrawArrow(int16_t angle, uint8_t lineLen, uint8_t thick, uint16_t color) {
		
		angle -= 180;
    float angleRad = angle * M_PI / 180.0;
    int x = round(cos(angleRad) * lineLen) + 120;
    int y = round(sin(angleRad) * lineLen) + 120;

    GC9A01_draw_line(color, x , y , 120 , 120 ,thick);

}

void DrawTriangleArrow(	int16_t 	angle, 								uint8_t lineLen, 			uint8_t thick,													\
												uint16_t 	color, 								uint8_t base_lenght, 	uint16_t back_tail_angle,								\
												uint8_t 	imaginable_circle_R) {
	
		angle -= 180;
		base_lenght+=imaginable_circle_R;
    float angleRad = angle * M_PI / 180.0;
    int x3 = round(cos(angleRad) * lineLen) + 120;
    int y3 = round(sin(angleRad) * lineLen) + 120;
		angle-=back_tail_angle;
		angleRad = angle * M_PI / 180.0;
		int x1 = round(cos(angleRad) * base_lenght) + 120;
    int y1 = round(sin(angleRad) * base_lenght) + 120;
		angle+=2*back_tail_angle;
		angleRad = angle * M_PI / 180.0;
		int x2 = round(cos(angleRad) * base_lenght) + 120;
    int y2 = round(sin(angleRad) * base_lenght) + 120;
    GC9A01_FillTriangle( x1,y1,x2,y2, x3 , y3 ,color);


}

void DrawLineAroundTheCircle(int16_t angle, uint8_t r1,uint8_t r2, uint8_t thick, uint16_t color) {
		angle -= 180;
    float angleRad = angle * M_PI / 180.0;
    int x1 = round(cos(angleRad) * (120-r1)) + 120;
    int y1 = round(sin(angleRad) * (120-r1)) + 120;
		int x2 = round(cos(angleRad) * (120 - r2)) + 120;
    int y2 = round(sin(angleRad) * (120 - r2)) + 120;
    GC9A01_draw_line(color, x1, y1, x2, y2,thick);
 
}

void GC9A01_DrawCircleArountTheCircle(uint16_t start_pos,int16_t angle,uint8_t r,uint8_t distance_to_center, uint16_t color){
	angle -= 180 + (360 - start_pos);
  float angleRad = angle * M_PI / 180.0;
  int x = round(cos(angleRad) * ( distance_to_center)) + 120;
  int y = round(sin(angleRad) * ( distance_to_center)) + 120;
	GC9A01_FillCircleFast(x,y,r,color);

}

void GC9A01_GradientScale(uint32_t min_value,uint32_t max_value, uint32_t value){
	uint8_t r_start = 3, g_start = 25, b_start = 31;
	uint8_t r_end = 31, g_end = 0, b_end = 0;
	uint8_t r, g, b;
  uint16_t color;
	color = (r << 11) | (g << 5) | b;
	uint16_t start_angle = 330;
	uint16_t end_angle = 210;
	uint16_t diff = abs(start_angle - end_angle);
	uint16_t total_degrees = end_angle + (360 - start_angle);
	
	/*******text********/
	float step =  (float)total_degrees/ (max_value - min_value);
	uint16_t angle = (uint16_t)roundf(step * value);
	char str[50];
	sprintf(str,"%dC",value);
	GC9A01_ClearWindow(120 - (float)((4 * lcdprop.pFont->Width)/2),107, 																								\
	(120 - (float)((4 * lcdprop.pFont->Width)/2)) +(4 * lcdprop.pFont->Width),107 + lcdprop.pFont->Height,BLACK);
	GC9A01_String(120 - (float)((strlen(str) * lcdprop.pFont->Width)/2),107 ,str);
	/********************/
	
	if(angle >= gradient_scale_current_pos){	
		for(uint16_t i = gradient_scale_current_pos; i!=angle; i++){
		if(i == 360) i = 0;
		float t = (float)i / (total_degrees - 1);
    r = (uint8_t)((1 - t) * r_start + t * r_end);
    g = (uint8_t)((1 - t) * g_start + t * g_end);
    b = (uint8_t)((1 - t) * b_start + t * b_end);
		color = (r << 11) | (g << 5) | b;
		GC9A01_DrawCircleArountTheCircle(start_angle,i,25,95,color);			
		}
		gradient_scale_current_pos = angle;
	}
	else{
		for(int32_t i = gradient_scale_current_pos; i!= angle; i--){
		if(i == 0) i = 360;
		float t = (float)i / (total_degrees - 1);
    r = (uint8_t)((1 - t) * r_start + t * r_end);
    g = (uint8_t)((1 - t) * g_start + t * g_end);
    b = (uint8_t)((1 - t) * b_start + t * b_end);
		color = (r << 11) | (g << 5) | b;
		GC9A01_DrawCircleArountTheCircle(start_angle,i+1,25,95,BLACK);
		}
		GC9A01_DrawCircleArountTheCircle(start_angle,angle,25,95,color);
		gradient_scale_current_pos = angle;
	}
	
}


void GC9A01_SetTextColor(uint16_t color)
{
  lcdprop.TextColor=color;
}

void GC9A01_SetBackColor(uint16_t color)
{
  lcdprop.BackColor=color;
}

void GC9A01_SetFont(sFONT *pFonts)
{
  lcdprop.pFont=pFonts;
}

void GC9A01_DrawChar(uint16_t x, uint16_t y, uint8_t c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *c_t;
  uint8_t *pchar;
  uint32_t line=0;
  height = lcdprop.pFont->Height;
  width  = lcdprop.pFont->Width;
  offset = 8 *((width + 7)/8) -  width ;
  c_t = (uint8_t*) &(lcdprop.pFont->table[(c-' ') * lcdprop.pFont->Height * ((lcdprop.pFont->Width + 7) / 8)]);
  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)c_t + (width + 7)/8 * i);
    switch(((width + 7)/8))
    {
      case 1:
          line =  pchar[0];      
          break;
      case 2:
          line =  (pchar[0]<< 8) | pchar[1];
          break;
      case 3:
      default:
        line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];      
        break;
    }
    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1))) 
      {
        GC9A01_DrawPixel((x + j), y, lcdprop.TextColor);
      }
      else
      {
        GC9A01_DrawPixel((x + j), y, lcdprop.BackColor);
      } 
    }
    y++;      
  }
}

void GC9A01_String(uint16_t x,uint16_t y, char *str) //fix bug with X_END_POS
{
	
	uint8_t lenght = strlen(str);
	uint8_t step = x;
	if((lenght ) * lcdprop.pFont->Width > (X_END_POS - x ) ){
		for(uint8_t i = 0; i != ((((X_END_POS - x )  )/lcdprop.pFont->Width)); i++){
			GC9A01_DrawChar(step,y,str[i]);
			step+=lcdprop.pFont->Width;
		}
	}else{		
  while(*str)
  {
    GC9A01_DrawChar(x,y,str[0]);
    x+=lcdprop.pFont->Width;
    (void)*str++;
  }
}
}

uint8_t check_lenght_of_the_next_word(char *str){
	uint8_t lenght = 0;
	while(*str != ' ' && *str != '\0'){
		str++;
		lenght++;
	}
	return lenght;
}

void GC9A01_Text(char *str, uint8_t page) {
	
    GC9A01_ClearScreen(WHITE);
    GC9A01_SetBackColor(default_background_color);
    GC9A01_SetFont(&Font16);
    GC9A01_SetTextColor(default_text_color);

    uint8_t text_start_pos = OUT_TEXT_LEFT_INDENTATION;
    uint8_t text_end_pos = OUT_TEXT_RIGHT_INDENTATION;
    uint8_t max_chars_per_line = (LCD_W - text_start_pos - text_end_pos) / lcdprop.pFont->Width;
    uint8_t max_lines = (LCD_W - text_start_pos - text_end_pos) / lcdprop.pFont->Height + 1;
    uint8_t max_capacity = max_chars_per_line * max_lines;

    str += max_capacity * (page - 1);

    while (*str) {
        for (uint8_t i = 0; i < max_chars_per_line && *str; i++) {
            if (*str == '\n') { 
                text_end_pos += lcdprop.pFont->Height;
                str++;
                break;
            } else {
                GC9A01_DrawChar(text_start_pos, text_end_pos, *str);
               text_start_pos += lcdprop.pFont->Width;
                str++;
            }
        }
        text_end_pos += lcdprop.pFont->Height;
        text_start_pos = OUT_TEXT_LEFT_INDENTATION;
        if (text_end_pos > (LCD_W - OUT_TEXT_RIGHT_INDENTATION)) {
            break;
        }
    }
}

void GC9A01_Rainbow_String(uint16_t x,uint16_t y, char *str)
{
	current_text_color = red;
  while(*str)
  {
		switch(current_text_color){
			case red:
				GC9A01_SetTextColor(RAINBOW_RED);
				current_text_color = orange;
			break;
			case orange:
				GC9A01_SetTextColor(RAINBOW_ORANGE);
				current_text_color = yellow;
			break;
			case yellow:
				GC9A01_SetTextColor(RAINBOW_YELLOW);
				current_text_color = green;
			break;
			case green:
				GC9A01_SetTextColor(RAINBOW_GREEN);
				current_text_color = blue;
			break;
			case blue:
				GC9A01_SetTextColor(RAINBOW_BLUE);
				current_text_color = indigo;
			break;
			case indigo:
				GC9A01_SetTextColor(INDIGO);
				current_text_color = violet;
			break;
			case violet:
				GC9A01_SetTextColor(VIOLET);
				current_text_color = pink;
			break;
			case pink:
				GC9A01_SetTextColor(PINK);
				current_text_color = red;
			break;
		}
    GC9A01_DrawChar(x,y,str[0]);
    x+=lcdprop.pFont->Width;
    (void)*str++;		
  }
}




void ShowMenu(struct MenuMember Members[AMOUNT_OF_MENU_MEMBERS],uint8_t page_num){
	
	char str[1];
	char page[1];
	
	GC9A01_SetTextColor(default_text_color);
	GC9A01_SetBackColor(default_background_color);
	GC9A01_SetFont(default_font);
	
	sprintf(page, "%d", 	page_num);	
	GC9A01_String((LCD_W/2)-(3*lcdprop.pFont->Width),20,"Page");
	GC9A01_String((LCD_W/2)+(lcdprop.pFont->Width),20, page);	
	
	for(int i = 0; i<AMOUNT_OF_MENU_MEMBERS;i++){
		if(Members[i].state == active){ 
			GC9A01_SetBackColor(default_active_color);
		}
		sprintf(str, "%d", 	Members[i].number);								
		GC9A01_String		(X_POS, Y_POS+((i+1)*lcdprop.pFont->Width+(i+1)*STEP), str);
		GC9A01_DrawChar	((Members[i].number > 9 ? X_POS + 2 * lcdprop.pFont->Width : X_POS + lcdprop.pFont->Width),			\
 		Y_POS+((i+1)*lcdprop.pFont->Width+(i+1)*STEP), 	'.');
		GC9A01_String		((Members[i].number > 9 ? X_POS + 3 * lcdprop.pFont->Width : X_POS + 2 * lcdprop.pFont->Width),	\
		Y_POS+((i+1)*lcdprop.pFont->Width+(i+1)*STEP), 	Members[i].text);
		GC9A01_SetBackColor(default_background_color);
		
	}
}

void refresh_menu_member(struct MenuMember Members, uint8_t pos){
	
	char str[1];
	pos++;
	GC9A01_SetTextColor(default_text_color);
	GC9A01_SetBackColor(default_background_color);
	GC9A01_SetFont(default_font);	
	
	if(Members.state == active){ 
		GC9A01_SetBackColor(default_active_color);
	}
	sprintf(str, "%d", 	Members.number);								
	GC9A01_String		(X_POS, Y_POS+(pos*lcdprop.pFont->Width)+(pos*STEP), str);
	GC9A01_DrawChar	((Members.number > 9 ? X_POS + 2 * lcdprop.pFont->Width : X_POS + lcdprop.pFont->Width), 			\
	Y_POS+(pos*lcdprop.pFont->Width)+(pos*STEP), '.');
	GC9A01_String		((Members.number > 9 ? X_POS + 3 * lcdprop.pFont->Width : X_POS + 2 * lcdprop.pFont->Width), 	\
	Y_POS+(pos*lcdprop.pFont->Width)+(pos*STEP), Members.text);
	GC9A01_SetBackColor(default_background_color);	
	
}

void menu_active_member_running_text_animation(struct MenuMember Members[AMOUNT_OF_MENU_MEMBERS], uint8_t pos){
	
	uint8_t active_pos = get_active_menu_member(Members);
	char str[256];	
	sprintf(str, "%d.%s %d.%s ", Members[active_pos].number,Members[active_pos].text,		\
	Members[active_pos].number,Members[active_pos].text);			
	uint8_t lenght = strlen(str);
	uint8_t capacity = ((X_END_POS - X_POS)/(lcdprop.pFont->Width));
	
	if(lenght < 2*(capacity)){
		//end
	}else if (shift >= ((lenght)/2)){
		shift = 0;
	}else {
		pos++;
		GC9A01_SetTextColor(default_text_color);
		GC9A01_SetFont(default_font);	
		GC9A01_SetBackColor(default_active_color);					
		GC9A01_String		(X_POS, 	Y_POS+(pos*lcdprop.pFont->Width)+(pos*STEP), str + shift);
		GC9A01_SetBackColor(default_background_color);		
		shift = shift + 1;
	}
	
}


uint8_t get_active_menu_member(struct MenuMember Members[AMOUNT_OF_MENU_MEMBERS]){
	for(uint8_t i = 0; i !=AMOUNT_OF_MENU_MEMBERS; i++){
		if(Members[i].state == active)
		{
			return i;
		}
	}
}

uint8_t chek_menu_member_for_the_file_type(struct MenuMember Member, char *str){
	uint8_t i = 0;
	while(Member.text[i] != '\0' && Member.text[i] != '.'){
		i++;
	}
	i++;
	while(*str){
		if(Member.text[i] != *str){
			return 0;
		}
		else 
		{
			i++;
			str++;
		} 	
		
	}
	return 1;
}
