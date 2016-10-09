/*
** Pop!
** by John Reid Perkins-Buzo
**  Copyright (C) 2013
** 
** Pop! is a game that would be about popping as
** many balloons as you can in a certain time
** interval.  This is the sound manager for the game

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
#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "AS3/AS3++.h" // using AS3 var wrapper class
#include "Flash++.h" // using AVM2 sync primitives

// use "ui" AS3 var wrappers which marshall var manipulations to the ui Worker
using namespace AS3::ui;

static bool soundLoaded = false;

class soundManager {
	private:
		flash::media::Sound soundFactory;
	public:
			
		soundManager()
			{
				soundFactory = flash::media::Sound::_new();
				String url = "assets/pop.mp3";
				flash::net::URLRequest request = flash::net::URLRequest::_new(url);
				
				soundFactory->addEventListener(flash::events::Event::COMPLETE, Function::_new(soundEventHandler, NULL));
				soundFactory->addEventListener(flash::events::Event::ID3, Function::_new(soundEventHandler, NULL));
				soundFactory->addEventListener(flash::events::IOErrorEvent::IO_ERROR, Function::_new(soundEventHandler, NULL));
				soundFactory->addEventListener(flash::events::ProgressEvent::PROGRESS, Function::_new(soundEventHandler, NULL));
				soundFactory->load(request);
				
				if (soundLoaded) 
				{
					soundPlayer((void *)&soundFactory);
				}
			};
			
		static var soundPlayer(void *arg)
			{
				flash::media::Sound soundToPlay = flash::media::Sound(*(var *)arg);
				flash::media::SoundChannel song = flash::media::SoundChannel::_new();
				song = soundToPlay->play();
				
				return internal::_undefined;
			};
			
		static var soundEventHandler(void *arg, var as3Args)
			{
				flash::events::Event event = (flash::events::Event) as3Args[0];
				var type = event->type;
				
				if (strcmp(internal::utf8_toString(type), "COMPLETE"))
				{
					//printf("Event [%s]\n", internal::utf8_toString(type));
					soundLoaded = true;
				}
				
				return internal::_undefined;
			};
			
		void playPop()
			{	
				if (soundLoaded) 
				{
					soundPlayer((void *)&soundFactory);
				}
			};
		
		~soundManager()
			{
				printf("soundManager\ndestructor\n**\n");
				//delete this
			}

};
		

#endif