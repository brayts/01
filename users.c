//
// Created by views on 23.05.18.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "users.h"
#include "errors.h"


int authorizate(char *login, char *password, bool *studetns_permission, bool *books_permission) {
    char buff[80];
    char *db_login;
    char *db_password;
    char *db_s_p;
    char *db_b_p;

    FILE *users_file = fopen("users.csv", "r");
    if (!users_file) {
        return OPEN_FILE_ERROR;
    }

    while (!feof(users_file)) {
        if (!fscanf(users_file, "%s", buff)) {
            continue;
        }

        db_login = strtok(buff, ";");
        db_password = strtok(NULL, ";");
        db_s_p = strtok(NULL, ";");
        db_b_p = strtok(NULL, ";");

        if (!strcmp(login, db_login) && !strcmp(password, db_password)) {
            *studetns_permission = *db_s_p == '1';
            *books_permission = *db_b_p == '1';

            fclose(users_file);
            return SUCCESS;
        }
    }

    fclose(users_file);
    return FAIL;
}