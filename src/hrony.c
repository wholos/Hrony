#include<stdlib.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<libconfig.h>
const char *repos;
int inst(int argc,char *argv[]){
    for(int i = 2; i < argc; i++){
        char instll[256];
        snprintf(instll,sizeof(instll),"wget %s%s",repos,argv[i]);
        system(instll);
    }
}
int rmv(int argc,char *argv[]){
    for(int i = 2; i < argc; i++){
        char rmve[256];
        snprintf(rmve,sizeof(rmve),"rm /bin/%s",argv[i]);
        system(rmve);
    }
}
int main(int argc,char *argv[]){
    //conf
    config_t conf;
    config_init(&conf);
    config_read_file(&conf,"hrony.conf");
    config_lookup_string(&conf,"repository",&repos);

    //arg
    if(strcmp(argv[1], "install")==0){
        inst(argc,argv);
    }
    if(strcmp(argv[1], "remove")==0){
        rmv(argc,argv);
    }
}
