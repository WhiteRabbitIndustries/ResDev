// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!
#define DEBUG true


#include <Adafruit_DotStar.h>

// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#ifdef __AVR_ATtiny85__
typedef uint8_t  line_t; // Max 255 lines/image on Trinket
#else
typedef uint16_t line_t; // Bigger images OK on other boards
#endif

#define NUMPIXELS 38 // Number of LEDs in strip


//  0   1   2    3   4           15  16  17  18
//    37  36  35  34  33            21    20    19
//    1   3   5   7   9             
int mappedPixelNum(int pixelNum)
{
  int val = 0;
  if( (pixelNum%2) == 0)
  {
    val = pixelNum/2;
   }
    else 
    {
      val = (NUMPIXELS-(pixelNum+1)/2);
      }
              if(DEBUG) 
        {
          Serial.print(pixelNum);
          Serial.print("to");
          Serial.print(val);
          Serial.print("  ");
          }
  return val;
  
  }
  
// CONFIGURABLE STUFF ------------------------------------------------------

#include "graphics.h" // Graphics data is contained in this header file.
// It's generated using the 'convert.py' Python script.  Various image
// formats are supported, trading off color fidelity for PROGMEM space
// (particularly limited on Trinket).  Handles 1-, 4- and 8-bit-per-pixel
// palette-based images, plus 24-bit truecolor.  1- and 4-bit palettes can
// be altered in RAM while running to provide additional colors, but be
// mindful of peak & average current draw if you do that!  Power limiting
// is normally done in convert.py (keeps this code relatively small & fast).
// 1/4/8/24 were chosen because the AVR can handle these operations fairly
// easily (have idea for handing arbitrary bit depth w/328P, but this margin
// is too narrow to contain).


// Here's how to control the LEDs from any two pins:
#define DATAPIN    A3
#define CLOCKPIN   A2
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);
//
//
//


  void     imageInit(void);
  
void setup() {
if(DEBUG) Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.clear();                // Make sure strip is clear
  strip.show();  // Turn all LEDs off ASAP

   imageInit(); 
   
}

uint32_t lastImageTime = 0L; // Time of last image change
uint8_t  imageNumber   = 0,  // Current image being displayed
           imageType,          // Image type: PALETTE[1,4,8] or TRUECOLOR
          *imagePalette,       // -> palette data in PROGMEM
          *imagePixels,        // -> pixel data in PROGMEM
           palette[16][3];     // RAM-based color table for 1- or 4-bit images
  line_t   imageLines,         // Number of lines in active image
           imageLine;          // Current line number in image


void imageInit() { // Initialize global image state for current imageNumber
    imageType    = pgm_read_byte(&images[imageNumber].type);
  #ifdef __AVR_ATtiny85__
    imageLines   = pgm_read_byte(&images[imageNumber].lines);
  #else
    imageLines   = pgm_read_word(&images[imageNumber].lines);
  #endif
    imageLine    = 0;
    imagePalette = (uint8_t *)pgm_read_word(&images[imageNumber].palette);
    imagePixels  = (uint8_t *)pgm_read_word(&images[imageNumber].pixels);
    // 1- and 4-bit images have their color palette loaded into RAM both for
    // faster access and to allow dynamic color changing.  Not done w/8-bit
    // because that would require inordinate RAM (328P could handle it, but
    // I'd rather keep the RAM free for other features in the future).
    if(imageType == PALETTE1)      memcpy_P(palette, imagePalette,  2 * 3);
    else if(imageType == PALETTE4) memcpy_P(palette, imagePalette, 16 * 3);
    lastImageTime = millis(); // Save time of image init for next auto-cycle
  }
void nextImage(void) {
  if(++imageNumber >= NUM_IMAGES) imageNumber = 0;
  imageInit();
}
boolean autoCycle = true; // Set to true to cycle images by default
#define CYCLE_TIME 10     // Time, in seconds, between auto-cycle images


// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

//int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
//uint32_t color = 0xFF0000;      // 'On' color (starts red)

void loop() {
 uint32_t t = millis();               // Current time, milliseconds
  if(autoCycle) {
    if((t - lastImageTime) >= (CYCLE_TIME * 1000L)) nextImage();
    // CPU clocks vary slightly; multiple poi won't stay in perfect sync.
    // Keep this in mind when using auto-cycle mode, you may want to cull
    // the image selection to avoid unintentional regrettable combinations.
  }

  if(DEBUG) Serial.println(imageType);
  switch(imageType) {

    case PALETTE1: { // 1-bit (2 color) palette-based image
      uint8_t  pixelNum = 0, byteNum, bitNum, pixels, idx,
              *ptr = (uint8_t *)&imagePixels[imageLine * NUM_LEDS / 8];
      for(byteNum = NUM_LEDS/8; byteNum--; ) { // Always padded to next byte
        pixels = pgm_read_byte(ptr++);  // 8 pixels of data (pixel 0 = LSB)
        for(bitNum = 8; bitNum--; pixels >>= 1) {
          idx = pixels & 1; // Color table index for pixel (0 or 1)
          
          //strip.setPixelColor(pixelNum++,
            //palette[idx][0], palette[idx][1], palette[idx][2]);
            
            strip.setPixelColor(mappedPixelNum(pixelNum++), palette[idx][0], palette[idx][1], palette[idx][2]);
        }
      }
      break;
    }

    case PALETTE4: { // 4-bit (16 color) palette-based image
      uint8_t  pixelNum, p1, p2,
              *ptr = (uint8_t *)&imagePixels[imageLine * NUM_LEDS / 2];
      for(pixelNum = 0; pixelNum < NUM_LEDS; ) 
      {
        p2  = pgm_read_byte(ptr++); // Data for two pixels...
        p1  = p2 >> 4;              // Shift down 4 bits for first pixel
        p2 &= 0x0F;                 // Mask out low 4 bits for second pixel

        strip.setPixelColor(mappedPixelNum(pixelNum++),
          palette[p1][0], palette[p1][1], palette[p1][2]);
        strip.setPixelColor(mappedPixelNum(pixelNum++),
          palette[p2][0], palette[p2][1], palette[p2][2]);
          
      }
      break;
    }

    case PALETTE8: { // 8-bit (256 color) PROGMEM-palette-based image
      uint16_t  o;
      uint8_t   pixelNum,
               *ptr = (uint8_t *)&imagePixels[imageLine * NUM_LEDS];
      for(pixelNum = 0; pixelNum < NUM_LEDS; pixelNum++) {
        o = pgm_read_byte(ptr++) * 3; // Offset into imagePalette
        strip.setPixelColor(mappedPixelNum(pixelNum),
          pgm_read_byte(&imagePalette[o]),
          pgm_read_byte(&imagePalette[o + 1]),
          pgm_read_byte(&imagePalette[o + 2]));
      }
      break;
    }

    case TRUECOLOR: { // 24-bit ('truecolor') image (no palette)
      uint8_t  pixelNum, r, g, b,
              *ptr = (uint8_t *)&imagePixels[imageLine * NUM_LEDS * 3];
      for(pixelNum = 0; pixelNum < NUM_LEDS; pixelNum++) {
        r = pgm_read_byte(ptr++);
        g = pgm_read_byte(ptr++);
        b = pgm_read_byte(ptr++);
        strip.setPixelColor(mappedPixelNum(pixelNum), r, g, b);
      }
      break;
    }
  }

  strip.show(); // Refresh LEDs
  if(++imageLine >= imageLines) imageLine = 0; // Next scanline, wrap around
delayMicroseconds(3500);







// 
//  strip.setPixelColor(head, color); // 'On' pixel at head
//  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
//  strip.show();                     // Refresh strip
//  delay(20);                        // Pause 20 milliseconds (~50 FPS)
//
//  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
//    head = 0;                       //  Yes, reset head index to start
//    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
//      color = 0xFF0000;             //   Yes, reset to red
//  }
//  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
}



