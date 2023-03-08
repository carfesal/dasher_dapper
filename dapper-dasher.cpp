#include "raylib.h"

int main()
{
    const int width = 750;
    const int height = 750;

    InitWindow(width, height, "Dapper Dasher");
    SetTargetFPS(60);    
    
    //acceleration pixels per second
    const int gravity{1000};

    // pixels per second
    const int jumpVel{-600};
    
    // SPRITE: 2D images
    //to load the sprite
    Texture2D character = LoadTexture("textures/scarfy.png");
    Rectangle character_rect;
    character_rect.width = character.width/6;
    character_rect.height = character.height;
    character_rect.x = 0;
    character_rect.y = 0;

    Vector2 character_pos;
    character_pos.x = width/2 - character_rect.width/2;
    character_pos.y = height - character.height;
    
    int velocity{0}; //pixels per frame

    bool isInTheAir{};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        float dT = GetFrameTime();
        if(character_pos.y >= height - character_rect.height){ // checking wheter rect is on the ground
            velocity = 0;
            isInTheAir = false;
        } else {
            velocity += gravity*dT;
            isInTheAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInTheAir){
            velocity += jumpVel;
        }

        character_pos.y += velocity*dT;

        
        DrawTextureRec(character, character_rect, character_pos, WHITE);
        
        // CODE GOES HERE
        EndDrawing();
    }
    UnloadTexture(character); // TO UNLOAD THE TEXTURE2D object
    CloseWindow();
}

