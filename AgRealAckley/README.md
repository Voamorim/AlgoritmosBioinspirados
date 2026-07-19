# Algoritmo Genético para Aproximar a Função Ackley

O algoritmo implementa um algoritmo genético com uma representação binária para aproximar o resultado mínimo da função Ackley.

## Compilação e Execução
O usuário pode selecionar qual das soluções ele quer executar. Para isso, é preciso inserir a flag "-s" (sem aspas) antes do id da solução desejada. Por padrão, é executada a solução básica do algoritmo.

Id das soluções:
    - 1: solução básica (padrão);
    - 2: solução que armazena os fitness dos indivíduos em csv;
    - 3: solução que aplica um teste fatorial nos parâmetros do algoritmo genético.

As instruções para execução do algoritmo foram definidas em um arquivo Makefile. O nome do executável gerado é "./ag_real".

Exemplo de compilação e execução do código em linguagem C utilizando as instruções definidas no Makefile:

```
make
./ag_real -s 1
```

## Entrada e Saída
O programa não possui entrada, portanto os parâmetros para a execução do algoritmo genético são todos definidos no código do programa.

A saída do programa depende da solução definida na execução. A solução básica imprime o resultado na saída padrão, enquanto as demais imprimem os resultados em tabelas .csv.

O resultado da solução padrão consiste apenas pelo menor valor obtido e os cromossomos do indivíduo de melhor aptidão.

### Exemplo de Saída do Programa
```
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
                  Resumo dos Resultados
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Menor valor: 0.0000000000000004
 Cromossomos: -0.0000000000000001 0.0000000000000003
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
```


