#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    printf("TCP Клієнт \n");
    printf("___________\n\n");
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in ServerAddr;
    u_short Port = 5150;
    int Ret;
    char DataBuffer[1024];

    if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("Помилка WSAStartup, номер помилки %d\n", Ret);
        return -1;
    }

    if ((ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Помилка socket, номер помилки %d\n", WSAGetLastError());
        WSACleanup();
        return -2;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(Port);
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(ConnectSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
    {
        printf("Помилка connect, номер помилки %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return -3;
    }

    printf("Успішно під'єдналися до сервера.\n");

    printf("Введіть кількість значень для масиву: ");
    int arraySize;
    scanf_s("%d", &arraySize);

    char sendBuffer[1024];
    sprintf_s(sendBuffer, "%d", arraySize);

    if ((Ret = send(ConnectSocket, sendBuffer, strlen(sendBuffer), 0)) == SOCKET_ERROR)
    {
        printf("Помилка send, номер помилки %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return -4;
    }

    printf("Дані відправлено на сервер: %s\n", sendBuffer);

    if ((Ret = recv(ConnectSocket, DataBuffer, sizeof(DataBuffer), 0)) == SOCKET_ERROR)
    {
        printf("Помилка recv, номер помилки %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return -5;
    }

    if (Ret <= 1023)
        DataBuffer[Ret] = '\0';
    else
    {
        printf("Повідомлення задовге!\n");
        return -6;
    }
    DataBuffer[Ret] = '\0';
    printf("Отримано дані від сервера:\n%s\n", DataBuffer);

    closesocket(ConnectSocket);
    WSACleanup();

    printf("Натисніть Enter для завершення.\n");
    getchar();
    getchar();

    return 0;
}
