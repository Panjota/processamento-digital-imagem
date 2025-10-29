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
bool texturesEnabled = true;
bool lightingEnabled = true;

// Variáveis para cálculo de FPS
int frameCount = 0;
auto startTime = std::chrono::high_resolution_clock::now();
float fps = 0.0f;

// IDs das texturas
GLuint textureIDs[3];

// Estrutura para armazenar dados de cada triângulo
struct Triangle {
    float x, y, z;
    float r, g, b;
    float rotX, rotY, rotZ;
    float scale;
    int textureIndex; // Qual textura usar (0, 1 ou 2)
};

std::vector<Triangle> triangles;
std::mt19937 rng(12345); // Gerador com seed fixa

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

// Função para criar texturas procedurais
void createTextures() {
    glGenTextures(3, textureIDs);
    
    // Textura 1: Xadrez vermelho/branco
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
    const int size1 = 64;
    unsigned char texture1[size1][size1][3];
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size1; j++) {
            if ((i/8 + j/8) % 2 == 0) {
                texture1[i][j][0] = 255; // Vermelho
                texture1[i][j][1] = 0;
                texture1[i][j][2] = 0;
            } else {
                texture1[i][j][0] = 255; // Branco
                texture1[i][j][1] = 255;
                texture1[i][j][2] = 255;
            }
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size1, size1, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Textura 2: Gradiente azul
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
    const int size2 = 64;
    unsigned char texture2[size2][size2][3];
    for (int i = 0; i < size2; i++) {
        for (int j = 0; j < size2; j++) {
            texture2[i][j][0] = 0;
            texture2[i][j][1] = static_cast<unsigned char>(i * 255 / size2); // Verde gradiente
            texture2[i][j][2] = static_cast<unsigned char>(j * 255 / size2); // Azul gradiente
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size2, size2, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Textura 3: Padrão circular
    glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
    const int size3 = 64;
    unsigned char texture3[size3][size3][3];
    for (int i = 0; i < size3; i++) {
        for (int j = 0; j < size3; j++) {
            float dx = (i - size3/2) / (float)(size3/2);
            float dy = (j - size3/2) / (float)(size3/2);
            float dist = sqrt(dx*dx + dy*dy);
            int intensity = (int)(255 * (0.5 + 0.5 * sin(dist * 10)));
            texture3[i][j][0] = intensity; // Vermelho
            texture3[i][j][1] = intensity / 2; // Verde
            texture3[i][j][2] = 255 - intensity; // Azul invertido
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size3, size3, 0, GL_RGB, GL_UNSIGNED_BYTE, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    std::cout << "Texturas procedurais criadas com sucesso!" << std::endl;
}

// Função para configurar iluminação
void setupLighting() {
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        
        // Luz ambiente global
        GLfloat globalAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
        
        // Luz principal
        glEnable(GL_LIGHT0);
        GLfloat light0_pos[] = {5.0f, 10.0f, 5.0f, 1.0f};
        GLfloat light0_diffuse[] = {0.9f, 0.9f, 0.9f, 1.0f};
        GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
        
        glEnable(GL_NORMALIZE);
    } else {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
}

// Função para salvar dados no arquivo CSV
void saveToCSV(int triangles, float fpsValue) {
    std::ofstream file("fps_textured.csv", std::ios::app);
    if (file.is_open()) {
        // Verifica se o arquivo está vazio para adicionar cabeçalho
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "Triangulos,FPS,Texturas,Iluminacao\n";
        }
        
        std::string tex = texturesEnabled ? "ON" : "OFF";
        std::string light = lightingEnabled ? "ON" : "OFF";
        file << triangles << "," << fpsValue << "," << tex << "," << light << "\n";
        file.close();
        
        std::cout << "TEXTURED - Dados salvos: T=" << triangles << ", FPS=" << fpsValue 
                  << ", Tex=" << tex << ", Light=" << light << std::endl;
    }
}

// Função para gerar triângulos aleatórios
void generateTriangles(int count) {
    triangles.clear();
    triangles.reserve(count);
    
    std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> depthDist(-20.0f, -2.0f);
    std::uniform_real_distribution<float> colorDist(0.5f, 1.0f); // Cores mais claras
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
    std::uniform_real_distribution<float> scaleDist(0.5f, 1.5f);
    std::uniform_int_distribution<int> texDist(0, 2); // 3 texturas
    
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
        t.textureIndex = texDist(rng);
        
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

        std::cout << "TEXTURED - Triangulos: " << numTriangles << ", FPS: " << fps 
                  << " (Tex:" << (texturesEnabled ? "ON" : "OFF") 
                  << ", Light:" << (lightingEnabled ? "ON" : "OFF") << ")" << std::endl;
        saveToCSV(numTriangles, fps);

        // Atualiza título da janela
        std::string title = "TEXTURED - Triangulos: " + std::to_string(numTriangles) + 
                           " | FPS: " + std::to_string(static_cast<int>(fps)) +
                           " | Tex:" + (texturesEnabled ? "ON" : "OFF") +
                           " | Light:" + (lightingEnabled ? "ON" : "OFF");
        glutSetWindowTitle(title.c_str());
    }
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    setupLighting();

    // Habilita ou desabilita texturas
    if (texturesEnabled) {
        glEnable(GL_TEXTURE_2D);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    // Renderiza todos os triângulos
    for (const auto& t : triangles) {
        glPushMatrix();
        
        // Aplica transformações
        glTranslatef(t.x, t.y, t.z);
        glRotatef(rotationAngle + t.rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationAngle + t.rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationAngle + t.rotZ, 0.0f, 0.0f, 1.0f);
        glScalef(t.scale, t.scale, t.scale);

        // Bind da textura específica
        if (texturesEnabled) {
            glBindTexture(GL_TEXTURE_2D, textureIDs[t.textureIndex]);
        }

        // Desenha triângulo com coordenadas de textura
        glBegin(GL_TRIANGLES);
            glNormal3f(0.0f, 0.0f, 1.0f);
            
            glColor3f(t.r, t.g, t.b);
            glTexCoord2f(0.5f, 1.0f); // Topo
            glVertex3f(0.0f, 1.0f, 0.0f);
            
            glColor3f(t.g, t.b, t.r);
            glTexCoord2f(0.0f, 0.0f); // Esquerda embaixo
            glVertex3f(-1.0f, -0.5f, 0.0f);
            
            glColor3f(t.b, t.r, t.g);
            glTexCoord2f(1.0f, 0.0f); // Direita embaixo
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
    switch (key) {
        case 't':
        case 'T':
            texturesEnabled = !texturesEnabled;
            std::cout << "Texturas: " << (texturesEnabled ? "LIGADAS" : "DESLIGADAS") << std::endl;
            frameCount = 0;
            startTime = std::chrono::high_resolution_clock::now();
            break;
        case 'l':
        case 'L':
            lightingEnabled = !lightingEnabled;
            std::cout << "Iluminação: " << (lightingEnabled ? "LIGADA" : "DESLIGADA") << std::endl;
            frameCount = 0;
            startTime = std::chrono::high_resolution_clock::now();
            break;
        case 27: // ESC
            exit(0);
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("OpenGL Performance Test - TEXTURED");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    
    printGPUInfo();
    createTextures();
    generateTriangles(numTriangles);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    std::cout << "=== CONTROLES TEXTURED ===" << std::endl;
    std::cout << "SETAS CIMA/BAIXO: +/- 1000 triangulos" << std::endl;
    std::cout << "T: ligar/desligar texturas" << std::endl;
    std::cout << "L: ligar/desligar iluminação" << std::endl;
    std::cout << "ESC: sair" << std::endl;
    std::cout << "Dados salvos em 'fps_textured.csv'" << std::endl;

    glutMainLoop();
    return 0;
}