#define _CRT_SECURE_NO_WARNINGS 1

#include <list>
#include <irrlicht.h>

#include "lua.hpp"

#include "templates.h"
#include "init.h"
#include "patent_office.h"

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

ISceneNode *playerChassis = nullptr;
ISceneNode *playerTurret = nullptr;


extern pdp::PatentOffice *patentOffice;

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

ISceneNode* spawnVehicle( const pdp::VehicleDesign *design, const vector3df &pos ) {
	ISceneManager* smgr = irrlicht->getSceneManager();
	ISceneNode *container_node = smgr->addSceneNode( "empty" );

	std::list<IAnimatedMesh*>::const_iterator it = design->chassis()->meshes().begin();
	std::list<IAnimatedMesh*>::const_iterator itEnd = design->chassis()->meshes().end();
	for ( ; it != itEnd; ++it) {
		IAnimatedMesh* mesh = *it;

		IAnimatedMeshSceneNode* mesh_node = smgr->addAnimatedMeshSceneNode( mesh, container_node );
		if (mesh_node) {
			mesh_node->setMaterialFlag( EMF_LIGHTING, false );
			mesh_node->setAnimationSpeed( 20.0f );
			mesh_node->setMaterialTexture( 0, design->chassis()->texture() );
			playerChassis = mesh_node;
		}
	}
	std::list<pdp::Component*>::const_iterator tit = design->components().begin();
	for (int i=0; i < design->chassis()->attachments().size(); ++i) {
		pdp::Attachment a = design->chassis()->attachments()[i];
		it = (*tit)->meshes().begin();
		itEnd = (*tit)->meshes().end();
		for (; it != itEnd; ++it) {
			IAnimatedMesh *mesh = *it;
			IMeshSceneNode *turr_node = smgr->addMeshSceneNode( mesh, container_node );
			if (turr_node) {
				turr_node->setMaterialFlag( EMF_LIGHTING, false );
				turr_node->setMaterialTexture( 0, (*tit)->texture() );
				turr_node->setPosition(a.offset());
				if (a.type() == "Turret") {
					playerTurret = turr_node;
				}
			}
		}
		++tit;
	}
	return container_node;
}

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

	// add terrain scene node
    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
        "terrain-heightmap.bmp",
        0,                  // parent node
        -1,                 // node id
        core::vector3df(0.f, 0.f, 0.f),     // position
        core::vector3df(0.f, 0.f, 0.f),     // rotation
        core::vector3df(1.f, 1.0f, 1.f),  // scale
        video::SColor ( 255, 255, 255, 255 ),   // vertexColor
        5,                  // maxLOD
        scene::ETPS_17,             // patchSize
        4                   // smoothFactor
        );

    terrain->setMaterialFlag(video::EMF_LIGHTING, false);

    terrain->setMaterialTexture(0,
            driver->getTexture("terrain-texture.jpg"));
    terrain->setMaterialTexture(1,
            driver->getTexture("detailmap3.jpg"));
    
    terrain->setMaterialType(video::EMT_DETAIL_MAP);

    terrain->scaleTexture(1.0f, 20.0f);


	pdp::VehicleDesign *design = pdp::getPatentOffice()->getVehicleDesign(pdp::getPlayerVehicle());
	
	ICameraSceneNode *camera = smgr->addCameraSceneNode( 0, vector3df( 0, 8, -20 ), vector3df( 0, 1, 0 ) );
	if (design) {
		player = spawnVehicle(design, vector3df(0.0f));
		camera->setParent(player);
	} 
	camera->setFOV( 0.34906585f );

	//player->setPosition(vector3df(0.0f, 100.0f, 0.0f));

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
			vector3df rot = (ctrl ? playerTurret : player)->getRotation();
			rot.Y += theta;
			if (rot.Y < 0.0f) {
				rot.Y += 360.0f;
			}
			(ctrl ? playerTurret : player)->setRotation( rot );
		} else if (eventHandler.IsKeyDown(KEY_RIGHT)) {
			theta += deltaT * turn_speed * clamp(speed / 30.f, 0.f, 1.f);
			vector3df rot = (ctrl ? playerTurret : player)->getRotation();
			rot.Y += theta;
			if (rot.Y >= 360.0) {
				rot.Y -= 360.0f;
			}
			(ctrl ? playerTurret : player)->setRotation( rot );
		}

		driver->beginScene( true, true, SColor(255,100,101,140) );

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}
	irrlicht->drop();
	return 0;
}

