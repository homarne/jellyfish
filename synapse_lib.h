#ifndef synapse_lib_h
#define synapse_lib_h

//#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>
#include <OctoWS2811.h>
#include "rgb_colors.h"

enum running_status {
    STOPPED, DELAYED, RUNNING, LAST_PIXEL
};
enum frame_status{DROP, RUN};

struct synapse_settings{
    bool random_start_delay_enable;
    int start_delay_min;
    int start_delay_max;
    int start_delay;

    bool random_color_enable;

    bool random_chase_direction_enable;
    int chase_direction;

    bool random_tail_factor_enable;
    int tail_factor_max;
    int tail_factor_min;

    bool random_rate_enable;
    int rate_max;
    int rate_min;
    int rate;
};

class Synapse {
    //Adafruit_NeoPixel _strip;
    OctoWS2811 &_strip;

    int red;
    int green;
    int blue;

    int strip_start_position; // starting pixel number
    int strip_end_position; // ending pixel number

    //int drop_count; // number of steps until one is dropped

    int first_lit_pixel_position; // first lit pixel of the chase
    int last_lit_pixel_position; // last lit pixel of the chase

    // frame rate control
    int step;
    int scaled_next_drop;
    int scaled_drop_one_frame_every;

    RGB color = RED;

public:

    bool random_start_delay_enable = true;
    int start_delay_min;
    int start_delay_max;
    int start_delay; // number of steps to wait before starting chase

    bool random_color_enable = true;

    bool random_chase_direction_enable = true;
    int chase_direction; // 0 = chase up 1 = chase down

    bool random_tail_factor_enable = true;
    int tail_factor_max = 99;
    int tail_factor_min = 85;
    float tail_factor;

    bool random_rate_enable = true;
    int rate_max = 80;
    int rate_min = 30;
    int rate = 100; // step drop rate

    Synapse(
            //Adafruit_NeoPixel &strip,
            OctoWS2811 &strip,
            int first,
            int last,
//            int red,
//            int green,
//            int blue,
            RGB color,
            float fade_factor,
            int rate);

    running_status run_status;

    void chaseLoop();

    int chaseStep();

    void chase();

    void setRandomColor();

    //void _set_Random_Speed();

    void setRandomTail();

    //void _Set_Random_Wait();

    int adjustChaseDirection(int pixel_number);

    void setPixel(int pixel_num, int red, int green, int blue);

    void newSynapse();

    void setFrameRate(int _frame_rate);

    void setColor(RGB _color);

    frame_status checkDroppedFrame();
};

#endif
