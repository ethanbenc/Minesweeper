#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Random.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void createMineField(vector<vector<Tile>>& MineField, int& width, int& height, int& numMines, int& tileSize) {
    // read variables and set configurations
    ifstream config("boards/config.cfg");
    if (config.is_open()) {
        string line;
        getline(config, line);
        width = stoi(line);
        getline(config, line);
        height = stoi(line);
        getline(config, line);
        numMines = stoi(line);
    }
    config.close();
    // create a new "MineField"
    MineField.clear();
    for (int i = 0; i < height; i++) {
        vector<Tile> tempVector;
        for (int j = 0; j < width; j++) {
            Tile tempTile = Tile(i * tileSize, j * tileSize);
            tempVector.push_back(tempTile);
        }
        MineField.push_back(tempVector);
    }
    // randomize mines
    int counter = 0;
    while (counter < numMines) {
        int randomX = Random::Int(0, width - 1);
        int randomY = Random::Int(0, height - 1);
        if (!MineField[randomY][randomX].getMineState()) {
            MineField[randomY][randomX].setMine();
            counter++;
        }
    }
    /*
    //test print field of mines, 1 representing a mine, 0 representing no mine
    cout << "Mines: " << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << MineField[i][j].getMineState() << " ";
        }
        cout << endl;
    } */

    // test print out tiles with the surrounding number of mines
    //cout << "\nNumber of Mines: " << endl;
    // counts number of mines around tile and assigns it to tile
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            MineField[i][j].countNumMines(MineField, i, j);
            //cout << MineField[i][j].getNumMines() << " ";
        }
        //cout << endl;
    }
}

// draws out the UI and positions it by int position times number of tiles (32 width) away from the center.
void drawUI(sf::RenderWindow& window, sf::Sprite& sprite, int& width, int& height, int& tileSize, int position = 0) {
    sprite.setOrigin(32.0, 32.0);
    sprite.setPosition(float(width * tileSize)/2.0f + float(tileSize * position), float(height * tileSize) + float(tileSize));
    window.draw(sprite);
}

void loadTestBoards(vector<vector<Tile>>& MineField, int& width, int& height, int& mineCount, const string& fileName) {
    MineField.clear();
    ifstream testBoard(fileName);
    string line;
    vector<vector<string>> temp2Vector; // stores values of 0's and 1's to later be read into MineField;
    if (testBoard.is_open()) {
        while(getline(testBoard, line)) {
            vector<string> temp;
            for (int i = 0; i < line.length(); i++) {
                temp.push_back(line.substr(i, 1));  // pushes back one character (in this case number) at a time into the vector
            }
            temp2Vector.push_back(temp);     // pushes entire line of split up characters into vector
        }
    }
    testBoard.close();
    // initialize MineField
    for (int i = 0; i < temp2Vector.size(); i++) {
        vector<Tile> tempVector;
        for (int j = 0; j < temp2Vector[0].size(); j++) {
            Tile tempTile = Tile(i * 32, j * 32);
            tempVector.push_back(tempTile);
        }
        MineField.push_back(tempVector);
    }
    // if  value of test board is 1, set equivalent location of MineField to have a mine
    mineCount = 0;
    for (int i = 0; i < temp2Vector.size(); i++) {
        for (int j = 0; j < temp2Vector[0].size(); j++) {
            if (temp2Vector[i][j] == "1") {
                MineField[i][j].setMine();
                mineCount++;
            }
        }
    }
    // counts number of mines around tile and assigns it to tile
    for (int i = 0; i < MineField.size(); i++) {
        for (int j = 0; j < MineField[0].size(); j++) {
            MineField[i][j].countNumMines(MineField, i, j);
        }
    }
}

int main()
{
    // Initialize configuration variables of Minesweeper Board
    int tileSize = 32;
    int windowWidth, windowHeight, mineCount = 0;
    // load sprites
    sf::Sprite debug(TextureManager::getTexture("debug"));
    //sf::Sprite digits(TextureManager::getTexture("digits"));      // not needed as digits.png will need to be cropped to isolate individual numbers
    sf::Sprite face_happy(TextureManager::getTexture("face_happy"));
    sf::Sprite face_lose(TextureManager::getTexture("face_lose"));
    sf::Sprite face_win(TextureManager::getTexture("face_win"));
    sf::Sprite flag(TextureManager::getTexture("flag"));
    sf::Sprite mine(TextureManager::getTexture("mine"));
    sf::Sprite number_1(TextureManager::getTexture("number_1"));
    sf::Sprite number_2(TextureManager::getTexture("number_2"));
    sf::Sprite number_3(TextureManager::getTexture("number_3"));
    sf::Sprite number_4(TextureManager::getTexture("number_4"));
    sf::Sprite number_5(TextureManager::getTexture("number_5"));
    sf::Sprite number_6(TextureManager::getTexture("number_6"));
    sf::Sprite number_7(TextureManager::getTexture("number_7"));
    sf::Sprite number_8(TextureManager::getTexture("number_8"));
    sf::Sprite test_1(TextureManager::getTexture("test_1"));
    sf::Sprite test_2(TextureManager::getTexture("test_2"));
    sf::Sprite test_3(TextureManager::getTexture("test_3"));
    sf::Sprite tile_hidden(TextureManager::getTexture("tile_hidden"));
    sf::Sprite tile_revealed(TextureManager::getTexture("tile_revealed"));

    // Initialize Board Values
    vector<vector<Tile>>MineField;
    createMineField(MineField, windowWidth, windowHeight, mineCount, tileSize);
    sf::RenderWindow window(sf::VideoMode(windowWidth * tileSize, (windowHeight * tileSize) + 100), "Minesweeper");

    //create variables
    bool revealMines, gameOver, gameWon, gameLost = false;

    while (window.isOpen())
    {
        int numTilesRevealed = 0;
        int minesLeft = mineCount;
        sf::Event event;
        window.clear();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                // inputs of mouse coords, convert into tile coordinates
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                int xValue = (mouseX) / tileSize;
                int yValue = (mouseY) / tileSize;
                if (event.mouseButton.button == sf::Mouse::Right) {
                    //cout << "Right: " << mouseX << " " << mouseY << endl;
                    // when a tile is right-clicked, flag it. If a flag is right-clicked, remove flag
                    if (!gameOver && mouseX < (tileSize * windowWidth) && mouseY < (tileSize * windowHeight) && !MineField[yValue][xValue].getRevealedState()) {
                        MineField[yValue][xValue].flag();
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Left) {
                    //cout << "Left: " << mouseX << " " << mouseY << endl;
                    // can't reveal tile with flag
                    if (mouseX < (tileSize * windowWidth) && mouseY < (tileSize * windowHeight) && MineField[yValue][xValue].getFlagState()) {
                        break;
                    }
                    // reveal tile, if tile has no nearby mines, reveal all other nearby tiles
                    if (!gameOver && mouseX < (tileSize * windowWidth) && mouseY < (tileSize * windowHeight) && !MineField[yValue][xValue].getRevealedState()) {
                        MineField[yValue][xValue].openNearbyTiles(MineField, yValue, xValue);
                        //MineField[yValue][xValue].setRevealed();
                        if (MineField[yValue][xValue].getMineState()) {
                            gameOver = true;
                            gameLost = true;
                            revealMines = true;
                        }
                    }
                    // smiley face is clicked, create new field/restart game
                    else if (mouseX <= ((16 * windowWidth) + 32) && mouseX >= ((16 * windowWidth) - 32) && mouseY >= (tileSize * windowHeight) && mouseY <= ((tileSize * windowHeight) + 64)) {
                        createMineField(MineField, windowWidth, windowHeight, mineCount, tileSize);
                        revealMines = false;
                        gameOver = false;
                        gameLost = false;
                    }
                    // debug/bomb icon is clicked, reveal bombs
                    else if (!gameOver && mouseX <= ((16 * windowWidth) + 32 * 5) && mouseX >= ((16 * windowWidth) + 32 * 3) && mouseY >= (tileSize * windowHeight) && mouseY <= ((tileSize * windowHeight) + 64)) {
                        revealMines = !revealMines;
                    }
                    // test1 icon clicked, set board to board1
                    else if (mouseX <= ((16 * windowWidth) + 32 * 7) && mouseX >= ((16 * windowWidth) + 32 * 5) && mouseY >= (tileSize * windowHeight) && mouseY <= ((tileSize * windowHeight) + 64)) {
                        loadTestBoards(MineField, windowWidth, windowHeight, mineCount, "boards/testboard1.brd");
                        revealMines = true;
                        gameOver = false;
                        gameLost = false;
                    }
                    // test2 icon clicked, set board to board2
                    else if (mouseX <= ((16 * windowWidth) + 32 * 9) && mouseX >= ((16 * windowWidth) + 32 * 7) && mouseY >= (tileSize * windowHeight) && mouseY <= ((tileSize * windowHeight) + 64)) {
                        loadTestBoards(MineField, windowWidth, windowHeight, mineCount, "boards/testboard2.brd");
                        revealMines = true;
                        gameOver = false;
                        gameLost = false;
                    }
                    // test3 icon clicked, set board to board3
                    else if (mouseX <= ((16 * windowWidth) + 32 * 11) && mouseX >= ((16 * windowWidth) + 32 * 9) && mouseY >= (tileSize * windowHeight) && mouseY <= ((tileSize * windowHeight) + 64)) {
                        loadTestBoards(MineField, windowWidth, windowHeight, mineCount, "boards/testboard3.brd");
                        revealMines = true;
                        gameOver = false;
                        gameLost = false;
                    }
                }
            }
        }
        // draw board
        // bug when pressing bomb as final tile revealed it will count as a win ::: draw face_lose needs to be called when game has not been won, game has been lost, and is over
        // bug when winning and must click reset button twice in order for it to return to happy face ::: draw board needs to be after poll event
        // test case 1, doesn't load as a win but as a loss ::: initialization of variables numFlagged and numTilesRevealed must be before pollEvent
        int numFlagged = 0; // bug where numFlagged kept increasing, fixed by placing it here instead of before poll event
        for (int i = 0; i < windowHeight; i++) {
            for (int j = 0; j < windowWidth; j++) {
                int x = MineField[i][j].getXCoord();
                int y = MineField[i][j].getYCoord();
                // set every tile in the board initially to tile_hidden
                tile_hidden.setPosition(x, y);
                window.draw(tile_hidden);
                // if revealMines is set to true from debug button or when game is lost, reveal all the mines
                if (revealMines) {
                    if (MineField[i][j].getMineState()) {
                        mine.setPosition(x, y);
                        window.draw(mine);
                    }
                }
                // display if a tile has a flag
                if (MineField[i][j].getFlagState()) {
                    flag.setPosition(x, y);
                    window.draw(flag);
                    numFlagged++;
                }
                // if a tile has been left-clicked, reveal the tile. Increase numTilesRevealed
                if (MineField[i][j].getRevealedState()) {
                    tile_revealed.setPosition(x, y);
                    window.draw(tile_revealed);
                    numTilesRevealed++;

                    if (MineField[i][j].getMineState()) {
                        mine.setPosition(x, y);
                        window.draw(mine);
                    }
                    // display how many mines are around the tile when revealed
                    int tileNumMines = MineField[i][j].getNumMines();
                    if (tileNumMines == 0) {
                        MineField[i][j].openNearbyTiles(MineField, i, j);
                    }
                    else if (tileNumMines == 1) {
                        number_1.setPosition(x, y);
                        window.draw(number_1);
                    }
                    else if (tileNumMines == 2) {
                        number_2.setPosition(x, y);
                        window.draw(number_2);
                    }
                    else if (tileNumMines == 3) {
                        number_3.setPosition(x, y);
                        window.draw(number_3);
                    }
                    else if (tileNumMines == 4) {
                        number_4.setPosition(x, y);
                        window.draw(number_4);
                    }
                    else if (tileNumMines == 5) {
                        number_5.setPosition(x, y);
                        window.draw(number_5);
                    }
                    else if (tileNumMines == 6) {
                        number_6.setPosition(x, y);
                        window.draw(number_6);
                    }
                    else if (tileNumMines == 7) {
                        number_7.setPosition(x, y);
                        window.draw(number_7);
                    }
                    else if (tileNumMines == 8) {
                        number_8.setPosition(x, y);
                        window.draw(number_8);
                    }
                }
            }
        }
        // when total tiles - mineCount is equal to number of tiles revealed, game is won (no mines were revealed)
        if (!gameLost && ((windowWidth * windowHeight) - mineCount) == numTilesRevealed)
            gameWon = true;
        else
            gameWon = false;
        // when game is won, display face_win
        if (gameWon) {
            drawUI(window, face_win, windowWidth, windowHeight, tileSize);
            gameOver = true;
        }

        // when game is lost, display face_lose
        if (gameOver && gameLost) {
            drawUI(window, face_lose, windowWidth, windowHeight, tileSize);
        }  // when game is neither lost, won, nor over, continue to display face_happy
        else if (!gameWon && !gameLost && !gameOver){
            drawUI(window, face_happy, windowWidth, windowHeight, tileSize);
        }


        // draw UI: debug, test_1, test_2, and test_3 icons
        drawUI(window, debug, windowWidth, windowHeight, tileSize, 4);
        drawUI(window, test_1, windowWidth, windowHeight, tileSize, 6);
        drawUI(window, test_2, windowWidth, windowHeight, tileSize, 8);
        drawUI(window, test_3, windowWidth, windowHeight, tileSize, 10);

        // display how many minesLeft after flagging
        minesLeft -= numFlagged;
        // crops '-' sign and displays when numFlagged exceeds number of mines
        if (minesLeft < 0) {
            sf::Texture digitsTexture;
            digitsTexture.loadFromFile("images/digits.png", sf::IntRect(210, 0, 21, 32));
            sf::Sprite digits;
            digits.setTexture(digitsTexture);
            digits.setPosition(0, windowHeight * 32);
            window.draw(digits);
        }
        // display hundreds place
        int hundreds = abs(minesLeft) / 100;
        sf::Texture digitsTexture;
        digitsTexture.loadFromFile("images/digits.png", sf::IntRect(21 * hundreds, 0, 21, 32));
        sf::Sprite digits;
        digits.setTexture(digitsTexture);
        digits.setPosition(21, windowHeight * 32);
        window.draw(digits);
        // display tens place
        int tens = (abs(minesLeft) % 100) / 10;
        digitsTexture.loadFromFile("images/digits.png", sf::IntRect(21 * tens, 0, 21, 32));
        digits.setTexture(digitsTexture);
        digits.setPosition(42, windowHeight * 32);
        window.draw(digits);
        // display ones place
        int ones = abs(minesLeft) % 10;
        digitsTexture.loadFromFile("images/digits.png", sf::IntRect(21 * ones, 0, 21, 32));
        digits.setTexture(digitsTexture);
        digits.setPosition(64, windowHeight * 32);
        window.draw(digits);

        window.display();
    }
    TextureManager::Clear();
    return 0;
}
