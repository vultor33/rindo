server
gera o fitness atraves de varias chamadas no client

client
fica em outra pasta, em geral, rodando, junto com os inputs do orca


RESUMO DO PROJETO

Passo 0
O otimizador chama rindo.exe

Passo 1
O rindo.exe copia o paras.txt para todas
as máquinas destino.

Passo 2
O rindo.exe fornece os nomes e ordena que os clientes
sejam executados atraves de varios scripts

Passo 2.1
O client copia o paras.txt no final do exemplo.mol
criando o exemplo.inp

Passo 2.2
O client executa o orca

Passo 2.3
O client deleta os arquivos lixo: .cw
O client procura pelo nome a sua referencia
e calcula abs(ref - resultado)
ele escreve no exemplo.fit a quantidade que precisar ser escrita

Passo 3
O servidor saberá que todos os clientes foram executados.
Portanto ele irá coletar (sftp get) todos os arquivos
de saída.

Passo 4
O servidor irá escrever um fitness.txt para o otimizador
--> Nesse ponto será feito uma mediana ou a fitness normal
a cargo do operador.

Passo 5
shut down.



