#include "column.h"
#include "beatmap.h"
#include "../sprite.h"
#include "../control_handler.h"
#include "../system.h"
#include "../timer.h"

void lm::Column::init(int column_index)
{
	m_h = 45;
	current_note_index = 0;
	is_pressing_ln = false;
	s_note = new Sprite;
	int current_w, current_h;
	switch (column_index)
	{
		case 0:
			current_w = 173.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			start_x = 333;
			start_y = 387;
			end_x = 36;
			end_y = 1088;
			m_x = 0;
			m_w = 166;
			start_scale = 0.0828f;
			s_note->init("assets/game/note_1.png", 0, 0, current_w, current_h);
		break;
		case 1:
			start_x = 347;
			start_y = 387;
			end_x = 197;
			end_y = 1088;
			m_x = 166;
			m_w = 194;
			start_scale = 0.0828f;
			current_w = 165.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			s_note->init("assets/game/note_2.png", 0, 0, current_w, current_h);
		break;
		case 2:
			start_x = 360;
			start_y = 387;
			end_x = 360;
			end_y = 1088;
			m_x = 166;
			m_w = 194;
			start_scale = 0.0828f;
			current_w = 165.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			s_note->init("assets/game/note_3.png", 0, 0, current_w, current_h);
		break;
		case 3:
			start_x = 371;
			start_y = 387;
			end_x = 517;
			end_y = 1088;
			m_x = 517;
			m_w = 166;
			start_scale = 0.0828f;
			current_w = 173.0f / 720.0f * System::instance()->GetWindowWidth();
			current_h = 45.0f / 1280.0f * System::instance()->GetWindowHeigh();
			s_note->init("assets/game/note_4.png", 0, 0, current_w, current_h);
		break;
	}

	is_tapped = false;
}	//void lm::Column::init(int column_index)

void lm::Column::clear()
{
	m_note.clear();
}

void lm::Column::update()
{
	is_released = false;
	is_tapped = false;
	if (!is_hold)
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.x >= m_x && load_finger.x <= (m_x + m_w))
			{
				if (!load_finger.moved)
				{
					is_hold = true;
					is_tapped = true;
					has_pressed_id = load_finger.id;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.id == has_pressed_id)
			{
				if (load_finger.x < m_x || load_finger.x > (m_x + m_w))
				{
					is_hold = false;
				}
				if (load_finger.released)
				{
					is_released = true;
					is_hold = false;
				}
			}
		}
	}

	if (current_note_index < m_note.size())
	{
		if (is_tapped)
		{
			Judgement current_judgement = Beatmap::instance()->judge(m_note[current_note_index]->time);
			if (current_judgement != JUDGEMENT_ER && current_judgement != JUDGEMENT_NONE)
			{
				if (m_note[current_note_index]->time != m_note[current_note_index]->time_end)
				{
					is_pressing_ln = true;
					//有長條的情況下先不跳過
				}
				else
				{
					current_note_index++;
					//正常note可跳過
				}
			}
			else if (current_judgement == JUDGEMENT_ER)
			{
				current_note_index++;
				//誤觸就ERROR的話，別說了，下一個note，請
			}
		}

		if (is_released)
		{
			is_pressing_ln = false;
			if (is_pressing_ln && m_note[current_note_index]->time != m_note[current_note_index]->time_end)
			{
				Beatmap::instance()->judge(m_note[current_note_index]->time_end, true, true);
				current_note_index++;
			}
		}

		if (!is_pressing_ln)
		//以防檢測開頭而導致意外ERROR
		{
			if (Beatmap::instance()->judge(m_note[current_note_index]->time, false) == JUDGEMENT_ER)
			{
				current_note_index++;
			}
		}
	}
}

void lm::Column::render()
{
	int i = current_note_index;
	bool is_note_in_screen = i < m_note.size();

	while (is_note_in_screen)
	{
		is_note_in_screen = DrawNote(m_note[i]->time, m_note[i]->time_end);
		is_note_in_screen = is_note_in_screen && i < m_note.size();
		i++;
	}
}

void lm::Column::AddNote(Note *load_note)
{
	m_note.push_back(load_note);
}

bool lm::Column::DrawNote(int time, int time_end)
{
	int time_diff = time - Timer::instance()->GetTime("game");
	float process = float(Beatmap::instance()->GetDuration() - time_diff) / float(Beatmap::instance()->GetDuration());
	process *= process;
	//note時間與當前時間的時間差
	int current_x = start_x + (end_x - start_x) * process;
	int current_y = start_y + (end_y - start_y) * process;
	float current_scale = start_scale + (1.0f - start_scale) * process;
	//時間差轉換成Y坐標
	//屏幕頂端到判定線距離 * 屏幕頂端到判定線所用時間與note出現時間之比 - 偏移

	if (time_diff > Beatmap::instance()->GetDuration())
	{
		//檢測當前note是否在屏幕外面
		return false;
	}

	if (time != time_end)
	//是否長條
	{
		int time_diff_end = time_end - Timer::instance()->GetTime("game");
		//長條尾時間與當前時間的時間差
		float process_end = float(Beatmap::instance()->GetDuration() - time_diff_end) / float(Beatmap::instance()->GetDuration());
		process_end *= process_end;
		//時間差轉換成Y坐標 * 2
		if (is_pressing_ln)
		{
			process = 1;
		}
		float ln_piece_process = process;
		//長條身Y坐標
		while (ln_piece_process > 0 && ln_piece_process > process_end)
		//長條身不超過屏幕 且 不超過尾部 時畫出來，循環
		{
			int current_x_piece = start_x + (end_x - start_x) * ln_piece_process;
			int current_y_piece = start_y + (end_y - start_y) * ln_piece_process;
			float current_scale_piece = start_scale + (1.0f - start_scale) * ln_piece_process;

			s_note->SetPos(current_x_piece, current_y_piece);
			s_note->SetScale(current_scale_piece);
			s_note->render();
			//將長條身往上挪動
			ln_piece_process -= 0.05f;
		}

		if (ln_piece_process < 0)
		{
			if (current_y < System::instance()->GetWindowHeigh())
			{
				s_note->SetPos(current_x, current_y);
				s_note->SetScale(current_scale);
				s_note->render();
			}
			return false;
		}
		else
		{
			int current_x_end = start_x + (end_x - start_x) * process_end;
			int current_y_end = start_y + (end_y - start_y) * process_end;
			float current_scale_end = start_scale + (1.0f - start_scale) * process_end;
			s_note->SetPos(current_x_end, current_y_end);
			s_note->SetScale(current_scale_end);
			s_note->render();
		}
		//畫長條尾
	}

	if (current_y < System::instance()->GetWindowHeigh())
	{
		s_note->SetPos(current_x, current_y);
		s_note->SetScale(current_scale);
		s_note->render();
	}
	return true;
	//畫note
}