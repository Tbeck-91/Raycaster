void render()
{
	drawBackground();
	drawOverheadMap();

	int verticalGrid;        // horizotal or vertical coordinate of intersection
	int horizontalGrid;      // theoritically, this will be multiple of TILE_SIZE
							 // , but some trick did here might cause
							 // the values off by 1
	int distToNextVerticalGrid; // how far to the next bound (this is multiple of
	int distToNextHorizontalGrid; // tile size)
	float xIntersection;  // x and y intersections
	float yIntersection;
	float distToNextXIntersection;
	float distToNextYIntersection;

	int xGridIndex;        // the current cell that the ray is in
	int yGridIndex;

	float distToVerticalGridBeingHit;      // the distance of the x and y ray intersections from
	float distToHorizontalGridBeingHit;      // the viewpoint

	int castArc, castColumn;

	castArc = fPlayerArc;
	// field of view is 60 degree with the point of view (player's direction in the middle)
	// 30  30
	//    ^
	//  \ | /
	//   \|/
	//    v
	// we will trace the rays starting from the leftmost ray
	castArc -= ANGLE30;
	// wrap around if necessary
	if (castArc < 0)
	{
		castArc = ANGLE360 + castArc;
	}

	for (castColumn = 0; castColumn<PROJECTIONPLANEWIDTH; castColumn += 5)
	{
		// ray is between 0 to 180 degree (1st and 2nd quadrant)
		// ray is facing down
		if (castArc > ANGLE0 && castArc < ANGLE180)
		{
			// truncuate then add to get the coordinate of the FIRST grid (horizontal
			// wall) that is in front of the player (this is in pixel unit)
			// ROUND DOWN
			horizontalGrid = (fPlayerY / TILE_SIZE)*TILE_SIZE + TILE_SIZE;

			// compute distance to the next horizontal wall
			distToNextHorizontalGrid = TILE_SIZE;

			/* TAYLORS NOTE: MULTIPLYING BY THE FITANTABLE IS THE SAME AS DIVIDING BY THE TAN OF THE ANGLE */
			float xtemp = fITanTable[castArc] * (horizontalGrid - fPlayerY);
			// we can get the vertical distance to that wall by
			// (horizontalGrid-GLplayerY)
			// we can get the horizontal distance to that wall by
			// 1/tan(arc)*verticalDistance
			// find the x interception to that wall
			xIntersection = xtemp + fPlayerX;
		}
		// else, the ray is facing up
		else
		{
			horizontalGrid = (fPlayerY / TILE_SIZE)*TILE_SIZE;
			distToNextHorizontalGrid = -TILE_SIZE;

			float xtemp = fITanTable[castArc] * (horizontalGrid - fPlayerY);
			xIntersection = xtemp + fPlayerX;

			horizontalGrid--;
		}
		// LOOK FOR HORIZONTAL WALL
		if (castArc == ANGLE0 || castArc == ANGLE180)
		{
			distToHorizontalGridBeingHit = 9999999F;//Float.MAX_VALUE;
		}
		// else, move the ray until it hits a horizontal wall
		else
		{
			distToNextXIntersection = fXStepTable[castArc];
			while (true)
			{
				xGridIndex = (int)(xIntersection / TILE_SIZE);
				// in the picture, yGridIndex will be 1
				yGridIndex = (horizontalGrid / TILE_SIZE);

				if ((xGridIndex >= MAP_WIDTH) ||
					(yGridIndex >= MAP_HEIGHT) ||
					xGridIndex<0 || yGridIndex<0)
				{
					distToHorizontalGridBeingHit = Float.MAX_VALUE;
					break;
				}
				else if ((fMap[yGridIndex*MAP_WIDTH + xGridIndex]) != O)
				{
					distToHorizontalGridBeingHit = (xIntersection - fPlayerX)*fICosTable[castArc];
					break;
				}
				// else, the ray is not blocked, extend to the next block
				else
				{
					xIntersection += distToNextXIntersection;
					horizontalGrid += distToNextHorizontalGrid;
				}
			}
		}


		// FOLLOW X RAY
		if (castArc < ANGLE90 || castArc > ANGLE270)
		{
			verticalGrid = TILE_SIZE + (fPlayerX / TILE_SIZE)*TILE_SIZE;
			distToNextVerticalGrid = TILE_SIZE;

			float ytemp = fTanTable[castArc] * (verticalGrid - fPlayerX);
			yIntersection = ytemp + fPlayerY;
		}
		// RAY FACING LEFT
		else
		{
			verticalGrid = (fPlayerX / TILE_SIZE)*TILE_SIZE;
			distToNextVerticalGrid = -TILE_SIZE;

			float ytemp = fTanTable[castArc] * (verticalGrid - fPlayerX);
			yIntersection = ytemp + fPlayerY;

			verticalGrid--;
		}
		// LOOK FOR VERTICAL WALL
		if (castArc == ANGLE90 || castArc == ANGLE270)
		{
			distToVerticalGridBeingHit = 9999999;//Float.MAX_VALUE;
		}
		else
		{
			distToNextYIntersection = fYStepTable[castArc];
			while (true)
			{
				// compute current map position to inspect
				xGridIndex = (verticalGrid / TILE_SIZE);
				yGridIndex = (int)(yIntersection / TILE_SIZE);

				if ((xGridIndex >= MAP_WIDTH) ||
					(yGridIndex >= MAP_HEIGHT) ||
					xGridIndex<0 || yGridIndex<0)
				{
					distToVerticalGridBeingHit = Float.MAX_VALUE;
					break;
				}
				else if ((fMap[yGridIndex*MAP_WIDTH + xGridIndex]) != O)
				{
					distToVerticalGridBeingHit = (yIntersection - fPlayerY)*fISinTable[castArc];
					break;
				}
				else
				{
					yIntersection += distToNextYIntersection;
					verticalGrid += distToNextVerticalGrid;
				}
			}
		}