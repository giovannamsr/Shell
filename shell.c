#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

char **string_to_array(char *input)
{
    char** result = malloc(100*sizeof(char)); //aloca memória para o resultado
    int i = 0;
	if(result != NULL){ // alocação foi um sucesso

        char* delim = " "; //delimitador usado como parâmetro para separar a string de entrada
        char* ptr = strtok(input, delim);
        while(ptr != NULL)
        {
            result[i] = ptr;
            ptr = strtok(NULL,delim);
            i++;
        }
        result[i] = NULL; //final da array
        return result;
    }
    else{
        perror("Erro! Sem memória!");
        exit(1);
    }
}

int cd(char *path) { //implementação de cd
    return chdir(path); //muda o diretório
}

int main() {
    char **command;
    char *input;
    int info;
    pid_t child;
    while (1) {
        input = readline(">>> "); //recebe o comando do usuário
        command = string_to_array(input);//transforma a string de entrada em uma array de strings
        if (command == NULL) { //caso o usuário não entre com nenhum comando
            free(input);
            free(command);
            continue;
        }
        if(strcmp(command[0], "exit") == 0){ //adicionando o commando exit para a shell
            exit(1); //sai do processo
        }
        if (strcmp(command[0], "cd") == 0) {
            if (cd(command[1]) == -1) { //comando inválido
                perror(command[1]);
            }
            continue;
        }
        child = fork();//cria o processo filho
        if (child == -1) { //o fork falhou
            perror("\nFork falhou\n");
            exit(1);
        }
        if (child == 0) {
        //processo filho
            execvp(command[0], command); //executa o comando do user
            if (execvp(command[0], command) < 0) { //execvp retorna -1 caso a execução falhe
                perror(command[0]);
                exit(1);
            }
            printf("\nErro de execução\n"); //caso o processo filho não seja sobrescrito
        }

         else {
         //processo pai
            waitpid(child, &info, WUNTRACED);//espera o processo filho acordar
        }
        free(input);
        free(command);
    }
    return 0;
}

