#include <SDL2/SDL.h>
#include <stdio.h>

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

    // Variables I guess
    int dash = 10;
    int dashCoolDown = 100; // 1/10 sec
    int allowDash = 1;

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
        }

        // Input and stuff
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        
        if (keys[SDL_SCANCODE_LEFT] && allowDash)
        {
            player.x -= dash;
            allowDash = 0;
        }

        if (keys[SDL_SCANCODE_RIGHT] && allowDash)
        {
            player.x += dash;
            allowDash = 0;
        }
        
        // Update

        Uint32 now = SDL_GetTicks();

        if (now - lastDash >= dashCoolDown)
        {
            lastDash = now;
            allowDash = 1; // enable dashing again
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
