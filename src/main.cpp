#include <iostream>     // Includes the Standard I/O library for input and output operations
#include <raylib.h>     // Includes the Raylib library, a simple and easy-to-use library to enjoy videogames programming
#include <deque>        // Includes the deque (double-ended queue) container from the C++ standard library
#include <raymath.h>    // Includes Raylib's math functions for vector operations
#include <vector>       // Includes the vector container from the C++ standard library
#include <string>       // Includes the string class from the C++ standard library
#include <algorithm>    // Includes the C++ standard library's algorithm library for various functions

#define MAX_INPUT_CHARS 12 // Define a constant for the maximum number of characters in input

using namespace std;    // Use the standard namespace to avoid prefixing with 'std::'

// Define colors for the game
Color backgroundColor = {173, 204, 96, 255};         // Background color
Color snakeColor = {43, 51, 24, 255};                // Snake color

// Define game parameters
int cellSize = 30;      // The size of each cell in pixels
int cellCount = 29;     // The number of cells in the grid
int offset = 75;        // The border offset for the game window
double lastUpdateTime = 0;   // The time of the last update for event handling

// Enumeration for different speed levels in the game
enum class SpeedLevel {
    SLOW,
    MEDIUM,
    FAST,
    VERY_FAST
};

// Structure to hold player data
struct PlayerData {
    string name; // Stores the player's name
    int score; // Stores the player's score
    SpeedLevel difficulty; // Stores the difficulty level chosen by the player
};

// Function to convert SpeedLevel enum to a string for display
string SpeedLevelToString(SpeedLevel level) {
    switch(level) {
        case SpeedLevel::SLOW: return "Slow";
        case SpeedLevel::MEDIUM: return "Medium";
        case SpeedLevel::FAST: return "Fast";
        case SpeedLevel::VERY_FAST: return "Very Fast";
        default: return "Unknown";
    }
}

// Function to check if an element is in a deque of Vector2s
bool ElementInDeque(Vector2 element, deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++) {
        if(Vector2Equals(deque[i], element)) {
            return true; // Returns true if the element is found
        }
    }
    return false; // Returns false if the element is not found
}

// Function to check if a certain time interval has passed
bool eventTriggered(double interval){
    double currentTime = GetTime(); // Gets the current time
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime; // Updates the last update time
        return true; // Returns true if the interval has passed
    }
    return false; // Returns false otherwise
}

// Remaining code for classes GameMenu, Snake, Food, and Game...



class GameMenu {
    private:
        bool active; // Indicates whether the menu is currently active (visible) or not.
        vector<Color> colorOptions; // Stores the available color options for the background and the snake.
        int backgroundColorIndex; // Keeps track of the currently selected index for the background color.
        int snakeColorIndex; // Keeps track of the currently selected index for the snake color.
        bool mouseOnNameInputBox; // Flag to check if the mouse is hovering over the name input box.
        int speedLevelIndex = 0; // Stores the index of the currently selected speed level.
        Rectangle nameInputBox; // Represents the graphical area where the player inputs their name.
        string playerName; // Stores the player's name.
        vector<PlayerData> players; // Stores data for all players in the current session.

    public:
        // Constructor initializes the menu with the first color options and sets the menu as active.
        GameMenu() : active(true), backgroundColorIndex(0), snakeColorIndex(1) {
            nameInputBox = {400, 320, 325, 50}; // Sets the position and size of the name input box.
            colorOptions = {{255, 255, 0, 255}, {255, 165, 0, 255}, {173, 204, 96, 255}, {0, 0, 0, 255}, {128, 0, 128, 255}};
        }

        // Activates the menu.
        void Activate() { active = true; }
        
        // Returns whether the menu is active.
        bool IsActive() const { return active; }
        
        // Deactivates the menu.
        void Deactivate() { active = false; }
        
        // Returns the player's name.
        string GetPlayerName() const { return playerName; }

        // Renders the menu screen.
        void DisplayMenu() {
            ClearBackground(RAYWHITE); // Clears the screen and sets a white background.
            DrawText("Game Menu", 400, 50, 20, BLACK); // Displays the menu title.
            DrawText("Use arrow keys to change colors", 350, 100, 20, DARKGRAY); // Shows instructions for color selection.
            DrawText("Background Color", 400, 150, 20, BLACK); // Label for background color selection.
            DrawRectangle(600, 145, 20, 20, colorOptions[backgroundColorIndex]); // Shows the selected background color.
            DrawText("Snake Color", 400, 200, 20, BLACK); // Label for snake color selection.
            DrawRectangle(600, 195, 20, 20, colorOptions[snakeColorIndex]); // Shows the selected snake color.
            DrawText("Enter your name: ", 400, 300, 20, BLACK); // Prompts for name input.
            DrawRectangleRec(nameInputBox, LIGHTGRAY); // Renders the name input box.
            DrawRectangleLines((int)nameInputBox.x, (int)nameInputBox.y, (int)nameInputBox.width, (int)nameInputBox.height, DARKGRAY); // Draws the outline for the input box.
            DrawText(playerName.c_str(), (int)nameInputBox.x + 5, (int)nameInputBox.y + 8, 40, MAROON); // Displays the entered name.
            DrawText("Press ENTER to start", 400, 500, 20, DARKBLUE); // Indicates how to start the game.
            DrawText("Speed Level < >", 400, 250, 20, BLACK); // Label for speed level selection.
            const char* speedLevels[] = {"Slow", "Medium", "Fast", "Very Fast"}; // Speed level options.
            DrawText(speedLevels[speedLevelIndex], 600, 245, 20, BLACK); // Displays the selected speed level.
        }

        // Handles user input for the menu.
        void HandleInput() {
            mouseOnNameInputBox = CheckCollisionPointRec(GetMousePosition(), nameInputBox); // Updates whether the mouse is over the name input box.
            if (IsKeyPressed(KEY_ENTER)) {
                active = false; // Deactivates the menu if Enter is pressed.
            }
            // The following if-statements handle the arrow key inputs for selecting colors and speed levels.
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
            if (backgroundColorIndex == snakeColorIndex) { //!!!Monocolor game check
                snakeColorIndex = (snakeColorIndex + 1) % colorOptions.size(); // Ensures snake and background colors are not the same.
            }
            // Handles player name input.
            if (mouseOnNameInputBox) {
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key >= 32) && (key <= 125) && (playerName.length() < MAX_INPUT_CHARS)) {
                        playerName += (char)key; // Adds characters to the player name.
                    }
                    key = GetCharPressed(); // Gets the next character.
                }
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (!playerName.empty()) {
                        playerName.pop_back(); // Removes the last character in the name.
                    }
                }
            }
            // Handles speed level selection.
            if (IsKeyPressed(KEY_COMMA)) {
                speedLevelIndex = (speedLevelIndex - 1 + 4) % 4; // Cycles backwards through the speed levels.
            }
            if (IsKeyPressed(KEY_PERIOD)) {
                speedLevelIndex = (speedLevelIndex + 1) % 4; // Cycles forwards through the speed levels.
            }
        }

        // Returns the selected speed level.
        SpeedLevel GetSpeedLevel() const {
            switch(speedLevelIndex) {
                case 0: return SpeedLevel::SLOW;
                case 1: return SpeedLevel::MEDIUM;
                case 2: return SpeedLevel::FAST;
                case 3: return SpeedLevel::VERY_FAST;
                default: return SpeedLevel::SLOW;
            }
        }

        // Returns the currently selected background color.
        Color GetBackgroundColor() const {
            return colorOptions[backgroundColorIndex];
        }

        // Returns the currently selected snake color.
        Color GetSnakeColor() const {
            return colorOptions[snakeColorIndex];
        }        
    };


class Snake {
    public:
        // A deque to represent the snake's body, each segment as a Vector2 (x, y position).
        deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        // The direction the snake is moving in, initially set to move right (1, 0).
        Vector2 direction = {1, 0};
        // A flag to indicate whether a new segment should be added to the snake's body.
        bool addSegment = false;
        // Method to draw the snake on the screen.
        void Draw() {
            // Iterates through each segment in the snake's body.
            for(long unsigned int i = 0; i < body.size(); i++) {
                // Creates a rectangle to visually represent each segment.
                Rectangle segment = Rectangle{
                    offset + body[i].x * cellSize, // X position
                    offset + body[i].y * cellSize, // Y position
                    (float)cellSize,              // Width
                    (float)cellSize               // Height
                };
                // Draws the segment as a rounded rectangle using the snake's color.
                DrawRectangleRounded(segment, 0.5, 6, snakeColor);
            }
        }
        // Method to update the snake's state, including its position and size.
        void Update() {
            // Adds a new segment to the front of the snake in its current direction.
            body.push_front(Vector2Add(body[0], direction));
            // Checks if a new segment should be added.
            if(addSegment) {
                addSegment = false; // Resets the flag if a segment was added.
            } else {
                body.pop_back(); // Removes the last segment of the body if not growing.
            }
        }
        // Method to reset the snake to its initial state.
        void Reset() {
            // Reinitializes the snake's body to its starting size and position.
            body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
            // Resets the direction of the snake to move right.
            direction = {1, 0};
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
            eatSounds.push_back(LoadSound("Sounds/d.mp3"));
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
    // Private member variables
    GameMenu* menu; // Pointer to the game menu, for accessing and modifying menu state.
    bool difficultyHasChanged; // Flag to track if the game's difficulty level has changed.
    SpeedLevel newDifficulty; // Stores the new difficulty level when a change is requested.

    // Public member variables and methods
    public:
        Snake snake; // An instance of the Snake class, representing the player-controlled snake.
        Food food;   // An instance of the Food class, representing the food in the game.
        bool running = true; // Flag indicating whether the game is currently running.
        int score = 0; // Variable to keep track of the player's score.
        Sound wallSound; // Sound effect for when the snake hits a wall.
        string playerName; // Variable to store the current player's name.
        vector<PlayerData> players; // Vector to store data for all players (e.g., for a leaderboard).
        SpeedLevel speedLevel = SpeedLevel::SLOW; // The current speed level of the game.
        vector<string> messages = {"Learning Classes and algorithms by Snake", "Message after first food", "Message after second food"}; // Messages to display at various points in the game.
        int currentMessageIndex = 0; // Index to keep track of which message is currently displayed.

        // Method to calculate the update interval based on speed level and score
        double GetUpdateInterval() {
            double baseInterval = 0.2; // Default interval
            // Adjust interval based on speed level
            switch(speedLevel) {
                case SpeedLevel::SLOW: baseInterval = 0.2; break;
                case SpeedLevel::MEDIUM: baseInterval = 0.15; break;
                case SpeedLevel::FAST: baseInterval = 0.1; break;
                case SpeedLevel::VERY_FAST: baseInterval = 0.05; break;
            }
            // Adjust interval based on score
            double interval = baseInterval - (score / 1000.0);
            return (interval > 0.05) ? interval : 0.05; // Ensure interval doesn't get too low.
        }

        // Method to set the game menu
        void SetMenu(GameMenu* menu) {
            this->menu = menu;
        }

        // Method to request a change in game difficulty
        void ChangeDifficulty(SpeedLevel newLevel) {
            newDifficulty = newLevel;
            difficultyHasChanged = true;
        }

        // Method to update the game's difficulty
        void SetDifficulty(SpeedLevel newLevel) {
            speedLevel = newLevel;
            food = Food(snake.body, speedLevel); // Update food according to new difficulty.
        }

        // Constructor for the Game class
        Game() : snake(), food(snake.body, speedLevel), difficultyHasChanged(false), newDifficulty(SpeedLevel::SLOW) {
            wallSound = LoadSound("Sounds/Crash_wall.mp3"); // Load the sound for hitting a wall.
        }

        // Destructor for the Game class
        ~Game() {
            UnloadSound(wallSound); // Unload the wall collision sound.
        }

        // Method to draw game elements
        void Draw() {
            food.Draw();  // Draw the food.
            snake.Draw(); // Draw the snake.
        }

        // Method to update the game state
        void Update() {
            if (running) {
                snake.Update(); // Update the snake's position and size.
                CheckCollisionWithFood(); // Check for collisions with food.
                CheckCollisionWithEdges(); // Check for collisions with the edges of the game area.
                CheckCollisionWithTail();  // Check for collisions with the snake's own tail.

                // If the difficulty has changed, update it and reset the flag.
                if (difficultyHasChanged) {
                    SetDifficulty(newDifficulty);
                    difficultyHasChanged = false;
                }
            }
        }

        // Method to check for collisions with food
        void CheckCollisionWithFood() {
            if(Vector2Equals(snake.body[0], food.position)) {
                // If the snake's head is at the same position as the food
                food.position = food.GenerateRandomPos(snake.body); // Move the food to a new position.
                food.textureIndex = (food.textureIndex + 1) % food.textures.size(); // Change the food's texture.
                snake.addSegment = true; // Grow the snake.
                score += 10; // Increase the score.
                currentMessageIndex = (currentMessageIndex + 1) % messages.size(); // Update the message index.
                PlaySound(food.eatSounds[food.soundIndex]); // Play the eating sound.
                food.soundIndex = (food.soundIndex + 1) % food.eatSounds.size(); // Change the eating sound.
            }
        }

        // Method to check for collisions with the game area edges
        void CheckCollisionWithEdges() {
            // Check if the snake's head is beyond the game area boundaries
            if(snake.body[0].x < 0 || snake.body[0].x >= cellCount || snake.body[0].y < 0 || snake.body[0].y >= cellCount) {
                GameOver(); // End the game if there is a collision.
            }
        }

        // Method to sort the leaderboard
        void SortLeaderboard() {
            sort(players.begin(), players.end(), [](const PlayerData& a, const PlayerData& b) {
                return a.score > b.score; // Sort players by score in descending order.
            });
        }

        // Method to handle game over scenarios
        void GameOver() {
            PlayerData playerData;
            playerData.name = playerName;
            playerData.score = score;
            playerData.difficulty = speedLevel; // Record the difficulty level at game over.
            players.push_back(playerData); // Add the player's data to the leaderboard.
            snake.Reset(); // Reset the snake to its initial state.
            food.position = food.GenerateRandomPos(snake.body); // Move the food to a new position.
            running = false; // Stop the game.
            SortLeaderboard(); // Update the leaderboard.
            score = 0; // Reset the score.
            PlaySound(wallSound); // Play the wall collision sound.
            menu->Activate(); // Reactivate the game menu.
        }

        // Method to check for collisions with the snake's own body
        void CheckCollisionWithTail() {
            // Iterate over the snake's body segments
            for(size_t i = 1; i < snake.body.size(); ++i) {
                if(Vector2Equals(snake.body[0], snake.body[i])) {
                    GameOver(); // End the game if the snake collides with its tail.
                    break;
                }
            }
        }
    };

int main() {
    // Initialize the game window with a specific size and title
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");

    // Set the game to run at 60 frames per second
    SetTargetFPS(60);

    // Initialize the audio system for playing sounds
    InitAudioDevice();

    // Create instances of the GameMenu and Game classes
    GameMenu menu;
    Game game;

    // Pass the address of 'menu' to 'game' so it can interact with the menu
    game.SetMenu(&menu);

    // Main game loop - continues until the window close event is triggered
    while (!WindowShouldClose()) {
        // Start drawing graphics
        BeginDrawing();

        // Display and handle the game menu
        if (menu.IsActive()) {
            menu.DisplayMenu(); // Display the game menu
            menu.HandleInput(); // Handle player input in the menu

            // Configure the game based on the menu's settings
            if (!menu.IsActive()) {
                backgroundColor = menu.GetBackgroundColor(); // Set background color
                snakeColor = menu.GetSnakeColor(); // Set snake color
                game.playerName = menu.GetPlayerName(); // Set player's name
                game.speedLevel = menu.GetSpeedLevel(); // Set game speed level
            }
        } else {
            // Handle snake direction input
            if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && game.snake.direction.y != 1) {
                game.snake.direction = {0, -1}; // Move up
                game.running = true;
            }
            if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && game.snake.direction.y != -1) {
                game.snake.direction = {0, 1}; // Move down
                game.running = true;
            }
            if ((IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) && game.snake.direction.x != 1) {
                game.snake.direction = {-1, 0}; // Move left
                game.running = true;
            }
            if ((IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) && game.snake.direction.x != -1) {
                game.snake.direction = {1, 0}; // Move right
                game.running = true;
            }

            // Update game state if it's time based on the update interval
            if (eventTriggered(game.GetUpdateInterval())) {
                game.Update();
            }

            // Draw game elements
            ClearBackground(backgroundColor);
            DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, snakeColor);
            DrawText(game.messages[game.currentMessageIndex].c_str(), offset-52, 20, 40, snakeColor);
            DrawText(TextFormat("%i", game.score), offset+360, offset+cellSize*cellCount+10, 40, snakeColor);
            game.Draw();

            // Display player data after the game ends
            if (!game.running) {
                for (size_t i = 0; i < game.players.size(); ++i) {
                    string difficulty = SpeedLevelToString(game.players[i].difficulty);
                    DrawText(TextFormat("Player: %s - Score: %i - Difficulty: %s", game.players[i].name.c_str(), game.players[i].score, difficulty.c_str()), offset + 50, offset + 50 + i * 30, 20, snakeColor);
                }
            }
        }

        // End the drawing process
        EndDrawing();
    }

    // Cleanup resources before closing
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
