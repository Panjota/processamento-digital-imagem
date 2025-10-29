#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <random>

// Estrutura para representar um triângulo
struct Triangle {
    float x, y;           // Posição
    float size;           // Tamanho
    float rotation;       // Rotação atual
    float rotationSpeed;  // Velocidade de rotação
    float colorOffset;    // Offset para variação de cor
};

// Variáveis globais
std::vector<Triangle> triangles;
auto startTime = std::chrono::high_resolution_clock::now();
std::random_device rd;
std::mt19937 gen(rd());

// Função para desenhar um único triângulo
void drawTriangle(const Triangle& tri) {
    // Calcular cor baseada no tempo e offset único
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float>(currentTime - startTime).count();
    
    float red = 0.5f + 0.5f * sin(time + tri.colorOffset);
    float green = 0.5f + 0.5f * sin(time + tri.colorOffset + 2.0f);
    float blue = 0.5f + 0.5f * sin(time + tri.colorOffset + 4.0f);
    
    // Aplicar transformações
    glPushMatrix();
    glTranslatef(tri.x, tri.y, 0.0f); // Posição
    glRotatef(tri.rotation * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f); // Rotação
    glScalef(tri.size, tri.size, 1.0f); // Tamanho
    
    // Desenhar triângulo com cores nos vértices
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);  // Vermelho
        glVertex2f(0.0f, 0.5f);       // Topo
        
        glColor3f(0.0f, 1.0f, 0.0f);  // Verde  
        glVertex2f(-0.5f, -0.5f);     // Esquerda
        
        glColor3f(0.0f, 0.0f, 1.0f);  // Azul
        glVertex2f(0.5f, -0.5f);      // Direita
    glEnd();
    
    glPopMatrix();
}

// Função para adicionar um novo triângulo
void addTriangle(float x, float y) {
    Triangle newTri;
    newTri.x = x;
    newTri.y = y;
    
    // Propriedades aleatórias
    std::uniform_real_distribution<float> sizeDist(0.3f, 0.8f);
    std::uniform_real_distribution<float> speedDist(0.02f, 0.1f);
    std::uniform_real_distribution<float> colorDist(0.0f, 6.28f);
    
    newTri.size = sizeDist(gen);
    newTri.rotation = 0.0f;
    newTri.rotationSpeed = speedDist(gen);
    newTri.colorOffset = colorDist(gen);
    
    triangles.push_back(newTri);
    
    std::cout << "Triangulo adicionado na posição (" << x << ", " << y << ")" << std::endl;
}

// Função de renderização (chamada pelo GLUT)
void display() {
    // Limpar tela
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Desenhar todos os triângulos
    for (const auto& triangle : triangles) {
        drawTriangle(triangle);
    }
    
    // Atualizar buffer
    glutSwapBuffers();
}

// Função de animação (chamada continuamente)
void update(int value) {
    // Atualizar rotação de todos os triângulos
    for (auto& triangle : triangles) {
        triangle.rotation += triangle.rotationSpeed;
        if (triangle.rotation > 2 * 3.14159f) {
            triangle.rotation -= 2 * 3.14159f;
        }
    }
    
    // Redesenhar
    glutPostRedisplay();
    
    // Reagendar próxima atualização (60 FPS)
    glutTimerFunc(16, update, 0);
}

// Função para cliques do mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Converter coordenadas da tela para coordenadas OpenGL (-1 a 1)
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        
        float glX = (2.0f * x / windowWidth) - 1.0f;
        float glY = 1.0f - (2.0f * y / windowHeight);
        
        addTriangle(glX, glY);
    }
}

// Função para teclas
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27: // ESC
            exit(0);
            break;
        case ' ': // Espaço - adiciona triângulo no centro
            addTriangle(0.0f, 0.0f);
            break;
        case 'c': // C - limpar todos os triângulos
        case 'C':
            triangles.clear();
            std::cout << "Todos os triangulos removidos!" << std::endl;
            break;
        case 'r': // R - adiciona triângulo aleatório
        case 'R': {
            std::uniform_real_distribution<float> posDist(-0.8f, 0.8f);
            float randX = posDist(gen);
            float randY = posDist(gen);
            addTriangle(randX, randY);
            break;
        }
    }
}

int main(int argc, char** argv) {
    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Triangulo Colorido Rotativo");
    
    // Configurar OpenGL
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fundo escuro
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Registrar callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0); // Iniciar animação (60 FPS)
    
    // Adicionar um triângulo inicial no centro
    addTriangle(0.0f, 0.0f);
    
    // Mostrar instruções
    std::cout << "=== CONTROLES ===" << std::endl;
    std::cout << "Clique esquerdo: Adicionar triangulo na posição do mouse" << std::endl;
    std::cout << "Espaço: Adicionar triangulo no centro" << std::endl;
    std::cout << "R: Adicionar triangulo em posição aleatória" << std::endl;
    std::cout << "C: Limpar todos os triangulos" << std::endl;
    std::cout << "ESC: Sair" << std::endl;
    
    // Iniciar loop principal
    glutMainLoop();
    
    return 0;
}
