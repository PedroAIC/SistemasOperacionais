Universidade Estadual de Campinas                                         Relatório Final do Projeto 1 de "Merge and sort"

#  Descrição da solução do problema 

Inicialmente, criamos uma função chamada **FileReader** que realiza a leitura de todos os arquivos e armazena as informações em um vetor global chamado **arrayNumbers**, ainda com os números desordenados. Além disso, essa função também armazenamos na variável **iContTotalLinhas** o total de números que está sendo processado após a leitura de todos os arquivos.

Após feito a leitura do arquivo, é criado as *threads*, iniciando assim a contagem de tempo, passando a função **merge_sort** para ser executada, se as threads forem criadas. Essa função divide igualmente o número total de dados para as *threads* de forma que cada uma possa executar pedaços iguais do processamento. Caso a divisão não seja exata, é feita uma validação no programa para que uma das *threads* fique responsável por uma maior parte do processamento, assim contemplando todos os números.

Essa divisão separa um intervalo de menor e maior valor para cada thread. Por exemplo, se houver 5000 números e 8 threads, a primeira thread ficará responsável por ordenar do intervalo 1 até o intervalo 625, a segunda thread do intervalo 626 até o 1250 e assim por diante.

Após feita a separação, é aplicado o algoritmo de “*Merge and Sort*”, que consiste em separar continuamente os números dentro desse intervalo até que eles estejam sozinhos, depois disso, é feito a junção novamente desses números de forma ordenada.

Dentro do intervalo de cada *thread*, explicado anteriormente, o nosso programa ainda subdivide em mais três intervalos : Menor (variável **low**), Médio (variável **mid**) e Maior (variável **high**).

Em seguida, o programa executa a função **merge_sort1** para os intervalos de *low* até *mid* e de *mid + 1* até *high*, essa função chama a si mesma de forma recursiva para que seja calculado novamente os intervalos low, mid e high, até que esse intervalo tenha somente um número.

Quando houver somente um número no intervalo, ele executa a função **merge**, passando como parâmetros as variáveis low, mid e high. Dentro dessa função, ele armazena os números do intervalo low até o mid em um array chamado **left** e os números do intervalo mid + 1 até high em um vetor chamado **right** e, então, é feita a junção (Merge) dos números em ordem crescente através de um laço de repetição, terminando assim a contagem de tempo do programa.

Posteriormente ao término da execução de todas as threads, a variável **arrayNumbers** estará preenchida com todos os números em ordem crescente, e o programa salva essas informações em um arquivo de saída.



# Instruções de compilação e execução

Para compilar o código é necessário que o cliente coloque o **nome do arquivo** em que o código se encontra, acompanhado de -lpthread -o e o <u>nome desejado do programa</u>. Por exemplo:

- gcc **mergesort.c** -lpthread -o <u>mergesort</u>

Compilado com sucesso, chega a etapa de abastecer o programa com N arquivos contendo os valores a serem ordenados. Nessa linha de comando será necessário colocar o nome referente ao programa, definido na primeira fase, a quantidade de *threads* que serão utilizadas, seguidos dos <u>N arquivos  "alimentadores"</u> .dat e por final o -o e o nome do **arquivo de saída**. Por exemplo: 

- ./mergesort  *16* <u>arq1.dat arq2.dat arq3.dat arq4.dat arq5.dat</u> -o **saída.txt**

# Gráfico e Tabela

Todos os testes referentes a esse programa foram feitos na LP01, laboratório de informática da faculdade de tecnologia da UNICAMP, no computador de código LI014.

O Gráfico, a seguir, mostra a eficiência do programa ao executar a tarefa com 2, 4, 8 e 16 threads e diferentes números de arquivos de entrada, podendo variar de 1 a 5, com 1000 dados de conteúdo cada: 

 ![img](https://lh5.googleusercontent.com/EDVAn4f4JKjfbth_YsLMWSpP3oUXSsj-KOZgTuxwkAMRmGs7l0eFMCbW1U-vhnq3YCyOP1QvzwzEbgdJyB9slI8UUO2u57g-ThLRdob8JAgvG80xwMVdr6-H0WtWtNAYU_4z8luX) 

​																									Gráfico1:TempoXThreads													

A tabela, em seguida, mostra o tempo que as N threads demoram para finalizar  o sistema com N arquivos. 

 ![img](https://lh4.googleusercontent.com/6mnlvlogZR0l9RFOEQe5-q9GwcAIvzCLV0CRo_zMrSdf6pz12vAXFVNpjwPa8lZh7uVBhpSsMWjSSoFDq3Qay0m7VKnBeVEpSaj0zulupM3RzkR2xSKAPiPbDTZYHvBqlzKg5Ek8) 

​															Tabela1: Desempenho do código

# Conclusão

Em virtude dos resultados demonstrados no tópico anterior, pode-se concluir que quanto mais threads executam a tarefa, mais eficiente se torna o processamento dos arquivos, pois o número total de informações disponíveis está sendo lido e dividido igualmente para que cada thread faça o processamento.

O link a seguir leva ao vídeo de funcionamento do programa:
