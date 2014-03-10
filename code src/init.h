#ifndef _PDP_INIT
#define _PDP_INIT

#include <string>
#include <vector>
#include <list>
#include <irrlicht.h>

#include "lua.hpp"
#include "templates.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

extern IrrlichtDevice *irrlicht;

namespace pdp {
	
void initTemplates();

std::string getPlayerVehicle();

}

#endif
