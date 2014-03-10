
-- Procedural Death Planet Vehicle init

RegisterChassis({
	Name = "PanzerBody",
	Meshes = { "panzerwagen_body.b3d" },
	Texture = "texture_terran_vehicle.png",
	Attachments = { { type = "Turret", offset = { 0.0, 1.6, 1.2 } } },
	Tags = { "panzer" }
});


RegisterTurret({
	Name = "PanzerTurret",
	Meshes = { "panzerwagen_turret.b3d" },
	Texture = "texture_terran_vehicle.png",
	-- origin is attachment position
	ChassisTags = { "panzer" } -- chassis must have each of these (use "*" for 'any')
});

RegisterDesign({
	Name = "Panzer",
	Chassis = "PanzerBody",
	Attachments = { "PanzerTurret" }
});

--SetPlayerVehicle("Panzer");
