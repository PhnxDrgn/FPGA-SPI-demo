#include "main.h"
#include "app.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void app()
{
    while (1)
    {
        uint32_t readLen = 0;
        char recvBuffer[64];
        memset(recvBuffer, 0, sizeof(recvBuffer));

        // get first number from user
        uint32_t firstNum = 0;
        serialPrint("First Number: ");
        readLen = serialReadString(recvBuffer, sizeof(recvBuffer), 100000);
        if (readLen <= 0)
        {
            serialPrint("\nFailed to receive first number.\n");
            continue;
        }
        firstNum = strtoul(recvBuffer, NULL, 10);
        char sendBuffer[64];
        memset(sendBuffer, '\0', sizeof(sendBuffer));
        snprintf(sendBuffer, sizeof(sendBuffer), "\nFirst Number Received: %ld\n", firstNum);
        serialPrint(sendBuffer);

        // get second number from user
        uint32_t secondNum = 0;
        serialPrint("Second Number: ");
        readLen = serialReadString(recvBuffer, sizeof(recvBuffer), 100000);
        if (readLen <= 0)
        {
            serialPrint("\nFailed to receive second number.\n");
            continue;
        }
        secondNum = strtoul(recvBuffer, NULL, 10);
        memset(sendBuffer, '\0', sizeof(sendBuffer));
        snprintf(sendBuffer, sizeof(sendBuffer), "\nSecond Number Received: %ld\n", secondNum);
        serialPrint(sendBuffer);
    }
}