#pragma once

#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>

#define EQUIPMENT_NUMBER 4
#define SENSOR_NUMBER 4
#define SIZE_ID 3

int countEntryNumbers(char temporary_string[]);

int validateEntry(int id);

char *cleanString(char string_temporaria[]);

int addSensor(int sensor_id, int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER]);

int listSensorsInEquipment(char msg[], int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER]);

int removeSensorInEquipment(int sensor_id, int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER]);

int readVariableOfProcess(char msg[], int sensor_id, int equipment_id, float data[SENSOR_NUMBER][EQUIPMENT_NUMBER]);

void handle(char buf[], float data[SENSOR_NUMBER][EQUIPMENT_NUMBER]);