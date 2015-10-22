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


void get_article_info(const char *article) {
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
    CGI_varlist *varlist;
    const char *value;

    if ((varlist = CGI_get_all(0)) == 0) {
        fputs("Content-type: text/plain\r\n\r\n", stdout);
        printf("No CGI data received\r\n");
        return 0;
    }

    value = CGI_lookup(varlist, "id");


    cl = clnt_create(PAPER_ADDRESS, PAPERSERVER_PROG, PAPERSERVER_VERS, "tcp");
    if (cl == NULL) {
        fputs("Content-type: text/plain\r\n\r\n", stdout);
        perror("Error creating RPC client!");
        CGI_free_varlist(varlist);  /* free variable list */
        exit(1);
    }

    if (value != NULL) {
        get_article_info(value);
    }
    else {
        printf("Content-Type: text/plain\n\n");
        printf("id value missing\n\n");
    }

    CGI_free_varlist(varlist);  /* free variable list */


    clnt_destroy(cl);


    return 0;
}