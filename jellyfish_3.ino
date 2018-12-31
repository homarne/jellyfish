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

#if MODE == TEST_MODE || MODE == MONOLITH
const int ledsPerStrip = 2 * 144;
#else
const int ledsPerStrip = 1*144;
#endif

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;
int position = 0;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

#if MODE == JELLYFISH || MODE == JELLYFISH_HEAD

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


#if MODE == MONOLITH
#define urate 100

// upper monolith is 110 LEDs
Strand upper_1 = Strand(leds, 0, 110, GREEN, urate, FORWARD, 5);
Strand upper_2 = Strand(leds, 144 + 34, 110, GREEN, urate, REVERSE, 5);
Strand upper_3 = Strand(leds, 288, 110, GREEN, urate, FORWARD, 5);
Strand upper_4 = Strand(leds, 432 + 34, 110, GREEN, urate, REVERSE, 5);
Strand upper_5 = Strand(leds, 576, 110, GREEN, urate, FORWARD, 5);
Strand upper_6 = Strand(leds, 720 + 34, 110, GREEN, urate, REVERSE, 5);
Strand upper_7 = Strand(leds, 864, 110, GREEN, urate, FORWARD, 5);
Strand upper_8 = Strand(leds, 1008 + 34, 110, GREEN, urate, REVERSE, 5);

#define lrate 31
// Lower monolith is 34 LEDs
Strand lower_1 = Strand(leds, 0 + 110, 34, BLUE, lrate, REVERSE, 5);
Strand lower_2 = Strand(leds, 144, 34, BLUE, lrate, FORWARD, 5);
Strand lower_3 = Strand(leds, 288 + 110, 34, BLUE, lrate, REVERSE, 5);
Strand lower_4 = Strand(leds, 432, 34, BLUE, lrate, FORWARD, 5);
Strand lower_5 = Strand(leds, 576 + 110, 34, BLUE, lrate, REVERSE, 5);
Strand lower_6 = Strand(leds, 720, 34, BLUE, lrate, FORWARD, 5);
Strand lower_7 = Strand(leds, 864 + 110, 34, BLUE, lrate, REVERSE, 5);
Strand lower_8 = Strand(leds, 1008, 34, BLUE, lrate, FORWARD, 5);

#define LIGHTHOUSE_COUNT 48
#define STRAND_CHASE_COUNT 24
int strand_index = 0;
int color_index = 0;
int level_index = 0;
int level_direction = 0;
RGB current_color = RED;
RGB ramp_up_color = current_color;
RGB hold_color = ramp_up_color;
RGB ramp_down_color = hold_color;
int upper_mode = 0;
int upper_mode_count = LIGHTHOUSE_COUNT;
Strand *upper_strands[8];
Strand *lower_strands[8];
running_status status = STOPPED;
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

#if MODE == JELLYFISH_HEAD
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

#if MODE == MONOLITH
    initUpperStrandArray();
    initLowerStrandArray();
    strandsSetColor(upper_strands, RED);
    strandsSetColor(lower_strands, WHITE);
    strandsSetWipe(upper_strands, true);
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


#if MODE == JELLYFISH || MODE == JELLYFISH_HEAD
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

#if MODE == MONOLITH

    if (upper_mode == 0) {
        if (upper_mode_count <= 0) {
            upper_mode = 1;
            upper_mode_count = STRAND_CHASE_COUNT;
        } else {
            status = lighthouse();
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
            status = strandsChaseStep(upper_strands);


            if (status == LAST_PIXEL) {
                RGB color = nextColor();
                strandsSetColor(upper_strands, color);
                upper_mode_count--;
            }
        }
    }

    strandsChaseStep(lower_strands);

    leds.show();
}

#endif


#if MODE == MONOLITH

running_status lighthouse() {
    RGB color;
    running_status status;

    // the current strand (strand_index) continues at full brightness...
    int next_strand = (strand_index + 1);
    if (next_strand > 7) { next_strand = next_strand - 8; }

    // ramp up
    color = scaleBrightness(ramp_up_color, level_index);
    upper_strands[next_strand]->setAll(color);

    int last_strand = (strand_index - 1);
    if (last_strand < 0) { last_strand = last_strand + 8; }

    // ramp down
    color = scaleBrightness(ramp_down_color, 100 - level_index);
    upper_strands[last_strand]->setAll(color);

    if (level_index >= 100) {
        level_index = 0;
        status = LAST_PIXEL;
    } else {
        level_index++;
        if ((level_index % 3) == 0) { level_index++; }
        status = RUNNING;
    }

    // if fade complete, update colors and move to next strand
    if (level_index == 0) {
        // colors are updated so as to ensure that the color ramped down
        // is the same as the color ramped up
        ramp_down_color = hold_color;
        hold_color = ramp_up_color;
        ramp_up_color = current_color;

        nextStrand();

        // rotation complete, move to next color
        if (strand_index == 0) {
            current_color = nextColor();
        }
    }
    return status;


}


void nextStrand() {
    if ((strand_index >= 7) || (strand_index < 0)) {
        strand_index = 0;
    } else {
        strand_index++;
    }
}


RGB nextColor() {
    if ((color_index >= 6) || (color_index < 0)) {
        color_index = 0;
    } else {
        color_index++;
    }

    return palette[color_index];
}


void pulseStrand() {
    RGB color;

    color = scaleBrightness(RED, level_index);
    upper_strands[0]->setAll(color);

    if (level_index >= 100) {
        level_direction = 1;
    }
    if (level_index <= 0) {
        level_direction = 0;
    }
    if (level_direction == 0) {
        level_index++;
    } else {
        level_index--;
    }
}

RGB scaleBrightness(RGB _color, int level) {
    RGB color;
    int level_factor = brightness[level];
    color.red = (_color.red * level_factor) / 255;
    color.green = (_color.green * level_factor) / 255;
    color.blue = (_color.blue * level_factor) / 255;
    return color;

}

// untested
RGB crossFadeColor(RGB start_color, RGB end_color, int fade_amount,
                   int fade_steps) {
    if (fade_steps < 1) { fade_steps = 1; }
    if (fade_steps > 64) { fade_steps = 64; }
    if (fade_amount <= 0) { fade_amount = 0; }
    if (fade_amount > fade_steps) { fade_amount = fade_steps; }

    int red_distance = end_color.red - start_color.red;
    int green_distance = end_color.green - start_color.green;
    int blue_distance = end_color.blue - start_color.blue;
    int red_step = red_distance / fade_steps;
    int green_step = green_distance / fade_steps;
    int blue_step = green_distance / fade_steps;

    RGB crossfade_color;
    crossfade_color.red = start_color.red + (fade_amount * red_step);
    crossfade_color.green = start_color.green + (fade_amount * green_step);
    crossfade_color.blue = start_color.blue + (fade_amount * blue_step);
    return crossfade_color;


}

void initUpperStrandArray() {
    upper_strands[0] = &upper_1;
    upper_strands[1] = &upper_2;
    upper_strands[2] = &upper_3;
    upper_strands[3] = &upper_4;
    upper_strands[4] = &upper_5;
    upper_strands[5] = &upper_6;
    upper_strands[6] = &upper_7;
    upper_strands[7] = &upper_8;
}

void initLowerStrandArray() {
    lower_strands[0] = &lower_1;
    lower_strands[1] = &lower_2;
    lower_strands[2] = &lower_3;
    lower_strands[3] = &lower_4;
    lower_strands[4] = &lower_5;
    lower_strands[5] = &lower_6;
    lower_strands[6] = &lower_7;
    lower_strands[7] = &lower_8;
}

running_status strandsChaseStep(Strand **strands) {
    running_status status;
    for (int i = 0; i < 8; i++) {
        status = strands[i]->chase_step();
    }

    return status;
}

void strandsSetColor(Strand **strands, RGB color) {
    for (int i = 0; i < 8; i++) {
        strands[i]->setColor(color);
    }
}

void strandsSetWipe(Strand **strands, bool wipe) {
    for (int i = 0; i < 8; i++) {
        strands[i]->setWipe(wipe);
    }
}

#endif
//---------- Test Code ---------------

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
