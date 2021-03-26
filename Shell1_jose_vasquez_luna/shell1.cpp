// Nanoshell 1 jose vasquez luna


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
 #include <sys/stat.h>
#include <string.h> 
#include <cstdlib>
#include <fstream>
#include <signal.h>
//Incluimos esta biblioteca para poder compararlas
#include <cctype> //Incluimos esta biblioteca para poder convertir a mayúsculas y minúsculas
#define max_args 13  //Numero maximo de argumentos (-1) cuando se trate de un comando externo
#define MAX 105 //Numero de caracteres maximo para comando las variables de ambiente
using namespace std;

 char comando[MAX]; //Esta var lee el comando que ingrese el usuario
char *args[MAX]; //en este arreglo se almacenan los argumentos del comando ingresado
char SHELL[MAX];//definido para almacenar la ruta absoluta del ejecutable.
char PATH[MAX];// definido para almacenar las rutas de los comandos externos.
char HOME[MAX];//definido para almacenar la ruta al directorio del usuario.
char PWD[MAX];//definido para almacenar  el directorio actual.
int  args_;

void tolower();// convierte la intruccion a letras minusculas para evitar problemas de interpretacion.
int compare();//busca si la inctruccion ingresada existe;
void separaArgs(void); //Esta funcion separa la cadena ingresada por el usuario en 
 	//palabras individuales, la 1ra palabra sera considerada comando y el resto sus argumentos
void echo(void); //Esta func ejecuta el comando echo
void cd(void); //Esta func ejecuta el comando cd
void mkdir(void); //Esta func ejecuta el comando mkdir 
void env(void);// funcion para listar las variables de ambiente
void sistema(void);//funcion para ejecutar llamadas al sistema.



int main(int argc, char *argv[])
{ // INICIO DE LA funcion main main 
 strcpy(PATH,getenv("PATH"));//obtenemos las rutas de los comandos externos
strcpy(HOME,getenv("HOME"));// obtenemos el directorio home y cargando en HOME
  strcpy(SHELL,getenv("SHELL"));// Obteniendo el directorio de shell y cargando en SHELL
 getcwd(PWD,MAX);//obtenemos el directorio actual y lo cargamos en SHELL

system("clear");// limpiamos la terminal
char com[MAX];
while(1) 
	{//
	
	char hostn[1204] = "";
	gethostname(hostn, sizeof(hostn));
	printf("%s@%s %s> ", getenv("LOGNAME"), hostn, PWD);// el indicador del shell muestra el nombre de usuario y el directorio actual
	scanf("\n%[^\n]",comando); // Escaneamos la cadena entera hasta que pulsa intro
	if(strlen(comando)>0){ /*Actuar solo si comando contiene algo y solo un enter*/
      separaArgs(); /*Separar comando de sus argumentos*/

	tolower();//convierte la intruccion a letras minusculas para evitar problemas de interpretacion.
	if(strcmp(comando, "exit")==0)// si el comando ingresado es "exit"
	exit(0);//termina la ejecucion
	compare();//llamamos a la funcion que compara el  comando ingresado con la lista de comandos disponibles.
	}


	}
 }
void tolower(){//convierte la intruccion a letras minusculas para evitar problemas de interpretacion.
	int comparacion=1,i;
	char comando1[MAX] ;//arreglo auxiliar para almacenar el comando en minusculas
	for ( i = 0;  i < strlen(comando) ; i++) {
        comando1[i] = tolower(comando[i]);//la funcion tolower pasa las letras de minuscullas a mayusculas 
    }
    for ( i = 0;  i < strlen(comando) ; i++)
    	 comando[i] = comando1[i];//volvemos a pasar el comando convertido a minusculas a la variable global comando
}
int compare(){//funcion comprueba el comando ingresado en la lista de comandos disponibles
	int resultado=1,i;
 if(strcmp(comando, "pwd")==0){// si comando es "pwd" muestra el directorio actual
    resultado=0;
    printf("%s\n",PWD); /*Mostrar el contenido de PWD (Directorio actual)*/

 }
 if(strcmp(comando, "ls")==0 || strcmp(comando, "dir" ) ==0){// si comando == dir o ls 
    resultado=0;
    sistema();// llamamos a la funcion sistema

 }
 if(strcmp(comando, "lsa")==0){// si comando == lsa 
    resultado=0;
    
	char arg[100];
	strcat( arg,"ls -l");// enviamos la frase ls -l a la variable arg
    std::system(arg); // ejecutamos el comando ls -l enviandole arg a system

 }
 if(strcmp(comando, "mkdir")==0){// si comando == mkdir
    resultado=0;
    mkdir();//llamamos a la funcion mkdir
 }
 if(strcmp(comando, "clear")==0){// si comando == clear
    resultado=0;
    system("clear");// limmpiamos la terminal
 }
 if(strcmp(comando, "echo")==0){// si comando == echo
    resultado=0;
    sistema();// llamamos a la funcion sistema
 }
 if(strcmp(comando, "who")==0){// si comando == who
    resultado=0;
      std::system("who ");// ejecutamos el comando who enviandole a system

 }
 if(strcmp(comando, "printenv")==0){// si comando == printenv
    resultado=0;
    env();// llamamos a la funcion env

 }
 if(strcmp(comando, "cd")==0){// si comando == cd
    resultado=0;
    cd();// llamamos a la funcion cd
 }
	if(resultado!=0){//si resultado no es igual a cero, impriminos el mensaje de error
		printf("Error de sintaxis o conmando no valido\n");
	}
	
}
void separaArgs(void){//separa la cadena ingresada por el usuario en palabras individuales, la 1ra palabra sera considerada comando y el resto sus argumentos
  int i;
  args_=0;
  for(i=0;i<(max_args-1);i++) args[i]=NULL; /*Borrar argumento previo que pudiera existir*/
  strtok(comando," "), i=0; /*separar palabras individuales usando tokens (espacio vacio)*/
  args[i]=comando; //El 1er argumento sera comando 
  while((args[++i]=strtok(NULL," "))!=NULL && i<(max_args-2));
  args_=i;//guardamos el total de argumentos detectados
}

void cd(void)//Esta func ejecuta el comando cd
{
if (args[1] == NULL){//si no se ingresa el directorio al que se quiere ingresar, muesra error 
		fprintf(stderr, "Error, falta el directorio\n");
	}
	else if (chdir(args[1]) < 0){//Si no faltan argumentos,se ingresa al directorio en caso de que exista 
		perror("minsh");//si chdir devuelve error, el,directorio no existe y muestra error.
	}
	getcwd(PWD, sizeof(PWD));	// actualiza la ruta actual

}
void mkdir(void) //Esta func ejecuta el comando mkdir
{
mkdir(args[1], S_IRWXU | S_IRWXG | S_IRWXO);// crea el directorio  ingresado
printf("Directorio creado\n");
}

void sistema(){
int i;
char arg[100];

strcpy (arg,comando);   //le pasamos el comando a la varable arg
if (args[1] != NULL)// si existe mas de in argumento
	for(i=1;i<args_;i++){// juntamos todos los argumentos para enviarseloas a system
	strcat( arg," ");
 	strcat( arg,args[i]);

	}

    std::system(arg); // ejecutamos el comando ingresado.
}
 
void env(void){// funcion para listar las variables de ambiente
	char **env_aux;
	 
			for(env_aux = environ; *env_aux != 0; env_aux ++){//imprimimos las variables de ambiente
				printf("%s\n", *env_aux);
			}
}

strtok(comando," "), i=0; /*separar palabras individuales usando tokens (espacio vacio)*/
  args[i]=comando; //El 1er argumento sera comando 
  while((args[++i]=strtok(NULL," "))!=NULL && i<(max_args-2));
  args_=i;//guardamos el total de argumentos detectados
for(i=0;i<args_;i++){// juntamos todos los argumentos para enviarseloas a system
	strcat( arg," ");
 	strcat( arg,args[i]);}
 
 int i,args_;
 char arg[100],args[100];
  for(i=0;i<(100);i++) args[i]=NULL; /*Borrar argumento previo que pudiera existir*/
  strtok(bienvenida," "), i=0; /*separar palabras individuales usando tokens (espacio vacio)*/
  args[i]=bienvenida; //El 1er argumento sera comando 
  while((args[++i]=strtok(NULL," "))!=NULL && i<(100))
  args_=i;//guardamos el total de argumentos detectados
 for(i=0;i<args_;i++){// juntamos todos los argumentos para enviarseloas a system
	strcat( arg," ");
 	strcat( arg,args[i]);}