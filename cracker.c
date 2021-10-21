#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <crypt.h>


char password[8];
char var;
char salt[3];
char* target;
pthread_mutex_t lock;



void* cracker(void* args){
    struct crypt_data data;
    data.initialized = 0;
    int size=(int)args;
    pthread_mutex_lock(&lock);
    for (int i=1;i<size;i++){
        password[i]='a';
    }
    password[size]='\0';
    while(password[0]!='z'){
        for(int i=1;i<size-1;i++){
            if(password[i]=='{'){
                password[i]='a';
                var=password[i-1];
                var++;
                password[i-1]=var;
            }

        }
        for (char c='a';c<='z';c++){
            password[size-1]=c;
            if(strcmp(crypt_r(password,salt,&data),target)==0){
                printf("This is password: %s\n",password);
                return NULL;
            }

        }
        var=password[size-2];
        var++;
        password[size-2]=var;




    }
    pthread_mutex_unlock(&lock);




}

int main(int argc, char* argv[]){
    
    int num_threads;
    int thread_sep[num_threads];
    num_threads=atoi(argv[1]);
    pthread_t tid[num_threads];

    int keySize;
    char first;
    keySize=atoi(argv[2]);
    target=argv[3];

    salt[0]=target[0];
    salt[1]=target[1];
    salt[2]='\0';
    int sep=26/num_threads;
    int sum=0;
    int mutex=pthread_mutex_init(&lock,NULL);
    for(int i=0;i<num_threads;i++){
        thread_sep[i]=sum;
        sum+=sep;

    }
    
    
    
    printf("this is the target: %s\n",target);
    printf("this is the salt: %s\n",salt);
    for(int i=0;i<num_threads;i++){
        first='a';
        for(int j=0;j<thread_sep[i];j++){
            first++;
        }
        password[0]=first;
        int creating_thread=pthread_create(&tid[i],NULL,cracker,(void*)keySize);
        if(creating_thread!=0){
            printf("Could not create thread\n");
        }
    }
    for (int i=0;i<num_threads;i++){
        int creating_join=pthread_join(tid[i],NULL);
        if(creating_join!=0){
            printf("Could not create join\n");
        }

    }
    pthread_mutex_destroy(&lock);
    return 0;
}
