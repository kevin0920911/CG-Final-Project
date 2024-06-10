#define _CRT_SECURE_NO_WARNINGS
#ifdef __WIN32__
#include <windows.h>
#endif
#define PI 3.14159265358979323846

#include <GL/glut.h>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>


#include "load_bmp.h"
#include "load3ds.h"
#include "objects.h"
#include "drawString.h"
#include "drawModel.h"
#include "mat_vec.h"
#include "Random.h"

obj_type spaceCraft;
// The width and height of your window, change them as you like
int screen_width = 640;
int screen_height = 480;

double move_x = 0, move_y = 0, move_z = 0;

// Flag for rendering as lines or filled polygons
int filling = 1; //0=OFF 1=ON
int gameTimer = 30;
int hp = 3;
bool gameEnd = false;
bool win = false;
bool protect = false;
int score = 0;
std::vector<Point> bullets;


Point meteorites[10] = {
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange (-1000, - 500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500)),
    Point(randomRange(-65, 35),randomRange(-40,40),randomRange(-1000, -500))
};

clock_t start=time(NULL), end;
clock_t Protect_S = time(NULL), Protect_E;
//Lights settings
GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_position[] = { 0.0f, 0.0f, -20.0f, 1.0f };

//Materials settings
GLfloat mat_ambient[] = { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular[] = { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat mat_shininess[] = { 1.0f };

double caluateDistance(double x1, double y1, double z1, double x2, double y2, double z2);
void init(void);
void resize(int p_width, int p_height);
void keyboard(unsigned char p_key, int p_x, int p_y);
void display(void);
void idle();
void menu(int option);
void resetGame();
void exitGame();


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screen_width, screen_height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("1112923-徐聖凱");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutCreateMenu(menu);
    glutAddMenuEntry("Game Resart", 1);
    glutAddMenuEntry("End The Game", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON); 
    init();
    glutMainLoop();

    return(0);
}
double caluateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}
void init(void)
{
    srand(time(NULL));
    glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color to black
    // Viewport transformation
    glViewport(0, 0, screen_width, screen_height);

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 5.0f, 10000.0f); // We define the "viewing volume"




    //Lights initialization and activation
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    //Materials initialization and activation
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    //Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glEnable(GL_TEXTURE_2D); // Texture mapping ON
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST); // Enable the depth test (also called z buffer)

    //Objects loading
    spaceCraft = *ObjLoad((char*)"fighter1.3ds", (char*)"skull.bmp");

}
void resize(int p_width, int p_height)
{

    screen_width = p_width; // We obtain the new screen width values and store it
    screen_height = p_height; // Height value

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0, 0, screen_width, screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 5.0f, 10000.0f);

    glutPostRedisplay(); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}
void keyboard(unsigned char p_key, int p_x, int p_y)
{
    switch (p_key)
    {
    case 'w': case 'W':
        move_y += 1;
        if (move_y >= 40) {
            move_y = 40;
        }
        break;
    case 'a': case 'A':
        move_x -= 1;
        if (move_x <= -65) {
            move_x = -65;
        }
        break;
    case 's': case 'S':
        move_y -= 1;
        if (move_y <= -40) {
            move_y = -40;
        }
        break;
    case 'd': case 'D':
        move_x += 1;
        if (move_x >= 50) {
            move_x = 50;
        }
        break;
    case 'z': case 'Z':
        move_z += 1;
        if (move_z >= 0) {
            move_z = 0;
        }
        break;
    case 'x': case 'X':
        move_z -= 1;
        if (move_z <= -500) {
            move_z = -500;
        }
        break;
    case 'k': case 'K':
        if (!gameEnd) {
            double bullet_x = move_x;
            double bullet_y = move_y;
            double bullet_z = move_z;
            bullets.push_back(Point(bullet_x, bullet_y, bullet_z));
        }
        break;
    }
    glutPostRedisplay();
}
void display(void)
{
    if (gameEnd && !win) {
        score = 0;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
    glMatrixMode(GL_MODELVIEW); // Modeling transformation
    glLoadIdentity(); // Initialize the model matrix as identity


    glTranslatef(0.0, 0.0, -100); // We move the object forward (the model matrix is multiplied by the translation matrix)
    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    

    // For Space Craft
    glPushMatrix();
    {

        glPushMatrix();
        {
            glTranslatef(move_x, move_y, move_z);
            glPushMatrix();
            {
                glScalef(2, 2, 2);
                glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
                if (!gameEnd || (gameEnd && win))
                    drawModel(&spaceCraft);
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();

    if (!gameEnd) {
        for (int i = 0; i < 10; i++) {
            glPushMatrix();
            {
                glTranslatef(meteorites[i].x, meteorites[i].y, meteorites[i].z);
                GLfloat meteor_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
                GLfloat meteor_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
                GLfloat meteor_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
                GLfloat meteor_shininess[] = { 5.0f };

                glMaterialfv(GL_FRONT, GL_AMBIENT, meteor_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, meteor_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, meteor_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, meteor_shininess);
                glutSolidSphere(5, 100, 100);
            }
            glPopMatrix();
        }

        glDisable(GL_LIGHTING);
        for (const auto& bullet : bullets) {
            glPushMatrix();
            {
                glColor3f(1, 0, 0);
                glTranslatef(bullet.x, bullet.y, bullet.z);
                glutSolidSphere(1, 100, 100);
            }
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
    }
    


    glDisable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor3f(1, 0, 0);
        glPushMatrix();
        {
            glTranslatef(35, 40, 0);
            selectFont(48, ANSI_CHARSET, "Comic Sans MS");
            glRasterPos2f(0.0f, 0.0f);
            std::string timeStr = std::to_string(gameTimer);
            timeStr = "Time: " + timeStr;
            glColor3f(1, 0, 0);
            drawString(timeStr.c_str());

        }
        glPopMatrix();
    }
    glPopAttrib();

    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor3f(1, 0, 0);
        glPushMatrix();
        {
            glTranslatef(-65, 40, 0);
            selectFont(48, ANSI_CHARSET, "Comic Sans MS");
            glRasterPos2f(0.0f, 0.0f);
            glColor3f(1, 0, 0);
            std::string heart = "*";
            std::string LifeStr = "Life: ";
            for (int i = 0; i < hp; i++) {
                LifeStr += heart;
            }
            drawString(LifeStr.c_str());
        }
        glPopMatrix();
    }
    glPopAttrib();
    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor3f(1, 1, 0);
        glPushMatrix();
        {
            glTranslatef(-20, -45, 0);
            selectFont(12, ANSI_CHARSET, "Comic Sans MS");
            glRasterPos2f(0.0f, 0.0f);
            glColor3f(1, 0, 0);
            std::string scoreStr = std::to_string(score);
            scoreStr = "Score: " + scoreStr;
            drawString(scoreStr.c_str());
        }
        glPopMatrix();
    }
    glPopAttrib();
    
    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor3f(1, 1, 1);
        if (gameEnd) {
            if (win) {
                glPushMatrix();
                {
                    glTranslatef(-40, 0, 0);
                    selectFont(48, ANSI_CHARSET, "Comic Sans MS");
                    glRasterPos2f(0.0f, 0.0f);
                    glColor3f(1, 1, 1);
                    drawString("GAME{Y0u_4r3_w1nnnn3r}");
                }
                glPopMatrix();
            }
            else {
                glPushMatrix();
                {
                    glTranslatef(-30, 0, 0);
                    selectFont(48, ANSI_CHARSET, "Comic Sans MS");
                    glRasterPos2f(0.0f, 0.0f);
                    glColor3f(1, 1, 1);
                    drawString("GAME{M1ss0n_F4i1}");
                }
                glPopMatrix();
            }
        }
    }
    glPopAttrib();
    glEnable(GL_LIGHTING);

    glFlush(); // This force the execution of OpenGL commands
    glutSwapBuffers(); // In double buffered mode we invert the positions of the visible buffer and the writing buffer
}
void idle() {
    if (protect) {
        Protect_E = time(NULL);
        if (Protect_E - Protect_S > 0.5) {
            protect = false;
        }
    }
    if (!gameEnd) {
        for (int i = 0; i < 10; i++) {
            meteorites[i].z += 0.5;
            if (meteorites[i].z >= 0) {
                meteorites[i].z = randomRange(-1000, -500);
                meteorites[i].x = randomRange(-70, 45);
                meteorites[i].y = randomRange(-60, 60);
            }
        }
    }
    if (gameEnd && win) {
        move_z -= 0.5;
    }
    float ellipsoidRadiusX = 10.0f; 
    float ellipsoidRadiusY = 3.0f;  
    float ellipsoidRadiusZ = 8.0f;  
    float meteoriteRadius = 5.0f;
    for (int i = 0; i < 10; i++) {
        // 縮放隕石的位置到橢圓體單位球體空間
        float scaledMeteoriteX = meteorites[i].x / ellipsoidRadiusX;
        float scaledMeteoriteY = meteorites[i].y / ellipsoidRadiusY;
        float scaledMeteoriteZ = meteorites[i].z / ellipsoidRadiusZ;

        // 縮放飛機位置到單位球體空間
        float scaledSpacecraftX = move_x / ellipsoidRadiusX;
        float scaledSpacecraftY = move_y / ellipsoidRadiusY;
        float scaledSpacecraftZ = move_z / ellipsoidRadiusZ;

        // 計算縮放後的距離
        float distance = caluateDistance(
            scaledSpacecraftX, scaledSpacecraftY, scaledSpacecraftZ,
            scaledMeteoriteX, scaledMeteoriteY, scaledMeteoriteZ
        );

        // 橢圓體半徑縮放後的等效球體半徑為 1
        float scaledSpacecraftRadius = 1.0f;
        float scaledMeteoriteRadius = meteoriteRadius / std::min({ ellipsoidRadiusX, ellipsoidRadiusY, ellipsoidRadiusZ });

        if (distance < (scaledSpacecraftRadius + scaledMeteoriteRadius) && !gameEnd ) {
            // 碰撞發生
            if (!protect) {
                hp -= 1;
                protect = true;
                Protect_S = time(NULL);
            }
            if (hp <= 0) {
                std::cout << "Game Over!" << std::endl;
                meteorites[i].z = -1000;
                gameTimer = 0;
                hp = 0;
            }
            else {
                // 重置隕石位置
                meteorites[i].z = randomRange(-1000, -500);
                meteorites[i].x = randomRange(-70, 45);
                meteorites[i].y = randomRange(-60, 60);
                break;
            }
        }
    }
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        
        auto& bullet = *it;
        bullet.z -= 0.5;
        
        bool removeBullet = false;
        for (int i = 0; i < 10; i++) {
            float distance = caluateDistance(bullet.x, bullet.y, bullet.z, meteorites[i].x, meteorites[i].y, meteorites[i].z);
            float bulletRadius = 1.0f;
            float meteoriteRadius = 5.0f;
            if (distance < (bulletRadius + meteoriteRadius) && !gameEnd) {
                removeBullet = true;
                meteorites[i].z = randomRange(-1000, -500);
                meteorites[i].x = randomRange(-70, 45);
                meteorites[i].y = randomRange(-60, 60);
                score++;
                break;
            }
        }
        if (bullet.z <= -1000) {
            removeBullet = true;
        }
        if (removeBullet) {
            it = bullets.erase(it);
        }
        else {
            ++it; 
        }
    }

    end = time(NULL);
    if (end - start >= 1) {
        start = time(NULL);
        gameTimer -= 1;
        if (gameTimer <= 0) {
            gameTimer = 0;
        }
    }

    gameEnd = (gameTimer == 0 || hp == 0);
    win = (hp != 0);
    glutPostRedisplay();
}
void menu(int option) {
    switch (option) {
    case 1: // Reset Game
        resetGame();
        break;
    case 2: // Exit Game
        exitGame();
        break;
    }
    glutPostRedisplay();
}
void resetGame() {
    // Reset Game Varible
    move_x = 0;
    move_y = 0;
    move_z = 0;
    score = 0;
    gameTimer = 30;
    hp = 3;
    gameEnd = false;
    win = false;
    // Reset meteorite
    for (int i = 0; i < 10; i++) {
        meteorites[i].x = randomRange(-65, 35);
        meteorites[i].y = randomRange(-40, 40);
        meteorites[i].z = randomRange(-1000, -500);
    }
}
void exitGame() {
    exit(0);
}

/*
    お嬢様、it's time to go to bed
    또 핸드폰 하는 거예요?
    늦게 자면 고운 피부가 다 무너진다구요
    最後の警告です
    지금 잠에 들지 않으면,
    우린 춤을 출 거예요

    やれやれ, 못 말리는 아가씨
    (๑ˇεˇ๑)lala•¨•.¸¸laa♪
    ♪♪♪ ヽ(･ˇ∀ˇ･ゞ)
    ♬♩♪♩( ◜◒◝ )♩♪♩♬
    ♪♪(o*゜∇゜)o～♪♪
    ((ヽ(๑╹◡╹๑)ﾉ))♬
    ♫ヽ(゜∇゜ヽ)♪♬(ノ゜∇゜)ノ♩♪
    (ﾉ・ω・)ﾉ♫♩ヽ(・ω・ヽ)♬ヽ(*・ω・)ﾉ

    CG Bye (◔⊖◔)つ
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
    CG IS HARD!!!!!
*/