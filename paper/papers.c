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


void get_article_list() {
    struct list_article_out *article;
    list_res *result;

    // article = list_1(NULL, cl);
    result = list_1(NULL, cl);
    if (result == NULL) {
        perror("Error while calling server");
        return;
    }

    if (result->err == 0) {
        article = &result->list_res_u.list;
        printf("Content-Type: application/json\n\n");
        printf("[\n");
        while (article) {
            printf("\t{\n");
            printf("\t\t\"id\": %ld,\n", article->num);
            printf("\t\t\"title\": \"%s\",\n", article->title);
            printf("\t\t\"author\": \"%s\"\n", article->author);
            if (article->next) {
                printf("\t},\n");
            }
            else {
                printf("\t}\n");
            }
            // printf("%ld\t%s\t%s\n", article->num, article->author, article->title);
            article = article->next;
        }
        printf("]\n");
    }

    return;

}


int main(int argc, char **argv) {
    
    cl = clnt_create(PAPER_ADDRESS, PAPERSERVER_PROG, PAPERSERVER_VERS, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        exit(1);
    }
    
    get_article_list();
    


    clnt_destroy(cl);


    return 0;
}