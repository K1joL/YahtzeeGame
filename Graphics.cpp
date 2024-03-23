#include "Graphics.h"
// todo : 
// round all of the vectors upon initialization
// 
//
void updatePlayerTextBox(TextBox& textBox, char currentPlayerNumber)
{
    std::string temp = textBox.getText();
    temp.back() = currentPlayerNumber;
    textBox.setText(temp);
}

// animate textBox according to animationType
// blocks window!
// duration in seconds
void AnimateTextBox(TextBox& textBox, int duration, int animationType, sf::RenderWindow& window)
{
    window.setActive(true);
    sf::Color rectColor = textBox.getFillColor();
    sf::Color textColor = textBox.getTextColor();
    uint8_t alpha = 0;
    switch (animationType)
    {
    case AnimationType::Appearance:
        alpha = 0;
        while (alpha < 255)
        {
            alpha += 1;
            textBox.setAlpha(alpha);
            sf::sleep(sf::Time(sf::milliseconds(duration/255.f)));
            window.draw(textBox);
            window.display();
        }
        break;
    case AnimationType::Disappearance:
        alpha = 255;
        while (alpha > 0)
        {
            alpha -= 1;
            textBox.setAlpha(alpha);
            sf::sleep(sf::Time(sf::milliseconds(duration / 255.f)));
            window.draw(textBox);
            window.display();
        }
        break;
    }
    window.setActive(false);
}

void AnimationOfCurrentPlayer(TextBox& textBox, int duration, sf::RenderWindow& window)
{
	sf::Thread thread1(std::bind(&AnimateTextBox, std::ref(textBox), duration / 3.f,
        AnimationType::Appearance, std::ref(window)));
	thread1.launch();
	thread1.wait();
	sf::sleep(sf::Time(sf::milliseconds(duration / 3.f)));
	sf::Thread thread2(std::bind(&AnimateTextBox, std::ref(textBox), duration / 3.f,
        AnimationType::Disappearance, std::ref(window)));
	thread2.launch();
	thread2.wait();
}

/////////////////////////////////////
//Table
/////////////////////////////////////

Table::Table(const std::string& texture, const sf::Vector2u& diceSize,
    std::vector<int>* map, const unsigned width, const unsigned height,
    const sf::RectangleShape& tableRect, const sf::RectangleShape& deckRect)
    :
    m_diceSize(diceSize),
    m_map(map),
    m_width(width),
    m_height(height),
    m_tableRect(tableRect),
    m_deckRect(deckRect)
{
	m_diceSet.loadFromFile(texture);
    m_diceSet.setSmooth(true);
    m_tableVertices.setPrimitiveType(sf::Triangles);
    m_tableVertices.resize(width * height * 6);
    m_deckVertices.setPrimitiveType(sf::Triangles);
    m_deckVertices.resize(5 * 6);   // 5 is maximum number of dice
    m_deck.reserve(5);
    m_deck = { 0,0,0,0,0 };
    m_startTablePos = tableRect.getPosition();
    m_startDeckPos = deckRect.getPosition();
    //Create 5 buttons for 5 dice
    for (int i = 0; i < 5; i++)
    {
        m_diceButtonSet.push_back(new DiceButton(sf::Vector2f(diceSize), i,
            sf::Color::Transparent, sf::Color::Black));
        m_diceButtonSet.back()->setText(std::to_string(i));
    }
}

void Table::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &m_diceSet;
	target.draw(m_tableVertices, states);
    target.draw(m_deckVertices, states);
    for (auto& diceButton : m_diceButtonSet)
        target.draw(*diceButton);
}

void Table::setDicesOnTable(int updateMapPlace)
{
    for (unsigned int i = 0; i < m_height; ++i)
        for (unsigned int j = 0; j < m_width; ++j)
        {
            int placeNumber = j + i * m_width;

            if (updateMapPlace != -1)
            {
                placeNumber = updateMapPlace;
                j = placeNumber % m_width;
                i = placeNumber / m_width;
            }

            // get the current tile number
            int tileNumber = m_map->at(placeNumber);
            // find its position in the tileset texture
            int tu = tileNumber % (m_diceSet.getSize().x / m_diceSize.x);
            int tv = tileNumber / (m_diceSet.getSize().x / m_diceSize.x);

            // get a pointer to the triangle`s vertices of the current tile
            sf::Vertex* triangles = &m_tableVertices[(placeNumber) * 6];

            // define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(j * m_diceSize.x + m_startTablePos.x,
                i * m_diceSize.y + m_startTablePos.y);
            triangles[1].position = sf::Vector2f((j + 1) * m_diceSize.x + m_startTablePos.x,
                i * m_diceSize.y + m_startTablePos.y);
            triangles[2].position = sf::Vector2f(j * m_diceSize.x + m_startTablePos.x,
                (i + 1) * m_diceSize.y + m_startTablePos.y);
            triangles[3].position = sf::Vector2f(j * m_diceSize.x + m_startTablePos.x,
                (i + 1) * m_diceSize.y + m_startTablePos.y);
            triangles[4].position = sf::Vector2f((j + 1) * m_diceSize.x + m_startTablePos.x,
                i * m_diceSize.y + m_startTablePos.y);
            triangles[5].position = sf::Vector2f((j + 1) * m_diceSize.x + m_startTablePos.x,
                (i + 1) * m_diceSize.y + m_startTablePos.y);

            // create offset tiles for more random view
            for (int k = 0; k < 6; k++)
                triangles[k].position.x += m_offsetX * j;
            for (int k = 0; k < 6; k++)
                triangles[k].position.y += m_offsetY * i;
            if (i % 2 != 0)
                for (int k = 0; k < 6; k++)
                    triangles[k].position.x += m_offsetX;

            // define the 6 matching texture coordinates
            triangles[0].texCoords = sf::Vector2f(tu * m_diceSize.x, tv * m_diceSize.y);
            triangles[1].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, tv * m_diceSize.y);
            triangles[2].texCoords = sf::Vector2f(tu * m_diceSize.x, (tv + 1) * m_diceSize.y);
            triangles[3].texCoords = sf::Vector2f(tu * m_diceSize.x, (tv + 1) * m_diceSize.y);
            triangles[4].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, tv * m_diceSize.y);
            triangles[5].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, (tv + 1) * m_diceSize.y);

            // to set only one place
            if (updateMapPlace != -1)
                return;
        }
}

void Table::setDicesOnDeck()
{
	for (unsigned int i = 0; i < m_deck.size(); ++i)
	{
		// get the current tile number
		int tileNumber = m_deck.at(i);
		// find its position in the tileset texture
		int tu = tileNumber % (m_diceSet.getSize().x / m_diceSize.x);
		int tv = tileNumber / (m_diceSet.getSize().x / m_diceSize.x);

		// get a pointer to the triangle`s vertices of the current tile
		sf::Vertex* triangles = &m_deckVertices[(i) * 6];
		// define the 6 corners of the two triangles
        triangles[0].position = sf::Vector2f(i * m_diceSize.x + m_startDeckPos.x,
            m_startDeckPos.y);
        triangles[1].position = sf::Vector2f((i + 1) * m_diceSize.x + m_startDeckPos.x,
            m_startDeckPos.y);
        triangles[2].position = sf::Vector2f(i * m_diceSize.x + m_startDeckPos.x,
            m_diceSize.y + m_startDeckPos.y);
        triangles[3].position = sf::Vector2f(i * m_diceSize.x + m_startDeckPos.x,
            m_diceSize.y + m_startDeckPos.y);
        triangles[4].position = sf::Vector2f((i + 1) * m_diceSize.x + m_startDeckPos.x,
            m_startDeckPos.y);
        triangles[5].position = sf::Vector2f((i + 1) * m_diceSize.x + m_startDeckPos.x,
            m_diceSize.y + m_startDeckPos.y);

        int offsetX = (m_deckRect.getSize().x / 5 - m_diceSize.x);
		for (int k = 0; k < 6; k++)
			triangles[k].position.x += offsetX * i;

		// define the 6 matching texture coordinates
		triangles[0].texCoords = sf::Vector2f(tu * m_diceSize.x, tv * m_diceSize.y);
		triangles[1].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, tv * m_diceSize.y);
		triangles[2].texCoords = sf::Vector2f(tu * m_diceSize.x, (tv + 1) * m_diceSize.y);
		triangles[3].texCoords = sf::Vector2f(tu * m_diceSize.x, (tv + 1) * m_diceSize.y);
		triangles[4].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, tv * m_diceSize.y);
		triangles[5].texCoords = sf::Vector2f((tu + 1) * m_diceSize.x, (tv + 1) * m_diceSize.y);
	}
}

void Table::applyDecorations()
{
    for (unsigned int i = 0; i < m_height; ++i)
        for (unsigned int j = 0; j < m_width; ++j)
        {
            int placeNumber = j + i * m_width;
            int tileNumber = m_map->at(placeNumber);
            if (tileNumber == 0)
                continue;
            sf::Vertex* triangles = &m_tableVertices[(placeNumber) * 6];

            sf::Vector2f tmp(j * m_diceSize.x + m_startTablePos.x + j * m_offsetX,
                             i * m_diceSize.y + m_startTablePos.y + i * m_offsetY);
            if (i % 2 != 0)
                tmp.x += m_offsetX;

            if (triangles[0].position.x != tmp.x && 
                triangles[0].position.y != tmp.y)
                continue;
            // some rotate dice tile
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
        }
}

//Sets dice map according to the diceTops
int Table::setPlaces(std::vector<int>& dicePlaces, std::vector<int>& diceTops)
{
    std::vector<int> dicePlacesCopy = dicePlaces;
    //clear places on the map
    for (auto& placeValue : dicePlacesCopy)
        placeValue = 0;

    //divide the size of map by the number of dice
    int shift = dicePlacesCopy.size() / 5,
        minValue = 0,
        maxValue = shift * 2 - 1;

    int place = 0,
        topNumber = 0,
        diceNumber = 0;
    bool reroll = false,
         logicFailureFlag = false;
    while (topNumber < diceTops.size())
    {
        if (m_diceButtonSet[diceNumber]->isKeeped())
        {
            minValue += shift;
            maxValue += shift;
            diceNumber++;
            continue;
        }
        // interval adjustment
        if (place > minValue)
            minValue = place;
        if (maxValue >= dicePlacesCopy.size() - 1)
            maxValue = dicePlacesCopy.size() - 1;
        // if first roll then skip
        if (!(m_diceButtonSet[0]->getDiceMapPlace() == 0 &&
            m_diceButtonSet[1]->getDiceMapPlace() == 0))
        {
            int adjustment = 0;
            if (diceNumber == 0)
                if (((adjustment = m_diceButtonSet[diceNumber + 1]->getDiceMapPlace()) <= maxValue) &&
                    (m_diceButtonSet[diceNumber + 1]->isKeeped()))
                    maxValue = --adjustment;
            if (diceNumber > 0 && diceNumber < m_diceButtonSet.size()-1)
            {
                if (((adjustment = m_diceButtonSet[diceNumber + 1]->getDiceMapPlace()) <= maxValue) &&
                    (m_diceButtonSet[diceNumber + 1]->isKeeped()))
                    maxValue = --adjustment;
                if (((adjustment = m_diceButtonSet[diceNumber - 1]->getDiceMapPlace()) >= minValue) &&
                    (m_diceButtonSet[diceNumber - 1]->isKeeped()))
                    minValue = ++adjustment;
            }
            if (diceNumber == m_diceButtonSet.size() - 1)
                if (((adjustment = m_diceButtonSet[diceNumber - 1]->getDiceMapPlace()) >= minValue) &&
                    (m_diceButtonSet[diceNumber - 1]->isKeeped()))
                    minValue = ++adjustment;
        }
        /*if ((maxValue - minValue + 1) != 0)
            place = minValue + (rand() % (maxValue - minValue + 1));
        else
            place = minValue + (rand() % (maxValue - minValue + 2));*/
        // if we have dividing by zero it seems 
        // that we have a wrong position for previous dice
        // needed to try setPlace again
        if ((maxValue - minValue + 1) == 0)
        {
            logicFailureFlag = true;
            break;
        }
        place = minValue + (rand() % (maxValue - minValue + 1));
        reroll = false;
        for (int j = 0; j < m_deck.size(); j++)
            if (m_deck[j] != 0)
                if (m_diceButtonSet[j]->getDiceMapPlace() == place)
                {
                    reroll = true;
                    break;
                }
        if (reroll)
            continue;

        int& placeValue = dicePlacesCopy[place];
        if (placeValue == 0)
        {
            placeValue = diceTops[topNumber];
            topNumber++;
        }
        else continue;
        minValue += shift;
        maxValue += shift;
        place++;
        diceNumber++;
    }
    
    if (logicFailureFlag)
        return -1;
    dicePlaces = dicePlacesCopy;
	setDicesOnTable();
	setDicesOnDeck();
	setButtonsPlaces();
	applyDecorations();
    return 0;
}

void Table::setButtonsPlaces(int updateMapPlace, int diceNumber)
{
    int l = 0;
    if (diceNumber != -1)
        l = diceNumber;
    for (unsigned int i = 0; i < m_height; ++i)
        for (unsigned int j = 0; j < m_width; ++j)
        {
            if (diceNumber != -1)
            {
                j = updateMapPlace % m_width;
                i = updateMapPlace / m_width;
            }
            int placeNumber = j + i * m_width;
            int tileNumber = m_map->at(placeNumber);
            if (tileNumber == 0 && m_deck[l] == 0)
                continue;
            sf::Vertex* triangles = nullptr;
            if (l < 5)
            {
                if (m_diceButtonSet[l]->isKeeped())
                {
                    triangles = &m_deckVertices[l*6];
                    m_diceButtonSet[l]->setPosition(triangles[0].position);
                }
                else
                {
                    triangles = &m_tableVertices[(placeNumber) * 6];
                    m_diceButtonSet[l]->setPosition(triangles[0].position);
                    std::string number;
                    number = m_diceButtonSet[l]->getText()[0];
                    m_diceButtonSet[l]->setDiceMapPlace(placeNumber);
                    m_diceButtonSet[l]->setDiceTop(tileNumber);
                    m_diceButtonSet[l]->setText(number +
                        std::to_string(placeNumber) +
                        std::to_string(tileNumber));
                }
                l++;
            }
            if (l == 5 || diceNumber != -1)
                return;
        }
}
void Table::update(int updateMapPlace, int diceNumber)
{
    setDicesOnTable(updateMapPlace);
    setDicesOnDeck();
    setButtonsPlaces(updateMapPlace, diceNumber);
    applyDecorations();
}

void Table::setOffset(const sf::Vector2f& offset)
{
    m_offsetX = offset.x;
    m_offsetY = offset.y;
}

Button* Table::whichButtonMouseOver(sf::RenderWindow& window)
{
    for (auto& btn : m_diceButtonSet)
        if (btn->isMouseOver(window))
            return btn;
    return nullptr;
}

void Table::keepSwitch(Button* pressedButton)
{
    if(DiceButton *dButton = dynamic_cast<DiceButton*>(pressedButton))
    {
        dButton->keepSwitch();
        if(dButton->isKeeped())
		{
            for (int i = 0; i < m_deck.size(); i++)
                if (i == dButton->getDiceNumber())
                {
                    m_deck.at(i) = dButton->getDiceTop();
                    m_map->at(dButton->getDiceMapPlace()) = 0;
                    break;
                }
        }
        else
        {
            for (int i = 0; i < m_deck.size(); i++)
                if (m_deck[i] == dButton->getDiceTop() &&
                    i == dButton->getDiceNumber())
                {
                    m_deck.at(i) = 0;
                    m_map->at(dButton->getDiceMapPlace()) = dButton->getDiceTop();
                    break;
                }
        }
        update(dButton->getDiceMapPlace(), dButton->getDiceNumber());
    }
}

void Table::unkeepAll()
{
    for (auto& btn : m_diceButtonSet)
        if(btn->isKeeped())
            keepSwitch(btn);
}

/////////////////////////////////////
//Button
/////////////////////////////////////

Button::Button(const sf::Vector2f& size, 
    const sf::Color& bgColor, const sf::Color& outlineColor)
{
    m_rect.setSize(size);
    m_rect.setFillColor(bgColor);
    m_rect.setOutlineColor(outlineColor);
    m_rect.setOutlineThickness(2);
}

Button::Button(const std::string& text, int charSize, const sf::Font& font,
    const sf::Vector2f& size, const sf::Vector2f& pos,
    const sf::Color& fillColor, const sf::Color& textColor)
{
    m_rect.setSize(size);
    m_rect.setFillColor(fillColor);
    m_rect.setOutlineColor(sf::Color::Black);
    m_rect.setOutlineThickness(2);

    m_text.setString(text);
    m_text.setCharacterSize(charSize);
    m_text.setFillColor(textColor);
    m_text.setFont(font);
    //center text
    setPosition(pos);
}

void Button::setText(const std::string &text)
{
    m_text.setString(text);
    setPosition(m_rect.getPosition());
}

void Button::setPosition(const sf::Vector2f& point) 
{
    if(point != m_rect.getPosition())
        m_rect.setPosition(point);
    //center text
    m_text.setOrigin(AuFunc::round(m_text.getGlobalBounds().getSize() /
        2.f + m_text.getLocalBounds().getPosition()));
    m_text.setPosition(AuFunc::round(m_rect.getPosition() + m_rect.getSize() / 2.f));
}

bool Button::isMouseOver(sf::RenderWindow& window) const
{
    int mouseX = sf::Mouse::getPosition(window).x;
    int mouseY = sf::Mouse::getPosition(window).y;

    int btnPosX = m_rect.getPosition().x;
    int btnPosY = m_rect.getPosition().y;

    int btnWidth = m_rect.getPosition().x + m_rect.getSize().x;
    int btnHeight = m_rect.getPosition().y + m_rect.getSize().y;

    if (mouseX < btnWidth && mouseX > btnPosX &&
        mouseY < btnHeight && mouseY > btnPosY) {
        return true;
    }
    return false;
}


/////////////////////////////////////
//DiceButton
/////////////////////////////////////

DiceButton::DiceButton(const sf::Vector2f& buttonSize, int diceNumber,
    const sf::Color& bgColor, const sf::Color& outlineColor)
    : Button(buttonSize, bgColor, outlineColor)
{
    m_diceNumber = diceNumber;
}

/////////////////////////////////////
//ScoreboardPlayerButtons
/////////////////////////////////////
int Scoreboard::PlayerButtons::m_idGenerator = 0;

Scoreboard::PlayerButtons::PlayerButtons(int positionX,
    const std::vector<int>& vecPositionY,
    const std::vector<sf::Vector2f>& sizeVec,
    int charSize, int thickness, const sf::Font &font,
    const sf::Color &bgColor, const sf::Color &textColor)
{
    m_id = m_idGenerator++;

    for (int i = 0; i < 13; i++)
    {
        m_buttons.push_back(new Button("", charSize, font,
            sf::Vector2f(sizeVec[i].x, sizeVec[i].y)));
        m_buttons.back()->setPosition(
            sf::Vector2f(positionX, vecPositionY[i]));
    }
}

Scoreboard::PlayerButtons::~PlayerButtons()
{
	for (auto& btn : m_buttons)
		delete btn;
}

void Scoreboard::PlayerButtons::updateScoreOnButton(int buttonNumber, int scoreToUpdate)
{
    if (m_buttons.at(buttonNumber)->isOn())
        if (scoreToUpdate != 0)
            m_buttons.at(buttonNumber)->setText(std::to_string(scoreToUpdate));
        else
            m_buttons.at(buttonNumber)->setText("");
}

Button* Scoreboard::PlayerButtons::whichButtonMouseOver(sf::RenderWindow& window)
{
    for (auto& btn : m_buttons)
        if (btn->isMouseOver(window))
            return btn;
    return nullptr;
}

void Scoreboard::PlayerButtons::onAllButtons()
{
    for (auto& btn : m_buttons)
        btn->onButton();
}

/////////////////////////////////////
//TextBox
/////////////////////////////////////

TextBox::TextBox(const std::string& text, const sf::Font& font, int charSize,
    const sf::Vector2f& pos, const sf::Vector2f& size,
    const sf::Color& fillColor, const sf::Color& textColor)
{
    m_text.setString(text);
    m_text.setFont(font);
    m_text.setCharacterSize(charSize);
    m_text.setFillColor(textColor);

    m_rect.setSize(size);
    m_rect.setFillColor(fillColor);
    m_rect.setOutlineColor(sf::Color::Black);
    m_rect.setOutlineThickness(2);
    //center text
    setPosition(pos);
}

void TextBox::setPosition(const sf::Vector2f& pos)
{
    if (pos != m_rect.getPosition())
        m_rect.setPosition(pos);

    m_text.setOrigin(AuFunc::round(m_text.getGlobalBounds().getSize() /
        2.f + m_text.getLocalBounds().getPosition()));
    m_text.setPosition(AuFunc::round(m_rect.getPosition() + m_rect.getSize() / 2.f));
}

void TextBox::setText(const std::string &text)
{
    m_text.setString(text);
    setPosition(m_rect.getPosition());
}

void TextBox::setAlpha(uint8_t alpha)
{
    sf::Color c = m_rect.getFillColor();
    c.a = alpha;
    /*c = { c.r, c.g, c.b, alpha };*/
    m_rect.setFillColor(c);

    c = m_rect.getOutlineColor();
    c.a = alpha;
    /*c = { c.r, c.g, c.b, alpha };*/
    m_rect.setOutlineColor(c);

    c = m_text.getFillColor();
    c.a = alpha;
    /*c = { c.r, c.g, c.b, alpha };*/
    m_text.setFillColor(c);

    c = m_text.getOutlineColor();
    c.a = alpha;
    /*c = { c.r, c.g, c.b, alpha };*/
    m_text.setOutlineColor(c);
}
/////////////////////////////////////
//Scoreboard
/////////////////////////////////////

Scoreboard::Scoreboard(const sf::Vector2f &startPosition, 
        const sf::Vector2f& overallSize,
        const sf::Vector2f& titleSize,
        const std::map<int, std::string>& comboNames,
        const sf::Font &font, unsigned charSize, unsigned thickness)
{
    // there`s 7 (+ 1 title) two line names and 9 one line names
    // but 
    auto sizesY = AuFunc::SizingMethod(overallSize.y - thickness * 16, 16, titleSize.y, 9, 8);
    int oneLineNameSizeY = sizesY[0];
    int twoLineNameSizeY = sizesY[1];
     m_comboNames = comboNames;
    TextBox *SBTitle = new TextBox(std::string("Scoreboard"), font, charSize,
        startPosition, 
        sf::Vector2f(titleSize.x, twoLineNameSizeY));
    int comboNamesSizeX = ((SBTitle->getRectSize().x - thickness * 4) / 17 * 5);
    std::vector<TextBox*> SBCombos;
    SBCombos.reserve(13);

    for (int i = 0; i < 13; i++)
    {
        //string handler
        std::string tempStr = comboNames.at(i);
        int spaceCounter = 0;
        for (auto& ch : tempStr)
        {
            if (ch == ' ')
                spaceCounter++;
            if ((i < 8 && spaceCounter == 2) ||
                (i >= 8 && spaceCounter == 1))
            {
                ch = '\n';
                break;
            }
        }
        int rectPositionY = 0;
        if(i == 0)
            rectPositionY = SBTitle->getRectPosition().y +
                            SBTitle->getRectSize().y + thickness;
        else
            rectPositionY = SBCombos.back()->getRectPosition().y +
                            SBCombos.back()->getRectSize().y + thickness;
        int rectSizeY = twoLineNameSizeY;
        if (i < 6 || i == 11 || i == 12)
            rectSizeY = oneLineNameSizeY;
        if (i == 6)
            rectPositionY += twoLineNameSizeY + oneLineNameSizeY + thickness * 2;
        //making combo names on the scoreboard
        SBCombos.push_back(new TextBox(tempStr, font, charSize,
            sf::Vector2f(trunc(SBTitle->getRectPosition().x), rectPositionY),
            sf::Vector2f(comboNamesSizeX,
                rectSizeY)));
    }
    for (auto& elem : SBCombos)
        m_drawableObjects.push_back(elem);

    TextBox *SBPartialResult = new TextBox(std::string("Partial\nresult"), font, charSize,
        sf::Vector2f(SBCombos[5]->getRectPosition().x,
            SBCombos[5]->getRectPosition().y +
            SBCombos[5]->getRectSize().y + thickness),
        sf::Vector2f(comboNamesSizeX,
            twoLineNameSizeY));
    m_drawableObjects.push_back(SBPartialResult);

    TextBox *SBOverallResult = new TextBox(std::string("Overall\nresult"), font, charSize,
        sf::Vector2f(SBCombos.back()->getRectPosition().x,
            SBCombos.back()->getRectPosition().y +
            SBCombos.back()->getRectSize().y + thickness),
        sf::Vector2f(comboNamesSizeX,
            twoLineNameSizeY));
    m_drawableObjects.push_back(SBOverallResult);

    TextBox *SBBonus = new TextBox(std::string("Bonus"), font, charSize,
        sf::Vector2f(SBPartialResult->getRectPosition().x,
            SBPartialResult->getRectPosition().y +
            SBPartialResult->getRectSize().y + thickness),
        sf::Vector2f(comboNamesSizeX,
            oneLineNameSizeY));
    m_drawableObjects.push_back(SBBonus);
    m_drawableObjects.push_back(SBTitle);
    int buttonsSizeX = (SBTitle->getRectSize().x - comboNamesSizeX) / 4 - thickness;
    //making textboxes for players data on the partial results line
    for (int i = 0; i < 4; i++)
    {
        m_drawableObjects.push_back(new TextBox("", font, charSize,
            sf::Vector2f(SBPartialResult->getRectPosition().x + (SBPartialResult->getRectSize().x + thickness) +
                (buttonsSizeX + thickness) * i,
                SBPartialResult->getRectPosition().y),
            sf::Vector2f(buttonsSizeX, twoLineNameSizeY)));
        m_partialResultTextBoxes.push_back(m_drawableObjects.back());
    }
    //making textboxes for players data on the overall results line
    for (int i = 0; i < 4; i++)
    {
        m_drawableObjects.push_back(new TextBox("", font, charSize,
            sf::Vector2f(SBOverallResult->getRectPosition().x + (SBOverallResult->getRectSize().x + thickness) +
                (buttonsSizeX + thickness) * i,
                SBOverallResult->getRectPosition().y),
            sf::Vector2f(buttonsSizeX, twoLineNameSizeY)));
        m_overallResultTextBoxes.push_back(m_drawableObjects.back());
    }
    //making textboxes for players data on the winner line
    for (int i = 0; i < 4; i++)
    {
        m_drawableObjects.push_back(new TextBox("", font, charSize,
            sf::Vector2f(SBBonus->getRectPosition().x + (SBBonus->getRectSize().x + thickness) +
                (buttonsSizeX + thickness) * i,
                SBBonus->getRectPosition().y),
            sf::Vector2f(buttonsSizeX, oneLineNameSizeY)));
        m_bonusTextBoxes.push_back(m_drawableObjects.back());
    }

    //data for buttons
    std::vector<sf::Vector2f> sizeButtonsVector;
    std::vector<int> vecPositionY;
    sizeButtonsVector.reserve(13);
    vecPositionY.reserve(13);

    for (int i = 0; i < 13; i++)
        sizeButtonsVector.push_back(sf::Vector2f(
            buttonsSizeX, SBCombos[i]->getRectSize().y));

    for (int i = 0; i < 13; i++)
        vecPositionY.push_back(SBCombos[i]->getRectPosition().y);

    //making buttons to take combo for 4 players
    for(int i = 0; i < 4; i++)
        m_playerBtns.push_back(new PlayerButtons(
            SBCombos[0]->getRectPosition().x + SBCombos[0]->getRectSize().x + 
            thickness + (buttonsSizeX + thickness) * i,
            vecPositionY,
            sizeButtonsVector,
            charSize, thickness, font));
    //initialize vectors
    for (int i = 0; i < 4; i++)
    {
        m_partialResult.push_back(0);
        m_overallResult.push_back(0);
    }
}

Button* Scoreboard::whichButtonMouseOver(sf::RenderWindow& window, int playerId)
{
    if (playerId < m_playerBtns.size())
        return m_playerBtns[playerId]->whichButtonMouseOver(window);
    else
        return nullptr;
}

void Scoreboard::updateScoreTips(int playerId, const std::vector<int>& scoresToUpdate)
{
    for (int comboNum = 0; comboNum < m_comboNames.size(); comboNum++)
        m_playerBtns.at(playerId)->updateScoreOnButton(
            comboNum, scoresToUpdate[comboNum]);
}

bool Scoreboard::isFirstSection(Button* btn)
{
    for (int i = 0; i < 6; i++)
        if (btn->getRectPosition().y == m_drawableObjects[i]->getRectPosition().y)
			return true;
    return false;
}

void Scoreboard::takeCombo(int playerId, Button* comboButton)
{
    comboButton->offButton();
    int comboScore = 0;
    if (comboButton->getText() != "")
        comboScore = stoi(comboButton->getText());
    else
        comboButton->setText("0");
    if (isFirstSection(comboButton))
        m_partialResult[playerId] += comboScore;
    m_overallResult[playerId] += comboScore;
    updateResults(playerId);
}

void Scoreboard::updateResults(int playerId)
{
    m_partialResultTextBoxes[playerId]->setText(std::to_string(m_partialResult[playerId]));
    m_overallResultTextBoxes[playerId]->setText(std::to_string(m_overallResult[playerId]));
}

const std::string& Scoreboard::getComboName(int playerId, Button* button)
{
    auto& buttons = m_playerBtns[playerId]->m_buttons;
    for (int i = 0; i < buttons.size(); i++)
        if (button == buttons[i])
            return m_comboNames.at(i);
    return "";
}

void Scoreboard::setWinner()
{
    int winnersScore = 0;
    int winnerId = 0;
    for (int id = 0; id < m_overallResult.size(); id++)
    {
        if (m_overallResult[id] > winnersScore)
        {
            winnersScore = m_overallResult[id];
            winnerId = id;
        }
    }
    m_overallResultTextBoxes[winnerId]->setFillColor(sf::Color::Green);
}

void Scoreboard::setWinner(int playerId)
{
    m_overallResultTextBoxes[playerId]->setFillColor(sf::Color::Green);
}

void Scoreboard::setBonus(int playerId)
{
    m_bonusTextBoxes[playerId]->setText("35");
    m_partialResult[playerId] += 35;
    m_overallResult[playerId] += 35;
}

bool Scoreboard::bonusCheck(int playerId)
{
    return m_partialResult[playerId] >= 63;
}

void Scoreboard::onAllButtons()
{
    for (auto& player : m_playerBtns)
        player->onAllButtons();
}

void Scoreboard::newGame()
{
    onAllButtons();
    for (int i = 0; i < 4; i++)
    {
        updateScoreTips(i, std::vector<int>(13, 0));
        m_partialResult[i] = 0;
        m_overallResult[i] = 0;
        m_bonusTextBoxes[i]->setText("");
        updateResults(i);
        m_overallResultTextBoxes[i]->setFillColor(sf::Color::White);
    }
}

/////////////////////////////////////
//GameMenu
/////////////////////////////////////

GameMenu::GameMenu(const sf::Vector2f& pos, const sf::Vector2f& size,
    const sf::Font& font, int charSize, int thickness, int offset)
    : 
    m_font(font),
    m_charSize(charSize),
    m_thickness(thickness),
    m_position(pos),
    m_size(size)
{
    sf::Vector2f titleSize(m_size.x, trunc(m_size.y/17));
    m_textBoxes.push_back(new TextBox(std::string("Scoreboard"), font, charSize,
        m_position, titleSize));

    //number of players textbox
    m_textBoxes.push_back(new TextBox(
        "Number of\nplayers", m_font, m_charSize,
        sf::Vector2f(m_position.x,
            m_position.y + m_textBoxes.back()->getRectSize().y + 2 * m_thickness + offset),
        sf::Vector2f(titleSize.x / 2, titleSize.y)));

    //number of players button
    m_buttons.push_back(new Button("1", charSize, font,
        sf::Vector2f(
            m_textBoxes.back()->getRectSize().x / 2,
            titleSize.y)));
    m_buttons.back()->setPosition(
        sf::Vector2f(m_position.x + m_size.x - m_buttons.back()->getRectSize().x,
            m_textBoxes.back()->getRectPosition().y));

    //start Button
    m_buttons.push_back(new Button("Start", charSize, font,
        sf::Vector2f(m_buttons.back()->getRectSize())));
    m_buttons.back()->setPosition(
        sf::Vector2f(m_position.x + m_size.x / 2 - m_buttons.back()->getRectSize().x / 2,
            m_position.y + m_size.y - m_buttons.back()->getRectSize().y));
}

GameMenu::~GameMenu()
{
    for (auto& textbox : m_textBoxes)
        delete textbox;
    for (auto& button : m_buttons)
        delete button;
}
const Button* GameMenu::getButton(int btnIndex)
{
	return m_buttons[btnIndex];
}

Button* GameMenu::whichButtonMouseOver(sf::RenderWindow &window)
{
    for (auto& btn : m_buttons)
        if (btn->isMouseOver(window))
            return btn;
    return nullptr;
}