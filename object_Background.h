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
#ifndef _OBJECT_BACKGROUND_H_
#define _OBJECT_BACKGROUND_H_

#include "AS3/AS3++.h" // using AS3 var wrapper class
#include "Flash++.h" // using AVM2 sync primitives

// use "ui" AS3 var wrappers which marshall var manipulations to the ui Worker
using namespace AS3::ui;

class object_Background {
	public: 
		object_Background()
		{
			update();
		};
		
		void update();
		
		static var moveBackground(void *arg, var as3Args)
		{
			flash::display::Stage stage = internal::get_Stage();
			flash::display::Sprite background = stage->getChildByName(internal::new_String("background"));
			
			if (background->x == 50) 
			{
				// hide the column for printf output
				background->x = 0;
			} else
			{
				//show the column for printf output
				background->x = 50; 
			}
		};
		
		~object_Background()
		{
			printf("background\ndestructor\n**\n");
			//delete this
		}
		
//	private
};

void object_Background::update()
{	
	// Setup the stage
		flash::display::Stage stage = internal::get_Stage();
		stage->scaleMode = flash::display::StageScaleMode::NO_SCALE;
		stage->align = flash::display::StageAlign::TOP_LEFT;
		stage->frameRate = 24;
		
		// drawing the backgroundObject sprite
		flash::display::Sprite backgroundObject = flash::display::Sprite::_new();
		backgroundObject->name = "background";
  		flash::display::Graphics graphics = backgroundObject->graphics;
  		graphics->beginFill(0x28AFFF, 1.0);
			graphics->drawRect(0,0,960,540);
		graphics->endFill();
		
		// drawing the move
		flash::display::Sprite buttonSprite = flash::display::Sprite::_new();
		buttonSprite->name = "moveBtnSprite";
		flash::display::Graphics btn_graphics = buttonSprite->graphics;
		btn_graphics->lineStyle(3,0x000000);
			btn_graphics->beginFill(0x990000, 1.0);
			btn_graphics->drawCircle(21,525,20);
		btn_graphics->endFill();			
		flash::text::TextField btn_Text = flash::text::TextField::_new();
		flash::text::TextFormat btn_Format = flash::text::TextFormat::_new();
		btn_Format->font = "Arial Unicode MS";
		btn_Format->size = internal::new_uint(20);
		btn_Text->defaultTextFormat = btn_Format; 
		btn_Text->border = false;
		btn_Text->x = 0; 
		btn_Text->y = 510;
		btn_Text->height = 40;
		btn_Text->width = 80;
		btn_Text->text = "☚☛";
		buttonSprite->addChild(btn_Text);
		buttonSprite->mouseChildren = false;
		buttonSprite->buttonMode = true;
		
		backgroundObject->addChild(buttonSprite);
		stage->addChild(backgroundObject);
		backgroundObject->x = 0;
		backgroundObject->y = 0;
		
		buttonSprite->addEventListener(flash::events::MouseEvent::MOUSE_DOWN, Function::_new(moveBackground, NULL), false, 0, false);
};

#endif