#include <SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
SDL_Rect rightPaddle;
SDL_Rect leftPaddle;

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

    rightPaddle.x = 750;  // Set the right paddle's initial x-position to the right side
    rightPaddle.y = 250;  // Set the right paddle's initial y-position to the center
    rightPaddle.w = 20;
    rightPaddle.h = 100;

    leftPaddle.x = 30;   // Set the left paddle's initial x-position to the left side
    leftPaddle.y = 250;  // Set the left paddle's initial y-position to the center
    leftPaddle.w = 20;
    leftPaddle.h = 100;
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

void update()
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

void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &leftPaddle);
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
        update();
        render();
        SDL_Delay(6);  // Introduce a delay of 10 milliseconds to slow down the loop
    }

    cleanup();
    return 0;
}
