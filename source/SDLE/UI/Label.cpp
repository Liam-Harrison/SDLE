#include "SDLE/UI/Label.h"
#include "SDLE/UI/UI.h"

Label::Label(TTF_Font* font, const std::string& content)
{
	this->font = font;
	this->content = content;

	auto* engine = UI::GetTextEngine();
	text = TTF_CreateText(engine, font, content.c_str(), content.length());
}

Label::~Label() 
{
	TTF_DestroyText(text);
}

Label::Label(const Label& other) 
{
	this->font = other.font;
	this->content = other.content;

	auto* engine = UI::GetTextEngine();
	text = TTF_CreateText(engine, font, content.c_str(), content.length());
}

Label& Label::operator=(const Label& other)
{
	if (this != &other)
	{
		this->font = other.font;
		this->content = other.content;

		auto* engine = UI::GetTextEngine();
		text = TTF_CreateText(engine, font, content.c_str(), content.length());
	}

    return *this;
}

Label::Label(Label&& other) noexcept
{
	font = other.font;
	text = other.text;
	content = std::move(other.content);
	
	other.font = nullptr;
	other.text = nullptr;
}

Label& Label::operator=(Label&& other) noexcept
{
	if (this != &other)
	{
		TTF_DestroyText(text);

		font = other.font;
		text = other.text;
		content = std::move(other.content);

		other.font = nullptr;
		other.text = nullptr;
	}

    return *this;
}

void Label::SetText(const std::string& content)
{
	this->content = content;
    TTF_SetTextString(text, content.c_str(), content.length());
}

void Label::Render(float x, float y)
{
    TTF_DrawRendererText(text, x, y);
}
