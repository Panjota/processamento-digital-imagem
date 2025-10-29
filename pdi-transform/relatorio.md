# Relatório: Transformações Geométricas em OpenGL

## Alexandro Pantoja dos Santos - 2115080001

## 1. Introdução

Este trabalho apresenta a implementação de um programa em C++ utilizando OpenGL e FreeGLUT para renderizar formas geométricas básicas aplicando transformações espaciais. O objetivo principal é comparar a eficácia entre funções nativas do OpenGL e implementações personalizadas de transformações geométricas.

O programa desenvolvido permite ao usuário visualizar três formas geométricas distintas, aplicar transformações em tempo real e alternar entre diferentes métodos de implementação das transformações, proporcionando uma análise comparativa prática entre as abordagens.

## 2. Desenvolvimento

### 2.1 Estrutura do Programa

O programa foi estruturado utilizando variáveis globais para controle das transformações e funções específicas para cada forma geométrica. A arquitetura permite a alternância entre dois métodos distintos de aplicação de transformações através de uma variável booleana de controle.

```cpp
// Variáveis globais para controle das transformações
float translateX = 0.0f, translateY = 0.0f;
float scale = 1.0f;
float rotationAngle = 0.0f;
int currentShape = 0;
bool useOpenGLTransforms = true;
```

### 2.2 Implementação das Formas Geométricas

As três formas geométricas foram implementadas utilizando primitivas OpenGL apropriadas para cada caso. O quadrado utiliza GL_QUADS com quatro vértices, o triângulo emprega GL_TRIANGLES com três vértices, e o círculo é aproximado através de GL_TRIANGLE_FAN com vinte segmentos.

### 2.3 Transformações Implementadas

#### 2.3.1 Método Interno OpenGL

O método interno utiliza as funções nativas do OpenGL para aplicar transformações através da manipulação da matriz de transformação atual. O código implementa as transformações na seguinte sequência:

```cpp
glPushMatrix();
glTranslatef(translateX, translateY, 0.0f);
glScalef(scale, scale, 1.0f);
glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
// Renderização da geometria
glPopMatrix();
```

Este método oferece vantagens significativas em termos de performance, pois as operações são executadas diretamente na GPU utilizando hardware especializado. A utilização das funções glPushMatrix() e glPopMatrix() garante que as transformações não afetem outras geometrias na cena.

#### 2.3.2 Método Personalizado

A implementação personalizada requer o cálculo manual das transformações aplicadas diretamente aos vértices antes da renderização. Foi desenvolvida uma função específica para rotação:

```cpp
void rotatePoint(float x, float y, float angle, float* newX, float* newY) {
    float rad = angle * 3.14159f / 180.0f;
    *newX = x * cos(rad) - y * sin(rad);
    *newY = x * sin(rad) + y * cos(rad);
}
```

Este método aplica as transformações na CPU, calculando as novas posições dos vértices através de operações matemáticas explícitas. Para cada vértice da geometria, as transformações são aplicadas sequencialmente: escala, rotação e translação.

## 3. Resultados Obtidos

### 3.1 Funcionalidades Implementadas

O programa final apresenta controles interativos completos permitindo ao usuário alternar entre as três formas geométricas, aplicar transformações de translação, escala e rotação, e comparar os dois métodos de implementação em tempo real. A alternância entre métodos é realizada através da tecla 'T', proporcionando feedback imediato sobre as diferenças entre as abordagens.

### 3.2 Análise Comparativa

A análise comparativa revela diferenças substanciais entre os métodos implementados. O método interno OpenGL demonstra performance superior devido à utilização de aceleração por hardware, enquanto o método personalizado oferece maior controle sobre as operações matemáticas envolvidas.

O método interno apresenta simplicidade de implementação e confiabilidade, pois utiliza funções amplamente testadas e otimizadas. Por outro lado, o método personalizado proporciona maior compreensão dos processos matemáticos subjacentes às transformações geométricas, sendo valioso para fins educacionais.

### 3.3 Performance e Eficiência

Durante os testes, o método interno demonstrou fluidez superior na renderização, especialmente em situações com múltiplas transformações aplicadas simultaneamente. O método personalizado, embora funcional, apresenta maior consumo computacional devido aos cálculos realizados na CPU.

## 4. Conclusões

O desenvolvimento deste projeto proporcionou uma compreensão aprofundada das transformações geométricas em computação gráfica e das diferenças práticas entre implementações nativas e personalizadas. Os resultados obtidos demonstram que ambas as abordagens possuem aplicabilidades específicas.

Para aplicações comerciais e jogos que requerem alta performance, o método interno OpenGL apresenta-se como a escolha mais adequada. Para fins educacionais e situações que demandam controle fino sobre as transformações, o método personalizado oferece maior flexibilidade e compreensão dos processos envolvidos.

A implementação bem-sucedida de ambos os métodos confirma a eficácia da abordagem comparativa proposta, proporcionando uma base sólida para desenvolvimentos futuros em computação gráfica e análise de performance em sistemas de renderização.