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

const int ledsPerStrip = 144;
//const int ledsPerStrip = 2*144;

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;
int position = 0;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

#define JELLYFISH_HEAD

//tentacles
Synapse strand_01 = Synapse(leds, 0, 144, RED, 0.8, 100);
Synapse strand_02 = Synapse(leds, 144, 288, GREEN, 0.8, 70);
Synapse strand_03 = Synapse(leds, 288, 432, BLUE, 0.8, 40);
Synapse strand_04 = Synapse(leds, 432, 576, YELLOW, 0.8, 20);
//head
Synapse strand_05 = Synapse(leds, 576, 720, CYAN, 0.8, 30);
Synapse strand_06 = Synapse(leds, 720, 864, MAGENTA, 0.8, 70);
Synapse strand_07 = Synapse(leds, 864, 1008, RED, 0.8, 50);
Synapse strand_08 = Synapse(leds, 1008, 1152, GREEN, 0.8, 60);



synapse_settings ss;


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

#ifdef JELLYFISH_HEAD
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

    leds.begin();
    leds.show();
}

void loop() {
    int microsec = 1000 / leds.numPixels();  // change them all in 2 seconds

    // test routines
    //do_colorStars(microsec);
    //do_colorWipe(microsec);
    //do_starsRbg(microsec);

    status_1 = strand_01.chaseStep();
    status_2 = strand_02.chaseStep();
    status_3 = strand_03.chaseStep();
    status_4 = strand_04.chaseStep();
    status_5 = strand_05.chaseStep();
    status_6 = strand_06.chaseStep();
    status_7 = strand_07.chaseStep();
    status_8 = strand_08.chaseStep();

    leds.show();

    //delayMicroseconds(10000);

}

//---------- Test Code ---------------

void do_colorWipe(int time_usec) {
    int microsec = time_usec;

//  colorWipe(RED, GREEN, BLUE, YELLOW, PINK, ORANGE, PURPLE, WHITE, microsec);
//  colorWipe(GREEN, BLUE, YELLOW, PINK, ORANGE, PURPLE, WHITE, RED, microsec);
//  colorWipe(BLUE, YELLOW, PINK, ORANGE, PURPLE, WHITE, RED, GREEN, microsec);
//  colorWipe(YELLOW, PINK, ORANGE, PURPLE, WHITE, RED, GREEN, BLUE, microsec);
//  colorWipe(PINK, ORANGE, PURPLE, WHITE, RED, GREEN, BLUE, YELLOW, microsec);
//  colorWipe(ORANGE, PURPLE, WHITE, RED, GREEN, BLUE, YELLOW, PINK, microsec);
//  colorWipe(PURPLE, WHITE, RED, GREEN, BLUE, YELLOW, PINK, ORANGE, microsec);
//  colorWipe(WHITE, RED, GREEN, BLUE, YELLOW, PINK, ORANGE, PURPLE, microsec);
}

void do_colorStars(int time_usec) {
    int microsec = time_usec;

//  colorStars(position, RED, GREEN, BLUE, YELLOW, PINK, ORANGE, PURPLE, WHITE, microsec);
    position++;
    if (position >= 144) {
        position = 0;
    }
}

void do_starsRbg(int time_usec) {
    int microsec = time_usec;

    starsRbg(position, 0xff, 0x00, 0x00);
    position = ((position >= 144) ? 0 : position++);
    starsRbg(position, 0x00, 0xff, 0x00);
    position = ((position >= 144) ? 0 : position++);
    starsRbg(position, 0x00, 0x00, 0xff);
    position = ((position >= 144) ? 0 : position++);
    starsRbg(position, 0xff, 0xff, 0x00);
    position = ((position >= 144) ? 0 : position++);
    starsRbg(position, 0xff, 0x00, 0xff);
    position = ((position >= 144) ? 0 : position++);
    starsRbg(position, 0x00, 0xff, 0xff);
    position = ((position >= 144) ? 0 : position++);
    starsRbg(position, 0xff, 0xff, 0xff);
    position = ((position >= 144) ? 0 : position++);

}

void colorWipe(int color0, int color1, int color2, int color3, int color4,
               int color5, int color6, int color7, int wait) {
    //for (int i=0; i < leds.numPixels(); i++) {
    for (int i = 0; i < 144; i++) {
        leds.setPixel(i + 0 * 144, color0);
        leds.setPixel(i + 1 * 144, color1);
        leds.setPixel(i + 2 * 144, color2);
        leds.setPixel(i + 3 * 144, color3);
        leds.setPixel(i + 4 * 144, color4);
        leds.setPixel(i + 5 * 144, color5);
        leds.setPixel(i + 6 * 144, color6);
        leds.setPixel(i + 7 * 144, color7);
        leds.show();
        //delayMicroseconds(20000);
        delayMicroseconds(00000);
    }
}

void
colorStars(int star_position, int color0, int color1, int color2, int color3,
           int color4, int color5, int color6, int color7, int wait) {
    int i = 0;
    //for (int i=0; i < leds.numPixels(); i++) {
    for (int j = 0; j < 144; j++) {
        i = j;
        leds.setPixel(i + 0 * 144, color0);
        leds.setPixel(i + 1 * 144, color1);
        leds.setPixel(i + 2 * 144, color2);
        leds.setPixel(i + 3 * 144, color3);
        leds.setPixel(i + 4 * 144, color4);
        leds.setPixel(i + 5 * 144, color5);
        leds.setPixel(i + 6 * 144, color6);
        leds.setPixel(i + 7 * 144, color7);

        if (i > 0) {
            i = j - 1;
            leds.setPixel(i + 0 * 144, OFF);
            leds.setPixel(i + 1 * 144, OFF);
            leds.setPixel(i + 2 * 144, OFF);
            leds.setPixel(i + 3 * 144, OFF);
            leds.setPixel(i + 4 * 144, OFF);
            leds.setPixel(i + 5 * 144, OFF);
            leds.setPixel(i + 6 * 144, OFF);
            leds.setPixel(i + 7 * 144, OFF);
        }

        leds.show();
        //delayMicroseconds(40000);
        delayMicroseconds(10000);
    }
}

void starsRbg(int star_position, int red, int green, int blue) {
    int color = (red << 16) + (green << 8) + blue;

    int i = 0;
    //for (int i=0; i < leds.numPixels(); i++) {
    for (int j = 0; j < 144; j++) {
        i = j;
        leds.setPixel(i + 0 * 144, color);
        leds.setPixel(i + 1 * 144, color);
        leds.setPixel(i + 2 * 144, color);
        leds.setPixel(i + 3 * 144, color);
        leds.setPixel(i + 4 * 144, color);
        leds.setPixel(i + 5 * 144, color);
        leds.setPixel(i + 6 * 144, color);
        leds.setPixel(i + 7 * 144, color);

        if (i > 0) {
            i = j - 1;
            leds.setPixel(i + 0 * 144, OFF);
            leds.setPixel(i + 1 * 144, OFF);
            leds.setPixel(i + 2 * 144, OFF);
            leds.setPixel(i + 3 * 144, OFF);
            leds.setPixel(i + 4 * 144, OFF);
            leds.setPixel(i + 5 * 144, OFF);
            leds.setPixel(i + 6 * 144, OFF);
            leds.setPixel(i + 7 * 144, OFF);
        }

        leds.show();
        //delayMicroseconds(40000);
        delayMicroseconds(10000);
    }
}
