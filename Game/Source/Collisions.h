#ifndef __Collisions_H__
#define __Collisions_H__

#include "Module.h"
#include "External/SDL/include/SDL.h"

enum class object_type;
struct Object;
struct Properties;


struct Collider
{
	SDL_Rect	rect;					//Collider box
	bool		to_delete = false;			//deletes collider if true
	object_type type;				//enum class collider type defined in map.h
	Module*	callback = nullptr;	
	Properties* userdata;

	Collider(SDL_Rect rectangle, object_type type, Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	Collider(Object object);

	Collider() {};

	inline void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	inline bool CheckCollision(const SDL_Rect& r) const;
};



class Collisions : public Module {
	
public:
	Collisions();

	bool Init();

	virtual ~Collisions() {};

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, object_type type, Module* callback, Properties* userdata = nullptr);
	void LoadFromMap(); //Loads colliders from tiled map
	

private:

	void DebugDraw(); //Draws the collider pressing F9
	
public:

	p2List<Collider*> colliders;
	bool debug_colliders;

private:

};


#endif