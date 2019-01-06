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
#include "monolith.h"
#include "jellyfish.h"

#define TEST_MODE 0
#define JELLYFISH 1
#define JELLYFISH_HEAD 2
#define MONOLITH 3

#define MODE JELLYFISH

const int ledsPerStrip = 1 * 144;

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;
int position = 0;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

#if MODE == JELLYFISH

Jellyfish jellyfish = Jellyfish(leds, 8);

#endif

#if MODE == JELLYFISH_HEAD

Jellyfish jellyfish = Jellyfish(leds, 4);
Jellyfish jellyfish_head = Jellyfish(leds, 4);

#endif

#if MODE == MONOLITH



#define LIGHTHOUSE_COUNT 48
#define STRAND_CHASE_COUNT 24

int upper_mode = 0;
int upper_mode_count = LIGHTHOUSE_COUNT;
running_status status = STOPPED;

Monolith upper_monolith = Monolith(leds, 8);
Monolith lower_monolith = Monolith(leds, 8);

#endif

void setup() {

#if MODE == JELLYFISH

    initializeJellyfish();

#endif

#if MODE == JELLYFISH_HEAD

    initializeJellyfishHead();

#endif

#if MODE == MONOLITH

    initializeMonolith();

    upper_monolith.strandsSetColor(RED);
    upper_monolith.strandsSetWipe(true);

    lower_monolith.strandsSetColor(WHITE);

#endif

    leds.begin();
    leds.show();
}

void loop() {

#if MODE == TEST_MODE
    // basic tests for eight 144-led strands connected to OctoWS2811
    int microseconds = 1000 / leds.numPixels();  // change them all in 2 seconds
    //doColorWipe(microseconds);
    doWhiteWipe(microseconds);
    //doColorStars(microseconds);
    //doRgbStars(microseconds);
    delayMicroseconds(10000);
#endif

#if MODE == JELLYFISH

    jellyfish.synapseChaseStep();
    leds.show();

#endif

#if MODE == JELLYFISH_HEAD

    jellyfish.synapseChaseStep();
    jellyfish_head.synapseChaseStep();
    leds.show();

#endif

#if MODE == MONOLITH

    if (upper_mode == 0) {
        if (upper_mode_count <= 0) {
            upper_mode = 1;
            upper_mode_count = STRAND_CHASE_COUNT;
        } else {
            //status = lighthouse();
            status = upper_monolith.lighthouse();
            if (status == LAST_PIXEL) {
                upper_mode_count--;
            }
        }
    }

    if (upper_mode == 1) {
        if (upper_mode_count <= 0) {
            upper_mode = 0;
            upper_mode_count = LIGHTHOUSE_COUNT;
        } else {
            //status = strandsChaseStep(upper_strands);
            status = upper_monolith.strandsChaseStep();


            if (status == LAST_PIXEL) {
                //RGB color = nextColor();
                //strandsSetColor(upper_strands, color);
                RGB color = upper_monolith.nextColor();
                upper_monolith.strandsSetColor(color);
                upper_mode_count--;
            }
        }
    }

    //strandsChaseStep(lower_strands);
    lower_monolith.strandsChaseStep();

    leds.show();

#endif

} // end loop

// ----------------- utility functions ----------------------

#if MODE == JELLYFISH

void initializeJellyfish() {
    jellyfish.addSynapse(0, 144, RED, 0.8, 100);
    jellyfish.addSynapse(144, 288, GREEN, 0.8, 70);
    jellyfish.addSynapse(288, 432, BLUE, 0.8, 40);
    jellyfish.addSynapse(432, 576, YELLOW, 0.8, 20);
    jellyfish.addSynapse(576, 720, CYAN, 0.8, 30);
    jellyfish.addSynapse(720, 864, MAGENTA, 0.8, 70);
    jellyfish.addSynapse(864, 1008, RED, 0.8, 50);
    jellyfish.addSynapse(1008, 1152, GREEN, 0.8, 60);
}

#endif

#if MODE == JELLYFISH_HEAD

void initializeJellyfishHead() {
    jellyfish.addSynapse(0, 144, RED, 0.8, 100);
    jellyfish.addSynapse(144, 288, GREEN, 0.8, 70);
    jellyfish.addSynapse(288, 432, BLUE, 0.8, 40);
    jellyfish.addSynapse(432, 576, YELLOW, 0.8, 20);

    jellyfish_head.addSynapse(576, 720, CYAN, 0.8, 30);
    jellyfish_head.addSynapse(720, 864, MAGENTA, 0.8, 70);
    jellyfish_head.addSynapse(864, 1008, RED, 0.8, 50);
    jellyfish_head.addSynapse(1008, 1152, GREEN, 0.8, 60);

    jellyfish_head.setRandomStartDelayEnabled(false);
    jellyfish_head.setRandomTailFactorEnabled(false);
    jellyfish_head.setTailFactor(0.99);
}

#endif


#if MODE == MONOLITH

void initializeMonolith(){

    int rate = 100;

    upper_monolith.addStrand(0, 110, GREEN, urate, FORWARD, 5);
    upper_monolith.addStrand( 144, 110, GREEN, urate, FORWARD, 5);
    upper_monolith.addStrand(288, 110, GREEN, urate, FORWARD, 5);
    upper_monolith.addStrand(432, 110, GREEN, urate, FORWARD, 5);
    upper_monolith.addStrand(576, 110, GREEN, urate, FORWARD, 5);
    upper_monolith.addStrand(720, 110, GREEN, urate, FORWARD, 5);
    upper_monolith.addStrand(864, 110, GREEN, urate, FORWARD, 5);
    upper_monolith.addStrand(1008, 110, GREEN, urate, FORWARD, 5);

    rate = 31;

    lower_monolith.addStrand(0 + 110, 34, BLUE, lrate, FORWARD, 5);
    lower_monolith.addStrand(144 + 110, 34, BLUE, lrate, FORWARD, 5);
    lower_monolith.addStrand(288 + 110, 34, BLUE, lrate, FORWARD, 5);
    lower_monolith.addStrand(432 + 110, 34, BLUE, lrate, FORWARD, 5);
    lower_monolith.addStrand(576 + 110, 34, BLUE, lrate, FORWARD, 5);
    lower_monolith.addStrand(720 + 110, 34, BLUE, lrate, FORWARD, 5);
    lower_monolith.addStrand(864 + 110, 34, BLUE, lrate, FORWARD, 5);
    lower_monolith.addStrand(1008 + 110, 34, BLUE, lrate, FORWARD, 5);
}

#endif
//---------------------- Test Code -------------------------------

void doColorWipe(int microsecond) {

    colorWipe(RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, RED, microsecond);
    colorWipe(GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE, RED, RED, microsecond);
    colorWipe(BLUE, YELLOW, CYAN, MAGENTA, WHITE, RED, RED, GREEN, microsecond);
    colorWipe(YELLOW, CYAN, MAGENTA, WHITE, RED, WHITE, RED, GREEN,
              microsecond);
    colorWipe(CYAN, MAGENTA, WHITE, RED, WHITE, RED, GREEN, BLUE, microsecond);
    colorWipe(CYAN, MAGENTA, WHITE, RED, GREEN, BLUE, YELLOW, CYAN,
              microsecond);
    colorWipe(MAGENTA, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA,
              microsecond);
    colorWipe(WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE,
              microsecond);
}

void doWhiteWipe(int microseconds) {
    colorWipe(WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
              microseconds);
}

void doColorStars(int microsecond) {
    colorStars(position, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, RED, WHITE,
               microsecond);
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

void setPixel(int pixel, RGB color) {
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
