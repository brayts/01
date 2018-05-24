//
// Created by views on 23.05.18.
//

#ifndef LABS_BOOKS_H
#define LABS_BOOKS_H

int books_menu(bool top_level_menu);

void add_book_menu_item();

void remove_book_menu_item();

void find_book_menu_item();

void show_books_menu_item();

int has_book(const char *isdn);

int find_books(const char *book_last_name);

int add_book(char *isbn, char *authors, char *title, char *count, char *available_count);

int remove_book(const char *number);

#endif //LABS_BOOKS_H
