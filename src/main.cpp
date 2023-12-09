#include <iostream>     // Standard I/O library for input and output operations
#include <raylib.h>     // The Raylib library for graphics and game development
#include <deque>        // The deque (double-ended queue) container from the standard library
#include <raymath.h>    // Raylib's math functions for vector operations
#include <vector>
#include <string>
#include <algorithm>


#define MAX_INPUT_CHARS 12 // desired maximum characters


using namespace std;    // Using standard library classes and functions without the 'std::' prefix

Color backgroundColor = {173, 204, 96, 255};         // A backgroundColor color for the background
Color snakeColor = {43, 51, 24, 255};       // A darker backgroundColor color for the snake

int cellSize = 30;      // The size of each cell in pixels
int cellCount = 29;     // The number of cells in the grid
int offset = 75;        // The border offset for the game window

double lastUpdateTime = 0;   // The time of the last update for event handling

enum class SpeedLevel {
    SLOW,
    MEDIUM,
    FAST,
    VERY_FAST
};
struct PlayerData {
    string name;
    int score;
    SpeedLevel difficulty; // New field to store the difficulty level
};


string SpeedLevelToString(SpeedLevel level) {
    switch(level) {
        case SpeedLevel::SLOW: return "Slow";
        case SpeedLevel::MEDIUM: return "Medium";
        case SpeedLevel::FAST: return "Fast";
        case SpeedLevel::VERY_FAST: return "Very Fast";
        default: return "Unknown";
    }
}


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
    bool mouseOnNameInputBox;

    int speedLevelIndex = 0; // Index for speed level selection

    Rectangle nameInputBox;


    string playerName;  // to store current player's name
    vector<PlayerData> players;  // to store session player data



    public:

    // Method to activate the menu
    void Activate() 
        { active = true; }
    GameMenu() : active(true), backgroundColorIndex(0), snakeColorIndex(1) {
            // Initialize name input box
        nameInputBox = {400, 320, 325, 50};     // X , Y, Width, Height
        // Initialize color options
        colorOptions = {{255, 255, 0, 255}, {255, 165, 0, 255}, {173, 204, 96, 255}, {0, 0, 0, 255}, {128, 0, 128, 255}};
    }

    bool IsActive() const {
        return active;
    }

    void Deactivate() {
        active = false;
    }

    string GetPlayerName() const {
    return playerName;
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
        
        DrawText("Enter your name: ", 400, 300, 20, BLACK);

        // Display the input box for the player's name
        DrawRectangleRec(nameInputBox, LIGHTGRAY);
        DrawRectangleLines((int)nameInputBox.x, (int)nameInputBox.y, (int)nameInputBox.width, (int)nameInputBox.height, DARKGRAY);
        DrawText(playerName.c_str(), (int)nameInputBox.x + 5, (int)nameInputBox.y + 8, 40, MAROON);

        DrawText("Press ENTER to start", 400, 500, 20, DARKBLUE);

        // Add display for speed level selection
        DrawText("Speed Level < >", 400, 250, 20, BLACK);
        const char* speedLevels[] = {"Slow", "Medium", "Fast", "Very Fast"};
        DrawText(speedLevels[speedLevelIndex], 600, 245, 20, BLACK);

 
    }


    void HandleInput() {
        // Update mouseOnNameInputBox based on mouse interaction
        mouseOnNameInputBox = CheckCollisionPointRec(GetMousePosition(), nameInputBox);


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
            if (backgroundColorIndex == snakeColorIndex) {
        snakeColorIndex = (snakeColorIndex + 1) % colorOptions.size(); // Change snake color
        }
        // Handle player name input
        if (mouseOnNameInputBox) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (playerName.length() < MAX_INPUT_CHARS)) {
                    playerName += (char)key;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (!playerName.empty()) {
                    playerName.pop_back();
                }
            }
        }
                // Handle speed level selection
        if (IsKeyPressed(KEY_COMMA)) {
            speedLevelIndex = (speedLevelIndex - 1 + 4) % 4; // Cycle backwards through speed levels
        }
        if (IsKeyPressed(KEY_PERIOD)) {
            speedLevelIndex = (speedLevelIndex + 1) % 4; // Cycle forwards through speed levels
        }


    }

     SpeedLevel GetSpeedLevel() const {
        switch(speedLevelIndex) {
            case 0: return SpeedLevel::SLOW;
            case 1: return SpeedLevel::MEDIUM;
            case 2: return SpeedLevel::FAST;
            case 3: return SpeedLevel::VERY_FAST;
            default: return SpeedLevel::SLOW;
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
    private:
    SpeedLevel currentDifficulty;
    public:
    Vector2 position;    // Stores the position of the food
    std::vector<Texture2D> textures; // Updated to store multiple textures
    int textureIndex; // Index to keep track of the current texture
    std::vector<Sound> eatSounds; // Updated to store multiple sounds
    int soundIndex; // Index to keep track of the current sound

    Food(deque<Vector2> snakeBody, SpeedLevel difficulty) : currentDifficulty(difficulty) {
        // Load multiple textures
        //textures.push_back(LoadTexture("Graphics/Beluga_food1.png"));
        textures.push_back(LoadTexture("Graphics/1.png"));
        textures.push_back(LoadTexture("Graphics/2.png"));
        textures.push_back(LoadTexture("Graphics/3.png"));
        textures.push_back(LoadTexture("Graphics/4.png"));
        // Initialize texture index
        textureIndex = GetRandomValue(0, textures.size() - 1);
        // Load multiple sounds
        eatSounds.push_back(LoadSound("Sounds/Siren_eat1.mp3"));
        eatSounds.push_back(LoadSound("Sounds/a.mp3"));
        eatSounds.push_back(LoadSound("Sounds/b.mp3"));
        eatSounds.push_back(LoadSound("Sounds/c.mp3"));
        // Initialize sound index
        soundIndex = GetRandomValue(0, eatSounds.size() - 1);

        for (const auto& sound : eatSounds) {
            if (sound.stream.buffer == nullptr) {
                // Display a message indicating that the sound failed to load
                printf("Error loading sound file.\n");
            }
        }
        position = GenerateRandomPos(snakeBody);
    }
    ~Food() {
        // Unload all textures
        for (const auto& texture : textures) {
            UnloadTexture(texture);
        }
        // Unload all sounds
        for (const auto& sound : eatSounds) {
            UnloadSound(sound);
        }
    }

    void Draw() {
        // Draw the current texture at the specified position
        DrawTexture(textures[textureIndex], offset + position.x * cellSize, offset + position.y * cellSize, Fade(WHITE, 0.5f)); // 
    }
    Vector2 GenerateRandomCell() { //UPDT difficulty level
    int minDistance;
    switch (currentDifficulty) {
        case SpeedLevel::SLOW:       minDistance = 4; break;
        case SpeedLevel::MEDIUM:     minDistance = 3; break;
        case SpeedLevel::FAST:       minDistance = 2; break;
        case SpeedLevel::VERY_FAST:  minDistance = 1; break;
        default:                     minDistance = 1;
    }

    float x = GetRandomValue(minDistance, cellCount - 1 - minDistance);
    float y = GetRandomValue(minDistance, cellCount - 1 - minDistance);
    return Vector2{x, y};
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
        bool difficultyHasChanged;
        SpeedLevel newDifficulty;
    public:
        Snake snake; // Instance of the Snake class
        Food food;   // Instance of the Food class
        bool running = true; // Indicates if the game is running
        int score = 0;       // The player's score
        //Sound eatSound;      // Sound for eating
        Sound wallSound;     // Sound for hitting a wall

        string playerName;  // to store current player's name
        vector<PlayerData> players;

        SpeedLevel speedLevel = SpeedLevel::SLOW; // New member for speed level

        vector<string> messages = {
            "Learning Classes and algorithms by Snake",
            "Message after first food",
            "Message after second food",};

        int currentMessageIndex = 0; // To keep track of the current message


        double GetUpdateInterval() {
        double baseInterval = 0.2;
        switch(speedLevel) {
            case SpeedLevel::SLOW: baseInterval = 0.2; break;
            case SpeedLevel::MEDIUM: baseInterval = 0.15; break;
            case SpeedLevel::FAST: baseInterval = 0.1; break;
            case SpeedLevel::VERY_FAST: baseInterval = 0.05; break;
        }
        double interval = baseInterval - (score / 1000.0);
        return (interval > 0.05) ? interval : 0.05;
    }
        // Setter method to set the GameMenu reference
    void SetMenu(GameMenu* menu) {
        this->menu = menu;
    }

    void ChangeDifficulty(SpeedLevel newLevel) {
    newDifficulty = newLevel;
    difficultyHasChanged = true;
    }

    void SetDifficulty(SpeedLevel newLevel) {
        speedLevel = newLevel;
        food = Food(snake.body, speedLevel); // Reinitialize food with the new difficulty
    }

    Game() : snake(), food(snake.body, speedLevel), difficultyHasChanged(false), newDifficulty(SpeedLevel::SLOW){ //passing the speedLevel to the food object to use difficulty level updt
        wallSound = LoadSound("Sounds/Crash_wall.mp3"); // Load collision sound
    }
    ~Game() {
        // UnloadSound(eatSound);  // Unload eating sound
        UnloadSound(wallSound); // Unload collision sound
       // CloseAudioDevice();     // Closes the audio device
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
            if (difficultyHasChanged) {
            SetDifficulty(newDifficulty);
            difficultyHasChanged = false; // Reset the flag after changing the difficulty
        
            }
        }
    }
    void CheckCollisionWithFood() {
        if(Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body); // Regenerates food position
            // Increment the texture index and loop back to the beginning if necessary
            food.textureIndex = (food.textureIndex + 1) % food.textures.size();
            snake.addSegment = true; // Snake grows
            score += 10;            // Increases score
            //Increment the message vector index
            currentMessageIndex = (currentMessageIndex + 1) % messages.size();
            // Play a ordered eating sound
            PlaySound(food.eatSounds[food.soundIndex]);
            food.soundIndex = (food.soundIndex + 1) % food.eatSounds.size();
        }
    }
    void CheckCollisionWithEdges() {
        // Check for collision with the grid boundaries
        if(snake.body[0].x < 0 || snake.body[0].x >= cellCount || 
           snake.body[0].y < 0 || snake.body[0].y >= cellCount) {
            GameOver();
        }
    }

    void SortLeaderboard() {
        sort(players.begin(), players.end(), [](const PlayerData& a, const PlayerData& b) {
            return a.score > b.score; // Sort in descending order of score
        });
        }

    void GameOver() {
        PlayerData playerData;
        playerData.name = playerName;
        playerData.score = score;
        playerData.difficulty = speedLevel; // Save the current speed level as the difficulty
        players.push_back(playerData); // Save player's data
        snake.Reset(); // Reset the snake
        food.position = food.GenerateRandomPos(snake.body); // Reset food position
        running = false; // Stops the game
        SortLeaderboard();
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
    InitAudioDevice(); // Initializes the audio device
    GameMenu menu;
    Game game; // Creates a game instance
    game.SetMenu(&menu); // Set the GameMenu reference
    // Main game loop
    while (!WindowShouldClose()) { // Detects window close request
        BeginDrawing(); // Starts the drawing process
         if (menu.IsActive()) {
                // Call to InputPlayerName method to get player's name
            menu.DisplayMenu();
            menu.HandleInput();
            if (!menu.IsActive()) {
                backgroundColor = menu.GetBackgroundColor();
                snakeColor = menu.GetSnakeColor();
                game.playerName = menu.GetPlayerName();  // Pass the player's name to the Game class
                game.speedLevel = menu.GetSpeedLevel(); // Set the selected speed level
                game.speedLevel = menu.GetSpeedLevel(); // Set the selected speed level
                //game.food = Food(game.snake.body, game.speedLevel); // Initialize food with the correct difficulty
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
            //if (eventTriggered(0.2)) { 
            if (eventTriggered(game.GetUpdateInterval())) {// Checks if it's time to update the game state
                game.Update();
            }
            // Drawing the game elements
        ClearBackground(backgroundColor);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, snakeColor);
        // Draw the game window borders
        DrawText(game.messages[game.currentMessageIndex].c_str(), offset-52, 20, 40, snakeColor);
        DrawText(TextFormat("%i", game.score), offset+360, offset+cellSize*cellCount+10, 40, snakeColor);  //Drawing the score on the middle bottom of the screen
        game.Draw();            // Draws the game elements
            if (!game.running) {
                // Display player's data after the game ends

            // Modifyed your leaderboard display logic
            for (size_t i = 0; i < game.players.size(); ++i) {
                string difficulty = SpeedLevelToString(game.players[i].difficulty);
                DrawText(TextFormat("Player: %s - Score: %i - Difficulty: %s", 
                    game.players[i].name.c_str(), 
                    game.players[i].score, 
                    difficulty.c_str()),
                    offset + 50, offset + 50 + i * 30, 20, snakeColor);
            }
        }
        } // end
        EndDrawing();// Ends the drawing process
    }
    CloseAudioDevice(); // Close the audio device 
    CloseWindow(); // Closes the game window
    return 0;
}

