#include <GL/freeglut.h>
#include <cmath>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vx_inicial = 0.6f;
    float vx_final = 0.02f;
    float altura = 0.4f;
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glBegin(GL_QUAD_STRIP);
        glVertex2f(-vx_inicial, -altura);
        glVertex2f(-vx_final, -altura);
        glVertex2f(-vx_final, altura);
        glVertex2f(-vx_inicial, altura);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 0.5f, 0.0f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(-0.6f, -0.1f);
        glVertex2f(-0.25f, 0.4f);
        glVertex2f(-0.02f, 0.4f);
        glVertex2f(-0.6f, -0.4f);
    glEnd();

    glDisable(GL_BLEND);

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Kotlin e Transparencia");

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}