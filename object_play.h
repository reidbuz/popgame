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
#ifndef _OBJECT_PLAY_H_
#define _OBJECT_PLAY_H_

#include "AS3/AS3++.h" // using AS3 var wrapper class
#include "Flash++.h" // using AVM2 sync primitives

// use "ui" AS3 var wrappers which marshall var manipulations to the ui Worker
using namespace AS3::ui;

class object_play {
	public: 
		object_play()
		{
			flash::display::Sprite dummySprite = flash::display::Sprite::_new();
			makePlayState((void*)&dummySprite);
			
		};
				  
		static var makePlayState(void *arg)
		{
		  	// get reference to the stage and the dummySprite
			flash::display::Stage stage = internal::get_Stage();
		  	flash::display::Sprite statePlaySprite = *(var *)arg;
		  
			// drawing the statePlaySprite
			flash::display::Graphics graphics = statePlaySprite->graphics;
			graphics->lineStyle(3,0x00CCFF);
			graphics->beginFill(0xFFFFFF, 1.0);
			graphics->drawRoundRect(70,20,860,490,20);
			graphics->endFill();
			statePlaySprite->name = "PLAY_STATE"; 
			
			// prepping the text elements
			flash::text::TextField titleText = flash::text::TextField::_new();
			flash::text::TextFormat titleFormat = flash::text::TextFormat::_new();
			titleFormat->font = "Trebuchet MS";
			titleFormat->size = internal::new_uint(9);
			titleText->defaultTextFormat = titleFormat; 
			titleText->border = true; 
			titleText->wordWrap = true; 
			titleText->multiline = true; 
			titleText->x = 75; 
			titleText->y = 25; 
			titleText->height = 480; //stage->stageHeight-20
			titleText->width = 850; //stage->stageWidth-20
			titleText->text = ".\n.\n.";
			
			statePlaySprite->addChild(titleText);
			
			stage->addChild(statePlaySprite);

		  return internal::_undefined;
		};
		
		~object_play()
		{
			printf("object_menu\ndestructor\n**\n");
			//delete this
		}
		
//	private
};


#endif