#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

const int WINDOW_WIDTH = 650;
const int WINDOW_HEIGHT = 450;

const int PADDLE_WIDTH = 15;
const int PADDLE_HEIGHT = 100;

const int BALL_SIZE = 10;

int paddle1Y = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2;
int paddle2Y = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2;
int ballX = WINDOW_WIDTH / 2;
int ballY = WINDOW_HEIGHT / 2;
int ballVelX = -4;
int ballVelY = 4;
int paddleSpeed = 8;
bool quit = false;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

Mix_Chunk* paddleHitSound = nullptr;
Mix_Chunk* wallHitSound = nullptr;


void init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Pong Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        std::cerr << "Mix_Init failed: " << Mix_GetError() << std::endl;
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        exit(1);
    }
    paddleHitSound = Mix_LoadWAV("paddle.wav");
    wallHitSound = Mix_LoadWAV("ball.wav");

    if (!paddleHitSound || !wallHitSound) {
        std::cerr << "Failed to load sound effects: " << Mix_GetError() << std::endl;
        exit(1);
    }
}


void close() {
    Mix_FreeChunk(paddleHitSound);
    Mix_FreeChunk(wallHitSound);

    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void draw() {
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for (int y = 0; y < WINDOW_HEIGHT; ++y){
        if (y % 15){
		SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
	}
}
    SDL_Rect paddle1 = {1, paddle1Y, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_Rect paddle2 = {WINDOW_WIDTH - PADDLE_WIDTH - 1, paddle2Y, PADDLE_WIDTH, PADDLE_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white color
    SDL_RenderFillRect(renderer, &paddle1);
    SDL_RenderFillRect(renderer, &paddle2);

    SDL_Rect ball = {ballX, ballY, BALL_SIZE, BALL_SIZE};
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);
}


void update() {
    ballX += ballVelX;
    ballY += ballVelY;

    if (ballY <= 0 || ballY + BALL_SIZE >= WINDOW_HEIGHT) {
        ballVelY = -ballVelY;
        Mix_PlayChannel(-1, wallHitSound, 0);
    }

    if (ballX <= PADDLE_WIDTH + 1 && ballY + BALL_SIZE >= paddle1Y && ballY <= paddle1Y + PADDLE_HEIGHT) {
        ballVelX = -ballVelX;
        Mix_PlayChannel(-1, paddleHitSound, 0);
    }

    if (ballX >= WINDOW_WIDTH - PADDLE_WIDTH - 1 - BALL_SIZE && ballY + BALL_SIZE >= paddle2Y && ballY <= paddle2Y + PADDLE_HEIGHT) {
        ballVelX = -ballVelX;
        Mix_PlayChannel(-1, paddleHitSound, 0);
    }

    if (ballX <= 0 || ballX >= WINDOW_WIDTH) {
        ballX = WINDOW_WIDTH / 2;
        ballY = WINDOW_HEIGHT / 2;
        ballVelX = -ballVelX;
        Mix_PlayChannel(-1, wallHitSound, 0);
    }
}


void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
    }
}
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W] && paddle1Y > 0) paddle1Y -= paddleSpeed;
    if (state[SDL_SCANCODE_S] && paddle1Y < WINDOW_HEIGHT - PADDLE_HEIGHT) paddle1Y += paddleSpeed;

    if (state[SDL_SCANCODE_UP] && paddle2Y > 0) paddle2Y -= paddleSpeed;
    if (state[SDL_SCANCODE_DOWN] && paddle2Y < WINDOW_HEIGHT - PADDLE_HEIGHT) paddle2Y += paddleSpeed;
}


int main(int argc, char* argv[]) {
    init();

    while (!quit) {
        handleInput();
        update();
        draw();
        SDL_Delay(16);
    }
    close();
    return 0;
}
