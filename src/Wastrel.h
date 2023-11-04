/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Wastrel.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "utils/NiceTypes.h"
#include "utils/Misc.h"
#include "utils/Colors.h"
#include "utils/MinMaxClamp.h"
#include "utils/Log.h"
#include "utils/RNG.h"

#include "core/Image.h"
#include "core/Frame.h"
#include "core/FrameSet.h"
#include "core/Tile.h"
#include "core/TileSet.h"
#include "core/DrawQueue.h"
#include "core/DFont.h"
#include "core/App.h"

#include "game/Game.h"