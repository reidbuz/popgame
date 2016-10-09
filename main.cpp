/*
** Pop!
** by John Reid Perkins-Buzo
**  Copyright (C) 2013
**
** Pop! is a game that would be about poping as
** many balloons as you can in a certain time
** interval.

    This file is part of Pop!.

    Pop! is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pop! is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pop!.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <AS3/AS3.h>
#include "popGame.h"


int main()
{   
	popGame *popper;
    popper = new popGame();
	
	printf("main\n**\n");
    
    //AS3_GoAsync();
    delete popper;
    return 0;
}
