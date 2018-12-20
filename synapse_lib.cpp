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
    rate = _rate;

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

    if (rate >= 10) {
        //  Serial.println("rate gt.e 10");
        Chase();
    } else if (rate >= 0) {
        //  Serial.println("rate gt.e 0");
        if (drop_count > 0) {
            Chase();
            drop_count -= 1;
        } else {
            drop_count = max(rate, 1);
        }
    } else if (rate > -10) {
//    Serial.println("rate gt -10");
//    Serial.print("drop count: ");
//    Serial.println(drop_count);
        if (drop_count == 0) {
            Chase();
            drop_count = max(-rate, 10);
        } else {
            drop_count -= 1;
        }
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
    rate = random(1, 5);
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

void Synapse::SetPixel(int pixel_num, int red, int green, int blue) {

    int color = (red << 16) + (green << 8) + blue;

    _strip.setPixel(pixel_num, color);

}

