//#include <Adafruit_NeoPixel.h>
#include <OctoWS2811.h>
#include "synapse_lib.h"
#include "rgb_colors.h"

Synapse::Synapse(
        //Adafruit_NeoPixel &strip,
        OctoWS2811 &strip,
        int _first_strip_position,
        int _last_strip_position,
        int _red,
        int _green,
        int _blue,
        float _fade_factor,
        int _rate) : _strip(strip) { // initializer required or object reference
    red = _red;
    green = _green;
    blue = _blue;
    fade_factor = _fade_factor;

    //rate = _rate;
    SetFrameRate(10 * _rate);

    strip_start_position = _first_strip_position;
    strip_end_position = _last_strip_position;

    first_lit_pixel_position = strip_start_position;
    last_lit_pixel_position = strip_start_position;

    drop_count = 0;
    start_delay = 0;
}


void Synapse::ChaseLoop() {
    while (true) {
        ChaseStep();
        _strip.show();
        delay(1);
    }
}

int Synapse::ChaseStep() {
    if (start_delay > 0) {
        start_delay -= 1;
        run_status = DELAYED;
        return 0;
    }

    frame_status status = CheckDroppedFrame();

    if (status == RUN){
        Chase();
    }

    if (last_lit_pixel_position == strip_end_position) {
        start_delay = random(0, 100);
        run_status = LAST_PIXEL;
        return 0;
    } else {
        run_status = RUNNING;
        return 1;
    }
}

void Synapse::Set_Random_Color() {

    int color_count = sizeof(palette) / (sizeof(RGB));
    RGB new_color = palette[random(0, color_count)];

    red = new_color.red;
    green = new_color.green;
    blue = new_color.blue;
}

int Synapse::AdjustChaseDirection(int pixel_number) {
    if (chase_direction == 0) {
        return pixel_number;
    } else {
        return strip_start_position + strip_end_position - pixel_number - 1;
    }
}

void Synapse::Set_Random_Tail() {
    int tail_factor = random(85, 99);
    fade_factor = (float) tail_factor / 100;
}

void Synapse::NewSynapse() {
    Set_Random_Color();
    chase_direction = random(0, 2);
    //chase_direction = 0;
    Set_Random_Tail();
    //rate = random(1, 5);
    SetFrameRate(random(25,75));
}

void Synapse::Chase() {
    if (last_lit_pixel_position >= strip_end_position) {

        NewSynapse();

        first_lit_pixel_position = strip_start_position;
        last_lit_pixel_position = strip_start_position;
    }

    if (first_lit_pixel_position < strip_end_position)
    {
        //_strip.setPixelColor(AdjustChaseDirection(first_lit_pixel_position), _strip.Color(red, green, blue));
        SetPixel(AdjustChaseDirection(first_lit_pixel_position), red, green, blue);

    }

    int fade_position = first_lit_pixel_position;

    int scaled_red = red;
    int scaled_green = green;
    int scaled_blue = blue;
    int color = (scaled_red << 16) + (scaled_green << 8) + scaled_blue;

    // walk back from the head and program dimmer pixels
    // until either strip_start_position is reached or brightness reaches zero
    while (fade_position > strip_start_position && color > 0) {
        scaled_red = (float) scaled_red * fade_factor;
        scaled_green = (float) scaled_green * fade_factor;
        scaled_blue = (float) scaled_blue * fade_factor;
        color = (scaled_red << 16) + (scaled_green << 8) + scaled_blue;

        fade_position -= 1;
        if (fade_position < strip_end_position)  // don't update pixels beyond the end
        {
            //_strip.setPixelColor(AdjustChaseDirection(fade_position), _strip.Color(scaled_red, scaled_green, scaled_blue));
            SetPixel(AdjustChaseDirection(fade_position), scaled_red, scaled_green, scaled_blue);
        }
    }
    last_lit_pixel_position = fade_position;  // update the tail position
    first_lit_pixel_position += 1;  // move the head forward

}

void Synapse::SetFrameRate(int _frame_rate) {
    rate = _frame_rate;
    if (rate >= 100) {
        rate = 100;
        scaled_drop_one_frame_every = 0;
    }
    else {
        scaled_drop_one_frame_every = (100 * 100) / (100 - rate);
    }
    if (rate < 0) rate = 0;

    scaled_next_drop += scaled_drop_one_frame_every;
}

frame_status Synapse::CheckDroppedFrame() {

    if (rate == 100) return (frame_status) RUN;
    if (rate == 0) return (frame_status) DROP;

    step += 1;
    if ((step > 100) || (step <= 0)) {
        step = 1;
        scaled_next_drop = scaled_drop_one_frame_every;
    }

    frame_status status = RUN;
    int scaled_step = 100 * step;

    //printf("step: %d  step (scaled): %d  drop every (scaled): %d  next drop (scaled): %d ", step, scaled_step, scaled_drop_one_frame_every, scaled_next_drop);

    if (scaled_step >= scaled_next_drop) {
        scaled_next_drop += scaled_drop_one_frame_every;
        status = DROP;
    }

    //printf("status: %d\n", status);

    return status;
}

void Synapse::SetPixel(int pixel_num, int red, int green, int blue) {

    int color = (red << 16) + (green << 8) + blue;

    _strip.setPixel(pixel_num, color);

}

