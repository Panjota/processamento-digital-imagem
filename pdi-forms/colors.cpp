#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

// Função de desenho
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    float vx_inicial = 0.8f;
    float vx_final = 0.01f;
    float altura = 0.4f;
    
    // Esquerda 
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUAD_STRIP);
        glVertex2f(-vx_inicial, -altura);
        glVertex2f(-vx_final, -altura);
        glVertex2f(-vx_final, altura);
        glVertex2f(-vx_inicial, altura);
    glEnd();

    // Direita
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(vx_final, -altura);
        glVertex2f(vx_inicial, -altura);
        glVertex2f(vx_inicial, altura);
        glVertex2f(vx_final, altura);
    glEnd();

    // Losango amarelo 
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.4f, 0.0f);
        glVertex2f(0.0f, 0.4f);
        glVertex2f(0.4f, 0.0f);
        glVertex2f(0.0f, -0.4f);
    glEnd();

    // Círculo azul 
    glColor3f(0.0f, 0.0f, 1.0f);
    float cx = 0.0f, cy = 0.0f, r = 0.28f;
    int num_segments = 100;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy); // centro
        for (int i = 0; i <= num_segments; ++i) {
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
            float x = r * cosf(theta);
            float y = r * sinf(theta);
            glVertex2f(cx + x, cy + y);
        }
    glEnd();

    // Faixa branca horizontal Branca
    float faixaWidth = 0.56f;
    float faixaHeight = 0.05f;
    float faixaCenterY = 0.0f; 

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-faixaWidth / 2, faixaCenterY + faixaHeight / 2);
        glVertex2f(faixaWidth / 2, faixaCenterY + faixaHeight / 2);
        glVertex2f(faixaWidth / 2, faixaCenterY - faixaHeight / 2);
        glVertex2f(-faixaWidth / 2, faixaCenterY - faixaHeight / 2);
    glEnd();

    // Texto "Ordem e Progresso"
    glColor3f(0.0f, 0.0f, 0.0f);
    void *font = GLUT_BITMAP_HELVETICA_18;
    const char* texto = "Ordem e Progresso";

    // Calcula largura do texto
    int textoLen = strlen(texto);
    int textoWidth = 0;
    for (int i = 0; i < textoLen; ++i) {
        textoWidth += glutBitmapWidth(font, texto[i]);
    }

    // Posição inicial à esquerda da faixa branca
    float windowWidth = 1000.0f;
    float faixaLeftX = -faixaWidth / 2.0f; // coordenada normalizada da borda esquerda da faixa
    float margem = 0.13f; // margem interna à esquerda

    glRasterPos2f(faixaLeftX + margem, faixaCenterY - 0.02f);
    for (int i = 0; i < textoLen; ++i) {
        glutBitmapCharacter(font, texto[i]);
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Cores RGB com FreeGLUT/OpenGL");

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}