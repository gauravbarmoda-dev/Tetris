#include "include.h"

#define WINDOW_WIDTH 1920.0f
#define WINDOW_HEIGHT 1080.0f

int main(){
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TETRIS");

    RenderTexture2D boardTarget = LoadRenderTexture(BOARD_WIDTH * CELL_SIZE + 2, BOARD_HEIGHT * CELL_SIZE + 2);
    RenderTexture2D gridTarget = LoadRenderTexture(BOARD_WIDTH * CELL_SIZE + 2, BOARD_HEIGHT * CELL_SIZE + 2);
    RenderTexture2D bgTarget = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    BOARD board;
    TETRIMINO shape;
    SCORE score;
    GameTheme theme;

    score.Initialize();

    const float boardCellWidth = BOARD_WIDTH * CELL_SIZE;
    const float boardCellHeight = BOARD_HEIGHT * CELL_SIZE;
    const float trueBoardWidth = boardCellWidth + 2.0f;
    const float trueBoardHeight = boardCellHeight + 2.0f;
    const float totalWidth = trueBoardWidth + PANEL_OFFSET_X + PANEL_WIDTH;
    const float offsetX = (WINDOW_WIDTH - totalWidth) / 2.0f;
    const float offsetY = (WINDOW_HEIGHT - trueBoardHeight) / 2.0f;
    
    const Rectangle sourceRec = {0.0f, 0.0f, (float)boardTarget.texture.width, (float)-boardTarget.texture.height};
    const Rectangle boardRect = {offsetX - 4.0f, offsetY - 4.0f, trueBoardWidth + 8.0f, trueBoardHeight + 8.0f};
    const Color TetrominoColors[7] = {RED, YELLOW, GREEN, BLUE, PURPLE, SKYBLUE, PINK};

    SetTargetFPS(60);
    LoadGameSprites();

        BeginTextureMode(gridTarget);
            ClearBackground(BLANK);
            board.DrawGrid();
        EndTextureMode();

        while(!WindowShouldClose()){

            float deltaTime = GetFrameTime();
            
            shape.Update(deltaTime);

            int curNum = shape.num;
            
            if(shape.CollisionReset(&board, &score)) theme.SetTarget(TetrominoColors[curNum]);
            
            theme.Update(deltaTime);
            
            Input(&shape, &board, deltaTime);
            
            shape.moveDOWN(&board, deltaTime, IsKeyDown(KEY_DOWN));
            
            BeginDrawing();

                if(theme.isDirty){
                    
                    BeginTextureMode(bgTarget);
                    
                        DrawRectangleGradientV(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, theme.bgTop, theme.bgBottom);
                    
                    EndTextureMode();
                    
                    theme.isDirty = false;
                }
                
                DrawTextureRec(bgTarget.texture, {0.0f, 0.0f, WINDOW_WIDTH, -WINDOW_HEIGHT}, {0, 0}, WHITE);

                if(board.isDirty){
                    
                    BeginTextureMode(boardTarget);
                        
                        ClearBackground(BLANK);
                        
                        board.DrawBoard();

                    EndTextureMode();
                    
                    board.isDirty = false;
                }

                DrawTextureRec(gridTarget.texture, sourceRec, {offsetX, offsetY}, WHITE);
                DrawTextureRec(boardTarget.texture, sourceRec, {offsetX, offsetY}, WHITE);

                DrawRectangleLinesEx(boardRect, 4.0f, theme.borderColor);

                shape.DrawGhost(&board, offsetX, offsetY);
                
                shape.DrawTetromino(offsetX, offsetY);
                
                score.Draw(offsetX, offsetY, trueBoardWidth, trueBoardHeight, theme.borderColor, shape.GetNextPieceNum());

            EndDrawing();
        }

        UnloadRenderTexture(boardTarget);
        UnloadRenderTexture(gridTarget);
        UnloadRenderTexture(bgTarget);
        score.Deinitialize();
    
    UnloadGameSprites();
    
    CloseWindow();
    
    return 0;
}
