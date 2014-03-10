#ifndef _PDP_PATENT_OFFICE
#define _PDP_PATENT_OFFICE

#include <list>
#include <map>
#include <irrlicht.h>

#include "lua.hpp"
#include "templates.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

namespace pdp {

class PatentOffice {
private:
	std::map<std::string, ChassisTemplate*> m_chassisMap;
	std::map<std::string, TurretTemplate*> m_turretMap;
	std::map<std::string, WeaponTemplate*> m_weaponMap;
	std::map<std::string, EngineTemplate*> m_engineMap;
	std::map<std::string, SensorTemplate*> m_sensorMap;
	std::map<std::string, VehicleDesign*> m_vehicleDesigns;

public:
	PatentOffice() {}

	void addWeapon(WeaponTemplate *t) { m_weaponMap[t->name()] = t; }
	void addTurret(TurretTemplate *t) { m_turretMap[t->name()] = t; }
	void addEngine(EngineTemplate *t) { m_engineMap[t->name()] = t; }
	void addSensor(SensorTemplate *t) { m_sensorMap[t->name()] = t; }
	void addChassis(ChassisTemplate *t) { m_chassisMap[t->name()] = t; }
	
	void addVehicleDesign(VehicleDesign *d) { m_vehicleDesigns[d->name()] = d; }

	WeaponTemplate* getWeaponTemplate(const std::string &key) {
		std::map<std::string, WeaponTemplate*>::iterator it = m_weaponMap.find(key);
		if (it != m_weaponMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	TurretTemplate* getTurretTemplate(const std::string &key) {
		std::map<std::string, TurretTemplate*>::iterator it = m_turretMap.find(key);
		if (it != m_turretMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	EngineTemplate*  getEngineTemplate(const std::string &key) {
		std::map<std::string, EngineTemplate*>::iterator it = m_engineMap.find(key);
		if (it != m_engineMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	SensorTemplate* getSensorTemplate(const std::string &key) {
		std::map<std::string, SensorTemplate*>::iterator it = m_sensorMap.find(key);
		if (it != m_sensorMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	ChassisTemplate* getChassisTemplate(const std::string &key) {
		std::map<std::string, ChassisTemplate*>::iterator it = m_chassisMap.find(key);
		if (it != m_chassisMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	VehicleDesign* getVehicleDesign(const std::string &key) {
		std::map<std::string, VehicleDesign*>::iterator it = m_vehicleDesigns.find(key);
		if (it != m_vehicleDesigns.end()) {
			return it->second;
		}
		return nullptr;
	}
};

PatentOffice* getPatentOffice();

}

#endif
