#ifndef SHAPE_H
#define SHAPE_H

#define LOCK_DELAY 0.5f
#define SMOOTH_SPEED 15.0f

class TETRIMINO {
public:
    inline static int bag[7] = {0, 1, 2, 3, 4, 5, 6};
    inline static int bagIndex = 7;
    
    inline static const uint64_t COLOR_TABLE[7] = {
        (unsigned long long)0x0000000000000001,
        (unsigned long long)0x0000000000010001,
        (unsigned long long)0x0000000100000001,
        (unsigned long long)0x0000000100010001,
        (unsigned long long)0x0001000000000001,
        (unsigned long long)0x0001000000010001,
        (unsigned long long)0x0001000100000001
    };

    uint64_t color_mult;
    const uint16_t* piece;
    
    float visualY, visualX;
    float fall_timer;
    float lockTimer;
    
    int py, px, dy, dx, num, rotatedState;
    int cachedGhostY;
    
    bool ghostDirty;
    bool isLanding;

    static void ShuffleBag(){
        for(int i = 0; i < 7; i++) bag[i] = i;

        for(int i = 6; i > 0; i--){
            int j = GetRandomValue(0, i);
            int temp = bag[i];
            bag[i] = bag[j];
            bag[j] = temp;
        }
        bagIndex = 0;
    }

    static int GetNextPieceNum() {
        if(bagIndex >= 7) ShuffleBag();
        return bag[bagIndex];
    }

    TETRIMINO() {Reset();}

    void Reset() {
        py = 0; px = 0; dy = 1; dx = 0;
        fall_timer = 0.0f;
        lockTimer = 0.0f;
        isLanding = false;
        cachedGhostY = 0;
        ghostDirty = true;
        
        if(bagIndex >= 7) ShuffleBag();
        num = bag[bagIndex++];
        
        color_mult = COLOR_TABLE[num];
        
        rotatedState = 0;
        visualY = 0.0f;
        visualX = 0.0f;

        piece = Tetriminos[num][rotatedState];
    }

    uint16_t shiftRow(uint16_t row, int shift){
        int right_shift = shift * (shift > 0);
        int left_shift = -shift * (shift < 0);
        
        return (row >> right_shift) << left_shift;
    }

    void Update(float deltaTime){
        visualY += ((float)py - visualY) * SMOOTH_SPEED * deltaTime;
        visualX += ((float)px - visualX) * SMOOTH_SPEED * deltaTime;
    }

    bool CollisionReset(BOARD* board, SCORE* score){
        if(!dy){
            for(int y = 0; y < TETRIMINO_HEIGHT; y++){
                if(piece[y]){
                    
                    int localY = y + py;
                    
                    if(localY >= 0 && localY < BOARD_HEIGHT){
                        uint64_t mask = shiftRow(piece[y], px);
                        
                        board->rows[localY] |= mask * color_mult;
                        board->isDirty = true;
                    }
                }
            }
            
            int lines = board->ClearLines();
            if(score) score->addLines(lines);
            
            Reset();
            return true;
        }

        return false;
    }

    bool isMoveValid(BOARD* board, int checkX, int checkY, int checkRotation){
        if(checkX >= BOARD_WIDTH || checkX <= -BOARD_WIDTH) return false;

        for(int i = 0; i < TETRIMINO_HEIGHT; i++){
            uint16_t row = Tetriminos[num][checkRotation][i];
            
            if(row == 0) continue;

            uint16_t shiftedRow = shiftRow(row, checkX);
            
            if(shiftRow(shiftedRow, -checkX) != row) return false;

            if(shiftedRow & board->getRow(i + checkY)) return false;
        }

        return true;
    }

    void Move(BOARD* board){
        if(0 == dx) return;
        
        int nextX = px + dx;

        if(isMoveValid(board, nextX, py, rotatedState)){
            px = nextX;
            ghostDirty = true;
        }

        dx = 0;
    }

    void moveDOWN(BOARD* board, float deltaTime, bool isSoftDrop, int level){
        if(isLanding){
            lockTimer += deltaTime;
            
            if(isMoveValid(board, px, py + 1, rotatedState)){
                isLanding = false;
            }

            else if(lockTimer >= LOCK_DELAY){
                dy = 0;
                return;
            }

            return;
        }
        
        float speed = isSoftDrop ? 4.0f : 1.0f;
        fall_timer += deltaTime * speed;
        
        float currentFallTick = 0.150f - ((level - 1) * 0.01f);
        if (currentFallTick < 0.016f) currentFallTick = 0.016f;
        
        while(fall_timer >= currentFallTick){
            fall_timer -= currentFallTick;
            int newY = py + dy;
            
            if(isMoveValid(board, px, newY, rotatedState)){
                py = newY;
                ghostDirty = true;
                lockTimer = 0.0f;
            }
            
            else{
                isLanding = true;
                return;
            }
        }   
    }

    void Rotate(BOARD* board){
        
        int nextRotation = (rotatedState + 1) & 3;
        
        const int kickX[] = {0, -1, 1, -2, 2,  0,  0, -1, 1};
        const int kickY[] = {0,  0, 0,  0, 0, -1, -2, -1, -1};

        for(int i = 0; i < 9; i++){
            int kx = kickX[i];
            int ky = kickY[i];
            
            if(isMoveValid(board, px + kx, py + ky, nextRotation)){
                px += kx;
                py += ky;

                rotatedState = nextRotation;
                ghostDirty = true;
                
                piece = Tetriminos[num][rotatedState];

                return;
            }
        }
    }

    void DrawPieceAt(float targetY, float drawX, float offsetX, float offsetY, bool isGhost){
        float baseX = offsetX + (drawX * CELL_SIZE) + 1;
        
        for(int y = 0; y < TETRIMINO_HEIGHT; y++){
            uint16_t m = piece[y] >> 6;
            
            if(m == 0) continue;
            
            float localY = offsetY + (y + targetY) * CELL_SIZE + 1;
            int x = 6;
            
            while(m) {
                if (m & 8) {
                    float localX = baseX + (x * CELL_SIZE); 
                    
                    if(isGhost){
                        DrawRectangle(localX, localY, CELL_SIZE - 2, CELL_SIZE - 2, Fade(WHITE, 0.04f));
                        DrawRectangleLines(localX, localY, CELL_SIZE - 2, CELL_SIZE - 2, Fade(WHITE, 0.1f));
                    } 
                    
                    else{
                        DrawSprite(num, localX, localY, CELL_SIZE, CELL_SIZE);
                    }
                }

                m = (m << 1) & 15;
                x++;
            }
        }
    }

    void DrawTetromino(float offsetX, float offsetY){
        DrawPieceAt(visualY, visualX, offsetX, offsetY, false);
    }

    void DrawGhost(BOARD* board, float offsetX, float offsetY){
        if(ghostDirty){
            cachedGhostY = py;
            
            while(isMoveValid(board, px, cachedGhostY + 1, rotatedState)){
                cachedGhostY++;
            }
            
            ghostDirty = false;
        }

        DrawPieceAt((float)cachedGhostY, visualX, offsetX, offsetY, true);
    }
};

#endif