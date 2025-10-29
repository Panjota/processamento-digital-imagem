import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
from pathlib import Path

class FPSAnalyzer:
    def __init__(self):
        self.data_basic = None
        self.data_lighting = None
        self.data_textured = None
        
    def load_data(self):
        files = {
            'basic': 'fps_basic.csv',
            'lighting': 'fps_lighting.csv',
            'textured': 'fps_textured.csv'
        }
        
        for name, filename in files.items():
            if os.path.exists(filename):
                try:
                    df = pd.read_csv(filename)
                    if name == 'basic':
                        self.data_basic = df
                    elif name == 'lighting':
                        self.data_lighting = df
                    elif name == 'textured':
                        self.data_textured = df
                except Exception:
                    pass
    
    def analyze_basic_performance(self):
        if self.data_basic is None:
            return
            
        print(f"Performance Básica - FPS médio: {self.data_basic['FPS'].mean():.2f}")
        print(f"Triângulos: {self.data_basic['Triangulos'].min()} a {self.data_basic['Triangulos'].max()}")
        print(f"FPS min/max: {self.data_basic['FPS'].min()}/{self.data_basic['FPS'].max()}")
    
    def analyze_lighting_performance(self):
        if self.data_lighting is None:
            return
            
        print(f"Performance Iluminação - FPS médio: {self.data_lighting['FPS'].mean():.2f}")
        for omni in ['ON', 'OFF']:
            for spot in ['ON', 'OFF']:
                subset = self.data_lighting[
                    (self.data_lighting['Omnidirecional'] == omni) & 
                    (self.data_lighting['SpotLight'] == spot)
                ]
                if not subset.empty:
                    print(f"  Omni:{omni}, Spot:{spot} - FPS: {subset['FPS'].mean():.2f}")
    
    def analyze_textured_performance(self):
        if self.data_textured is None:
            return
            
        print(f"Performance Texturas - FPS médio: {self.data_textured['FPS'].mean():.2f}")
        for tex in ['ON', 'OFF']:
            for light in ['ON', 'OFF']:
                subset = self.data_textured[
                    (self.data_textured['Texturas'] == tex) & 
                    (self.data_textured['Iluminacao'] == light)
                ]
                if not subset.empty:
                    print(f"  Tex:{tex}, Light:{light} - FPS: {subset['FPS'].mean():.2f}")
    
    def create_comparison_graphs(self):
        plt.style.use('default')
        fig, axes = plt.subplots(1, 4, figsize=(20, 5))
        
        colors = {
            'basic': '#3498db',
            'lighting': '#e74c3c',
            'textured': '#2ecc71'
        }
        
        if self.data_basic is not None:
            basic_grouped = self.data_basic.groupby('Triangulos')['FPS'].mean()
            axes[0].plot(basic_grouped.index, basic_grouped.values, 'o-', 
                        color=colors['basic'], linewidth=2, markersize=6)
            axes[0].set_title('Performance Básica')
            axes[0].set_xlabel('Número de Triângulos')
            axes[0].set_ylabel('FPS')
            axes[0].grid(True, alpha=0.3)
            
            avg_fps = basic_grouped.mean()
            axes[0].text(0.02, 0.98, f'FPS médio: {avg_fps:.1f}', 
                        transform=axes[0].transAxes, verticalalignment='top',
                        bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
        else:
            axes[0].text(0.5, 0.5, 'Dados não disponíveis', 
                        transform=axes[0].transAxes, ha='center', va='center')
            axes[0].set_title('Performance Básica')
        
        if self.data_lighting is not None:
            lighting_grouped = self.data_lighting.groupby('Triangulos')['FPS'].mean()
            axes[1].plot(lighting_grouped.index, lighting_grouped.values, 's-', 
                        color=colors['lighting'], linewidth=2, markersize=6)
            axes[1].set_title('Performance com Iluminação')
            axes[1].set_xlabel('Número de Triângulos')
            axes[1].set_ylabel('FPS')
            axes[1].grid(True, alpha=0.3)
            
            avg_fps = lighting_grouped.mean()
            axes[1].text(0.02, 0.98, f'FPS médio: {avg_fps:.1f}', 
                        transform=axes[1].transAxes, verticalalignment='top',
                        bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
        else:
            axes[1].text(0.5, 0.5, 'Dados não disponíveis', 
                        transform=axes[1].transAxes, ha='center', va='center')
            axes[1].set_title('Performance com Iluminação')
        
        if self.data_textured is not None:
            textured_grouped = self.data_textured.groupby('Triangulos')['FPS'].mean()
            axes[2].plot(textured_grouped.index, textured_grouped.values, '^-', 
                        color=colors['textured'], linewidth=2, markersize=6)
            axes[2].set_title('Performance com Texturas')
            axes[2].set_xlabel('Número de Triângulos')
            axes[2].set_ylabel('FPS')
            axes[2].grid(True, alpha=0.3)
            
            avg_fps = textured_grouped.mean()
            axes[2].text(0.02, 0.98, f'FPS médio: {avg_fps:.1f}', 
                        transform=axes[2].transAxes, verticalalignment='top',
                        bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
        else:
            axes[2].text(0.5, 0.5, 'Dados não disponíveis', 
                        transform=axes[2].transAxes, ha='center', va='center')
            axes[2].set_title('Performance com Texturas')
        
        axes[3].set_title('Comparação Geral')
        axes[3].set_xlabel('Número de Triângulos')
        axes[3].set_ylabel('FPS')
        axes[3].grid(True, alpha=0.3)
        
        if self.data_basic is not None:
            basic_grouped = self.data_basic.groupby('Triangulos')['FPS'].mean()
            axes[3].plot(basic_grouped.index, basic_grouped.values, 'o-', 
                        color=colors['basic'], linewidth=2, markersize=4, label='Básico')
        
        if self.data_lighting is not None:
            lighting_grouped = self.data_lighting.groupby('Triangulos')['FPS'].mean()
            axes[3].plot(lighting_grouped.index, lighting_grouped.values, 's-', 
                        color=colors['lighting'], linewidth=2, markersize=4, label='Com Iluminação')
        
        if self.data_textured is not None:
            textured_grouped = self.data_textured.groupby('Triangulos')['FPS'].mean()
            axes[3].plot(textured_grouped.index, textured_grouped.values, '^-', 
                        color=colors['textured'], linewidth=2, markersize=4, label='Com Texturas')
        
        if any([self.data_basic is not None, self.data_lighting is not None, self.data_textured is not None]):
            axes[3].legend(loc='upper right')
        
        plt.tight_layout()
        plt.savefig('fps_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def generate_report(self):
        with open('performance_report.txt', 'w', encoding='utf-8') as f:
            f.write("RELATÓRIO DE ANÁLISE DE PERFORMANCE GRÁFICA\n")
            f.write("=" * 50 + "\n\n")
            
            if self.data_basic is not None:
                f.write("PERFORMANCE BÁSICA:\n")
                f.write(f"- Registros: {len(self.data_basic)}\n")
                f.write(f"- FPS médio: {self.data_basic['FPS'].mean():.2f}\n")
                f.write(f"- FPS min/max: {self.data_basic['FPS'].min()}/{self.data_basic['FPS'].max()}\n\n")
            
            if self.data_lighting is not None:
                f.write("PERFORMANCE COM ILUMINAÇÃO:\n")
                f.write(f"- Registros: {len(self.data_lighting)}\n")
                f.write(f"- FPS médio: {self.data_lighting['FPS'].mean():.2f}\n\n")
            
            if self.data_textured is not None:
                f.write("PERFORMANCE COM TEXTURAS:\n")
                f.write(f"- Registros: {len(self.data_textured)}\n")
                f.write(f"- FPS médio: {self.data_textured['FPS'].mean():.2f}\n\n")
            
            if all([self.data_basic is not None, self.data_lighting is not None, self.data_textured is not None]):
                basic_avg = self.data_basic['FPS'].mean()
                lighting_avg = self.data_lighting['FPS'].mean()
                textured_avg = self.data_textured['FPS'].mean()
                
                f.write("COMPARAÇÕES:\n")
                f.write(f"- Básico: {basic_avg:.2f} FPS\n")
                f.write(f"- Iluminação: {lighting_avg:.2f} FPS ({((lighting_avg/basic_avg-1)*100):+.1f}%)\n")
                f.write(f"- Texturas: {textured_avg:.2f} FPS ({((textured_avg/basic_avg-1)*100):+.1f}%)\n")
    
    def run_analysis(self):
        self.load_data()
        self.analyze_basic_performance()
        self.analyze_lighting_performance()
        self.analyze_textured_performance()
        self.create_comparison_graphs()
        self.generate_report()

if __name__ == "__main__":
    analyzer = FPSAnalyzer()
    analyzer.run_analysis()