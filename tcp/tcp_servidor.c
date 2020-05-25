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

    int sock_sv, ret;
    struct sockaddr_in endereco_server;

    int novo_socket;
    struct sockaddr_in novo_endereco;

    socklen_t tam_end;

    char buffer[TAM_BUFFER];
    pid_t pid_filho;

    // Cria socket do servidor
    // AF_INET = Protocolo IPv4, SOCK_STREAM = Comunicação de dois lados baseada na conexão TCP
    sock_sv = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_sv < 0)
    {
        printf("Erro na conexao.\n");
        exit(1);
    }
    else
    {
        printf("Socket do servidor criado.\n");
    }

    // Limpa endereço do server
    memset(&endereco_server, '\0', sizeof(endereco_server));

    // Designa protocolo IP e porta
    endereco_server.sin_family = AF_INET; // IPv4
    endereco_server.sin_port = htons(PORTA);
    endereco_server.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    // Associa socket a um endereço especificado na estrutura
    ret = bind(sock_sv, (struct sockaddr *)&endereco_server, sizeof(endereco_server));

    if (ret < 0)
    {
        printf("Erro no bind.\n");
        exit(1);
    }
    printf("Bind na porta %d\n", PORTA);

    // Permite que o socket receba conexões e limita o número de pessoas conectadas simultaneamente, 5
    if (listen(sock_sv, 5) == 0)
    {
        printf("Ouvindo...\n");
    }
    else
    {
        printf("Erro no listen.\n");
    }

    while (1)
    {
        // Aceita o pacote de dados do cliente
        novo_socket = accept(sock_sv, (struct sockaddr *)&novo_endereco, &tam_end);
        if (novo_socket < 0)
        {
            exit(1);
        }
        printf("Conexao aceita de %s:%d\n", inet_ntoa(novo_endereco.sin_addr), ntohs(novo_endereco.sin_port));

        if ((pid_filho = fork()) == 0)
        {
            close(sock_sv);

            while (1)
            {   // Limpa buffer
                bzero(buffer, sizeof(buffer));

                // Recebe buffer no novo socket
                recv(novo_socket, buffer, TAM_BUFFER, 0);

                if (strcmp(buffer, "sair") == 0)
                {
                    printf("Desconectado %s:%d\n", inet_ntoa(novo_endereco.sin_addr), ntohs(novo_endereco.sin_port));
                    break;
                }
                else
                {
                    printf("Cliente: %s", buffer);

                    // Envia buffer de volta para o cliente
                    send(novo_socket, buffer, strlen(buffer), 0);

                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }

    close(novo_socket);

    return 0;
}