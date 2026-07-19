# Otimização por Nuvem de Partículas (PSO) para Minimizar a Função de Ackley

Este projeto implementa um algoritmo de otimização por **Nuvem de Partículas (PSO)**, também conhecido como **Particle Swarm Otimization**, para minimizar a função de **Ackley**.

## Compilação e Execução

A compilação do programa é feita via `Makefile`, que gera um executável chamado `./pso`.

### Exemplo de Compilação
```bash
make
```

A execução permite escolher entre diferentes variações do algoritmo, usando a *flag* `-s` seguida do **ID da solução** desejada. Se nenhum ID for especificado, a **solução padrão (ID 1)** é executada.

### ID das Soluções Disponíveis
- `1`: solução básica (padrão)
- `2`: solução que armazena os valores de fitness das formigas em um arquivo `.csv`.
- `3`: solução que aplica um teste fatorial nos parâmetros do algoritmo genético e salva os resultados em um arquivo `.csv`.

Além disso, é possível especificar qual topologia será utilizada pelo algoritmo, usando a *flag* `-t` seguida do **ID da topologia** desejada. Se nenhum ID for especificado, a **Topologia de Clique (ID 1)** é a utilizada.

### ID das Topologias Disponíveis
- `1`: topologia de clique (padrão), na qual todas as partículas têm acesso ao melhor global encontrado pelo enxame;
- `2`: topologia de grid, na qual as partículas tem acesso somente acesso à melhor solução encontrada por 4 vizinhos adjacentes.

Por fim, também é possível especificar o arquivo de saída (para as soluções de ID `2` e `3`). Para mais informações, consulte a seção [Saída do Programa](#saída-do-programa).

### Formato do Comando de execução
```bash
./pso -s <ID_da_solução> -t <ID_da_topologia> -o <nome_arquivo_de_saída>
```

# Saída do Programa

A saída do programa depende da solução escolhida:
- **Solução 1 (básica)**: imprime na saída padrão o valor mínimo encontrado, a posição da partícula que encontrou este valor e a geração em que o valor foi encontrado;
- **Solução 2 (dados sobre as gerações)**: por padrão, salva o *fitness* de cada geração no arquivo `data/tables/generations.csv`. É possível substituir o nome do arquivo de saída utilizando a *flag* `-o`, seguida pelo nome do novo arquivo de saída;
- **Solução 3 (teste fatorial)**: por padrão, salva os resultados de cada conjunto de parâmetros no arquivo `data/tables/results.csv`. É possível substituir o nome do arquivo de saída utilizando a *flag* `-o`, seguida pelo nome do novo arquivo de saída.

## Exemplo de Saída do Programa
```
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    Algoritmo de Otimizacao por Enxame de Particulas
            para otimizar a funcao de Ackley
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
- Melhor resultado encontrado: -0.0000000000000001
- Posicao: (0.0000000000000000, -0.0000000000000000)
- Encontrado na geracao: 101
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
```

## Observações
- Os parâmetros para testes fatoriais são definidos diretamente no código fonte. Ajuste como necessário antes de compilar.
