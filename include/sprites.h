#ifndef SPRITE_H
#define SPRITE_H

#define NO_OF_SPRITES 7

Texture2D tetrominoTextures[NO_OF_SPRITES];

const char* texturePath[NO_OF_SPRITES] = {
    "Sprites/Red.png",          // I_SHAPE
    "Sprites/Yellow.png",       // O_SHAPE
    "Sprites/Green.png",        // T_SHAPE
    "Sprites/Blue.png",         // J_SHAPE
    "Sprites/Purple.png",       // L_SHAPE
    "Sprites/Cyan.png",         // Z_SHAPE
    "Sprites/Pink.png",         // S_SHAPE
};

void LoadGameSprites(){
    for(int i = 0; i < NO_OF_SPRITES; i++){
        tetrominoTextures[i] = LoadTexture(texturePath[i]);
    }
};

void UnloadGameSprites(){
    for(int i = 0; i < NO_OF_SPRITES; i++){
        UnloadTexture(tetrominoTextures[i]);
    }
}

void DrawSprite(int sprite_id, float x, float y, float width, float height){
    Texture2D& texture = tetrominoTextures[sprite_id];

    DrawTexturePro(
        texture,
        { 0.0f, 0.0f, (float)texture.width, (float)texture.height},
        {x, y, width, height},
        {0.0f, 0.0f},
        0.0f,
        WHITE
    );
}

#endif
