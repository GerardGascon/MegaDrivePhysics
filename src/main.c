#include <genesis.h>

#include "balls.h"

int main() {
	SPR_init();

	initializeBalls();
	
	while (TRUE){
		moveBalls();
		
		SPR_update();
		SYS_doVBlankProcess();
	}
}
