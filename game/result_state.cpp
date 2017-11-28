#include "result_state.h"
#include "beatmap.h"
#include "../button.h"
#include "../sprite.h"
#include "../system.h"
#include "../texture_manager.h"
#include "../select/select_state.h"
#include "../select/song_list.h"
#include "../loading/loading_state.h"
#include "../user/character.h"

lm::ResultState *lm::ResultState::m_instance = 0;

void lm::ResultState::init()
{
	song_base = new Sprite;
	score_base = new Sprite;
	rank_base = new Sprite;
	performance_base = new Sprite;
	user_base = new Sprite;
	b_return = new Button;
	b_retry = new Button;
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 32);
	TextureManager::instance()->loadfont("assets/fonts/Ubuntu-R.ttf", 24);
	song_base->init("assets/result/song_base.png", 0, System::instance()->GetWindowHeigh() - 720);
	score_base->init("assets/result/score_base.png", 0, System::instance()->GetWindowHeigh() - 544);
	rank_base->init("assets/result/rank_base.png", System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 720);
	performance_base->init("assets/result/performance_base.png", System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 488);
	user_base->init("assets/result/user_base.png", System::instance()->GetWindowWidth() - 232,  System::instance()->GetWindowHeigh() - 318);
	b_return->init("assets/base/sort_button.png", 0, System::instance()->GetWindowHeigh() - 80, 176, 80);
	b_return->AddPressedFrame( "assets/base/sort_button_pressed.png");
	b_return->AddText("Return", b_return->GetW() / 2, b_return->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
	b_retry->init("assets/base/sort_button.png", System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80, 176, 80);
	b_retry->AddPressedFrame( "assets/base/sort_button_pressed.png");
	b_retry->AddText("Retry", b_retry->GetW() / 2, b_retry->GetH() / 2, "assets/fonts/Audiowide.ttf", 36, 0x00, 0x00, 0x00);
}

void lm::ResultState::clear()
{
	
}

void lm::ResultState::update()
{
	b_return->update();
	b_retry->update();

	Character::instance()->render();
	song_base->render();
	score_base->render();
	rank_base->render();
	performance_base->render();
	user_base->render();
	b_return->render();
	b_retry->render();

	char *difficulty_ch = new char[10];
	char *pure_ch = new char[10];
	char *great_ch = new char[10];
	char *good_ch = new char[10];
	char *error_ch = new char[10];
	char *score_ch = new char[10];
	sprintf(difficulty_ch, "Lv.%d", m_information->difficulty);
	sprintf(pure_ch, "PURE  %d", m_score->pure);
	sprintf(great_ch, "GREAT %d", m_score->great);
	sprintf(good_ch, "GOOD  %d", m_score->error);
	sprintf(error_ch, "ERROR %d", m_score->error);
	sprintf(score_ch, "SCORE  %d", m_score->score);
	if (System::instance()->IsWindowModified())
	{
		song_base->SetPos(0, System::instance()->GetWindowHeigh() - 720);
		score_base->SetPos(0, System::instance()->GetWindowHeigh() - 544);
		rank_base->SetPos(System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 720);
		performance_base->SetPos(System::instance()->GetWindowWidth() - 232, System::instance()->GetWindowHeigh() - 488);
		user_base->SetPos(System::instance()->GetWindowWidth() - 232,  System::instance()->GetWindowHeigh() - 318);
		b_return->SetPos(0, System::instance()->GetWindowHeigh() - 80);
		b_retry->SetPos(System::instance()->GetWindowWidth() - 176, System::instance()->GetWindowHeigh() - 80);
	}

	TextureManager::instance()->render(m_information->title, 32, System::instance()->GetWindowHeigh() - 696, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 416);
	TextureManager::instance()->render(m_information->artist, 32, System::instance()->GetWindowHeigh() - 648, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(m_information->version, 96, System::instance()->GetWindowHeigh() - 608, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(difficulty_ch, 32, System::instance()->GetWindowHeigh() - 608, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 56);
	TextureManager::instance()->render(score_ch, 32, System::instance()->GetWindowHeigh() - 520, "assets/fonts/Ubuntu-R.ttf", 32, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(pure_ch, 32, System::instance()->GetWindowHeigh() - 472, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(great_ch, 32, System::instance()->GetWindowHeigh() - 432, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(good_ch, 32, System::instance()->GetWindowHeigh() - 392, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);
	TextureManager::instance()->render(error_ch, 32, System::instance()->GetWindowHeigh() - 352, "assets/fonts/Ubuntu-R.ttf", 24, 0x00, 0x00, 0x00, TEXTFORMAT_LEFT, 184);

	if (b_return->IsReleased())
	{
		LoadingState::instance()->init(SelectState::instance(), this);
	}

	delete [] difficulty_ch;
	delete [] pure_ch;
	delete [] great_ch;
	delete [] good_ch;
	delete [] error_ch;
	delete [] score_ch;
}

void lm::ResultState::LoadScore(lm::SongInformation *load_information, lm::Score *load_score)
{
	m_information = load_information;
	m_score = load_score;
}