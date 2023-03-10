
#include <X11/Xft/Xft.h>
#include "st.h"

/* Rounded non-negative integers division of n / d  */
#define DIV_ROUND(n, d) (((n) + (d) / 2) / (d))

enum draw_elem_type {
    END_ELEMENTS = 0,
    DRAW_LINE = 1,
    DRAW_RECTANGLE = 2,
    DRAW_ARC = 3,
    DRAW_CHAR = 4
};

enum draw_elem_color {
    DRAW_FOREGROUND = 0,
    DRAW_BACKGROUND = 1
};

enum anchor_type {
    ANCHOR_NONE = 0,
    ANCHOR_BOTTOM = 1,
    ANCHOR_TOP = 2,
    ANCHOR_LEFT = 3,
    ANCHOR_RIGHT = 4,
    ANCHOR_TOP_LEFT = 5,
    ANCHOR_TOP_RIGHT = 6,
    ANCHOR_BOTTOM_LEFT = 7,
    ANCHOR_BOTTOM_RIGHT = 8
};

typedef struct {
    enum draw_elem_type type;
    enum draw_elem_color color;
    float parm1;
    float parm2;
    float parm3;
    float parm4;
    float parm5;
    float parm6;
} DrawElement;


static const DrawElement leftArrow[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.5, 0.9, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.5, 0.35, 0.35 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.5, 0.35, 0.65 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement rightArrow[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.5, 0.9, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.9, 0.5, 0.65, 0.35 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.9, 0.5, 0.65, 0.65 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement leftDiag[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 1, 1, 0 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement rightDiag[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0, 1, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement blank[] = {
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement midHorizLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 1, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement midVertLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0, 0.5, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement upTee[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 1, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 0 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement downTee[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 1, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement rightTee[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0, 0.5, 1 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement leftTee[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0, 0.5, 1 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topRightLineCorner[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0, 0.5, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomRightLineCorner[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomLeftLineCorner[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 0.5, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topLeftLineCorner[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 0.5, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 0 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement connectBottomLeftTopCenter[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 1, 0.5, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 0 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement connectBottomCenterTopLeft[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 1, 0.5, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0, 0 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement connectBottomCenterTopRight[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 1, 0.5, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0 },
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 0.5, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement connectBottomRightTopCenter[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 1, 1, 0.5, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 0 },
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 0.5, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement filledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 0.5, ANCHOR_TOP },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.5, 1, 0.5, ANCHOR_BOTTOM },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement leftFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 0.5, 1, ANCHOR_LEFT },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement rightFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.5, 0, 0.5, 1, ANCHOR_RIGHT },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.5, 1, 0.5, ANCHOR_BOTTOM },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 0 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 0.5, ANCHOR_TOP },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement leftConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 0.5, 1, ANCHOR_LEFT },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement rightConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.5, 0, 0.5, 1, ANCHOR_RIGHT },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0, 0.5 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topRightFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.5, 0, 0.5, 0.5, ANCHOR_TOP_RIGHT },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomRightFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 0.5, ANCHOR_BOTTOM_RIGHT },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomLeftFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.5, 0.5, 0.5, ANCHOR_BOTTOM_LEFT },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topLeftFilledBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 0.5, 0.5, ANCHOR_TOP_LEFT },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topRightConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.5, 0, 0.5, 0.5, ANCHOR_TOP_RIGHT },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomRightConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.5, 0.5, 0.5, 0.5, ANCHOR_BOTTOM_RIGHT },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 0, 0 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomLeftConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.5, 0.5, 0.5, ANCHOR_BOTTOM_LEFT },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 0 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topLeftConnectorBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 0.5, 0.5, ANCHOR_TOP_LEFT },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.5, 1, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topRightDot[] = {
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomRightDot[] = {
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomLeftDot[] = {
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topLeftDot[] = {
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement topLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0, 1, 0 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement bottomLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 1, 1, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement leftLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0, 0, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement rightLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 1, 0, 1, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar1[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar2[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 0.125, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar3[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 0.25, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar4[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 0.375, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar5[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 0.5, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar6[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 0.625, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar7[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 0.75, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar8[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.775, 1, 0.1 },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 0.875, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement statsBar9[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.125, 1, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement rightStatsLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 1, 0.125, 1, 0.875 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement leftStatsLine[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.125, 0, 0.875 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement xDiags[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0, 1, 1 },
        { DRAW_LINE, DRAW_FOREGROUND, 1, 0, 0, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement cross[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0, 0.5, 1, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0, 0.5, 1 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement upArrow[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.5, 0.5, 0.15 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.15, 0.9, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.15, 0.5, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement downArrow[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.5, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.85, 0.9, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.15, 0.5, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement upDownArrow[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.4, 0.5, 0.15 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.15, 0.9, 0.4 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.1, 0.55, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.8, 0.9, 0.55 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.15, 0.5, 0.8 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement hollowBox[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 0.08, ANCHOR_TOP },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0.92, 1, 0.08, ANCHOR_BOTTOM },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 0.1, 1, ANCHOR_LEFT },
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.9, 0, 0.1, 1, ANCHOR_RIGHT },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

//static const DrawElement questionMark[] = {
//        { DRAW_LINE, DRAW_FOREGROUND, 0.2, 0.375, 0.5, 0.25 },
//        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.8, 0.375 },
//        { DRAW_LINE, DRAW_FOREGROUND, 0.8, 0.375, 0.82, 0.5 },
//        { DRAW_LINE, DRAW_FOREGROUND, 0.82, 0.5, 0.5, 0.6 },
//        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.6, 0.5, 0.7 },
//        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0.45, 0.75, 0.1, .1 },
//        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
//};

static const DrawElement questionMark[] = {
        { DRAW_CHAR, DRAW_FOREGROUND, '?' },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeA[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.45, 0.4 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.45, 0.4, 0.85, 0.25 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.45, 0.45, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeB[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.6, 0.4 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.6, 0.4, 0.15, 0.55 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.55, 0.6, 0.7 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.6, 0.7, 0.15, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeC[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.45, 0.25, 0.45, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.55, 0.45, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.45, 0.25, 0.85, 0.55 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeD[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.25, 0.85, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.85, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.25, 0.15, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeE[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.25, 0.85, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.5, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.25, 0.5, 0.45 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeF[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.45, 0.7, 0.25 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.65, 0.7, 0.45 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeG[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.85, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.25, 0.15, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeH[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.25, 0.25, 0.25, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.75, 0.25, 0.75, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.25, 0.35, 0.75, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.25, 0.55, 0.75, 0.7 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeI[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeJ[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.55, 0.5, 0.35 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.55, 0.5, 0.75 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.55, 0.5, 0.35 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.55, 0.5, 0.75 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeK[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.55, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.55, 0.85, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeL[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.85, 0.45 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeM[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.25, 0.85, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.85, 0.65 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.25, 0.15, 0.65 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeN[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.4, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.4, 0.45, 0.6, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.6, 0.45, 0.85, 0.65 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeO[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.45, 0.4 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.45, 0.4, 0.85, 0.25 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.45, 0.45, 0.65 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.45, 0.65, 0.85, 0.45 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeP[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.85, 0.5, 0.6 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.6, 0.85, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeQ[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.6, 0.4, 0.6 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.4, 0.6, 0.65, 0.7 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.65, 0.7, 0.65, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeR[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.6, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.6, 0.45, 0.15, 0.65 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.65, 0.6, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeS[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.65 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.65, 0.8, 0.35 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.8, 0.35, 0.8, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeT[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.85, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.25, 0.45 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeU[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.8, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.8, 0.5, 0.8, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeV[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.6875, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.6875, 0.45, 0.85, 0.25 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.3375, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.3375, 0.45, 0.15, 0.25 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeW[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.6, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.6, 0.45, 0.15, 0.65 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeX[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.5, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.5, 0.45 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.45, 0.85, 0.25 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeY[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.15, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.25, 0.8, 0.5 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.8, 0.5, 0.8, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.6, 0.45, 0.6 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.45, 0.6, 0.45, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement runeZ[] = {
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.15, 0.4 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.15, 0.4, 0.85, 0.85 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.5, 0.25, 0.85, 0.4 },
        { DRAW_LINE, DRAW_FOREGROUND, 0.85, 0.4, 0.15, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement reverseUpArrow[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 1 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.1, 0.5, 0.5, 0.15 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.5, 0.15, 0.9, 0.5 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.5, 0.15, 0.5, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement reverseDownArrow[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 1 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.1, 0.5, 0.5, 0.85 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.5, 0.85, 0.9, 0.5 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.5, 0.15, 0.5, 0.85 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement reverseUpDownArrow[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 1 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.1, 0.4, 0.5, 0.15 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.5, 0.15, 0.9, 0.4 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.1, 0.55, 0.5, 0.85 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.5, 0.8, 0.9, 0.55 },
        { DRAW_LINE, DRAW_BACKGROUND, 0.5, 0.15, 0.5, 0.8 },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement reverseQuestionMark[] = {
        { DRAW_RECTANGLE, DRAW_FOREGROUND, 0, 0, 1, 1 },
        { DRAW_CHAR, DRAW_BACKGROUND, '?' },
        { END_ELEMENTS, DRAW_BACKGROUND, 0, 0, 0, 0 }
};

static const DrawElement *zorkFont[256] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        leftArrow,
        rightArrow,
        leftDiag,
        rightDiag,
        blank,
        midHorizLine,
        midHorizLine,
        midVertLine,
        midVertLine,
        upTee,
        downTee,
        rightTee,
        leftTee,
        topRightLineCorner,
        bottomRightLineCorner,
        bottomLeftLineCorner,
        topLeftLineCorner,
        connectBottomLeftTopCenter,
        connectBottomCenterTopLeft,
        connectBottomCenterTopRight,
        connectBottomRightTopCenter,
        filledBox,
        topFilledBox,
        bottomFilledBox,
        leftFilledBox,
        rightFilledBox,
        bottomConnectorBox,
        topConnectorBox,
        leftConnectorBox,
        rightConnectorBox,
        topRightFilledBox,
        bottomRightFilledBox,
        bottomLeftFilledBox,
        topLeftFilledBox,
        topRightConnectorBox,
        bottomRightConnectorBox,
        bottomLeftConnectorBox,
        topLeftConnectorBox,
        topRightDot,
        bottomRightDot,
        bottomLeftDot,
        topLeftDot,
        topLine,
        bottomLine,
        leftLine,
        rightLine,
        statsBar1,
        statsBar2,
        statsBar3,
        statsBar4,
        statsBar5,
        statsBar6,
        statsBar7,
        statsBar8,
        statsBar9,
        rightStatsLine,
        leftStatsLine,
        xDiags,
        cross,
        upArrow,
        downArrow,
        upDownArrow,
        hollowBox,
        questionMark,
        runeA,
        runeB,
        runeC,
        runeD,
        runeE,
        runeF,
        runeG,
        runeH,
        runeI,
        runeJ,
        runeK,
        runeL,
        runeM,
        runeN,
        runeO,
        runeP,
        runeQ,
        runeR,
        runeS,
        runeT,
        runeU,
        runeV,
        runeW,
        runeX,
        runeY,
        runeZ,
        reverseUpArrow,
        reverseDownArrow,
        reverseUpDownArrow,
        reverseQuestionMark,
};

static Display *xdpy;
static Colormap xcmap;
static XftDraw *xd;
static Visual *xvis;

static void drawzork(int, int, int, int, XftColor *, XftColor *, const XftGlyphFontSpec *);

/* public API */

void
zorkdraw_xinit(Display *dpy, Colormap cmap, XftDraw *draw, Visual *vis) {
    xdpy = dpy;
    xcmap = cmap;
    xd = draw, xvis = vis;
}

int
iszorkdraw(Rune u) {
    Rune block = u & ~0xff;
    return (zorkdraw && block == 0xe000);
}

void
drawzorks(int x, int y, int cw, int ch, XftColor *fg, XftColor *bg,
          const XftGlyphFontSpec *specs, int len) {
    for (; len-- > 0; x += cw, specs++)
        drawzork(x, y, cw, ch, fg, bg, specs);
}

/* implementation */

void
drawzork(int x, int y, int w, int h, XftColor *fg, XftColor *bg, const XftGlyphFontSpec *spec) {
    Display *display;
    Drawable drawable;
    GC gc;
    XGCValues values;

    int rectX;
    int rectY;
    int rightX;
    int bottomY;
    unsigned int rectWidth;
    unsigned int rectHeight;

    ushort ch = spec->glyph;

    /* Display, Drawable, Colormap, and Visual of an XftDraw can be queried with
     * XftDrawDisplay(), XftDrawDrawable(), XftDrawColormap(), and XftDrawVisual(),
     * respectively. The X Rendering Extension Picture associated with an XftDraw is
     * returned by XftDrawPicture().  */
    display = XftDrawDisplay(xd);
    drawable = XftDrawDrawable(xd);

    /* create the Graphics Context */

    /* s: stem thickness. width/8 roughly matches underscore thickness. */
    int mwh = MIN(w, h);
    int base_s = MAX(1, DIV_ROUND(mwh, 8));

    values.foreground = fg->pixel;
    values.background = bg->pixel;
    values.line_width = base_s;
    gc = XCreateGC(display, drawable, GCForeground | GCBackground | GCLineWidth, &values);

    const DrawElement *charInstr = zorkFont[ch & 0xff];

    if(charInstr != NULL) {
        enum draw_elem_color curColor = DRAW_FOREGROUND;

        for(int i = 0; charInstr[i].type != END_ELEMENTS; i++) {
            if(charInstr[i].color != curColor) {
                curColor = charInstr[i].color;
                if(curColor == DRAW_FOREGROUND) {
                    values.foreground = fg->pixel;
                    values.background = bg->pixel;
                } else {
                    values.foreground = bg->pixel;
                    values.background = fg->pixel;
                }
                gc = XCreateGC(display, drawable, GCForeground | GCBackground | GCLineWidth, &values);
            }

            switch(charInstr[i].type) {
                case DRAW_LINE:
                    XDrawLine(display, drawable, gc,
                              x + w * charInstr[i].parm1,
                              y + h * charInstr[i].parm2,
                              x + w * charInstr[i].parm3,
                              y + h * charInstr[i].parm4);
                    break;

                case DRAW_RECTANGLE:
                    rectX = x + w * charInstr[i].parm1;
                    rectY = y + h * charInstr[i].parm2;
                    rectWidth = w * charInstr[i].parm3;
                    rectHeight = h * charInstr[i].parm4;
                    rightX = x + w;
                    bottomY = y + h;

                    if(charInstr[i].parm5 == ANCHOR_RIGHT || charInstr[i].parm5 == ANCHOR_TOP_RIGHT) {
                        rectWidth = rightX - rectX;
                    } else if(charInstr[i].parm5 == ANCHOR_BOTTOM || charInstr[i].parm5 == ANCHOR_BOTTOM_LEFT) {
                        rectHeight = bottomY - rectY;
                    } else if(charInstr[i].parm5 == ANCHOR_BOTTOM_RIGHT) {
                        rectWidth = rightX - rectX;
                        rectHeight = bottomY - rectY;
                    }

                    XFillRectangle(display, drawable, gc,
                                   rectX,
                                   rectY,
                                   rectWidth,
                                   rectHeight);
                    break;

                case DRAW_ARC:
                    XDrawArc(display, drawable, gc,
                             x + w * charInstr[i].parm1,
                             y + h * charInstr[i].parm2,
                             w * charInstr[i].parm3,
                             h * charInstr[i].parm4,
                             charInstr[i].parm5 * 64,
                             charInstr[i].parm6 * 64);
                    break;

                case DRAW_CHAR:
                    char val = charInstr[i].parm1;
                    if(charInstr[i].color == DRAW_FOREGROUND) {
                        XftDrawString8(xd, fg, spec->font,
                                       spec->x, spec->y, &val, 1);
                    } else {
                        XftDrawString8(xd, bg, spec->font,
                                       spec->x, spec->y, &val, 1);
                    }
                    break;

                default:
                    break;
            }
        }
    }
}
