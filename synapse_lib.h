#ifndef synapse_lib_h
#define synapse_lib_h

//#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>
#include <OctoWS2811.h>

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
    float fade_factor;

    int strip_start_position; // starting pixel number
    int strip_end_position; // ending pixel number

    //int drop_count; // number of steps until one is dropped

    int first_lit_pixel_position; // first lit pixel of the chase
    int last_lit_pixel_position; // last lit pixel of the chase

    // frame rate control
    int step;
    int scaled_next_drop;
    int scaled_drop_one_frame_every;

    // new synapse control
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

    bool random_rate_enable = true;
    int rate_max = 80;
    int rate_min = 30;
    int rate = 100; // step drop rate

public:
    synapse_settings GetSynapse();

    void SetSynapse(synapse_settings settings);

public:
    Synapse(
            //Adafruit_NeoPixel &strip,
            OctoWS2811 &strip,
            int first,
            int last,
            int red,
            int green,
            int blue,
            float fade_factor,
            int rate);

    running_status run_status;

    void ChaseLoop();

    int ChaseStep();

    void Chase();

    void Set_Random_Color();

    //void _set_Random_Speed();

    void Set_Random_Tail();

    //void _Set_Random_Wait();

    int AdjustChaseDirection(int pixel_number);

    void SetPixel(int pixel_num, int red, int green, int blue);

    void NewSynapse();

    void SetFrameRate(int _frame_rate);

    frame_status CheckDroppedFrame();
};

#endif
