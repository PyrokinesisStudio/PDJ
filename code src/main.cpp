#define _CRT_SECURE_NO_WARNINGS 1

#include <list>
#include <irrlicht.h>

#include "lua.hpp"

#include "templates.h"
#include "init.h"

using namespace irr;

using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;

#ifdef _IRR_WINDOWS_
//#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
//#pragma comment(lib, "lua52.lib")
#endif

IrrlichtDevice *irrlicht = nullptr;


ISceneNode *player = nullptr;

class EventHandler : public IEventReceiver {
public:
	virtual bool OnEvent(const SEvent& event) {
		if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
			m_keyflags[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}
		return false;
	}

	virtual bool IsKeyDown(EKEY_CODE keyCode) const {
		return m_keyflags[keyCode];
	}
	
	EventHandler() {
		memset(m_keyflags, 0, sizeof(bool) * KEY_KEY_CODES_COUNT);
	}

private:
	bool m_keyflags[KEY_KEY_CODES_COUNT];
};

int main() {
	EventHandler eventHandler;
	irrlicht = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 32, false, false, false, &eventHandler);

	if (!irrlicht) {
		return 1;
	}
	pdp::initTemplates();

	irrlicht->setWindowCaption(L"Procedural Death Planet");

	IVideoDriver* driver = irrlicht->getVideoDriver();
	ISceneManager* smgr = irrlicht->getSceneManager();
	IGUIEnvironment* guienv = irrlicht->getGUIEnvironment();


	IAnimatedMesh* body_mesh = smgr->getMesh("panzerwagen_body.b3d");
	if (!body_mesh) {
		irrlicht->drop();
		return 1;
	}
	IMesh *turret_mesh = smgr->getMesh("panzerwagen_turret.b3d");
	if (!turret_mesh) {
		irrlicht->drop();
		return 1;
	}
	
	ISceneNode *container_node = smgr->addSceneNode("empty");

	ITexture *tex = driver->getTexture("texture_terran_vehicle.png");

	IAnimatedMeshSceneNode* body_node = smgr->addAnimatedMeshSceneNode( body_mesh, container_node );
	if (body_node) {
		body_node->setMaterialFlag(EMF_LIGHTING, false);
		body_node->setAnimationSpeed(20.0f);
		body_node->setMaterialTexture( 0, tex );

	}
	IMeshSceneNode *turr_node = smgr->addMeshSceneNode( turret_mesh, container_node );
	if (turr_node) {
		turr_node->setMaterialFlag( EMF_LIGHTING, false );
		turr_node->setMaterialTexture( 0, tex );
		turr_node->setPosition(vector3df(0.0f, 1.6f, 1.2f));
		//turr_node->setPosition();
	}

	//container_node->setRotation( vector3df( 0.0f, 90.0f, 0.0f ) );


	ICameraSceneNode *camera = smgr->addCameraSceneNode( 0, vector3df( 0, 8, -20 ), vector3df( 0, 1, 0 ) );

	camera->setFOV( 0.34906585f );

	u32 prevT = irrlicht->getTimer()->getTime();

	float speed = 10.f;
	float turn_speed = 100.f;

	while (irrlicht->run()) {
		const u32 t = irrlicht->getTimer()->getTime();
		float deltaT = (t - prevT) / 1000.f;
		prevT = t;

		float theta = 0.f;
		bool ctrl = eventHandler.IsKeyDown(KEY_LSHIFT);
		if (eventHandler.IsKeyDown(KEY_LEFT)) {
			theta -= deltaT * turn_speed * clamp(speed / 30.f, 0.f, 1.f);
			vector3df rot = (ctrl ? turr_node : container_node)->getRotation();
			rot.Y += theta;
			if (rot.Y < 0.0f) {
				rot.Y += 360.0f;
			}
			(ctrl ? turr_node : container_node)->setRotation( rot );
		} else if (eventHandler.IsKeyDown(KEY_RIGHT)) {
			theta += deltaT * turn_speed * clamp(speed / 30.f, 0.f, 1.f);
			vector3df rot = (ctrl ? turr_node : container_node)->getRotation();
			rot.Y += theta;
			if (rot.Y >= 360.0) {
				rot.Y -= 360.0f;
			}
			(ctrl ? turr_node : container_node)->setRotation( rot );
		}

		driver->beginScene( true, true, SColor(255,100,101,140) );

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
	irrlicht->drop();
	return 0;
}

