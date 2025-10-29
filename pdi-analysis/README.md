# Análise de Performance Gráfica OpenGL

Três versões de renderização OpenGL para análise comparativa de performance.

## EXECUÇÃO RÁPIDA

```cmd
# Testes automatizados
python run_tests.py

# Execução manual
triangles_basic.exe                   # Renderização básica
triangles_lighting.exe                # Com iluminação  
triangles_textured.exe                # Com texturas

# Análise dos dados
python analyze_fps.py
```

## Estrutura - Arquivos Base

```
├── triangles_basic.cpp      # Renderização básica
├── triangles_lighting.cpp   # Com iluminação
├── triangles_textured.cpp   # Com texturas
├── analyze_fps.py          # Análise dos dados
├── run_tests.py            # Automação dos testes
├── compile.bat             # Script de compilação
├── requirements.txt        # Dependências Python
└── README.md               # Este arquivo
```

## Arquivos Gerados

- **Executáveis:** `triangles_*.exe`
- **Dados:** `fps_*.csv` 
- **Análise:** `fps_analysis.png`, `performance_report.txt`
- **Ambiente:** `.venv/`

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

## Objetivos

1. Comparar performance entre renderização básica, com iluminação e texturas
2. Medir FPS em função da quantidade de triângulos (1k-50k)
3. Gerar gráficos comparativos e relatórios de análise

## Troubleshooting

**Erro de compilação:** Verificar paths no `compile.bat` e instalar FreeGLUT
**FPS baixo:** Atualizar drivers gráficos, fechar outros programas
**CSV não gerado:** Usar teclas ↑↓ nos programas e aguardar alguns segundos