#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include <chrono>
#include <thread>
#include <functional>
#include <fstream>

class tile {
public:
    sf::Sprite graphic;
    bool hasMine;
    int nearMine = 0;
    sf::Texture emptyTexture;
    sf::Texture flagTexture;
    sf::Texture overlayGraphic;
    bool flagged = false;
    bool numberDisplayed = false;
    bool ranThrough = false;
    tile(sf::Sprite* graphic, bool hasMine){
        this->graphic = *graphic;
        this->hasMine = hasMine;
        if(hasMine)
            overlayGraphic.loadFromFile("images/mine.png");
        else
            overlayGraphic.loadFromFile("images/tile_revealed.png");
        emptyTexture.loadFromFile("images/tile_revealed.png");
        flagTexture.loadFromFile("images/flag.png");
    }
    void addMineNumber(){
        nearMine ++;
        if(!hasMine){
            if(nearMine > 0){
                overlayGraphic.loadFromFile("images/number_" +  std::to_string(nearMine) + ".png");
            }else{
                overlayGraphic.loadFromFile("images/tile_revealed.png");
            }
            
        }
        else{
            overlayGraphic.loadFromFile("images/mine.png");
        }
    }
};

void drawGame(double width, double height, sf::RenderWindow* window, int rows, int columns, int mines, std::string name);
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
int readConfigFile(int valueToRead){
    std::ifstream inFile("config.cfg"); // Open the file for reading
    if (!inFile) {
        std::cerr << "Error opening file for reading." << std::endl;
    }
    
    std::string content;
    int lineIndex = 0;
    while (std::getline(inFile, content)) { // Read line by line
        if(valueToRead == lineIndex)
        {
            inFile.close();
            return stoi(content);
        }
            
        lineIndex++;
    }
    inFile.close();
    return -1;
}
void drawTitle(double width, double height, sf::RenderWindow* window, std::string inputString){
    window->clear(sf::Color::Blue);
    
    // Declare and load a font
    sf::Font font;
    font.loadFromFile("font.ttf");
    
    // Create a text
    sf::Text title("WELCOME TO MINESWEEPER!", font);
    title.setCharacterSize(24);
    title.setStyle(sf::Text::Underlined | sf::Text::Bold);
    title.setFillColor(sf::Color::White);
    setText(title, width/2, height/2-150);
    sf::Text nameTitle("Enter your name:", font);
    nameTitle.setCharacterSize(20);
    nameTitle.setStyle(sf::Text::Bold);
    nameTitle.setFillColor(sf::Color::White);
    setText(nameTitle, width/2, height/2-75);
    // Draw it
    window->draw(title);
    window->draw(nameTitle);
    //user text:
    sf::Text userText(inputString + "|", font);
    userText.setCharacterSize(18);
    userText.setStyle(sf::Text::Bold);
    userText.setFillColor(sf::Color::Yellow);
    setText(userText, width/2, height/2-45);
    window->draw(userText);
    window->display();
}
bool drawLeaderboard(int asterixSpot, std::chrono::_V2::steady_clock::time_point start, int lastUpdatedTime, int pausedTime) {
    double width = 1000;
    double height = 800;
    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::RenderWindow window(sf::VideoMode(width, height), "Leaderboard", sf::Style::Close);
    sf::Text title("LEADERBOARD", font);
    title.setCharacterSize(24);
    title.setStyle(sf::Text::Underlined | sf::Text::Bold);
    title.setFillColor(sf::Color::White);
    std::ifstream inFile("leaderboard.txt"); // Open the file for reading
    if (!inFile) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 1;
    }
    
    std::string content;
    std::string bigText;
    int lineIndex = 1;
    while (std::getline(inFile, content)) { // Read line by line
        if(asterixSpot == lineIndex-1)
            content += "*";
        bigText += (std::to_string(lineIndex) + ". " + content+ "\n");
        lineIndex++;
    }
    
    inFile.close(); // Close the file
    setText(title, width/2, height/2-150);
    sf::Text nameTitle(bigText, font);
    nameTitle.setCharacterSize(20);
    nameTitle.setStyle(sf::Text::Bold);
    nameTitle.setFillColor(sf::Color::White);
    setText(nameTitle, width/2, height/2);
    // Draw it
    window.draw(title);
    window.draw(nameTitle);
    window.display();
    std::string inputString = "";
    while(window.isOpen()) {
        sf::Event event;
        auto CurrentTime = std::chrono::steady_clock::now();
        pausedTime = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(CurrentTime - start).count())-lastUpdatedTime;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
                return true;
            }
            /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if(inputString.size() > 0)//start game
                {
                    window.close();
                    double width = 1000;
                    double height = 800;
                    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
                    drawGame(width, height, &gameWindow, 20, 31, 5);
                }
                window.display();
            }
            if(event.type == sf::Event::TextEntered) {
                if(event.text.unicode == 8)//backspace
                {
                    if(inputString.size() > 0)
                        inputString.pop_back();
                }else if(event.text.unicode < 128 && event.text.unicode > 64 && inputString.size() < 10){
                    inputString += static_cast<char>(event.text.unicode);
                    if(inputString.size() >= 1){
                        for(int i = 1; i < inputString.size(); i++){
                            inputString[i] = std::tolower(inputString[i]);
                        }
                        inputString[0] = std::toupper(inputString[0]);
                    }
                }
                
                drawTitle(width, height, &window, inputString);
            }*/
        }


    //window.draw(shape);
    
    }
}
void editLeaderboard(int time, int lastUpdatedTime, std::string name, int pausedTime){
    std::ifstream inFile("leaderboard.txt"); // Open the file for reading
    if (!inFile) {
        std::cerr << "Error opening file for reading." << std::endl;
    }
    
    std::string content;
    std::vector<std::string> scores;
    int lineIndex = 1;
    while (std::getline(inFile, content)) { // Read line by line
        scores.push_back(content);
        lineIndex++;
    }
    inFile.close();
    std::ofstream outFile("leaderboard.txt"); // Open the file for reading
    std::vector<std::string> newScores;
    int whereMyNameAt = 10;
    bool newScoreAdded = false;
    for(int i = 0; i < scores.size(); i++){
        if(time < stoi(scores[i].substr(0,2)) && newScoreAdded == false){
            newScoreAdded = true;
            if(time<10)
                newScores.push_back("0"+std::to_string(time)+":00, "+name);
            else
                newScores.push_back(std::to_string(time)+":00, "+name);
            whereMyNameAt = i;
            if(newScores.size() < scores.size())
                newScores.push_back(scores[i]);
        }else if(newScores.size() < scores.size()){
            newScores.push_back(scores[i]);
        }
    }
    for(int j = 0; j < newScores.size(); j++){
        outFile << newScores[j] << std::endl;
    }

    outFile.close();
    auto CurrentTime = std::chrono::steady_clock::now();
    drawLeaderboard(whereMyNameAt, CurrentTime, lastUpdatedTime, pausedTime);
}
std::vector<sf::Sprite> drawUI(double width, double height, sf::RenderWindow* window, int faceType, int flags, int elapsedSeconds, int paused){
    sf::Texture faceTexture[6];
    faceTexture[0].loadFromFile("images/face_happy.png");
    faceTexture[1].loadFromFile("images/face_lose.png");
    faceTexture[2].loadFromFile("images/face_win.png");
    faceTexture[3].loadFromFile("images/leaderboard.png");
    faceTexture[4].loadFromFile("images/pause.png");
    faceTexture[5].loadFromFile("images/play.png");
    faceTexture[6].loadFromFile("images/debug.png");
    sf::Texture digits;
    // Create a sprite
    sf::Sprite faceSprite;
    faceSprite.setTexture(faceTexture[faceType]);
    faceSprite.setPosition(width/2-32, height/2+300);
    window->draw(faceSprite);
    int widthOffset = width/10;
    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(faceTexture[3]);
    leaderboardSprite.setPosition(width/2 +300-widthOffset, height/2+300);
    window->draw(leaderboardSprite);
    sf::Sprite pauseSprite;
    if(paused)
        pauseSprite.setTexture(faceTexture[5]);
    else
        pauseSprite.setTexture(faceTexture[4]);
    pauseSprite.setPosition(width/2+236-widthOffset, height/2+300);
    window->draw(pauseSprite);
    sf::Sprite debugSprite;
    debugSprite.setTexture(faceTexture[6]);
    debugSprite.setPosition(width/2+172-widthOffset, height/2+300);
    window->draw(debugSprite);
    std::vector<sf::Sprite> buttons = {faceSprite, leaderboardSprite, pauseSprite, debugSprite};
    digits.loadFromFile("images/digits.png");
    sf::Sprite FlagNum[3];
    bool negative = false;
    if(flags < 0)
    {
        negative = true;
        flags *= -1;
    }
        
    for(int i = 0; i < 3; i++)
    {
        int modNumber = pow(10, i+1);//10, 100, etc
        int prevModNumber = pow(10, i);
        int offsetValue = 0;
        
        if(i == 2 && negative){
            offsetValue = 10;
        }else{
            if(i != 0){
                offsetValue = flags % modNumber - (flags % prevModNumber);
                offsetValue /= (10*i);
            }else{
                offsetValue = flags % modNumber;
            }
        }
        
        FlagNum[i].setTexture(digits);
        FlagNum[i].setTextureRect(sf::IntRect(21*offsetValue, 0, 21, 32));
        FlagNum[i].setPosition(width/2-300-21*i, height/2+300);
        window->draw(FlagNum[i]);
    }
    sf::Sprite TimeNum[4];
    for(int i = 0; i < 4; i++)
    {
        int modNumber = pow(10, i+1);//10, 100, etc
        int prevModNumber = pow(10, i);
        int offsetValue = 0;
        if(i != 0){
            offsetValue = elapsedSeconds % modNumber - (elapsedSeconds % prevModNumber);
            offsetValue /= (10*i);
        }else{
            offsetValue = elapsedSeconds % modNumber;
        }
        TimeNum[i].setTexture(digits);
        TimeNum[i].setTextureRect(sf::IntRect(21*offsetValue, 0, 21, 32));
        TimeNum[i].setPosition(width/2+450-21*i-widthOffset, height/2+300);
        window->draw(TimeNum[i]);
    }
    return buttons;
}
void drawTime(int width, int height, sf::RenderWindow* window, int elapsedSeconds){
    sf::Texture digits;
    digits.loadFromFile("images/digits.png");
    sf::Sprite TimeNum[4];
    for(int i = 0; i < 4; i++)
    {
        int modNumber = pow(10, i+1);//10, 100, etc
        int prevModNumber = pow(10, i);
        int offsetValue = 0;
        if(i != 0){
            offsetValue = elapsedSeconds % modNumber - (elapsedSeconds % prevModNumber);
            offsetValue /= (10*i);
        }else{
            offsetValue = elapsedSeconds % modNumber;
        }
        TimeNum[i].setTexture(digits);
        TimeNum[i].setTextureRect(sf::IntRect(21*offsetValue, 0, 21, 32));
        TimeNum[i].setPosition(width/2+450-21*i, height/2+300);
        window->draw(TimeNum[i]);
    }
    window->display();
}
void boundChecker(std::vector<std::vector<tile>>* grid, int i, int j, sf::RenderWindow* window){
    if(grid->at(i)[j].ranThrough) 
        return;
    grid->at(i)[j].numberDisplayed = true;
    int rows = grid->size();
    int columns = grid->at(i).size();
    //not a mine, not near any mines
    if(!grid->at(i)[j].hasMine &&  grid->at(i)[j].ranThrough == false){
        grid->at(i)[j].ranThrough = true;
        sf::Sprite gridSprite;
        gridSprite.setTexture(grid->at(i)[j].emptyTexture);
        gridSprite.setPosition(j*32,i*32);
        window->draw(gridSprite);//draws this sprite
        sf::Sprite overlaySprite;
        overlaySprite.setTexture(grid->at(i)[j].overlayGraphic);
        overlaySprite.setPosition(j*32,i*32);
        window->draw(overlaySprite);//draws this sprite
        if(grid->at(i)[j].nearMine == 0)
        {
            if(i > 0)
            {
                boundChecker(grid, i-1, j, window);
            }
            if(i < rows-1)
            {
                boundChecker(grid, i+1, j, window);
            }
            if(j > 0)
            {
                boundChecker(grid, i, j-1, window);
            }
            if(j < columns-1)
            {
                boundChecker(grid, i, j+1, window);
            }
        }
   
    }
}
void redrawGrid(double width, double height, sf::RenderWindow* window, std::vector<std::vector<tile>>* grid, bool showMines, int mines, bool paused, int flags, std::chrono::_V2::steady_clock::time_point startTime, bool debug, int lastUpdatedTime, int gameCondition, std::string name, int pausedTime){
    window->clear(sf::Color::White);
    int tilesDisplayed = 0;
    int totalTiles = (grid->size()*grid->at(0).size())-mines;//l*w-mines
    for(int i = 0; i < grid->size(); i++)
    {   
        for(int j = 0; j < grid->at(i).size(); j++)
        {
            if(paused == false)
            {
                window->draw(grid->at(i)[j].graphic);
                if(grid->at(i)[j].numberDisplayed == true || (showMines && grid->at(i)[j].hasMine))
                {
                    if(grid->at(i)[j].hasMine && showMines == false){
                        //lose game
                        redrawGrid(width, height, window, grid, true, mines, false, flags, startTime, debug, lastUpdatedTime, gameCondition, name, pausedTime);
                        return;
                    }else if(grid->at(i)[j].nearMine == 0 && grid->at(i)[j].numberDisplayed == true){
                        //fill out empty tiles
                        boundChecker(grid, i, j, window);
                    }else{
                        
                    }
                    sf::Sprite emptySprite;
                    emptySprite.setTexture(grid->at(i)[j].emptyTexture);
                    emptySprite.setPosition(j*32,i*32);
                    window->draw(emptySprite);//draws this sprite
                    sf::Sprite overlaySprite;
                    overlaySprite.setTexture(grid->at(i)[j].overlayGraphic);
                    overlaySprite.setPosition(j*32,i*32);
                    window->draw(overlaySprite);//draws this sprite
                    
                }
                if(grid->at(i)[j].numberDisplayed == true || grid->at(i)[j].ranThrough == true)   
                {
                    tilesDisplayed ++;
                }
                if(grid->at(i)[j].flagged == true){
                    if(grid->at(i)[j].numberDisplayed == false)
                    {
                        sf::Sprite flagSprite;
                        flagSprite.setTexture(grid->at(i)[j].flagTexture);
                        flagSprite.setPosition(j*32,i*32);
                        window->draw(flagSprite);//draws this sprite
                        flags --;
                    }else{
                        grid->at(i)[j].flagged = false;
                    }
                }         
            }else{
                sf::Sprite emptySprite;
                emptySprite.setTexture(grid->at(i)[j].emptyTexture);
                emptySprite.setPosition(j*32,i*32);
                window->draw(emptySprite);
            }
        }
    }
    int face = 0;
    //totalTiles -= mines;
    auto finish{std::chrono::steady_clock::now()};
    std::chrono::duration<double> elapsed_seconds{finish - startTime};
    if(gameCondition == 0){
        if(showMines && debug == false)
    {
        face = 1;// sad cause I fucking died
        if(gameCondition == 0) lastUpdatedTime = elapsed_seconds.count();
        gameCondition = 1;
        drawUI(width, height, window, face, flags, lastUpdatedTime-pausedTime, paused);
        window->display();
    } 
    else if(tilesDisplayed >= totalTiles){
        face = 2; // timmy survives his mine sweeping duties and can visit his family.
        if(gameCondition == 0) lastUpdatedTime = elapsed_seconds.count();
        gameCondition = 2;
        drawUI(width, height, window, face, flags, lastUpdatedTime-pausedTime, paused);
        window->display();
        editLeaderboard(lastUpdatedTime-pausedTime, lastUpdatedTime, name, pausedTime);
    }else{
        
        drawUI(width, height, window, face, flags, elapsed_seconds.count()-pausedTime, paused);
        window->display();
    }
    }
    
    //debug text
    /*sf::Font font;
    font.loadFromFile("font.ttf");
    // Create a text
    sf::Text debugTilesShown("Tiles Shown " + std::to_string(tilesDisplayed) + "\n Tiles total" + std::to_string(totalTiles), font);
    debugTilesShown.setCharacterSize(24);
    debugTilesShown.setFillColor(sf::Color::Black);
    setText(debugTilesShown, width/2-100, height/2+350);
    window->draw(debugTilesShown);
*/

    
}
void drawGame(double width, double height, sf::RenderWindow* window, int rows, int columns, int mines, std::string name){
    window->clear(sf::Color::White);
    int gameCondition = 0;
    int pausedTime = 0;
    sf::Texture gridTexture[2];
    gridTexture[0].loadFromFile("images/tile_hidden.png");
    gridTexture[1].loadFromFile("images/tile_revealed.png");
    std::random_device rd;  // Obtain a seed from the system
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, rows);
    std::uniform_int_distribution<> distrib2(0, columns);
    int ogMineCount = mines;
    int lastUpdatedTime = 0;
    std::vector<std::pair<int, int>> randomSpots;
    for (int i = 0; i < mines; ++i) {
        bool freeSpot = false;
        while(freeSpot == false)
        {
            //generate pair
            std::pair<int, int> randomSpot (distrib(gen), distrib2(gen));
            bool foundDuplicate = false;
            for(int j = 0; j < randomSpots.size(); j++){
                if(randomSpots[j] == randomSpot){
                    foundDuplicate = true;
                    break;
                }
            }
            if(foundDuplicate == false)
            {
                randomSpots.push_back(randomSpot);
                freeSpot = true;
            }

        }
        
        //mines --;
    }
    //std::cout << "texture " << std::endl;
    //draw grid, set which ones are mines
    std::vector<std::vector<tile>> grid;
    sf::Sprite gridSprite;
    gridSprite.setTexture(gridTexture[0]);
    for(int i = 0; i < rows; i++)
    {   
        std::vector<tile> tempRow;
        for(int j = 0; j < columns; j++)
        {
            gridSprite.setPosition(j*32,i*32);
            window->draw(gridSprite);
            bool hasMine = false;
            tempRow.push_back(tile(&gridSprite, hasMine));
            //window->display();
        }
        grid.push_back(tempRow);
    }
    if(mines > 0)
    {
        for(int k = 0; k < randomSpots.size(); k++){
            if(randomSpots[k].first < rows && randomSpots[k].second < columns)
            {
                grid[randomSpots[k].first][randomSpots[k].second].hasMine = true;
                mines --;
            } 
        }
    }
    int flags = ogMineCount;
    //assign number values to tiles
    for(int i = 0; i < rows; i++)
    {   
        for(int j = 0; j < columns; j++)
        {
            if(grid[i][j].hasMine){
                if(i > 0)
                {
                    grid[i-1][j].addMineNumber();
                    if(j > 0)
                    {
                        grid[i-1][j-1].addMineNumber();
                    }
                    if(j < columns-1)
                    {
                        grid[i-1][j+1].addMineNumber();
                    }
                }
                if(i < rows-1)
                {
                    grid[i+1][j].addMineNumber();
                    if(j > 0)
                    {
                        grid[i+1][j-1].addMineNumber();
                    }
                    if(j < columns-1)
                    {
                        grid[i+1][j+1].addMineNumber();
                    }
                }
                if(j > 0)
                {
                    grid[i][j-1].addMineNumber();
                }
                if(j < columns-1)
                {
                    grid[i][j+1].addMineNumber();
                }
                    
            }
        }
    }
    //face, leaderboard, pause, debug
    std::vector<sf::Sprite> buttons = drawUI(width, height, window, 0, flags, 0, false);
    bool leaderboard = false;
    bool paused = false;
    bool debug = false;
    auto localbounds1 = buttons[0].getGlobalBounds();
    auto localbounds2 = buttons[1].getGlobalBounds();
    auto localbounds3 = buttons[2].getGlobalBounds();
    auto localbounds4 = buttons[3].getGlobalBounds();
    window->display();
    auto start{std::chrono::steady_clock::now()};
    while(window->isOpen()) {
        sf::Event event;
        if (paused == false && leaderboard == false && gameCondition == 0) {
            auto CurrentTime = std::chrono::steady_clock::now();
            auto SecondsPast = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(CurrentTime - start).count());
            SecondsPast -= pausedTime;
            if (lastUpdatedTime != SecondsPast){
                lastUpdatedTime = SecondsPast;
                redrawGrid(width, height, window, &grid, debug, ogMineCount-mines, false, flags, start, debug, lastUpdatedTime, gameCondition, name, pausedTime);
            } 
        }else{
            //game paused
            auto CurrentTime = std::chrono::steady_clock::now();
            pausedTime = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(CurrentTime - start).count())-lastUpdatedTime;
        }
        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window->close();
            }
            //left mouse clicked
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                //reset button
                if (localbounds1.contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y)) {
                    drawGame(width, height, window, readConfigFile(1), readConfigFile(0), readConfigFile(2), name);
                }
                if (localbounds2.contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y)) {
                    bool ogPaused = paused;
                    leaderboard != drawLeaderboard(10, start, lastUpdatedTime, pausedTime);
                    paused = ogPaused + leaderboard;
                    //open up new window and do shit
                }
                if (localbounds3.contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y)) {
                    paused = !paused;
                }
                if (localbounds4.contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y)) {
                    debug = !debug;
                }
                if(!(paused || leaderboard || debug))
                {
                    //grid checkers
                    int xstartPoint = 0;
                    int ystartPoint = 0;
                    /*if(grid[rows/2][columns/2].graphic.getPosition().y+height/4-readConfigFile(1) <= sf::Mouse::getPosition().y)
                    {
                        ystartPoint = rows/2;
                    }
                    if(grid[rows/2][columns/2].graphic.getPosition().x+width/4-readConfigFile(0) <= sf::Mouse::getPosition().x)
                    {
                        xstartPoint = columns/2;
                    }*/
                    for(int i = ystartPoint; i < rows; i++)
                    {   
                        for(int j = xstartPoint; j < columns; j++)
                        {
                            sf::Sprite sprite = grid[i][j].graphic;
                            auto localbounds = sprite.getGlobalBounds();
                            //if (sprite.getPosition().x+width/4-readConfigFile(0) <= sf::Mouse::getPosition().x + 10 && sprite.getPosition().x+width/4-readConfigFile(0) >= sf::Mouse::getPosition().x -20 && sprite.getPosition().y+height/4-readConfigFile(1) <= sf::Mouse::getPosition().y +10 && sprite.getPosition().y+height/4-readConfigFile(1) >= sf::Mouse::getPosition().y-20)
                            if (localbounds.contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
                            {
                                //code goes here
                                sprite.setTexture(gridTexture[1]);
                                grid[i][j].graphic = sprite;
                                grid[i][j].numberDisplayed = true;

                                //window->display();
                            }
                        
                            
                        }
                    }
                }
                
                if(debug == false && paused == false && leaderboard == false)
                {
                    redrawGrid(width, height, window, &grid, false, ogMineCount-mines, false, flags, start, debug, lastUpdatedTime, gameCondition, name, pausedTime);
                }else if(debug == true){
                    redrawGrid(width, height, window, &grid, true, ogMineCount-mines, paused, flags, start, debug, lastUpdatedTime, gameCondition, name, pausedTime);
                }
                else if(paused == true || leaderboard){
                    redrawGrid(width, height, window, &grid, false, ogMineCount-mines, true, flags, start, debug, lastUpdatedTime, gameCondition, name, pausedTime);
                }
                
            }
            //right mouse, flag mine
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
                if(!(paused || leaderboard || debug))
                {
                    //grid checkers
                    int xstartPoint = 0;
                    int ystartPoint = 0;
                    /*if(grid[rows/2][columns/2].graphic.getPosition().y+height/4-16 <= sf::Mouse::getPosition().y)
                    {
                        ystartPoint = rows/2;
                    }
                    if(grid[rows/2][columns/2].graphic.getPosition().x+width/2.5+64 <= sf::Mouse::getPosition().x)
                    {
                        xstartPoint = columns/2;
                    }*/
                    for(int i = ystartPoint; i < rows; i++)
                    {   
                        for(int j = xstartPoint; j < columns; j++)
                        {
                            sf::Sprite sprite = grid[i][j].graphic;
                            auto localbounds = sprite.getGlobalBounds();
                            //if (sprite.getPosition().x+width/4-readConfigFile(0) <= sf::Mouse::getPosition().x + 10 && sprite.getPosition().x+width/4-readConfigFile(0) >= sf::Mouse::getPosition().x -20 && sprite.getPosition().y+height/4-readConfigFile(1) <= sf::Mouse::getPosition().y +10 && sprite.getPosition().y+height/4-readConfigFile(1) >= sf::Mouse::getPosition().y-20)
                            if (localbounds.contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
                            {
                                //RED FLAG 
                                if(grid[i][j].numberDisplayed == false){
                                    grid[i][j].flagged = !grid[i][j].flagged;
                                }
                            }
                        
                            
                        }
                    }
                    redrawGrid(width, height, window, &grid, false, ogMineCount-mines, false, flags, start, debug, lastUpdatedTime, gameCondition, name, pausedTime);
                    
                }
                
                    
            }
            
        }
        /*
        }*/
    }
}


int main() {
    double width = 1000;
    double height = 800;
    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::RenderWindow window(sf::VideoMode(width, height), "MinesweeperTitle", sf::Style::Close);
    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Red);
    drawTitle(width, height, &window, "");
    std::string inputString = "";
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if(inputString.size() > 0)//start game
                {
                    std::string name = inputString;
                    window.close();
                    double width = 32*readConfigFile(0);
                    double height = 32* readConfigFile(1) +288;
                    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
                    drawGame(width, height, &gameWindow, readConfigFile(1), readConfigFile(0), readConfigFile(2), name);
                }
                window.display();
            }
            if(event.type == sf::Event::TextEntered) {
                if(event.text.unicode == 8)//backspace
                {
                    if(inputString.size() > 0)
                        inputString.pop_back();
                }else if(event.text.unicode < 128 && event.text.unicode > 64 && inputString.size() < 10){
                    inputString += static_cast<char>(event.text.unicode);
                    if(inputString.size() >= 1){
                        for(int i = 1; i < inputString.size(); i++){
                            inputString[i] = std::tolower(inputString[i]);
                        }
                        inputString[0] = std::toupper(inputString[0]);
                    }
                }
                
                drawTitle(width, height, &window, inputString);
            }
        }


    //window.draw(shape);
    
    }
    return 0;
}
