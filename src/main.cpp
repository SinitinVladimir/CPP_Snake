#include <iostream>     // standard I/O library for input and output operations
#include <raylib.h>     // the Raylib library for graphics and game development
#include <deque>        // the deque (double-ended queue) container from the standard library
#include <raymath.h>    // Raylib's math functions for vector operations

using namespace std;    // using standard library classes and functions without the 'std::' prefix

Color green = {173, 204, 96, 255};         // a green color for the background
Color darkGreen = {43, 51, 24, 255};       // a darker green color for the snake

int cellSize = 30;      // the size of each cell in pixels
int cellCount = 25;     // the number of cells in the grid

double lastUpdateTime = 0;   // the time of the last update for event handling

bool eventTriggered(double interval)
{
    double currentTime = GetTime();   // Retrieves the current time
    if (currentTime - lastUpdateTime >= interval) // Checks if the specified interval has passed
    {
        lastUpdateTime = currentTime; // Updates the last update time
        return true;                  // Returns true to trigger the event
    }
    return false;                     // Otherwise, returns false
}

class Snake {
    public:
        deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}}; // Initializes the snake's body as a deque of Vector2 positions
        Vector2 direction = {1, 0};   // the initial direction of the snake

    void Draw() {
        for(long unsigned int i = 0; i < body.size(); i++) // Iterates through each segment of the snake's body
        {
            int x = body[i].x; // Retrieves the x-coordinate of the segment
            int y = body[i].y; // Retrieves the y-coordinate of the segment
            Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize}; // Creates a rectangle for the segment
            DrawRectangleRounded(segment, 0.5, 6, darkGreen); // Draws the rounded rectangle for the segment
        }
    }

    void Update()
    {
        body.pop_back(); // Removes the last segment of the body
        body.push_front(Vector2Add(body[0], direction)); // Adds a new segment at the front in the current direction
    }
};

class Food {
public:
    Vector2 position;    // Stores the position of the food
    Texture2D texture;   // Stores the texture of the food

    Food() 
    {
        Image image = LoadImage("Graphics/Beluga_food.png"); // Loads the food image
        texture = LoadTextureFromImage(image); // Creates a texture from the loaded image
        UnloadImage(image);                   // Frees the memory used by the image
        position = GenerateRandomPos();       // Generates a random position for the food
    }

    ~Food()
    {
        UnloadTexture(texture); // Frees the memory used by the texture
    }

    void Draw() 
    {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE); // Draws the food texture at the specified position
    }

    Vector2 GenerateRandomPos() 
    {
        float x = GetRandomValue(0, cellCount - 1); // Generates a random x-value
        float y = GetRandomValue(0, cellCount - 1); // Generates a random y-value
        return Vector2{x, y};                       // Returns the new position as a Vector2
    }
};

class Game
{
public:
    Snake snake = Snake(); // Creates an instance of the Snake class
    Food food = Food();    // Creates an instance of the Food class

    void Draw()
    {
        food.Draw();       // Draws the food on the screen
        snake.Draw();      // Draws the snake on the screen
    }

    void Update()
    {
        snake.Update();    // Updates the state of the snake
    }    

    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], food.position)) // Checks for collision between the snake's head and the food
        {
            food.position = food.GenerateRandomPos(); // Generates a new position for the food
        }
    }
};

int main()
{
    cout<<"Starting the game..."<<endl; // Displays a start message
    InitWindow(cellSize*cellCount, cellSize*cellCount, "Retro Snake"); // Initializes the game window
    SetTargetFPS(60); // a stable target frame rate to not rely on hardware capacity

    Game game = Game(); // Creates a game instance

    while(WindowShouldClose() == false) // Keeps running until the window is closed
    {
        BeginDrawing(); // Starts the drawing process

        if (eventTriggered(0.2)) // Checks if it's time to update the game state
        {
            game.Update(); // Updates the game state
        }

        // Checks for key presses and updates the snake's direction accordingly
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
        }

        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
        }

        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
        }

        ClearBackground(green); // Clears the background with the green color
        game.Draw();            // Draws the game elements

        EndDrawing();           // Ends the drawing process
    }

    CloseWindow(); // Closes the game window
    return 0;
}
