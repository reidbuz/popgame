T0: check
	@echo "-------- popGame ABC--------"
	
	"$(FLASCC)/usr/bin/g++" $(BASE_CFLAGS) popGame.cpp main.cpp -swf-size=960x540 -emit-swf -o popGameABC.swf -lAS3++ -lFlash++ -pthread

include ../Makefile.common

clean:
	rm -f *.swf
