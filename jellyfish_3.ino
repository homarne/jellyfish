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

void setup() {

    initializeJellyfish();

    leds.begin();
    leds.show();
}

void loop() {
    jellyfish.synapseChaseStep();
    leds.show();
}

#endif

#if MODE == JELLYFISH_HEAD

Jellyfish jellyfish = Jellyfish(leds, 4);
Jellyfish jellyfish_head = Jellyfish(leds, 4);

void setup() {

    initializeJellyfishHead();

    leds.begin();
    leds.show();
}

void loop() {
    jellyfish.synapseChaseStep();
    jellyfish_head.synapseChaseStep();
    leds.show();
}

#endif

#if MODE == MONOLITH

#define LIGHTHOUSE_COUNT 48
#define STRAND_CHASE_COUNT 24

    int upper_mode = 0;
    int upper_mode_count = LIGHTHOUSE_COUNT;
    running_status status = STOPPED;

    Monolith upper_monolith = Monolith(leds, 8);
    Monolith lower_monolith = Monolith(leds, 8);

void setup() {

    initializeMonolith();

    upper_monolith.strandsSetColor(RED);
    upper_monolith.strandsSetWipe(true);

    lower_monolith.strandsSetColor(WHITE);

    leds.begin();
    leds.show();
}

void loop() {

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

}

#endif

#if MODE == TEST_MODE

// simplest possible code to exercise the OctoWS2811
// set leds_per_strip above to match the number of leds
// attached to any one channel of the OctoWS2811

#define _RED    0xFF0000
#define _GREEN  0x00FF00
#define _BLUE   0x0000FF

void setup() {
    leds.begin();
    leds.show();
}

void loop() {
    //int microsec = 2000000 / leds.numPixels();  // change them all in 2 seconds
    int microsec = 1;  // go fast!

    // uncomment for voltage controlled speed
    // millisec = analogRead(A9) / 40;

    colorWipe(_RED, microsec);
    colorWipe(_GREEN, microsec);
    colorWipe(_BLUE, microsec);
}

void colorWipe(int color, int wait) {
    for (int i = 0; i < leds.numPixels(); i++) {
        leds.setPixel(i, color);
        leds.show();
        delayMicroseconds(wait);
    }
}

#endif

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

    upper_monolith.addStrand(0, 110, GREEN, rate, FORWARD, 5);
    upper_monolith.addStrand( 144, 110, GREEN, rate, FORWARD, 5);
    upper_monolith.addStrand(288, 110, GREEN, rate, FORWARD, 5);
    upper_monolith.addStrand(432, 110, GREEN, rate, FORWARD, 5);
    upper_monolith.addStrand(576, 110, GREEN, rate, FORWARD, 5);
    upper_monolith.addStrand(720, 110, GREEN, rate, FORWARD, 5);
    upper_monolith.addStrand(864, 110, GREEN, rate, FORWARD, 5);
    upper_monolith.addStrand(1008, 110, GREEN, rate, FORWARD, 5);

    rate = 31;

    lower_monolith.addStrand(0 + 110, 34, BLUE, rate, FORWARD, 5);
    lower_monolith.addStrand(144 + 110, 34, BLUE, rate, FORWARD, 5);
    lower_monolith.addStrand(288 + 110, 34, BLUE, rate, FORWARD, 5);
    lower_monolith.addStrand(432 + 110, 34, BLUE, rate, FORWARD, 5);
    lower_monolith.addStrand(576 + 110, 34, BLUE, rate, FORWARD, 5);
    lower_monolith.addStrand(720 + 110, 34, BLUE, rate, FORWARD, 5);
    lower_monolith.addStrand(864 + 110, 34, BLUE, rate, FORWARD, 5);
    lower_monolith.addStrand(1008 + 110, 34, BLUE, rate, FORWARD, 5);
}

#endif

