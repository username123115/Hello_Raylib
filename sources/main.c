#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#define WINDOW_TITLE "ERMAHGAWD ITS A GAME"

typedef struct Player {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
} Player;

typedef struct Bullet{
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    bool active;
} Bullet;
typedef struct Asteroid{
    int radius;
    Vector2 position;
    Vector2 speed;
    bool active;
} Asteroid;

typedef struct BulletNode{
    Bullet bullet;
    struct BulletNode *next;
} BulletNode;

void remove_middle_node(BulletNode*, BulletNode*);

const int ScreenWidth = 650;
const int ScreenHeight = 800;
const int MAX_ASTEROIDS = 15;
const double ASTEROID_ACCEL = 0.0001;

static Player player;
BulletNode *FirstBullet = NULL;
BulletNode *next = NULL; //for iterating
BulletNode *prev = NULL; //also for iterating
Asteroid Asteroids[MAX_ASTEROIDS] = { 0 };

int main(void)
{
    player.position = (Vector2) {ScreenWidth / 2, ScreenHeight * 7/8};
    player.size = (Vector2) {25, 10};
    player.speed = (Vector2) {5, 0};

    for (int i = 0; i < MAX_ASTEROIDS; i++)
    {
        Asteroids[i].radius = GetRandomValue(5, 15);
        Asteroids[i].active = false;
    }
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(ScreenWidth, ScreenHeight, WINDOW_TITLE);

    // SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        //Update
        //------------------------------------------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.position.x += player.speed.x;
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.position.x -= player.speed.x;
        if (IsKeyPressed(KEY_SPACE)) {
            BulletNode *OldFirst = FirstBullet;
            FirstBullet = malloc(sizeof(BulletNode));
            FirstBullet->next = OldFirst;
            FirstBullet->bullet.position = player.position;
            //center bullet onto player
            FirstBullet->bullet.size = (Vector2) {5, 10};
            FirstBullet->bullet.speed = (Vector2) {0, -5};
            FirstBullet->bullet.active = true;
            FirstBullet->bullet.position.x += (player.size.x - FirstBullet->bullet.size.x) / 2;

        }
        if (IsKeyPressed(KEY_C)) {
            while (FirstBullet) {
                BulletNode *temp = FirstBullet;
                FirstBullet = temp->next;
                free(temp);
    }
        }
        for (int i = 0; i < MAX_ASTEROIDS; i++)
        {
            if (Asteroids[i].active) {
                Asteroids[i].position.x += Asteroids[i].speed.x;
                Asteroids[i].position.y += Asteroids[i].speed.y;
                Asteroids[i].speed.y += Asteroids[i].speed.y * ASTEROID_ACCEL;
                if ((Asteroids[i].position.y >= ScreenHeight) || (Asteroids[i].position.x <= 0) || (Asteroids[i].position.x >= ScreenWidth))
                {
                    Asteroids[i].active = false;
                }
            }
        }

        next = FirstBullet;
        while (next) {
            BulletNode *tmp = next;
            next = tmp->next;
            if (tmp->bullet.active)
            {
                tmp->bullet.position.x = tmp->bullet.position.x + tmp->bullet.speed.x;
                tmp->bullet.position.y = tmp->bullet.position.y + tmp->bullet.speed.y;
                for (int i = 0; i < MAX_ASTEROIDS; i++)
                {
                    if (Asteroids[i].active)
                    {
                        // Rectangle bullet_info = {tmp->bullet.size.y, tmp->bullet.size.x, tmp->bullet.position.x, tmp->bullet.position.y};
                        Rectangle bullet_info = {tmp->bullet.position.x, tmp->bullet.position.y, tmp->bullet.size.x, tmp->bullet.size.y};
                        bool hit = CheckCollisionCircleRec(Asteroids[i].position, Asteroids[i].radius, bullet_info);
                        if (hit)
                        {
                            // tmp->bullet.active = false;
                            Asteroids[i].active = false;
                        }
                    }
                }
                if ((tmp->bullet.position.y <= 20) || (tmp->bullet.position.y >= ScreenHeight))
                {
                    tmp->bullet.active = false;
                }
            }
        }
        //-------------------------------
        //Respawn and removal logic
        for (int i = 0; i < MAX_ASTEROIDS; i++)
        {
            if (!Asteroids[i].active)
            {
                Asteroids[i].position.x = GetRandomValue(ScreenWidth * 0.25, ScreenWidth * 0.75);
                Asteroids[i].position.y = -30;
                Asteroids[i].speed.x = GetRandomValue(-1, 1);
                Asteroids[i].radius = GetRandomValue(5, 15);
                Asteroids[i].speed.y = GetRandomValue(1, 4);
                Asteroids[i].active = true;
            }
        }


        next = FirstBullet;
        prev = FirstBullet;
        bool firstnode = true;
        while (next) 
        {
            BulletNode *tmp = next;
            next = tmp-> next;
            if (tmp->bullet.active == false) {
                if (firstnode)
                {
                    FirstBullet = next;
                    free(tmp);
                }
                if (!firstnode)
                {
                    remove_middle_node(tmp, prev);
                }
                
            }
            if (!firstnode)
            {
                prev = prev->next;
            }
            firstnode = false;
        }

        //------------------------------------------------------------------------------------------------------------------

        //Draw
        //------------------------------------------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleV(player.position, player.size, SKYBLUE);

        for (int i = 0; i < MAX_ASTEROIDS; i++)
        {
            if (Asteroids[i].active)
            {
                DrawCircleV(Asteroids[i].position, Asteroids[i].radius, BLUE);
            }
        }

        next = FirstBullet;
        while (next) {
            BulletNode *tmp = next;
            next = tmp->next;
            DrawRectangleV(tmp->bullet.position, tmp->bullet.size, SKYBLUE);
        }

        DrawRectangle(0, 0, 10, 20, BLUE);
        DrawFPS(10, 10);
        EndDrawing();
        //------------------------------------------------------------------------------------------------------------------
    }
    //clean up and deallocate memory
    int count = 0;
    while (FirstBullet) {
        BulletNode *temp = FirstBullet;
        FirstBullet = temp->next;
        free(temp);
        count += 1;
    }
    CloseWindow();
    printf("removed %d node(s)\n", count);
    return 0;
}

void remove_middle_node(BulletNode* Remove, BulletNode* PrevNode)
{
    PrevNode->next = Remove->next;
    free(Remove);
}
