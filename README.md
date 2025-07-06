# Algoritmo de Otimização por Seleção Clonal (CLONALG) para Minimizar Soluções para o Problema do Caixeiro Viajante (PCV)

Este projeto implementa um algoritmo de otimização por seleção clonal (CLONALG) para encontrar uma solução aproximada para o **Problema do Caixeiro Viajante**. O objetivo é encontrar a melhor combinação de arestas a serem utilizadas para visitar todos os vértices de um grafo completo exatamente uma vez e retornar para o vértice inicial, de forma a minimizar o custo total das arestas utilizadas.

## Compilação e Execução

A compilação do programa é feita via `Makefile`, que gera um executável chamado `./clonalg`.

### Exemplo de Compilação 
```bash
make
```

A execução permite escolher entre diferentes variações do algoritmo, usando a *flag* `-s` sequida do **ID da solução** desejada. Se nenhum ID for especificado, a **solução padrão (ID 1)** é executada.

### ID das Soluções Disponíveis
- `1`: solução básica (padrão)
- `2`: solução que armazena os valores de fitness dos anticorpos em um arquivo `.csv`.
- `3`: solução que aplica um teste fatorial nos parâmetros do algoritmo genético e salva os resultados em um arquivo `.csv`.

Além disso, é possível especificar os arquivos de entrada com as especificações do problema e o arquivo de saída (para as soluções de ID `2` e `3`). Para mais informações, consulte a seção [Entrada](#entrada).

### Formato do Comando de execução
```bash
./clonalg -s <ID_da_solução> -i <nome_arquivo_de_entrada> -o <nome_arquivo_de_saída>
```

# Entrada e Saída
## Entrada

O arquivo de entrada deve estar localizado na pasta `inputs/`, localizado na raíz do projeto. Por padrão, é assumido o arquivo de entrada: `sgb128_dist.txt`.

Este arquivo pode ser substituído. Para isso, o nome do novo arquivo deve ser precedido pela flag `-i` como parâmetro de execução.

Para um exemplo de uso desta flag, vá para a seção [Exemplo de Execução](#exemplo-de-execucao)

## Saída

A saida depende da solução escolhida:
- **Solução 1 (básica)**: imprime na saída padrão o custo e o ciclo de custo mínimo encontrado durante a execução do algoritmo;
- **Solução 2 (dados sobre as gerações)**: por padrão, salva o fitness de cada indivíduo na geração no arquivo `data/generations.csv`. É possível substituir o nome do arquivo de saída utilizando a flag `-o` seguida pelo nome do novo arquivo de saída.
- **Solução 3 (teste fatorial)**: por padrão, salva os resultados de cada conjunto de parâmetros no arquivo `data/results.csv`. É possível substituir o nome do arquivo de saída utilizando a flag `-o`seguida pelo nome do novo arquivo de saída.

### Exemplo de Saída do Programa
```
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
       Algoritmo de Seleção Clonal (CLONALG) para        
     minimizar o Problema do Caixeiro Viajante (PCV)     
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
- Custo do menor ciclo encontrado: 40609.00
- Ciclo: 100 -> 126 -> 116 -> 92 -> 117 -> 94 -> 20 -> 110 -> 50 -> 35 -> 47 -> 86 -> 113 -> 59 -> 127 -> 62 -> 104 -> 32 -> 121 -> 40 -> 7 -> 77 -> 28 -> 73 -> 43 -> 55 -> 60 -> 39 -> 68 -> 70 -> 120 -> 83 -> 13 -> 114 -> 17 -> 69 -> 61 -> 122 -> 14 -> 30 -> 33 -> 49 -> 18 -> 76 -> 34 -> 67 -> 79 -> 52 -> 103 -> 42 -> 66 -> 10 -> 19 -> 57 -> 16 -> 26 -> 91 -> 63 -> 115 -> 84 -> 31 -> 125 -> 85 -> 111 -> 119 -> 109 -> 78 -> 23 -> 27 -> 87 -> 1 -> 51 -> 46 -> 126 -> 98 -> 112 -> 118 -> 80 -> 106 -> 24 -> 65 -> 95 -> 81 -> 54 -> 25 -> 82 -> 29 -> 72 -> 101 -> 124 -> 53 -> 11 -> 22 -> 15 -> 2 -> 64 -> 48 -> 71 -> 90 -> 4 -> 93 -> 107 -> 36 -> 56 -> 99 -> 74 -> 105 -> 58 -> 21 -> 37 -> 9 -> 5 -> 96 -> 3 -> 89 -> 97 -> 38 -> 6 -> 12 -> 0 -> 41 -> 8 -> 45 -> 75 -> 44 -> 88 -> 108 -> 102 -> 123
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
```

## Observações

- Os parâmetros para testes fatoriais são definidos diretamente no código fonte. Ajuste como necessário antes de compilar.
- Certifique-se que os arquivos de entrada estão corretamente formatados, como nos arquivos disponibilizados em [CITIES](https://people.sc.fsu.edu/~jburkardt/datasets/cities/cities.html)

