/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Colors.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

enum
{
	COLOR_ID_CLEAR,
	COLOR_ID_WHITE,
	COLOR_ID_BLACK,
	COLOR_ID_RED,
	COLOR_ID_GREEN,
	COLOR_ID_BLUE,
	COLOR_ID_YELLOW,
	COLOR_ID_CYAN,
	COLOR_ID_MAGENTA,
    COLOR_ID_GRAY,
    NUM_COLOR_IDS
};

static inline SDL_Color GetColorFromColorId(u8 colorId)
{
    // Wrap color ids.
    colorId = colorId % NUM_COLOR_IDS;
    switch(colorId)
    {
        case COLOR_ID_CLEAR: return (SDL_Color) { 0, 0, 0, 0 };
        case COLOR_ID_WHITE: return (SDL_Color) { 255, 255, 255, 255 };
        case COLOR_ID_BLACK: return (SDL_Color) { 0, 0, 0, 255 };
        case COLOR_ID_RED: return (SDL_Color) { 255, 0, 0, 255 };
        case COLOR_ID_GREEN: return (SDL_Color) { 0, 255, 0, 255 };
        case COLOR_ID_BLUE: return (SDL_Color) { 0, 0, 255, 255 };
        case COLOR_ID_YELLOW: return (SDL_Color) { 255, 255, 0, 255 };
        case COLOR_ID_CYAN: return (SDL_Color) { 0, 255, 255, 255 };
        case COLOR_ID_MAGENTA: return (SDL_Color) { 255, 0, 255, 255 };
        case COLOR_ID_GRAY: return (SDL_Color) { 128, 128, 128, 255 };
    }
    return (SDL_Color) { 255, 255, 255, 255 };
}
