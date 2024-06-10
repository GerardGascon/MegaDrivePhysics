//
// Created by ggasc on 10/06/2024.
//
#include <genesis.h>
#include "resources.h"

#define BOXES 30
#define VELOCITY_RANGE 3

struct box {
	Vect2D_f16 position;
	Vect2D_s16 integerPosition;

	Vect2D_f16 velocity;

	Vect2D_s16 size;

	Sprite* sprite;
};

struct aabb {
	Vect2D_s16 position;
	Vect2D_s16 size;
};

struct box* boxes;

void initializeBoxes(){
	boxes = MEM_alloc(sizeof(*boxes) * BOXES);

	PAL_setColor(1, RGB24_TO_VDPCOLOR(0xffffff));
	PAL_setColor(17, RGB24_TO_VDPCOLOR(0xff0000));
	PAL_setColor(33, RGB24_TO_VDPCOLOR(0x00ff00));
	PAL_setColor(49, RGB24_TO_VDPCOLOR(0x0000ff));

	for (int i = 0; i < BOXES; ++i) {
		s16 posX = random() % 300;
		s16 posY = random() % 200;
		fix16 velX = random() % (FIX16(VELOCITY_RANGE) - FIX16(-VELOCITY_RANGE) + 1) + FIX16(-VELOCITY_RANGE);
		fix16 velY = random() % (FIX16(VELOCITY_RANGE) - FIX16(-VELOCITY_RANGE) + 1) + FIX16(-VELOCITY_RANGE);

		boxes[i].position = (Vect2D_f16){intToFix16(posX), intToFix16(posY)};
		boxes[i].integerPosition = (Vect2D_s16){posX, posY};
		boxes[i].velocity = (Vect2D_f16){velX, velY};

		int pal = random() % 4;

		boxes[i].size = (Vect2D_s16){16, 16};
		boxes[i].sprite = SPR_addSprite(&ball, posX, posY, TILE_ATTR(pal, 0, FALSE, FALSE));
	}
}

void moveBox(struct box* b){
	int posX = fix16ToInt(b->position.x);
	int posY = fix16ToInt(b->position.y);
	if (posX >= (320 - 16) || posX <= 0){
		b->velocity.x = -b->velocity.x;
	}
	if (posY >= (224 - 16) || posY <= 0){
		b->velocity.y = -b->velocity.y;
	}

	b->position.x += b->velocity.x;
	b->position.y += b->velocity.y;

	b->integerPosition = (Vect2D_s16){fix16ToInt(b->position.x), fix16ToInt(b->position.y)};

	SPR_setPosition(b->sprite, b->integerPosition.x, b->integerPosition.y);
}

bool intersectBoxes(struct box* b1, struct box* b2, struct aabb* intersection){
	s16 b1X1 = b1->integerPosition.x;
	s16 b1X2 = b1->integerPosition.x + b1->size.x;
	s16 b1Y1 = b1->integerPosition.y;
	s16 b1Y2 = b1->integerPosition.y + b1->size.y;

	s16 b2X1 = b2->integerPosition.x;
	s16 b2X2 = b2->integerPosition.x + b2->size.x;
	s16 b2Y1 = b2->integerPosition.y;
	s16 b2Y2 = b2->integerPosition.y + b2->size.y;
	
	if (b1X1 <= b2X2 && b1X2 >= b2X1 && b1Y1 <= b2Y2 && b1Y2 >= b2Y1){
		s16 x1 = max(b1X1, b2X1);
		s16 x2 = min(b1X2, b2X2);
		s16 y1 = max(b1Y1, b2Y1);
		s16 y2 = min(b1Y2, b2Y2);
		*intersection = (struct aabb){{x1, y1}, {x2, y2}};
		return TRUE;
	}
	return FALSE;
}

void resolveCollision(struct box* b1, struct box* b2, struct aabb intersection){
	
}

void moveBoxes(){
	for (int i = 0; i < BOXES; ++i) {
		moveBox(&boxes[i]);

		for (int j = i + 1; j < BOXES; ++j) {
			struct aabb intersection;
			if (intersectBoxes(&boxes[i], &boxes[j], &intersection)){
				resolveCollision(&boxes[i], &boxes[j], intersection);
				moveBox(&boxes[i]);
			}
		}
	}
}