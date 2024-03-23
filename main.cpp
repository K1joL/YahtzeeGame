#include "Game.h"
#include "Graphics.h"
#include <iostream>
#include <SFML/System.hpp>

using namespace std;
using namespace sf;

constexpr auto MAX_COMBO = 13;

int main()
{
	srand(time(NULL));
	rand();

	Game g;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow mainWindow;
	mainWindow.create(VideoMode(640, 480), "YAHTZEE!", Style::Titlebar | Style::Close, settings);

	Font arialFont;
	arialFont.loadFromFile("./fonts/arialmt.ttf");

	int		thickness = 2,
			offset = 8,
			charSize = 12;

	//UI design
	RectangleShape MainUIRect(Vector2f(430, 460));
	MainUIRect.setPosition(Vector2f(offset+2, offset));
	MainUIRect.setOutlineColor(Color::Black);
	MainUIRect.setOutlineThickness(thickness);

	RectangleShape tableRect(Vector2f(410, 300));
	tableRect.setPosition(Vector2f(MainUIRect.getPosition().x + thickness + offset,
								   MainUIRect.getPosition().y + thickness + offset));
	tableRect.setOutlineColor(Color::Black);
	tableRect.setOutlineThickness(thickness);
	
	RectangleShape tableRollBar(Vector2f(410, 50));
	tableRollBar.setPosition(Vector2f(tableRect.getPosition().x,
									  tableRect.getPosition().y + tableRect.getSize().y + thickness));
	tableRollBar.setOutlineColor(Color::Black);
	tableRollBar.setOutlineThickness(thickness);

	RectangleShape tableKeepingDeck(Vector2f(410, 76));
	tableKeepingDeck.setPosition(Vector2f(tableRollBar.getPosition().x,
		tableRollBar.getPosition().y + tableRollBar.getSize().y + thickness + thickness + offset));
	tableKeepingDeck.setOutlineColor(Color::Black);
	tableKeepingDeck.setOutlineThickness(thickness);

	Scoreboard scoreboard(Vector2f(MainUIRect.getPosition().x + MainUIRect.getSize().x +
		thickness + thickness + offset,
		MainUIRect.getPosition().y),
		Vector2f(mainWindow.getSize().x - MainUIRect.getSize().x - 3 * offset,
			MainUIRect.getSize().y),
		Vector2f(178, 28), // need to delete
		g.getInvertComboMap(),
		arialFont, charSize, thickness);
	
	string diceTexturePath = "./images/blue_dice.png";
	Vector2u diceSize(66, 62);
	vector<int> map = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};
	unsigned mapWidth = 4,
			 mapHeight = 4;
	Table table(diceTexturePath, diceSize, &map, mapWidth, mapHeight,
		tableRect, tableKeepingDeck);
	table.setStartTablePos(Vector2f(tableRect.getPosition().x + offset, 
			tableRect.getPosition().y + offset));
	table.setStartDeckPos(Vector2f(tableKeepingDeck.getPosition().x + offset,
		tableKeepingDeck.getPosition().y + offset));

	Button rollButton("Roll!", charSize, arialFont,
		AuFunc::round(Vector2f(tableRollBar.getSize().x / 4, tableRollBar.getSize().y / 1.5f)),
		AuFunc::round(Vector2f(
			tableRollBar.getPosition().x + tableRollBar.getSize().x
			/ 2 - tableRollBar.getSize().x / 4 / 2,
			tableRollBar.getPosition().y + tableRollBar.getSize().y
			/ 2 - tableRollBar.getSize().y / 1.5f / 2)));

	Button unkeepButton("Unkeep all!", charSize, arialFont,
		AuFunc::round(Vector2f(rollButton.getRectSize().x / 1.5f, rollButton.getRectSize().y / 2)));
	unkeepButton.setPosition(AuFunc::round(Vector2f(
		(rollButton.getRectPosition().x + rollButton.getRectSize().x * 1.5f),
		(tableRollBar.getPosition().y + tableRollBar.getSize().y
			/ 2 - unkeepButton.getRectSize().y / 2))));

	TextBox currentPlayerTextBox("Player: ", arialFont, charSize,
		AuFunc::round(Vector2f(
			(tableRect.getPosition().x + tableRect.getSize().x / 2 -
				(rollButton.getRectSize().x / 1.5f) / 2),
			(tableRect.getPosition().y + rollButton.getRectSize().y / 4))),
		AuFunc::round(Vector2f(rollButton.getRectSize().x / 1.5f, rollButton.getRectSize().y / 2)));

	Button menuButton("Menu", charSize, arialFont,
		AuFunc::round(Vector2f(rollButton.getRectSize().x / 1.5f, rollButton.getRectSize().y / 2)));
	menuButton.setPosition(AuFunc::round(Vector2f(
		(rollButton.getRectPosition().x - rollButton.getRectSize().x / 6.0f * 7),
		(tableRollBar.getPosition().y +
			tableRollBar.getSize().y / 2 - unkeepButton.getRectSize().y / 2))));
		
	//menu
	GameMenu menu(AuFunc::round(Vector2f(MainUIRect.getPosition().x + MainUIRect.getSize().x +
			thickness + thickness + offset,
			MainUIRect.getPosition().y)),
		AuFunc::round(Vector2f(mainWindow.getSize().x - MainUIRect.getSize().x - 3 * offset - 4 * thickness,
			MainUIRect.getSize().y)), 
			arialFont, charSize, thickness, offset);

	int playerNumber = 1;
	Event event;
	Button* pressedButton = nullptr;
	bool isStartClicked = false;

	bool isAppearanceOn = false;
	bool isDisappearanceOn = false;
	float alpha = 0;

	vector<Player*> playerQueue;
	vector<int> playerDice(5, 0);

	//game variables
	bool isMenuOpen = true;
	int turnCycle = 0;
	int currentPlayerNum = 0;
	int yahtzeeCounter = 0;
	int rollClickedCounter = 0;
	Player *currentPlayer = nullptr;
	int tryCount = 0;
	//Main loop
	while (mainWindow.isOpen())
	{
		while (mainWindow.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				mainWindow.close();
				break;
			case Event::MouseButtonPressed:
				//menu
				if (menuButton.isMouseOver(mainWindow) && isStartClicked)
				{
					isMenuOpen = !isMenuOpen;
					break;
				}
				if(isMenuOpen)
				{
					if ((pressedButton =
						menu.whichButtonMouseOver(mainWindow)) ==
						menu.getButton(GameMenu::NumberOfPlayers))
					{
						// min number of players = 1
						if (event.mouseButton.button == Mouse::Right)
						{
							if (pressedButton->getText().back() > '1')
							{
								string numOfPlayers = pressedButton->getText();
								numOfPlayers.back() -= 1;
								pressedButton->setText(numOfPlayers);
							}
						}
						// max number of players = 4
						// else - event.mouseButton.button == Mouse::Left
						else if (pressedButton->getText().back() < '4')
						{
							string numOfPlayers = pressedButton->getText();
							numOfPlayers.back() += 1;
							pressedButton->setText(numOfPlayers);
						}
						pressedButton->setFillColor(Color::Green);
						playerNumber = stoi(pressedButton->getText());
						break;
					}
					if ((pressedButton =
						menu.whichButtonMouseOver(mainWindow)) ==
						menu.getButton(GameMenu::Start))
					{
						isStartClicked = true;
						scoreboard.newGame();
						table.unkeepAll();
						playerQueue.reserve(4);
						//delete old players
						for (auto& player : playerQueue)
							delete player;
						playerQueue.clear();
						//create new players
						for (int i = 0; i < playerNumber; i++)
							playerQueue.push_back(new Player());
						g = Game();

						turnCycle = 0;
						currentPlayerNum = 0;
						yahtzeeCounter = 0;
						rollClickedCounter = 0;
						//animation current player number 
						isDisappearanceOn = true;
						alpha = 255;

						currentPlayer = playerQueue[currentPlayerNum];
						playerDice = g.showDices(*currentPlayer);

						tryCount = 0;
						while (table.setPlaces(map, playerDice) != 0 || tryCount > 10)
							tryCount++;
						if (tryCount == 10)
						{
							for (auto& elem : playerQueue)
								delete elem;
							mainWindow.close();
							return-1;
						}

						updatePlayerTextBox(currentPlayerTextBox, currentPlayerNum + '1');
						break;
					}
				}
				//game
				else
				{
					if (rollButton.isMouseOver(mainWindow) && rollClickedCounter < 3)
					{
						pressedButton = &rollButton;
						pressedButton->setFillColor(Color::Green);
						g.rollDices(*currentPlayer);
						playerDice = g.showDices(*currentPlayer);

						tryCount = 0;
						while (table.setPlaces(map, playerDice) != 0 || tryCount > 10)
							tryCount++;
						if (tryCount == 10)
						{
							for (auto& elem : playerQueue)
								delete elem;
							mainWindow.close();
							return-1;
						}

						vector<int> scoreTemp;
						scoreTemp.reserve(MAX_COMBO);
						for (int i = 0; i < g.getInvertComboMap().size(); i++)
							scoreTemp.push_back(
								g.getScoreForCombo(*currentPlayer, g.getInvertComboMap().at(i), false));
						scoreboard.updateScoreTips(currentPlayerNum, scoreTemp);
						rollClickedCounter++;
						break;
					}
					if (unkeepButton.isMouseOver(mainWindow))
					{
						pressedButton = &unkeepButton;
						pressedButton->setFillColor(Color::Green);
						g.unkeepAll(*currentPlayer);
						table.unkeepAll();
						break;
					}
					if ((pressedButton =
						table.whichButtonMouseOver(mainWindow)) != nullptr)
					{
						if (rollClickedCounter != 0)
						{
							pressedButton = table.whichButtonMouseOver(mainWindow);
							//pressedButton->setBackColor(Color::Green);
							if (DiceButton* pressedDice = dynamic_cast<DiceButton*>(pressedButton))
								g.keepSwitch(*currentPlayer, std::to_string(pressedDice->getDiceNumber()));
							cout << "num: " << pressedButton->getText() << endl;
							table.keepSwitch(pressedButton);
						}
						break;
					}
					if ((pressedButton =
						scoreboard.whichButtonMouseOver(mainWindow, currentPlayerNum)) != nullptr)
					{
						if (rollClickedCounter != 0)
						{
							pressedButton->setFillColor(Color::Green);

							bool isJoker = false;
							if (g.isYahtzee(*currentPlayer))
							{
								if (yahtzeeCounter > 1)
									isJoker = true;
								yahtzeeCounter++;
							}
							g.takeCombo(*currentPlayer,
								scoreboard.getComboName(currentPlayerNum, pressedButton),
								isJoker);
							scoreboard.takeCombo(currentPlayerNum, pressedButton);

							//bonus check
							if (currentPlayer->isUpperSectionFilled())
								if (scoreboard.bonusCheck(currentPlayerNum))
									scoreboard.setBonus(currentPlayerNum);

							vector<int> scoreTemp;
							scoreTemp.reserve(MAX_COMBO);
							for (int i = 0; i < MAX_COMBO; i++)
								scoreTemp.push_back(0);
							scoreboard.updateScoreTips(
								currentPlayerNum, scoreTemp);
							for (int i = 0; i < playerQueue.size(); i++)
								cout << "Player " << i + 1 << ": " << playerQueue[i]->getScores() << endl;

							//End of turn
							//сделать табличку с надписью текущего игрока//////////////////////////////
							g.unkeepAll(*currentPlayer);
							table.unkeepAll();
							rollClickedCounter = 0;
							if (currentPlayerNum < playerNumber - 1)
								currentPlayer = playerQueue[++currentPlayerNum];
							else
							{
								currentPlayerNum = 0;
								currentPlayer = playerQueue[currentPlayerNum];
								turnCycle++;
							}
							updatePlayerTextBox(currentPlayerTextBox, currentPlayerNum + '1');
							//animation of current player textbox
							isAppearanceOn = true;
							alpha = 0;

							rollButton.setFillColor(Color{ 255,255,255 });
							rollButton.setTextColor(Color{ 0,0,0 });
						}
						break;
					}
				}
				break;

			case Event::MouseButtonReleased:
				if (pressedButton == &rollButton)
				{
					if (rollClickedCounter == 3)
					{
						pressedButton->setFillColor(Color{ 255,255,255,127 });
						pressedButton->setTextColor(Color{ 0,0,0,127 });
						rollClickedCounter++;
						break;
					}
					else if(rollClickedCounter > 0 && rollClickedCounter < 3)
					{
						// take combo immediately if the number you got yahtzees with 
						// has not been filled out in the upper section
						if (g.isYahtzee(*currentPlayer) && yahtzeeCounter > 1 &&
							g.isValidCombo(*currentPlayer, g.getComboName(currentPlayer->getDices()[0].getTop())))
						{
							g.takeCombo(*currentPlayer, g.getComboName(currentPlayer->getDices()[0].getTop()));
							cout << "Upper Section Filled!" << endl;
						}
					}
					pressedButton->setFillColor(Color::White);
					break;
				}
				if (pressedButton != nullptr)
					if(pressedButton->isOn())
						pressedButton->setFillColor(Color::Transparent);
				break;
			}
		}
		if (turnCycle == 13)
		{
			scoreboard.setWinner();
			//scoreboard.setWinner(g.whoWins(playerQueue);
		}
		//animation of current player textbox
		if (isAppearanceOn && alpha < 255)
			alpha += 0.1f;
		else if (isDisappearanceOn && alpha > 0)
			alpha -= 0.1f;
		currentPlayerTextBox.setAlpha(static_cast<int>(alpha));
		if (alpha >= 255)
		{
			isAppearanceOn = false;
			isDisappearanceOn = true;
		}
		if(alpha <= 0)
			isDisappearanceOn = false;

			//mainWindow.close();
		mainWindow.clear(Color::White);
		mainWindow.draw(MainUIRect);
		mainWindow.draw(tableRect);
		mainWindow.draw(tableRollBar);
		mainWindow.draw(tableKeepingDeck);
		mainWindow.draw(table);
		mainWindow.draw(rollButton);
		mainWindow.draw(unkeepButton);
		mainWindow.draw(currentPlayerTextBox);
		mainWindow.draw(menuButton);

		//menu of scoreboard
		if (isMenuOpen)
			mainWindow.draw(menu);
		else
			mainWindow.draw(scoreboard);

		mainWindow.display();
	}
	
	for (auto& elem : playerQueue)
		delete elem;

	return 0;
}