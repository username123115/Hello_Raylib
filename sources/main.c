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

typedef struct BulletNode{
    Bullet bullet;
    struct BulletNode *next;
} BulletNode;

void remove_middle_node(BulletNode*, BulletNode*);

const int screenWidth = 650;
const int screenHeight = 800;

static Player player;
BulletNode *FirstBullet = NULL;
BulletNode *next = NULL; //for iterating


int main(void)
{
    player.position = (Vector2) {screenWidth / 2, screenHeight * 7/8};
    player.size = (Vector2) {25, 10};
    player.speed = (Vector2) {5, 0};

    InitWindow(screenWidth, screenHeight, WINDOW_TITLE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
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
            FirstBullet->bullet.size = (Vector2) {5, 10};
            FirstBullet->bullet.speed = (Vector2) {0, -5};
        }
        if (IsKeyPressed(KEY_C)) {
            while (FirstBullet) {
                BulletNode *temp = FirstBullet;
                FirstBullet = temp->next;
                free(temp);
    }
        }

        next = FirstBullet;
        BulletNode *prev = FirstBullet;
        bool firstnode = true;
        while (next) {
            BulletNode *tmp = next;
            next = tmp->next;
            tmp->bullet.position.x = tmp->bullet.position.x + tmp->bullet.speed.x;
            tmp->bullet.position.y = tmp->bullet.position.y + tmp->bullet.speed.y;
            if ((tmp->bullet.position.y <= 20) || (tmp->bullet.position.y >= screenHeight))
            {
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
        next = FirstBullet;
        while (next) {
            BulletNode *tmp = next;
            next = tmp->next;
            DrawRectangleV(tmp->bullet.position, tmp->bullet.size, SKYBLUE);
        }
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
