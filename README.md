# Redes-TP02

Neste trabalho prático, você será responsável por desenvolver um sistema que coordene múltiplas
conexões simultâneas entre os equipamentos e permita a comunicação entre eles. Neste sistema, você
deve desenvolver um servidor, responsável por coordenar as múltiplas conexões à medida em que os
equipamentos entram e saem da rede, e os equipamentos (os clientes), que solicitam informações de
outros equipamentos por intermédio do servidor. Toda conexão deve utilizar a interface de sockets na
linguagem C.

Você desenvolverá 2 programas para um sistema simples de troca de mensagens de texto utilizando
apenas as funcionalidades da biblioteca de sockets POSIX e a comunicação via protocolo TCP. O código do
servidor deverá utilizar múltiplas threads para a manutenção das múltiplas conexões. As próximas seções
detalham o que cada entidade (servidor ou equipamento) deve fazer.
Os objetivos gerais deste trabalho são:
  1. Implementar servidor utilizando a interface de sockets na linguagem C;
  2. Implementar equipamento utilizando a interface de sockets na linguagem C;
  3. Escrever o relatório;
