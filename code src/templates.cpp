#include "templates.h"
#include "patent_office.h"
#include "init.h"

namespace pdp {

void Component::Load(lua_State *L, int t) {
	lua_getfield(L, t, "Name");
	this->m_name = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, t, "Meshes");
	if (lua_type(L, -1) == LUA_TSTRING) {
		this->addMesh(lua_tostring(L, -1));
	} else if (lua_type(L, -1) == LUA_TTABLE) {
		int tt = lua_gettop(L);
		lua_pushnil(L);
		while (lua_next(L, tt) != 0) {
			this->addMesh(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	} else {
		printf("Error: 'Meshes' must be a single string or a list of strings.");
	}
	lua_pop(L, 1);

	lua_getfield(L, t, "Texture");
	this->setTexture(lua_tostring(L, -1));
	lua_pop(L, 1);

	lua_getfield(L, t, "Tags");
	if (lua_type(L, -1) == LUA_TTABLE) {
		int tt = lua_gettop(L);
		lua_pushnil(L);
		while (lua_next(L, tt) != 0) {
			this->m_tags.push_back(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	} else {
		if (lua_type(L, -1) != LUA_TNIL) {
			printf("Error: 'Tags' must be a list ie: { 'tag1', 'tag2' }");
		}
	}
	lua_pop(L, 1);
}

void MountComponent::Load(lua_State *L, int t) {
	lua_getfield(L, t, "MountPoints");
	if (lua_type(L, -1) == LUA_TTABLE) {
		int tt = lua_gettop(L);
		lua_pushnil(L);
		while (lua_next(L, tt) != 0) {
			if (lua_type(L, -1) == LUA_TTABLE) {
				std::string type;
				float pos[3];
				int ttt = lua_gettop(L);

				lua_getfield(L, ttt, "Type");
				type = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_getfield(L, ttt, "Offset");
				int tttt = lua_gettop(L);
				lua_pushnil(L);
				while (lua_next(L, tttt) != 0) {
					int n = (int)lua_tonumber(L, -2);
					if (n > 0 && n < 4) {
						pos[n - 1] = (float)lua_tonumber(L, -1);
					}
					lua_pop(L, 1);
				}
				lua_pop(L, 1);
				this->addAttachment(pdp::Attachment(type, vector3df(pos[0], pos[1], pos[2])));
			}
			lua_pop(L, 1);
		}
	} else if (lua_type(L, -1) != LUA_TNIL) {
		printf("Error: 'MountPoints' must be a table, ie: MountPoints = { }");
	}
	lua_pop(L, 1);
}

void WeaponTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	getPatentOffice()->addWeapon(this);
}

void EngineTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	getPatentOffice()->addEngine(this);
}

void SensorTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	getPatentOffice()->addSensor(this);
}

void TurretTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	MountComponent::Load(L, t);
	loadChassisTags(L, t);
	getPatentOffice()->addTurret(this);
}

void TurretTemplate::loadChassisTags(lua_State *L, int t) {
	lua_getfield(L, t, "ChassisTags");
	if (lua_type(L, -1) == LUA_TTABLE) {
		int tt = lua_gettop(L);
		lua_pushnil(L);
		while (lua_next(L, tt) != 0) {
			this->m_chassisTags.push_back(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	} else {
		if (lua_type(L, -1) != LUA_TNIL) {
			printf("Error: 'ChassisTags' must be a list ie: { 'tag1', 'tag2' }");
		}
	}
	lua_pop(L, 1);
}

void ChassisTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	MountComponent::Load(L, t);
	getPatentOffice()->addChassis(this);
}

void VehicleDesign::Load(lua_State *L, int t) {
	lua_getfield(L, t, "Name");
	this->m_name = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, t, "Chassis");
	std::string c = lua_tostring(L, -1);
	lua_pop(L, 1);
	this->m_chassis = getPatentOffice()->getChassisTemplate(c);

	lua_getfield(L, t, "Components");

	std::vector<std::string> attachments;

	int tt = lua_gettop(L);
	lua_pushnil(L);
	while (lua_next(L, tt) != 0) {
		attachments.push_back(lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	for (int i=0; i < m_chassis->attachments().size(); ++i) {
		if (i >= (int)attachments.size()) {
			break;
		}
		const Attachment &a = m_chassis->attachments()[i];
		if (a.type() == "Turret") {
			m_components.push_back(getPatentOffice()->getTurretTemplate(attachments[i]));
		} else if (a.type() == "Fixed" || a.type() == "Rotary") {
			m_components.push_back(getPatentOffice()->getWeaponTemplate(attachments[i]));
		} else if (a.type() == "Engine") {
			m_components.push_back(getPatentOffice()->getEngineTemplate(attachments[i]));
		} else if (a.type() == "Sensor") {
			m_components.push_back(getPatentOffice()->getSensorTemplate(attachments[i]));
		}
	}
	lua_pop(L, 1);

	getPatentOffice()->addVehicleDesign(this);
}

}