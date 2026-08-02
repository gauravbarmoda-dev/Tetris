#ifndef BOARD_H
#define BOARD_H

#define BOARD_HEIGHT 32
#define BOARD_WIDTH 16
#define CELL_SIZE 33.0f
#define ALL_FULL 0b1111111111111111

class BOARD{
public:
    uint64_t rows[BOARD_HEIGHT] = {0};
    bool isDirty = true;

    uint16_t getRow(int y){
        if(y < 0) return 0;
        if(y >= BOARD_HEIGHT) return ALL_FULL;
        
        return (uint16_t)(rows[y] & ALL_FULL);
    }

    int ClearLines(){
        int write_y = BOARD_HEIGHT - 1;
        int linesCleared = 0;
        
        for(int read_y = BOARD_HEIGHT - 1; read_y >= 0; read_y--){
            rows[write_y] = rows[read_y];
            bool isFull = ((rows[read_y] & ALL_FULL) == ALL_FULL);
            
            linesCleared += isFull;
            write_y -= !isFull;
        }
        
        while(write_y >= 0){
            rows[write_y] = 0;
            write_y--;
        }
        
        if(linesCleared > 0) isDirty = true;
        
        return linesCleared;
    }

    void DrawGrid(){
        float boardPixelWidth = BOARD_WIDTH * CELL_SIZE;
        float boardPixelHeight = BOARD_HEIGHT * CELL_SIZE;
        
        for(int i = 1; i < BOARD_WIDTH; i++){
            
            Vector2 startpos = {(float)i * CELL_SIZE, 0};
            Vector2 endpos = {(float)i * CELL_SIZE, boardPixelHeight};

            DrawLineV(startpos, endpos, Fade(WHITE, 0.03f));
        }
        
        for(int i = 1; i < BOARD_HEIGHT; i++){
            
            Vector2 startpos = {0, (float)i * CELL_SIZE};
            Vector2 endpos = {boardPixelWidth, (float)i * CELL_SIZE};

            DrawLineV(startpos, endpos, Fade(WHITE, 0.03f));
        }
    }

    void DrawBoard(){
        
        for(int y = 0; y < BOARD_HEIGHT; y++){
            
            uint64_t row = rows[y];
            uint16_t collision = (uint16_t)(row & ALL_FULL);
            
            if(collision == 0) continue;

            float ypos = y * CELL_SIZE + 1;

            int x = BOARD_WIDTH - 1;
            
            while(collision){

                if(collision & 1){
                    int bit_0 = ((row >> 16) & 1);
                    int bit_1 = ((row >> 31) & 2);
                    int bit_2 = ((row >> 46) & 4);

                    int sprite_id = bit_0 | bit_1 | bit_2;

                    DrawSprite(sprite_id, x * CELL_SIZE + 1, ypos, CELL_SIZE, CELL_SIZE);
                }
                
                collision >>= 1;
                row >>= 1;
                x--;
            }
        }
    }
};

#endif