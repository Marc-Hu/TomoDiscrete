/**
 * \file Tomographie
 * \author HU Marc
 * \date 27 nomvembre 2017
 *
 * Programme regroupant les fonctions du mini-projet tomographie discrète
 */

#include <stdio.h>

typedef struct{
    int **M;
    int **L;
    int **C;
    int nbLigne;
    int nbColonne;
} tomo;

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

int main(){
    return 0;
}