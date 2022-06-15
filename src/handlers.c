// common  functions declareted
#include "handlers.h"

// C libraries
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// socket libraries
#include <arpa/inet.h>

// error codes
#define EXIST 1
#define NO_EXIST -1
#define INVALID_STRING -2
#define INVALID_ID -3
#define INVALID_SENSOR -4
#define INVALID_EQUIPMENT -5
#define SUCCESS 0

// terminal commands
#define ADD "add"
#define REMOVE "remove"
#define LIST "list"
#define READ "read"
#define SENSOR "sensor"
#define SENSORS "sensors"
#define KILL "kill"

// constants using in this file
#define BUFSZ 500
#define EQUIPMENT_NUMBER 4
#define SENSOR_NUMBER 4
#define SIZE_ID 3
#define SPACE ' '

int countEntryNumbers(char temporary_string[])
{
    int result = 1, counter = 0;

    while (temporary_string[counter] != '\n')
    {
        if (temporary_string[counter] == SPACE)
        {
            result += 1;
        }
        counter += 1;
    }

    return result;
}

int validateEntry(int id)
{
    if ((id > 0) && (id < 6))
    {
        return SUCCESS;
    }
    return INVALID_STRING;
}

char *cleanString(char string_temporaria[])
{
    for (int c = 0; c < SIZE_ID; c++)
    {
        if (string_temporaria[c] == '\n' || string_temporaria[c] == '\0')
        {
            string_temporaria[c] = '\0';
            return string_temporaria;
        }
    }
    return "";
}

int addSensor(int sensor_id, int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER])
{
    int response, counter = 1;
    if (validateEntry(sensor_id) == INVALID_STRING)
    {
        response = INVALID_SENSOR;
    }
    else if (validateEntry(equipment_id) == INVALID_STRING)
    {
        response = INVALID_EQUIPMENT;
    }
    else
    {
        response = INVALID_STRING;
    }

    for (int i = 1; i <= EQUIPMENT_NUMBER; i++)
    {
        for (int j = 1; j <= SENSOR_NUMBER; j++)
        {
            if ((i == equipment_id) && (j == sensor_id))
            {
                if ((data[i][j] == -1) && (counter < 16))
                {
                    data[i][j] = rand() % 1000 / 100.0;
                    response = EXIST;
                }
                else
                {
                    response = NO_EXIST;
                    break;
                }
            }
            counter++;
        }
    }
    return response;
}

int listSensorsInEquipment(char msg[], int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER])
{
    char aux[16];
    int response = NO_EXIST;

    for (int j = 1; j <= SENSOR_NUMBER; j++)
    {
        if (data[equipment_id][j] != -1.00)
        {
            sprintf(aux, "0%d ", j);
            strcat(msg, aux);

            response = EXIST;
        }
    }

    return response;
}

int removeSensorInEquipment(int sensor_id, int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER])
{
    int response = 0;

    if (validateEntry(sensor_id) == INVALID_STRING)
    {
        response = INVALID_SENSOR;
    }
    else if (validateEntry(equipment_id) == INVALID_STRING)
    {
        response = INVALID_EQUIPMENT;
    }
    else
    {
        response = INVALID_STRING;
    }

    for (int i = 1; i <= EQUIPMENT_NUMBER; i++)
    {
        for (int j = 1; j <= SENSOR_NUMBER; j++)
        {
            if ((i == equipment_id) && (j == sensor_id))
            {
                if (data[i][j] != -1)
                {
                    data[i][j] = -1;
                    response = EXIST;
                }
                else
                {
                    response = NO_EXIST;
                    break;
                }
            }
        }
    }

    return response;
}

int readVariableOfProcess(char msg[], int sensor_id, int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER])
{
    char aux[16];
    int response = 0;

    if (validateEntry(sensor_id) == INVALID_STRING)
    {
        response = INVALID_SENSOR;
    }
    else if (validateEntry(equipment_id) == INVALID_STRING)
    {
        response = INVALID_EQUIPMENT;
    }
    else if (data[equipment_id][sensor_id] != -1.00)
    {
        sprintf(aux, "%.2f ", data[equipment_id][sensor_id]);
        strcat(msg, aux);
        response = EXIST;
    }
    else
    {
        response = NO_EXIST;
    }

    return response;
}

void handle(char buf[], float data[SENSOR_NUMBER][EQUIPMENT_NUMBER])
{
    char msg[BUFSZ];
    strcpy(msg, buf);
    memset(buf, 0, BUFSZ);

    int currentPosition = 0;
    int entryNumbers = countEntryNumbers(msg);

    char *entries[entryNumbers];

    entries[0] = strtok(msg, " ");
    for (currentPosition = 1; currentPosition < entryNumbers; currentPosition++)
    {
        entries[currentPosition] = strtok(NULL, " ");
    }

    if (strncmp(KILL, entries[0], 4) == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else if ((strcmp(ADD, entries[0]) == 0) && (strcmp(SENSOR, entries[1]) == 0))
    {
        char output[BUFSZ] = "";
        char printed[20] = "";

        for (int c = 2; c < entryNumbers - 2; ++c)
        {
            if (c > 2)
                strcat(output, " ");
            int auxAdd = addSensor(atoi(entries[c]), atoi(entries[entryNumbers - 1]), data);
            if (auxAdd == EXIST)
            {
                strcat(output, cleanString(entries[c]));
                strcpy(printed, "added");
            }
            else if (auxAdd == NO_EXIST)
            {
                strcat(output, cleanString(entries[c]));
                strcpy(printed, entries[entryNumbers - 1]);
                strcat(output, " already exists in");
            }
            else if (auxAdd == INVALID_EQUIPMENT)
            {
                strcat(output, "invalid equipment");
            }
            else if (auxAdd == INVALID_SENSOR)
            {
                strcat(output, "invalid sensor");
            }
            else if (auxAdd == INVALID_STRING)
            {
                strcat(output, "invalid message");
            }
            else
            {
                strcat(output, "limit exceeded");
            }
        }

        sprintf(buf, "sensor %s %s", output, printed);
    }
    else if ((strcmp(REMOVE, entries[0]) == 0) && (strcmp(SENSOR, entries[1]) == 0))
    {
        char output[BUFSZ] = "";
        char printed[20] = "";

        int auxRemove = removeSensorInEquipment(atoi(entries[2]), atoi(entries[entryNumbers - 1]), data);
        if (auxRemove == EXIST)
        {
            strcat(output, cleanString(entries[2]));
            strcat(output, " removed");
        }
        else if (auxRemove == NO_EXIST)
        {
            strcat(output, cleanString(entries[2]));
            strcpy(printed, entries[entryNumbers - 1]);
            strcat(output, " does not exist in");
        }
        else if (auxRemove == INVALID_EQUIPMENT)
        {
            strcat(output, "invalid equipment");
        }
        else if (auxRemove == INVALID_SENSOR)
        {
            strcat(output, "invalid sensor");
        }
        else
        {
            strcat(output, "invalid message");
        }
        sprintf(buf, "sensor %s %s", output, printed);
    }
    else if ((strcmp(LIST, entries[0]) == 0) && (strcmp(SENSORS, entries[1]) == 0))
    {
        int auxList = listSensorsInEquipment(buf, atoi(entries[3]), data);
        if (auxList != EXIST)
        {
            strcat(buf, "there are no sensors installed");
        }
    }
    else if ((strcmp(READ, entries[0])) == 0)
    {
        char output[BUFSZ] = "";
        for (int c = 1; c < entryNumbers - 2; ++c)
        {
            if (c > 2)
                strcat(output, " ");
            int auxRead = readVariableOfProcess(buf, atoi(entries[c]), atoi(entries[entryNumbers - 1]), data);
            if (auxRead == EXIST)
            {
                strcat(output, cleanString(entries[c]));
            }
            else if (auxRead == NO_EXIST)
            {
                strcat(output, " sensor(s) not intalled");
            }
            else if (auxRead == INVALID_EQUIPMENT)
            {
                strcat(output, "invalid equipment");
            }
            else if (auxRead == INVALID_SENSOR)
            {
                strcat(output, "invalid sensor");
            }
            else
            {
                strcat(output, "invalid message");
            }
        }
    }
    else
    {
        exit(EXIT_SUCCESS);
    }
}
