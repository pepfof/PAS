#include <SDL.h>
#include <SDL_ttf.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int winw = 1000;
int winh = 200;
int winx = 0;
int winy = 0;
int scroll = 1;
int done = 0;
double maxtime = -100000000;
float textspeed = 0.25;
int main(int argc, char ** argv)
{
    char message[128], string[128];
    char *argv0 = argv[0];
    int j;
    for (j=1; argv[j] && argv[j][0] == '-'; ++j) {  
            if (strcmp(argv[j], "-i") == 0) {
            if (sscanf (argv[++j], "%s", message) != 1) {
                fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory. -s for noscroll and -i for message");
                return(1);
            }
            }else
            if (strcmp(argv[j], "-w") == 0) {
            if (sscanf (argv[++j], "%d", &winw) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-h") == 0) {
            if (sscanf (argv[++j], "%d", &winh) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-x") == 0) {
            if (sscanf (argv[++j], "%d", &winx) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-y") == 0) {
            if (sscanf (argv[++j], "%d", &winy) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-t") == 0) {
            if (sscanf (argv[++j], "%lf", &maxtime) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-s") == 0) {
            if (sscanf (argv[++j], "%f", &textspeed) != 1) {
                return(1);
            }
            } else
            if (strcmp(argv[j], "-l") == 0) {
            if (sscanf (argv[++j], "%d", &scroll) != 1) {
                return(1);
            }
            } else 
            {
            fprintf(stderr, "Improper usage. use -w to specify screen width, -h: height, -x and -y are explanatory.");
            return(1);
        }
    }
   if (!strcmp(message, "")) strcpy(message, "DEFAULT_TEXT");
    argv += j;
    argc -= j;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font * font = TTF_OpenFont("ror.ttf", 100);
    
    SDL_Window * window = SDL_CreateWindow("pas-marquee", winx, winy, winw, winh, SDL_WINDOW_BORDERLESS);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Color bgcolor = { 37, 37, 37 };
    SDL_Color fgcolor = { 230, 230, 230 };
    SDL_Surface * fontsurface = TTF_RenderUTF8_Shaded(font, message, fgcolor, bgcolor);
    SDL_Texture * fonttexture = SDL_CreateTextureFromSurface(renderer, fontsurface);
    SDL_QueryTexture(fonttexture, 0, 0, 0, 0);
    int wbr;
    int wbb;
    SDL_GetWindowSize(window, &wbr, &wbb);
    SDL_Rect dstrect = {wbr, 0, fontsurface->w, fontsurface->h};
    dstrect.x=wbr;
    dstrect.y = wbb/2-(fontsurface->h/2);
    timespec timems;
    timespec initial;
    clock_gettime(CLOCK_MONOTONIC, &initial);
    int maxleftposition = 0;
    if(fontsurface->w>wbr){maxleftposition =(fontsurface->w*2);}
    else{maxleftposition=(wbr+fontsurface->w);}
    while (!quit)
    {
        SDL_PollEvent(&event);
        int olddone=done;
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }
        if(scroll!=1){dstrect.x=5;}
        else{
            if(!done){
            clock_gettime(CLOCK_MONOTONIC, &timems);}
            double timeelapsed = ((timems.tv_sec*1000+timems.tv_nsec/1000000)-(initial.tv_sec*1000+initial.tv_nsec/1000000));
        if(textspeed*timeelapsed>maxleftposition) {
            if(maxtime!=-100000000 and !done) maxtime-=(maxleftposition/textspeed);
            if(maxtime>=maxleftposition/textspeed | maxtime==-100000000){
            clock_gettime(CLOCK_MONOTONIC, &initial);}
            if(maxtime<maxleftposition/textspeed and maxtime!=-100000000){done = 1;};
        }
        dstrect.x = wbr-textspeed*timeelapsed;
        }
        if(olddone!=done){printf("2\n");};
        SDL_SetRenderDrawColor(renderer, 0x25, 0x25, 0x25, 0x25);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, fonttexture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(fonttexture);
    SDL_FreeSurface(fontsurface);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();  

    return 0;
}
