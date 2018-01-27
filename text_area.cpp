#include "text_area.h"
#include <SDL2/SDL.h>
#include "texture_manager.h"

void fr::TextArea::init(std::string text, int x, int y, std::string font_path, int font_size, char r, char g, char b, TextFormat format, int limited_w)
{
	m_text = text;
	m_x = x;
	m_y = y;
	m_font_path = font_path;
	m_font_size = font_size;
	m_r = r;
	m_g = g;
	m_b = b;
	m_format = format;
	m_limited_w = limited_w;
	m_scale = 1;
}

void fr::TextArea::render()
{
	if (m_text.length() > 0)
	{
		TextureManager::instance()->render(m_text, m_x, m_y, m_font_path, m_font_size, m_r, m_g, m_b, m_format, m_limited_w, m_scale);
	}
}

void fr::TextArea::render(int x, int y)
{
	if (m_text.length() > 0)
	{
		TextureManager::instance()->render(m_text, x, y, m_font_path, m_font_size, m_r, m_g, m_b, m_format, m_limited_w, m_scale);
	}
}

void fr::TextArea::clear()
{
	
}

void fr::TextArea::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
}

void fr::TextArea::SetText(std::string text)
{
	m_text = text;
}

void fr::TextArea::SetColor(char r, char g, char b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

void fr::TextArea::SetFont(std::string font_path, int font_size)
{
	m_font_path = font_path;
	m_font_size = font_size;
}

void fr::TextArea::SetScale(float scale)
{
	m_scale = scale;
}

int fr::TextArea::GetX()
{
	return m_x;
}

int fr::TextArea::GetY()
{
	return m_y;
}

std::string fr::TextArea::GetText()
{
	return m_text;
}

float fr::TextArea::GetScale()
{
	return m_scale;
}