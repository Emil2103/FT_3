//
//  main.cpp
//  FT_3_Client
//
//  Created by Эмиль Саубанов on 26.04.2024.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char* message = "Hello from client";

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразование IP-адреса и присоединение к серверу
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        return -1;
    }

    // Отправка сообщения на сервер
    send(sock, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);

    // Прием подтверждения от сервера
    valread = read(sock, buffer, BUFFER_SIZE);
    printf("Server response: %s\n", buffer);

    close(sock);
    return 0;
}

