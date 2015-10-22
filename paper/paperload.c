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

void add_article(char *author, char *title, char *filename) {
    struct add_article_in *in;
    article_num *num;

    FILE *fp;
    struct stat file_stat;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Error when opening file");
        return;
    }

    stat(filename, &file_stat);

    in = (struct add_article_in *) malloc(sizeof(struct add_article_in));

    // in->author = strdup(author);
    in->author = author;
    // in->title = strdup(title);
    in->title = title;
    in->filename = filename;

    in->content.content_val = malloc(sizeof(char) * file_stat.st_size);
    in->content.content_len = fread(in->content.content_val, 1, file_stat.st_size, fp);
    if (in->content.content_len == 0) {
        perror("Error while reading");
        fclose(fp);
        exit(1);
    }

    fclose(fp);
    


    num = add_1(in, cl);
    printf("%ld\n", *num);
    free(in->content.content_val);
    free(in);

    return;
}




int main(int argc, char **argv) {
    char **cgivars;
    char *author;
    char *title;
    char *file;

    if (strcmp(getenv("REQUEST_METHOD"), "POST")) {
        exit(1);
    }



    cl = clnt_create(PAPER_ADDRESS, PAPERSERVER_PROG, PAPERSERVER_VERS, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        exit(1);
    }


    
    cgivars = getcgivars();
    int i;
    // for (i = 0; cgivars[i]; i += 2) {
    //     if (!strcmp(cgivars[i], "author")) {
    //         author = strdup(cgivars[i+1]);
    //     }
    //     else if (!strcmp(cgivars[i], "title")) {
    //         title = strdup(cgivars[i+1]);
    //     }
    //     else if (!strcmp(cgivars[i], "file")) {
    //         file = strdup(cgivars[i+1]);
    //     }
    // }
    //add_article(author, title, file);
    // printf("Content-type: text/html\n\n") ;
    
    // /** Finally, print out the complete HTML response page.         **/
    // printf("<html>\n") ;
    // printf("<head><title>CGI Results</title></head>\n") ;
    // printf("<body>\n") ;
    // printf("<h1>Hello, world.</h1>\n") ;
    // printf("Your CGI input variables were:\n") ;
    // printf("<ul>\n") ;
    
    // /** Print the CGI variables sent by the user.  Note the list of **/
    // /**   variables alternates names and values, and ends in NULL.  **/
    // for (i=0; cgivars[i]; i+= 2)
    //     printf("<li>[%s] = [%s]\n", cgivars[i], cgivars[i+1]) ;
        
    // printf("</ul>\n") ;
    // printf("</body>\n") ;
    // printf("</html>\n") ;

    
    for (i = 0; cgivars[i]; i++) {
        free(cgivars[i]);
    }
    free(cgivars);



    clnt_destroy(cl);


    return 0;
}