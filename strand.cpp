#include <OctoWS2811.h>
#include "synapse_lib.h"
#include "rgb_colors.h"
#include "strand.h"


Strand::Strand(OctoWS2811 &_strip,
               int first,
               int length,
               RGB _color,
               int _rate,
               chase_direction _direction) : strip(_strip){
    strip_start_position = first;
    strip_end_position = strip_start_position + length;
    position = first;
    color = _color;
    setFrameRate(_rate);
    direction = _direction;
}

running_status Strand::chase_step() {
    if (checkDroppedFrame() == DROP) {
        return (running_status) RUNNING;
    }
    chase();
    return RUNNING;
}

void Strand::chase(){
    if (position >= strip_end_position){
        position = strip_start_position;
    }

    int adjusted_position = adjustChaseDirection(position);
    setPixel(adjusted_position, color);

    adjusted_position = adjustChaseDirection(last_position);
    setPixel(adjusted_position, OFF);

    last_position = position;
    position ++;
}

int Strand::adjustChaseDirection(int pixel_number) {
    if (direction == FORWARD) {
        return pixel_number;
    } else {
        return strip_start_position + strip_end_position - pixel_number - 1;
    }
}

int Strand::rgbToInt(RGB _color) {
    int color = (_color.red << 16) + (_color.green << 8) + _color.blue;
    return color;
}

void Strand::setPixel(int position, RGB color){
    strip.setPixel(position, rgbToInt(color));
}

void Strand::setFrameRate(int _frame_rate) {
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

frame_status Strand::checkDroppedFrame() {

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