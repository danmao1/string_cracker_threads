#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <crypt.h>



char var;
char salt[3];
char* target;
int size;
char letter_to_check='a';
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void* cracker(void* args){
    struct crypt_data data;
    data.initialized = 0;
    char password[8];
    
    
    for (int i=0;i<size;i++){
        password[i]='a';
    }
    password[size]='\0';
    while(password[0]!='{'){
        for(int i=1;i<size-1;i++){
            if(password[i]=='{'){
                password[i]='a';
                if(i==1){
                    pthread_mutex_lock(&mutex);
                    letter_to_check++;
                    password[0]=letter_to_check;
                    pthread_mutex_unlock(&mutex);
        

                }
                else{
                    var=password[i-1];
                    var++;
                    password[i-1]=var;
                }
                
            }

        }
        for (char c='a';c<='z';c++){
            password[size-1]=c;
            
            if(strcmp(crypt_r(password,salt,&data),target)==0){
                printf("This is password: %s\n",password);
                exit(0);
                return NULL;
            }

        }
        var=password[size-2];
        var++;
        password[size-2]=var;




    }




}

int main(int argc, char* argv[]){
    
    int num_threads;
    int thread_sep[num_threads];
    num_threads=atoi(argv[1]);
    pthread_t tid[num_threads];

   
    
    size=atoi(argv[2]);
    target=argv[3];

    salt[0]=target[0];
    salt[1]=target[1];
    salt[2]='\0';
   
    
    
    
    printf("this is the target: %s\n",target);
    printf("this is the salt: %s\n",salt);
    while(size!=0){
        for(int i=0;i<num_threads;i++){
            
            int creating_thread=pthread_create(&tid[i],NULL,cracker,NULL);
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
        size--;
    }
    
    return 0;
}