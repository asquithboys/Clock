/* Clock Program v0.1.0
   Copyright (C) 2025  Oscar Ronberg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <getopt.h>
#include <stdlib.h>

#define VERSION "0.1.0"

const char *chars = ":012345677889";

int main(int argc, char **argv) {
  Color color = {0, 0, 0, 255};

  // Handle Options

  int c;

  while (1) {
    static struct option long_options[] =
      {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"color", required_argument, 0, 'c'},
        {0, 0, 0, 0}
      };

    int option_index = 0;

    c = getopt_long (argc, argv, "hvc:", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0:
      break;

    case 'h':
      puts("Displays a clock with local time\n\
\t-c --color   sets the color of the text\n\
\t-v --version displays the version and license info\n\
\t-h --help    displays this help message");
      exit(0);

    case 'v':
      printf("clock %s\n", VERSION);
      printf("Copyright (C) 2025 Oscar Ronberg\n");

      printf(("\
License GPLv3+: GNU GPL version 3 or later <%s>.\n\
This is free software: you are free to change and redistribute it.\n\
There in NO WARRANTY, to the extent permitted by law.\n\n\
"), "https://gnu.org/licenses/gpl.html");
      printf("Written by %s.\n", "Oscar Ronberg");
      exit(0);

    case 'c':
      int r, g, b;
      if (sscanf(optarg+1, "%02x%02x%02x", &r, &g, &b) != 3) {
          puts("Incorrect formating for color");
          exit(1);
        }
      color = (Color){r, g, b, 255};
      break;

    default:
      exit(1);
    }
  }

  const int screenWidth = 800;
  const int screenHeight = 450;

  const int fontHeight = screenHeight/3;

  InitWindow(screenWidth, screenHeight, "raylib Clock");

  int codepointCount = 0;
  int *codepoints = LoadCodepoints(chars, &codepointCount);

  Font font = LoadFontEx("resources/NotoSansMono-Regular.ttf", fontHeight,
                         codepoints, codepointCount);

  Vector2 size = MeasureTextEx(font, "31:00:23", fontHeight, 10);

  Vector2 textPos = {.x = (screenWidth - size.x)/2,
                     .y = (screenHeight - size.y)/2};

  Texture2D background = LoadTexture("resources/background.jpg");

  // Calculate Texture Scale Factor
  // Finds smallest side of image then scales to respective screen size

  float scaleFactor;
  scaleFactor = (float) screenWidth / background.width;
  Vector2 imgPos = {.x = 0,
                    .y = floor((screenHeight
                                - (scaleFactor * background.height))
                               /2)};
  if (scaleFactor < (float) screenHeight / background.height) {
    scaleFactor = (float) screenHeight / background.height;
    imgPos = (Vector2) {.x = floor(
                                 (screenWidth -scaleFactor * background.width)
                                 /2),
                        .y = 0};
  };

  SetTargetFPS(60);

  time_t t;
  char TIME[70];
  while (!WindowShouldClose()) {
    time(&t);
    strftime(TIME, sizeof(TIME), "%H:%M:%S", localtime(&t));
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTextureEx(background, imgPos, 0.0f, scaleFactor, WHITE);
    DrawTextEx(font, TIME, textPos, fontHeight, 10, color);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
