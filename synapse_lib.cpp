//#include <Adafruit_NeoPixel.h>
#include <OctoWS2811.h>
#include "synapse_lib.h"
#include "rgb_colors.h"

Synapse::Synapse(
        //Adafruit_NeoPixel &strip,
        OctoWS2811 &strip,
        int _first_strip_position,
        int _last_strip_position,
        RGB _color,
        float _fade_factor,
        int _rate) : _strip(strip) { // initializer for object reference

    setColor(_color);

    tail_factor = _fade_factor;

    setFrameRate(_rate);

    strip_start_position = _first_strip_position;
    strip_end_position = _last_strip_position;

    first_lit_pixel_position = strip_start_position;
    last_lit_pixel_position = strip_start_position;

    start_delay = 0;
}

void Synapse::chaseLoop() {
    while (true) {
        chaseStep();
        _strip.show();
        delay(1);
    }
}

void Synapse::setRandomColor() {

    int color_count = sizeof(palette) / (sizeof(RGB));
//    RGB new_color = palette[random(0, color_count)];
//
//    red = new_color.red;
//    green = new_color.green;
//    blue = new_color.blue;
    setColor(palette[random(0, color_count)]);
}

int Synapse::adjustChaseDirection(int pixel_number) {
    if (chase_direction == 0) {
        return pixel_number;
    } else {
        return strip_start_position + strip_end_position - pixel_number - 1;
    }
}

void Synapse::setRandomTail() {
    int tail_factor = random(tail_factor_min, tail_factor_max);
    tail_factor = (float) tail_factor / 100;
}

void Synapse::newSynapse() {
    if (random_start_delay_enable) start_delay = random(0, 100);
    if (random_color_enable) setRandomColor();
    if (random_chase_direction_enable) chase_direction = random(0, 2);
    if (random_tail_factor_enable) setRandomTail();
    if (random_rate_enable) setFrameRate(random(rate_min, rate_max));
}

int Synapse::chaseStep() {
    if (start_delay > 0) {
        start_delay -= 1;
        return (running_status) DELAYED;
    }

    if (checkDroppedFrame() == DROP) {
        return (running_status) RUNNING;
    }

    chase();

    if (last_lit_pixel_position == strip_end_position) {
        newSynapse();
        return (running_status) LAST_PIXEL;
    }

    return (running_status) RUNNING;
}

void Synapse::chase() {
    if (last_lit_pixel_position >= strip_end_position) {

        first_lit_pixel_position = strip_start_position;
        last_lit_pixel_position = strip_start_position;
    }

    if (first_lit_pixel_position < strip_end_position) {
        //_strip.setPixelColor(adjustChaseDirection(first_lit_pixel_position), _strip.Color(red, green, blue));
        int pixel_position = adjustChaseDirection(first_lit_pixel_position);
        setPixel(pixel_position, red, green, blue);
    }

    int fade_position = first_lit_pixel_position;

    int scaled_red = red;
    int scaled_green = green;
    int scaled_blue = blue;

    int color = scaled_red + scaled_green + scaled_blue;

    // walk back from the first lit pixel and set dimmer pixels
    // until either strip_start_position is reached or brightness reaches zero
    while (fade_position > strip_start_position && color > 0) {
        scaled_red = (float) scaled_red * tail_factor;
        scaled_green = (float) scaled_green * tail_factor;
        scaled_blue = (float) scaled_blue * tail_factor;
        color = scaled_red + scaled_green + scaled_blue;

        fade_position -= 1;

        if (fade_position < strip_end_position)
        {
            //_strip.setPixelColor(adjustChaseDirection(fade_position), _strip.Color(scaled_red, scaled_green, scaled_blue));
            int pixel_position = adjustChaseDirection(fade_position);
            setPixel(pixel_position, scaled_red, scaled_green, scaled_blue);
        }
    }

    last_lit_pixel_position = fade_position;
    first_lit_pixel_position += 1;
}

void Synapse::setFrameRate(int _frame_rate) {
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

frame_status Synapse::checkDroppedFrame() {

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

void Synapse::setColor(RGB _color) {
    color = _color;
    red = color.red;
    green = color.green;
    blue = color.blue;
}

void Synapse::setPixel(int pixel_num, int red, int green, int blue) {

    int color = (red << 16) + (green << 8) + blue;

    _strip.setPixel(pixel_num, color);

}




