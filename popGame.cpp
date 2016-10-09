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
#include <AS3/AS3++.h> // using AS3 var wrapper class
#include <Flash++.h> // using AVM2 sync primitives
#include <string>
#include <pthread.h>
#include "popGame.h"
#include "object_Background.h"

// use "ui" AS3 var wrappers which marshall var manipulations to the ui Worker
using namespace AS3::ui;

popGame::popGame()
{
	// put the sky background onto the stage
	object_Background *background;
	background = new object_Background();
			
	// create the initial game state sprites
	flash::display::Sprite sgSprite = flash::display::Sprite::_new();
	
	// set the initial sgSprite
	sgSprite->name = "not set";
	
	// set game stat to 0, i.e., MENU_STATE
    var tempAS3int = internal::new_int(0);
    this->setCurrrentState((void*)&tempAS3int, internal::_null);
	
	// declare & create a new thread to update popGame
	pthread_t thread;
    pthread_create(&thread, NULL, popGame::update, &sgSprite);
	// wait for exit to be signaled
	avm2_self_msleep(&mainCond, 0);
	// wait for it to finish
	pthread_join(thread, NULL);
	
	printf("popGame\nconstructor\n**\n");  
};
