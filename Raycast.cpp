#include "Raycast.h"
#include <stdio.h>
 


//#define DEBUG_PRINT_1

POINT castRay(POINT * playerCoord, double angle, int mapArray[][8]) {

	bool downwardRay = angle > 180;
	bool leftwardRay = angle > 90 && angle < 270;
	bool horizontalRay;

	if (ceil(angle) == 270 || floor(angle) == 270) angle = 270;
	else if (ceil(angle) == 90 || floor(angle) == 90) angle = 90;
	else if (ceil(angle) == 180 || floor(angle) == 180) angle = 180;
	else if (floor(angle) == 0 || ceil(angle) == 360) angle = 0;

	/* Initial intersection points for the Vertical and Horizontal edges */
	POINT initialHorEdge = getInitialIntersection(angle, playerCoord, false);
	POINT initialVerEdge = getInitialIntersection(angle, playerCoord, true);

	/* Calculate Offsets to advance to next edge when checking horizontal and vertical edges */

	int horDX = fabs(tan(degToRad(angle))) != 0 ? TILE_HEIGHT / fabs(tan(degToRad(angle))) : 0; // delta x for horizontal intersections
	if (leftwardRay) horDX = -horDX;
	int horDY = downwardRay ? TILE_HEIGHT : -TILE_HEIGHT; // delta y for horizontal intersections

	int verDY = TILE_HEIGHT * fabs(tan(degToRad(angle)));
	if (!downwardRay) verDY = -verDY; // we're moving up

	int verDX = leftwardRay ? -TILE_HEIGHT : TILE_HEIGHT;

	bool isWallHit = false;
	POINT curEdge = { 0, 0 };
	
	int distToHorEdge = getDistanceToWall(playerCoord, &initialHorEdge);
	int distToVerEdge = getDistanceToWall(playerCoord, &initialVerEdge);

	if (angle == 0 || angle == 180) distToHorEdge = MAX_INT; // ray is parallel to hor edge
	else if (angle == 90 || angle == 270) distToVerEdge = MAX_INT;  // ray is parallel to ver edge
	
	/* Check if the initial intersections hit a wall */
	if (!(angle == 0 || angle == 180) && distToHorEdge < distToVerEdge && !mapArray[unitToGrid(initialHorEdge.y)][unitToGrid(initialHorEdge.x)]) {
		isWallHit = true;
		curEdge = initialHorEdge;
	}

	else if (!(angle == 270 || angle == 90) && distToVerEdge < distToHorEdge && !mapArray[unitToGrid(initialVerEdge.y)][unitToGrid(initialVerEdge.x)]) {
		isWallHit = true;
		curEdge = initialVerEdge;
	}	
	
	// advance ray until we hit a wall 
	while (!isWallHit) {

		// STEP to next intersections
		if (!(angle == 0 || angle == 180) && mapArray[unitToGrid(initialHorEdge.y)][unitToGrid(initialHorEdge.x)]) { // stop stepping if we've already hit a wall
			initialHorEdge.x += horDX;
			initialHorEdge.y += horDY;
		}
		if (initialHorEdge.x < 0) initialHorEdge.x = 0;
		if (initialHorEdge.y < 0) initialHorEdge.y = 0;

		if (!(angle == 270 || angle == 90) && mapArray[unitToGrid(initialVerEdge.y)][unitToGrid(initialVerEdge.x)]) {
			initialVerEdge.x += verDX;
			initialVerEdge.y += verDY;
		}
		if (initialVerEdge.x < 0) initialVerEdge.x = 0;
		if (initialVerEdge.y < 0) initialVerEdge.y = 0;

		distToHorEdge = getDistanceToWall(playerCoord, &initialHorEdge);
		distToVerEdge = getDistanceToWall(playerCoord, &initialVerEdge);

		if (angle == 0 || angle == 180) distToHorEdge = MAX_INT; // ray is parallel to hor edge
		else if (angle == 90 || angle == 270) distToVerEdge = MAX_INT;  // ray is parallel to ver edge	

		// step to closest intersection
		if (distToHorEdge < distToVerEdge) {
			curEdge = initialHorEdge;
		}
		else {
			curEdge = initialVerEdge;
		}

		if (curEdge.x < 0) curEdge.x = 0;
		else if (curEdge.y < 0) curEdge.y = 0;

		/* Convert current intersection to grid/map coords */
		int testY = unitToGrid(curEdge.y);
		int testX = unitToGrid(curEdge.x);	
		
		if (!mapArray[testY][testX]) isWallHit = true; // We've hit a wall, walls are represented by zero
	}

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

		intersection.x = unitToGrid(playerCoord->x) << SHIFT_VALUE;
		intersection.x += leftwardRay ? -1 : TILE_HEIGHT;

		if (leftwardRay) xOffset = (playerCoord->x - intersection.x);
		else xOffset = (intersection.x - playerCoord->x);

		yOffset = xOffset * fabs(tan(degToRad(angle)));
		if (angle == 0 || angle == 180) yOffset = 0;
		if (!downwardRay) yOffset = -yOffset; // line is decreasing on the y axis, so negate the offset
		intersection.y = playerCoord->y + yOffset;

	}
	else {


		intersection.y = unitToGrid(playerCoord->y) << SHIFT_VALUE; // get current tile in unit coords. Here we shift instead of multiplying as it's faster
		intersection.y = (intersection.y + (downwardRay ? TILE_HEIGHT : -1));

		/* Get Y offset from player y to first horizontal intersection */
		if (downwardRay) yOffset = (intersection.y - playerCoord->y);
		else yOffset = (playerCoord->y - intersection.y);

		/* Get X offset from player x to first horizontal intersection */
		xOffset = (angle == 90 || angle == 270) ? 0 : yOffset / fabs(tan(degToRad(angle))); // make sure we account for division by zero errors
		if (leftwardRay) xOffset = -xOffset; // line is decreasing on the x axis, so negate the offset
		intersection.x = playerCoord->x + xOffset;
	}

	return intersection;
}


int getDistanceToWall(POINT* wallCoord, POINT* playerCoord) {

	int x = abs((playerCoord->x - wallCoord->x));
	int y = abs((playerCoord->y - wallCoord->y ));
	int c = pow(x, 2) + pow(y, 2);

	return sqrt(c);
}

Point Point::operator-(const Point & op2) {

	Point retPoint;
	retPoint.x = this->x - op2.x;
	retPoint.y = this->y - op2.y;

	return retPoint;
}
