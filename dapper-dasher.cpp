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
    Vector2 character_pos;

    character_rect.width = character.width/6; // is divided by 6 cause there are 6 squares. each square width will be the total width/6
    character_rect.height = character.height;
    character_rect.x = 0;
    character_rect.y = 0; // to fix the rectangle in the first square from left to right.
    
    character_pos.x = width/2 - character_rect.width/2;
    character_pos.y = height - character.height;
    
    //Hazards variables
    Texture2D hazard = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle hazard_rect{0.0, 0.0, hazard.width/8, hazard.height/8};
    Vector2 hazard_pos{width, height - hazard_rect.height};

    int hazard_vel{-200}; // velocity of the hazard (pixels/second)

    /*hazard_rect.width = hazard.width/8;
    hazard_rect.height = hazard.height/8;
    hazard_rect.x = 0;
    hazard_rect.y = 0;

    character_pos.x = width;
    character_pos.y = height - character_rect.height;*/


    int velocity{0}; //pixels per frame

    bool isInTheAir{};
    
    //Animations frame
    int frame{};
    int hazardFrame{};

    //time before update the animation frmae
    const float updateTime{1.0/12.0};
    const float hazardUpdateTime{1.0/12.0};

    float runningTime{};
    float hazardRunningTime{};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float dT = GetFrameTime();

        if(character_pos.y >= height - character_rect.height){ // checking wheter rect is on the ground
            velocity = 0;
            isInTheAir = false;
        } else {
            velocity += gravity * dT;
            isInTheAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInTheAir){
            velocity += jumpVel;
        }

        //update hazard position
        hazard_pos.x += hazard_vel * dT;

        //updating character position
        character_pos.y += velocity * dT;

        //Updating character animation frames
        runningTime += dT;
        if(!isInTheAir){
            if (runningTime >= updateTime)
            {
                runningTime = 0.0;
                //Update animation frame to move the rectangle to the next square of the sheet
                character_rect.x = frame * character_rect.width;
                frame++;

                if(frame > 5){
                    frame = 0; // to maintain the loop in 6 frames cause there are 6 sprites in the sheet
                }
            }
        }
        
        //update hazard animation frames        
        hazardRunningTime += dT;
        if(hazardRunningTime >= hazardUpdateTime){
            hazardRunningTime = 0;
            hazard_rect.x = hazardFrame * hazard_rect.width;
            hazardFrame++;

            if(hazardFrame > 7){
                hazardFrame = 0;
            }
        }

        //Drawing hazard
        DrawTextureRec(hazard, hazard_rect, hazard_pos, WHITE);

        //Draw Character
        DrawTextureRec(character, character_rect, character_pos, WHITE);
        
        // CODE GOES HERE
        EndDrawing();
    }

    UnloadTexture(character); // TO UNLOAD THE TEXTURE2D object
    UnloadTexture(hazard);
    CloseWindow();
}

