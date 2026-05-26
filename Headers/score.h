#ifndef SCORE_H
#define SCORE_H

#define PANEL_WIDTH 300.0f
#define PANEL_OFFSET_X 40.0f
#define STAT_START_Y 100.0f
#define STAT_TITLE_SPACING 40.0f
#define STAT_BLOCK_SPACING 120.0f
#define DIVIDER_MARGIN 40.0f
#define DIVIDER_SPACING 60.0f
#define NEXT_TITLE_SPACING 50.0f
#define NEXT_BOX_MARGIN_X 50.0f
#define NEXT_BOX_HEIGHT 150.0f

class SCORE {
public:
    int totalScore;
    int linesCleared;
    int level;
    int cacheNextPiece;

    RenderTexture2D nextTarget;

    SCORE() : totalScore(0), linesCleared(0), level(1), cacheNextPiece(-1) {}

    void Initialize() {
        nextTarget = LoadRenderTexture(PANEL_WIDTH - (NEXT_BOX_MARGIN_X * 2.0f), NEXT_BOX_HEIGHT);
    }

    void Deinitialize() {
        UnloadRenderTexture(nextTarget);
    }

    void addLines(int lines) {
        if(lines <= 0) return;

        const int pointsArray[5] = {0, 100, 300, 600, 1200};
        int safeLines = lines > 4 ? 4 : lines;

        totalScore += pointsArray[safeLines] * level;
        linesCleared += lines;
        level = (linesCleared / 10) + 1;
    }

    void DrawCenteredText(const char* text, float x, float y, float width, int fontSize, Color color) {
        int textWidth = MeasureText(text, fontSize);
        DrawText(text, x + (width - textWidth) / 2.0f, y, fontSize, color);
    }

    void DrawStat(const char* title, const char* val, float x, float& y, float width, Color valColor) {
        DrawCenteredText(title, x, y, width, 30, Fade(WHITE, 0.5f));
        y += STAT_TITLE_SPACING;
        DrawCenteredText(val, x, y, width, 50, valColor);
        y += STAT_BLOCK_SPACING;
    }

    void Draw(float offsetX, float offsetY, float boardWidth, float boardHeight, Color borderColor, int nextPieceNum) {
        float x = offsetX + boardWidth + PANEL_OFFSET_X;
        
        DrawRectangleLinesEx({x - 4.0f, offsetY - 4.0f, PANEL_WIDTH + 8.0f, boardHeight + 8.0f}, 4.0f, borderColor);

        float currentY = offsetY + STAT_START_Y;
        
        DrawStat("SCORE", TextFormat("%06d", totalScore), x, currentY, PANEL_WIDTH, WHITE);
        DrawLine(x + DIVIDER_MARGIN, currentY, x + PANEL_WIDTH - DIVIDER_MARGIN, currentY, Fade(WHITE, 0.1f));
        currentY += DIVIDER_SPACING;
        
        DrawStat("LEVEL", TextFormat("%02d", level), x, currentY, PANEL_WIDTH, borderColor); 
        DrawLine(x + DIVIDER_MARGIN, currentY, x + PANEL_WIDTH - DIVIDER_MARGIN, currentY, Fade(WHITE, 0.1f));
        currentY += DIVIDER_SPACING;
        
        DrawStat("LINES", TextFormat("%03d", linesCleared), x, currentY, PANEL_WIDTH, WHITE);
        DrawLine(x + DIVIDER_MARGIN, currentY, x + PANEL_WIDTH - DIVIDER_MARGIN, currentY, Fade(WHITE, 0.1f));
        currentY += DIVIDER_SPACING;
        
        DrawCenteredText("NEXT", x, currentY, PANEL_WIDTH, 30, Fade(WHITE, 0.5f));
        currentY += NEXT_TITLE_SPACING;
        
        Rectangle nextBox = {x + NEXT_BOX_MARGIN_X, currentY, PANEL_WIDTH - (NEXT_BOX_MARGIN_X * 2.0f), NEXT_BOX_HEIGHT};
        DrawRectangleRec(nextBox, Fade(BLACK, 0.5f));
        DrawRectangleLinesEx(nextBox, 2.0f, Fade(borderColor, 0.5f)); 
        
        if (nextPieceNum != cacheNextPiece) {
            BeginTextureMode(nextTarget);
            ClearBackground(BLANK);

            float gridStartX = nextTarget.texture.width / 2.0f - (2.0f * CELL_SIZE);
            float gridStartY = nextTarget.texture.height / 2.0f - (2.0f * CELL_SIZE);
            
            gridStartX += (nextPieceNum == 0 || nextPieceNum == 1) ? 0.0f : 0.5f * CELL_SIZE;
            gridStartY += (nextPieceNum == 0) ? 0.5f * CELL_SIZE : ((nextPieceNum == 1) ? 0.0f : 1.0f * CELL_SIZE);
            
            for (int row = 0; row < TETRIMINO_HEIGHT; row++) {
                uint16_t m = Tetriminos[nextPieceNum][0][row] >> 6;
                
                if (m == 0) continue;
                
                int col = 6;
                float by = gridStartY + row * CELL_SIZE;
                
                while(m){
                    if (m & 8) {
                        DrawSprite(nextPieceNum, gridStartX + (col - 6) * CELL_SIZE, by, CELL_SIZE, CELL_SIZE);
                    }
                    
                    m = (m << 1) & 15;
                    col++;
                }
            }

            EndTextureMode();
            cacheNextPiece = nextPieceNum;
        }

        Rectangle sourceRec = {0.0f, 0.0f, (float)nextTarget.texture.width, (float)-nextTarget.texture.height};
        DrawTextureRec(nextTarget.texture, sourceRec, {nextBox.x, nextBox.y}, WHITE);
    }
};

#endif
