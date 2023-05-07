#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>
#include <string>

// Screen dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Paddle dimensions
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

// Scoreboard initial position
const int SCOREBOARD_X = WINDOW_WIDTH / 2;
const int SCOREBOARD_Y = 30;

// Paddle movement speed
const int PADDLE_SPEED = 6;

// Ball movement
const float INIT_BALL_SPEED = 1;
float ballSpeed = INIT_BALL_SPEED;
int ballDirectionX = 1;
int ballDirectionY = 1;
bool ballMoving = false;

// Scoreboard
int leftScore = 0;
int rightScore = 0;

bool gameOn = true;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
SDL_Rect rightPaddle;
SDL_Rect leftPaddle;
SDL_Rect ball;
TTF_Font* font;
SDL_Color textColor = { 255, 255, 255, 255 };

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

    // Scoreboard
    TTF_Init();
    font = TTF_OpenFont("montserrat.ttf", 40);
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
                case SDLK_SPACE:
                    if (event.type == SDL_KEYDOWN)
                        ballMoving = true;
                    break;
                default:
                    break;
            }
        }
    }

}

void movePaddle()
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


void resetBall()
{
    ball.x = BALL_START_X;
    ball.y = BALL_START_Y;
    ballSpeed = INIT_BALL_SPEED;
    ballMoving = false;
}

void increaseLeftScore()
{
    leftScore++;
}
void increaseRightScore()
{
    rightScore++;
}

void checkScore()
{
    if (rightScore >=5 || leftScore >= 5)
    {
        gameOn = false;
    }
}

void moveBall()
{
    if (ballMoving)
    {
        ball.x += ballSpeed * ballDirectionX;
        ball.y += ballSpeed * ballDirectionY;

        if (ball.y <= 0 || ball.y >= WINDOW_HEIGHT - BALL_SIZE)
        {
            ballDirectionY *= -1;
        }

        if (SDL_HasIntersection(&ball, &leftPaddle) || SDL_HasIntersection(&ball, &rightPaddle))
        {
            ballDirectionX *= -1;
            ballSpeed += 0.2f;
        }

        if (ball.x <= 0)
        {
            increaseRightScore();
            checkScore();
            resetBall();
        }

        if (ball.x >= WINDOW_WIDTH - BALL_SIZE)
        {
            increaseLeftScore();
            checkScore();
            resetBall();
        }
    }

}

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &leftPaddle);
    SDL_RenderFillRect(renderer, &ball);

    // Scoreboard
    // Render the scores
    std::string leftScoreStr = std::to_string(leftScore);
    std::string rightScoreStr = std::to_string(rightScore);

    SDL_Surface* leftScoreSurface = TTF_RenderText_Solid(font, leftScoreStr.c_str(), textColor);
    SDL_Texture* leftScoreTexture = SDL_CreateTextureFromSurface(renderer, leftScoreSurface);

    SDL_Surface* rightScoreSurface = TTF_RenderText_Solid(font, rightScoreStr.c_str(), textColor);
    SDL_Texture* rightScoreTexture = SDL_CreateTextureFromSurface(renderer, rightScoreSurface);

    SDL_Rect leftScoreRect = { WINDOW_WIDTH / 4, 10, leftScoreSurface->w, leftScoreSurface->h };
    SDL_Rect rightScoreRect = { WINDOW_WIDTH * 3 / 4, 10, rightScoreSurface->w, rightScoreSurface->h };

    SDL_RenderCopy(renderer, leftScoreTexture, NULL, &leftScoreRect);
    SDL_RenderCopy(renderer, rightScoreTexture, NULL, &rightScoreRect);

    SDL_DestroyTexture(leftScoreTexture);
    SDL_FreeSurface(leftScoreSurface);
    SDL_DestroyTexture(rightScoreTexture);
    SDL_FreeSurface(rightScoreSurface);

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
    SDL_SetError(SDL_GetError());  // Clear any existing error
    SDL_ClearError();  // Clear the SDL error state

    initialize();

    // Print SDL error, if any
    const char* sdlError = SDL_GetError();
    if (sdlError[0] != '\0') {
        std::cout << "SDL Error: " << sdlError << std::endl;
    }

    while (gameOn)
    {
        handleInput();
        movePaddle();
        moveBall();
        render();
        SDL_Delay(PADDLE_SPEED);  // Introduce a delay of 10 milliseconds to slow down the loop
    }

    cleanup();
    return 0;
}
