#ifndef THEME_H
#define THEME_H

struct GameTheme {
    Color bgTop = {45, 25, 75, 255};
    Color bgBottom = {10, 30, 60, 255};
    Color borderColor = Fade(SKYBLUE, 1.0f);

    float curTop[3] = {45, 25, 75};
    float curBot[3] = {10, 30, 60};
    float tarTop[3] = {45, 25, 75};
    float tarBot[3] = {10, 30, 60};

    bool isTransitioning = true;
    bool isDirty = true;

    void SetTarget(Color c) {
        borderColor = c;
        tarTop[0] = c.r * 0.2f + 15.0f;  tarTop[1] = c.g * 0.2f + 15.0f;  tarTop[2] = c.b * 0.2f + 35.0f;
        tarBot[0] = c.r * 0.1f + 5.0f;   tarBot[1] = c.g * 0.1f + 10.0f;  tarBot[2] = c.b * 0.1f + 25.0f;
        isTransitioning = true;
        isDirty = true;
    }

    static unsigned char clampByte(float v) {
        return (unsigned char)(v < 0.0f ? 0.0f : (v > 255.0f ? 255.0f : v));
    }

    void Update(float dt) {
        if (!isTransitioning) return;

        float speed = 5.0f * dt;
        bool stillTransitioning = false;

        for(int i = 0; i < 3; i++) {
            float diffTop = tarTop[i] - curTop[i];
            float diffBot = tarBot[i] - curBot[i];

            curTop[i] += diffTop * speed;
            curBot[i] += diffBot * speed;

            if ((diffTop > 0.5f || diffTop < -0.5f) || (diffBot > 0.5f || diffBot < -0.5f)) {
                stillTransitioning = true;
            }
        }

        isTransitioning = stillTransitioning;
        isDirty = true;

        bgTop = Color{ clampByte(curTop[0]), clampByte(curTop[1]), clampByte(curTop[2]), 255 };
        bgBottom = Color{ clampByte(curBot[0]), clampByte(curBot[1]), clampByte(curBot[2]), 255 };
    }
};

#endif
