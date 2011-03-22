

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


/*##################################################################*/
/*DEFINE															*/
/*##################################################################*/
#define NB_POSSIBILITES 9
#define NB_LIGNES 9
#define NB_COLONNES 9


/*#################################################################*/
/*DÉFINITION DE LA STRUC SUDOKU										*/
/*##################################################################*/
/*Définition de Valeurs pour conserver la valeur initiale*/
typedef struct{
	int initiale;		/*Valeur initiale de la case*/
	int finale;			/*Valeur finale de la case*/
}t_valeur;


/*Définition du type case 9 possibilités*/
typedef struct{
	int possibilites[NB_POSSIBILITES];		/*Possibilités de valeur que la case peut prendre*/
	t_valeur valeur;						/*La valeur initiale et finale*/
}t_case;


/*Définition du type*/
typedef struct{
	t_case colonne[9];
}t_ligne;

/*Définition du type*/
typedef struct{
	t_ligne ligne[9];
}t_sudoku;

/*Déclaration d'un type handle pour prendre un l'adresse d'un sudoku*/
typedef t_sudoku * t_handle;


/*##################################################################*/
/*DÉCLARATIONS DE FONCTIONS											*/
/*##################################################################*/
t_sudoku * init_sudoku(void);
void print_sudoku(t_handle sudoku, const char condition[10]);
void solve_sudoku(t_handle sudoku);
void verif_croise(t_handle sudoku);
void analyse_prob(t_handle sudoku);
void demande_valeur(t_handle sudoku);
void connues_finale(t_handle sudoku);
void valeurs_test(t_handle sudoku1);
int verif_complet(t_handle sudoku);

int main (void){

	int i = 0; int j = 0;

	/*
	Diagramme d'un sudoku selon ma structure :
	=========================================================
	| 0.0 | 0.1 | 0.1 || 1.0 | 1.1 | 1.2 || 2.0 | 2.1 | 2.2 |
	|-----------------||-----------------||-----------------|
	| 0.3 | 0.4 | 0.5 || 1.3 | 1.4  |1.5 || 2.3 | 2.4 | 2.5 |
	|-----------------||-----------------||-----------------|
	| 0.6 | 0.7 | 0.8 || 1.6 | 1.7 | 1.8 || 2.6 | 2.7 | 2.8 |
	|=======================================================|
	| 3.0 | 3.1 | 3.1 || 4.0 | 4.1 | 4.2 || 5.0 | 5.1 | 5.2 |
	|-----------------||-----------------||-----------------|
	| 3.3 | 3.4 | 3.5 || 4.3 | 4.4  |4.5 || 5.3 | 5.4 | 5.5 |
	|-----------------||-----------------||-----------------|
	| 3.6 | 3.7 | 3.8 || 4.6 | 4.7 | 4.8 || 5.6 | 5.7 | 5.8 |
	|=======================================================|
	| 6.0 | 6.1 | 6.1 || 7.0 | 7.1 | 7.2 || 8.0 | 8.1 | 8.2 |
	|-----------------||-----------------||-----------------|
	| 6.3 | 6.4 | 6.5 || 7.3 | 7.4 | 7.5 || 8.3 | 8.4 | 8.5 |
	|-----------------||-----------------||-----------------|
	| 6.6 | 6.7 | 6.8 || 7.6 | 7.7 | 7.8 || 8.6 | 8.7 | 8.8 |
	|=======================================================|
	*/

	/*Création d'un handle pour la struct du sudoku*/
	t_handle sudoku;

	/*Création d'un sudoku*/
	sudoku = init_sudoku();

	/*Impression du sudoku*/
	printf("\nUn sudoku est initialise et vide :\n");
	print_sudoku(sudoku, "init");
	printf("\n\n");
	system("PAUSE");
	system("CLS");

	/*Attitration des valeur connues au sudoku*/
	//demande_valeur(sudoku);
	
	/*Fonction definissant un sudoku à titre de test*/
	valeurs_test(sudoku);
	system("CLS");

	/*Impression du sudoku précédemment entré*/
	printf("\nVoici donc le sudoku a resoudre :\n\n");
	print_sudoku(sudoku, "init");
	printf("\n\n");
	system("PAUSE");

	/*Transfert des valeur connues initiales au finales*/
	connues_finale(sudoku);

	/*Résolution des possibilités en vérifiant les impossibilités
	à répétitions*/
	solve_sudoku(sudoku);
	system("CLS");

	/*Impression du sudoku final*/
	printf("\n\nVoici donc le sudoku resolu :\n\n");
	print_sudoku(sudoku, "final");
	printf("\n\n");

	/*Belle finale*/
	system("PAUSE");
	return EXIT_SUCCESS;
}


t_sudoku * init_sudoku(void){
	t_handle sudoku;

	int i = 0; int j = 0; int k = 0;	

	sudoku = (t_sudoku*) malloc(sizeof(t_sudoku));

	for(i = 0; i < NB_LIGNES; ++i)
		for(j = 0; j < NB_COLONNES; ++j){
			/*Initialiser les valeurs initiales et finales à 0*/
			sudoku->ligne[i].colonne[j].valeur.initiale = 0;
			sudoku->ligne[i].colonne[j].valeur.finale = 0;

			/*Mettre toutes les possibilités à 1 pour les éliminer lors de la
			découverte d'impossibilités*/
			for(k = 0; k < NB_POSSIBILITES; ++k)
				sudoku->ligne[i].colonne[j].possibilites[k] = 1;
		}
	return sudoku;
}

void print_sudoku(t_handle sudoku, const char condition[6]){
	int i = 0; int j = 0;

	/*Pour toutes les lignes on imprime un séparateur + 1 pour la fin*/
	for(i = 0; i <= NB_LIGNES; ++i){
		printf("\n=======================================\n");

		/*Si on est encore dans le sudoku*/
		if(i != NB_LIGNES){

			/*Pour toutes les colonnes on imprime un séparateur*/
			for(j = 0; j < NB_COLONNES; ++j){
				printf("| ");

				/*Si on demande le sudoku initial*/
				if(condition == "init"){

					/*Si un multiple de 3 on doit ajouter un séparateur à la fin*/
					if((j+1)%3 == 0)
						printf("%i |",sudoku->ligne[i].colonne[j].valeur.initiale);

					else
						printf("%i ",sudoku->ligne[i].colonne[j].valeur.initiale);
				}

				/*Si on demande le sudoku final*/
				else{
					/*Si un multiple de 3 on doit ajouter un séparateur à la fin*/
					if((j+1)%3 == 0)
						printf("%i |",sudoku->ligne[i].colonne[j].valeur.finale);

					else
						printf("%i ",sudoku->ligne[i].colonne[j].valeur.finale);
				}
			}
		}
	}
}

void solve_sudoku(t_handle sudoku){
	int complet = 1;

	while(complet){
		verif_croise(sudoku);
		analyse_prob(sudoku);

		if(verif_complet(sudoku))
			complet = 0;
	}
}

void verif_croise(t_handle sudoku){
	int i = 0; int j = 0; int k = 0; int l = 0; int m = 0;

	/*Pour chaque case du sudoku*/
	for(i = 0; i < NB_LIGNES; ++i) for(j = 0; j < NB_COLONNES; ++j)

		/*Pour chaque possibilités*/
		for(k = 1; k <= NB_POSSIBILITES; ++k){

			/*Tester toutes les cases de la même ligne*/
			if(sudoku->ligne[i].colonne[j].possibilites[k] != 0){
				for(l = 0; l < NB_COLONNES; ++l)
					if(sudoku->ligne[i].colonne[l].valeur.initiale == k){
						sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						l = NB_COLONNES;
					}


				/*Tester toutes les cases de la même colonne*/
				if(sudoku->ligne[i].colonne[j].possibilites[k] != 0){
					for(l = 0; l < NB_LIGNES; ++l)
						if(sudoku->ligne[l].colonne[j].valeur.initiale == k){
							sudoku->ligne[i].colonne[j].possibilites[k] = 0;
							l = NB_LIGNES;
						}
				}

				/*Tester toutes les cases du même bloc 3x3*/
				if(sudoku->ligne[i].colonne[j].possibilites[k] != 0){
					if(i < 3){
						if(j < 3){
							if(sudoku->ligne[0].colonne[0].valeur.initiale == k || sudoku->ligne[0].colonne[1].valeur.initiale == k || sudoku->ligne[0].colonne[2].valeur.initiale == k || sudoku->ligne[1].colonne[0].valeur.initiale == k || sudoku->ligne[1].colonne[1].valeur.initiale == k || sudoku->ligne[1].colonne[2].valeur.initiale == k || sudoku->ligne[2].colonne[0].valeur.initiale == k || sudoku->ligne[2].colonne[1].valeur.initiale == k || sudoku->ligne[2].colonne[2].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}

						else if(j < 6){
							if(sudoku->ligne[0].colonne[3].valeur.initiale == k || sudoku->ligne[0].colonne[4].valeur.initiale == k || sudoku->ligne[0].colonne[5].valeur.initiale == k || sudoku->ligne[1].colonne[3].valeur.initiale == k || sudoku->ligne[1].colonne[4].valeur.initiale == k || sudoku->ligne[1].colonne[5].valeur.initiale == k || sudoku->ligne[2].colonne[3].valeur.initiale == k || sudoku->ligne[2].colonne[4].valeur.initiale == k || sudoku->ligne[2].colonne[5].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}

						else{
							if(sudoku->ligne[0].colonne[6].valeur.initiale == k || sudoku->ligne[0].colonne[7].valeur.initiale == k || sudoku->ligne[0].colonne[8].valeur.initiale == k || sudoku->ligne[1].colonne[6].valeur.initiale == k || sudoku->ligne[1].colonne[7].valeur.initiale == k || sudoku->ligne[1].colonne[8].valeur.initiale == k || sudoku->ligne[2].colonne[6].valeur.initiale == k || sudoku->ligne[2].colonne[7].valeur.initiale == k || sudoku->ligne[2].colonne[8].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}
					}

					else if(i < 6){
						if(j < 3){
							if(sudoku->ligne[3].colonne[0].valeur.initiale == k || sudoku->ligne[3].colonne[1].valeur.initiale == k || sudoku->ligne[3].colonne[2].valeur.initiale == k || sudoku->ligne[4].colonne[0].valeur.initiale == k || sudoku->ligne[4].colonne[1].valeur.initiale == k || sudoku->ligne[4].colonne[2].valeur.initiale == k || sudoku->ligne[5].colonne[0].valeur.initiale == k || sudoku->ligne[5].colonne[1].valeur.initiale == k || sudoku->ligne[5].colonne[2].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}

						else if(j < 6){
							if(sudoku->ligne[3].colonne[3].valeur.initiale == k || sudoku->ligne[3].colonne[4].valeur.initiale == k || sudoku->ligne[3].colonne[5].valeur.initiale == k || sudoku->ligne[4].colonne[3].valeur.initiale == k || sudoku->ligne[4].colonne[4].valeur.initiale == k || sudoku->ligne[4].colonne[5].valeur.initiale == k || sudoku->ligne[5].colonne[3].valeur.initiale == k || sudoku->ligne[5].colonne[4].valeur.initiale == k || sudoku->ligne[5].colonne[5].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}

						else{
							if(sudoku->ligne[3].colonne[6].valeur.initiale == k || sudoku->ligne[3].colonne[7].valeur.initiale == k || sudoku->ligne[3].colonne[8].valeur.initiale == k || sudoku->ligne[4].colonne[6].valeur.initiale == k || sudoku->ligne[4].colonne[7].valeur.initiale == k || sudoku->ligne[4].colonne[8].valeur.initiale == k || sudoku->ligne[5].colonne[6].valeur.initiale == k || sudoku->ligne[5].colonne[7].valeur.initiale == k || sudoku->ligne[5].colonne[8].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}
					}

					else{
						if(j < 3){
							if(sudoku->ligne[6].colonne[0].valeur.initiale == k || sudoku->ligne[6].colonne[1].valeur.initiale == k || sudoku->ligne[6].colonne[2].valeur.initiale == k || sudoku->ligne[7].colonne[0].valeur.initiale == k || sudoku->ligne[7].colonne[1].valeur.initiale == k || sudoku->ligne[7].colonne[2].valeur.initiale == k || sudoku->ligne[8].colonne[0].valeur.initiale == k || sudoku->ligne[8].colonne[1].valeur.initiale == k || sudoku->ligne[8].colonne[2].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}

						else if(j < 6){
							if(sudoku->ligne[6].colonne[3].valeur.initiale == k || sudoku->ligne[6].colonne[4].valeur.initiale == k || sudoku->ligne[6].colonne[5].valeur.initiale == k || sudoku->ligne[7].colonne[3].valeur.initiale == k || sudoku->ligne[7].colonne[4].valeur.initiale == k || sudoku->ligne[7].colonne[5].valeur.initiale == k || sudoku->ligne[8].colonne[3].valeur.initiale == k || sudoku->ligne[8].colonne[4].valeur.initiale == k || sudoku->ligne[8].colonne[5].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}

						else{
							if(sudoku->ligne[6].colonne[6].valeur.initiale == k || sudoku->ligne[6].colonne[7].valeur.initiale == k || sudoku->ligne[6].colonne[8].valeur.initiale == k || sudoku->ligne[7].colonne[6].valeur.initiale == k || sudoku->ligne[7].colonne[7].valeur.initiale == k || sudoku->ligne[7].colonne[8].valeur.initiale == k || sudoku->ligne[8].colonne[6].valeur.initiale == k || sudoku->ligne[8].colonne[7].valeur.initiale == k || sudoku->ligne[8].colonne[8].valeur.initiale == k)
								sudoku->ligne[i].colonne[j].possibilites[k] = 0;
						}
					}
				}
			}
		}				
}

void analyse_prob(t_handle sudoku){
	int i = 0; int j = 0; int k = 0;
	int somme;
	
	/*Pour chaque case du sudoku*/
	for(i = 0; i < NB_LIGNES; ++i) for(j = 0; j < NB_COLONNES; ++j){
			/*Réinitialisation de la somme*/
			somme = 0;

			/*Pour toutes les possibilitées de la case*/
			for(k = 0; k < NB_POSSIBILITES; ++k)
				somme += sudoku->ligne[i].colonne[j].possibilites[k];

			/*Si une seule possibilité restante*/ 
			if(somme == 1){
				/*Trouver la possibilité restante et l'associé*/
				for(k = 0; k < NB_POSSIBILITES; ++k)
					if(sudoku->ligne[i].colonne[j].possibilites[k] == 1)
						sudoku->ligne[i].colonne[j].valeur.finale = (k+1);
			}
	}
}

void demande_valeur(t_handle sudoku){
	int i = 0;

	printf("\n\nEntrez les valeurs de chacunes de lignes une par une en format\n");
	printf("1 2 3 4 5 6 7 8 9   et entrer des 0 pour les valeurs inconnues\n\n");
	for(i = 0; i < NB_LIGNES; ++i){
		scanf("%i %i %i %i %i %i %i %i %i", &sudoku->ligne[i].colonne[0].valeur.initiale, &sudoku->ligne[i].colonne[1].valeur.initiale, &sudoku->ligne[i].colonne[2].valeur.initiale, &sudoku->ligne[i].colonne[3].valeur.initiale, &sudoku->ligne[i].colonne[4].valeur.initiale, &sudoku->ligne[i].colonne[5].valeur.initiale, &sudoku->ligne[i].colonne[6].valeur.initiale, &sudoku->ligne[i].colonne[7].valeur.initiale, &sudoku->ligne[i].colonne[8].valeur.initiale);
	}
}

int verif_complet(t_handle sudoku){
	int somme = 0;
	int i = 0; int j = 0;

	/*Pour toutes les cases*/
	for(i = 0; i < NB_LIGNES; ++i) for(j = 0; j < NB_COLONNES; ++j){
		/*Si la valeur finale a été trouvée*/
		if(sudoku->ligne[i].colonne[j].valeur.finale >= 0)
			++somme;
	}

	/*Si toutes les valeurs finales on été trouvées*/
	if(somme >= (NB_LIGNES * NB_COLONNES))
		return 1;

	else
		return 0;
}

void connues_finale(t_handle sudoku){
	int i = 0; int j = 0; int k = 0;

	/*Pour toutes la cases du sudoku*/
	for(i = 0; i < NB_LIGNES; ++i) for(j = 0; j < NB_COLONNES; ++j)

		/*Si la valeur initiale est différente de NULL*/
		if(sudoku->ligne[i].colonne[j].valeur.initiale != 0){

			/*Reset de toutes les possibilités de la case et attitration de
			sa valeur finale*/
			for(k = 0; k < NB_POSSIBILITES; ++k)
				sudoku->ligne[i].colonne[j].possibilites[k] = 0;
			sudoku->ligne[i].colonne[j].valeur.finale = sudoku->ligne[i].colonne[j].valeur.initiale;
		}
}

void valeurs_test(t_handle sudoku1){
	int sudoku2[9][9] = {{6,0,3,0,9,0,0,0,0},{0,7,2,0,0,0,0,0,1},{0,0,9,6,1,7,0,3,0},{7,0,0,4,2,8,0,5,0},{3,0,0,1,0,9,0,0,8},{0,8,0,7,6,3,0,9},{0,3,0,5,7,6,2,0,0},{4,0,0,0,0,0,3,7,0},{0,0,0,0,4,0,1,0,6}};
	
	for(int i = 0; i < NB_LIGNES; ++i) for(int j = 0; j < NB_COLONNES; ++j)
		sudoku1->ligne[i].colonne[j].valeur.initiale = sudoku2[i][j];
}