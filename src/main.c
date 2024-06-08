#include <genesis.h>

int main() {
	VDP_drawText("Hello, Mega Drive World!", 8, 5);

	while (TRUE){
		SYS_doVBlankProcess();
	}
}
