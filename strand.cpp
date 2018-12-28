#include <OctoWS2811.h>
#include "synapse_lib.h"
#include "rgb_colors.h"
#include "strand.h"


Strand::Strand(OctoWS2811 &_strip,
               int first,
               int length,
               RGB _color,
               chase_direction _direction) : strip(_strip){
    strip_start_position = first;
    strip_end_position = strip_start_position + length;
    position = first;
    color = _color;
    direction = _direction;
}

running_status Strand::chase_step() {
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