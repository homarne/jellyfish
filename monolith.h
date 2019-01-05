#ifndef JELLYFISH_3_MONOLITH_H
#define JELLYFISH_3_MONOLITH_H

#include "strand.h"
#include "rgb_colors.h"

#define LIGHTHOUSE_COUNT 48
#define STRAND_CHASE_COUNT 24

typedef Strand *strandpointer;

class Monolith {

    OctoWS2811 strip;

    int max_strands;
    strandpointer *strands;
    int strand_count = 0;

    int strand_index = 0;
    int color_index = 0;
    int level_index = 0;
    int level_direction = 0;
    RGB current_color = RED;
    RGB ramp_up_color = current_color;
    RGB hold_color = ramp_up_color;
    RGB ramp_down_color = hold_color;

    running_status status = STOPPED;

public:
    Monolith(OctoWS2811 _strip, int _max_strands);

    Monolith();

//private:
    running_status lighthouse();

    void nextStrand();

    RGB nextColor();

    void pulseStrand();

    RGB scaleBrightness(RGB _color, int level);

    RGB
    crossFadeColor(RGB start_color, RGB end_color, int fade_amount,
                   int fade_steps);

    running_status strandsChaseStep();

    void strandsSetColor(RGB color);

    void strandsSetWipe(bool wipe);

    bool addStrand(int first, int length, RGB _color, int _rate,
                   chase_direction _direction, int _chase_length);
};

#endif //JELLYFISH_3_MONOLITH_H
