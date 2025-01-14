# GC9A01-LL Library & Menu  
**Low-Level Display Driver for GC9A01 with Menu Functionality**  

This is a lightweight library for working with the GC9A01 display driver. It includes basic functions required for interacting with displays and can be easily ported to other display drivers if needed.  

If you want to port this library, modify the functions in `GC9A01.c` (lines 10 to 342). Most of this section contains initialization code, so adapting it should not be overly complex.  

---

## Setup

1. **Open CubeMX**  
   Select your MCU. On the **Connectivity** tab, enable SPI in *Transmit Master Only* mode:

   ![SPI Mode](https://github.com/user-attachments/assets/51039e09-256a-4dc6-b274-c347436b4c47)

2. **Enable DMA**  

   ![Enable DMA](https://github.com/user-attachments/assets/1d377596-fc8c-45c8-a1f2-8f04eb9eda57)

3. **Setup Pins**  
   Configure the pins as output, as shown below (or use different pins, but match their names):

   ![Pin Configuration](https://github.com/user-attachments/assets/bad27748-8c4b-4ae5-8d81-c85806b9d11a)

4. **Switch to LL Drivers**  
   In **Project Manager → Advanced Settings**, select **LL** instead of **HAL**:

   ![Select LL](https://github.com/user-attachments/assets/96d48e5b-b4dc-4f88-a0c9-8f0d5a3013ff)

5. **Other Project Properties**  

   ![Other Properties](https://github.com/user-attachments/assets/083c37fe-305a-44f2-9038-c6b5eecd7d56)

6. **Copy Library Files**  
   Open the project folder and copy the following files:
   - `GC9A01.h` and `fonts.h` → `Core/Inc/`
   - `GC9A01.c`, `font8.c`, `font12.c`, `font16.c`, `font20.c`, and `font24.c` → `Core/Src/`

   ![Copy Headers](https://github.com/user-attachments/assets/61fba6eb-89b1-493c-bbea-ea3cff2a92fa)  
   ![Copy Sources](https://github.com/user-attachments/assets/9c055c67-9f0d-46c7-8eb6-d0f2c6356b92)

7. **Add Files to the Project**  
   Open Keil and add the files to your project:

   ![Add Files to Keil](https://github.com/user-attachments/assets/b13f29ab-4fbe-4228-9dae-7db7801f4f0f)

8. **Include Header Files**  
   Open `main.h` and include the following headers:
   ```c
   #include "stdio.h"
   #include "stdlib.h"
   #include "string.h"
   #include "math.h"
   #include "fonts.h"
   #include "GC9A01.h"
   ```
   ![Include Headers](https://github.com/user-attachments/assets/57bcd08f-5ec2-4718-904a-96e611f42161)

9. **Declare Variables**  
   In `main.c`, declare this variable:
   ```c
   volatile uint8_t dma_spi_fl1 = 0;
   ```
   ![Declare Variable](https://github.com/user-attachments/assets/b1dde559-17c3-43ef-a9ee-644d4dc7df0d)

10. **Handle DMA Flags**  
    Open `stm32f4xx_it.c` and declare the variable as `extern`:
    ```c
    extern volatile uint8_t dma_spi_fl1;
    ```
    ![Extern Variable](https://github.com/user-attachments/assets/a6da9981-7a6f-417e-b837-f5d643de0b39)

    Paste this code in your **DMA IRQ Handler**:
    ```c
    if (LL_DMA_IsActiveFlag_TC(DMA_NO) == 1) {
        LL_DMA_ClearFlag_TC(DMA_NO);
        dma_spi_fl1 = 1;
    } else if (LL_DMA_IsActiveFlag_TE(DMA_NO) == 1) {
        LL_DMA_ClearFlag_TE(DMA_NO);
        Error_Handler();
    }
    ```
    ![DMA Handler](https://github.com/user-attachments/assets/cac4b32d-ca1e-4025-bef4-88fc43f0fe62)

11. **Configure GC9A01**  
    In `GC9A01.h`, fill in these lines correctly:
    ```c
    #define LCD_RST_1 LL_GPIO_SetOutputPin(RES_GPIO_Port, RES_Pin) // LCD reset pin high
    #define LCD_RST_0 LL_GPIO_ResetOutputPin(RES_GPIO_Port, RES_Pin) // LCD reset pin low

    #define LCD_CS_1 LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin) // LCD chip select high
    #define LCD_CS_0 LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin) // LCD chip select low

    #define LCD_DC_1 LL_GPIO_SetOutputPin(DC_GPIO_Port, DC_Pin) // Data/Command pin high
    #define LCD_DC_0 LL_GPIO_ResetOutputPin(DC_GPIO_Port, DC_Pin) // Data/Command pin low

    #define SPI_NO SPI1 // SPI instance
    #define DMA_NO DMA2 // DMA instance
    #define DMA_STREAM LL_DMA_STREAM_3 // DMA stream

    #define LL_DMA_ClearFlag_TC LL_DMA_ClearFlag_TC3 // Adjust for your stream
    #define LL_DMA_ClearFlag_TE LL_DMA_ClearFlag_TE3
    #define LL_DMA_IsActiveFlag_TC LL_DMA_IsActiveFlag_TC3
    #define LL_DMA_IsActiveFlag_TE LL_DMA_IsActiveFlag_TE3
    ```

12. **Update `main.c`**  
    Add these initializations in your **main function**:
    ```c
    /* USER CODE BEGIN Init */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
    /* USER CODE END Init */
    ```

    And this:
    ```c
    /* USER CODE BEGIN 2 */
    LL_DMA_DisableStream(DMA_NO, DMA_STREAM);
    LL_DMA_ClearFlag_TC(DMA_NO);
    LL_DMA_ClearFlag_TE(DMA_NO);
    LL_DMA_EnableIT_TC(DMA_NO, DMA_STREAM);
    LL_DMA_EnableIT_TE(DMA_NO, DMA_STREAM);
    LL_SPI_EnableDMAReq_TX(SPI_NO);
    LL_SPI_Enable(SPI_NO);
    /* USER CODE END 2 */
    ```

13. **Test the Library**  
    Verify functionality with this code:
    ```c
    GC9A01_Initial();
    GC9A01_ClearScreen(WHITE);
    GC9A01_SetBackColor(BLACK);
    GC9A01_SetFont(&Font16);
    GC9A01_Text("Hello world", 1);
    GC9A01_DrawCircle(120, 120, 60, BLUE);
    ```
    ![Test Output](https://github.com/user-attachments/assets/39050295-68b2-4a38-a09f-e0064e6e2a08)




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



