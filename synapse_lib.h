#ifndef synapse_lib_h
#define synapse_lib_h

//#include "Arduino.h"
//#include <Adafruit_NeoPixel.h>
#include <OctoWS2811.h>

enum running_status {
    STOPPED, DELAYED, RUNNING, LAST_PIXEL
};
enum frame_status{DROP, RUN};

class Synapse {
    //Adafruit_NeoPixel _strip;
    OctoWS2811 &_strip;
    int red;
    int green;
    int blue;
    float fade_factor;

    int strip_start_position; // starting pixel number
    int strip_end_position; // ending pixel number

    int rate; // step drop rate
    int drop_count; // number of steps until one is dropped

    int first_lit_pixel_position; // first lit pixel of the chase
    int last_lit_pixel_position; // last lit pixel of the chase

    int chase_direction; // 0 = chase up 1 = chase down

    int start_delay; // number of steps to wait before starting chase

    //frame rate control
    int step;
    int scaled_next_drop;
    int scaled_drop_one_frame_every;

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
