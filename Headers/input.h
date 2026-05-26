#ifndef INPUT_H
#define INPUT_H

float timer_right = 0.0f;
float timer_left = 0.0f;
const float DELAY = 0.116f;
const float SPEED = 0.033f;

void Input(TETRIMINO* piece, BOARD* board, float deltaTime){    

    // Move Right
    if(IsKeyPressed(KEY_RIGHT)){
        piece->dx = 1; piece->Move(board);
        timer_right = 0.0f;
    }

    else if(IsKeyDown(KEY_RIGHT)){
        timer_right += deltaTime;
        
        if(timer_right >= DELAY){
            piece->dx = 1; piece->Move(board);
            timer_right -= SPEED;
        }
    }

    else timer_right = 0.0f;

    // Move Left
    if(IsKeyPressed(KEY_LEFT)){
        piece->dx = -1; piece->Move(board);
        timer_left = 0.0f;
    }

    else if(IsKeyDown(KEY_LEFT)){
        timer_left += deltaTime;
        
        if(timer_left >= DELAY){
            piece->dx = -1; piece->Move(board);
            timer_left -= SPEED;
        }
    }

    else timer_left = 0.0f;

    // Rotate
    if(IsKeyPressed(KEY_UP)) piece->Rotate(board);
}

#endif