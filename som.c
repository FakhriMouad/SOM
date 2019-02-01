
#include <sys/types.h>
#include <unistd.h>
pid_t getpid(void);

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

struct data_v{ 
	double *vec;
	double Norme;
	char *nom; 	
};
typedef struct data_v data_v;

struct neurone
{
	double * vecteur;
	char etiquette;
	double activation;
};
typedef struct neurone neurone;


double norme(data_v x)
{
	int i;
	double N=0.0;
	for (i = 0 ; i < 4 ; i++)
	{
		
		N+= x.vec[i] * x.vec[i];
    }
return sqrt(N);
}




size_t nombre_ligne(FILE* input){
	int g=0;
	size_t taille=10000;
	char*tmp=malloc(taille);
	while(getline(&tmp,&taille,input)!=-1)	
		g++;
return g;
}

int random_int(int min, int max)
{
   return min + rand() % (max+1 - min);
}

double rand_tab_float(double min, double max)
{
	return(rand()/(double)RAND_MAX) * (max-min)+min;
}


int* tab_indice_shuffle(size_t nb_ligne)
{
	//creation d'un tableau d'indice
	int *indices = malloc(sizeof(int)*nb_ligne);
	for(int i = 0; i < nb_ligne; i++)
	{
		indices[i] = i;
	}
	int rand_i;
	
	//swap le tableau d'indice
	for(int i = 0; i < nb_ligne; i++)
	{
		rand_i = rand()%nb_ligne;
		int tmp = indices[i];

		indices[i] = indices[rand_i]; 
		indices[rand_i] = tmp;
	}	
	return indices;
}

data_v* extraction_vector(FILE* f, data_v* iris_data){
    char buffer[10000];
    char* mot=NULL;
    int i = 0;
    int y = 0;
    srand(getpid());
  
	iris_data = malloc(sizeof(data_v)* 150);
    while ((fgets(buffer, 10000, f) != NULL)){
        
		iris_data[i].vec = malloc(sizeof(double)*4);
		iris_data[i].nom = malloc(sizeof(char) * 30);            

		while(y < 4)
 		{
			if (y == 0)
				mot = strtok(buffer, ",");
			else
				mot = strtok(NULL, ",");			


			
			iris_data[i].vec[y] = atof(mot);
			//printf("%lf ", iris_data[i].vec[y]);
			y++;
		}
		y=0;
		char* c;

		c = strtok(NULL, "\n");
		iris_data[i].nom = strcpy(iris_data[i].nom, c);

		iris_data[i].Norme = norme(iris_data[i]);
		i++;
		
	}
	return iris_data;
}



void normalisation(data_v* iris_data)
{

	for( int i=0;i<150;i++)
	{
		for(int y=0; y<4 ;y++)
		{
			iris_data[i].vec[y]= iris_data[i].vec[y]/iris_data[i].Norme; 

		}
	}
}


double* moyenne(data_v* iris_data, double* vecteur_moyen)
{

	for(int i=0; i<150 ;i++)
	{
		for(int y=0; y<4; y++)
		{
 
			vecteur_moyen[y] += iris_data[i].vec[y];
			
		}
  	}

  	for(int i=0; i<4; i++)
  	{
  		vecteur_moyen[i] /= 150;

  	}

  	return vecteur_moyen;

}


double distance_euclidienne(double* vec, double* inconnu){	
	double n = 0;
    for(int i = 0; i < 4; i++)
    {
        n += pow(fabs(vec[i]- inconnu[i]), 2);
    }   
	return sqrt(n);
}



neurone** carte(double* vecteur_moyen, int nb_ligne, int nb_colonne)
{
	neurone ** carte_neuronal= NULL;

	/*printf("nb_neuronnes----->	%d\n",nb_neuronnes); //------ 60
	printf("colone----->	%d\n",colone);	//--------6
	printf("ligne----->	%d\n",ligne);	//---------10 */
		
	carte_neuronal = malloc(sizeof(neurone*)*nb_ligne);
	for (int i = 0; i < nb_ligne; i++)
	{
		carte_neuronal[i]= malloc(sizeof(neurone)*nb_colonne); //------- malloc 10
	}

	for (int i = 0; i < nb_ligne;i++)
	{
		for (int j = 0; j < nb_colonne; j++)
		{
			carte_neuronal[i][j].vecteur = malloc(sizeof(double)*4);
			for (int k = 0; k < 4; k++)
			{
				carte_neuronal[i][j].vecteur[k]= vecteur_moyen[k]+rand_tab_float(-0.002,0.005);
				carte_neuronal[i][j].etiquette = '.';

			}
		}
	}
	return carte_neuronal;
}

int* get_bmu(neurone** carte_neuronal, int nb_ligne, int nb_colonne)
{
	int* indice_minimum = malloc(sizeof(int)*2);
	double min_distance = 9999.9;
	for(int i = 0;i < nb_ligne;i++)
	{
		for(int y=0; y<nb_colonne;y++)
		{
			if(carte_neuronal[i][y].activation < min_distance)
			{
				min_distance = carte_neuronal[i][y].activation;
				indice_minimum[0] = i;
				indice_minimum[1] = y;
			}
		}
	}

	return indice_minimum;
}


void print_int_tab(int * tab, size_t size)
{
	
		for(int i = 0; i < size; i++)
		{
			printf("%d ", tab[i]);
			if(i%10 == 0 && i != 0)
				printf("\n");
		}

}

void print_double_tab(data_v* iris_data, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		for(int y = 0; y < 4; y++)
		{
			printf("%lf ", iris_data[i].vec[y]);
		}
		printf(" %s", iris_data[i].nom);
		printf(" norme :%lf", iris_data[i].Norme);
		printf("\n");
	}
	printf("\n\n");
}



void affich_carte_neuronne_avec_activation(neurone** carte_neuronal, int nb_ligne, int nb_colonne)
{
	for(int i = 0;i< nb_ligne; i++)
	{
		for(int y = 0;y< nb_colonne; y++)
		{
			for(int z = 0; z < 4; z++)
			{
				printf("%lf ", carte_neuronal[i][y].vecteur[z]);
			}
			printf("distance= %lf\n", carte_neuronal[i][y].activation);
		}
	}
}


void affich_carte_neuronne_sans_activation(neurone** carte_neuronal, int nb_ligne, int nb_colonne)
{
	for(int i = 0;i< nb_ligne; i++)
	{
		for(int y = 0;y< nb_colonne; y++)
		{
			for(int z = 0; z < 4; z++)
			{
				printf("%lf ", carte_neuronal[i][y].vecteur[z]);
			}
			printf("\n");
		}
	}
}

void best_voisinage(double* iris_vec, char* iris_vec_label, neurone** carte_neuronal, int nb_ligne, int nb_colonne, int rayon, int ligne_bmu, int colonne_bmu, double alpha)
{
	for(int i =0;i<4;i++)
	{
		carte_neuronal[ligne_bmu][colonne_bmu].vecteur[i] += alpha * (iris_vec[i] - carte_neuronal[ligne_bmu][colonne_bmu].vecteur[i]);
		if(!strcmp(iris_vec_label,"Iris-setosa"))
			carte_neuronal[ligne_bmu][colonne_bmu].etiquette = 'a';
		else if(!strcmp(iris_vec_label,"Iris-versicolor"))
			carte_neuronal[ligne_bmu][colonne_bmu].etiquette = 'b';
		else if(!strcmp(iris_vec_label,"Iris-virginica"))
			carte_neuronal[ligne_bmu][colonne_bmu].etiquette = 'c';
	}
	
	int left_limit = rayon;
	int right_limit = rayon;
	int up_limit = rayon;
	int down_limit = rayon;
	while(colonne_bmu-left_limit < 0)
		left_limit--;
	while(colonne_bmu+right_limit > nb_colonne)
		right_limit--;
	while(ligne_bmu-up_limit < 0)
		up_limit--;	
	while(ligne_bmu+down_limit > nb_ligne)
		down_limit--;	

	for(int i = ligne_bmu-up_limit; i < down_limit;i++)
	{
		for(int y = colonne_bmu-left_limit; y < right_limit; y++)
		{
			for(int j = 0; j < 4; j++)
				carte_neuronal[i][y].vecteur[j] += alpha * (iris_vec[j] - carte_neuronal[i][y].vecteur[j]); 
		}
	}
}

void voisinage(double* iris_vec, char* iris_vec_label, neurone** carte_neuronal, int nb_ligne, int nb_colonne, int rayon, int ligne_bmu, int colonne_bmu, double alpha)
{	
	//Apprentissage du BMU
	for(int i =0;i<4;i++)
	{
		carte_neuronal[ligne_bmu][colonne_bmu].vecteur[i] += alpha * (iris_vec[i] - carte_neuronal[ligne_bmu][colonne_bmu].vecteur[i]);
		if(!strcmp(iris_vec_label,"Iris-setosa"))
			carte_neuronal[ligne_bmu][colonne_bmu].etiquette = 'a';
		else if(!strcmp(iris_vec_label,"Iris-versicolor"))
			carte_neuronal[ligne_bmu][colonne_bmu].etiquette = 'b';
		else if(!strcmp(iris_vec_label,"Iris-virginica"))
			carte_neuronal[ligne_bmu][colonne_bmu].etiquette = 'c';
	}
	for(int i = 0; i < rayon; i++)
	{
		//printf("ligne_bmu-i:%d ligne_bmu+i%d colonne_bmu-i:%d colonne_bmu+i:%d\n", ligne_bmu-i, ligne_bmu+i, colonne_bmu-i, colonne_bmu+i);
		for(int y = 0; y < 4; y++)
		{
			if((ligne_bmu+i > 0 && ligne_bmu+i < nb_ligne) && (colonne_bmu+i > 0 && colonne_bmu+i < nb_colonne))
				carte_neuronal[ligne_bmu+i][colonne_bmu+i].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu+i][colonne_bmu+i].vecteur[y]);
			//se

			if((ligne_bmu > 0 && ligne_bmu < nb_ligne) && (colonne_bmu+i > 0 && colonne_bmu+i < nb_colonne))
				carte_neuronal[ligne_bmu][colonne_bmu+i].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu][colonne_bmu+i].vecteur[y]);
			//E

			if((ligne_bmu+i > 0 && ligne_bmu+i < nb_ligne) && (colonne_bmu > 0 && colonne_bmu < nb_colonne))
				carte_neuronal[ligne_bmu+i][colonne_bmu].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu+i][colonne_bmu].vecteur[y]);
			//S

			if((ligne_bmu > 0 && ligne_bmu < nb_ligne) && (colonne_bmu-i > 0 && colonne_bmu-i < nb_colonne))
				carte_neuronal[ligne_bmu][colonne_bmu-i].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu][colonne_bmu-i].vecteur[y]);
			//O	

			if((ligne_bmu-i > 0 && ligne_bmu-i < nb_ligne) && (colonne_bmu > 0 && colonne_bmu < nb_colonne))
				carte_neuronal[ligne_bmu-i][colonne_bmu].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu-i][colonne_bmu].vecteur[y]);
			//N

			if((ligne_bmu-i > 0 && ligne_bmu-i < nb_ligne) && (colonne_bmu-i > 0 && colonne_bmu-i < nb_colonne))
				carte_neuronal[ligne_bmu-i][colonne_bmu-i].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu-i][colonne_bmu-i].vecteur[y]);
			//NO

			if((ligne_bmu+i > 0 && ligne_bmu+i < nb_ligne) && (colonne_bmu-i > 0 && colonne_bmu-i < nb_colonne))
				carte_neuronal[ligne_bmu+i][colonne_bmu-i].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu+i][colonne_bmu-i].vecteur[y]);
			//SO

			if((ligne_bmu-i > 0 && ligne_bmu-i < nb_ligne) && (colonne_bmu+i > 0 && colonne_bmu+i < nb_colonne))
				carte_neuronal[ligne_bmu-i][colonne_bmu+i].vecteur[y] += alpha * (iris_vec[y] - carte_neuronal[ligne_bmu-i][colonne_bmu+i].vecteur[y]);
			//NE
		}
	}
}

void tab_final(neurone** carte_neuronal, int nb_ligne, int nb_colonne)
{
	for(int i = 0;i< nb_ligne;i++)
	{
		for(int y = 0;y<nb_colonne;y++)
		{
			printf("%c ", carte_neuronal[i][y].etiquette);
		}
		printf("\n");
	}
}

int main( ){

	FILE *fp;    
	fp = fopen("iris.txt", "r");
	FILE* fpin = fopen("iris.txt", "r");	
	int nb_line = nombre_ligne(fpin);
	data_v * iris_data = malloc(sizeof(data_v)*nb_line);

	
    
	if (fp == NULL)
		printf("le fichier n'existe pas\n");

	else
	{
		iris_data = extraction_vector(fp, iris_data);
	
	}

 	


 	// Traitement des données
 	normalisation(iris_data);



 	double*vecteur_moyen=NULL;
	vecteur_moyen=malloc(4*sizeof(double));
	vecteur_moyen=moyenne(iris_data, vecteur_moyen);


	int nb_neuronnes = sqrt(150);
	nb_neuronnes = nb_neuronnes * 5;
	
	int nb_colonne = nb_neuronnes/10;
	int nb_ligne = nb_neuronnes/nb_colonne;

	neurone ** carte_neuronal = carte(vecteur_moyen, nb_ligne, nb_colonne);
	
	printf("\n\n");
	
	int nb_ite = 5000*150;
	int* indexes = tab_indice_shuffle(150);
	double alpha = 0.8f;
	int rayon = ceil(sqrt(nb_neuronnes/2)/2);
	int change_alpha = nb_ite/4;

	int nb_ver = 0;
	int nb_vir = 0;
	int nb_set = 0;

	for(int iteration = 0; iteration < nb_ite; iteration++)//
	{	

		int f_ind = iteration % 149; 
		if(f_ind == 0)
		{
			indexes = tab_indice_shuffle(150); 
		}
	
	    if(iteration == change_alpha/3 && rayon > 1)
	    	rayon--;
	    else if(iteration == change_alpha/2 && rayon > 1)
	    	rayon--;
	    else if(iteration == change_alpha)
	    {
	    	rayon = 2;
	    	alpha = 0.08f;
	    }
	   	else
	    	alpha = alpha * (1.0f- ((double) iteration/nb_ite));

		double* rand_iris_data_vec =  NULL;
		char* rand_iris_label = NULL;
		rand_iris_data_vec = malloc(sizeof(double) * 4); 

		for(int i = 0; i < 4; i++)
		{
			rand_iris_data_vec[i] = iris_data[indexes[f_ind]].vec[i];
			
		}

		rand_iris_label = malloc(sizeof(char)* sizeof(iris_data[indexes[f_ind]].nom));
		rand_iris_label = iris_data[indexes[f_ind]].nom;

		if(!strcmp(rand_iris_label,"Iris-setosa"))
			nb_set++;
		else if(!strcmp(rand_iris_label,"Iris-versicolor"))
			nb_ver++;
		else if(!strcmp(rand_iris_label,"Iris-virginica"))
			nb_vir++;	
	
		for(int i = 0;i < nb_ligne; i++)
		{
			for(int y = 0;y < nb_colonne; y++)
			{
				carte_neuronal[i][y].activation = distance_euclidienne(rand_iris_data_vec, carte_neuronal[i][y].vecteur);

			}
		}

		int* indice_best_match = get_bmu(carte_neuronal, nb_ligne, nb_colonne);

//------------------------------------------------------------------------------------------------
		best_voisinage(rand_iris_data_vec,rand_iris_label , carte_neuronal, nb_ligne, nb_colonne, rayon, indice_best_match[0], indice_best_match[1], alpha);
	}
	tab_final(carte_neuronal, nb_ligne, nb_colonne);
	
  	fclose(fp);
}