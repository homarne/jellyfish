#include <OctoWS2811.h>
#include "rgb_colors.h"
#include "synapse_lib.h"

enum chase_direction {FORWARD, REVERSE};

class Strand{
    OctoWS2811 &strip;

    int strip_start_position;
    int strip_end_position;
    int position;
    int last_position;

    int rgbToInt(RGB color);
    void setPixel(int position, RGB color);

public:
    RGB color;
    chase_direction direction;

    Strand(OctoWS2811 &_strip, int first, int _length, RGB color, chase_direction direction);
    running_status chase_step();
    void chase();
    int adjustChaseDirection(int position);


};
