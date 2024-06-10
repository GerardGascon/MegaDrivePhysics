#include <genesis.h>

#include "boxes.h"

int main() {
	SPR_init();

	initializeBoxes();
	
	while (TRUE){
		VDP_showFPS(FALSE);
		moveBoxes();
		
		SPR_update();
		SYS_doVBlankProcess();
	}
}
