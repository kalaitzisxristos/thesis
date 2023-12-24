#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int timeTokenToMillis (char * token) {
  int tokenIdx = 0;
  while (token[tokenIdx++] != 'T');

  int timeHours = atoi((char[]) { token[tokenIdx++], token[tokenIdx++], '\0' });
  int timeMinutes = atoi((char[]) { token[tokenIdx++], token[tokenIdx++], '\0' });
  int timeSeconds = atoi((char[]) { token[tokenIdx++], token[tokenIdx++], '\0' });
  int timeMillis = atoi((char[]) { token[tokenIdx++], token[tokenIdx++], token[tokenIdx++], '\0' });

  return
    timeMillis +
    timeSeconds * 1000 +
    timeMinutes * 1000 * 60 +
    timeHours * 1000 * 60 * 60;
}

int main () {
  char *line = NULL;
  size_t size = 0;
  while (getline(&line, &size, stdin) > -1) {
    if (line[0] < '0' || line[0] > '9') continue;

    int lineIdx = 0;
    while (line[++lineIdx] != '\n');
    line[lineIdx] = '\0';

    char *token = strtok(line, ";");

    int time = timeTokenToMillis(token);
    token = strtok(NULL, ";");

    int isExtended = token[0] == '0' ? 0 : 1;
    token = strtok(NULL, ";");

    char s_id[5] = { 0 };
    strcpy(s_id, token);
    unsigned long long id = strtoll(s_id, NULL, 16);
    token = strtok(NULL, ";");

    char s_data[17] = { 0 };
    strcpy(s_data, token);
    unsigned long long data = strtoll(s_data, NULL, 16);

    printf(", { .time = %012d, .isExtended = %d, .id = 0x%04x, .size = %d, .data = 0x%016llx }\n",
           time,
           isExtended,
           id,
           strlen(s_data) >> 1,
           data);
  }
}

