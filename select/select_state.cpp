#include "select_state.h"
#include "../button.h"
#include "../sprite.h"
#include "../debug_widget.h"
#include "../texture_manager.h"
#include "../main/main_state.h"
#include "../loading/loading_state.h"
#include "song_list.h"

lm::SelectState *lm::SelectState::m_instance = 0;

void lm::SelectState::init()
{
	TextureManager::instance()->loadfont("assets/GeosansLight.ttf", 30);
	SongList::instance()->init();
	select_back = new Button;
	background = new Sprite;
	select_back->load("assets/sortbutton.png", "assets/sortbutton_pressed.png", 0, 660, 180, 81);
	background->load("assets/bg_light.png", 0, 0, 1286, 965);
	select_back->SetText("Back", "assets/GeosansLight.ttf", 0x00, 0x00, 0x00);
}

void lm::SelectState::clear()
{
	SongList::instance()->clear();
	select_back->clear();
	background->clear();
}

void lm::SelectState::update()
{
	SongList::instance()->update();
	select_back->update();

	background->render();
	select_back->render();
	SongList::instance()->render();
	if (select_back->IsReleased())
	{
		LoadingState::instance()->init(MainState::instance(), this);
		DebugWidget::instance()->PushLog("back to main menu from select_state");
	}
}