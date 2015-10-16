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
        printf("\n[\n");
        printf("\t{\n");
        printf("\t\t\"id\": %ld,\n", num);
        printf("\t\t\"title\": \"%s\",\n", art.title);
        printf("\t\t\"author\": \"%s\"\n", art.author);
        printf("\t}\n");
        printf("]\n");
        // printf("%s\t%s\n", art.author, art.title);
        
    }


    return;
}




int main(int argc, char **argv) {
    cl = clnt_create(PAPER_ADDRESS, PAPERSERVER_PROG, PAPERSERVER_VERS, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        exit(1);
    }


    get_article_info("0");
    



    clnt_destroy(cl);


    return 0;
}