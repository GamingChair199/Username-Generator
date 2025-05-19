#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 100
#define MAX_SEGMENTS 10
#define MAX_USERNAME_LEN 256

typedef struct {
    char filename[100];
    int line_count;
} WordList;


int count_lines(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return -1;
    int count = 0;
    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), f)) {
        count++;
    }
    fclose(f);
    return count;
}


int read_random_line(const char* filename, int line_count, char* out_str) {
    if (line_count <= 0) return -1;

    int target_line = rand() % line_count;
    FILE* f = fopen(filename, "r");
    if (!f) return -1;

    char buffer[MAX_LINE_LEN];
    int current_line = 0;
    while (fgets(buffer, sizeof(buffer), f)) {
        if (current_line == target_line) {
            
            buffer[strcspn(buffer, "\r\n")] = 0;
            strcpy(out_str, buffer);
            fclose(f);
            return 0;
        }
        current_line++;
    }

    fclose(f);
    return -1;
}

int main() {
    srand(time(NULL));

    int segment_count;
    printf("Wie viele Segmente soll der Username haben? (max %d): ", MAX_SEGMENTS);
    scanf("%d", &segment_count);
    while (getchar() != '\n');

    if (segment_count < 1 || segment_count > MAX_SEGMENTS) {
        printf("Ungültige Segmentanzahl.\n");
        return 1;
    }

    WordList segments[MAX_SEGMENTS];

   
    for (int i = 0; i < segment_count; i++) {
        printf("Dateiname für Segment %d: ", i + 1);
        fgets(segments[i].filename, sizeof(segments[i].filename), stdin);
        segments[i].filename[strcspn(segments[i].filename, "\r\n")] = 0; 

        segments[i].line_count = count_lines(segments[i].filename);
        if (segments[i].line_count <= 0) {
            printf("Fehler: Datei '%s' nicht gefunden oder leer.\n", segments[i].filename);
            return 1;
        }
    }

    FILE* fout = fopen("usernames.txt", "w");
    if (!fout) {
        printf("Kann Ausgabedatei nicht öffnen.\n");
        return 1;
    }

    char segment_word[MAX_LINE_LEN];
    char username[MAX_USERNAME_LEN];

   
    for (int i = 0; i < 5; i++) {
        username[0] = 0; 

        for (int s = 0; s < segment_count; s++) {
            if (read_random_line(segments[s].filename, segments[s].line_count, segment_word) == 0) {
                strcat(username, segment_word);
            } else {
                printf("Fehler beim Lesen von Segment %d\n", s + 1);
                fclose(fout);
                return 1;
            }
        }

        fprintf(fout, "%s\n", username);
        printf("Username %d: %s\n", i + 1, username);
    }

    fclose(fout);
    printf("Fertig! Usernames stehen in 'usernames.txt'\n");

    return 0;
}
