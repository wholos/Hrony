#include<stdlib.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<libconfig.h>
#include"colorlib.h"
const char *repos;
int inst(int argc,char *argv[]){
    for(int i = 2; i < argc; i++){
        char instll[256];
        char chmd[256];
        printf(GREEN"Installing package: %s\n"reset, argv[i]);
        snprintf(instll,sizeof(instll),"wget -q --directory-prefix=/bin/ %s%s",repos,argv[i]);
        snprintf(chmd,sizeof(chmd),"chmod +x /bin/%s",argv[i]);
        system(instll);
        system(chmd);
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
    config_read_file(&conf,"/etc/hrony.conf");
    config_lookup_string(&conf,"repository",&repos);

    //arg
    if(strcmp(argv[1], "install")==0){
        inst(argc,argv);
    }
    if(strcmp(argv[1], "remove")==0){
        rmv(argc,argv);
    }
}