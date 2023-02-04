#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int back=0; //variable globale permettant de definir si on est en foreground ou background

/* résultat : renvoie la chaine de caractères entrée par l'utilisateur*/
char* entrerString(){
    char* chaine;
    chaine=malloc(sizeof(char)*4096); //on alloue de la mémoire à chaine
    fgets(chaine,4096,stdin); //on attend l'entrée utilisateur et on la sauvegarde dans chaine
    chaine[strlen(chaine)-1]='\0'; //on remplace le '\n' crée par l'appui sur ENTRER de l'utilisateur par '\0' afin de créer une chaine de caractères
    return chaine;
}

/* entrée : chaine de caractères
   résultat : tableau dont chaque element represente une partition de la chaine donnée en entrée
*/
char** separerChar(char* string){
    char **args;
    args=malloc(sizeof(char*)*100); //on alloue la mémoire à args
    int i=0;
    char* decoupe=strtok(string," "); //on partitionne string avec pour délimiteur " "
    while(decoupe!=NULL){
        args[i]=decoupe; //on place la partition précédente dans args[i]
        i++;
        decoupe=strtok(NULL," "); //on passe à la prochaine partie
    }
    if(args[i-1][0]=='&'){ //on vérifie si le dernier element est &
        back=1; //on modifie la variable globale
        args[i-1]=NULL; //le tableau doit finir par NULL donc on le place en dernier element tout en retirant le &
    }
    else{
        args[i]=NULL; //le tableau doit finir par NULL donc on le place en dernier element
    }
    return args;
}

/* entrée : tableau d'arguments pour la fonction exec
   resultat : execute la commande grace au tableau et attend que les processus fils terminent si on est en mode foreground
*/
void executer(char** args){
    pid_t pid=fork(); //création du processus fils
    if(pid==0){ //si le pid est celui du fils
        execvp(args[0],args); //on execute la commande
        exit(0); //on fait quitter le processus fils
    }
    if(back==0){ //si on est en foreground (seul le processus pere peut acceder à cette ligne)
        wait(NULL); //on attend la fin du processus fils
    }
}

int main(){
    while(1){ //on simule le comportement d'un shell donc on fait une boucle infinie
        printf("\033[1;32m---> ");
        char* chaine=entrerString(); //l'utilisateur entre la commande
        char** args=separerChar(chaine); //on met la commande separée dans un tableau
        executer(args); //on execute la commande
        if(back==1){
            back=0; //on remet en mode foreground si on à deja réalisé la commande en background
        }
    }
}