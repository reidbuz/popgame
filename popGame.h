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
#ifndef _POPGAME_H_
#define _POPGAME_H_

#include <AS3/AS3++.h> // using AS3 var wrapper class
#include <Flash++.h> // using AVM2 sync primitives
#include "soundManager.h"
#include "object_menu.h"
#include "object_play.h"
#include "object_gameover.h"

// use "ui" AS3 var wrappers which marshall var manipulations to the ui Worker
using namespace AS3::ui;

static char checkGameStateCond; // just use the address as a condition
static char mainCond; // address used to wake the avm2_self_msleep in main (though no one signals it right now)
static int current_state;
static int balloonCount;
static int numberOfBalloonsPopped;


class popGame {
	private:
		
	public:
		// see popGame.cpp for the constructor
		popGame();
				
		// update popGame 
		static void *update(void *arg) 
		{
			// sgSprite was passed to us as a ptr
			// it's a "ui" var so any operations are marshalled to the ui Worker...
			// so we can use it from any thread
			flash::display::Sprite sgSprite = *(var *)arg;
	
			// get reference to the stage and set a listener to the stage's ENTER_FRAME event
			flash::display::Stage stage = internal::get_Stage();
			stage->addEventListener(flash::events::Event::ENTER_FRAME, Function::_new(stateMachine, (void*)&sgSprite), false, 0, false);
			
			for(;;)
			{
				// sleep, allowing the ui thread to call stateMachine for us
				avm2_self_msleep(&checkGameStateCond, 0);
			}
			
			// this is never reached
			return NULL;
		  };
		
		static var stateMachine(void *arg, var as3Args)
		{
			// update the sgSprite
			flash::display::Sprite sgSprite = *(var *)arg;
			
			// get reference to the stage
			flash::display::Stage stage = internal::get_Stage();

   	 		// this var tracks the upcoming state
    		var next_state = internal::new_int((current_state + 1) % 3);
    		
			// this boolean tracks if the state has changed from the previous frame; it is intialized as true 
			bool noChange = true;
			
			// find the Game State currently on the stage and track it with sgSprite
			flash::display::DisplayObject target = stage->getChildByName(internal::new_String("MENU_STATE"));
			bool targetIsNull = internal::equals(target, internal::_null);
			if (!targetIsNull) 
			{
				sgSprite = stage->getChildByName(internal::new_String("MENU_STATE"));
			} else
			{
				target = stage->getChildByName(internal::new_String("PLAY_STATE"));
				targetIsNull = internal::equals(target, internal::_null);
				if (!targetIsNull) 
				{
					sgSprite = stage->getChildByName(internal::new_String("PLAY_STATE"));
				} else
				{
					target = stage->getChildByName(internal::new_String("GAMEOVER_STATE"));
					targetIsNull = internal::equals(target, internal::_null);
					if (!targetIsNull) 
					{
						sgSprite = stage->getChildByName(internal::new_String("GAMEOVER_STATE"));
					}
				}
			  }
			 // get the name of the present sgSprite
			 var sgName = sgSprite->name;
			
			switch (current_state)
				{
					case 0: //CASE MENU_STATE
						if (strcmp(internal::utf8_toString(sgName), "MENU_STATE") != 0)	
						{	
							// check to see if this is the first time the game is being played; if not remove the present sgSprite 
							if (strcmp(internal::utf8_toString(sgName), "not set") != 0) 
							{
								printf("removing\n%s\n**\n", internal::utf8_toString(sgName));
								stage->removeChild(sgSprite);
							}
							
							// make the stateMenuSprite
							object_menu *menu_state;
							menu_state = new object_menu();
							sgSprite = stage->getChildByName(internal::new_String("MENU_STATE"));
							sgName = sgSprite->name;
							
							// add an mouse event listener to the buttonSprite 
							sgSprite->getChildByName(internal::new_String("buttonSprite"))->addEventListener(flash::events::MouseEvent::MOUSE_DOWN, Function::_new(setCurrrentState, (void*)&next_state), false, 0, false);

							numberOfBalloonsPopped = 0;
							noChange = false;
						}
    				break;
			
					case 1: //CASE PLAY_STATE
						if (strcmp(internal::utf8_toString(sgName), "PLAY_STATE") != 0)	
						{
							// remove present sgSprite
							printf("removing\n%s\n**\n", internal::utf8_toString(sgName));
							stage->removeChild(sgSprite);
							
							// make the statePlaySprite
							object_play *play_state;
							play_state = new object_play();
							sgSprite = stage->getChildByName(internal::new_String("PLAY_STATE"));
							sgName = sgSprite->name;
							noChange = false;
							
							// start the game play
							playHandler((void*)&sgSprite, internal::_null);
						}
					break;
					
					case 2: //CASE GAMEOVER_STATE
						if (strcmp(internal::utf8_toString(sgName), "GAMEOVER_STATE") != 0)	
						{
							// remove present sgSprite
							printf("removing\n%s\n**\n", internal::utf8_toString(sgName));
							stage->removeChild(sgSprite);
							
							// make the stateGameOverSprite
							object_gameover *gameover_state;
							gameover_state = new object_gameover(numberOfBalloonsPopped);
							sgSprite = stage->getChildByName(internal::new_String("GAMEOVER_STATE"));
							sgName = sgSprite->name;
							
							// add the mouse event listener to the buttonSprite
							sgSprite->getChildByName(internal::new_String("buttonSprite"))->addEventListener(flash::events::MouseEvent::MOUSE_DOWN, Function::_new(setCurrrentState, (void*)&next_state), false, 0, false);
							
							noChange = false;
						}
					break;
					
					default:  //cover all possibilties
						printf("In\nNULLState\n**\n");
					break;
				}
			
			if (!noChange)
			{
				printf("Sprite =\n%s\n**\n", internal::utf8_toString(sgName));
				printf("next_state\n(%d)\n**\n", internal::int_valueOf(next_state));
			}

			// signal threadProc's avm2_self_msleep to wake up
  			avm2_wake(&checkGameStateCond);
			return internal::_undefined;		
		};
		
		static var playHandler(void *arg, var as3Args)
		{	
			// get reference to the stage and the playStateSprite if it's on the stage
			flash::display::Stage stage = internal::get_Stage();
			flash::display::Sprite sgSprite = stage->getChildByName(internal::new_String("PLAY_STATE"));
			
			// if there's no pen in the playSprite, create one and add it to the playSprite.
			flash::display::DisplayObject target = sgSprite->getChildByName(internal::new_String("pen"));
			if (internal::equals(target, internal::_null)) 
			{
				// set up penSprite as a new Sprite & get penSprite.graphics
				flash::display::Sprite penSprite = flash::display::Sprite::_new();
				penSprite->name = "pen";
				flash::display::Graphics penGraphics = penSprite->graphics;
				
				// hide the mouse cursor
				flash::ui::Mouse::hide();
				
				// draw pen
				int size = 100;
				penGraphics->beginFill(0x000000, 1.0);
				penGraphics->moveTo(0, 0); 
				penGraphics->curveTo(100, 0, 50, 25);
				penGraphics->endFill();
				
				// add it to the playSprite
				sgSprite->addChild(penSprite);
				penSprite->x = 480;
				penSprite->y = 270;
				
				// add a listener & if a mouseDown or mouseMove event happens, open a Function to respond
				sgSprite->addEventListener(flash::events::MouseEvent::MOUSE_DOWN, Function::_new(mouseDownHandler, (void*)&sgSprite), false, 0, false);
				sgSprite->addEventListener(flash::events::MouseEvent::MOUSE_MOVE, Function::_new(mouseMoveHandler, (void*)&sgSprite), false, 0, false);
				sgSprite->addEventListener(flash::events::MouseEvent::MOUSE_OVER, Function::_new(makeBalloon, (void*)&sgSprite), false, 0, false);
			}
			
			return internal::_undefined;
		};
				
		// mouseMove listener makes the Sprite follow the mouse -- associated with a background thread
		static var mouseMoveHandler(void *arg, var as3Args)
		{
			//printf("mouse\nMoveH\n**\n");
			flash::events::MouseEvent event = var(as3Args[0]);
			
			// get references to the stage, the playStateSprite and the penSprite
			flash::display::Stage stage = internal::get_Stage();
			flash::display::Sprite playSprite = stage->getChildByName(internal::new_String("PLAY_STATE"));
		    flash::display::Sprite pSprite = playSprite->getChildByName(internal::new_String("pen"));
			
			// set the sprite's coords to the mouse's
			pSprite->x = event->stageX;
			pSprite->y = event->stageY;
			  
			return internal::_undefined;
		};
		
		// mouseDown handler changes the Sprite's opacity -- associated with main thread
		static var mouseDownHandler(void *arg, var as3Args)
		{
			// get references to the stage, the playStateSprite and the penSprite
			flash::display::Stage stage = internal::get_Stage();
			flash::display::Sprite playSprite = stage->getChildByName(internal::new_String("PLAY_STATE"));
		    flash::display::Sprite pSprite = playSprite->getChildByName(internal::new_String("pen"));
		    
			if (pSprite->alpha > 0.1)
			{
				pSprite->alpha = pSprite->alpha  * 0.9;
			} else
			{
				printf("Out of ink!\n**\n");
				printf("exit\nPLAY_STATE\n**\n");
				pSprite->alpha = 0;
				balloonCount = 0;
				// show the mouse cursor
				flash::ui::Mouse::show();
				var tempAS3int = internal::new_int(2);
				setCurrrentState((void*)&tempAS3int, internal::_null);
			}
			
		  return internal::_undefined;
		};
		
		static var makeBalloon (void *arg, var as3Args)
		{
			if (balloonCount < 31)
			{
				// get references to the stage and the playStateSprite
				flash::display::Stage stage = internal::get_Stage();
				flash::display::Sprite sgSprite = stage->getChildByName(internal::new_String("PLAY_STATE"));
				
				// set up the balloon sprite
				flash::display::Sprite balloonSprite = flash::display::Sprite::_new();
				flash::display::Graphics graphics = balloonSprite->graphics;
				
				// increment the balloonCount, convert the integer balloonCount to a string, countToChars, and name the balloon to be that string
				balloonCount++;
				char countToChars[4]; // string which will contain the number
				sprintf (countToChars, "%d", balloonCount); // %d makes the result be a decimal integer
				balloonSprite->name = internal::new_String(countToChars);
			
				//draw the balloon
				int indexer = rand() % 3;
				int colors[3] ={0xFF0000, 0x00FF00, 0xFF00FF};
				// circular balloon shape of colors[indexer]
				graphics->beginFill(colors[indexer], 0.5);
					graphics->drawCircle(0, 0, 50.0);
				graphics->endFill();
				// balloon strings
				graphics->beginFill(0x000000);
					graphics->moveTo(0, 40); 
					graphics->curveTo(10, 40, 20, 80);
					graphics->moveTo(20, 80); 
					graphics->curveTo(10, 60, 20, 150);
				graphics->endFill();
				
				// place the balloon in the playStateSprite (which is on the stage)
				int placeX = 120 + (rand() % 790);
				int placeY = 60 + (rand() % 380);
				sgSprite->addChild(balloonSprite);
				balloonSprite->x = placeX;
				balloonSprite->y = placeY;
				printf("B# (%d)\n**\n", balloonCount);
				
				// add an event listener to the balloon sprite to handle being popped
				balloonSprite->addEventListener(flash::events::MouseEvent::MOUSE_DOWN, Function::_new(balloonPopHandler, (void*)&internal::_null), false, 0, false);
			}
			if (balloonCount > 29 || balloonCount < 1)
			{
				printf("exit\nPLAY_STATE\n**\n");				
				balloonCount = 0;
				// show the mouse cursor
				flash::ui::Mouse::show();
				var tempAS3int = internal::new_int(2);
				setCurrrentState((void*)&tempAS3int, internal::_null);
			}
		};
		
		// balloonPopHandler 
		static var balloonPopHandler(void *arg, var as3Args)
		{
			// create a soundManager to play sounds
			soundManager *doSound;
			doSound = new soundManager();
			doSound->playPop();
			
			// get references to the stage and the playStateSprite
			flash::display::Stage stage = internal::get_Stage();
			flash::display::Sprite playSprite = stage->getChildByName(internal::new_String("PLAY_STATE"));
			
			// two temporary sprites we need
 		    flash::display::Sprite poppedBSprite = flash::display::Sprite::_new();
 		    flash::display::Sprite tempSprite = flash::display::Sprite::_new();
			
			// look at the MouseEvent to find which balloon triggered it; store it in poppedBalloonObj
		    flash::events::MouseEvent evnt = var(as3Args[0]);

			// cast the event object currentTarget, stored in poppedBalloonObj, to the sprite poppedBSprite
			poppedBSprite = (flash::display::Sprite)evnt->currentTarget;
			
			// get the name of poppedBSprite (this will be the balloon number)
	  		var poppedName = poppedBSprite->name;
	  		
	  		// then get the balloon that triggered the MOUSEDOWN by name from inside playSprite
	  		tempSprite = playSprite->getChildByName(poppedName);
	  		
	  		// remove the popped balloon from playSprite (this also removes it from the stage)
	  		playSprite->removeChildAt(playSprite->getChildIndex(tempSprite));
	  		
	  		// decrement the number of balloons on the stage, and increment the balloons the player has popped
	  		balloonCount--;
	  		numberOfBalloonsPopped++;
			printf("popped\n%s\n**\n", internal::utf8_toString(poppedName));

		  return internal::_undefined;
		};
		
		// current_state getter & setter
		static var getCurrrentState()
			{
				var j = internal::new_int(current_state);
				return j;
			};
		static var setCurrrentState(void *arg, var as3Args)
			{
				int k = internal::int_valueOf(*(var *)arg);
				current_state = k;
				return internal::_undefined;
			};
		
		~popGame(){
			printf("popGame\ndestructor\n**\n");
			};
};

#endif