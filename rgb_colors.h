#ifndef rgb_colors_h
#define rgb_colors_h

#define LEVEL 255

struct RGB {int red; int green; int blue;};

// must be defined as const in order to avoid multiple definition error at link
//const RGB RED {255, 0, 0};
//const RGB GREEN {0, 255, 0};
//const RGB BLUE {0, 0, 255};
//const RGB YELLOW {255, 255, 0};
//const RGB CYAN {0, 255, 255};
//const RGB MAGENTA {255, 0, 255};
//const RGB WHITE {255, 255, 255};
const RGB RED {LEVEL, 0, 0};
const RGB GREEN {0, LEVEL, 0};
const RGB BLUE {0, 0, LEVEL};
const RGB YELLOW {LEVEL, LEVEL, 0};
const RGB CYAN {0, LEVEL, LEVEL};
const RGB MAGENTA {LEVEL, 0, LEVEL};
const RGB WHITE {LEVEL, LEVEL, LEVEL};

const RGB OFF {0, 0, 0};

const RGB palette[7]{RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};
//crossfade palette - transiton from one color to another without intermediary
const RGB crossfade_palette[7]{RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA};

const RGB WHITE_2 {128, 128, 128};
const RGB WHITE_4 {64, 64, 64};
const RGB WHITE_8 {32, 32, 32};

//const RGB white_lighthouse[8]{OFF, WHITE_4, WHITE_2, WHITE, WHITE_2, WHITE_4, OFF, OFF};
const RGB white_lighthouse[8]{OFF, OFF, WHITE_8, WHITE, WHITE_8, OFF, OFF, OFF};

//#define OFF  0x000000

const int brightness[101]{
        1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 3, 3, 3,
        3, 3, 3, 4, 4, 4, 4, 4, 5, 5,
        5, 6, 6, 6, 7, 7, 7, 8, 8, 9,
        9, 10, 10, 11, 11, 12, 13, 14, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 24, 25, 26,
        28, 29, 31, 33, 35, 37, 39, 41, 43, 46,
        49, 51, 54, 57, 61, 64, 68, 72, 76, 80,
        84, 89, 94, 100, 105, 111, 118, 124, 132, 139,
        147, 155, 164, 174, 184, 194, 205, 217, 229, 242,
        255};

#endif
