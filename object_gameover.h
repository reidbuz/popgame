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
#ifndef _OBJECT_GAMEOVER_H_
#define _OBJECT_GAMEOVER_H_

#include "AS3/AS3++.h" // using AS3 var wrapper class
#include "Flash++.h" // using AVM2 sync primitives

// use "ui" AS3 var wrappers which marshall var manipulations to the ui Worker
using namespace AS3::ui;

class object_gameover {

	public: 
		object_gameover(int pb)
		{
			// need to get the number of balloons popped from popGame.h.  But how?
			var bsPopped = internal::new_int(pb);
			makeGameOverState((void*)&bsPopped);
			
		};
		
		static var makeGameOverState(void *arg)
		{
			// get reference to the stage and the dummySprite
			flash::display::Stage stage = internal::get_Stage();
		    flash::display::Sprite stateGameOverSprite = flash::display::Sprite::_new();
		    int balloonsPopped = internal::int_valueOf(*(var *)arg);
		  
			// drawing the stateGameOverSprite
			flash::display::Graphics graphics = stateGameOverSprite->graphics;
			graphics->lineStyle(3,0x00CCFF);
			graphics->beginFill(0x006699, 1.0);
			graphics->drawRoundRect(70,20,860,490,20);
			graphics->endFill();
			stateGameOverSprite->name = "GAMEOVER_STATE"; 
			
			// drawing the play again buttonSprite
		    flash::display::Sprite buttonSprite = flash::display::Sprite::_new();
		    buttonSprite->name = "buttonSprite";
			flash::display::Graphics btn_graphics = buttonSprite->graphics;
			btn_graphics->lineStyle(3,0x00FF00);
			btn_graphics->beginFill(0xFFCC33, 1.0);
			btn_graphics->drawRoundRect(400,300,200,50,20);
			btn_graphics->endFill();			
			flash::text::TextField btn_Text = flash::text::TextField::_new();
			flash::text::TextFormat btn_Format = flash::text::TextFormat::_new();
            btn_Format->font = "Georgia";
			btn_Format->size = internal::new_uint(30);
			btn_Text->defaultTextFormat = btn_Format; 
            btn_Text->border = true; 
            btn_Text->wordWrap = true; 
            btn_Text->multiline = true; 
            btn_Text->x = 410; 
            btn_Text->y = 305; 
            btn_Text->height = 40;
            btn_Text->width = 180;
            btn_Text->text = "  Play Again!";
            buttonSprite->addChild(btn_Text);
			buttonSprite->mouseChildren = false;
			buttonSprite->buttonMode = true;
			
			// prepping the text elements to show the score
			flash::text::TextField titleText = flash::text::TextField::_new();
			flash::text::TextFormat titleFormat = flash::text::TextFormat::_new();
			titleFormat->font = "Georgia";
			titleFormat->size = internal::new_uint(40);
			var colorProp = internal::new_String("color");
			var colorVal = internal::new_uint(0xFFCC66);
			internal::setproperty(titleFormat, colorProp, colorVal);
			titleText->defaultTextFormat = titleFormat; 
			titleText->border = true; 
			titleText->wordWrap = true; 
			titleText->multiline = true; 
			titleText->x = 75; 
			titleText->y = 25; 
			titleText->height = 480; //stage->stageHeight-20
			titleText->width = 850; //stage->stageWidth-20
			std::string message = "\n  Pop!\n\n                                 Game Over!\n                                    Score: ";
			char countToChars[4]; // string which will contain the number
			sprintf (countToChars, "%d", balloonsPopped); // %d makes the result be a decimal integer
			std::string messageScore = message + countToChars;
			const char* messageScoreChars = messageScore.c_str();
			titleText->text = internal::utf8_toString(internal::new_String(messageScoreChars));
			titleText->selectable = false;
			
			// add score message to the gameOver Sprite, then add the buttonSprite
			stateGameOverSprite->addChild(titleText);
			stateGameOverSprite->addChild(buttonSprite);

			stage->addChild(stateGameOverSprite);
			
		  	return internal::_undefined;
		  };
		
		~object_gameover()
		{
			printf("object_menu\ndestructor\n**\n");
			//delete this
		}
		
//	private
};


#endif