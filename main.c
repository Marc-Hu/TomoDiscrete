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
    int *M;
    int *L;
    int *C;
    int *TT;
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
                ENUMERATION

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
        if(t->M[i*t->nbColonne+j]==2){ //Si la case est colorié alors on est dans une séquence de bloc
            if(estDansBloc==0){ //Si on était pas dans un bloc auparavant
                sequence++; //On incrémente sequence qui nous permet de récupérer la valeur dans la tableau L
                estDansBloc=1; //On est désormais dans un bloc
            }
            temp++;//On incrémente temp pour le tester plus tard
            if(temp>t->L[t->nbLigne*i+sequence])    //Si on constate que temp est supérieur à la valeur du bloc d'index séquence
                return 0;
        } else if (estDansBloc && temp!=t->L[t->nbLigne*j+sequence]){ // si la case est différente de 2 et qu'on était dans un bloc auparavant, on regarde si la valeur est différente
            return 0;
        } else {    //sinon si c'est pas colorié et que tout va bien on réinitialise la val temp et estDansBloc
            temp=0;
            estDansBloc=0;
        }
    }
    if(t->L[t->nbLigne*i]==sequence)
        return 1;
    return 0; //Retourne vrai (1) si tout va bien
}

//Même fonction que celui au-dessus sauf que c'est pour comparer une colonne
int compare_seq_col(tomo *t, int j){
    int i=0, sequence=0, temp=0, estDansBloc=0;
    for(i=0; i<t->nbLigne; i++){
        if(t->M[t->nbColonne*i+j]==2){
            if(estDansBloc==0){
                sequence++;
                estDansBloc=1;
            }
            temp++;
            if(temp>t->C[t->nbColonne*j+sequence])
                return 0;
        }else if(estDansBloc && temp!=t->C[t->nbColonne*j+sequence]){
            return 0;
        }else{
            temp=0;
            estDansBloc=0;
        }
    }
    if(t->C[t->nbColonne*j]==sequence)
        return 1;
    return 0;
}

int enumeration_rec(int k, int c, tomo *t){
    printf("cycle numéro : %d\n", k);
    if(c==1)
        printf("Appel c=1\n");
    else   
        printf("Appel c=2\n");
    int ok, raz, i, j;
    i=k/t->nbColonne;
    j=k%t->nbColonne;
    if(t->M[i*t->nbColonne+j]==0){
        t->M[i*t->nbColonne+j]=c;
        raz=1;
    }else{
        if(t->M[i*t->nbColonne+j]!=c){
            return 0;
        }else {
            raz=0;
        }
    }
    ok=1;
    if(i==t->nbLigne-1){
        ok=compare_seq_col(t, j);
    }
    if(ok && j==t->nbColonne-1)
        ok=compare_seq_ligne(t, i);
    if(ok){
        if(i==t->nbLigne-1 && j==t->nbColonne-1){
            return 1;
        }
        ok=enumeration_rec(k+1, 1, t) || enumeration_rec(k+1, 2, t);
    }
    if(!ok&&raz)
        t->M[i*t->nbColonne+j]=0;
    return ok;
}

/*********************************************

    FIN PARTIE FONCTIONS DE TOMOGRAPHIE
                ENUMERATION

*********************************************/  
/*********************************************

        PARTIE FONCTIONS DE TOMOGRAPHIE
                    VECTEUR

*********************************************/
/*
Fonction testSiAucun qui renvoie 1 si val n'apparaît dans aucune case dans la matrice tomo entre j1 et j2
*/
int testSiAucun(tomo *t, int j1, int j2, int val){
    int i;
    for(i=j1; i<j2; i++){
        if(t->M[i]==val)
            return 0;
    }
    return 1;
}

/*
Fonction qui va tester s'il existe un coloriage possible pour une ligne de la matrice
*/
int testVecteurLigne_Rec(tomo *t, int j, int l){
    printf("j vaut : %d et l vaut : %d\n", j, l);
    int c1, c2, colonne=t->nbColonne, Ll=t->L[t->nbLigne*l+1];
    if(l==0)
        return testSiAucun(t, 0, j, 2);
    if(l==1 && j==Ll-1)
        return testSiAucun(t, 0, j, 1);
    if(j<=Ll-1)
        return 0;
    if(t->TT[j*colonne+l]!=-1)
        return t->TT[j*colonne+l];
    if(t->M[j]==2){
        c1=0;
    }else{
        c1=testVecteurLigne_Rec(t, j-1, l);
    }
    if(!testSiAucun(t, j-(Ll-1), j, 1)){
        c2=0;
    }else{
        if(t->M[j-Ll]==2){
            c2=0;
        }else{
            c2=testVecteurLigne_Rec(t, j-Ll-1, l-1);
        }
    }
    t->TT[j*colonne+l]=c1 || c2;
    return t->TT[j*colonne+l];
}

/*
Fonction qui va tester s'il existe un coloriage possible pour une colonne de la matrice
*/
int testVecteurColonne_Rec(tomo *t, int j, int l){
    int c1, c2, colonne=t->nbColonne, Cl=t->C[t->nbColonne*l];
    if(l==0)
        return testSiAucun(t, 0, j, 2);
    if(l==1 && j==Cl-1)
        return testSiAucun(t, 0, j, 1);
    if(j<=Cl-1)
        return 0;
    if(t->TT[j*colonne+l]!=-1)
        return t->TT[j*colonne+l];
    if(t->M[j]==2){
        c1=0;
    }else{
        c1=testVecteurLigne_Rec(t, j-1, l);
    }
    if(!testSiAucun(t, j-(Cl-1), j, 1)){
        c2=0;
    }else{
        if(t->M[j-Cl]==2){
            c2=0;
        }else{
            c2=testVecteurLigne_Rec(t, j-Cl-1, l-1);
        }
    }
    t->TT[j*colonne+l]=c1 || c2;
    return t->TT[j*colonne+l];
}
/*********************************************

    FIN PARTIE FONCTIONS DE TOMOGRAPHIE
                    VECTEUR

*********************************************/
/*********************************************

        PARTIE FONCTIONS DE TOMOGRAPHIE
                PROPAGATION

*********************************************/
int propagLigne(tomo *t, int *marqueC, int i, int nb){
    int j, cptcolor=0, c1, c2;
    nb=0;
    for(j=0; j<t->nbColonne; i++){
        if(t->M[i*t->nbColonne+j]==0){
            t->M[i*t->nbColonne+j]=1;
            c1=testVecteurLigne_Rec(t, i, t->L[i*t->nbLigne]);
            t->M[i*t->nbColonne+j]=2;
            c2=testVecteurLigne_Rec(t, i, t->L[i*t->nbLigne]);
            t->M[i*t->nbColonne+j]=0;
            if(!c1 && !c2)
                return 0;
            if(c1 && !c2){
                t->M[i*t->nbColonne+j]=1;
                cptcolor=cptcolor+1;
                if(!marqueC[j]){
                    marqueC[j]=1;
                    nb++;
                }
            }
            if(!c1 && c2){
                t->M[i*t->nbColonne+j]=2;
                cptcolor++;
                if(!marqueC[j]){
                    marqueC[j]=1;
                    nb++;
                }
            }
        }
    }
}

int propagColonne(tomo *t, int *marqueL, int i, int nb){
    int j, cptcolor=0, c1, c2;
    nb=0;
    for(j=0; j<t->nbColonne; i++){
        if(t->M[i*t->nbColonne+j]==0){
            t->M[i*t->nbColonne+j]=1;
            c1=testVecteurColonne_Rec(t, i, t->C[i*t->nbColonne]);
            t->M[i*t->nbColonne+j]=2;
            c2=testVecteurColonne_Rec(t, i, t->C[i*t->nbColonne]);
            t->M[i*t->nbColonne+j]=0;
            if(!c1 && !c2)
                return 0;
            if(c1 && !c2){
                t->M[i*t->nbColonne+j]=1;
                if(!marqueL[j]){
                    marqueL[j]=1;
                    nb++;
                }
            }
            if(!c1 && c2){
                t->M[i*t->nbColonne+j]=2;
                cptcolor++;
                if(!marqueL[j]){
                    marqueL[j]=1;
                    nb++;
                }
            }
        }
    }
}

int propagation(tomo *t){
    int *marqueL=malloc(t->nbLigne*sizeof(int)), *marqueC=malloc(t->nbColonne*sizeof(int));
    int nbmL, nbmC, nb=0, i, j, ok=1;
    for(i=0; i<t->nbLigne; i++)
        marqueL[i]=1;
    for(i=0; i<t->nbColonne; i++)
        marqueC[i]=1;
    i=0;
    while(ok && nbmL!=0 || nbmC!=0){
        i=0;
        while(ok && i<t->nbLigne){
            if(marqueL[i]){
                ok=propagLigne(t, &marqueC, j, nb);
                nbmC=nbmC+nb;
                marqueL[i]=0;
                nbmL=nbmL-1;
            }
            i++;
        }
        j=0;
        while(ok && j<t->nbColonne){
            if(marqueC[j]){
                ok=propagColonne(t, &marqueL, j, nb);
                nbmL=nbmL+nb;
                marqueC[j]=0;
                nbmC=nbmC-1;
            }
            j++;
        }
    }
    return ok;
}
/*********************************************

    FIN PARTIE FONCTIONS DE TOMOGRAPHIE
                PROPAGATION

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
int chargerUnFichier(tomo *t, char question){
    FILE* fichier = NULL;
    char filename[10]="", info[6]="", nbColonne[3]="", nbLigne[3]="";
    int i=0, j=0;
    printf("Entrer le nom du fichier à charger parmi ces propositions (écrire le nom entier): \n");
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
        if(question=='e'){
            initSegBloc(t, fichier, question);
        }
        if(question=='v'){
            initSegBloc(t, fichier, question);
            printf("%d\n", testVecteurLigne_Rec(t, t->nbColonne-1, t->L[0]));
        }
    }
    fclose(fichier);
    return 1;
}

/*
Fonction qui va allouer dynamiquement les tableaux à 2 dimensions de la structure tomographie discrète
Va allouer n*m pour M qui est la matrice principal
Va allouer n*m-1 pour L qui est la matrice avec les segments de blocs de la ligne
Va allour (n-1)*m pour C qui est la matrice avec les segments de blocs de la colonne
*/
int alloueTomo(tomo *t){
    int *matrice=malloc((t->nbColonne*t->nbLigne)*sizeof(int));
    int *segColonne=malloc((t->nbColonne*t->nbColonne)*sizeof(int));
    int *segLigne=malloc((t->nbLigne*t->nbLigne)*sizeof(int));
    int i=0;
    t->M=matrice;
    t->L=segLigne;
    t->C=segColonne;
    t->TT=matrice;
    initialiseLigneColonne(t);
    return 1;
}

void initialiseLigneColonne(tomo *t){
    int i=0, nbLigne=t->nbLigne, nbColonne=t->nbColonne;
    for(i=0; i<nbLigne*nbLigne; i++)
        t->L[i]=-1;
    for(i=0; i<nbColonne*nbColonne; i++)
        t->C[i]=-1;
    for(i=0; i<nbLigne*nbColonne; i++){
        t->M[i]=0;
        t->TT[i]==-1;
    }
        
}

/*
Fonction qui va initialiser les deux tableau 2d afin d'y mettre les nombres de bloc par segement
*/
void initSegBloc(tomo *t, FILE* fichier, char question){
    char val[]="";
    int i=0, j=0, k=0, temp=0;
    //Tableau des segements pour les lignes
    for(i=0; i<t->nbLigne; i++){
        if(fgets(val, 4+t->nbColonne*2, fichier)!=NULL){
            printf("%s\n", &val);
            t->L[i*t->nbLigne]=val[0]-48;
            for(j=0; j<4+t->L[i*t->nbLigne]*2; j++){ //Limitation de la chaîne de caractère en fonction de la première valeur
                // printf("%c\t", val[j]);
                if(val[j]>=48 && val[j]<=57){
                    // printf("%d\t", val[j]-48);
                    if(val[j+1]>=48 && val[j+1]<=57){
                        t->L[t->nbLigne*i+k]=(val[j]-48)*10+val[j+1]-48;
                        j++;
                    }else{
                        t->L[t->nbLigne*i+k]=val[j]-48;
                    }
                    k++;
                }
            }
            k=0;
        }
    }
    fgets(val, 10, fichier); // Il y a une ligne vide dans le fichier, on saute juste la ligne vide
    //Tableau des segements pour les colonnes
    for(i=0; i<t->nbColonne; i++){
        if(fgets(val, 4+t->nbLigne*2, fichier)!=NULL){
            //printf("%s\n", &val);
            t->C[i*t->nbColonne]=val[0]-48;
            for(j=0; j<4+t->C[i*t->nbColonne]*2; j++){
                if(val[j]>=48 && val[j]<=57){
                    // printf("%d\t", val[j]-48);
                    if(val[j+1]>=48 && val[j+1]<=57){
                        t->C[t->nbColonne*i+k]=(val[j]-48)*10+val[j+1]-48;
                        j++;
                    }else{
                        t->C[t->nbColonne*i+k]=val[j]-48;
                    }
                    k++;
                }
            }
            k=0;
        }
    }
    affichageMatrice(t, question);
}

void affichageMatrice(tomo *t, char question){
    int i=0, j=0, result;
    char reponse;
    printf("Affichage test de la récupération des valeur d'un fichier\nSegment bloc ligne:");
    for(i=0; i<t->nbLigne*t->nbLigne; i++){
        if(i%t->nbLigne==0)
            printf("\n");
        if(t->L[i]!=-1)
            printf("%d\t", t->L[i]);
    }
    printf("\nAffichage test de la récupération des valeur d'un fichier\nSegment bloc colonne:");
    for(i=0; i<t->nbColonne*t->nbColonne; i++){
        if(i%t->nbColonne==0)
            printf("\n");
        if(t->C[i]!=-1)
            printf("%d\t", t->C[i]);
    }
    printf("\n\nLe fichier a-t-il était bien chargé dans les matrices? [O/n]\n");
    getchar();
    scanf("%c", &reponse);
    if(reponse=="n")
        exit;
    if(question=='e'){
        result = enumeration_rec(0, 1, t) || enumeration_rec(0, 2, t);
        for(i=0; i<t->nbColonne*t->nbLigne; i++){
            if(i%t->nbColonne==0)
                printf("\n");
            printf("%d\t", t->M[i]);
        }
        printf("\n");
    }
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
    p->choix[0]="Enumération";
    p->choix[1]="Cas du vecteur";
    p->choix[2]="Propagation";
	p->choix[3]="Quitter";
}

//Libère la mémoire pour le menu, la flèche, le tableau de chaine de caractère, les chaines de charactère

void libereMemoire(menu *p, tomo *t){
	free(p->menu);
	free(p->fleche);
    free(p->choix);
    free(t->C);
    free(t->L);
    free(t->M);
    free(t->TT);
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
	initialisationMenu(&p, 4);
	affichageMenu(&p);
	while(i!=3){
		i=lanceMenu(&p);
		switch (i){
            case 1 : 
                clear_terminal();
                chargerUnFichier(&t, 'e');
                libereMemoire(&p, &t);
				break;
			case 2 :
                chargerUnFichier(&t, 'v');
                break;
            case 3 :
                chargerUnFichier(&t, 'p');
                break;
            case 4:
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