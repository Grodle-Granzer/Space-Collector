#include <raylib.h>
#include <fstream>
#include <cmath>
using namespace std;

enum GameScreen
{
    TITLE,
    GAMEPLAY,
    GAMEOVER
};
struct Enemy
{
    Vector2 posEnemy;
    float speedEnemy;
};
struct bullets
{
    Rectangle rect;
    bool active;
};
struct Particles
{
    Vector2 position;
    Vector2 speed;
    float alpha;
    bool active;
};

static int high = 0;
int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Space Collector");
    InitAudioDevice();

    // File input
    ifstream loadFile("assets\\storage.bat");
    if (loadFile.is_open())
    {
        loadFile >> high;
        loadFile.close();
    }
    Image icon = LoadImage("assets\\Images\\spaceship.png");
    SetWindowIcon(icon);

    // Game
    GameScreen currentScreen = TITLE;
    Texture2D background = LoadTexture("assets\\Images\\Space.png");
    float scrollingy = 0.0f;
    float shakeIntensity = 0.0f;
    Music bgm = LoadMusicStream("assets\\Sound\\BGM.mp3");
    bgm.looping = true;
    PlayMusicStream(bgm);

    // Player setup
    Vector2 player = {screenWidth / 2, screenHeight / 2};
    float speed = 250.0f;
    Texture2D playerImg = LoadTexture("assets\\Images\\spaceship.png");

    // Bullets setup
    const int maxBullets = 10;
    bullets Bullets[maxBullets];
    for (int i = 0; i < maxBullets; i++)
        Bullets[i].active = false;
    float lastShootTime = 0;
    Sound explo = LoadSound("assets\\Sound\\Explosion.mp3");

    // Enemy Setup
    const int nEnemy = 10;
    Enemy enemy[nEnemy];
    for (int i = 0; i < nEnemy; i++)
    {
        enemy[i].posEnemy = {(float)GetRandomValue(0, 800), (float)GetRandomValue(-400, 0)};
        enemy[i].speedEnemy = (float)GetRandomValue(100, 200);
    }
    Texture2D enemyRock = LoadTexture("assets\\Images\\Rock.png");

    // Coin Setup
    Vector2 coin = {(float)GetRandomValue(0, 800), (float)GetRandomValue(0, 600)};
    int score = 0;
    Sound coinSound = LoadSound("assets\\Sound\\coin.mp3");

    // Particle System Setup
    const int maxParticles = 100;
    Particles particles[maxParticles];
    for (int i = 0; i < maxParticles; i++)
    {
        particles[i].active = false;
        particles[i].alpha = 0.0f;
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        scrollingy -= 2.0f;
        if (shakeIntensity > 0)
            shakeIntensity -= 2.0f * deltaTime;

        if (scrollingy <= -background.height)
            scrollingy = 0;
        float blinking = 8.0f;
        float alpha = ((sin(GetTime() * blinking) + 1.0f) / 2.0f);
        float alphaCoin = 0.8f;
        // Rectangle object
        Rectangle playerRec = {player.x, player.y, 70, 57};
        Rectangle coinRec = {coin.x, coin.y, 10, 10};

        // Particle System Logic
        for (int i = 0; i < maxParticles; i++)
        {
            if (particles[i].active)
            {
                particles[i].position.x += particles[i].speed.x * deltaTime;
                particles[i].position.y += particles[i].speed.y * deltaTime;
                particles[i].alpha -= 1.0f * deltaTime;

                if (particles[i].alpha <= 0.0f)
                    particles[i].active = false;
            }
        }
        UpdateMusicStream(bgm);
        switch (currentScreen)
        {
        case TITLE:
            if (IsKeyPressed(KEY_ENTER))
                currentScreen = GAMEPLAY;
            break;
        case GAMEPLAY:
            // Player Logic
            if (IsKeyDown(KEY_D) && player.x + 70 < screenWidth)
                player.x += speed * deltaTime;
            if (IsKeyDown(KEY_A) && player.x > 0)
                player.x -= speed * deltaTime;
            if (IsKeyDown(KEY_W) && player.y > 0)
                player.y -= speed * deltaTime;
            if (IsKeyDown(KEY_S) && player.y + 57 < screenHeight)
                player.y += speed * deltaTime;

            // Bullets logic
            if (IsKeyDown(KEY_SPACE) && (GetTime() - lastShootTime) > 0.25f)
            {
                for (int i = 0; i < maxBullets; i++)
                {
                    if (!Bullets[i].active)
                    {
                        Bullets[i].rect = {player.x + 32, player.y, 5, 15};
                        Bullets[i].active = true;
                        lastShootTime = GetTime();
                        break;
                    }
                }
            }
            for (int i = 0; i < maxBullets; i++)
            {
                if (Bullets[i].active)
                {
                    Bullets[i].rect.y -= 500 * deltaTime;
                    if (Bullets[i].rect.y < -10)
                        Bullets[i].active = false;
                }
            }
            // check collision between coin and player
            if (CheckCollisionRecs(playerRec, coinRec))
            {
                score += 100;
                coin.x = GetRandomValue(0, screenWidth);
                coin.y = GetRandomValue(0, screenHeight);
                PlaySound(coinSound);
                alphaCoin = sin((GetTime() * blinking) + 1.0f) / 2.0f;
            }

            // Enemy Logic
            for (int i = 0; i < nEnemy; i++)
            {
                enemy[i].posEnemy.y += enemy[i].speedEnemy * deltaTime;
                if (enemy[i].posEnemy.y > 600)
                    enemy[i].posEnemy.y = (float)GetRandomValue(-400, 0);
                for (int j = 0; j < maxBullets; j++)
                {
                    if (Bullets[j].active && CheckCollisionRecs(Bullets[j].rect, {enemy[i].posEnemy.x, enemy[i].posEnemy.y, 20, 20}))
                    {
                        score += 50;
                        Vector2 blastPosition = {Bullets[j].rect.x, Bullets[j].rect.y};
                        Bullets[j].active = false;
                        enemy[i].posEnemy = {(float)GetRandomValue(10, 790), (float)GetRandomValue(-400, 0)};
                        enemy[i].speedEnemy = (float)GetRandomValue(100, 200);
                        int spawned = 0;
                        for (int k = 0; k < maxParticles && spawned < 20; k++)
                        {
                            if (!particles[k].active)
                            {
                                particles[k].active = true;
                                particles[k].position = blastPosition;
                                particles[k].speed = {(float)GetRandomValue(-200, 200), (float)GetRandomValue(-200, 200)};
                                particles[k].alpha = 1.0f;
                                spawned++;
                            }
                        }
                        PlaySound(explo);
                    }
                }
                if (CheckCollisionRecs(playerRec, {enemy[i].posEnemy.x, enemy[i].posEnemy.y, 20, 20}))
                {
                    shakeIntensity = 0.5f;
                    int spawned = 0;
                    for (int k = 0; k < maxParticles && spawned < 20; k++)
                    {
                        if (!particles[k].active)
                        {
                            particles[k].active = true;
                            particles[k].position = {playerRec.x + (70 / 2), playerRec.y + (54 / 2)};
                            particles[k].speed = {(float)GetRandomValue(-200, 200), (float)GetRandomValue(-200, 200)};
                            particles[k].alpha = 1.0f;
                            spawned++;
                        }
                    }
                    PlaySound(explo);
                    if (score > high)
                        high = score;
                    ofstream saveFile("assets\\storage.bat");
                    if (saveFile.is_open())
                    {
                        saveFile << high;
                        saveFile.close();
                    }
                    currentScreen = GAMEOVER;
                    break;
                }
            }
            break;
        case GAMEOVER:
            if (IsKeyPressed(KEY_ENTER))
            {
                score = 0;
                // player = {screenWidth / 2, screenHeight / 2};
                for (int i = 0; i < nEnemy; i++)
                {
                    enemy[i].posEnemy = {(float)GetRandomValue(10, 790), (float)GetRandomValue(-400, 0)};
                    enemy[i].speedEnemy = (float)GetRandomValue(100, 200);
                }
                currentScreen = GAMEPLAY;
            }
            break;
        }

        BeginDrawing();
        {
            ClearBackground(BLACK);
            Vector2 shakeOffset = {0, 0};
            if (shakeIntensity > 0)
            {
                shakeOffset.x = GetRandomValue(-10, 10) * shakeIntensity;
                shakeOffset.y = GetRandomValue(-10, 10) * shakeIntensity;
            }

            DrawTextureEx(background, {shakeOffset.x, scrollingy + shakeOffset.y}, 0.0f, 1.0f, WHITE);
            DrawTextureEx(background, {shakeOffset.x, scrollingy + background.height + shakeOffset.y}, 0.0f, 1.0f, WHITE);

            for (int i = 0; i < maxParticles; i++)
            {
                if (particles[i].active)
                    DrawCircleV({particles[i].position.x + shakeOffset.x, particles[i].position.y + shakeOffset.y}, 2.4f, Fade(ORANGE, particles[i].alpha));
            }
            switch (currentScreen)
            {
            case TITLE:
                DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.6f));
                DrawText("SPACE COLLECTOR", 220, 250, 40, RED);
                DrawText("Press Enter to Start", 300, 310, 20, Fade(PURPLE, alpha));
                if (high != 0)
                    DrawText(TextFormat("Highest Score: %d", high), 340, 360, 15, Fade(SKYBLUE, 0.5f));
                break;
            case GAMEPLAY:

                DrawTexture(playerImg, player.x, player.y, WHITE);
                DrawCircle(coin.x, coin.y, 8, YELLOW);
                for (int i = 0; i < maxBullets; i++)
                {
                    if (Bullets[i].active)
                        DrawRectangleRec(Bullets[i].rect, YELLOW);
                }
                for (int i = 0; i < nEnemy; i++)
                    // DrawRectangleRec({enemy[i].posEnemy.x, enemy[i].posEnemy.y, 20, 20}, RED);
                    DrawTexture(enemyRock, enemy[i].posEnemy.x, enemy[i].posEnemy.y, WHITE);
                DrawText(TextFormat("Score: %d", score), 10, 10, 20, Fade(GREEN, alphaCoin));
                break;
            case GAMEOVER:
                DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.8f));
                DrawText(TextFormat("Your Score: %d \t\t High Score: %d", score, high), 270, 270, 15, Fade(GREEN, 0.4f));
                DrawText("HULL has been DESTROYED", 160, 300, 40, RED);
                DrawText("Press ENTER to RE-DEPLoY...", 250, 360, 20, SKYBLUE);
                break;
            }
        }
        EndDrawing();
    }
    UnloadTexture(playerImg);
    UnloadTexture(background);
    UnloadTexture(enemyRock);
    UnloadMusicStream(bgm);
    UnloadSound(explo);
    UnloadSound(coinSound);
    UnloadImage(icon);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}