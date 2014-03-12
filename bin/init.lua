
-- Procedural Death Planet Vehicle init

Chassis({
	Name = "PanzerBody",
	Meshes = { "panzerwagen_body.b3d" },
	Texture = "texture_terran_vehicle.png",
	MountPoints = {
		{ Type = "Turret", Offset = { 0.0, 1.6, 1.2 } }
	},
	Tags = { "panzer" }
});

Turret({
	Name = "PanzerTurret",
	Meshes = { "panzerwagen_turret.b3d" },
	Texture = "texture_terran_vehicle.png",
	-- origin is attachment position
	ChassisTags = { "panzer" } -- chassis must have each of these (use "*" for 'any')
});

Design({
	Name = "Panzer",
	Chassis = "PanzerBody",
	Components = { "PanzerTurret" }
});

Chassis({
	Name = "SkimmerChassis",
	Meshes = { "skimmer_body.b3d" },
	Texture = "skimmer_diffuse.png",
	MountPoints = {
		{ Type = "Engine", Offset = { 0.0, 1.8, 1 } },
		{ Type = "Turret", Offset = { 0.0, 1.2, -0.7 } }
	},
	Tags = { "Skimmer", "Player" }
});

Turret({
	Name = "SkimmerCannons",
	Meshes = { "skimmer_cannons.b3d" },
	Texture = "skimmer_diffuse.png",
	ChassisTags = { "Skimmer" }
});

Engine({
	Name = "SkimmerBoosters",
	Meshes = { "skimmer_boosters.b3d" },
	Texture = "skimmer_diffuse.png",
	ChassisTags = { "Skimmer" }
});

Design({
	Name = "Skimmer",
	Chassis = "SkimmerChassis",
	Components = { "SkimmerBoosters", "SkimmerCannons" }
});

SetPlayerVehicle("Panzer");
