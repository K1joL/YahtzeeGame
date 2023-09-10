#include "Graphics.h"

TableMap::TableMap(const std::string& texture, const sf::Vector2u &diceSize,
    std::vector<int>* map, const unsigned width, const unsigned height, const sf::Vector2u& startPos)
{
	m_diceSet.loadFromFile(texture);
    m_diceSet.setSmooth(true);
	m_vertices.setPrimitiveType(sf::Triangles);
	m_vertices.resize(width * height * 6);
    srand((unsigned)time(NULL));
    rand();
    m_map = map;
    m_width = width;
    m_height = height;
    m_startPos = startPos;
    m_diceSize = diceSize;
    //Create 5 buttons for 5 dice
    for (int i = 0; i < 5; i++)
        m_diceButtonSet.push_back(new Button(sf::Vector2f(diceSize),
            sf::Color::Transparent, sf::Color::Black));
    drawDices();
}

void TableMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &m_diceSet;
	target.draw(m_vertices, states);
    for (auto& diceButton : m_diceButtonSet)
        target.draw(*diceButton);
}

void TableMap::drawDices()
{
    int l = 0;
    for (unsigned int i = 0; i < m_width; ++i)
        for (unsigned int j = 0; j < m_height; ++j)
        {
            // get the current tile number
            int tileNumber = m_map->at(i + j * m_width);

            // find its position in the tileset texture
            int tu = tileNumber % (m_diceSet.getSize().x / m_diceSize.x);
            int tv = tileNumber / (m_diceSet.getSize().x / m_diceSize.x);

            // get a pointer to the triangle`s vertices of the current tile
            sf::Vertex* triangles = &m_vertices[(i + j * m_width) * 6];
            // define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(i * m_diceSize.x + m_startPos.x,
                j * m_diceSize.y + m_startPos.y);
            triangles[1].position = sf::Vector2f((i + 1) * m_diceSize.x + m_startPos.x,
                j * m_diceSize.y + m_startPos.y);
            triangles[2].position = sf::Vector2f(i * m_diceSize.x + m_startPos.x,
                (j + 1) * m_diceSize.y + m_startPos.y);
            triangles[3].position = sf::Vector2f(i * m_diceSize.x + m_startPos.x,
                (j + 1) * m_diceSize.y + m_startPos.y);
            triangles[4].position = sf::Vector2f((i + 1) * m_diceSize.x + m_startPos.x,
                j * m_diceSize.y + m_startPos.y);
            triangles[5].position = sf::Vector2f((i + 1) * m_diceSize.x + m_startPos.x,
                (j + 1) * m_diceSize.y + m_startPos.y);

            // create offset tiles for more random view
            for (int k = 0; k < 6; k++)
                triangles[k].position.x += m_offsetX * i;
            for (int k = 0; k < 6; k++)
                triangles[k].position.y += m_offsetY * j;

            if (j % 2 != 0)
                for (int k = 0; k < 6; k++)
                    triangles[k].position.x += 30;
            if (tileNumber != 0 && l < 5)
            {
                m_diceButtonSet[l]->setPosition(triangles[0].position);
                l++;
            }

            sf::Transform t;
            sf::Vector2f center((triangles[0].position.x + triangles[5].position.x) / 2,
                (triangles[0].position.y + triangles[5].position.y) / 2);

            int angle = i + j + (rand() % 90);
            t.rotate(angle, center);
            for (int k = 0; k < 6; k++)
            {
                triangles[k].position = t.transformPoint(triangles[k].position.x,
                    triangles[k].position.y);
            }

            // define the 6 matching texture coordinates
            triangles[0].texCoords = sf::Vector2f(tu * m_diceSize.x, tv * m_diceSize.y);
            triangles[1].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, tv * m_diceSize.y);
            triangles[2].texCoords = sf::Vector2f(tu * m_diceSize.x, (tv + 1) * m_diceSize.y);
            triangles[3].texCoords = sf::Vector2f(tu * m_diceSize.x, (tv + 1) * m_diceSize.y);
            triangles[4].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, tv * m_diceSize.y);
            triangles[5].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, (tv + 1) * m_diceSize.y);
        }
}

void TableMap::setPlaces(std::vector<int>& dicePlaces, std::vector<int>& diceTops)
{
    srand((unsigned)time(NULL));
    rand();
    int i = 0;
    for (auto& placeValue : dicePlaces)
        placeValue = 0;
    while (i < 5)
    {
        int place = 0 + (rand() % dicePlaces.size());
        auto& placeValue = dicePlaces[place];
        if (placeValue == 0)
        {
            placeValue = diceTops[i];
            i++;
        }
    }
    drawDices();
}

Button* TableMap::whichButtonMouseOver(sf::RenderWindow& window)
{
    for (auto& btn : m_diceButtonSet)
        if (btn->isMouseOver(window))
            return btn;
    return nullptr;
}

void Button::setPosition(sf::Vector2f point) 
{
    m_button.setPosition(point);
    // Center text on button:
    float xPos = (point.x + m_btnWidth / 2.0) - (m_text.getLocalBounds().width / 2);
    float yPos = (point.y + m_btnHeight / 2.2) - (m_text.getLocalBounds().height / 2);

    m_text.setPosition(xPos, yPos);
}

bool Button::isMouseOver(sf::RenderWindow& window)
{
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    int btnPosX = m_button.getPosition().x;
    int btnPosY = m_button.getPosition().y;

    int btnxPosWidth = m_button.getPosition().x + m_btnWidth;
    int btnyPosHeight = m_button.getPosition().y + m_btnHeight;

    if (mouseX < btnxPosWidth && mouseX > btnPosX && 
        mouseY < btnyPosHeight && mouseY > btnPosY) {
        return true;
    }
    return false;
}

Button::Button(sf::Vector2f buttonSize, sf::Color bgColor,
    sf::Color outlineColor)
{
    m_button.setSize(buttonSize);
    m_button.setFillColor(bgColor);
    m_button.setOutlineColor(outlineColor);
    m_button.setOutlineThickness(2);
    m_btnWidth = buttonSize.x;
    m_btnHeight = buttonSize.y;
}

Button::Button(std::string btnText, sf::Vector2f buttonSize,
                int charSize, sf::Font &font, sf::Color bgColor, sf::Color textColor)
{
    m_button.setSize(buttonSize);
    m_button.setFillColor(bgColor);
    m_button.setOutlineColor(sf::Color::Black);
    m_button.setOutlineThickness(2);

    // Get these for later use:
    m_btnWidth = buttonSize.x;
    m_btnHeight = buttonSize.y;

    m_text.setString(btnText);
    m_text.setCharacterSize(charSize);
    m_text.setFillColor(textColor);
    m_text.setFont(font);
}

YahtzeeTable::YahtzeeTable(int charSize, sf::Font& font, sf::Color bgColor, sf::Color textColor)
{
    int i = 0;
    for (i = 0; i < 13; i++)
    {
        m_combos.push_back(new Button(std::to_string(i), sf::Vector2f(164, 30), charSize, font));
        m_combos[i]->setPosition(sf::Vector2f(458, 13 + i * (30 + 2)));
    }
    m_totalScoreTextBox = new TextBox(std::string("0"), font, charSize, textColor,
        sf::Vector2f(458, 13 + i * (30 + 2)), sf::Vector2f(164, 38));
}

void YahtzeeTable::updateScore(sf::RenderWindow& window)
{
    m_totalScoreTextBox->setText(std::to_string(m_totalScore));
}

void YahtzeeTable::drawTo(sf::RenderWindow& window)
{
    for (auto& btn : m_combos)
        btn->drawTo(window);
    m_totalScoreTextBox->drawTo(window);
}

YahtzeeTable::~YahtzeeTable()
{
	for (auto& btn : m_combos)
		delete btn;
}

Button* YahtzeeTable::whichButtonMouseOver(sf::RenderWindow& window)
{
    for (auto& btn : m_combos)
        if (btn->isMouseOver(window))
            return btn;
    return nullptr;
}

TextBox::TextBox(const std::string& text, const sf::Font& font, int charSize, const sf::Color& charColor,
    const sf::Vector2f& pos, const sf::Vector2f& size)
{
    m_text = new sf::Text(text, font, charSize);
    //center text
    float xPos = (pos.x + size.x / 2) - (m_text->getLocalBounds().width / 2);
    float yPos = (pos.y + size.y / 2.2) - (m_text->getLocalBounds().height / 2);
    m_text->setPosition(sf::Vector2f(xPos, yPos));
    m_text->setFillColor(charColor);

    m_textRect = new sf::RectangleShape(size);
    m_textRect->setPosition(pos);
    m_textRect->setOutlineColor(sf::Color::Black);
    m_textRect->setOutlineThickness(2);
}
