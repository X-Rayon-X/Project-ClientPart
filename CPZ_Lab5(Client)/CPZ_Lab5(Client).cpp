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
    printf("TCP �볺�� \n");
    printf("___________\n\n");
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in ServerAddr;
    u_short Port = 5150;
    int Ret;
    char DataBuffer[1024];

    if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        printf("������� WSAStartup, ����� ������� %d\n", Ret);
        return -1;
    }

    if ((ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("������� socket, ����� ������� %d\n", WSAGetLastError());
        WSACleanup();
        return -2;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(Port);
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(ConnectSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
    {
        printf("������� connect, ����� ������� %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return -3;
    }

    printf("������ ��'�������� �� �������.\n");

    printf("������ ������� ������� ��� ������: ");
    int arraySize;
    scanf_s("%d", &arraySize);

    char sendBuffer[1024];
    sprintf_s(sendBuffer, "%d", arraySize);

    if ((Ret = send(ConnectSocket, sendBuffer, strlen(sendBuffer), 0)) == SOCKET_ERROR)
    {
        printf("������� send, ����� ������� %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return -4;
    }

    printf("��� ���������� �� ������: %s\n", sendBuffer);

    if ((Ret = recv(ConnectSocket, DataBuffer, sizeof(DataBuffer), 0)) == SOCKET_ERROR)
    {
        printf("������� recv, ����� ������� %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return -5;
    }

    if (Ret <= 1023)
        DataBuffer[Ret] = '\0';
    else
    {
        printf("����������� �������!\n");
        return -6;
    }
    DataBuffer[Ret] = '\0';
    printf("�������� ��� �� �������:\n%s\n", DataBuffer);

    closesocket(ConnectSocket);
    WSACleanup();

    printf("�������� Enter ��� ����������.\n");
    getchar();
    getchar();

    return 0;
}
