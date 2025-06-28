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

```

## Observações

- Os parâmetros para testes fatoriais são definidos diretamente no código fonte. Ajuste como necessário antes de compilar.
- Certifique-se que os arquivos de entrada estão corretamente formatados, como nos arquivos disponibilizados em [CITIES](https://people.sc.fsu.edu/~jburkardt/datasets/cities/cities.html)

