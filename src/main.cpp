#include <iostream>     // Standard I/O library for input and output operations
#include <raylib.h>     // The Raylib library for graphics and game development
#include <deque>        // The deque (double-ended queue) container from the standard library
#include <raymath.h>    // Raylib's math functions for vector operations
#include <vector>


using namespace std;    // Using standard library classes and functions without the 'std::' prefix

Color backgroundColor = {173, 204, 96, 255};         // A backgroundColor color for the background
Color snakeColor = {43, 51, 24, 255};       // A darker backgroundColor color for the snake

int cellSize = 30;      // The size of each cell in pixels
int cellCount = 30;     // The number of cells in the grid
int offset = 75;        // The border offset for the game window

double lastUpdateTime = 0;   // The time of the last update for event handling

bool ElementInDeque(Vector2 element, deque<Vector2> deque){
    // Iterates over each element in the deque
    for(unsigned int i = 0; i < deque.size(); i++) {
        // Checks if the current element in the deque equals the given element
        if(Vector2Equals(deque[i], element)) {
            return true; // Returns true if the given element is found in the deque
        }
    }
    return false; // Returns false if the element is not found
}

bool eventTriggered(double interval){
    double currentTime = GetTime();   // Retrieves the current time
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime; // Updates the last update time
        return true;                  // Returns true to trigger the event
    }
    return false;                     // Otherwise, returns false
}

class GameMenu {
    private:
    bool active;
    vector<Color> colorOptions;
    int backgroundColorIndex;
    int snakeColorIndex;

    public:

    // Method to activate the menu
    void Activate() 
        { active = true; }
    GameMenu() : active(true), backgroundColorIndex(0), snakeColorIndex(1) {
        // Initialize color options
        colorOptions = {{255, 255, 0, 255}, {255, 165, 0, 255}, {173, 204, 96, 255}, {0, 0, 0, 255}, {128, 0, 128, 255}};
    }

    bool IsActive() const {
        return active;
    }

    void DisplayMenu() {
        // Clear the entire screen with a white background.
        ClearBackground(RAYWHITE);

        // Display the title "Game Menu" at the specified position (x: 400, y: 50) with font size 20 in black color.
        DrawText("Game Menu", 400, 50, 20, BLACK);

        // Display instructions "Use arrow keys to change colors" on the screen at position (x: 350, y: 100) with font size 20 in dark gray color.
        DrawText("Use arrow keys to change colors", 350, 100, 20, DARKGRAY);

        // Display the text "Background Color" on the screen to label the background color selection area.
        DrawText("Background Color", 400, 150, 20, BLACK);

        // Draw a small rectangle (20x20 pixels) representing the current background color. 
        // The color of the rectangle is determined by the current `backgroundColorIndex` selected in the `colorOptions`.
        DrawRectangle(600, 145, 20, 20, colorOptions[backgroundColorIndex]);

        // Display the text "Snake Color" on the screen to label the snake color selection area.
        DrawText("Snake Color", 400, 200, 20, BLACK);

        // Draw a small rectangle (20x20 pixels) representing the current snake color. 
        // The color of the rectangle is determined by the current `snakeColorIndex` selected in the `colorOptions`.
        DrawRectangle(600, 195, 20, 20, colorOptions[snakeColorIndex]);

        // Display the text "Press ENTER to start" on the screen. This serves as an instruction for the player to start the game.
        DrawText("Press ENTER to start", 400, 300, 20, DARKBLUE);
    }


    void HandleInput() {
        if (IsKeyPressed(KEY_ENTER)) {
            active = false;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            backgroundColorIndex = (backgroundColorIndex + 1) % colorOptions.size();
        }
        if (IsKeyPressed(KEY_LEFT)) {
            backgroundColorIndex = (backgroundColorIndex + colorOptions.size() - 1) % colorOptions.size();
        }
        if (IsKeyPressed(KEY_UP)) {
            snakeColorIndex = (snakeColorIndex + 1) % colorOptions.size();
        }
        if (IsKeyPressed(KEY_DOWN)) {
            snakeColorIndex = (snakeColorIndex + colorOptions.size() - 1) % colorOptions.size();
        }
    }

    Color GetBackgroundColor() const {
        return colorOptions[backgroundColorIndex];
    }

    Color GetSnakeColor() const {
        return colorOptions[snakeColorIndex];
    }
};


class Snake {
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}}; // Initializes the snake's body
    Vector2 direction = {1, 0};   // The initial direction of the snake
    bool addSegment = false;      // Flag to indicate whether to grow the snake

    void Draw() {
        // Iterates through each segment of the snake's body
        for(long unsigned int i = 0; i < body.size(); i++) {
            // Creates a rectangle for the segment
            Rectangle segment = Rectangle{
                offset + body[i].x * cellSize, 
                offset + body[i].y * cellSize, 
                (float)cellSize, (float)cellSize
            };
            DrawRectangleRounded(segment, 0.5, 6, snakeColor); // Draws the rounded rectangle for the segment
        }
    }

    void Update() {
        // Adds a new segment at the front in the current direction
        body.push_front(Vector2Add(body[0], direction));

        if(addSegment) {
            addSegment = false;
        } else {
            body.pop_back(); // Removes the last segment of the body
        }
    }

    void Reset() {
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}}; // Reinitializes the snake's body
        direction = {1, 0};   // Resets the initial direction of the snake
    }
};

class Food {
public:
    Vector2 position;    // Stores the position of the food
    Texture2D texture;   // Stores the texture of the food

    Food(deque<Vector2> snakeBody) {
        Image image = LoadImage("Graphics/Beluga_food.png"); // Loads the food image
        texture = LoadTextureFromImage(image); // Creates a texture from the loaded image
        UnloadImage(image);                   // Frees the memory used by the image
        position = GenerateRandomPos(snakeBody); // Generates a random position for the food
    }

    ~Food() {
        UnloadTexture(texture); // Frees the memory used by the texture
    }

    void Draw() {
        // Draws the food texture at the specified position
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell() {
        // Generates random x and y values within the grid boundaries
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y}; // Returns the generated random position
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
        Vector2 position = GenerateRandomCell();
        // Ensures the generated position is not already occupied by the snake's body
        while(ElementInDeque(position, snakeBody)) {
            position = GenerateRandomCell(); // Generates a new random cell position
        }
        return position; // Returns the final unoccupied position
    }
};

class Game {
        GameMenu* menu;
public:
    Snake snake; // Instance of the Snake class
    Food food;   // Instance of the Food class
    bool running = true; // Indicates if the game is running
    int score = 0;       // The player's score
    Sound eatSound;      // Sound for eating
    Sound wallSound;     // Sound for hitting a wall

        // Setter method to set the GameMenu reference
    void SetMenu(GameMenu* menu) {
        this->menu = menu;
    }

    Game(): snake(), food(snake.body) {
        InitAudioDevice(); // Initializes the audio device
        eatSound = LoadSound("Sounds/Siren_eat.mp3"); // Load eating sound
        wallSound = LoadSound("Sounds/Crash_wall.mp3"); // Load collision sound
    }

    ~Game() {
        UnloadSound(eatSound);  // Unload eating sound
        UnloadSound(wallSound); // Unload collision sound
        CloseAudioDevice();     // Closes the audio device
    }

    void Draw() {
        food.Draw();  // Draws the food
        snake.Draw(); // Draws the snake
    }

    void Update() {
        if (running) {
            snake.Update(); // Updates the snake
            CheckCollisionWithFood(); // Checks collision with food
            CheckCollisionWithEdges(); // Checks collision with edges
            CheckCollisionWithTail();  // Checks collision with tail
        }
    }

    void CheckCollisionWithFood() {
        if(Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body); // Regenerates food position
            snake.addSegment = true; // Snake grows
            score += 10;            // Increases score
            PlaySound(eatSound);    // Play eating sound
        }
    }

    void CheckCollisionWithEdges() {
        // Check for collision with the grid boundaries
        if(snake.body[0].x < 0 || snake.body[0].x >= cellCount || 
           snake.body[0].y < 0 || snake.body[0].y >= cellCount) {
            GameOver();
        }
    }

    void GameOver() {
        snake.Reset(); // Reset the snake
        food.position = food.GenerateRandomPos(snake.body); // Reset food position
        running = false; // Stops the game
        score = 0;       // Resets the score
        PlaySound(wallSound); // Play collision sound
        menu->Activate(); // Reactivates the menu at the collision WITH THE WALL OR ITSELF
    }

    void CheckCollisionWithTail() {
        // Check for collision with the snake's own body
        for(size_t i = 1; i < snake.body.size(); ++i) {
            if(Vector2Equals(snake.body[0], snake.body[i])) {
                GameOver();
                break;
            }
        }
    }
};

int main() {
    // Initialization
    cout << "Starting the game..." << endl; // Displays a start message
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake"); // Initializes the game window
    SetTargetFPS(60); // Sets a stable target frame rate

    GameMenu menu;
    Game game; // Creates a game instance
    game.SetMenu(&menu); // Set the GameMenu reference


    // Main game loop
    while (!WindowShouldClose()) { // Detects window close request
        BeginDrawing(); // Starts the drawing process

         if (menu.IsActive()) {
            menu.DisplayMenu();
            menu.HandleInput();
            if (!menu.IsActive()) {
                backgroundColor = menu.GetBackgroundColor();
                snakeColor = menu.GetSnakeColor();
            }
        } else {
            // Input handling for snake directions
            if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y != 1) {
                game.snake.direction = {0, -1};
                game.running = true;
            }
            if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1) {
                game.snake.direction = {0, 1};
                game.running = true;
            }
            if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1) {
                game.snake.direction = {-1, 0};
                game.running = true;
            }
            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1) {
                game.snake.direction = {1, 0};
                game.running = true;
            }

            // Game state update
            if (eventTriggered(0.2)) { // Checks if it's time to update the game state
                game.Update();
            }

            // Drawing the game elements
            ClearBackground(backgroundColor);

        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, snakeColor);
        // Draw the game window borders
        DrawText("Learning Classes and algorithms by Snake ", offset-52, 20, 40, snakeColor); //Drawing the title upside middle
        DrawText(TextFormat("%i", game.score), offset+360, offset+cellSize*cellCount+10, 40, snakeColor);  //Drawing the score on the middle bottom of the screen
        game.Draw();            // Draws the game elements
        } // end

        EndDrawing();           // Ends the drawing process
    }

    CloseWindow(); // Closes the game window
    return 0;
}
