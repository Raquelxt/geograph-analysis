import subprocess

class Grafo:
    _contador: int
    _caminhosEsperados: int
    
    capital: str
    batalhoes: list[str]
    
    matrizIncidencia: list[list[int]]
    mapID: dict
    
    def __init__(self, n: int):
        self._contador = 0
        self._caminhosEsperados = 0
        
        self.capital = ""
        self.batalhoes = []
        self.mapID = dict()
        self.matrizIncidencia = [[0] * n] * n
    
    def existeAresta(self, inicio: str, destino: str) -> bool:
        inicioID: int = self.mapID[inicio]
        destinoID: int = self.mapID[destino]
        
        return self.matrizIncidencia[inicioID][destinoID] > 0
    
    def setCapital(self, capital: str):
        self.capital = capital
    
    def inserirBatalhao(self, batalhao: str):
        self.batalhoes.append(batalhao)
    
    def setCaminhosEsperados(self, numCaminhos: int):
        self._caminhosEsperados = numCaminhos
    
    def inserirAresta(self, inicio: str, destino: str):
        if inicio not in self.mapID.keys():
            self.mapID[inicio] = self._contador
            self._contador += 1
        
        if destino not in self.mapID.keys():
            self.mapID[destino] = self._contador
            self._contador += 1
        
        inicioID: int = self.mapID[inicio]
        destinoID: int = self.mapID[destino]
        
        self.matrizIncidencia[inicioID][destinoID] += 1
    
    def validarCaminho(self, caminho: list[str]) -> bool:
        if caminho[-1] == "":
            caminho.pop(-1)

        if caminho[0] != self.capital and caminho[0] not in self.batalhoes:
            return False

        if caminho[0] != caminho[-1]:
            return False

        for iterador in range(0, len(caminho) - 1):
            verticeInicio = caminho[iterador]
            verticeDestino = caminho[iterador + 1]

            if not self.existeAresta(verticeInicio, verticeDestino):
                print(f"Não existe a aresta: {verticeInicio} -> {verticeDestino}.")
                return False
        return True

    def validarSaida(self, linhas: list[str]):
        # Separador para melhorar a leitura
        def imprimir_separador():
            print("=" * 50)

        # Validando a capital
        imprimir_separador()
        capitalSaida = linhas[0]
        print("== [ Validação da Capital ] ==")
        if capitalSaida == self.capital:
            print(f"[✓] Capital VALIDA: {capitalSaida}")
        else:
            print(f"[✗] Capital INCORRETA! Esperado: {self.capital} | Recebido: {capitalSaida}")

        # Validando quantidade de batalhões
        imprimir_separador()
        assert linhas[1].isnumeric()
        print("== [ Validação de Quantidade de Batalhões ] ==")
        if int(linhas[1]) == len(self.batalhoes):
            print(f"[✓] Quantidade de batalhões VALIDA: {len(self.batalhoes)}")
        else:
            print(f"[✗] Quantidade de batalhões INCORRETA! Esperado: {len(self.batalhoes)} | Recebido: {linhas[1]}")

        # Validando os batalhões
        imprimir_separador()
        print("== [ Validação dos Batalhões ] ==")
        batalhoesValidos = True
        fimBatalhoes = int(linhas[1]) + 2
        for it in range(2, fimBatalhoes):
            if linhas[it] not in self.batalhoes:
                batalhoesValidos = False
                print(f"[✗] Batalhão INCORRETO: {linhas[it]}")

        if batalhoesValidos:
            print("[✓] Todos os batalhões estão VALIDOS!")

        # Validando quantidade de caminhos
        imprimir_separador()
        assert linhas[fimBatalhoes].isnumeric()
        caminhosPossiveis = int(linhas[fimBatalhoes])
        print("== [ Validação de Quantidade de Caminhos ] ==")
        if caminhosPossiveis == self._caminhosEsperados:
            print(f"[✓] Quantidade de caminhos VALIDA: {caminhosPossiveis}")
        else:
            print(
                f"[✗] Quantidade de caminhos INCORRETA! Esperado: {self._caminhosEsperados} | Recebido: {caminhosPossiveis}")

        # Validando os caminhos
        imprimir_separador()
        print("== [ Validação dos Caminhos ] ==")
        for it in range(fimBatalhoes + 1, len(linhas)):
            caminhoIndicado = linhas[it].split(" ")
            validacao = self.validarCaminho(caminhoIndicado)
            status = "✓ Válido" if validacao else "✗ Inválido"
            print(f"Caminho {caminhoIndicado[0]}: {status}")

        imprimir_separador()


saidaEsperada = ""
saidaEfetuada = ""

for i in range(1, 9):
    print("\n\nTESTE", i)
    
    grafoCidade = None
    
    # Inicie o processo
    processo = subprocess.Popen(
        ["./tp1"],  # Comando para rodar o script
        stdin=subprocess.PIPE,            # Redireciona a entrada padrão
        stdout=subprocess.PIPE,           # Redireciona a saída padrão
        stderr=subprocess.PIPE,           # Redireciona o erro padrão (opcional)
        text=True                         # Para trabalhar com strings em vez de bytes
    )

    with open(f"./tests/inputs/testCase0{i}.txt", "r") as testFile:
        linhaIt: int = 0
        arestas: int = -1

        entrada = ""
        for linha in testFile:
            linha = linha.strip()
            
            if linhaIt == 0:
                entrada += linha + "\n"
                linha = linha.split(" ")
                
                assert len(linha) == 2
                assert linha[0].isnumeric()
                assert linha[1].isnumeric()
                
                numVertices = int(linha[0])
                numArestas = int(linha[1])
                
                grafoCidade = Grafo(numVertices)
                arestas = numArestas

                linhaIt += 1

                continue
            
            assert isinstance(grafoCidade, Grafo)
            
            if linhaIt <= arestas:
                entrada += linha + "\n"
                linha = linha.split(" ")
                
                centroInicio = linha[0]
                centroDestino = linha[1]
                
                grafoCidade.inserirAresta(centroInicio, centroDestino)

                linhaIt += 1
                
                continue
            
    # Ler a saida correta que o programa espera
    with open(f"./tests/outputs/testCase0{i}.txt", "r") as outFile:
        linhaIt: int = 0
        batalhoes: int = -1
        caminhosPossiveis: int = -1
        ultimaLinha: int = -1
        
        for linha in outFile:
            linha = linha.strip()
            
            if linhaIt == 0:
                grafoCidade.setCapital(linha)
                linhaIt += 1
            
            elif linhaIt == 1:
                assert linha.isnumeric()
                
                batalhoes = int(linha)
                linhaIt += 1
            
            elif linhaIt <= batalhoes + 1:
                grafoCidade.inserirBatalhao(linha)
                linhaIt += 1
            
            elif linhaIt == batalhoes + 2:
                assert linha.isnumeric()
                
                caminhosPossiveis = int(linha)
                grafoCidade.setCaminhosEsperados(caminhosPossiveis)
                ultimaLinha = linhaIt + caminhosPossiveis
                linhaIt += 1
            
            saidaEsperada += linha + "\n"
            
        # Enviar entrada para o programa
        saida, erro = processo.communicate(input=entrada)

        # Imprime possíveis erros
        if erro:
            print("Erro do programa:")
            print(erro)
        else:
            saidaEfetuada = saida

        linhas_texto2 = saidaEfetuada.splitlines()
        
        grafoCidade.validarSaida(linhas_texto2)
