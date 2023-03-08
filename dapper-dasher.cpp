#include "raylib.h"

int main()
{
    const int width = 750;
    const int height = 750;

    InitWindow(width, height, "Dapper Dasher");
    SetTargetFPS(60);    
    
    //acceleration pixels per frame per frame
    const int gravity{1};
    const int jumpVel{-20};
 
    // rect dimensions
    const int rect_width{50};
    const int rect_height{80};

    int posY{height - rect_height};
    int velocity{0}; //pixels per frame

    bool isInTheAir{};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if(posY >= height - rect_height){ // checking wheter rect is on the ground
            velocity = 0;
            isInTheAir = false;
        } else {
            velocity += gravity;
            isInTheAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInTheAir){
            velocity += jumpVel;
        }

        posY += velocity;

        
        DrawRectangle(width/2, posY, rect_width, rect_height, BLACK);

        
        // CODE GOES HERE
        EndDrawing();
    }

    CloseWindow();
}

