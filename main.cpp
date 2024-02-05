#include <iostream>
#include <SFML/Graphics.hpp>
int scoreCount = 0;
bool isDragging = false;
bool isTileSelected = false;
int selectedTileRow = -1;
int selectedTileCol = -1;
sf::Vector2u screenSize;
sf::Vector2f initialClickPosition;
sf::Vector2f initialTilePosition;

using namespace sf;
struct Tile {
    sf::RectangleShape shape;
    int type;
};

const int NUM_TILE_TYPES = 5;  

void setSquareColor(Tile& square, int row, int col, bool b = false) {
  
    std::cout << row << " " << col << std::endl;
    square.type = rand() % NUM_TILE_TYPES;

    switch (square.type) {
    case 0:
        square.shape.setFillColor(sf::Color::Red);
        
        break;
    case 1:
        square.shape.setFillColor(sf::Color::Green);
       
        break;
    case 2:
        square.shape.setFillColor(sf::Color::Blue);
        
        break;
    case 3:
        square.shape.setFillColor(sf::Color::Yellow);
       
        break;
    case 4:
        square.shape.setFillColor(sf::Color::Magenta);
        
        break;
       
    default:
        square.shape.setFillColor(sf::Color::White);
    }
    
}
void generateBoard(std::vector<std::vector<Tile>>&gameBoard, int ROWS, int COLS, Vector2u&screenSize) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            
           
            gameBoard[row][col].shape.setSize(sf::Vector2f(35, 35));  
            int padding = 20;
            gameBoard[row][col].shape.setPosition(((screenSize.x - COLS * 50) / 2 + col * 50)+padding, ((screenSize.y - ROWS * 50) / 2 + row * 50) + padding);
            setSquareColor(gameBoard[row][col], row, col, true);
        }
    }
}

//void removeAndReplaceTiles(std::vector<std::vector<Tile>> gameBoard, int ROWS, int COLS) {
//    for (int col = 0; col < COLS; col++) {
//        int shift = 0;
//        for (int row = ROWS - 1; row >= 0; row--) {
//            if (gameBoard[row][col].type == -1) {
//                shift++;
//            }
//            else if (shift > 0) {
//                setSquareColor(gameBoard[row + shift][col]);
//
//            }
//        }
//       
//    }
//}

void checkMatchesAndRemove(std::vector<std::vector<Tile>>& gameBoard, int ROWS, int COLS) {
    bool matchFound = false;

    
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col <= COLS - 3; col++) {
            int currentType = gameBoard[row][col].type;
            if (currentType != -1 && currentType == gameBoard[row][col + 1].type && currentType == gameBoard[row][col + 2].type) {
                setSquareColor(gameBoard[row][col], row, col);
                setSquareColor(gameBoard[row][col+1], row, col);
                setSquareColor(gameBoard[row][col+2], row, col);
              /*  gameBoard[row][col].type = -1;
                gameBoard[row][col + 1].type = -1;
                gameBoard[row][col + 2].type = -1;*/
                matchFound = true;
                scoreCount++;
            }
        }
    }

    
    for (int col = 0; col < COLS; col++) {
        for (int row = 0; row <= ROWS - 3; row++) {
            int currentType = gameBoard[row][col].type;
            if (currentType != -1 && currentType == gameBoard[row + 1][col].type && currentType == gameBoard[row + 2][col].type) {
               
                //gameBoard[row][col].type = -1;  
                //gameBoard[row + 1][col].type = -1;
                //gameBoard[row + 2][col].type = -1;
                
                setSquareColor(gameBoard[row][col], row, col);
                setSquareColor(gameBoard[row + 1][col], row, col);
                setSquareColor(gameBoard[row + 2][col], row, col);
                matchFound = true;
                scoreCount++;
            }
        }
    }
    /*if (matchFound) {
        scoreCount++;
    }*/
    //if (matchFound) {
    //    std::cout << "Match found2" << std::endl;
    //   
    //    removeAndReplaceTiles(gameBoard, ROWS, COLS);
    //}
}


int main()
{
    srand(time(NULL));
    int windowWidth = 600; 
    int windowHeight = 400;  

    RenderWindow window(VideoMode(windowWidth, windowHeight), L"3 в ряд");

    screenSize = window.getSize();

    const int ROWS = 8;
    const int COLS = 8;
    Font font;
   
    sf::Text scoreText;
    font.loadFromFile("arial.ttf");
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setString("Score: " + std::to_string(scoreCount));
    scoreText.setPosition(0, 0);
    
    std::vector<std::vector<Tile>> gameBoard(ROWS, std::vector<Tile>(COLS));
    
    generateBoard(gameBoard, ROWS, COLS, screenSize);
    
    

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                initialClickPosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                isDragging = true;

                
                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++) {
                        if (gameBoard[row][col].shape.getGlobalBounds().contains(initialClickPosition)) {
                            isTileSelected = true;
                            selectedTileRow = row;
                            selectedTileCol = col;
                            initialTilePosition = gameBoard[row][col].shape.getPosition();
                            break;
                        }
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (isDragging && isTileSelected) {
                    isDragging = false;

                    sf::Vector2f finalClickPosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    float deltaX = finalClickPosition.x - initialClickPosition.x;
                    float deltaY = finalClickPosition.y - initialClickPosition.y;
                    Vector2f tempPos;
                    //horizontal
                    if (std::abs(deltaX) > std::abs(deltaY)) {
                        
                        if (deltaX > 50 && selectedTileCol < COLS - 1) {
                            // left
                            tempPos = gameBoard[selectedTileRow][selectedTileCol].shape.getPosition();
                            gameBoard[selectedTileRow][selectedTileCol].shape.setPosition(gameBoard[selectedTileRow][selectedTileCol + 1].shape.getPosition());
                            gameBoard[selectedTileRow][selectedTileCol + 1].shape.setPosition(tempPos);
                            std::swap(gameBoard[selectedTileRow][selectedTileCol], gameBoard[selectedTileRow][selectedTileCol + 1]);
                        }
                        else if (deltaX < -50 && selectedTileCol > 0) {
                            tempPos = gameBoard[selectedTileRow][selectedTileCol].shape.getPosition();
                            gameBoard[selectedTileRow][selectedTileCol].shape.setPosition(gameBoard[selectedTileRow][selectedTileCol - 1].shape.getPosition());
                            gameBoard[selectedTileRow][selectedTileCol - 1].shape.setPosition(tempPos);
                            // right
                            std::swap(gameBoard[selectedTileRow][selectedTileCol], gameBoard[selectedTileRow][selectedTileCol - 1]);
                        }
                    }
                    // vertical
                    else {
                        
                        if (deltaY > 50 && selectedTileRow < ROWS - 1) {
                            tempPos = gameBoard[selectedTileRow][selectedTileCol].shape.getPosition();
                            gameBoard[selectedTileRow][selectedTileCol].shape.setPosition(gameBoard[selectedTileRow+1][selectedTileCol].shape.getPosition());
                            gameBoard[selectedTileRow+1][selectedTileCol ].shape.setPosition(tempPos);
                            // down
                            std::swap(gameBoard[selectedTileRow][selectedTileCol], gameBoard[selectedTileRow + 1][selectedTileCol]);
                        }
                        else if (deltaY < -50 && selectedTileRow > 0) {
                            // uop
                            tempPos = gameBoard[selectedTileRow][selectedTileCol].shape.getPosition();
                            gameBoard[selectedTileRow][selectedTileCol].shape.setPosition(gameBoard[selectedTileRow-1][selectedTileCol].shape.getPosition());
                            gameBoard[selectedTileRow-1][selectedTileCol].shape.setPosition(tempPos);
                            std::swap(gameBoard[selectedTileRow][selectedTileCol], gameBoard[selectedTileRow - 1][selectedTileCol]);
                        }
                    }
                }
                checkMatchesAndRemove(gameBoard, ROWS, COLS);

                isTileSelected = false;
            }
        }
    
    
    
    

    

         

        window.clear();

        //rednder
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                window.draw(gameBoard[row][col].shape);
            }
        }
        scoreText.setString("Score: " + std::to_string(scoreCount));
        window.draw(scoreText);
        window.display();
    }

    return 0;
}