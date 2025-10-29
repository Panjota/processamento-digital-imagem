#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <string>

// Variáveis globais
int numTriangles = 1000;
float rotationAngle = 0.0f;

// Variáveis para cálculo de FPS
int frameCount = 0;
auto startTime = std::chrono::high_resolution_clock::now();
float fps = 0.0f;

// Estrutura para armazenar dados de cada triângulo
struct Triangle {
    float x, y, z;
    float r, g, b;
    float rotX, rotY, rotZ; // Rotações individuais
    float scale;
};

std::vector<Triangle> triangles;
std::mt19937 rng(12345); // Gerador de números aleatórios com seed fixa

// Função para verificar informações da GPU
void printGPUInfo() {
    std::cout << "\n=== INFORMAÇÕES DA GPU/OpenGL ===" << std::endl;
    
    const char* vendor = (const char*)glGetString(GL_VENDOR);
    const char* renderer = (const char*)glGetString(GL_RENDERER);
    const char* version = (const char*)glGetString(GL_VERSION);
    
    std::cout << "Vendor: " << (vendor ? vendor : "Desconhecido") << std::endl;
    std::cout << "Renderer (GPU): " << (renderer ? renderer : "Desconhecido") << std::endl;
    std::cout << "Versão OpenGL: " << (version ? version : "Desconhecido") << std::endl;
    std::cout << "================================\n" << std::endl;
}

// Função para salvar dados no arquivo CSV
void saveToCSV(int triangles, float fpsValue) {
    // Criar diretório data se não existir (Windows)
    system("if not exist data mkdir data");
    
    std::ofstream file("data/fps_basic.csv", std::ios::app);
    if (file.is_open()) {
        // Verifica se o arquivo está vazio para adicionar cabeçalho
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "Triangulos,FPS\n";
        }
        
        file << triangles << "," << fpsValue << "\n";
        file.close();
        
        std::cout << "Basic - Dados salvos: T=" << triangles << ", FPS=" << fpsValue << std::endl;
    }
}

// Função para gerar triângulos aleatórios
void generateTriangles(int count) {
    triangles.clear();
    triangles.reserve(count);
    
    std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> depthDist(-20.0f, -2.0f);
    std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
    std::uniform_real_distribution<float> scaleDist(0.5f, 1.5f);
    
    for (int i = 0; i < count; ++i) {
        Triangle t;
        t.x = posDist(rng);
        t.y = posDist(rng);
        t.z = depthDist(rng);
        
        t.r = colorDist(rng);
        t.g = colorDist(rng);
        t.b = colorDist(rng);
        
        t.rotX = rotDist(rng);
        t.rotY = rotDist(rng);
        t.rotZ = rotDist(rng);
        
        t.scale = scaleDist(rng);
        
        triangles.push_back(t);
    }
}

// Função para calcular FPS
void calculateFPS() {
    frameCount++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    float timeInterval = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

    if (timeInterval >= 1.0f) {
        fps = frameCount / timeInterval;
        frameCount = 0;
        startTime = currentTime;

        std::cout << "BASIC - Triangulos: " << numTriangles << ", FPS: " << fps << std::endl;
        saveToCSV(numTriangles, fps);

        // Atualiza título da janela
        std::string title = "BASIC - Triangulos: " + std::to_string(numTriangles) + " | FPS: " + std::to_string(static_cast<int>(fps));
        glutSetWindowTitle(title.c_str());
    }
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Renderiza todos os triângulos
    for (const auto& t : triangles) {
        glPushMatrix();
        
        // Aplica transformações
        glTranslatef(t.x, t.y, t.z);
        glRotatef(rotationAngle + t.rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationAngle + t.rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationAngle + t.rotZ, 0.0f, 0.0f, 1.0f);
        glScalef(t.scale, t.scale, t.scale);

        // Desenha triângulo
        glBegin(GL_TRIANGLES);
            glColor3f(t.r, t.g, t.b);
            glVertex3f(0.0f, 1.0f, 0.0f);
            
            glColor3f(t.g, t.b, t.r);
            glVertex3f(-1.0f, -0.5f, 0.0f);
            
            glColor3f(t.b, t.r, t.g);
            glVertex3f(1.0f, -0.5f, 0.0f);
        glEnd();

        glPopMatrix();
    }

    calculateFPS();
    glutSwapBuffers();
}

// Função de redimensionamento
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Função de animação
void update(int value) {
    rotationAngle += 1.0f;
    if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// Função para teclas especiais
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            numTriangles += 1000;
            if (numTriangles > 50000) numTriangles = 50000;
            generateTriangles(numTriangles);
            frameCount = 0;
            startTime = std::chrono::high_resolution_clock::now();
            break;
        case GLUT_KEY_DOWN:
            numTriangles -= 1000;
            if (numTriangles < 1000) numTriangles = 1000;
            generateTriangles(numTriangles);
            frameCount = 0;
            startTime = std::chrono::high_resolution_clock::now();
            break;
    }
}

// Função para teclas normais
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("OpenGL Performance Test - BASIC TRIANGLES");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    printGPUInfo();
    generateTriangles(numTriangles);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    std::cout << "=== CONTROLES BASIC ===" << std::endl;
    std::cout << "SETAS CIMA/BAIXO: +/- 1000 triangulos" << std::endl;
    std::cout << "ESC: sair" << std::endl;
    std::cout << "Dados salvos em 'data/fps_basic.csv'" << std::endl;

    glutMainLoop();
    return 0;
}