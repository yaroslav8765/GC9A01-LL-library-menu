# GC9A01-LL Library & Menu  
**Low-Level Display Driver for GC9A01 with Menu Functionality**  

This is a lightweight library for working with the GC9A01 display driver. It includes basic functions required for interacting with displays and can be easily ported to other display drivers if needed.  

If you want to port this library, modify the functions in `GC9A01.c` (lines 10 to 342). Most of this section contains initialization code, so adapting it should not be overly complex.  

---

## Setup  

If you are using the GC9A01 driver, edit the following lines in `GC9A01.h`:  

```c
#define LCD_RST_1 LL_GPIO_SetOutputPin(RES_GPIO_Port, RES_Pin) // Set LCD reset pin high  
#define LCD_RST_0 LL_GPIO_ResetOutputPin(RES_GPIO_Port, RES_Pin) // Set LCD reset pin low  

#define LCD_CS_1 LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin) // Set chip select pin high  
#define LCD_CS_0 LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin) // Set chip select pin low  

#define LCD_DC_1 LL_GPIO_SetOutputPin(DC_GPIO_Port, DC_Pin) // Set data/command pin high  
#define LCD_DC_0 LL_GPIO_ResetOutputPin(DC_GPIO_Port, DC_Pin) // Set data/command pin low  

#define SPI_NO SPI1 // Define the SPI instance used  
#define DMA_NO DMA2 // Define the DMA instance used  
#define DMA_STREAM LL_DMA_STREAM_3 // Define the DMA stream used
 
```

Replace the above definitions if you are using different SPI, DMA, or GPIO pins. For example, here are the pins I used in my configuration:

```c
#define DC_Pin LL_GPIO_PIN_2
#define DC_GPIO_Port GPIOA  

#define RES_Pin LL_GPIO_PIN_3  
#define RES_GPIO_Port GPIOA  

#define CS_Pin LL_GPIO_PIN_10  
#define CS_GPIO_Port GPIOB  

```

## MAIN FUNCTIONS:
---
```c
The library provides the following functions for interacting with the display:

void GC9A01_ClearScreen(uint16_t bColor);
Fill the entire screen with a specified color.

void GC9A01_ClearWindow(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint16_t bColor);
Fill a rectangular area with a specified color.

void GC9A01_show_picture(uint16_t *picture, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
Display an image starting at position (x, y).

void GC9A01_DrawPixel_2x2(uint8_t x, uint8_t y, uint16_t color);
Draw a 2x2 pixel at (x, y) with a specified color.

void GC9A01_draw_line(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
Draw a line between two points.

void GC9A01_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
Draw a rectangle.

void GC9A01_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
Draw a circle.

void GC9A01_FilledDrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
Draw a filled circle.

void GC9A01_SetTextColor(uint16_t color);
Set the text color.

void GC9A01_SetBackColor(uint16_t color);
Set the background color.

void GC9A01_SetFont(sFONT *pFonts);
Set the font.

void GC9A01_DrawChar(uint16_t x, uint16_t y, uint8_t c);
Draw a single character.

void GC9A01_String(uint16_t x, uint16_t y, char *str);
Draw a string of text at (x, y).

void GC9A01_Text(char *str, uint8_t page);
Display long text, scrolling across multiple pages.

void GC9A01_Rainbow_String(uint16_t x, uint16_t y, char *str);
Draw a string of text with a rainbow gradient.
```

## MENU FUNCTIONALITY:
---
The library includes menu-related functions for displaying and interacting with menus or lists.

To create a menu, define a structure like this:	```c struct MenuMember Members[8]; // Replace 8 with the desired number of menu items  ```

This structure have 3 main parameters:
```c
Each menu item in the structure has the following parameters:

Members[i].text – The text for the menu item.
Members[i].number – The index of the menu item.
Members[i].state – The state of the menu item (e.g., active or inactive).
```

If you want to change the number of menu members, menu location, or other parameters, update the following lines in the `GC9A01.h` file:

```c
/******************** TEXT STYLE **********************/
#define default_text_color BLACK            // Default text color
#define default_background_color WHITE      // Default background color
#define default_active_color GREEN          // Color for the active menu item
#define default_font &Font20                // Default font used
/******************************************************/

/******************** MENU PARAMETERS *****************/
#define X_POS 35                            // X-coordinate of the menu
#define Y_POS 25                            // Y-coordinate of the menu
#define X_END_POS 205                       // X-coordinate limit of the menu
#define STEP 6                              // Space between menu lines in pixels
#define AMOUNT_OF_MENU_MEMBERS 8            // Number of menu items
/******************************************************/

/******************** TEXT POSITION *******************/
//only for GC9A01_Text
#define OUT_TEXT_LEFT_INDENTATION 35        // Left margin for text
#define OUT_TEXT_RIGHT_INDENTATION 35       // Right margin for text
/******************************************************/
```

### Menu Functions

After setting up the parameters, you can use the following functions:

```c
void ShowMenu(struct MenuMember Members[], uint8_t page_num);
// Display the menu on the screen.

void refresh_menu_member(struct MenuMember Members[], uint8_t pos);
// Update the state of a specific menu item.

uint8_t get_active_menu_member(struct MenuMember Members[]);
// Retrieve the index of the currently active menu item.

void menu_active_member_running_text_animation(struct MenuMember Members[], uint8_t pos);
// Animate the text of the active menu item by scrolling it to the left.
```

### Example Output

After setting up the menu, it should look like this:  
![Menu Example](https://github.com/user-attachments/assets/44ef9ad1-e0a2-454c-99ea-357c57a315ce)



