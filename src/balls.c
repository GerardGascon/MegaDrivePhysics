//
// Created by ggasc on 08/06/2024.
//
#include <genesis.h>

#include "balls.h"
#include "resources.h"

#define BALLS 40

struct ball {
	Vect2D_f16 position;
	Vect2D_s16 integerPosition;
	
	Vect2D_f16 velocity;
	
	int radius;
	
	Sprite* sprite;
};

struct ball* balls;

void initializeBalls(){
	balls = MEM_alloc(sizeof(*balls) * BALLS);

	PAL_setColor(1, RGB24_TO_VDPCOLOR(0xffffff));
	PAL_setColor(17, RGB24_TO_VDPCOLOR(0xff0000));
	PAL_setColor(33, RGB24_TO_VDPCOLOR(0x00ff00));
	PAL_setColor(49, RGB24_TO_VDPCOLOR(0x0000ff));
	
	for (int i = 0; i < BALLS; ++i) {
		s16 posX = random() % 300;
		s16 posY = random() % 200;
		fix16 velX = random() % (FIX16(1) - FIX16(-1) + 1) + FIX16(-1);
		fix16 velY = random() % (FIX16(1) - FIX16(-1) + 1) + FIX16(-1);
		
		balls[i].position = (Vect2D_f16){intToFix16(posX), intToFix16(posY)};
		balls[i].integerPosition = (Vect2D_s16){posX, posY};
		balls[i].velocity = (Vect2D_f16){velX, velY};
		
		int pal = random() % 4;
		
		balls[i].radius = 7;
		balls[i].sprite = SPR_addSprite(&ball, posX, posY, TILE_ATTR(pal, 0, FALSE, FALSE));
	}
}

void moveBall(struct ball* b){
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

bool intersectCircles(struct ball* b1, struct ball* b2){
	u32 distance = getApproximatedDistance(
			b1->integerPosition.x - b2->integerPosition.x, 
			b1->integerPosition.y - b2->integerPosition.y);
	u16 radii = b1->radius + b2->radius;

	if (distance >= radii)
		return FALSE;
	return TRUE;
}

void resolveCircleCollision(struct ball* b1, struct ball* b2){
	int middlePointX = (b1->integerPosition.x + b2->integerPosition.x) >> 1;
	int middlePointY = (b1->integerPosition.y + b2->integerPosition.y) >> 1;

	bool velocityToPick = random() % 2; //It's just an experiment it doesn't have to be realistic
	Vect2D_f16 resultantVelocity = (Vect2D_f16){
		abs(velocityToPick ? b1->velocity.x : b2->velocity.x),
		abs(velocityToPick ? b1->velocity.y : b2->velocity.y)
	};

	if (middlePointX > b1->integerPosition.x){
		b1->velocity.x = -resultantVelocity.x;
	}else if (middlePointX < b1->integerPosition.x){
		b1->velocity.x = resultantVelocity.x;
	}else{
		b1->velocity.x = 0;
	}
	if (middlePointY > b1->integerPosition.y){
		b1->velocity.y = -resultantVelocity.y;
	}else if (middlePointY < b1->integerPosition.y){
		b1->velocity.y = resultantVelocity.y;
	}else{
		b1->velocity.y = 0;
	}

	if (middlePointX > b2->integerPosition.x){
		b2->velocity.x = -resultantVelocity.x;
	}else if (middlePointX < b2->integerPosition.x){
		b2->velocity.x = resultantVelocity.x;
	}else{
		b2->velocity.x = 0;
	}
	if (middlePointY > b2->integerPosition.y){
		b2->velocity.y = -resultantVelocity.y;
	}else if (middlePointY < b2->integerPosition.y){
		b2->velocity.y = resultantVelocity.y;
	}else{
		b2->velocity.y = 0;
	}
}

void moveBalls(){
	for (int i = 0; i < BALLS; ++i) {
		moveBall(&balls[i]);

		for (int j = i + 1; j < BALLS; ++j) {
			if (intersectCircles(&balls[i], &balls[j])){
				resolveCircleCollision(&balls[i], &balls[j]);
				moveBall(&balls[i]);
			}
		}
	}
}