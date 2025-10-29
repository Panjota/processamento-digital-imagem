# Análise de Performance Gráfica OpenGL

Três versões de renderização OpenGL para análise comparativa de performance.

## EXECUÇÃO RÁPIDA

```cmd
# 1. Compilar programas
compile.bat

# 2. Executar testes automatizados
python run_tests.py

# 3. Ver resultados em results/
```

### Execução Manual (Alternativa)
```cmd
triangles_basic.exe                   # Renderização básica
triangles_lighting.exe                # Com iluminação  
triangles_textured.exe                # Com texturas

# Análise manual dos dados
python analyze_fps.py
```

## Estrutura do Projeto

```
pdi-analysis/
├── triangles_basic.cpp      # Renderização básica
├── triangles_lighting.cpp   # Com iluminação
├── triangles_textured.cpp   # Com texturas
├── analyze_fps.py          # Análise dos dados
├── run_tests.py            # Automação dos testes
├── compile.bat             # Script de compilação
├── requirements.txt        # Dependências Python
├── README.md               # Este arquivo
├── data/                   # Pasta para dados CSV
│   ├── fps_basic.csv       # Dados renderização básica
│   ├── fps_lighting.csv    # Dados com iluminação
│   └── fps_textured.csv    # Dados com texturas
└── results/                # Pasta para resultados
    ├── fps_analysis.png    # Gráficos de análise
    └── performance_report.txt # Relatório detalhado
```

## Arquivos Gerados Automaticamente

- **Executáveis:** `triangles_*.exe` (raiz do projeto)
- **Dados CSV:** `data/fps_*.csv` (dados coletados durante testes)
- **Resultados:** `results/` (gráficos e relatórios de análise)

## Requisitos

- **MinGW-w64** (compilador C++)
- **FreeGLUT** (biblioteca OpenGL)
- **Python 3.7+** com: matplotlib, pandas, numpy

```cmd
pip install -r requirements.txt
```

## Como Usar

### Compilação
```cmd
compile.bat
```

### Execução e Controles

**triangles_basic.exe:**
- **↑↓**: Adicionar/remover 1000 triângulos
- **ESC**: Sair

**triangles_lighting.exe:**
- **↑↓**: Triângulos
- **O**: Liga/desliga luz omnidirecional
- **S**: Liga/desliga spot light
- **ESC**: Sair

**triangles_textured.exe:**
- **↑↓**: Triângulos  
- **T**: Liga/desliga texturas
- **L**: Liga/desliga iluminação
- **ESC**: Sair

### Análise
```cmd
python analyze_fps.py
```

Gera automaticamente:
- `results/fps_analysis.png` - Gráficos comparativos
- `results/performance_report.txt` - Relatório detalhado

## Fluxo de Trabalho

### 1. Compilação
```cmd
compile.bat
```

### 2. Coleta de Dados (Automática)
```cmd
python run_tests.py
# Escolha opção 1 para suite completa
```

### 3. Coleta Manual (Alternativa)
Execute cada programa individualmente:
- Use ↑↓ para ajustar triângulos
- Aguarde 15s em cada configuração
- Os dados são salvos em `data/`

### 4. Análise dos Resultados
```cmd
python run_tests.py
# Escolha opção 2 para analisar dados existentes
```

## Objetivos

1. Comparar performance entre renderização básica, com iluminação e texturas
2. Medir FPS em função da quantidade de triângulos (1k-50k)
3. Gerar gráficos comparativos e relatórios de análise

## Organização dos Dados

### Entrada (`data/`)
- `fps_basic.csv` - Dados de performance da renderização básica
- `fps_lighting.csv` - Dados com diferentes configurações de iluminação
- `fps_textured.csv` - Dados com texturas e iluminação

### Saída (`results/`)
- `fps_analysis.png` - Gráficos comparativos de performance
- `performance_report.txt` - Relatório com estatísticas detalhadas

### Opções do Sistema Automatizado
```cmd
python run_tests.py
```
1. **Executar suite completa** - Coleta dados e gera análise
2. **Apenas analisar dados existentes** - Usa dados em `data/`
3. **Verificar configuração** - Mostra status do projeto
0. **Sair**

## Troubleshooting

**Erro de compilação:** Verificar paths no `compile.bat` e instalar FreeGLUT
**FPS baixo:** Atualizar drivers gráficos, fechar outros programas
**CSV não gerado:** Verificar se dados estão sendo salvos em `data/`
**Arquivos não encontrados:** Use `python run_tests.py` opção 3 para verificar estrutura
**Erro na análise:** Certificar que existem arquivos CSV em `data/` com dados válidos