#include "init.h"

namespace pdp {


int addChassis(lua_State *L) {
	int t = lua_gettop(L); // table index
	pdp::ChassisTemplate *chassis = new pdp::ChassisTemplate();
	chassis->Load(L, t);
	return 0;
}

int addTurret(lua_State *L) {
	int t = lua_gettop(L); // table index
	pdp::TurretTemplate *turret = new pdp::TurretTemplate();
	turret->Load(L, t);
	return 0;
}

int addWeapon(lua_State *L) {
	int t = lua_gettop(L); // table index
	pdp::WeaponTemplate *weapon= new pdp::WeaponTemplate();
	weapon->Load(L, t);
	return 0;
}

int addDesign(lua_State *L) {
	return 0;
}

int testlua(lua_State *L) {
	printf("testlua()\n");
	return 0;
}

void initTemplates() {
	lua_State *L = luaL_newstate();
	lua_register(L, "RegisterChassis", addChassis);
	lua_register(L, "RegisterTurret", addTurret);
	lua_register(L, "RegisterWeapon", addWeapon);
	lua_register(L, "RegisterDesign", addDesign);

	printf("doing some lua...\n");
	int errorCode  = luaL_loadfile(L, "init.lua");
	if (errorCode) {
		const  char *msg = luaL_checkstring(L, -1);
		printf("luaL_loadfile() failed, error code: %d\nError Msg: %s\n\n", errorCode, msg);
	}
	errorCode = lua_pcall(L, 0, 0, 0);
	if (errorCode) {
		const  char *msg = luaL_checkstring(L, -1);
		printf("lua_pcall() failed, error code: %d\nError Msg: %s\n\n", errorCode, msg);
	}
	lua_close(L);
}

}
