/**
 * \file Tomographie
 * \author HU Marc
 * \date 27 novembre 2017
 *
 * Programme regroupant les fonctions du mini-projet tomographie discrète
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"saisieM.c"
#include"couleurs_terminal.c"

/*********************************************

		PARTIE STRUCTURE DE DONNEE

*********************************************/

typedef struct{
    int **M;
    int **L;
    int **C;
    int nbLigne;
    int nbColonne;
} tomo;

typedef struct{
	char *fleche;
	int *menu;
	int nbChoix;
	char **choix;
} menu;

/*********************************************

		FIN PARTIE STRUCTURE DE DONNEE

*********************************************/

/*********************************************

		PARTIE FONCTIONS DE TOMOGRAPHIE

*********************************************/

/*
Fonction compare_seq_ligne qui va prendre en argument la tomographie et le numéro de la ligne à comparer
Variables : j pour chaque case de la ligne i; sequence pour le numéro de séquence initialisé à 0 mais 
en réalité commencera à un puisque la case 0 de la séquence est réservé au nombre de séquence dans la ligne;
temp pour compter le nombre de bloc dans une séquence; estDansBloc pour pouvoir incrémenter la variable
séquence lorsqu'on rentre dans une séquence de x blocs.
*/
int compare_seq_ligne(tomo *t, int i){
    int j=0, sequence=0, temp=0, estDansBloc=0;
    for(j=0; j<t->nbColonne; j++){
        if(t->M[j][i]!=0){ //Si la case est colorié alors on est dans une séquence de bloc
            if(!estDansBloc){
                estDansBloc=1;
                sequence++;
            }
            temp++;         //On incrémente le compteur
            if(temp>t->L[i][sequence]) //On regarde si c'est pas supérieur au nb de bloc dans la séquence
                return 0;
        } else {    //sinon si c'est pas colorié
            if(estDansBloc && temp!=t->L[i][sequence]) //si on était dans une séquence on regarde si le compteur est égale au nb bloc dans la séquence
                return 0;   //Retourne faux
            estDansBloc=0;  //si c'est toujours juste alors on est plus dans un bloc
        }
    }
    return 1; //Retourne vrai (1) si tout va bien
}

//Même fonction que celui au-dessus sauf que c'est pour comparer une colonne
int compare_seq_col(tomo *t, int j){
    int i=0, sequence=0, temp=0, estDansBloc=0;
    for(i=0; i<t->nbColonne; i++){
        if(t->M[j][i]!=0){
            if(!estDansBloc){
                estDansBloc=1;
                sequence++;
            }
            temp++; 
            if(temp>t->C[i][sequence])
                return 0;
        } else { 
            if(estDansBloc && temp!=t->C[i][sequence])
                return 0; 
            estDansBloc=0;
        }
    }
    return 1;
}

int enumeration_rec(int k, int c, tomo *t){
    int ok, raz, i, j;
    i=k/t->nbColonne;
    j=k%t->nbColonne;
    if(t->M[i][j]==0){
        t->M[i][j]=c;
        raz=1;
    }else{
        if(t->M[i][j]!=c){
            return 0;
        }else {
            raz=0;
        }
    }
    ok=1;
    if(i==t->nbLigne-1){
        ok=compare_seq_col(t, j);
    }
    if(ok){
        if(i==t->nbLigne-1 && j==t->nbLigne-1){
            return 1;
        }
        enumeration_rec(k+1, c, t);
    }
    if(!ok&&raz)
        t->M[i][j]=0;
    return ok;
}

/*********************************************

	FIN PARTIE FONCTIONS DE TOMOGRAPHIE

*********************************************/
/*********************************************

        PARTIE CHARGEMENT D'UN FICHIER
                
*********************************************/

/*
Fonction qui charge un fichier
Demande à l'utilisateur de selectionner un fichier qui se trouve dans instances
Si il n'existe pas alors on reviens au menu principal
Sinon on récupère le nombre de ligne et le nombre de colonne
Et pour finir on va lancer la fonction d'allocation dynamique de la mémoire
*/
int chargerUnFichier(tomo *t){
    FILE* fichier = NULL;
    char filename[10]="", info[6]="", nbColonne[3]="", nbLigne[3]="";
    int i=0, j=0;
    printf("Entrer le nom du fichier à charger parmi ces propositions (uniquement le chiffre): \n");
    system("ls instances/*.tom");
    scanf("%s", &filename);
    fichier = fopen(&filename, "r");
    if(fichier==NULL){
        printf("Impossible d'atteindre le fichier! Retour au menu principal\n");	//Si le fichier n'est pas accessible alors on sort de la fonction
        sleep(3);
        return 0;
    }
    printf("Fichier ouvert avec succès!\n");
    fgets (info, 6, fichier);
    for(i=0; i<6; i++){				//On va mettre dans les différents chaînes de caractères les valeurs.
    	if(info[i]!=' ' && i<2)		//Si on ne rencontre pas d'expace et qu'on a déjà lu 2 caractère on met les caractères dans la chaine de caractère nbCol
    		nbLigne[i]=info[i];
    	else if (info[i]==' '){		//Sinon si on rencontre le caractère vide (cad l'espace) on incrémente i pour passer au nombre de ligne et on met le première caractère dans nbLign (on oublie pas d'incrémenter j)
    		i++;
    		nbColonne[j]=info[i];
    		j++;
    	}
    	else{
    		nbColonne[j]=info[i];		//Sinon si on a passé toutes les conditions on va mettre les caractères dans nbLign
    		j++;
    	}
    }
    printf("Nombre de lignes : %d; Nombre de colonnes : %d\n", atoi(nbLigne), atoi(nbColonne));
    t->nbLigne=atoi(nbLigne);
    t->nbColonne=atoi(nbColonne);
    if(alloueTomo(t)){
        if(initSegBloc(t, fichier)){
            //fclose(fichier);
            return 1;
        }
    }
        
}

/*
Fonction qui va allouer dynamiquement les tableaux à 2 dimensions de la structure tomographie discrète
Va allouer n*m pour M qui est la matrice principal
Va allouer n*m-1 pour L qui est la matrice avec les segments de blocs de la ligne
Va allour (n-1)*m pour C qui est la matrice avec les segments de blocs de la colonne
*/
int alloueTomo(tomo *t){
    int *colonne=malloc(t->nbColonne*sizeof(int));
    int *ligne=malloc(t->nbLigne*sizeof(int));
    int *segColonne=malloc((t->nbColonne-1)*sizeof(int));
    int *segLigne=malloc((t->nbLigne-1)*sizeof(int));
    int i=0;
    t->M=ligne;
    for(i=0; i<t->nbLigne;i++)
        t->M[i]=colonne;
    t->L=ligne;
    for(i=0; i<t->nbLigne; i++)
        t->L[i]=segLigne;
    t->C=colonne;
    for(i=0; i<t->nbColonne; i++)
        t->C[i]=segColonne;
    return 1;
}

/*
Fonction qui va initialiser les deux tableau 2d afin d'y mettre les nombres de bloc par segement
*/
int initSegBloc(tomo *t, FILE* fichier){
    char val[10]="";
    int i=0, j=0, k=0, temp=0;
    //Tableau des segements pour les lignes
    for(i=0; i<t->nbLigne; i++){
        if(fgets(val, 15, fichier)!=NULL){
            //printf("%s\n", &val);
            for(j=0; j<15; j++){
                if(val[j]-48>=0){
                    t->L[i][k]=val[j]-48;
                    k++;
                }
            }
            while(k<t->nbLigne){
                t->L[i][k]=-1;
                k++;
            }
            k=0;
        }
        //printf("\n%d\n\n", i);
    }
    // fgets(val, 10, fichier); // Il y a une ligne vide dans le fichier, on saute juste la ligne vide
    // //Tableau des segements pour les colonnes
    // for(i=0; i<t->nbColonne; i++){
    //     printf("test1\n");
    //     if(fgets(val, 15, fichier)!=NULL){
    //         for(j=0; j<15; j++){
    //             if(val[j]-48>=0){
    //                 t->C[i][k]=val[j]-48;
    //                 k++;
    //             }
    //         }
    //         while(k<t->nbColonne){
    //             t->C[i][k]=-1;
    //             k++;
    //         }
    //         k=0;
    //     }
    // }
    affichageTest(t);
    return 1;
}

void affichageTest(tomo *t){
    int i=0, j=0;
    for(i=0; i<t->nbLigne; i++){
        for(j=0; t->L[i][j]!=-1 && j<t->nbLigne;j++)
            printf("test%d\t", t->L[i][j]);
        j=0;
        printf("\n");
    }
    sleep(3);
}
/*********************************************

    FIN PARTIE CHARGEMENT D'UN FICHIER
                
*********************************************/

/*********************************************

                PARTIE MENU
                
*********************************************/


/*
* Fonction qui va initialiser un menu 
* Ici on va initialiser un menu avec un nombre de choix possible définit en paramètre d'entré
* On va allouer dynamiquement les différents tableaux (tableau de la flèche, tableau des du numéro des choix et tableau de chaine de caractère)
* On remarque que pour le tableau de chaîne de caractère le sizeof est de type char * car ce sont des chaîne de caractère et non un char normal
* Ensuite on va allouer chaque case ou plutôt ligne du tableau de chaîne de caractère. Si tu veux un tableau de chaine de caractère 
* contient plusieurs lignes et chaque ligne contient une chaine de caractère et c'est indéxé comme pour un tableau, la première ligne
* c'est la chaîne de caractère n°1, la deuxième ligne c'est la chaîne de caractère n°2 etc etc...
* On va aussi initialiser le tableau menu avec 1, 2, 3, 4 etc jusqu'au nombre maximum de choix possible
* Ensuite on va mettre la flèche à la position 0 du menu c'est à dire on va mettre une flèche sur p->menu = 1 donc jouer.
* Et pour finir on va mettre les chaîne de caractère (une par une) sur chaque ligne du tableau de chaîne de caractère (p->choix)
*/

void initialisationMenu(menu *p, int nb){
	p->nbChoix=nb;
	p->menu=malloc((p->nbChoix)*sizeof(int)); 
	p->fleche=malloc((p->nbChoix)*sizeof(char));
	p->choix=malloc((p->nbChoix)*sizeof(char *));
	int i;
	for (i=0; i<(p->nbChoix); i++)
		p->choix[i]=malloc(15*sizeof(char));
	for (i=0; i<(p->nbChoix); i++)
		p->menu[i]=i+1;
	p->fleche[0]='>';
    p->choix[0]="Chargement d'un fichier";
    p->choix[1]="A compléter";
	p->choix[2]="Quitter";
}

//Libère la mémoire pour le menu, la flèche, le tableau de chaine de caractère, les chaines de charactère

void libereMemoire(menu *p, tomo *t){
	free(p->menu);
	free(p->fleche);
    free(p->choix);
    free(t->C);
}

/*
* On va vérifier ici que la flèche se trouve oui ou non à la ligne a
* Donc si la flèche est sur jouer et qu'on cherche à savoir si flecheValide(&p, 0) (0 pour ligne 1) sa va retourner true car 
* la flèche se situer bien sur la ligne 0 donc sur jouer
* sinon sa retourne 0;
*/

int flecheValide(menu *p, int a){
	if (p->fleche[a]=='>')
		return 1;
	return 0;
}

/*
* Fonction qui va afficher le menu
* \33[34;01m    \33[00m  sont les couleurs pour la sélection
* Il va afficher en blanc si la flèche n'est pas devant la sélection 
* Il va afficher en bleu si la flèche pointe la sélection
* Il y a un ptit aide au menu sur les touches à effectuer : HAUT BAS et DROITE
* Il va donc afficher chaque ligne du menu et si c'est la ligne ou la flèche est dessus alors on l'affiche avec de la surbrillance (bleu ici si je me trompe pas)
* sinon on l'affiche normalement, on va afficher le numéro du choix puis le nom du choix (jouer, sauvegarder etc...)
*/

void affichageMenu(menu *p){
	int i; 
	printf("/**********************************\n        Tomographie discrète\n                by Marc Hu \n                and Raphael Zekri\n**********************************/\n\n");
	for (i=0; i<(p->nbChoix); i++){
		if (flecheValide(p, i))
			printf(" %c \33[34;01m%d  %s\33[00m\n", p->fleche[i], p->menu[i], p->choix[i]);
		else
			printf("     %d  %s\n", p->menu[i], p->choix[i]);
	}
	printf("\n\nMonter/descendre : flèche haut/bas; selectionner : flèche droite\n");
}

// C'est exactement la même fonction que dans le jeu

int saisieD(){
    debutTerminalSansR();
    Key saisieDir = NO_KEY;
    while (saisieDir==NO_KEY){
        saisieDir=lectureFleche();
    }
    switch (saisieDir){ 
        case KEY_DOWN: 
            finTerminalSansR();
            return 0;           
            break;
        case KEY_RIGHT: 
            finTerminalSansR();
            return 1;
            break;
        case KEY_UP: 
            finTerminalSansR();
            return 2;
            break;
        case KEY_LEFT: 
            finTerminalSansR();
            return 3;
            break;
        case KEY_ESCAPE: 
            finTerminalSansR();
            return -1;
            break;
        default :
            finTerminalSansR();
            return -2;
            break;
    }
}

/*
* Fonction qui va retourné la position de la flèche dans flèche
* On va donc retourné l'indice ou se trouve la flèche 
* par exemple si la flèche est sur jouer alors sa retourne 0 sinon si la flèche est sur sauvegarder il retourne 1 etc etc... 
*/

int getFleche(menu *p){
	int i, j=0;
	for (i=0; i<(p->nbChoix); i++){
		if (p->fleche[i]=='>')
			return j;
		j++;
	}
	return 0;
}

/*
* Fonction qui va lancer le menu 
* Si on appuie sur flèche du bas ou du haut alors la flèche dans p->flèche bouge selon la direction
* Si la flèche atteint la limite (haut ou bas) alors la flèche ne bouge plus
* On retourne le menu selectionné (1, 2, 3 ou 4)
* Ici on execute l'affichage du menu, on demande à l'utilisateur de saisir une touche
* tant que c'est pas la flèche DROITE on continue le clear et l'affichage
* Si c'est 0 (flèche du BAS) alors on cherche ou se situe la flèche avec getFleche()
* Ensuite on descend la flèche d'une ligne (on va ajouter un > sur la ligne en dessous et " " sur la ligne ou il y avait la flèche)
* Maintenant si la flèche est déjà tout en bas c'est à dire sur QUITTER alors on fait rien
* C'est la même chose pour le cas de 2 c'est à dire si on fait flèche du HAUT on enlève la flèche sur la ligne ou il y avait la flèche
* et on le mettra sur la ligne juste au dessus et on vérifie bien si on est pas déjà tout en haut sinon on va dépasser et on va perdre la flèche
* Et donc dernier cas si on fait flèche DROITE on va sélectionner un vhoix dans le menu et sa retourne la valeur du choix sélectionné (1 pour jouer etc...)
*/

int lanceMenu (menu *p){
	int saisie, j;
	while (saisie!=1){
		clear_terminal();
		affichageMenu(p);
		saisie=saisieD();
		switch (saisie){
			case 0 : 
				j=getFleche(p);
				if (j!=(p->nbChoix-1)){
					p->fleche[j+1]='>';
					p->fleche[j]=' ';
				}
				break;
			case 2 :
				j=getFleche(p);
				if (j!=0){
					p->fleche[j-1]='>';
					p->fleche[j]=' ';
				}
				break;
			case 1 :
				return getFleche(p)+1;
		}
	}
	return 0;
}

/*
* Fonction menu qui va gérer le menu
* Initialise un menu
* Il va afficher le menu
* Et il va retourné le choix opté par l'utilisateur
* On libère la mémoire
*/

int menuD(){
    menu p;
    tomo t;
	int i;
	initialisationMenu(&p, 3);
	affichageMenu(&p);
	while(i!=3){
		i=lanceMenu(&p);
		switch (i){
            case 1 : 
                chargerUnFichier(&t);
				break;
			case 2 :
				libereMemoire(&p, &t);
                break;
            case 3 :
                libereMemoire(&p, &t);
				break;
		} 
	}
	return 0;
}


/*********************************************
				FIN MENU
*********************************************/

int main(){
    menuD();
    return 0;
}