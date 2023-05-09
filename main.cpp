#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
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
const int GAME_SPEED = 3;

// Ball movement
const float INIT_BALL_SPEED = 1;
float ballSpeed = INIT_BALL_SPEED;
int ballDirectionX = 1;
int ballDirectionY = 1;
bool ballMoving = false;
bool gamePaused = false;
bool waitUntilQuit = true;

// Scoreboard
int leftScore = 0;
int rightScore = 0;
bool gameOn = true;

// Sounds
Mix_Chunk* paddleSound;
Mix_Chunk* wallSound;
Mix_Chunk* scoreSound;

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
    font = TTF_OpenFont("fonts/montserrat.ttf", 40);

    // Sound mixer
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    // Paddle sound w error handling
    paddleSound = Mix_LoadWAV("sounds/paddle.mp3");
    if (!paddleSound) {
        const char* mixError = Mix_GetError();
        std::cout << "Failed to load paddle sound effect: " << mixError << std::endl;

    }
    // Wall sound w error handling
    wallSound = Mix_LoadWAV("sounds/wall.mp3");
    if (!wallSound) {
        const char* mixError = Mix_GetError();
        std::cout << "Failed to load wall sound effect: " << mixError << std::endl;

    }
    // Score sound w error handling
    scoreSound = Mix_LoadWAV("sounds/score.mp3");
    if (!scoreSound) {
        const char* mixError = Mix_GetError();
        std::cout << "Failed to load score sound effect: " << mixError << std::endl;
    }
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
                case SDLK_q:
                    if (event.type == SDL_KEYDOWN)
                        waitUntilQuit = true;
                    break;
                case SDLK_ESCAPE:
                    if (event.type == SDL_KEYDOWN)
                        gamePaused = !gamePaused;
                    break;
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
                        Mix_PlayChannel(-1, paddleSound, 0);
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
        waitUntilQuit = false;
        gameOn = false;

    }
}

void moveBall()
{
    if (!gamePaused && ballMoving)
    {
        ball.x += ballSpeed * ballDirectionX;
        ball.y += ballSpeed * ballDirectionY;

        if (ball.y <= 0 || ball.y >= WINDOW_HEIGHT - BALL_SIZE)
        {
            Mix_PlayChannel(-1, wallSound, 0);
            ballDirectionY *= -1;
            ballSpeed += 0.2f;
        }

        if (SDL_HasIntersection(&ball, &leftPaddle) || SDL_HasIntersection(&ball, &rightPaddle))
        {
            Mix_PlayChannel(-1, paddleSound, 0);
            ballDirectionX *= -1;
            ballSpeed += 0.2f;
        }

        if (ball.x <= 0)
        {
            Mix_PlayChannel(-1, scoreSound, 0);
            increaseRightScore();
            checkScore();
            resetBall();
        }

        if (ball.x >= WINDOW_WIDTH - BALL_SIZE)
        {
            Mix_PlayChannel(-1, scoreSound, 0);
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

    if (!gameOn)
    {
        // Render the "GAME OVER" message in the center of the screen
        SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "GAME OVER", textColor);
        SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
        SDL_Rect gameOverRect = { WINDOW_WIDTH / 2 - gameOverSurface->w / 2, WINDOW_HEIGHT / 2 - gameOverSurface->h / 2, gameOverSurface->w, gameOverSurface->h };
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
        SDL_DestroyTexture(gameOverTexture);
        SDL_FreeSurface(gameOverSurface);

    }

    SDL_RenderPresent(renderer);
}



void cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Cleanup sound effects
    Mix_FreeChunk(paddleSound);
    Mix_FreeChunk(wallSound);
    Mix_FreeChunk(scoreSound);

    // Quit SDL_Mixer
    Mix_CloseAudio();
    Mix_Quit();
}

int main(int argc, char* argv[])
{
    SDL_SetError(SDL_GetError());
    SDL_ClearError();

    initialize();

    // Print SDL error, if any
    const char* sdlError = SDL_GetError();
    if (sdlError[0] != '\0') {
        std::cout << "SDL Error: " << sdlError << std::endl;
    }

    while (gameOn || !waitUntilQuit)
    {
        handleInput();
        movePaddle();
        moveBall();
        render();
        SDL_Delay(GAME_SPEED);
    }

    cleanup();
    return 0;


}
