#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h> 
#include <signal.h>

 #include <sys/stat.h>
#include <string.h> 
#include <cstdlib>
#include <fstream>
//Incluimos esta biblioteca para poder compararlas
#include <cctype> //Incluimos esta biblioteca para poder convertir a mayúsculas y minúsculas
#define max_args 13  //Numero maximo de argumentos (-1) cuando se trate de un comando externo
#define maxln_Com_Amb 105 //Numero de caracteres maximo para comando las variables de ambiente
#define MAX 105
#define BUILTIN_COMMANDS 9	// Number of builtin commands defined
using namespace std;

 char comando[MAX]; //Esta var lee el comando que ingrese el usuario
  char comando2[MAX]; //Esta var lee el comando que ingrese el usuario, se usa en
  char auxiliar[MAX];
char *args[MAX]; //en este arreglo se almacenan los argumentos del comando ingresado
char *args2[MAX]; //en este arreglo se almacenan los argumentos del comando ingresado
char SHELL[MAX];//definido para almacenar la ruta absoluta del ejecutable.
char PATH[MAX];// definido para almacenar las rutas de los comandos externos.
char HOME[MAX];//definido para almacenar la ruta al directorio del usuario.
char PWD[MAX];//definido para almacenar  el directorio actual.
int found=1,flag,args_;

 
void tolower();// convierte la intruccion a letras minusculas para evitar problemas de interpretacion.
int compare();//busca si la inctruccion ingresada existe;
void separaArgs(void); //Esta funcion separa la cadena ingresada por el usuario en 
 	//palabras individuales, la 1ra palabra sera considerada comando y el resto sus argumentos
void echo(void); //Esta func ejecuta el comando echo
void cd(void); //Esta func ejecuta el comando cd
void env(void);// funcion para listar las variables de ambiente
void launchProg();
void cat();// funcion que ejecuta comandos de redireccionamiento.
void systema();//funcion para ejecutar llamadas al sistema.

void execute(void);


int main(int argc, char *argv[])
{ /* INICIO DE LA FUNCI�N main */
flag=0;
 strcpy(PATH,getenv("PATH"));//obtenemos las rutas de los comandos externos
strcpy(HOME,getenv("HOME"));// obtenemos el directorio home y cargando en HOME
  strcpy(SHELL,getenv("SHELL"));// Obteniendo el directorio de shell y cargando en SHELL
 getcwd(PWD,MAX);//obtenemos el directorio actual y lo cargamos en SHELL

system("clear");// limpiamos la terminal
int comand=1, salir=1;
char com[MAX]; 
while(1) 
	{//
	
	char hostn[1204] = "";
	gethostname(hostn, sizeof(hostn));//obtenemos el nombre de usuario
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
     launchProg();

 }
 if(strcmp(comando, "cat")==0){
    resultado=0;
     cat();

 }
 if(strcmp(comando, "ls")==0 || strcmp(comando, "dir" ) ==0){// si comando == dir o ls 
    resultado=0;
     launchProg();


 }
 if(strcmp(comando, "lsa")==0){// si comando == lsa 
    resultado=0;
     launchProg();


 }
 if(strcmp(comando, "clear")==0){// si comando == clear
    resultado=0;
    launchProg();
 }
 if(strcmp(comando, "echo")==0){// si comando == echo
    resultado=0; 
    systema();
 }
 if(strcmp(comando, "who")==0){// si comando == who
    resultado=0;
     launchProg();
 }
  if(strcmp(comando, "mkdir")==0){// si comando == mkdir
    resultado=0;
     launchProg();
 }
 if(strcmp(comando, "printenv")==0){// si comando == printenv
    resultado=0;
    launchProg();

 }
 if(strcmp(comando, "cd")==0){
    resultado=0;
      cd();
 }
 //launchProg();
	if(resultado!=0){//si resultado no es igual a cero, impriminos el mensaje de error
		printf("Error de sintaxis o conmando no valido\n");
	}
	else{
		found=0;
	}

}
void systema(){
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

void launchProg(){
int background = 0;	 
	 int err = -1;
	 int pid;
	 if((pid=fork())==-1){
		 return;
	 }
	if(pid==0){
	
		signal(SIGINT, SIG_IGN);
		setenv("parent",getcwd(PWD, 1024),1);	
		
		if (execvp(comando,args)==err){
			printf("comando no encontrado");
			kill(getpid(),SIGTERM);
		}
		else{
			found=1;
		}
	 }
	 if (background == 0){
		 waitpid(pid,NULL,0);
	 }else{
		 printf("Process created with PID: %d\n",pid);
	 }	
	 
  
}
void separaArgs(void){
	memset(args2,'\0',105);
	memset(comando2,'\0',105);
	flag=0;
  int i;
  found=1;
  for(i=0;i<(max_args-1);i++) args[i]=NULL; /*Borrar argumento previo que pudiera existir*/
  for(i=0;i<(max_args-1);i++) args2[i]=NULL; /*Borrar argumento previo que pudiera existir*/
  strtok(comando," "), i=0; /*separar palabras individuales usando tokens (espacio vacio)*/
  args[i]=comando; /*El 1er argumento sera comando por un requerimiento de execvp*/
  while((args[++i]=strtok(NULL," "))!=NULL && i<(max_args-2)){
  	if (strcmp(args[i-1], "|")==0){
  		printf("%s\n",args[i-1] );
  	strcat( comando2,args[i]);
  	flag=1;}
  	if(flag!=0)
  	if((args[++i]=strtok(NULL," "))!=NULL)
  	strcat( args2[i],args[i]);
  	found++;}
  	  args_=i;
}
void tuberias()
{ /* INICIO DE LA FUNCI�N tuberias */
  int fd[2],estado;
	pid_t hijo; 
	hijo=fork();
	

	if (hijo==-1) printf("ERROR Creacion de proceso"); 
	else if (hijo==0) {
		   pipe(&fd[0]); /* Funcion pipe encargada de crear la tuberia */
   			if (fork()!=0) {
      				close(fd[0]); /* cerramos el lado de lectura del pipe */
      				close(1);
      				dup(fd[1]); /* STDOUT = extremo de salida del pipe */
      				close(fd[1]);/* cerramos el descriptor de fichero que sobra 
								tras el dup(fd[1])*/
      				execvp(comando,args);
   				}
   			else {
      				close(fd[1]);/* cerramos el lado de lectura del pipe */
      				close(0);
      				dup(fd[0]);/* STDOUT = extremo de salida del pipe */
      				close(fd[0]);/* cerramos el descriptor de fichero que sobra 
								tras el dup(fd[0])*/
     				execvp(comando2,args2);
      
  				 }
		   }
	else  hijo=wait(&estado);
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

void env(void){// funcion para listar las variables de ambiente
	char **env_aux;
	 
			for(env_aux = environ; *env_aux != 0; env_aux ++){//imprimimos las variables de ambiente
				printf("%s\n", *env_aux);
			}
}


void cat(){
//int mycat(int argc, char *argv[])
	int fd1, fd2;
	int count,i;
	char buffer[100];
	int argc= found;
	if(argc < 2)//si e numero de argumentos encontrados es menos a 2
	{
		printf("Error, se requieren mas argumentos\n");//muestra que hay una falta de argumentos
		return ;
	}
	
	if(argc == 2)
	{
		if(!(access(args[1],F_OK)))//busca si el archivo existe.
		{
			fd1 = open(args[1], O_RDONLY);//siel  archivo existe, lo abre en modo lectura.
			if(fd1 > 0)
			{
				while((count = read(fd1, buffer, sizeof(buffer))) > 0)//le los datos del archivo.
				{
					write(1, buffer, count);//Escribe datos desde el búfer a la salida estándar.
				}
				close(fd1); //cierra el archivo
			}	
			else
			{
				perror("open");//si ocurre un error mientras muestra el error en la pantalla.
			}
		}
		else
		{
			perror("access");//muestra error si el archivo no existe.
		}

		printf("\n\n");
	}
	else if(!strcmp(args[2],">"))// si en los argumentos se encuentra el comando > 
	{
		if(!access(args[1],F_OK)) //busca si existe el primer archivo requerido
		{
			fd1=open(args[1],O_RDONLY); //si el archivo  existe, lo abre en modo lectura
			if(fd1>0)
			{	
				fd2=open(args[3],O_CREAT|O_WRONLY|O_TRUNC,0777); //If files to exists, then open in trunc mode otherwise create the file.
				if(fd2>0)
				{
					while((count=read(fd1,&buffer,sizeof(buffer)))>0) //lle los datos del primer archivo y los escribe en el  segundo archivo
						write(fd2,&buffer,count);

					close(fd2); //cierra el primer archivo
					close(fd1); //cierra el segundo archivo
				}
				else
					perror("open"); //muestra si hay un error abriendo el segundo archivo
			}
			else
				perror("open"); //muestra error abriendo el archivo 1
		}
		else  
			perror("access"); //muestra si ocurrio un error buscando la existencia del primer archivo
	}
	else if(!strcmp(args[2],">>"))//si detecta que se ingreso el comando >> 
	{
		if(!access(args[1],F_OK)) //busca si existe el primer archivo requerido
		{
			fd1=open(args[1],O_RDONLY); //si el archivo  existe, lo abre en modo lectura
			if(fd1>0)
			{	
				fd2=open(args[3],O_CREAT|O_WRONLY|O_APPEND,0777); //If files to exists, then open in append mode otherwise create the file.
				if(fd2>0)
				{
					while((count=read(fd1,&buffer,sizeof(buffer)))>0) //lee los datos del primer archivo y los escribe en el  segundo archivo
						write(fd2,&buffer,count);

					close(fd2); //cierra el primer archivo
					close(fd1); //cierra el segundo archivo
				}
				else
					perror("open"); //muestra error abriendo el segundo archivo
			}
			else
				perror("open"); //muestra error abriendo el archivo 1
		}
		else  
			perror("access"); //muestra si ocurrio un error buscando la existencia del primer archivo
		
	
	}
	else if(!strcmp(args[1],">"))// si el segundo argumento ingresado es >, abre el archivo seleccionado en modo escritura
	{
		printf("Para salir presionar Ctrl+d\n");
		fd1 = open(args[2],O_CREAT|O_WRONLY|O_TRUNC,0777);//si existe el archivo, lo habre en modo escritura, de lo contrario, crea el archivo.
		if(fd1 > 0)
		{
			while((count=read(0,&buffer,sizeof(buffer)))>0) //lee los datos de la entrada y los escribe en el archivo seleccionado
				write(fd1,&buffer,count);

					close(fd1); //cierra el archivo
					
		}	 
	}
	else if(!strcmp(args[1],">>"))//si el segundo argumento ingresado es >>, abre el archivo seleccionado en modo escritura 
	{
		printf("Para salir presionar Ctrl+d\n");
		fd1 = open(args[2],O_CREAT|O_WRONLY|O_APPEND,0777);///si existe el archivo, lo habre en modo escritura, de lo contrario, crea el archivo.
		if(fd1 > 0)
		{
			while((count=read(0,&buffer,sizeof(buffer)))>0) //lee los datos de la intrada y los escribe en el archivo seleccionado
				write(fd1,&buffer,count);

					close(fd1); //cierra el archivo
					
		}
	}
	else
	{
		for(i=1;i<argc;i++) 
		{
			if(!(access(args[i],F_OK)))//busca si el archivo existe
			{
				fd1 = open(args[i], O_RDONLY);//si existe el archivo, lo habre en modo escritura, de lo contrario, crea el archivo.
				if(fd1 > 0)
				{
					while((count = read(fd1, buffer, sizeof(buffer))) > 0)//lee los datos del archivo
					{
						write(1, buffer, count);
					}
					close(fd1); //cierra el archivo
				}	
				else
				{
					perror("open");//muestra error abriendo el archivo 1
				}
			}
			else
			{
				perror("access");//muestra error si e archivo no existe
			}

			printf("\n");
		}

}
	
}
	

