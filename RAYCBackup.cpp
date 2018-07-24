#include "Raycast.h"
#include <stdio.h>

 #define DEBUG_PRINT_1

// TODO : Break up logical areas of this huge function into different functions
// TODO : Turn this function into a class?
POINT castRay(POINT * playerCoord, double angle, int mapArray[][8]) {

	bool downwardRay = angle > 180;
	bool leftwardRay = angle > 90 && angle < 270;

	/* Initial intersection points for the Vertical and Horizontal edges */
	POINT initialHorEdge = getInitialIntersection(angle, playerCoord, false); 
	POINT initialVerEdge = getInitialIntersection(angle, playerCoord, true);

	/* Calculate Offsets to advance to next edge when checking horizontal and vertical edges */

	int horDX = TILE_HEIGHT / fabs( tan(degToRad(angle)) ); // delta x for horizontal intersections
	if (leftwardRay) horDX = -horDX;
	int horDY = downwardRay ? TILE_HEIGHT : -TILE_HEIGHT; // delta y for horizontal intersections
	
	int verDY = TILE_HEIGHT * fabs ( tan(angle) );
	if (!downwardRay) verDY = -verDY; // we're moving up

	int verDX = leftwardRay ? -TILE_HEIGHT : TILE_HEIGHT;
	
	bool hitWall = false;
	POINT curEdge = { 0, 0 };

	// advance ray until we hit a wall 
	while (!hitWall) { 

		int distToHorEdge = abs(initialHorEdge.x - playerCoord->x) + abs(initialHorEdge.y - playerCoord->y);
		int distToVerEdge = abs(initialVerEdge.x - playerCoord->x) + abs(initialVerEdge.y - playerCoord->y);

		if (angle == 0 || angle == 180) distToHorEdge = 0xFFFFFFFF; // ray is parallel to hor edge
		else if (angle == 90 || angle == 270) distToVerEdge = 0xFFFFFFFF;  // ray is parallel to ver edge

		// step to closest intersection

		if (distToHorEdge < distToVerEdge) {

			initialHorEdge.x += horDX;
			initialHorEdge.y += horDY;		

			curEdge = initialHorEdge;
		}
		else {		

			initialVerEdge.x += verDX;
			initialVerEdge.y += verDY;	

			curEdge = initialVerEdge;
		}	

		if (curEdge.x < 0) curEdge.x = 0;
		else if (curEdge.y < 0) curEdge.y = 0;

		int testY = (curEdge.y >> SHIFT_VALUE);
		int testX = (curEdge.x >> SHIFT_VALUE);



		if (testY >= MAP_HEIGHT || testX >= MAP_WIDTH || testY < 0 || testX < 0) {

		#ifdef DEBUG_PRINT_1
					printf("Ray has exceeded map boundaries at: (%d, %d) \n", curEdge.x >> SHIFT_VALUE, curEdge.y >> SHIFT_VALUE);
		#endif // DEBUG_PRINT					
					break;
		}

		if (!mapArray[curEdge.y >> SHIFT_VALUE][curEdge.x >> SHIFT_VALUE]) hitWall = true;		
	}

	#ifdef DEBUG_PRINT

		printf("Final tile hit was (%d, %d) \n", curEdge.x >> SHIFT_VALUE, curEdge.y >> SHIFT_VALUE);

		printf("*************** HORIZONTAL INTERSECTION ********************* \n\n");
		printf("X Equals Unit Coords: %d Grid Coords: %d\n", initialHorEdge.x, initialHorEdge.x >> SHIFT_VALUE);
		printf("Y Equals Unit Coords: %d Grid Coords: %d\n\n", initialHorEdge.y, initialHorEdge.y >> SHIFT_VALUE);
	
		printf("*************** VERTICAL INTERSECTION ********************* \n\n");
		printf("X Equals Unit Coords: %d Grid Coords: %d\n", initialVerEdge.x, initialVerEdge.x >> SHIFT_VALUE);
		printf("Y Equals Unit Coords: %d Grid Coords: %d\n", initialVerEdge.y, initialVerEdge.y >> SHIFT_VALUE);
	#endif

	return curEdge;
}

POINT getInitialIntersection(double angle, POINT* playerCoord, bool verticalIntersection) {

	bool downwardRay = angle > 180;
	bool leftwardRay = angle > 90 && angle < 270;

	/* Initial intersection points for the Vertical and Horizontal edges */
	POINT intersection;

	int yOffset;
	int xOffset;

	if (verticalIntersection) {

		intersection.x = (playerCoord->x >> SHIFT_VALUE) << SHIFT_VALUE;
		intersection.x += leftwardRay ? -1 : TILE_HEIGHT;		

		if (leftwardRay) xOffset = (playerCoord->x - intersection.x);
		else xOffset = (intersection.x - playerCoord->x);

		yOffset = xOffset * fabs( tan(degToRad(angle)) );
		if (angle == 0 || angle == 180) yOffset = 0;
		if (!downwardRay) yOffset = -yOffset; // line is decreasing on the y axis, so negate the offset
		intersection.y = playerCoord->y + yOffset;

	}
	else {


		intersection.y = (playerCoord->y >> SHIFT_VALUE) << SHIFT_VALUE; // get current tile in unit coords. Here we shift instead of multiplying as it's faster
		intersection.y = (intersection.y + (downwardRay ? TILE_HEIGHT : -1));
		
	    /* Get Y offset from player y to first horizontal intersection */
		if (downwardRay) yOffset = (intersection.y - playerCoord->y);
		else yOffset = (playerCoord->y - intersection.y);

		/* Get X offset from player x to first horizontal intersection */
		// TODO: what do we do when tan equation is negative?
		xOffset = (angle == 90 || angle == 270) ? 0 : yOffset / fabs( tan(degToRad(angle)) ); // make sure we account for division by zero errors
		if (leftwardRay) xOffset = -xOffset; // line is decreasing on the x axis, so negate the offset
		intersection.x = playerCoord->x + xOffset;
	}

	return intersection;
}


int getDistanceToWall(POINT* wallCoord, POINT* playerCoord) {

	int x = (wallCoord->x - playerCoord->x);
	int y = (wallCoord->x - playerCoord->x);

	return sqrt(pow(x, 2) + pow(y, 2));
}

