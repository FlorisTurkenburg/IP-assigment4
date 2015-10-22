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
#include <ccgi.h>

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
    CGI_varlist *varlist;
    char *value;

    if ((varlist = CGI_get_all(0)) == 0) {
        fputs("Content-type: text/plain\r\n\r\n", stdout);
        printf("No CGI data received\r\n");
        return 0;
    }
    
    value = CGI_lookup(varlist, "id");


    cl = clnt_create(PAPER_ADDRESS, PAPERSERVER_PROG, PAPERSERVER_VERS, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        CGI_free_varlist(varlist);
        exit(1);
    }

    if (value != NULL) {
        fetch_article(value);
    }
    else {
        printf("Content-Type: text/plain\n\n");
        printf("id value missing\n\n");
    }

    CGI_free_varlist(varlist);  /* free variable list */


    clnt_destroy(cl);


    return 0;
}