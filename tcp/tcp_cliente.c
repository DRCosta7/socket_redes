#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA 8080
#define TAM_BUFFER 80

int main()
{

    int sock_clien, ret;
    struct sockaddr_in serverAddr;
    char buffer[TAM_BUFFER];

    // Cria socket do servidor
    // AF_INET = Protocolo IPv4, SOCK_STREAM = Comunicação de dois lados baseada na conexão TCP
    sock_clien = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_clien < 0)
    {
        printf("Erro na conexao.\n");
        exit(1);
    }
    printf("Socket do cliente criado.\n");

    // Limpa endereço do server
    memset(&serverAddr, '\0', sizeof(serverAddr));

    // Designa protocolo IP e porta
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(PORTA);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // Associa socket a um endereço especificado na estrutura
    ret = connect(sock_clien, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        printf("Erro na conexao.\n");
        exit(1);
    }
    printf("Conectado ao servidor.\n");

    while (1)
    {
        printf("Cliente: ");

        int pos_buff = 0;
        // Limpa buffer
        bzero(buffer, sizeof(buffer));

        // Copia mensagem do cliente para o buffer até apertar enter
        while ((buffer[pos_buff++] = getchar()) != '\n');

        // Envia mensagem
        send(sock_clien, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "sair") == 0)
        {
            close(sock_clien);
            printf("Desconectado do servidor.\n");
            exit(1);
        }

        if (recv(sock_clien, buffer, TAM_BUFFER, 0) < 0)
        {
            printf("Erro ao receber dados.\n");
        }
        else
        {
            printf("Servidor: %s", buffer);
        }
    }

    return 0;
}