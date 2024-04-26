//
//  main.cpp
//  FT_3
//
//  Created by Эмиль Саубанов on 26.04.2024.
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Создание дескриптора сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к указанному порту
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Привязка сокета к порту и адресу
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Прослушивание входящих соединений
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Принятие входящего соединения
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Обработка сообщений от клиента
    while (true) {
        // Чтение данных от клиента
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("read");
            break;
        } else if (valread == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        // Подтверждение получения сообщения
        const char* acknowledgment = "Message received";
        send(new_socket, acknowledgment, strlen(acknowledgment), 0);

        // Вывод сообщения от клиента
        printf("Client: %s\n", buffer);

        // Очистка буфера
        memset(buffer, 0, BUFFER_SIZE);
    }

    // Закрытие сокетов
    close(new_socket);
    close(server_fd);

    return 0;
}

