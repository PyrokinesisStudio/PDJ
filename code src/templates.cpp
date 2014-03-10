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
		printf("Error: 'Tags' must be a list ie: { 'tag1', 'tag2' }");
	}
	lua_pop(L, 1);
}

void MountComponent::Load(lua_State *L, int t) {
	lua_getfield(L, t, "Attachments");
	if (lua_type(L, -1) == LUA_TTABLE) {
		int tt = lua_gettop(L);
		lua_pushnil(L);
		while (lua_next(L, tt) != 0) {
			if (lua_type(L, -1) == LUA_TTABLE) {
				std::string type;
				float pos[3];
				int ttt = lua_gettop(L);

				lua_getfield(L, ttt, "type");
				type = lua_tostring(L, -1);
				lua_pop(L, 1);

				lua_getfield(L, ttt, "offset");
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
	} else {
		printf("Error: 'Attachments' must be a table, ie: Attachments = { }");
	}
	lua_pop(L, 1);
}

void WeaponTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	patentOffice->addWeapon(this);
}

void TurretTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	MountComponent::Load(L, t);
	loadChassisTags(L, t);
	patentOffice->addTurret(this);
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
		printf("Error: 'ChassisTags' must be a list ie: { 'tag1', 'tag2' }");
	}
	lua_pop(L, 1);
}

void ChassisTemplate::Load(lua_State *L, int t) {
	Component::Load(L, t);
	MountComponent::Load(L, t);
	patentOffice->addChassis(this);
}

void VehicleDesign::Load(lua_State *L, int t) {
	lua_getfield(L, t, "Name");
	this->m_name = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, t, "Chassis");
	std::string c = lua_tostring(L, -1);
	lua_pop(L, 1);
	this->m_chassis = patentOffice->getChassisTemplate(c);

	lua_getfield(L, t, "Attachments");

	std::vector<std::string> attachments;

	int tt = lua_gettop(L);
	lua_pushnil(L);
	while (lua_next(L, tt) != 0) {
		attachments.push_back(lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	for (int i=0; i < m_chassis->numAttachments(); ++i) {
		if (i >= (int)attachments.size()) {
			break;
		}
		Attachment a = m_chassis->getAttachment(i);
		if (a.m_type.compare("Turret") == 0) {
			m_turrets.push_back(patentOffice->getTurretTemplate(attachments[i]));
		} else {
			m_weapons.push_back(patentOffice->getWeaponTemplate(attachments[i]));
		}
	}
	lua_pop(L, 1);

	patentOffice->addVehicleDesign(this);
}

}