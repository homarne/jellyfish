#ifndef JELLYFISH_3_STRAND_H
#define JELLYFISH_3_STRAND_H

#include <OctoWS2811.h>
#include "rgb_colors.h"
#include "synapse_lib.h"

enum chase_direction {
    FORWARD, REVERSE
};

class Strand {
    OctoWS2811 strip;

    int strip_start_position;
    int strip_end_position;
    int position;
    int last_position;

    // frame rate control
    int rate;
    int step;
    int scaled_next_drop;
    int scaled_drop_one_frame_every;

    int rgbToInt(RGB color);

    void setPixel(int position, RGB color);

public:
    RGB color;
    chase_direction direction;

    Strand(OctoWS2811 _strip, int first, int _length, RGB color, int rate,
           chase_direction direction, int chase_length);

    Strand();

    running_status chase_step();

    void chase();

    int adjustChaseDirection(int position);

    //void setColor(RGB _color);
    void setFrameRate(int _frame_rate);

    frame_status checkDroppedFrame();

    void setColor(RGB _color);

    void setAll(RGB _color);

    void setWipe(bool _wipe);

    int chase_length;
    bool wipe;
};

#endif
