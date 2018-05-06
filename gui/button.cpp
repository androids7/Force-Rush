#include "button.h"
#include "../control_handler.h"
#include "../texture_manager.h"
#include "../loading/loading_state.h"

void fr::Button::init(std::string path, Rect load_dest_rect, Rect load_source_rect)
{
	Sprite::init(path, load_dest_rect, load_source_rect);
	is_pressed = false;
}

void fr::Button::update()
{
	is_released = false;
	is_outside_pressed = false;
	int x, y;
/*
	ControlHandler::instance()->GetMousePos(x, y);
	if (x >= m_x && y >= m_y && x <= (m_x + m_w) && y <= (m_y + m_h))
	{
		if (ControlHandler::instance()->IsMouseButtonDown(MOUSEBUTTON_LEFT))
		{
			is_pressed = true;
		}
		else
		{
			if (is_pressed)
			{
				is_released = true;
			}
			is_pressed = false;
		}
	}
	else
	{
		is_pressed = false;
	}
*/
	if (!is_pressed)
	{
		for (int i = 0; i < ControlHandler::instance()->GetFingerCount(); i++)
		{
			Finger load_finger = ControlHandler::instance()->GetFinger(i);
			if (load_finger.x >= dest_rect.x && load_finger.y >= dest_rect.y && load_finger.x <= (dest_rect.x + dest_rect.w) && load_finger.y <= (dest_rect.y + dest_rect.h))
			{
				if (!load_finger.moved)
				//從外側劃進來的是不會被檢測到的。
				{
					is_pressed = true;
					has_pressed_id = load_finger.id;
					//防止干擾記錄下標
				}
			}
			else
			{
				is_outside_pressed = true;
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
//				if (load_finger.x < dest_rect.x || load_finger.y < dest_rect.y || load_finger.x > (dest_rect.x + dest_rect.w) || load_finger.y > (dest_rect.y + dest_rect.h))
				if (load_finger.moved)
				{
					is_pressed = false;
				}
				if (load_finger.released)
				{
					is_released = true;
					is_pressed = false;
				}
			}
		}
	}
	Sprite::update();
	if (is_pressed)
	{
		current_index = pressed_index;
	}
	else
	{
		current_index = base_index;
	}
}	//void fr::Button::update()

void fr::Button::render()
{
	Sprite::render();
	for (int i = 0; i < text.size(); i++)
	{
		int x, y;
		x = source_rect.x > text[i]->GetX() ? dest_rect.x : text[i]->GetX() + dest_rect.x - source_rect.x;
		y = source_rect.y > text[i]->GetY() ? dest_rect.y : text[i]->GetY() + dest_rect.y - source_rect.y;
		text[i]->render(x, y, TEXTFORMAT_LEFT);
	}
}

void fr::Button::clear()
{
	
}

void fr::Button::SetRotation(Point load_center, double load_angle)
{
	Sprite::SetRotation(load_center, load_angle);
}

void fr::Button::AddPressedFrame(std::string path)
{
	Sprite::AddFrame(path);
	pressed_index = frame.size() - 1;
}

bool fr::Button::IsPressed()
{
	return is_pressed && !LoadingState::instance()->IsSwitching();
}

bool fr::Button::IsOutsidePressed()
{
	return is_outside_pressed && !LoadingState::instance()->IsSwitching();
}

bool fr::Button::IsReleased()
{
	return is_released && !LoadingState::instance()->IsSwitching();
}

void fr::Button::AddText(std::string load_text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w)
{
	TextArea *new_text = new TextArea;
	new_text->init(load_text, x, y, font_path, font_size, r, g, b, format, limited_w);
	text.push_back(new_text);
}

void fr::Button::AddText(TextArea *load_text)
{
	text.push_back(load_text);
}

void fr::Button::ClearText()
{
	text.clear();
}

fr::TextArea *fr::Button::GetText(int index)
{
	return text[index];
}

void fr::Button::SetSize(int w, int h)
{
	Sprite::SetSize(w, h);
}

void fr::Button::SetSrcRect(fr::Rect load_source_rect)
{
	Sprite::SetSrcRect(load_source_rect);
	for (int i = 0; i < text.size(); i++)
	{
		Rect text_source_rect = Rect(0, 0, 0, 0);
		text_source_rect.x = source_rect.x < text[i]->GetX() ? 0 : source_rect.x - text[i]->GetX();
		text_source_rect.y = source_rect.y < text[i]->GetY() ? 0 : source_rect.y - text[i]->GetY();
		text_source_rect.w = (source_rect.x + source_rect.w > text[i]->GetX() + text[i]->GetTextureW()) ? text[i]->GetTextureW() - text_source_rect.x : text[i]->GetX() + text[i]->GetTextureW() - source_rect.x - source_rect.w - text_source_rect.x;
//		text_source_rect.h = (source_rect.y + source_rect.h > text[i]->GetY() + text[i]->GetTextureH()) ? text[i]->GetTextureH() - text_source_rect.y : -(text[i]->GetY()) - text[i]->GetTextureH() + source_rect.y + source_rect.h - text_source_rect.y;
		text_source_rect.h = text_source_rect.y < text[i]->GetTextureH() ? text[i]->GetTextureH() - text_source_rect.y : 0;
//		text_source_rect.h + text_source_rect.y == source_rect.y + source_rect.h > text[i]->GetY() + text->h ? text->h : source_rect.y + source_rect.h - text[i]->GetY() - text->h
		text[i]->SetSrcRect(text_source_rect);
		text[i]->SetSize(text_source_rect.w, text_source_rect.h);
	}
}