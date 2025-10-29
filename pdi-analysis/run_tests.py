import subprocess
import time
import os
import sys
import threading
from datetime import datetime

class AutomatedTest:
    def __init__(self):
        self.test_config = {
            'triangles_basic.exe': {
                'csv_file': 'fps_basic.csv',
                'description': 'Renderização Básica'
            },
            'triangles_lighting.exe': {
                'csv_file': 'fps_lighting.csv', 
                'description': 'Com Iluminação'
            },
            'triangles_textured.exe': {
                'csv_file': 'fps_textured.csv',
                'description': 'Com Texturas'
            }
        }
        
        # Sequência de triângulos para teste
        self.triangle_counts = [1000, 2000, 3000, 5000, 7000, 10000, 15000, 20000, 30000, 50000]
        self.measurement_time = 15  # segundos por medição
        
    def check_executables(self):
        """Verifica se todos os executáveis existem"""
        print("Verificando executáveis...")
        missing = []
        
        for exe in self.test_config.keys():
            if os.path.exists(exe):
                print(f"OK: {exe}")
            else:
                print(f"ERRO: {exe} não encontrado")
                missing.append(exe)
        
        if missing:
            print(f"\nERRO: Execute compile.bat primeiro!")
            print(f"Arquivos faltando: {', '.join(missing)}")
            return False
        
        return True
    
    def start_system_monitor(self):
        """Inicia monitor do sistema em processo separado"""
        try:
            print("Iniciando monitor do sistema...")
            monitor_process = subprocess.Popen([
                sys.executable, 'system_monitor.py', '--monitor'
            ], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            
            time.sleep(3)  # Aguarda inicialização
            
            if monitor_process.poll() is None:
                print("Monitor do sistema ativo")
                return monitor_process
            else:
                print("Falha ao iniciar monitor")
                return None
                
        except Exception as e:
            print(f"Erro ao iniciar monitor: {e}")
            return None
    
    def clear_csv_files(self):
        """Limpa arquivos CSV anteriores"""
        print("Limpando dados anteriores...")
        
        for config in self.test_config.values():
            csv_file = config['csv_file']
            if os.path.exists(csv_file):
                os.remove(csv_file)
                print(f"Removido: {csv_file}")
        
        if os.path.exists('system_monitor.csv'):
            os.remove('system_monitor.csv')
            print("Removido: system_monitor.csv")
    
    def run_single_test(self, executable, triangle_count, duration):
        """Executa um teste individual"""
        print(f"Testando {executable} com {triangle_count} triângulos...")
        
        try:
            # Inicia o programa
            process = subprocess.Popen([executable], 
                                     stdin=subprocess.PIPE,
                                     stdout=subprocess.PIPE, 
                                     stderr=subprocess.PIPE,
                                     text=True)
            
            # Aguarda inicialização
            time.sleep(2)
            
            # Simula teclas para ajustar número de triângulos
            # Começa com 1000, precisa ajustar para triangle_count
            current_triangles = 1000
            
            while current_triangles < triangle_count:
                # Seta para cima (+1000)
                # Nota: Isso é uma simulação, o ideal seria usar pyautogui
                current_triangles += 1000
                time.sleep(0.1)
            
            # Aguarda medição estabilizar
            print(f"Coletando dados por {duration} segundos...")
            time.sleep(duration)
            
            # Fecha programa (ESC)
            process.terminate()
            process.wait(timeout=5)
            
            print(f"Teste concluído: {triangle_count} triângulos")
            
        except Exception as e:
            print(f"Erro durante teste: {e}")
            try:
                process.terminate()
            except:
                pass
    
    def run_manual_tests(self):
        """Executa testes manuais com instruções para o usuário"""
        print("\nExecutando testes manuais...")
        
        for exe, config in self.test_config.items():
            print(f"\nTESTE: {config['description']}")
            print(f"Arquivo: {exe}")
            print(f"Dados salvos em: {config['csv_file']}")
            print("\nINSTRUÇÕES:")
            print("1. O programa será executado automaticamente")
            print("2. Use SETAS CIMA/BAIXO para ajustar triângulos")
            print("3. Aguarde ~15 segundos em cada configuração")
            print("4. Pressione ESC quando terminar")
            
            print(f"\nSequência de teste: {', '.join(map(str, self.triangle_counts))}")
            
            input("\nPressione ENTER para iniciar este teste...")
            
            try:
                # Executa programa
                print(f"Executando {exe}...")
                subprocess.run([exe])
                
                print(f"Teste {config['description']} concluído")
                
                # Verifica se arquivo CSV foi criado
                if os.path.exists(config['csv_file']):
                    print(f"Dados salvos em {config['csv_file']}")
                else:
                    print(f"AVISO: Arquivo {config['csv_file']} não encontrado")
                
            except Exception as e:
                print(f"Erro ao executar {exe}: {e}")
                continue_test = input("Continuar com próximo teste? (s/n): ").lower() == 's'
                if not continue_test:
                    break
    
    def analyze_results(self):
        """Executa análise dos resultados"""
        print("\nAnalisando resultados...")
        
        # Verifica se arquivos CSV existem
        csv_files = []
        for config in self.test_config.values():
            if os.path.exists(config['csv_file']):
                csv_files.append(config['csv_file'])
                print(f"OK: {config['csv_file']}")
            else:
                print(f"ERRO: {config['csv_file']} não encontrado")
        
        if not csv_files:
            print("ERRO: Nenhum arquivo de dados encontrado!")
            return False
        
        print(f"\nExecutando análise com {len(csv_files)} arquivo(s)...")
        
        try:
            subprocess.run([sys.executable, 'analyze_fps.py'])
            print("Análise concluída!")
            
            # Lista arquivos gerados
            output_files = ['fps_analysis.png', 'performance_report.txt']
            print("\nArquivos gerados:")
            for file in output_files:
                if os.path.exists(file):
                    print(f"OK: {file}")
                else:
                    print(f"ERRO: {file} não encontrado")
            
            return True
            
        except Exception as e:
            print(f"Erro durante análise: {e}")
            return False
    
    def run_full_test_suite(self):
        """Executa suite completa de testes"""
        print("SUITE AUTOMATIZADA DE TESTES DE PERFORMANCE GRÁFICA")
        print("=" * 60)
        print(f"Data/Hora: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        
        # Verificações iniciais
        if not self.check_executables():
            return False
        
        # Configuração inicial
        clear_data = input("\nLimpar dados anteriores? (s/n): ").lower() == 's'
        if clear_data:
            self.clear_csv_files()
        
        # Inicia monitor do sistema
        monitor_process = self.start_system_monitor()
        
        print("\nINSTRUÇÕES PARA OS TESTES:")
        print("1. Cada programa será executado sequencialmente")
        print("2. Use as SETAS para ajustar número de triângulos") 
        print("3. Aguarde 15 segundos em cada configuração")
        print("4. Os dados são salvos automaticamente")
        print("5. Pressione ESC para ir ao próximo teste")
        
        print(f"\nTeste estas quantidades: {', '.join(map(str, self.triangle_counts))}")
        
        if monitor_process:
            print("\nMonitor do sistema ativo - não feche a janela!")
        
        input("\nPressione ENTER para começar os testes...")
        
        # Executa testes
        try:
            self.run_manual_tests()
            
            # Para monitor do sistema
            if monitor_process:
                print("\nParando monitor do sistema...")
                monitor_process.terminate()
                monitor_process.wait(timeout=5)
                print("Monitor parado")
            
            # Análise dos resultados
            print("\nAguarde enquanto analisamos os resultados...")
            time.sleep(2)
            
            if self.analyze_results():
                print("\nTESTES CONCLUÍDOS COM SUCESSO!")
                print("\nVerifique os arquivos gerados:")
                print("   - fps_analysis.png (gráficos)")
                print("   - performance_report.txt (relatório)")
                print("   - system_monitor.csv (dados sistema)")
            else:
                print("\nTestes concluídos, mas houve problemas na análise")
                
        except KeyboardInterrupt:
            print("\nTestes interrompidos pelo usuário")
            if monitor_process:
                monitor_process.terminate()
        except Exception as e:
            print(f"\nErro durante testes: {e}")
            if monitor_process:
                monitor_process.terminate()
        
        return True

def main():
    """Função principal"""
    tester = AutomatedTest()
    
    print("FERRAMENTA DE TESTES AUTOMATIZADOS")
    print("=" * 40)
    print("1. Executar suite completa")
    print("2. Apenas analisar dados existentes") 
    print("3. Verificar configuração")
    print("0. Sair")
    
    choice = input("\nEscolha uma opção (0-3): ").strip()
    
    if choice == '1':
        tester.run_full_test_suite()
    elif choice == '2':
        tester.analyze_results()
    elif choice == '3':
        tester.check_executables()
        print("\nVerifique também:")
        print("- Python: python --version")
        print("- Bibliotecas: pip list | findstr \"matplotlib pandas numpy\"")
        print("- Compilador: g++ --version")
    elif choice == '0':
        print("Saindo...")
    else:
        print("Opção inválida!")

if __name__ == "__main__":
    main()