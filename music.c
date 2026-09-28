#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Song {
    int id;
    char title[50];
    char artist[50];
    char album[50];
    char genre[30];
};

void addSong();
void viewSongs();
void searchSong();
void deleteSong();

int main() {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("        MUSIC MANAGEMENT SYSTEM         \n");
        printf("========================================\n");
        printf("1. Add New Song\n");
        printf("2. View All Songs\n");
        printf("3. Search Song by Title\n");
        printf("4. Delete Song Record\n");
        printf("5. Exit\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addSong(); break;
            case 2: viewSongs(); break;
            case 3: searchSong(); break;
            case 4: deleteSong(); break;
            case 5:
                printf("\nExiting Music Management System. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}

void addSong() {
    FILE *fp = fopen("music.txt", "a");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    struct Song s;
    printf("\nEnter Song ID: ");
    scanf("%d", &s.id);
    getchar(); // Clear buffer

    printf("Enter Song Title: ");
    fgets(s.title, sizeof(s.title), stdin);
    s.title[strcspn(s.title, "\n")] = 0;

    printf("Enter Artist Name: ");
    fgets(s.artist, sizeof(s.artist), stdin);
    s.artist[strcspn(s.artist, "\n")] = 0;

    printf("Enter Album Name: ");
    fgets(s.album, sizeof(s.album), stdin);
    s.album[strcspn(s.album, "\n")] = 0;

    printf("Enter Genre: ");
    fgets(s.genre, sizeof(s.genre), stdin);
    s.genre[strcspn(s.genre, "\n")] = 0;

    fwrite(&s, sizeof(struct Song), 1, fp);
    fclose(fp);

    printf("\nSong record added successfully!\n");
}

void viewSongs() {
    FILE *fp = fopen("music.txt", "r");
    if (fp == NULL) {
        printf("\nNo music records found.\n");
        return;
    }

    struct Song s;
    printf("\n%-6s %-22s %-20s %-20s %-15s\n", "ID", "Title", "Artist", "Album", "Genre");
    printf("----------------------------------------------------------------------------------\n");

    while (fread(&s, sizeof(struct Song), 1, fp)) {
        printf("%-6d %-22s %-20s %-20s %-15s\n", s.id, s.title, s.artist, s.album, s.genre);
    }

    fclose(fp);
}

void searchSong() {
    FILE *fp = fopen("music.txt", "r");
    if (fp == NULL) {
        printf("\nNo music records found.\n");
        return;
    }

    char searchTitle[50];
    int found = 0;
    struct Song s;

    getchar(); // Clear buffer
    printf("\nEnter Song Title to search: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = 0;

    while (fread(&s, sizeof(struct Song), 1, fp)) {
        if (strcasecmp(s.title, searchTitle) == 0) {
            printf("\n--- Song Details ---\n");
            printf("ID: %d\n", s.id);
            printf("Title: %s\n", s.title);
            printf("Artist: %s\n", s.artist);
            printf("Album: %s\n", s.album);
            printf("Genre: %s\n", s.genre);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nSong titled '%s' not found.\n", searchTitle);
    }

    fclose(fp);
}

void deleteSong() {
    FILE *fp = fopen("music.txt", "r");
    if (fp == NULL) {
        printf("\nNo music records found.\n");
        return;
    }

    FILE *tempFp = fopen("temp.txt", "w");
    if (tempFp == NULL) {
        printf("\nError opening temporary file!\n");
        fclose(fp);
        return;
    }

    int deleteId, found = 0;
    struct Song s;

    printf("\nEnter Song ID to delete: ");
    scanf("%d", &deleteId);

    while (fread(&s, sizeof(struct Song), 1, fp)) {
        if (s.id == deleteId) {
            found = 1;
        } else {
            fwrite(&s, sizeof(struct Song), 1, tempFp);
        }
    }

    fclose(fp);
    fclose(tempFp);

    remove("music.txt");
    rename("temp.txt", "music.txt");

    if (found) {
        printf("\nSong record deleted successfully!\n");
    } else {
        printf("\nSong ID %d not found.\n", deleteId);
    }
}