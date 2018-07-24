#pragma once
//#include "SDLMain.h"
//#include "Texture.h"
#include <math.h>

#define M_PI 3.14159265358979323846
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

#define FOV 60 // Players field of view 
#define PLAYER_HEIGHT 32
#define TILE_HEIGHT 64// tiles must be a power of 2 so they can be shifted neatly
#define SHIFT_VALUE 6 // shift value is x where 2^x = TILE_HEIGHT 

#define VIEW_WIDTH 640
#define VIEW_HEIGHT 400
#define PLANE_DIST 255
#define VIEW_DIMENSION VIEW_WIDTH * VIEW_HEIGHT // total viewport dimension
#define ANGLE_INCREMENT FOV / VIEW_WIDTH // angle between rays

#define MAP_WIDTH 8
#define MAP_HEIGHT 6
#define MAX_INT 2147483647

#define PROJ_WALL_HEIGHT TILE_HEIGHT / 277

#define unitToGrid(x) x >> SHIFT_VALUE
#define gridToUnit(x) x << SHIFT_VALUE


typedef struct Point {
	int x;
	int y;	
	Point operator-(const Point& op2);
} POINT;

template <typename array > class Raycaster {

public:

	//Raycaster(array _mapArray, POINT* _playerCoords, int _fov, int _viewWidth, int _viewHeight, int _tileSize);
	//Raycaster(array _mapArray, POINT* _playerCoords) // use defaults
	

private:
	int fov; // the players field of view
	int viewWidth; // the visible plane
	int viewHeight; // the visible plane
	int distToProjPlane; // distance to projection plane
	int playerAngle; // angle the player is currently looking
	double rayAngle; // angle of current ray being cast
	double angleIncrement; // advances ray to next column line
	array mapArray; // two dimensional map grid
	int tileSize; // size of each grid unit
	POINT* playerCoords; // unit coordinates of the player
};

// takes respective player coords and angle of the cast, and 2D array of grid
POINT castRay(POINT* playerCoord, double angle, int mapArray[][8]);
int getDistanceToWall(POINT* wallCoord, POINT* playerCoord);
POINT getInitialIntersection(double angle, POINT* playerCoord, bool verticalIntersection);
