#include "Defs.h"
#include "Log.h"
#include "Application.h"
#include "Projectile.h"
#include "EntityPlayer.h"
#include "EntityEnemy.h"
#include "Collisions.h"
#include "Map.h"
#include "Render.h"
#include "Audio.h"
#include "Textures.h"
#include "GameScene.h"
#include <ctime>


Projectile::Projectile() : Module()
{
	name.Create("projectile");
}


// Destructor
Projectile::~Projectile()
{}

bool Projectile::Start()
{
	name = "beam";
	testBeam = new Beam;
	testBeam->animInfo.flip = false;
	uiBeamTex = app->tex->Load("Assets/Textures/UI/beam.png");

	return true;
}

bool Projectile::PreUpdate()
{
	return true;
}

bool Projectile::Update(float dt)
{
	DoUpdate(dt);
	DrawAnimations();

	if (active)
	{
		if (showCd)
		{
			ShowCd();
		}
		else
		{
			app->render->DrawTexture(uiBeamTex, -app->render->camera.x + 185, -app->render->camera.y + app->render->camera.h - 69);
			testBeam->animInfo.i = 0;
			testBeam->animInfo.frameCount = 0.0f;
		}
	}
	

	return true;
}

bool Projectile::PostUpdate()
{
	return true;
}

bool Projectile::CleanUp()
{
	// Remove pickups
	ListItem<Beam*>* item;
	item = beamList.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	beamList.clear();
	return true;
}

void Projectile::SpawnBeam(bool flip)
{
	Beam* newBeam = new Beam;

	if (flip)
	{
		newBeam->position.x = app->player->player.position.x - 32;
		newBeam->position.y = app->player->player.position.y + 48;
		newBeam->velocity = -10.0f;
	}
	else
	{
		newBeam->position.x = app->player->player.position.x + 64;
		newBeam->position.y = app->player->player.position.y + 48;
		newBeam->velocity = 10.0f;
	}

	newBeam->beamHitbox.x = newBeam->position.x;
	newBeam->beamHitbox.y = newBeam->position.y;
	newBeam->beamHitbox.w = 24;
	newBeam->beamHitbox.h = 24;

	newBeam->beamCollider = app->collisions->AddCollider(newBeam->beamHitbox, ObjectType::PROJECTILE, this);

	newBeam->animInfo.flip = flip;

	beamList.add(newBeam);

}
void Projectile::OnCollision(Collider* A, Collider* B)
{
	if (A->type == ObjectType::PROJECTILE && B->type == ObjectType::GROUND)
	{
		A->toDelete = true;
		app->audio->PlayFx(app->player->hitSound);
	}
	if (A->type == ObjectType::PROJECTILE && B->type == ObjectType::ENEMY)
	{
		A->toDelete = true;
		app->audio->PlayFx(app->player->hitSound);
		B->toDelete = true;
		app->enemy->GetKilled();

		int newScore;
		srand(time(NULL));
		newScore = (rand() % 550) + 450;
		app->gameScene->AddScore(newScore);
	}


}
void Projectile::DrawAnimations()
{
	ListItem<Beam*>* beamIterator = beamList.start;

	while (beamIterator != NULL)
	{
		if (beamIterator->data->beamCollider->toDelete == false)
		{
			app->map->DrawStaticAnimation("beam", "projectile_tileset", { beamIterator->data->position.x, beamIterator->data->position.y }, &beamIterator->data->animInfo);
		}
		beamIterator = beamIterator->next;
	}
}

void Projectile::DoUpdate(float dt)
{
	ListItem<Beam*>* beamIterator = beamList.start;

	while (beamIterator != NULL)
	{
		beamIterator->data->UpdatePos();

		if (beamIterator->data->animInfo.flip)
		{
			beamIterator->data->beamCollider->SetPos(beamIterator->data->position.x + 48, beamIterator->data->position.y - 10);
		}
		else
		{
			beamIterator->data->beamCollider->SetPos(beamIterator->data->position.x - 12, beamIterator->data->position.y - 10);
		}
		beamIterator = beamIterator->next;
	}
}
 
void Projectile::ShowCd()
{
	app->map->DrawStaticAnimation("cooldown", "cooldown", { -app->render->camera.x + 200, -app->render->camera.y + app->render->camera.h - 50 }, &testBeam->animInfo);
}
