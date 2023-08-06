#include <SFML/Graphics.hpp>
#include <fstream>;
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
//#include "leaker.h"
using namespace std;


struct Tile
{
    sf::Sprite sprite;
    bool bomb = false;
    bool revealed = false;
    bool flagged = false;
    Tile(sf::Sprite sprite)
    {
        this->sprite = sprite;

    }

    ~Tile()
    {

    }
};
class Board
{
public:
    int col = 0;
    int row = 0;
    int numMines = 0;
    int bombs[1][1];
    vector<Tile*> tiles;
    sf::Texture debug;


    sf::Texture digit1;
    sf::Texture digit2;
    sf::Texture digit3;
    sf::Texture digit4;
    sf::Texture digit5;
    sf::Texture digit6;
    sf::Texture digit7;
    sf::Texture digit8;
    sf::Texture digit9;
    sf::Texture digit0;
    sf::Texture minus;


    sf::Texture happyFace;


    sf::Texture loseFace;


    sf::Texture winFace;


    sf::Texture flag;
    sf::Texture mine;
    sf::Texture number1;
    sf::Texture number2;
    sf::Texture number3;
    sf::Texture number4;
    sf::Texture number5;
    sf::Texture number6;
    sf::Texture number7;
    sf::Texture number8;
    sf::Texture test1;
    sf::Texture test2;
    sf::Texture test3;
    sf::Texture hiddenTile;
    sf::Texture revealedTile;
    vector<vector<Tile*>> adjacentMines;
    int mineCounter;
    bool gameover = false;
    bool gamewon = false;
    vector<Tile*> sped;
    Board(int col, int row)
    {
        this->col = col;
        this->row = row;
        ifstream file;
        file.open("boards/config.cfg");
        string line;
        (getline(file, line));
        col = stoi(line);
        (getline(file, line));
        row = stoi(line);
        (getline(file, line));
        numMines = stoi(line);
        mineCounter = numMines;
        bombs[col][row];
        debug.loadFromFile("images/debug.png");
        digit0.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
        digit1.loadFromFile("images/digits.png", sf::IntRect(21, 0, 21, 32));
        digit2.loadFromFile("images/digits.png", sf::IntRect(42, 0, 21, 32));
        digit3.loadFromFile("images/digits.png", sf::IntRect(63, 0, 21, 32));
        digit4.loadFromFile("images/digits.png", sf::IntRect(84, 0, 21, 32));
        digit5.loadFromFile("images/digits.png", sf::IntRect(105, 0, 21, 32));
        digit6.loadFromFile("images/digits.png", sf::IntRect(126, 0, 21, 32));
        digit7.loadFromFile("images/digits.png", sf::IntRect(147, 0, 21, 32));
        digit8.loadFromFile("images/digits.png", sf::IntRect(168, 0, 21, 32));
        digit9.loadFromFile("images/digits.png", sf::IntRect(189, 0, 21, 32));
        minus.loadFromFile("images/digits.png", sf::IntRect(210, 0, 21, 32));
        happyFace.loadFromFile("images/face_happy.png");
        loseFace.loadFromFile("images/face_lose.png");
        winFace.loadFromFile("images/face_win.png");
        flag.loadFromFile("images/flag.png");
        mine.loadFromFile("images/mine.png");
        number1.loadFromFile("images/number_1.png");
        number2.loadFromFile("images/number_2.png");
        number3.loadFromFile("images/number_3.png");
        number4.loadFromFile("images/number_4.png");
        number5.loadFromFile("images/number_5.png");
        number6.loadFromFile("images/number_6.png");
        number7.loadFromFile("images/number_7.png");
        number8.loadFromFile("images/number_8.png");
        test1.loadFromFile("images/test_1.png");
        test2.loadFromFile("images/test_2.png");
        test3.loadFromFile("images/test_3.png");
        hiddenTile.loadFromFile("images/tile_hidden.png");
        revealedTile.loadFromFile("images/tile_revealed.png");


    }
    void calculateNeighbors()
    {
        adjacentMines.clear();
        for (int i = 0; i < tiles.size(); ++i)
        {
            int count = 0;
            vector<Tile*> neighborBombs;
            if (i - 1 >= 0 && i % col != 0 && (*(tiles.at(i - 1))).bomb)
            {
                neighborBombs.push_back(tiles.at(i - 1));
            }
            if (i + 1 < col * row && i % col != (col - 1) && (*(tiles.at(i + 1))).bomb)
            {
                neighborBombs.push_back(tiles.at(i + 1));
            }
            if ((i - col >= 0) && (*(tiles.at(i - col))).bomb)
            {
                neighborBombs.push_back(tiles.at(i - col));
            }
            if ((i + col < col * row) && (*(tiles.at(i + col))).bomb)
            {
                neighborBombs.push_back(tiles.at(i + col));
            }
            if (i - col - 1 >= 0 && i % col != 0 && (*(tiles.at(i - col - 1))).bomb)
            {
                neighborBombs.push_back(tiles.at(i - col - 1));
            }
            if (i - col + 1 >= 0 && i % col != (col - 1) && (*(tiles.at(i - col + 1))).bomb)
            {
                neighborBombs.push_back(tiles.at(i - col + 1));
            }
            if (i + col + 1 < col * row && i % col != (col - 1) && (*(tiles.at(i + col + 1))).bomb)
            {
                neighborBombs.push_back(tiles.at(i + col + 1));
            }
            if (i + col - 1 < col * row && i % col != 0 && (*(tiles.at(i + col - 1))).bomb)
            {
                neighborBombs.push_back(tiles.at(i + col - 1));
            }
            adjacentMines.push_back(neighborBombs);
        }
    }
    void reset()
    {
        adjacentMines.clear();
        mineCounter = numMines;
        gamewon = false;
        gameover = false;
        for (int i = 0; i < tiles.size(); ++i)
        {
            (*(tiles.at(i))).bomb = false;
            (*(tiles.at(i))).flagged = false;
            (*(tiles.at(i))).revealed = false;
            (*(tiles.at(i))).sprite.setTexture(hiddenTile);
        }
    }

    void readBoard(string fileName)
    {
        //RESET
        reset();
        ifstream file;
        string line;
        file.close();
        file.open("boards/" + fileName);
        int count = 0;
        while (getline(file, line))
        {
            for (int i = 0; i < line.length(); ++i)
            {
                char c = line[i];
                if (c == '1')
                {
                    (*tiles.at(count)).bomb = true;
                }
                ++count;
            }
        }
        calculateNeighbors();
    }

    mt19937 random_mt;

    int Random(int min, int max)
    {
        uniform_int_distribution<int> dist(min, max);
        return dist(random_mt);
    }

    void random()
    {
        reset();
        ifstream file;
        file.open("boards/config.cfg");
        string line;
        (getline(file, line));
        col = stoi(line);
        (getline(file, line));
        row = stoi(line);
        (getline(file, line));
        numMines = stoi(line);
        mineCounter = numMines;
        for (int i = 0; i < numMines; ++i)
        {
            int index = Random(0, tiles.size());
            while (tiles.at(index)->bomb)
            {
                index = Random(0, tiles.size());
            }
            tiles.at(index)->bomb = true;
        }
        calculateNeighbors();
    }
    bool ShowYourself(int index)
    {
        Tile* tile = tiles.at(index);
        if ((*tile).revealed || (*tile).flagged)
            return true;
        (*tile).revealed = true;
        tile->sprite.setTexture(revealedTile);
        sped.push_back(tile);




        if ((*tile).bomb)
        {
            (*tile).sprite.setTexture(mine);
            return false;
        }
        int size = adjacentMines.at(index).size();
        if (size == 1)
        {
            (*tile).sprite.setTexture(number1);
        }
        else if (size == 2)
        {
            (*tile).sprite.setTexture(number2);
        }
        else if (size == 3)
        {
            (*tile).sprite.setTexture(number3);
        }
        else if (size == 4)
        {
            (*tile).sprite.setTexture(number4);
        }
        else if (size == 5)
        {
            (*tile).sprite.setTexture(number5);
        }
        else if (size == 6)
        {
            (*tile).sprite.setTexture(number6);
        }
        else if (size == 7)
        {
            (*tile).sprite.setTexture(number7);
        }
        else if (size == 8)
        {
            (*tile).sprite.setTexture(number8);
        }
        else if (size == 0)
        {
            if (index - 1 >= 0 && index % col != 0)
            {
                ShowYourself(index - 1);
            }
            if (index + 1 < col * row && index % col != (col - 1))
            {
                ShowYourself(index + 1);
            }
            if (index - col >= 0)
            {
                ShowYourself(index - col);
            }
            if (index + col < col * row)
            {
                ShowYourself(index + col);
            }
            if (index - col - 1 >= 0 && index % col != 0)
            {
                ShowYourself(index - col - 1);
            }
            if (index - col + 1 >= 0 && index % col != (col - 1))
            {
                ShowYourself(index - col + 1);
            }
            if (index + col + 1 < col * row && index % col != (col - 1))
            {
                ShowYourself(index + col + 1);
            }
            if (index + col - 1 < col * row && index % col != 0)
            {
                ShowYourself(index + col - 1);
            }
        }



        return true;
    }
    void play()
    {

        sf::RenderWindow window(sf::VideoMode(col * 32, row * 32 + 100), "Minesweeper");

        bool create = true;

        sf::Style::Close;
        while (window.isOpen())
        {
            if (create)
            {
                //window.clear(sf::Color::White);
                create = false;

                for (int i = 0; i < 32 * row; i += 32)
                {
                    for (int j = 0; j < 32 * col; j += 32)
                    {
                        sf::Sprite tile(hiddenTile);
                        tile.setPosition(sf::Vector2f(j, i));
                        Tile* newTile = new Tile(tile);
                        tiles.push_back(newTile);
                    }
                }

                //readBoard("testboard1.brd");   //IMPORTANT RANDOMIZE
                random();

            }



            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    
                    window.close();
                }
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    if (!gameover && position.y < row * 32)
                    {
                        if (!ShowYourself(col * (position.y / 32) + (position.x / 32)))
                        {
                            for (int i = 0; i < tiles.size(); ++i)
                            {
                                if (tiles.at(i)->bomb)
                                {
                                    tiles.at(i)->sprite.setTexture(mine);
                                    sped.push_back(tiles.at(i));
                                }
                            }
                            gameover = true;
                            break;
                        }
                        else
                        {
                            int count = 0;
                            for (int i = 0; i < tiles.size(); ++i)
                            {
                                if (!tiles.at(i)->bomb && tiles.at(i)->revealed)
                                    ++count;
                            }
                            if (count == row * col - numMines)
                            {
                                gamewon = true;
                                for (int i = 0; i < tiles.size(); ++i)
                                {
                                    if (tiles.at(i)->bomb)
                                    {
                                        tiles.at(i)->sprite.setTexture(flag);
                                        --mineCounter;
                                    }
                                }
                            }
                        }

                    }
                    else
                    {
                        if (position.y < row * 32 + 64 && position.y > row * 32 && position.x > col * 32 - 320 && position.x < col * 32 - 256)
                        {
                            for (int i = 0; i < tiles.size(); ++i)
                            {
                                Tile* tile = tiles.at(i);
                                if ((*tile).bomb)
                                {
                                    (*tile).sprite.setTexture(mine);
                                }


                            }
                        }
                        else if (position.y < row * 32 + 64 && position.y > row * 32 && position.x > col * 32 - 192 && position.x < col * 32 - 128)
                        {
                            readBoard("testboard1.brd");
                        }
                        else if (position.y < row * 32 + 64 && position.y > row * 32 && position.x > col * 32 - 128 && position.x < col * 32 - 64)
                        {
                            readBoard("testboard2.brd");
                        }
                        else if (position.y < row * 32 + 64 && position.y > row * 32 && position.x > col * 32 - 64 && position.x < col * 32 - 0)
                        {
                            readBoard("testboard3.brd");
                        }
                        else if (position.y < row * 32 + 64 && position.y > row * 32 && position.x > col * 32 - 448 && position.x < col * 32 - 384)
                        {
                            random();
                        }
                    }
                }
                if (!gameover && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                {
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    if (position.y < row * 32)
                    {
                        Tile* clickedTile = tiles.at(col * (position.y / 32) + (position.x / 32));
                        if ((*clickedTile).flagged)
                        {
                            (*clickedTile).sprite.setTexture(hiddenTile);
                            (*clickedTile).flagged = false;
                            ++mineCounter;
                        }
                        else if ((*clickedTile).revealed == false)
                        {
                            (*clickedTile).sprite.setTexture(flag);
                            (*clickedTile).flagged = true;
                            --mineCounter;
                        }
                    }


                }
            }
            for (int i = 0; i < sped.size(); ++i)
            {
                if (!sped.at(i)->flagged)
                {
                    sf::Sprite newTile(revealedTile);
                    newTile.setPosition(sped.at(i)->sprite.getPosition());
                    window.draw(newTile);
                }
            }
            for (int i = 0; i < tiles.size(); ++i)
            {
                sf::Sprite sprite = (*(tiles.at(i))).sprite;
                window.draw(sprite);
            }



            sf::Sprite smile;
            if (gameover)
            {
                smile.setTexture(loseFace);
                smile.setPosition(sf::Vector2f(col * 32 - 448, row * 32));
                window.draw(smile);
            }
            else if (gamewon)
            {
                smile.setTexture(winFace);
                smile.setPosition(sf::Vector2f(col * 32 - 448, row * 32));
                window.draw(smile);
            }
            else
            {
                smile.setTexture(happyFace);
                smile.setPosition(sf::Vector2f(col * 32 - 448, row * 32));
                window.draw(smile);
            }

            sf::Sprite test_3;
            test_3.setTexture(test3);
            test_3.setPosition(sf::Vector2f(col * 32 - 64, row * 32));
            window.draw(test_3);

            sf::Sprite test_2;
            test_2.setTexture(test2);
            test_2.setPosition(sf::Vector2f(col * 32 - 128, row * 32));
            window.draw(test_2);

            sf::Sprite test_1;
            test_1.setTexture(test1);
            test_1.setPosition(sf::Vector2f(col * 32 - 192, row * 32));
            window.draw(test_1);

            sf::Sprite debugButton;
            debugButton.setTexture(debug);
            debugButton.setPosition(sf::Vector2f(col * 32 - 320, row * 32));
            window.draw(debugButton);

            bool thing = false;
            if (mineCounter < 0)
            {
                sf::Sprite sign;
                sign.setTexture(minus);
                sign.setPosition(sf::Vector2f(0, row * 32));
                window.draw(sign);
                thing = true;
                mineCounter *= -1;
            }

            sf::Sprite firstDigit;
            if (mineCounter / 100 == 0)
                firstDigit.setTexture(digit0);
            else if (mineCounter / 100 == 1)
                firstDigit.setTexture(digit1);
            else if (mineCounter / 100 == 2)
                firstDigit.setTexture(digit2);
            else if (mineCounter / 100 == 3)
                firstDigit.setTexture(digit3);
            else if (mineCounter / 100 == 4)
                firstDigit.setTexture(digit4);
            else if (mineCounter / 100 == 5)
                firstDigit.setTexture(digit5);
            else if (mineCounter / 100 == 6)
                firstDigit.setTexture(digit6);
            else if (mineCounter / 100 == 7)
                firstDigit.setTexture(digit7);
            else if (mineCounter / 100 == 8)
                firstDigit.setTexture(digit8);
            else if (mineCounter / 100 == 9)
                firstDigit.setTexture(digit9);
            firstDigit.setPosition(sf::Vector2f(21, row * 32));
            window.draw(firstDigit);

            sf::Sprite secondDigit;
            if ((mineCounter / 10) % 10 == 0)
                secondDigit.setTexture(digit0);
            else if ((mineCounter / 10) % 10 == 1)
                secondDigit.setTexture(digit1);
            else if ((mineCounter / 10) % 10 == 2)
                secondDigit.setTexture(digit2);
            else if ((mineCounter / 10) % 10 == 3)
                secondDigit.setTexture(digit3);
            else if ((mineCounter / 10) % 10 == 4)
                secondDigit.setTexture(digit4);
            else if ((mineCounter / 10) % 10 == 5)
                secondDigit.setTexture(digit5);
            else if ((mineCounter / 10) % 10 == 6)
                secondDigit.setTexture(digit6);
            else if ((mineCounter / 10) % 10 == 7)
                secondDigit.setTexture(digit7);
            else if ((mineCounter / 10) % 10 == 8)
                secondDigit.setTexture(digit8);
            else if ((mineCounter / 10) % 10 == 9)
                secondDigit.setTexture(digit9);
            secondDigit.setPosition(sf::Vector2f(42, row * 32));
            window.draw(secondDigit);

            sf::Sprite thirdDigit;
            if (mineCounter % 10 == 0)
                thirdDigit.setTexture(digit0);
            else if (mineCounter % 10 == 1)
                thirdDigit.setTexture(digit1);
            else if (mineCounter % 10 == 2)
                thirdDigit.setTexture(digit2);
            else if (mineCounter % 10 == 3)
                thirdDigit.setTexture(digit3);
            else if (mineCounter % 10 == 4)
                thirdDigit.setTexture(digit4);
            else if (mineCounter % 10 == 5)
                thirdDigit.setTexture(digit5);
            else if (mineCounter % 10 == 6)
                thirdDigit.setTexture(digit6);
            else if (mineCounter % 10 == 7)
                thirdDigit.setTexture(digit7);
            else if (mineCounter % 10 == 8)
                thirdDigit.setTexture(digit8);
            else if (mineCounter % 10 == 9)
                thirdDigit.setTexture(digit9);
            thirdDigit.setPosition(sf::Vector2f(63, row * 32));
            window.draw(thirdDigit);

            if (thing)
                mineCounter *= -1;

            window.display();
        }
    }

    ~Board()
    {
        reset();
        for (int i = 0; i < tiles.size(); ++i)
        {
            if (tiles.at(i) != nullptr)
            {
                delete tiles.at(i);
            }
        }
    }
};
int main()
{
    Board board(25, 16);
    board.play();
    return 0;
}