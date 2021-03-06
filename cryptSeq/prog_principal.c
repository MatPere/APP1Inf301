#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "cryptSeq.h"
#include  <string.h>

/*int connect(char *login, char *reponse) {
  if (! connexion()) {
        fprintf (stderr, "Impossible de se connecter à AppoLab!\n");
        return 1;
    }

    envoyer_recevoir(login, reponse);
    return 0;
}
*/

void convert(char *message, sequence *seq) {
		//convertion chaine caractères en structure séquence def dans crypteSeq.c
		seq->longueur=0;
		for (int i=0; message[i] != 0 ; i++){
				seq->liste[i]=message[i] ;
				seq->longueur++;
				}
}
void reformat(sequence seq, char *reponse){
		seq.liste[seq.longueur] = 0 ;
		strcpy(reponse,seq.liste);
}

void copie(sequence *retour, sequence *seq, int decal){
		char *point = &seq->liste[decal];
		strcpy(retour->liste,point);
		retour->longueur = seq->longueur - decal ;
}

char decalage(char lettreLue, int decal)
{
		// applique le décalage à une lettreLue
    char lettreSuivante;
    decal=(decal*decal<(26-decal)*(26-decal))?decal:26-decal  ;
    
    if ('a' <= lettreLue && lettreLue <= 'z' ) {
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'z'){
            lettreSuivante =  lettreSuivante-26;
        }
        if (lettreSuivante < 'a'){
            lettreSuivante =  lettreSuivante+26;
        }

    }

    else if ('A' <= lettreLue && lettreLue <= 'Z' ) {
        lettreSuivante = lettreLue + decal;
        if (lettreSuivante > 'Z'){
            lettreSuivante = lettreSuivante-26;
        }
        if (lettreSuivante < 'A'){
            lettreSuivante = lettreSuivante+26;
        }

    }

    else lettreSuivante = lettreLue;

    return lettreSuivante;
}
void appliquer_decalage(sequence *message, int decal){
		//interface avec decalage() pour applications aux structures de type séquence 
		for(int i = 0; i<message->longueur;i++){
				message->liste[i] = decalage(message->liste[i], decal);
				}
}
  
int planB(sequence *mess) {
    char reponse[100000];
    sequence message;
    message.longueur = 0 ;
    //int taille;

    // Affiche les échanges avec le serveur
    mode_debug(true);

    // Attend après chaque échange l'appui sur entrée
    attente_automatique(false);

    // Commentez ou passez à 'false' les lignes ci-dessus pour en désactiver 
    // les effets.

    //if (connect("login PEREZ 11713143",reponse)) return 1;
    if (! connexion()) {
        fprintf (stderr, "Impossible de se connecter à AppoLab!\n");
        return 1 ;
    }

    envoyer_recevoir("login PEREZ 11713143", reponse);
    
    envoyer_recevoir("load planB", reponse);
    envoyer_recevoir("help", reponse);
    
    
    convert(reponse, &message) ;
    
		//Calcul du décalage dc à partir de la différence entre C (du "Chère Alice [...]" usuel) et le 1er caractère de la chaine reçue
    int dc='C'-message.liste[0];
    appliquer_decalage(&message,dc);
    reformat(message,reponse);
    envoyer_recevoir(reponse, reponse);
    
    
    sequence has ; 
    convert("hasta la revolucion", &has) ;
    appliquer_decalage(&has,dc);
    envoyer_recevoir("start", reponse);
    reformat(has,reponse);
    envoyer_recevoir(reponse, reponse);
    
    
    convert(reponse, &message) ;
    int dc2='C'-message.liste[18];
    appliquer_decalage(&message,dc2);
        
   
    /* CODE POUR VALIDER L'EXERCICE PLAN_B SEUL
    
    sequence has2;
    convert("hasta la victoria siempre", has2) ;
    appliquer_decalage(&has2,dc2);
    envoyer_recevoir(has2.liste, reponse);
   
    */
		
    reformat(message,reponse);
    envoyer_recevoir(reponse, reponse);
    envoyer_recevoir("\n", reponse);
    copie(mess, &message,18) ;
    return 0 ;
   }
int crypteSeq(sequence *mess){
		//code partie exercice crypteSeq sur appolab

    char reponse[100000];
    sequence message;
    message.longueur = 0 ;
    mode_debug(true);

    
    attente_automatique(false);

    
    if (! connexion()) {
        fprintf (stderr, "Impossible de se connecter à AppoLab!\n");
        return 1;
    }
    
    envoyer_recevoir("login PEREZ 11713143", reponse);
    envoyer_recevoir("load crypteSeq", reponse);
    envoyer_recevoir("start", reponse);
    
    cryptseq(mess);
    mess->longueur+= -1 ;
    copie(&message, mess, 0);
    reformat(message,reponse);
    envoyer_recevoir(reponse, reponse);
    
    convert(reponse, &message) ;
    sequence seqq;
    copie(&seqq, &message, 83);
    decryptseq(&seqq);
    
    reformat(seqq,reponse);
    envoyer_recevoir(reponse, reponse);
    return 0;
}

int main() {
    sequence message;
		
    planB(&message);
     
    crypteSeq(&message) ;
    
    
    
    
    return 0;
}
