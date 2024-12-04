#include "main.h"
#include "app.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void app()
{
    // set FPGA control pins
    setFpgaCs(GPIO_PIN_SET);
    setFpgaRst(GPIO_PIN_SET);

    // reset FPGA
    setFpgaRst(GPIO_PIN_RESET);
    setFpgaRst(GPIO_PIN_SET);

    while (1)
    {
        uint32_t readLen = 0;
        char recvBuffer[64];
        memset(recvBuffer, 0, sizeof(recvBuffer));

        // get first number from user
        uint16_t firstNum = 0;
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
        snprintf(sendBuffer, sizeof(sendBuffer), "\nFirst Number Received: %d\n", firstNum);
        serialPrint(sendBuffer);

        // get second number from user
        uint16_t secondNum = 0;
        serialPrint("Second Number: ");
        readLen = serialReadString(recvBuffer, sizeof(recvBuffer), 100000);
        if (readLen <= 0)
        {
            serialPrint("\nFailed to receive second number.\n");
            continue;
        }
        secondNum = strtoul(recvBuffer, NULL, 10);
        memset(sendBuffer, '\0', sizeof(sendBuffer));
        snprintf(sendBuffer, sizeof(sendBuffer), "\nSecond Number Received: %d\n", secondNum);
        serialPrint(sendBuffer);

        // calculate GCD and LCM
        uint16_t lcm[10] = {0, 1};
        uint16_t gcd[10] = {0, 1};
        bool stable = false;
        uint8_t readIdx = 0;

        do // hacky fix to confirm reliable data between 2 reads
        {
            uint16_t rxBuffer[2] = {0, 0};
            uint16_t txBuffer[2] = {firstNum, secondNum};
            setFpgaCs(GPIO_PIN_RESET);
            txRxFpgaSpi(txBuffer, rxBuffer, 2);
            setFpgaCs(GPIO_PIN_SET);

            lcm[readIdx] = rxBuffer[0];
            gcd[readIdx] = rxBuffer[1];

            readIdx++;
            readIdx %= sizeof(lcm) / sizeof(lcm[0]);

            // check stable
            stable = true;
            uint16_t lastLcm = lcm[0];
            uint16_t lastGcd = gcd[0];
            for (uint8_t ii = 0; ii < sizeof(lcm) / sizeof(lcm[0]); ii++)
            {
                if (lcm[ii] != lastLcm || gcd[ii] != lastGcd)
                {
                    stable = false;
                    break;
                }
                lastLcm = lcm[ii];
                lastGcd = gcd[ii];
            }

        } while (!stable);

        memset(sendBuffer, '\0', sizeof(sendBuffer));
        snprintf(sendBuffer, sizeof(sendBuffer), "\nGCD: %d\nLCM: %d\n\n\n", gcd[0], lcm[0]);
        serialPrint(sendBuffer);
    }
}