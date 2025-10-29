# Processamento Digital de Imagem

Repositório para atividades da disciplina de Processamento Digital de Imagem, contendo implementações em OpenGL e análises de performance gráfica.

## Estrutura do Projeto

```
processamento-digital-imagem/
├── pdi-analysis/  # Atividade 3 - Análise de performance OpenGL
├── pdi-forms/     # Atividade 1 - Formas geométricas
├── pdi-transform/ # Atividade 2 - Transformações geométricas
└── README.MD      # Este arquivo
```

## Módulos

### pdi-analysis
Análise comparativa de performance em renderização OpenGL com três implementações:
- Renderização básica de triângulos
- Renderização com iluminação
- Renderização com texturas

**Características:**
- Medição de FPS em tempo real
- Coleta automática de dados de performance
- Geração de gráficos e relatórios comparativos
- Testes automatizados

### pdi-forms
Implementação de formas geométricas básicas em OpenGL e  Freeglut:
- Manipulação de cores
- Transparência e efeitos visuais

### pdi-transform
Implementação de transformações geométricas:
- Rotação, translação e escala
- Aplicação de matrizes de transformação

## Requisitos Gerais

- **Compilador:** MinGW-w64 (g++)
- **Bibliotecas:** FreeGLUT, OpenGL
- **Python:** 3.7+ (para análises)
- **Sistema:** Windows

## Uso

Cada módulo possui documentação específica em seu respectivo diretório. Para execução rápida:

```cmd
# Navegue para o módulo desejado
cd pdi-analysis

# Compile os programas
compile.bat

# Execute conforme documentação local
```

## Estrutura de Dados

- **Código fonte:** Arquivos .cpp
- **Executáveis:** Arquivos .exe
- **Dados:** Pasta data/ (quando aplicável)
- **Resultados:** Pasta results/ (quando aplicável)
- **Documentação:** Arquivos .pdf e README.md locais

## Atividades Implementadas

1. **Formas Geométricas** (pdi-forms) - Atividade 1
2. **Transformações** (pdi-transform) - Transformações geométricas
3. **Análise de Performance** (pdi-analysis) - Estudo comparativo OpenGL
