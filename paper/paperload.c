#include <unistd.h>
#include <stdio.h>
#include <errno.h>
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

void add_article(char *author, char *title, char *tmp_filename, char *filename) {
    struct add_article_in *in;
    article_num *num;
    CGI_varlist *redirect_vars;
    char *reason;
    // char *error_location;
    // char *success_location;
    // sprintf(error_location, "%s/papererror.php", WEB_BASEPHP);
    // sprintf(success_location, "%s/paperinfo.php", WEB_BASEPHP);


    FILE *fp;
    struct stat file_stat;

    fp = fopen(tmp_filename, "rb");
    if (fp == NULL) {
        // sprintf(reason, "Error when opening file %s", strerror(errno));
        redirect_vars = CGI_add_var(NULL, "reason", "Error when opening file");
        printf("Location: %s%s%s\n\n", WEB_BASEPHP, "/papererror.php?", CGI_encode_varlist(redirect_vars, NULL));
        CGI_free_varlist(redirect_vars);

        // perror("Error when opening file");
        return;
    }

    stat(tmp_filename, &file_stat);

    in = (struct add_article_in *) malloc(sizeof(struct add_article_in));

    // in->author = strdup(author);
    in->author = author;
    // in->title = strdup(title);
    in->title = title;
    in->filename = filename;

    in->content.content_val = malloc(sizeof(char) * file_stat.st_size);
    in->content.content_len = fread(in->content.content_val, 1, file_stat.st_size, fp);
    if (in->content.content_len == 0) {
        // sprintf(reason, "Error while reading file %s", strerror(errno));
        redirect_vars = CGI_add_var(NULL, "reason", "Error while reading file");
        printf("Location: %s%s%s\n\n", WEB_BASEPHP, "/papererror.php?", CGI_encode_varlist(redirect_vars, NULL));
        CGI_free_varlist(redirect_vars);
        // perror("Error while reading");
        fclose(fp);
        exit(1);
    }

    fclose(fp);
    


    num = add_1(in, cl);
    redirect_vars = CGI_add_var(NULL, "id", num);
    redirect_vars = CGI_add_var(redirect_vars, "new", "success");
    printf("Location: %s%s%s\n\n", WEB_BASEPHP, "/paperinfo.php?", CGI_encode_varlist(redirect_vars, NULL));
    CGI_free_varlist(redirect_vars);
    // printf("%ld\n", *num);
    free(in->content.content_val);
    free(in);

    return;
}




int main(int argc, char **argv) {
    char *author;
    char *title;

    CGI_varlist *varlist;
    CGI_varlist *redirect_vars;
    CGI_value *value;
    char *reason;

    cl = clnt_create(PAPER_ADDRESS, PAPERSERVER_PROG, PAPERSERVER_VERS, "tcp");
    if (cl == NULL) {
        // sprintf(reason, "Error creating RPC client %s", strerror(errno));
        redirect_vars = CGI_add_var(NULL, "reason", "Error creating RPC client");
        printf("Location: %s%s%s\n\n", WEB_BASEPHP, "/papererror.php?", CGI_encode_varlist(redirect_vars, NULL));

        // perror("Error creating RPC client!");
        CGI_free_varlist(redirect_vars);
        exit(1);
    }


    // fputs("Content-type: text/plain\r\n\r\n", stdout);
    varlist = CGI_get_all("/tmp/cgi-upload-XXXXXX");
    author = CGI_lookup(varlist, "author");
    title = CGI_lookup(varlist, "title");
    value = CGI_lookup_all(varlist, "fileToUpload");
    if (value == 0 || value[1] == 0) {
        redirect_vars = CGI_add_var(NULL, "reason", "No file was uploaded");
        printf("Location: %s%s%s\n\n", WEB_BASEPHP, "/papererror.php?", CGI_encode_varlist(redirect_vars, NULL));
        CGI_free_varlist(redirect_vars);
        // fputs("No file was uploaded\r\n", stdout);
    }
    else {
        // printf("Author: %s\n Title: %s\n", author, title);
        // printf("Your file \"%s\" was uploaded to my file \"%s\"\r\n",
        //     value[1], value[0]);

        add_article(author, title, strdup(value[0]), strdup(value[1]));
        /* Do something with the file here */

        unlink(value[0]);
    }
    CGI_free_varlist(varlist);


    clnt_destroy(cl);


    return 0;
}