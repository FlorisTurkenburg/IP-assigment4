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

void fetch_article(char *article) {
    struct fetch_article_out *article_content;

    article_num num = atol(article);
    
    article_content = fetch_1(&num, cl);

    if (article_content != NULL) {
        if (article_content->filename != NULL) {
            printf("Content-Disposition: filename=\"%s\"\n\n", article_content->filename);
            
        }
        // printf("Content-Type: application/msword\n\n");
        int i;
        for (i = 0; i < article_content->content.content_len; i++) {
            printf("%c", article_content->content.content_val[i]);
        }
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
    //             fetch_article(value);
                
    //         }

    //     }
    // }
    
    cgivars = getcgivars();
    int i;
    for (i = 0; cgivars[i]; i += 2) {
        if (!strcmp(cgivars[i], "id")) {
            fetch_article(strdup(cgivars[i+1]));
            break;
        }
    }
    
    for (i = 0; cgivars[i]; i++) {
        free(cgivars[i]);
    }
    free(cgivars);



    clnt_destroy(cl);


    return 0;
}