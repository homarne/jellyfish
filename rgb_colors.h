#ifndef rgb_colors_h
#define rgb_colors_h

#define LEVEL 32

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

//#define OFF  0x000000

#endif
