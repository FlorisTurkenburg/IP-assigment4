#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "paperserver.h"
#include "config.h"
#include "getcgivars.h"

CLIENT *cl;


void get_article_info(char *article) {
    struct article_info_out art;
    info_res *result;
    article_num num = atol(article);


    result = info_1(&num, cl);

    if (result == NULL) {
        perror("Error while calling server");
        return;
    }

    if (result->err == 0) {
        art = result->info_res_u.info;
        printf("Content-Type: application/json\n\n");
        // printf("[\n");
        printf("{\n");
        printf("\t\"id\": %ld,\n", num);
        printf("\t\"title\": \"%s\",\n", art.title);
        printf("\t\"author\": \"%s\"\n", art.author);
        printf("}\n");
        // printf("]\n");
        // printf("%s\t%s\n", art.author, art.title);
        
    }


    return;
}




int main(int argc, char **argv) {
    // char *request_method;
    // char *qs;
    // char *key;
    // char *value;
    // char *eqpos;

    char **cgivars;

    cl = clnt_create(PAPER_ADDRESS, PAPERSERVER_PROG, PAPERSERVER_VERS, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        exit(1);
    }

    cgivars = getcgivars();
    int i;
    for (i = 0; cgivars[i]; i += 2) {
        if (!strcmp(cgivars[i], "id")) {
            get_article_info(strdup(cgivars[i+1]));
            break;
        }
    }

    // request_method = getenv("REQUEST_METHOD");
    // if (!strcmp(request_method, "GET")) {
    //     qs = getenv("QUERY_STRING");
    //     if (qs != NULL) {
    //         if ((eqpos = strchr(qs, '='))) {
    //             *eqpos = '\0';
    //             value = strdup(eqpos+1);
    //         }
    //         else {
    //             value = "";
    //         }
    //         key = strdup(qs);

    //         if (!strcmp(key, "id")) {
    //             get_article_info(value);
                
    //         }

    //     }
    // }
    
    for (i = 0; cgivars[i]; i++) {
        free(cgivars[i]);
    }
    free(cgivars);


    clnt_destroy(cl);


    return 0;
}