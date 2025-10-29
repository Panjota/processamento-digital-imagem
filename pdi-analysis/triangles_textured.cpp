#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <string>
#include <cstdio>
#include <cstdlib>

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
GLuint textureID; // Apenas uma textura (UEA)

// Estrutura para armazenar dados de cada triângulo
struct Triangle {
    float x, y, z;
    float r, g, b;
    float rotX, rotY, rotZ;
    float scale;
    float texRotation; // A.2 - Rotação da textura
    float texScaleU, texScaleV; // A.2 - Escala da textura
    float texOffsetU, texOffsetV; // A.2 - Offset da textura
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

GLuint LoadTexture(const char* filename, int width, int height) {
    GLuint texture;
    unsigned char* data;
    FILE* file;
    
    // D.1 - Carregar a imagem
    file = fopen(filename, "rb");
    if (file == NULL) {
        std::cout << "Erro: Não foi possível carregar a imagem " << filename << std::endl;
        return 0;
    }
    
    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);
    
    // D.2 - Criar uma textura em OpenGL
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // D.3 - Definir parâmetros da textura
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    free(data);
    
    std::cout << "Textura carregada com sucesso: " << filename << std::endl;
    return texture;
}

// Função para criar texturas
void createTextures() {
    // Carrega apenas a textura da UEA
    textureID = LoadTexture("uea.raw", 256, 256);
    
    if (textureID == 0) {
        std::cout << "ERRO: Não foi possível carregar uea!" << std::endl;
        std::cout << "Certifique-se de que o arquivo existe no diretório e é um arquivo RAW RGB." << std::endl;
        exit(1);
    }
    
    std::cout << "Textura UEA carregada com sucesso!" << std::endl;
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
    // Criar diretório data se não existir (Windows)
    system("if not exist data mkdir data");
    
    std::ofstream file("data/fps_textured.csv", std::ios::app);
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
    
    // A.2 - Parâmetros para transformações das texturas
    std::uniform_real_distribution<float> texRotDist(0.0f, 360.0f);
    std::uniform_real_distribution<float> texScaleDist(0.5f, 2.0f);
    std::uniform_real_distribution<float> texOffsetDist(0.0f, 1.0f);
    
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
        
        // A.2 - Transformações geométricas das texturas
        t.texRotation = texRotDist(rng);
        t.texScaleU = texScaleDist(rng);
        t.texScaleV = texScaleDist(rng);
        t.texOffsetU = texOffsetDist(rng);
        t.texOffsetV = texOffsetDist(rng);
        
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
        
        // Aplica transformações geométricas
        glTranslatef(t.x, t.y, t.z);
        glRotatef(rotationAngle + t.rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationAngle + t.rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationAngle + t.rotZ, 0.0f, 0.0f, 1.0f);
        glScalef(t.scale, t.scale, t.scale);

        // A.1 - Bind da textura UEA
        if (texturesEnabled) {
            glBindTexture(GL_TEXTURE_2D, textureID);
        }

        // A.2 - Transformações geométricas das texturas
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glTranslatef(t.texOffsetU, t.texOffsetV, 0);
        glRotatef(t.texRotation, 0, 0, 1);
        glScalef(t.texScaleU, t.texScaleV, 1);
        glMatrixMode(GL_MODELVIEW);

        // A.1 - Definir mapeamento das coordenadas das texturas nas coordenadas das primitivas
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

        // Restaura matriz de textura
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);

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
    glEnable(GL_TEXTURE_2D); // Habilita texturas por padrão
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
    std::cout << "L: ligar/desligar iluminacao" << std::endl;
    std::cout << "ESC: sair" << std::endl;
    std::cout << "Dados salvos em 'data/fps_textured.csv'" << std::endl;

    glutMainLoop();
    return 0;
}