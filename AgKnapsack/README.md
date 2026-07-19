# Algoritmo Genético para Resolver o Problema da Mochila Binária

Este projeto implementa um algoritmo genético com representação binária para encontrar uma solução aproximada do **Problema da Mochila Binária**. O objetivo é selecionar a melhor combinação de itens a serem colocados em uma mochila, respeitando o limite de peso, de forma a maximizar a utilidade total.

## Compilação e Execução

A compilação do programa é feita via `Makefile`, que gera um executável chamado `./knapsack`.

### Exemplo de Compilação
```bash
make
```

A execução permite escolher entre diferentes variações do algoritmo, usando a flag `-s` seguida do **ID da solução** desejada. Se nenhum ID for especificado, a **solução padrão (ID 1)** é executada.

### ID das Soluções Disponíveis
- `1`: solução básica (padrão);
- `2`: solução que armazena os valores de fitness dos indivíduos em um arquivo `.csv`;
- `3`: solução que aplica um teste fatorial nos parâmetros do algoritmo genético e salva os resultados em um arquivo `.csv`.

Além disso, é possível especificar os arquivos de entrada com as especificações do problema. Para mais infomações, consulte a seção [Entrada](#entrada).

### Exemplo de Execução
```bash
./knapsack -u p08_p.txt -w p08_w.txt -b p08_c.txt -s 1
```

## Entrada e Saída
### Entrada

Os arquivos de entrada devem estar localizados na pasta `inputs/`, na raiz do projeto. Por padrão, o programa assume os seguintes arquivos:
- `p08_p.txt`: utilidade dos itens;
- `p08_w.txt`: peso dos itens;
- `p08_c.txt`: capacidade máxima da mochila.

Estes arquivos podem ser substituidos. Para isso, o nome dos novos arquivos devem ser precedidos pelas seguintes flags:
- `-u`: arquivo com a utilidade dos itens;
- `-w`: arquivo com o peso dos itens;
- `-c`: arquivo com a capacidade da mochila.

Para um exemplo de uso destas flags, vá para a seção [Exemplo de Execução](#exemplo-de-execucao)

### Saída

A saída depende da solução escolhida:
- **Solução 1 (básica)**: imprime na saída padrão o fitness máximo encontrado e os itens incluídos na mochila;
- **Solução 2 (dados sobre as gerações)**: salva o fitness de cada geração no arquivo `data/generations.csv`;
- **Solução 3 (teste fatorial)**: salva os resultados de cada conjunto de parâmetros no arquivo `data/results.csv`.

### Exemplo de Saída do Programa
```
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                  Resumo dos Resultados
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Melhor fitness (maior utilidade): 13524340.000000
 Itens: 1 1 0 1 1 1 1 0 0 1 1 0 1 0 0 1 1 0 0 1 0 1 0 0
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
```

## Observações

- Os parâmetros para testes fatoriais são definidos diretamente no código-fonte. Ajuste como necessário antes de compilar.
- Certifique-se que os arquivos de entrada estão corretamente formatados, com valores numéricos separados por quebras de linha, como nos arquivos disponibilizados em [KNAPSACK_01](https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/)
