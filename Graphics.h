#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>
#include <vector>
#include <math.h>
#include <string>
#include <functional>

#include "AuxiliaryFunctions.h"

class Button;
class TextBox;

void updatePlayerTextBox(TextBox& textBox, char currentPlayerNumber);

enum AnimationType
{
	Appearance,
	Disappearance,
	MAX_ANIMATIONS
};


void AnimateTextBox(TextBox& textBox, int duration, int animationType, sf::RenderWindow& window);
void AnimationOfCurrentPlayer(TextBox& textBox, int duration, sf::RenderWindow& window);

class Button : public sf::Drawable
{
private:
	sf::RectangleShape m_rect;
	sf::Text m_text;
	bool m_isOn = true;
	sf::Color m_disabledColor = sf::Color::Cyan;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_rect);
		target.draw(m_text);
	}

public:
	Button(const std::string &text, int charSize, const sf::Font &font,
		const sf::Vector2f& size = {0, 0},
		const sf::Vector2f& pos = sf::Vector2f(0, 0), 
		const sf::Color &fillColor = sf::Color::White, 
		const sf::Color &textColor = sf::Color::Black);
	Button(const sf::Vector2f & size, 
		const sf::Color &fillColor = sf::Color::White,
		const sf::Color &outlineColor = sf::Color::Black);
	~Button(){}
	//text
	void setText(const std::string& text);
	void setFont(sf::Font& font) { m_text.setFont(font); }
	void setCharSize(int size) { m_text.setCharacterSize(size); }
	void setTextColor(const sf::Color& color) { m_text.setFillColor(color); }
	void setTextOutlineColor(const sf::Color& color) { m_text.setOutlineColor(color); }
	void setTextOutlineThickness(int thickness) { m_text.setOutlineThickness(thickness); }

	int getCharSize() const { return m_text.getCharacterSize(); }
	std::string getText() const { return m_text.getString(); }
	const sf::Font* getFont() const { return m_text.getFont(); }
	const sf::Vector2f& getTextPosition() const { return m_text.getPosition(); }
	const sf::Color& getTextColor() const { return m_text.getFillColor(); }
	const sf::Color& getTextOutlineColor() const { return m_text.getOutlineColor(); }
	//rect
	void setFillColor(const sf::Color& color) { m_rect.setFillColor(color); }
	void setRectOutlineColor(const sf::Color& color) { m_rect.setOutlineColor(color); }
	void setRectOutlineThickness(int thickness) { m_rect.setOutlineThickness(thickness); }
	void setSize(const sf::Vector2f& size) { m_rect.setSize(size); setPosition(m_rect.getPosition()); }

	const sf::Color& getFillColor() const { return m_rect.getFillColor(); }
	const sf::Color& getRectOutlineColor() const { return m_rect.getOutlineColor(); }
	const sf::Color& getDisabledColor() const { return m_disabledColor; }
	const sf::Vector2f &getRectSize() const { return m_rect.getSize(); }
	const sf::Vector2f &getRectPosition() const { return m_rect.getPosition(); }

	void setDisabledColor(const sf::Color& color) { m_disabledColor = color; }
	void setPosition(const sf::Vector2f& point);

	// Check if the mouse is within the bounds of the button:
	bool isMouseOver(sf::RenderWindow& window) const;
	void offButton() { setFillColor(m_disabledColor);  m_isOn = false; }
	void onButton() { setFillColor(sf::Color::White); m_isOn = true; }
	bool isOn() const { return m_isOn; }
};

class DiceButton : public Button
{
private:
	int m_diceNumber = 0;
	int m_diceMapPlace = 0;
	int m_diceTop = 0;
	bool m_isKeeped = false;
public:
	DiceButton(const sf::Vector2f& buttonSize, int diceNumber,
		const sf::Color& bgColor = sf::Color::White,
		const sf::Color& outlineColor = sf::Color::Black);
	int getDiceNumber() const { return m_diceNumber; }
	int getDiceTop() const { return m_diceTop; }
	int getDiceMapPlace() const { return m_diceMapPlace; }
	int isKeeped() const { return m_isKeeped; }
	void setDiceTop(int top) { m_diceTop = top; }
	void setDiceMapPlace(int place) { m_diceMapPlace = place; }
	void keepSwitch() { m_isKeeped = m_isKeeped ? false : true; }
};

class TextBox : public sf::Drawable
{
private: 
	sf::RectangleShape m_rect;
	sf::Text m_text;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_rect);
		target.draw(m_text);
	}
public:
	TextBox(const std::string& text, const sf::Font& font, int charSize,
		const sf::Vector2f &pos = sf::Vector2f(0, 0),
		const sf::Vector2f &size = sf::Vector2f(0, 0),
		const sf::Color& fillColor = sf::Color::White,
		const sf::Color& textColor = sf::Color::Black);
	virtual ~TextBox(){}

	void setPosition(const sf::Vector2f& pos);
	void setAlpha(uint8_t alpha);

	//text
	void setText(const std::string& text);
	void setFont(sf::Font& font) { m_text.setFont(font); }
	void setCharSize(int size) { m_text.setCharacterSize(size); }
	void setTextColor(const sf::Color& color) { m_text.setFillColor(color); }
	void setTextOutlineColor(const sf::Color& color) { m_text.setOutlineColor(color); }
	void setTextOutlineThickness(int thickness) { m_text.setOutlineThickness(thickness); }

	int getCharSize() const { return m_text.getCharacterSize(); }
	std::string getText() const { return m_text.getString(); }
	const sf::Font* getFont() const { return m_text.getFont(); }
	const sf::Vector2f& getTextPosition() const { return m_text.getPosition(); }
	const sf::Color& getTextColor() const { return m_text.getFillColor(); }
	const sf::Color& getTextOutlineColor() const { return m_text.getOutlineColor(); }
	//rect
	void setFillColor(const sf::Color& color) { m_rect.setFillColor(color); }
	void setRectOutlineColor(const sf::Color& color) { m_rect.setOutlineColor(color); }
	void setRectOutlineThickness(int thickness) { m_rect.setOutlineThickness(thickness); }
	void setSize(const sf::Vector2f& size) { m_rect.setSize(size); setPosition(m_rect.getPosition());}

	const sf::Color& getFillColor() const { return m_rect.getFillColor(); }
	const sf::Color& getRectOutlineColor() const { return m_rect.getOutlineColor(); }
	const sf::Vector2f& getRectSize() const { return m_rect.getSize(); }
	const sf::Vector2f& getRectPosition() const { return m_rect.getPosition(); }
};

class GameMenu : public sf::Drawable
{
public:
	enum ButtonNames
	{
		NumberOfPlayers = 0,
		Start,
		MAX_BUTTONNAMES
	};
private:
	sf::Font m_font;
	int m_charSize = 0;
	int m_thickness = 0;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	std::vector<TextBox*> m_textBoxes;
	std::vector<Button*> m_buttons;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for(auto &textbox : m_textBoxes)
			target.draw(*textbox);
		for (auto &button : m_buttons)
			target.draw(*button);
	}
public:
	GameMenu(const sf::Vector2f& pos, const sf::Vector2f& size,
		const sf::Font& font, int charSize, int thickness = 2, int offset = 8);
	~GameMenu();

	// btnIndex - index of button according to the enum ButtonNames 
	const Button* getButton(int btnIndex);
	Button* whichButtonMouseOver(sf::RenderWindow& window);

};

class Table : public sf::Drawable
{
private:
	sf::VertexArray m_tableVertices;
	sf::VertexArray m_deckVertices;
	sf::Texture m_diceSet;
	std::vector<int>* m_map = nullptr;
	std::vector<int> m_deck;
	unsigned m_width = 0;
	unsigned m_height = 0;
	sf::RectangleShape m_tableRect;
	sf::RectangleShape m_deckRect;
	sf::Vector2f m_startTablePos;
	sf::Vector2f m_startDeckPos;
	sf::Vector2u m_diceSize;
	std::vector<DiceButton*> m_diceButtonSet;
	int m_offsetX = 26;
	int m_offsetY = 12;
private:

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

public:
	Table(const std::string& texture, const sf::Vector2u& diceSize,
		std::vector<int>* map, const unsigned width, const unsigned height,
		const sf::RectangleShape& tableRect,
		const sf::RectangleShape& deckRect);
	void setDicesOnTable(int updateMapPlace = -1);
	void setDicesOnDeck();
	void applyDecorations();
	int setPlaces(std::vector<int>& dicePlaces, std::vector<int> &diceTops);
	void setButtonsPlaces(int updateMapPlace = -1, int diceNumber = -1);
	void update(int updateMapPlace = -1, int diceNumber = -1);
	Button* whichButtonMouseOver(sf::RenderWindow& window);
	void keepSwitch(Button* pressedButton);
	void unkeepAll();

	void setStartTablePos(const sf::Vector2f& startTablePos) { m_startTablePos = startTablePos; }
	void setStartDeckPos(const sf::Vector2f& startDeckPos) { m_startDeckPos = startDeckPos; }
	void setTableRect(const sf::RectangleShape& rect) { m_tableRect = rect; }
	void setDeckRect(const sf::RectangleShape& rect) { m_deckRect = rect; }
	void setOffset(const sf::Vector2f& offset);
};

class Scoreboard : public sf::Drawable
{
	class PlayerButtons;
private:
	std::vector<TextBox*> m_drawableObjects;
	std::vector<PlayerButtons*> m_playerBtns;
	std::vector<TextBox*> m_partialResultTextBoxes;
	std::vector<TextBox*> m_overallResultTextBoxes;
	std::vector<TextBox*> m_bonusTextBoxes;
	std::map<int, std::string> m_comboNames;
	std::vector<int> m_partialResult;
	std::vector<int> m_overallResult;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto &obj : m_drawableObjects)
			target.draw(*obj);
		for (auto &obj : m_playerBtns)
			target.draw(*obj);
	}

public:
	Scoreboard(const sf::Vector2f& startPosition,
		const sf::Vector2f& overallSize,
		const sf::Vector2f& titleSize,
		const std::map<int, std::string>& comboNames,
		const sf::Font& font, unsigned charSize, unsigned thickness);

	virtual ~Scoreboard()
	{
		for (auto& elem : m_drawableObjects)
			delete elem;
		for (auto& elem : m_playerBtns)
			delete elem;
	}
	Button* whichButtonMouseOver(sf::RenderWindow& window, int playerId);
	void updateScoreTips(int playerId, const std::vector<int> &scoresToUpdate);
	bool isFirstSection(Button* btn);
	void takeCombo(int playerId, Button* comboButton);
	void updateResults(int playerId);
	const std::string& getComboName(int playerId, Button* button);
	void setWinner();
	void setWinner(int playerId);
	void setBonus(int playerId);
	bool bonusCheck(int playerId);
	void onAllButtons();
	void newGame();

private:
	class PlayerButtons : public sf::Drawable
	{
	private:
		std::vector<Button*> m_buttons;
		static int m_idGenerator;
		int m_id;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			for (auto& combo : m_buttons)
				target.draw(*combo);
		}
	public:
		PlayerButtons(int positionX,
			const std::vector<int>& vecPositionY,
			const std::vector<sf::Vector2f>& sizeVec,
			int charSize, int thickness, const sf::Font& font,
			const sf::Color& bgColor = sf::Color::White, const sf::Color& textColor = sf::Color::Black);
		virtual ~PlayerButtons();
		Button* whichButtonMouseOver(sf::RenderWindow& window);
		void updateScoreOnButton(int buttonNumber, int scoreToUpdate);
		void onAllButtons();
		friend const std::string& Scoreboard::getComboName(int playerId, Button* button);
	};
};

#endif // !GRAPHICS_H
