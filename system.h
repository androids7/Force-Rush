#ifndef LUNATIC_MELODY_SYSTEM_H
#define LUNATIC_MELODY_SYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>

namespace lm
{
	class State;

	class System
	{
		public:
			static System *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new System;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void run();
			void clear();
			void PushState(std::string id, State *s);
			void SwitchState(std::string id);
			int GetWindowWidth();
			int GetWindowHeigh();
			int GetScreenWidth();
			int GetScreenHeigh();

			State *current_state;
		private:
			System() {}
			~System() {}
			static System *m_instance;
			SDL_Window *system_window;
			SDL_Renderer *system_renderer;
			int window_width;
			int window_heigh;
			int screen_width;
			int screen_heigh;

//			std::map<std::string, State*> m_state;
			bool m_loading;
	};	//class System
};	//namespace lm

#endif	//LUNATIC_MELODY_SYSTEM_H