#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc == 1) {
      printf("请指定要查看的PPM图片的位置！\n");
      return 1;
    }

    FILE *file = fopen(argv[1], "rb");

    if (file == NULL) {
      printf("文件未找到！\n");
      return 1;
    }

    char *pnumber = calloc(3, sizeof(char));

    fgets(pnumber, 3, file);
    
    if (strcmp(pnumber, "P3") && strcmp(pnumber, "P6")) {
      printf("非PPM文件！\n");
      return 1;
    }
    
    char *pwh = calloc(200, sizeof(char));

    for (int i = 0; i <= 2; i ++) {
      fgets(pwh, 200, file);
    }

    int width = 0;

    int height = 0;

    sscanf(pwh, "%d %d", &width, &height);

    // 读取并丢弃第四行数据
    fgets(pwh, 200, file);
    free(pwh);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
      return 1;
    }

    const char *title = "ivr";
  
    SDL_Window *pwindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

    if (pwindow == NULL) {
      fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
    }

    SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

    unsigned char rgb[3];

    unsigned char r;

    unsigned char g;

    unsigned char b;

    Uint32 color = 0;

    SDL_Rect pixel = (SDL_Rect) {0, 0, 1, 1};
    
    for(int y = 0; y < height; y ++) {
      for(int x = 0; x < width; x ++) {

        if (!strcmp(pnumber, "P6")) {
           if (fread(rgb, 1, 3, file) != 3) {
            fprintf(stderr, "P6文件读取RGB数据失败！\n");
            return 1;
          }
          
            r = rgb[0];
            g = rgb[1];
            b = rgb[2];          
                   
        } else {
          int temp_r;
          int temp_g;
          int temp_b;
          if (fscanf(file, "%d %d %d", &temp_r, &temp_g, &temp_b) != 3) {
            fprintf(stderr, "P3文件读取RGB数据失败！\n");
            return 1;
          }

          r = temp_r;
          g = temp_g;
          b = temp_b;
        }
        
        color = SDL_MapRGB(psurface -> format, r, g, b);
        pixel.x = x;
        pixel.y = y;
        SDL_FillRect(psurface, &pixel, color);
      }
    }

    free(pnumber);

    SDL_UpdateWindowSurface(pwindow);
    
    SDL_Event event;

    int running = 1;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_Delay(16);
    }

    return 0;
}
