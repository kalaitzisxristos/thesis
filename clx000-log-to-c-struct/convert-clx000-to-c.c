#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {
  char *line = NULL;
  size_t size;

  char formattedTime[64];

  char s_timeHours[3] = { 0 };
  char s_timeMinutes[3] = { 0 };
  char s_timeSeconds[3] = { 0 };
  char s_timeMillis[4] = { 0 };

  while (getline(&line, &size, stdin) > -1) {
    if (line[0] < '0' || line[0] > '9') continue;

    int lineIdx = 0;
    while (line[lineIdx++] != 'T');

    strncpy(s_timeHours, &line[lineIdx], 2);
    lineIdx += 2;
    strncpy(s_timeMinutes, &line[lineIdx], 2);
    lineIdx += 2;
    strncpy(s_timeSeconds, &line[lineIdx], 2);
    lineIdx += 2;
    strncpy(s_timeMillis, &line[lineIdx], 3);
    lineIdx += 3;

    int timeHours = atoi(s_timeHours);
    int timeMinutes = atoi(s_timeMinutes);
    int timeSeconds = atoi(s_timeSeconds);
    int timeMillis = atoi(s_timeMillis);
    int time = timeMillis
      + 1000 * timeSeconds
      + 1000 * 60 * timeMinutes
      + 1000 * 60 * 60 * timeHours;

    lineIdx++;

    int isExtended = line[lineIdx] == '0' ? 0 : 1;
    lineIdx += 1;

    lineIdx++;

    int idBegin = lineIdx;
    while (line[++lineIdx] != ';');
    int idEnd = lineIdx;
    char s_id[5] = { 0 };
    strncpy(s_id, &line[idBegin], idEnd - idBegin);

    lineIdx++;

    int dataBegin = lineIdx;
    while (line[++lineIdx] != '\n');
    int dataEnd = lineIdx;
    char s_data[17] = { 0 };
    strncpy(s_data, &line[dataBegin], dataEnd - dataBegin);
    unsigned long long data = strtoll(s_data, NULL, 16);

    printf(", { .time = %d, .isExtended = %d, .id = 0x%s, .size = %d, .data = 0x%016llx }\n",
           time,
           isExtended,
           s_id,
           (dataEnd - dataBegin) / 2,
           data);
  }
}

