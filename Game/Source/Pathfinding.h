#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include "Module.h"
#include "Point.h"
#include "DynArray.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

enum class PathMovement
{
	NO_MOVE = -1,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	UP_RIGHT,
	UP_LEFT,
	DOWN_RIGHT,
	DOWN_LEFT
};

class Pathfinding : public Module
{
public:

	Pathfinding();

	// Destructor
	~Pathfinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	DynArray<iPoint>* CreatePath(const iPoint& origin, const iPoint& destination);

	// To request all tiles involved in the last generated path
	const DynArray<iPoint>* GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

	PathMovement CheckDirection(DynArray<iPoint>& path, PathMovement lastMov) const;
	PathMovement CheckDirectionGround(DynArray<iPoint>& path) const;

private:

	DynArray<iPoint>* path = nullptr;

	// size of the map
	uint width;
	uint height;
	// all map walkability values [0..255]
	uchar* map;
	// we store the created path here
	DynArray<iPoint> lastPath;
};

// forward declaration
struct PathList;

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& listToFill) const;
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);

	// -----------
	int g;
	int h;
	iPoint pos;
	const PathNode* parent = nullptr; // needed to reconstruct the path in the end
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	ListItem<PathNode>* Find(const iPoint& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	ListItem<PathNode>* GetNodeLowestScore() const;

	// -----------
	// The list itself, note they are not pointers!
	List<PathNode> list;
};



#endif // __PATHFINDING_H__