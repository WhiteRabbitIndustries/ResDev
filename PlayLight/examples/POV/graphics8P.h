// Don't edit this file!  It's software-generated.
// See convert.py script instead.

#define PALETTE1  0
#define PALETTE4  1
#define PALETTE8  2
#define TRUECOLOR 3

#define NUM_LEDS 17

// heart17.gif -------------------------------------------------------------

const uint8_t PROGMEM palette00[][3] = {
  {   0,   0,   0 },
  {   0,   0,   0 },
  {   0,   0,   0 },
  {   1,   0,   0 },
  {   2,   0,   0 },
  {   2,   0,   0 },
  {   4,   0,   0 },
  {   7,   0,   0 },
  {  18,   0,   0 },
  {  22,   0,   0 },
  {  23,   0,   0 },
  {  37,   0,   0 },
  {  39,   0,   0 },
  {  47,   0,   0 },
  {  57,   0,   0 },
  {  62,   0,   0 },
  {  76,   0,   0 },
  {  81,   0,   0 },
  {  91,   0,   0 } };

const uint8_t PROGMEM pixels00[] = {
  0X00, 0X00, 0X00, 0X06, 0X0C, 0X0F, 0X0E, 0X09,
  0X02, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X08, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X11, 0X04, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X07, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X11, 0X03, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X0D, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X10, 0X04, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X0F, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X11,
  0X03, 0X00, 0X00, 0X00, 0X00, 0X00, 0X0E, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X10, 0X04, 0X00, 0X00, 0X00, 0X00, 0X0C,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X11, 0X03, 0X00, 0X00, 0X00,
  0X04, 0X11, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X10, 0X05, 0X00,
  0X00, 0X00, 0X0A, 0X12, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X0D,
  0X00, 0X00, 0X04, 0X11, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X10,
  0X05, 0X00, 0X00, 0X0C, 0X12, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X11,
  0X03, 0X00, 0X00, 0X00, 0X0E, 0X12, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X10,
  0X04, 0X00, 0X00, 0X00, 0X01, 0X0F, 0X12, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X11,
  0X03, 0X00, 0X00, 0X00, 0X00, 0X00, 0X0D, 0X12,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X10,
  0X04, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X07,
  0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X12, 0X11,
  0X03, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X08, 0X12, 0X12, 0X12, 0X12, 0X12, 0X10,
  0X04, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X06, 0X0B, 0X0F, 0X0E, 0X09,
  0X02, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00 };

typedef struct {
  uint8_t        type;    // PALETTE[1,4,8] or TRUECOLOR
  line_t         lines;   // Length of image (in scanlines)
  const uint8_t *palette; // -> PROGMEM color table (NULL if truecolor)
  const uint8_t *pixels;  // -> Pixel data in PROGMEM
} image;

const image PROGMEM images[] = {
  { PALETTE8 ,   17, (const uint8_t *)palette00, pixels00 }
};

#define NUM_IMAGES (sizeof(images) / sizeof(images[0]))
