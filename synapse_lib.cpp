//#include <Adafruit_NeoPixel.h>
#include <OctoWS2811.h>
#include "synapse_lib.h"

Synapse::Synapse(
        //Adafruit_NeoPixel &strip,
        OctoWS2811 &strip,
        int _first,
        int _last,
        int _red,
        int _green,
        int _blue,
        float _fade_factor,
        int _rate) : _strip(strip) {
    //_strip = strip;
    red = _red;
    green = _green;
    blue = _blue;
//  fade_factor = fade_factor;
    fade_factor = _fade_factor;
    rate = _rate;

    strip_start_position = _first;
    strip_end_position = _last;

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

    int _color_count = sizeof(synapse_palette) / (sizeof(synapse_palette[0]));
    int _new_color = random(0, _color_count);

    red = synapse_palette[_new_color][0];
    green = synapse_palette[_new_color][1];
    blue = synapse_palette[_new_color][2];
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

    int _fade_count = first_lit_pixel_position;

    int _scaled_red = red;
    int _scaled_green = green;
    int _scaled_blue = blue;
    int _total_color = _scaled_red + _scaled_green + _scaled_blue;

    // walk back from the head and program dimmer pixels
    // until either strip_start_position is reached or brightness reaches zero
    while (_fade_count > strip_start_position && _total_color > 0) {
        _scaled_red = (float) _scaled_red * fade_factor;
        _scaled_green = (float) _scaled_green * fade_factor;
        _scaled_blue = (float) _scaled_blue * fade_factor;
        _total_color = _scaled_red + _scaled_green + _scaled_blue;

        _fade_count -= 1;
        if (_fade_count < strip_end_position)  // don't update pixels beyond the end
        {
            //_strip.setPixelColor(AdjustChaseDirection(_fade_count), _strip.Color(_scaled_red, _scaled_green, _scaled_blue));
            SetPixel(AdjustChaseDirection(_fade_count), _scaled_red, _scaled_green, _scaled_blue);
        }
    }
    last_lit_pixel_position = _fade_count;  // update the tail position
    first_lit_pixel_position += 1;  // move the head forward

}

void Synapse::SetPixel(int pixel_num, int red, int green, int blue) {
    int color = (red << 16) + (green << 8) + blue;

    _strip.setPixel(pixel_num, color);

}

