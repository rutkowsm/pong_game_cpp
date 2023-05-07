#include <SDL.h>
#include <iostream>

// Screen dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Pddle dimensions
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;

// Ball dimensions
const int BALL_SIZE = 10;

// Initial positions of the paddles and the ball
const int LEFT_PADDLE_X = 30;
const int RIGHT_PADDLE_X = WINDOW_WIDTH - 30;
const int PADDLE_START_Y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;
const int BALL_START_X = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
const int BALL_START_Y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;

// Paddle movement speed
const int PADDLE_SPEED = 6;

// Ball movement
const int BALL_SPEED = 1;
int ballDirectionX = 1;
int ballDirectionY = 1;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
SDL_Rect rightPaddle;
SDL_Rect leftPaddle;
SDL_Rect ball;

bool upKeyPressed = false;
bool downKeyPressed = false;
bool wKeyPressed = false;
bool sKeyPressed = false;

void initialize()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Right paddle
    rightPaddle.x = RIGHT_PADDLE_X;  // Set the right paddle's initial x-position to the right side
    rightPaddle.y = PADDLE_START_Y;  // Set the right paddle's initial y-position to the center
    rightPaddle.w = PADDLE_WIDTH;
    rightPaddle.h = PADDLE_HEIGHT;

    // Left paddle
    leftPaddle.x = LEFT_PADDLE_X;   // Set the left paddle's initial x-position to the left side
    leftPaddle.y = PADDLE_START_Y;  // Set the left paddle's initial y-position to the center
    leftPaddle.w = PADDLE_WIDTH;
    leftPaddle.h = PADDLE_HEIGHT;

    // Ball
    ball.x = BALL_START_X;
    ball.y = BALL_START_Y;
    ball.w = BALL_SIZE;
    ball.h = BALL_SIZE;
}

void handleInput()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            SDL_Quit();
        else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                    upKeyPressed = event.type == SDL_KEYDOWN;
                    break;
                case SDLK_DOWN:
                    downKeyPressed = event.type == SDL_KEYDOWN;
                    break;
                case SDLK_w:
                    wKeyPressed = event.type == SDL_KEYDOWN;
                    break;
                case SDLK_s:
                    sKeyPressed = event.type == SDL_KEYDOWN;
                    break;
                default:
                    break;
            }
        }
    }
}

void move_paddle()
{
    if (upKeyPressed)
        rightPaddle.y -= 1;  // Reduce the y-position change for slower movement
    if (downKeyPressed)
        rightPaddle.y += 1;  // Increase the y-position change for slower movement
    if (wKeyPressed)
        leftPaddle.y -= 1;  // Reduce the y-position change for slower movement
    if (sKeyPressed)
        leftPaddle.y += 1;  // Increase the y-position change for slower movement
}

void moveBall()
{
    ball.x += BALL_SPEED * ballDirectionX;
    ball.y += BALL_SPEED * ballDirectionY;

}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &leftPaddle);
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderPresent(renderer);
}

void cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    initialize();

    while (true)
    {
        handleInput();
        move_paddle();
        moveBall();
        render();
        SDL_Delay(PADDLE_SPEED);  // Introduce a delay of 10 milliseconds to slow down the loop
    }

    cleanup();
    return 0;
}
