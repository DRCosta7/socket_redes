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
Cliente UDP:

Crie um socket UDP.
Envia mensagem para o servidor.
Aguarda até que a resposta do servidor seja recebida.
Processa a resposta.
Fecha o descritor do socket e saia.
*/

int main()
{
	int sock_cli;
	char buffer[MAX];
	char *mensagem = "Mensagem de teste do cliente";
	struct sockaddr_in servaddr;

	// Cria um novo socket
	// AF_INET = Protocolo IPv4, SOCK_DGRAM = Comunicação de dois lados baseada na conexão UDP
	sock_cli = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));

	// Designa protocolo IP e porta
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_port = htons(PORTA);
	servaddr.sin_addr.s_addr = INADDR_ANY; // Qualquer endereço disponível

	int dados, tamanho;

	// Envia dados ao servidor
	sendto(sock_cli, (const char *)mensagem, strlen(mensagem), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	printf("Mensagem enviada.\n");
	
	// Recebe dados que o servidor envia
	dados = recvfrom(sock_cli, (char *)buffer, MAX, MSG_WAITALL, (struct sockaddr *)&servaddr, &tamanho);
	buffer[dados] = '\0';
	printf("Servidor enviou: %s\n", buffer);
	
	// Fecha o socket
	close(sock_cli);
	return 0;
}
