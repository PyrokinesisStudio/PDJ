#ifndef _PDP_TEMPLATES
#define _PDP_TEMPLATES

#define _CRT_SECURE_NO_WARNINGS 1

#include <string>
#include <list>
#include <irrlicht.h>

#include <vector>
#include "lua.hpp"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

extern IrrlichtDevice *irrlicht;

namespace pdp {

struct Attachment {
	//enum Type { Turret, Sponson, FixedMount, RotaryMount, Engine, Sensor };
private:
	std::string m_type;
	vector3df m_offset;

public:
	Attachment(const std::string &t, const vector3df &pos) : m_type(t), m_offset(pos) {}
	Attachment(const Attachment &cpy) : m_type(cpy.m_type), m_offset(cpy.m_offset) {}

	const std::string& type() const { return m_type; }
	vector3df offset() const { return m_offset; }
};

class Component {
protected:
	std::string m_name;
	std::list<std::string> m_tags;
	std::list<IAnimatedMesh*> m_meshes;
	ITexture *m_texture;

public:
	virtual void Load(lua_State *L, int t);

	const std::string& name() const { return m_name; }
	const std::list<std::string>& tags() const { return m_tags; }
	const std::list<IAnimatedMesh*>& meshes() const { return m_meshes; }
	ITexture* texture() const { return m_texture; }

private:
	void addMesh(const char *m) {
		m_meshes.push_back(irrlicht->getSceneManager()->getMesh(m));
	}

	void setTexture(const char *t) {
		m_texture = irrlicht->getVideoDriver()->getTexture(t);
	}
};

class MountComponent {
protected:
	std::vector<Attachment> m_attachments;

public:
	virtual void Load(lua_State *L, int t);

	const std::vector<Attachment>& attachments() const { return m_attachments; }

private:
	void addAttachment(Attachment a) {
		m_attachments.push_back(a);
	}
};

class WeaponTemplate : public Component {
public:
	virtual void Load(lua_State *L, int t) override;
};

class TurretTemplate : public Component, public MountComponent {
protected:
	std::list<std::string> m_chassisTags;

public:
	virtual void Load(lua_State *L, int t) override;

	const std::list<std::string>& chassisTags() const { return m_chassisTags; }

private:
	void loadChassisTags(lua_State *L, int t);
};

class EngineTemplate : public Component {
public:
	virtual void Load(lua_State *L, int t) override;
};

class SensorTemplate : public Component {
public:
	virtual void Load(lua_State *L, int t) override;
};

class ChassisTemplate : public Component, public MountComponent {
public:
	virtual void Load(lua_State *L, int t) override;
};

class VehicleDesign {
private:
	std::string m_name;
	ChassisTemplate *m_chassis;
	std::list<Component*> m_components;
	//std::list<TurretTemplate*> m_turrets;
	//std::list<WeaponTemplate*> m_weapons;

public:
	VehicleDesign() : m_chassis(nullptr) {}

	void Load(lua_State *L, int t);

	const std::string& name() const { return m_name; }
	const ChassisTemplate* chassis() const { return m_chassis; }

	const std::list<Component*>& components() const { return m_components; }
	//const std::list<TurretTemplate*>& turrets() const { return m_turrets; }
	//const std::list<WeaponTemplate*>& weapons() const { return m_weapons; }
};

}

#endif
