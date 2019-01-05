#include <OctoWS2811.h>
#include "synapse_lib.h"
#include "rgb_colors.h"
#include "strand.h"


Strand::Strand(OctoWS2811 _strip,
               int first,
               int length,
               RGB _color,
               int _rate,
               chase_direction _direction,
               int _chase_length) : strip(_strip) {
    strip_start_position = first;
    strip_end_position = strip_start_position + length;
    position = first;
    color = _color;
    setFrameRate(_rate);
    direction = _direction;
    chase_length = _chase_length;
    wipe = false;
}

//Strand::Strand(){}

running_status Strand::chase_step() {
    if (checkDroppedFrame() == DROP) {
        return (running_status) RUNNING;
    }

    chase();

    if (position == strip_end_position) {
        return (running_status) LAST_PIXEL;
    }

    return RUNNING;
}

void Strand::chase() {
    if (position >= strip_end_position) {
        position = strip_start_position;
    }

    int adjusted_position = adjustChaseDirection(position);
    setPixel(adjusted_position, color);

    if (wipe == false) {
        int off_position = position - chase_length;
        if (off_position < strip_start_position) {
            off_position =
                    strip_end_position - (strip_start_position - off_position);
        }
        adjusted_position = adjustChaseDirection(off_position);
        setPixel(adjusted_position, OFF);
    }


    position++;
}

void Strand::setAll(RGB color) {
    for (int i = strip_start_position; i < strip_end_position; i++) {
        setPixel(i, color);
    }
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

void Strand::setPixel(int position, RGB color) {
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

void Strand::setColor(RGB _color) {
    color = _color;
}

void Strand::setWipe(bool _wipe) {
    wipe = _wipe;
}

