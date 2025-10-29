#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

// Variáveis globais para controle das transformações
float translateX = 0.0f, translateY = 0.0f;
float scale = 1.0f;
float rotationAngle = 0.0f;
int currentShape = 0; // 0=quadrado, 1=triangulo, 2=circulo
bool useOpenGLTransforms = true;

// Função de rotação
void rotatePoint(float x, float y, float angle, float* newX, float* newY) {
    float rad = angle * 3.14159f / 180.0f;
    *newX = x * cos(rad) - y * sin(rad);
    *newY = x * sin(rad) + y * cos(rad);
}

void drawSquare() {
    if (useOpenGLTransforms) {
        // Usando funções internas do OpenGL
        glPushMatrix();
        glTranslatef(translateX, translateY, 0.0f);
        glScalef(scale, scale, 1.0f);
        glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
        
        glBegin(GL_QUADS);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();
        
        glPopMatrix();
    } else {
        // Usando função personalizada apenas para rotação
        float vertices[4][2] = {
            {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f}
        };
        
        glBegin(GL_QUADS);
        for (int i = 0; i < 4; i++) {
            float rotX, rotY;
            rotatePoint(vertices[i][0] * scale, vertices[i][1] * scale, rotationAngle, &rotX, &rotY);
            glVertex2f(rotX + translateX, rotY + translateY);
        }
        glEnd();
    }
}

void drawTriangle() {
    if (useOpenGLTransforms) {
        glPushMatrix();
        glTranslatef(translateX, translateY, 0.0f);
        glScalef(scale, scale, 1.0f);
        glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
        
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
        
        glPopMatrix();
    } else {
        float vertices[3][2] = {
            {0.0f, 0.5f}, {-0.5f, -0.5f}, {0.5f, -0.5f}
        };
        
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; i++) {
            float rotX, rotY;
            rotatePoint(vertices[i][0] * scale, vertices[i][1] * scale, rotationAngle, &rotX, &rotY);
            glVertex2f(rotX + translateX, rotY + translateY);
        }
        glEnd();
    }
}

void drawCircle() {
    int segments = 20;
    
    if (useOpenGLTransforms) {
        glPushMatrix();
        glTranslatef(translateX, translateY, 0.0f);
        glScalef(scale, scale, 1.0f);
        glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
        
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.14159f * i / segments;
            glVertex2f(0.5f * cos(angle), 0.5f * sin(angle));
        }
        glEnd();
        
        glPopMatrix();
    } else {
        glBegin(GL_TRIANGLE_FAN);
        float centerX, centerY;
        rotatePoint(0.0f * scale, 0.0f * scale, rotationAngle, &centerX, &centerY);
        glVertex2f(centerX + translateX, centerY + translateY);
        
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.14159f * i / segments;
            float x = 0.5f * cos(angle) * scale;
            float y = 0.5f * sin(angle) * scale;
            float rotX, rotY;
            rotatePoint(x, y, rotationAngle, &rotX, &rotY);
            glVertex2f(rotX + translateX, rotY + translateY);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (currentShape == 0) {
        glColor3f(1.0f, 0.0f, 0.0f); // Vermelho para quadrado
        drawSquare();
    } else if (currentShape == 1) {
        glColor3f(0.0f, 1.0f, 0.0f); // Verde para triângulo
        drawTriangle();
    } else {
        glColor3f(0.0f, 0.0f, 1.0f); // Azul para círculo
        drawCircle();
    }
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1': currentShape = 0; break;
        case '2': currentShape = 1; break;
        case '3': currentShape = 2; break;
        case 't': case 'T': 
            useOpenGLTransforms = !useOpenGLTransforms;
            std::cout << "Modo: " << (useOpenGLTransforms ? "OpenGL" : "Personalizada") << std::endl;
            break;
        case 'w': case 'W': translateY += 0.1f; break;
        case 's': case 'S': translateY -= 0.1f; break;
        case 'a': case 'A': translateX -= 0.1f; break;
        case 'd': case 'D': translateX += 0.1f; break;
        case 'q': case 'Q': scale += 0.1f; break;
        case 'e': case 'E': scale -= 0.1f; if (scale < 0.1f) scale = 0.1f; break;
        case 'r': case 'R': rotationAngle += 15.0f; break;
        case 'f': case 'F': rotationAngle -= 15.0f; break;
        case 'z': case 'Z': 
            translateX = translateY = 0; 
            scale = 1; 
            rotationAngle = 0;
            break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void printInstructions() {
    std::cout << "Controles:" << std::endl;
    std::cout << "1/2/3: Alternar formas" << std::endl;
    std::cout << "T: Alternar metodo (OpenGL/Personalizada)" << std::endl;
    std::cout << "WASD: Translacao" << std::endl;
    std::cout << "Q/E: Escala" << std::endl;
    std::cout << "R/F: Rotacao" << std::endl;
    std::cout << "Z: Reset" << std::endl;
    std::cout << "ESC: Sair" << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Transformacoes Geometricas - OpenGL vs Personalizada");
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    printInstructions();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    return 0;
}
