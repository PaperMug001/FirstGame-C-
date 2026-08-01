#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SPIKES 100

struct Spike
{
    float x, y;
    float speed;
    int alive;
};

struct Spike spikes[MAX_SPIKES];
int spikeCount = 0;

void SpawnEnemy(float x, float y)
{
    if (spikeCount >= MAX_SPIKES)
        return;

    struct Spike spike;

    spike.x = x;
    spike.y = y;
    spike.speed = 5.0f;
    spike.alive = 1;

    spikes[spikeCount] = spike;
    spikeCount++;
}


int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL failed: %s\n", SDL_GetError());
        return 1;
    }

    // makes the pixel sharp
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); 

    SDL_Window *window = SDL_CreateWindow(
        "My First Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        768,
        768,
        0
    );

    if (!window)
    {
        printf("Window failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    SDL_RenderSetLogicalSize(renderer, 128, 128); // world size as 128 128

    // Create Objects here
    SDL_Rect player = {60, 60, 8, 8};

    
    // Player state
    enum {NORMAL, DASHING};
    int state = NORMAL;
    int playerHealth = 100;

    // Variables I guess
    int dashSpeed = 5;
    int dashDirection = 1;
    int dashCoolDown = 200; // millisecond
    int canDash = 1;

    int dashTimer = 0;

    // Timer
    Uint32 lastDash = SDL_GetTicks();

    int running = 1;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
            {
                // LEFT AND RIGHT
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT && canDash && state == NORMAL)
                {
                    state = DASHING;
                    dashDirection = 1;
                    dashTimer = 100;

                    canDash = 0;
                    lastDash = SDL_GetTicks();
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT && canDash && state == NORMAL)
                {
                    state = DASHING;
                    dashDirection = -1;
                    dashTimer = 100;

                    canDash = 0;
                    lastDash = SDL_GetTicks();
                }
                                
            }

        }

        // Input and stuff
        const Uint8 *keys = SDL_GetKeyboardState(NULL);


                
        // Update
        if (state == DASHING)
        {
            player.x += dashDirection * dashSpeed;
            
            dashTimer -= 16;

            if (dashTimer <= 0)
            {
                state = NORMAL;
            }
        }

        Uint32 now = SDL_GetTicks();

        if (!canDash && now - lastDash >= dashCoolDown)
        {
            canDash  = 1; // enable dashing again
        }

        
        // Rendering Stuff (Probably)
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                
        SDL_RenderFillRect(renderer, &player);


        // Stuff i should not touch probably
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // idk helps in the key press and stuff
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
