//
// Created by tom on 1/2/19.
//

#include <OctoWS2811.h>
#include "strand.h"
#include "rgb_colors.h"
#include "monolith.h"


Monolith::Monolith(OctoWS2811 _strip,
                   int _max_strands) : strip(_strip),
                                       max_strands(_max_strands) {

    strands = new strandpointer[max_strands];

    strand_index = 0;
    color_index = 0;
    level_index = 0;
    level_direction = 0;
    current_color = RED;
    ramp_up_color = current_color;
    hold_color = ramp_up_color;
    ramp_down_color = hold_color;
    setFrameRate(rate);
    status = STOPPED;
}


bool Monolith::addStrand(int first,
                         int length,
                         RGB _color,
                         int _rate,
                         chase_direction _direction,
                         int _chase_length) {

    if (strand_count < max_strands) {
        // create with 'new' to ensure strand objects persist beyond function call
        // i.e. object memory is allocated on stack
        strands[strand_count] = new Strand(strip, first, length, _color, _rate,
                                           _direction, _chase_length);
        strand_count++;
        return true;
    } else {
        return false;
    }
}


running_status Monolith::strandsChaseStep() {
    running_status status;
    for (int i = 0; i < strand_count; i++) {
        status = strands[i]->chase_step();
    }

    if (chase_color_rotate_enable == true){
        if (status == LAST_PIXEL) {
            RGB color = nextColor();
            strandsSetColor(color);
        }
    }

    return status;
}


running_status Monolith::lighthouse() {
    RGB color;
    running_status status;

    if (checkDroppedFrame() == DROP) {
        return (running_status) RUNNING;
    }

    // the current strand (strand_index) continues at full brightness...
    int next_strand = (strand_index + 1);
    if (next_strand > 7) { next_strand = next_strand - 8; }

    // ramp up
    color = scaleBrightness(ramp_up_color, level_index);
    strands[next_strand]->setAll(color);

    int last_strand = (strand_index - 1);
    if (last_strand < 0) { last_strand = last_strand + 8; }

    // ramp down
    color = scaleBrightness(ramp_down_color, 100 - level_index);
    strands[last_strand]->setAll(color);

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

void Monolith::nextStrand() {
    if ((strand_index >= 7) || (strand_index < 0)) {
        strand_index = 0;
    } else {
        strand_index++;
    }
}


RGB Monolith::nextColor() {
    if ((color_index >= 6) || (color_index < 0)) {
        color_index = 0;
    } else {
        color_index++;
    }

    return palette[color_index];
}


void Monolith::pulseStrand() {
    RGB color;

    color = scaleBrightness(RED, level_index);
    strands[0]->setAll(color);

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

RGB Monolith::scaleBrightness(RGB _color, int level) {
    RGB color;
    int level_factor = brightness[level];
    color.red = (_color.red * level_factor) / 255;
    color.green = (_color.green * level_factor) / 255;
    color.blue = (_color.blue * level_factor) / 255;
    return color;

}

void Monolith::setFrameRate(int _frame_rate) {
    rate = _frame_rate;

    if (rate >= 100) {
        rate = 100;
        scaled_drop_one_frame_every = 0;
    } else {
        scaled_drop_one_frame_every = (100 * 100) / (100 - rate);
    }

    if (rate < 0) rate = 0;

    scaled_next_drop += scaled_drop_one_frame_every;
}

frame_status Monolith::checkDroppedFrame() {

    if (rate == 100) return (frame_status) RUN;
    if (rate == 0) return (frame_status) DROP;

    step += 1;
    if ((step > 100) || (step <= 0)) {
        step = 1;
        scaled_next_drop = scaled_drop_one_frame_every;
    }

    frame_status status = RUN;
    int scaled_step = 100 * step;

    if (scaled_step >= scaled_next_drop) {
        scaled_next_drop += scaled_drop_one_frame_every;
        status = DROP;
    }

    return status;
}

// untested
RGB Monolith::crossFadeColor(RGB start_color, RGB end_color, int fade_amount,
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


void Monolith::strandsSetColor(RGB color) {
    for (int i = 0; i < strand_count; i++) {
        strands[i]->setColor(color);
    }
}

void Monolith::strandsSetWipe(bool wipe) {
    for (int i = 0; i < strand_count; i++) {
        strands[i]->setWipe(wipe);
    }
}
