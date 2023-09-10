#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>
#include <vector>

class Button : public sf::Drawable
{
private:
	sf::RectangleShape m_button;
	sf::Text m_text;

	int m_btnWidth = 0;
	int m_btnHeight = 0;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_button);
		target.draw(m_text);
	}

public:
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Font &font,
		sf::Color bgColor = sf::Color::White, sf::Color textColor = sf::Color::Black);
	Button(sf::Vector2f buttonSize, sf::Color bgColor = sf::Color::White,
		sf::Color outlineColor = sf::Color::Black);
	void setFont(sf::Font& font) { m_text.setFont(font); }
	void setBackColor(sf::Color color) { m_button.setFillColor(color); }
	void setTextColor(sf::Color color) { m_text.setFillColor(color); }
	void setPosition(sf::Vector2f point);

	void drawTo(sf::RenderWindow& window) 
	{
		window.draw(m_button);
		//If m_text initialized by font
		if(m_text.getFont() != nullptr)
			window.draw(m_text);
	}
	// Check if the mouse is within the bounds of the button:
	bool isMouseOver(sf::RenderWindow& window);
};

class TextBox
{
private: 
	sf::RectangleShape *m_textRect = nullptr;
	sf::Text *m_text;
public:
	TextBox(const std::string& text, const sf::Font& font, int charSize, const sf::Color& charColor,
		const sf::Vector2f &pos = sf::Vector2f(0, 0), const sf::Vector2f &size = sf::Vector2f(0, 0));
	void setText(const std::string& text) { m_text->setString(text); }
	void drawTo(sf::RenderWindow& window)
	{
		window.draw(*m_textRect);
		window.draw(*m_text);
	}
};

class YahtzeeTable
{
private:
	std::vector<Button*> m_combos;
	TextBox* m_totalScoreTextBox = nullptr;
	int m_totalScore = 0;

public:
	YahtzeeTable(int charSize, sf::Font& font,
	sf::Color bgColor = sf::Color::White, sf::Color textColor = sf::Color::Black);
	~YahtzeeTable();
	Button* whichButtonMouseOver(sf::RenderWindow& window);
	void updateScore(sf::RenderWindow& window);
	void drawTo(sf::RenderWindow& window);
};

class TableMap : public sf::Drawable
{
private:
	int m_offsetX = 16;
	int m_offsetY = 12;
	sf::VertexArray m_vertices;
	sf::Texture m_diceSet;
	std::vector<int>* m_map = nullptr;
	unsigned m_width = 0;
	unsigned m_height = 0;
	sf::Vector2u m_startPos;
	sf::Vector2u m_diceSize;
	std::vector<Button*> m_diceButtonSet;
private:

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
	TableMap(const std::string& texture, const sf::Vector2u& diceSize,
		std::vector<int>* map, const unsigned width, const unsigned height,
		const sf::Vector2u& startPos = sf::Vector2u(0,0));
	void drawDices();
	void setPlaces(std::vector<int>& dicePlaces, std::vector<int> &diceTops);
	Button* whichButtonMouseOver(sf::RenderWindow& window);
};

#endif // !GRAPHICS_H
