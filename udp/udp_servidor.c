#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORTA 8080
#define MAX 256

/*
Servidor UDP:

Cria um socket UDP.
Liga o socket ao endereço do servidor.
Aguarda até o pacote de datagramas chegar do cliente.
Processa o pacote de datagrama e envia uma resposta ao cliente.
*/

int main()
{
	int sock_sv;
	char buffer[MAX];
	char *mensagem = "Mensagem de teste do servidor";
	struct sockaddr_in servaddr, cliaddr;

	// Cria um novo socket
	// AF_INET = Protocolo IPv4, SOCK_DGRAM = Comunicação de dois lados baseada na conexão UDP
	sock_sv = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Designa protocolo IP e porta
	servaddr.sin_family = AF_INET;		   // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // Qualquer endereço disponível
	servaddr.sin_port = htons(PORTA);

	// Associa socket a um endereço especificado na estrutura
	bind(sock_sv, (const struct sockaddr *)&servaddr, sizeof(servaddr));

	int tam, dados;

	tam = sizeof(cliaddr);

	// Recebe dados que o cliente envia
	dados = recvfrom(sock_sv, (char *)buffer, MAX, MSG_WAITALL, (struct sockaddr *)&cliaddr, &tam);

	buffer[dados] = '\0';
	printf("Mensagem do cliente: %s\n", buffer);

	// Envia dados para o cliente
	sendto(sock_sv, (const char *)mensagem, strlen(mensagem), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, tam);
	printf("Mensagem do servidor enviada.\n");

	return 0;
}
