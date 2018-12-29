/*  OctoWS2811 BasicTest.ino - Basic RGB LED Test
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.

  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.
*/

#include <OctoWS2811.h>
#include "rgb_colors.h"
#include "synapse_lib.h"
#include "strand.h"

#define TEST_MODE 0
#define JELLYFISH 1
#define JELLYFISH_HEAD 2
#define MONOLITH 3

#define MODE MONOLITH

#if MODE==MONOLITH
const int ledsPerStrip = 2*144;
#else
const int ledsPerStrip = 1*144;
#endif

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;
int position = 0;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

#if MODE==JELLYFISH || MODE==JELLYFISH_HEAD

// These are tentacle strands
Synapse strand_01 = Synapse(leds, 0, 144, RED, 0.8, 100);
Synapse strand_02 = Synapse(leds, 144, 288, GREEN, 0.8, 70);
Synapse strand_03 = Synapse(leds, 288, 432, BLUE, 0.8, 40);
Synapse strand_04 = Synapse(leds, 432, 576, YELLOW, 0.8, 20);
// These are head strands if JELLYFISH_HEAD, else tentacle strands
Synapse strand_05 = Synapse(leds, 576, 720, CYAN, 0.8, 30);
Synapse strand_06 = Synapse(leds, 720, 864, MAGENTA, 0.8, 70);
Synapse strand_07 = Synapse(leds, 864, 1008, RED, 0.8, 50);
Synapse strand_08 = Synapse(leds, 1008, 1152, GREEN, 0.8, 60);

#endif // if JELLYFISH || JELLYFISH_HEAD



#if MODE==MONOLITH

// upper monolith is 110 LEDs
Strand upper_1 = Strand(leds, 0, 110, GREEN, 100, FORWARD, 5);
Strand upper_2 = Strand(leds, 144+34, 110, GREEN, 100, REVERSE, 5);
Strand upper_3 = Strand(leds, 288, 110, GREEN, 100, FORWARD, 5);
Strand upper_4 = Strand(leds, 432+34, 110, GREEN, 100, REVERSE, 5);
Strand upper_5 = Strand(leds, 576, 110, GREEN, 100, FORWARD, 5);
Strand upper_6 = Strand(leds, 720+34, 110, GREEN, 100, REVERSE, 5);
Strand upper_7 = Strand(leds, 864, 110, GREEN, 100, FORWARD, 5);
Strand upper_8 = Strand(leds, 1008+34, 110, GREEN, 100, REVERSE, 5);

// Lower monolith is 34 LEDs
Strand lower_1 = Strand(leds, 0+110, 34, BLUE, 31, REVERSE, 5);
Strand lower_2 = Strand(leds, 144, 34, BLUE, 31, FORWARD, 5);
Strand lower_3 = Strand(leds, 288+110, 34, BLUE, 31, REVERSE, 5);
Strand lower_4 = Strand(leds, 432, 34, BLUE, 31, FORWARD, 5);
Strand lower_5 = Strand(leds, 576+110, 34, BLUE, 31, REVERSE, 5);
Strand lower_6 = Strand(leds, 720, 34, BLUE, 31, FORWARD, 5);
Strand lower_7 = Strand(leds, 864+110, 34, BLUE, 31, REVERSE, 5);
Strand lower_8 = Strand(leds, 1008, 34, BLUE, 31, FORWARD, 5);

int strand_index = 0;
running_status strand_status = STOPPED;

#endif
//
int status_1 = 0;
int status_2 = 0;
int status_3 = 0;
int status_4 = 0;
int status_5 = 0;
int status_6 = 0;
int status_7 = 0;
int status_8 = 0;


void setup() {

#if MODE==JELLYFISH_HEAD
    strand_05.random_tail_factor_enable = false;
    strand_05.random_start_delay_enable = false;
    strand_05.tail_factor=0.99;
    strand_06.random_tail_factor_enable = false;
    strand_06.random_start_delay_enable = false;
    strand_06.tail_factor=0.99;
    strand_07.random_tail_factor_enable = false;
    strand_07.random_start_delay_enable = false;
    strand_07.tail_factor=0.99;
    strand_08.random_tail_factor_enable = false;
    strand_08.random_start_delay_enable = false;
    strand_08.tail_factor=0.99;
#endif

#if MODE==MONOLITH

#endif

    leds.begin();
    leds.show();
}

void loop() {

#if MODE==TEST_MODE
    // basic tests for eight 144-led strands connected to OctoWS2811
    int microseconds = 1000 / leds.numPixels();  // change them all in 2 seconds
    //doColorWipe(microseconds);
    doColorStars(microseconds);
    //doRgbStars(microseconds);
    delayMicroseconds(10000);
#endif


#if MODE==JELLYFISH || MODE==JELLYFISH_HEAD
    status_1 = strand_01.chaseStep();
    status_2 = strand_02.chaseStep();
    status_3 = strand_03.chaseStep();
    status_4 = strand_04.chaseStep();
    status_5 = strand_05.chaseStep();
    status_6 = strand_06.chaseStep();
    status_7 = strand_07.chaseStep();
    status_8 = strand_08.chaseStep();

    leds.show();
#endif

#if MODE==MONOLITH
    //int microseconds = 1000 / leds.numPixels();  // change them all in 2 seconds
    //baseChase(microseconds);
    // 1) wipe colors
    // 2) wipe single color
    // 3)  rotate colors
    // 4) rotate single color


    upper_1.chase_step();
    upper_2.chase_step();
    upper_3.chase_step();
    upper_4.chase_step();
    upper_5.chase_step();
    upper_6.chase_step();
    upper_7.chase_step();
    strand_status = upper_8.chase_step();

    if (strand_status == LAST_PIXEL ){
        set_strand_colors(strand_index);
        //strand_index++;// =
        if ((strand_index >= 7) || (strand_index < 0)){
            strand_index = 0;
        }
        else {
            strand_index++;
        }
    }

    // 1) chase white up
    // 2) chase white down
    lower_1.chase_step();
    lower_2.chase_step();
    lower_3.chase_step();
    lower_4.chase_step();
    lower_5.chase_step();
    lower_6.chase_step();
    lower_7.chase_step();
    lower_8.chase_step();


    leds.show();
#endif

}
//---------- monolith ----------------

void set_strand_colors(int offset){
//    if (strand_index >= 8){
//        strand_index = 0;
//    }
    RGB color;
    color = palette[(0+offset) % 6];
    upper_1.color = color;
    color = palette[(1+offset) % 6];
    upper_2.color = color;
    color = palette[(2+offset) % 6];
    upper_3.color = color;
    color = palette[(3+offset) % 6];
    upper_4.color = color;
    color = palette[(4+offset) % 6];
    upper_5.color = color;
    color = palette[(5+offset) % 6];
    upper_6.color = color;
    color = palette[(6+offset) % 6];
    upper_7.color = color;
    color = palette[(7+offset) % 6];
    upper_8.color = color;


}
//---------- Test Code ---------------

void doColorWipe(int microsecond) {

  colorWipe(RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, RED, microsecond);
  colorWipe(GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, RED, RED, microsecond);
  colorWipe(BLUE, YELLOW, CYAN, MAGENTA, WHITE, RED, RED, GREEN, microsecond);
  colorWipe(YELLOW, CYAN, MAGENTA, WHITE, RED, WHITE, RED, GREEN, microsecond);
  colorWipe(CYAN, MAGENTA, WHITE, RED, WHITE, RED, GREEN, BLUE, microsecond);
  colorWipe(CYAN, MAGENTA, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, microsecond);
  colorWipe(MAGENTA, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, microsecond);
  colorWipe(WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, microsecond);
}

void doColorStars(int microsecond) {
    colorStars(position, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, RED, WHITE, microsecond);
    position++;
    if (position >= 144) {
        position = 0;
    }
}

void doRgbStars(int time_usec) {
    int microsec = time_usec;

    rgbStars(position, 0xff, 0x00, 0x00);
    position = ((position >= 144) ? 0 : position++);
    rgbStars(position, 0x00, 0xff, 0x00);
    position = ((position >= 144) ? 0 : position++);
    rgbStars(position, 0x00, 0x00, 0xff);
    position = ((position >= 144) ? 0 : position++);
    rgbStars(position, 0xff, 0xff, 0x00);
    position = ((position >= 144) ? 0 : position++);
    rgbStars(position, 0xff, 0x00, 0xff);
    position = ((position >= 144) ? 0 : position++);
    rgbStars(position, 0x00, 0xff, 0xff);
    position = ((position >= 144) ? 0 : position++);
    rgbStars(position, 0xff, 0xff, 0xff);
    position = ((position >= 144) ? 0 : position++);

}

int rgbToInt(RGB _color) {
    int color = (_color.red << 16) + (_color.green << 8) + _color.blue;
    return color;
}

void setPixel(int pixel, RGB color){
    leds.setPixel(pixel, rgbToInt(color));
}

void colorWipe(RGB color0, RGB color1, RGB color2, RGB color3, RGB color4,
               RGB color5, RGB color6, RGB color7, int wait) {
    //for (int i=0; i < leds.numPixels(); i++) {
    for (int i = 0; i < 144; i++) {
        setPixel(i + 0 * 144, color0);
        setPixel(i + 1 * 144, color1);
        setPixel(i + 2 * 144, color2);
        setPixel(i + 3 * 144, color3);
        setPixel(i + 4 * 144, color4);
        setPixel(i + 5 * 144, color5);
        setPixel(i + 6 * 144, color6);
        setPixel(i + 7 * 144, color7);
        leds.show();
        //delayMicroseconds(20000);
        delayMicroseconds(00000);
    }
}

void
colorStars(int star_position, RGB color0, RGB color1, RGB color2, RGB color3,
           RGB color4, RGB color5, RGB color6, RGB color7, int wait) {
    int i = 0;
    //for (int i=0; i < leds.numPixels(); i++) {
    for (int j = 0; j < 144; j++) {
        i = j;
        setPixel(i + 0 * 144, color0);
        setPixel(i + 1 * 144, color1);
        setPixel(i + 2 * 144, color2);
        setPixel(i + 3 * 144, color3);
        setPixel(i + 4 * 144, color4);
        setPixel(i + 5 * 144, color5);
        setPixel(i + 6 * 144, color6);
        setPixel(i + 7 * 144, color7);

        if (i > 0) {
            i = j - 1;
            setPixel(i + 0 * 144, OFF);
            setPixel(i + 1 * 144, OFF);
            setPixel(i + 2 * 144, OFF);
            setPixel(i + 3 * 144, OFF);
            setPixel(i + 4 * 144, OFF);
            setPixel(i + 5 * 144, OFF);
            setPixel(i + 6 * 144, OFF);
            setPixel(i + 7 * 144, OFF);
        }

        leds.show();
        //delayMicroseconds(40000);
        delayMicroseconds(10000);
    }
}

void rgbStars(int star_position, int red, int green, int blue) {
    RGB color = {red, green, blue};

    int i = 0;
    //for (int i=0; i < leds.numPixels(); i++) {
    for (int j = 0; j < 144; j++) {
        i = j;
        setPixel(i + 0 * 144, color);
        setPixel(i + 1 * 144, color);
        setPixel(i + 2 * 144, color);
        setPixel(i + 3 * 144, color);
        setPixel(i + 4 * 144, color);
        setPixel(i + 5 * 144, color);
        setPixel(i + 6 * 144, color);
        setPixel(i + 7 * 144, color);

        if (i > 0) {
            i = j - 1;
            setPixel(i + 0 * 144, OFF);
            setPixel(i + 1 * 144, OFF);
            setPixel(i + 2 * 144, OFF);
            setPixel(i + 3 * 144, OFF);
            setPixel(i + 4 * 144, OFF);
            setPixel(i + 5 * 144, OFF);
            setPixel(i + 6 * 144, OFF);
            setPixel(i + 7 * 144, OFF);
        }

        leds.show();
        //delayMicroseconds(40000);
        delayMicroseconds(10000);
    }
}
