//
// Created by tom on 1/5/19.
//

#include <OctoWS2811.h>
#include "synapse_lib.h"
#include "rgb_colors.h"
#include "jellyfish.h"

Jellyfish::Jellyfish(OctoWS2811 _strip,
                   int _max_synapses) : strip(_strip),
                                       max_synapses(_max_synapses) {

    synapses = new synapsepointer[max_synapses];

//    strand_index = 0;
//    color_index = 0;
//    level_index = 0;
//    level_direction = 0;
//    current_color = RED;
//    ramp_up_color = current_color;
//    hold_color = ramp_up_color;
//    ramp_down_color = hold_color;
//    status = STOPPED;
}


bool Jellyfish::addSynapse(int first, int last, RGB color, float fade_factor,
                           int rate) {

    if (synapse_count < max_synapses) {
        // create with 'new' to ensure strand objects persist beyond function call
        // i.e. object memory is allocated on stack
        synapses[synapse_count] = new Synapse(strip, first, last,
                color, fade_factor, rate);
        synapse_count++;
        return true;
    } else {
        return false;
    }
}

//running_status Jellyfish::synapseChaseStep() {
int Jellyfish::synapseChaseStep() {
    //running_status status;
    int status;
    for (int i = 0; i < synapse_count; i++) {
        status = (int) synapses[i]->chaseStep();
    }

    return status;
}

void Jellyfish::setRandomTailFactorEnabled(bool tf_value) {
    for (int i = 0; i < synapse_count; i++) {
        synapses[i]->random_tail_factor_enable = tf_value;
    }
}

void Jellyfish::setRandomStartDelayEnabled(bool tf_value) {
    for (int i = 0; i < synapse_count; i++) {
        synapses[i]->random_start_delay_enable = tf_value;
    }
}

void Jellyfish::setTailFactor(float _tail_factor) {
    for (int i = 0; i < synapse_count; i++) {
        synapses[i]->tail_factor = _tail_factor;
    }
}