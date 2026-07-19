# Otimização por Colônia de Formigas para Resolver o Problema do Caixeiro Viajante (PCV)

Este projeto implementa um algoritmo de otimização por Colônia de Formigas para encontrar uma solução aproximada do **Problema do Caixeira Viajante (PCV)**. O objetivo é encontrar a melhor combinação de arestas a serem utilizadas para visitar todos os vértices de um grafo completo exatamente uma vez e retornar para o vértice inicial, de forma a minimizar o custo total das arestas utilizadas.

## Compilação e Execução

A compilação do programa é feita via `Makefile`, que gera um executável chamado `./aco`.

### Exemplo de Compilação
```bash
make
```

A execução permite escolher entre diferentes variações do algoritmo, usando a flag `-s` seguida do **ID da solução** desejada. Se nenhum ID for especificado, a **solução padrão (ID1)** é executada.

### ID das Soluções Disponíveis
- `1`: solução básica (padrão)
- `2`: solução que armazena os valores de fitness das formigas em um arquivo `.csv`.
- `3`: solução que aplica um teste fatorial nos parâmetros do algoritmo genético e salva os resultados em um arquivo `.csv`.


Além disso, é possível especificar os arquivos de entrada com as especificações do problema e o arquivo de saída (para as soluções de ID `2` e `3`). Para mais informações, consulte a seção [Entrada](#entrada).

### Exemplo de Execução
```bash
./aco -i sgb128_dist.txt -o results.csv -s 3
```

# Entrada e Saída
## Entrada

O arquivo de entrada deve estar localizado na pasta `inputs/`, localizado na raíz do projeto. Por padrão, é assumido o arquivo de entrada: `sgb128_dist.txt`.

Este arquivo pode ser substituído. Para isso, o nome do novo arquivo deve ser precedido pela flag `-i` como parâmetro de execução.

Para um exemplo de uso desta flag, vá para a seção [Exemplo de Execução](#exemplo-de-execucao)

### Saida

A saida depende da solução escolhida:
- **Solução 1 (básica)**: imprime na saída padrão o custo e o ciclo de custo mínimo encontrado durante a execução do algoritmo;
- **Solução 2 (dados sobre as gerações)**: por padrão, salva o fitness de cada geração no arquivo `data/generations.csv`. É possível substituir o nome do arquivo de saída utilizando a flag `-o` seguida pelo nome do novo arquivo de saída.
- **Solução 3 (teste fatorial)**: por padrão, salva os resultados de cada conjunto de parâmetros no arquivo `data/results.csv`. É possível substituir o nome do arquivo de saída utilizando a flag `-o`seguida pelo nome do novo arquivo de saída.

### Exemplo de Saída do Programa
```
Ciclo de menor custo encontrado:
75 -> 1 -> 9 -> 115 -> 93 -> 4 -> 107 -> 86 -> 41 -> 91 -> 43 -> 76 -> 22 -> 65 -> 127 -> 126 -> 80 -> 17 -> 53 -> 33 -> 72 -> 21 -> 37 -> 10 -> 28 -> 77 -> 11 -> 34 -> 73 -> 59 -> 109 -> 87 -> 97 -> 85 -> 25 -> 82 -> 24 -> 121 -> 6 -> 7 -> 98 -> 8 -> 38 -> 104 -> 113 -> 124 -> 95 -> 63 -> 123 -> 12 -> 58 -> 46 -> 101 -> 122 -> 100 -> 13 -> 114 -> 74 -> 111 -> 29 -> 89 -> 3 -> 92 -> 119 -> 5 -> 27 -> 42 -> 79 -> 20 -> 51 -> 103 -> 35 -> 15 -> 14 -> 117 -> 96 -> 56 -> 36 -> 83 -> 105 -> 40 -> 66 -> 62 -> 94 -> 64 -> 2 -> 49 -> 48 -> 57 -> 81 -> 44 -> 78 -> 23 -> 61 -> 45 -> 19 -> 110 -> 50 -> 99 -> 84 -> 31 -> 55 -> 120 -> 68 -> 70 -> 47 -> 69 -> 71 -> 0 -> 90 -> 30 -> 102 -> 106 -> 125 -> 39 -> 60 -> 116 -> 88 -> 108 -> 54 -> 118 -> 112 -> 67 -> 18 -> 26 -> 16 -> 52 -> 32 -> 75
Custo: 19896.00
```

## Observações

- Os parâmetros para testes fatoriais são definidos diretamente no código fonte. Ajuste como necessário antes de compilar.
- Certifique-se que os arquivos de entrada estão corretamente formatados, como nos arquivos disponibilizados em [CITIES](https://people.sc.fsu.edu/~jburkardt/datasets/cities/cities.html)
