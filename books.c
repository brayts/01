//
// Created by views on 23.05.18.
//

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "books.h"
#include "errors.h"
/*
int getline(FILE *file, char * format, size_t size) {
    for (int i; i < size; i++) {
        
    }
}
*/
static int main() {
    books_menu(false);
}

int books_menu(bool top_level_menu) {
    int item_number;
    do {
        printf("1 - Add book\n");
        printf("2 - Remove book\n");
        printf("3 - Find book\n");
        printf("4 - Show books\n");
        printf("9 - Exit\n");

        if (top_level_menu) {
            printf("0 - Back\n");
        }

        printf("Select menu item: ");
        scanf("%i", &item_number);
        printf("\n");

        switch (item_number) {
            case 1:
                add_book_menu_item();
                break;
            case 2:
                remove_book_menu_item();
                break;
            case 3:
                find_book_menu_item();
                break;
            case 4:
                show_books_menu_item();
                break;
            case 9:
                return EXIT;
            case 0:
                if (top_level_menu) {
                    return SUCCESS;
                }
            default:
                printf("There is no such item in the menu\n\n");
        }
    } while (item_number);
}

void add_book_menu_item() {
    char isbn[80];
    printf("Book ISBN: ");
    scanf("%s", isbn);

    switch (has_book(isbn)) {
        case SUCCESS:
            printf("book already exists\n\n");
            return;
        case OPEN_FILE_ERROR:
            printf("Error: cannot open file books.csv\n\n");
            return;
        case FAIL:
            break;
        default:
            return;
    }

    char authors[80];
    printf("Authors: ");
    scanf("%s", authors);

    char title[80];
    printf("Title: ");
    scanf("%s", title);

    char count[10];
    printf("Books count: ");
    scanf("%s", count);

    char available_count[10];
    printf("Available books count: ");
    scanf("%s", available_count);

    switch (add_book(isbn, authors, title, count, available_count)) {
        case OPEN_FILE_ERROR:
            printf("Error: cannot open file books.csv\n\n");
            break;
        case SUCCESS:
            printf("SUCCESS\n\n");
            break;
        default:
            break;
    }
};

void remove_book_menu_item() {
    char isbn[80];
    printf("Book ISBN: ");
    scanf("%s", isbn);

    switch (remove_book(isbn)) {
        case OPEN_FILE_ERROR:
            printf("Error: cannot open file books.csv\n\n");
            break;
        case FILE_PERMISSION_ERROR:
            printf("Error: cannot edit file books.csv\n\n");
            break;
        case SUCCESS:
            printf("SUCCESS\n\n");
        default:
            break;
    }
}

void find_book_menu_item() {
    char isbn[80];
    printf("Book ISBN: ");
    scanf("%s", isbn);

    printf("\n");

    switch (find_books(isbn)) {
        case OPEN_FILE_ERROR:
            printf("Error: cannot open file books.csv\n\n");
            break;
        case SUCCESS:
            printf("SUCCES\n\n");
            break;
        case FAIL:
            printf("No books with ISBN %s\n\n", isbn);
            break;
        default:
            break;
    }
}

void show_books_menu_item() {

}

int has_book(const char *isdn) {
    char *db_isdn;
    char buff[80];

    FILE *books_file = fopen("books.csv", "r");
    if (!books_file) {
        return OPEN_FILE_ERROR;
    }

    while (!feof(books_file)) {
        fscanf(books_file, "%s\n", buff);

        db_isdn = strtok(buff, ";");

        if (!strcmp(isdn, db_isdn)) {
            fclose(books_file);
            return SUCCESS;
        }
    }
    fclose(books_file);

    return FAIL;
}

int find_books(const char *book_isbn) {
    char buff[80];
    bool succes = false;

    char *isbn;
    char *authors;
    char *title;
    char *count;
    char *available_count;

    FILE *books_file = fopen("books.csv", "r");
    if (!books_file) {
        return OPEN_FILE_ERROR;
    }

    while (!feof(books_file)) {
        fscanf(books_file, "%s\n", buff);
        isbn = strtok(buff, ";");
        authors = strtok(NULL, ";");
        title = strtok(NULL, ";");
        count = strtok(NULL, ";");
        available_count = strtok(NULL, ";");

        if (!strcmp(book_isbn, isbn)) {
            printf("Title: %s\n", title);
            printf("Authors: %s\n", authors);
            printf("Count: %s\n", count);
            printf("Available count: %s\n", available_count);

            fclose(books_file);
            return SUCCESS;
        }
    }

    fclose(books_file);
    return FAIL;
}

int add_book(char *isbn, char *authors, char *title, char *count, char *available_count) {
    FILE *books_file = fopen("books.csv", "a");
    if (!books_file) {
        return OPEN_FILE_ERROR;
    }

    fprintf(books_file, "%s;%s;%s;%s;%s\n", isbn, authors, title, count, available_count);
    fclose(books_file);

    return SUCCESS;
}

int remove_book(const char *isbn) {
    char buff[80];

    FILE *books_file = fopen("books.csv", "r");
    if (!books_file) {
        return OPEN_FILE_ERROR;
    }

    FILE *result_file = fopen(".books.csv", "w");
    if (!result_file) {
        return OPEN_FILE_ERROR;
    }

    while (!feof(books_file)) {
        fscanf(books_file, "%s\n", buff);

        if (strncmp(isbn, buff, strlen(isbn)) != 0) {
            fprintf(result_file, "%s\n", buff);
        }
    }

    fclose(books_file);
    fclose(result_file);

    if (remove("books.csv") != 0) {
        return FILE_PERMISSION_ERROR;
    }

    if (rename(".books.csv", "books.csv") != 0) {
        return FILE_PERMISSION_ERROR;
    }

    return SUCCESS;
}