//
// Created by tom on 1/5/19.
//

#ifndef JELLYFISH_3_JELLYFISH_H
#define JELLYFISH_3_JELLYFISH_H

#include "strand.h"
#include "rgb_colors.h"

typedef Synapse *synapsepointer;

class Jellyfish {
    OctoWS2811 strip;

    int max_synapses;
    synapsepointer *synapses;
    int synapse_count = 0;

//    int strand_index = 0;
//    int color_index = 0;
//    int level_index = 0;
//    int level_direction = 0;
//    RGB current_color = RED;
//    RGB ramp_up_color = current_color;
//    RGB hold_color = ramp_up_color;
//    RGB ramp_down_color = hold_color;

//    running_status status = STOPPED;

public:
    Jellyfish(OctoWS2811 _strip, int _max_synapses);

    Jellyfish();

//private:
//    running_status lighthouse();

//    void nextStrand();
//
//    RGB nextColor();
//
//    void pulseStrand();
//
//    RGB scaleBrightness(RGB _color, int level);
//
//    RGB
//    crossFadeColor(RGB start_color, RGB end_color, int fade_amount,
//                   int fade_steps);
//
//    running_status strandsChaseStep();
//
//    void strandsSetColor(RGB color);
//
//    void strandsSetWipe(bool wipe);

    bool addSynapse( int first, int last, RGB color, float fade_factor,
                    int rate);

    void setRandomTailFactorEnabled(bool wipe);

    void setRandomStartDelayEnabled(bool tf_value);

    void setTailFactor(float _tail_factor);

    //running_status synapseChaseStep();
    int synapseChaseStep();
};

#endif //JELLYFISH_3_JELLYFISH_H
