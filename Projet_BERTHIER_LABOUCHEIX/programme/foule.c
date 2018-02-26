/**************************************************************************************/
/* Dans le programme, nous avons décidé de choisir un déplacement semi-aléatoire.     */
/* Tant que le personnage ne rencontre pas un mur ou un autre personnage, il va       */
/* directement vers son objectif. Sinon, il choisit au hasard un déplacement.         */
/* Nous avons mis un valeur nommée énervement qui multiplie son nombre de déplacement */
/* au hasard en fonction du nombre de fois qu'il a rencontré un mur ou un personnage  */
/**************************************************************************************/
#include "../lib/libgraphique.h"
#include "../lib/lib_foule.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define UNITE 10    //côté d'une case en pixels
#define NB_CASES_X 80  // nb de cases en abscisse (et dans le tableau 'plateau')
#define NB_CASES_Y 60  // nb de cases en ordonnée (et dans le tableau 'plateau')
#define FICHIER_PLAN "../data/plan.txt"
#define FICHIER_JOUEUR "../data/joueurs"
#define IMAGE_DEPART "../data/image_depart_projet.bmp"
#define IMAGE_SORTIE "../data/image_arret_projet.bmp"
/******************************************************************************/
/* Déclaration des structures et variables globales                           */
/******************************************************************************/

//contient des 1 et des 0 suivant qu'il y ait un mur ou non
int plateau[NB_CASES_X][NB_CASES_Y]={{0}};
 

typedef struct Personnage
{
 int x;//position du départ
 int y;

 int but_x;//position d'arrivée
 int but_y;
 
 int compteur_de_deplacement;
 int couleur;
 int enervement;//lorsqu'il est bloque l'enervement augmente les tentatives de second chemin

}
Personnage;

Personnage peuple[50];

/******************************************************************************/
/* Déclaration de vos fonctions                                               */
/******************************************************************************/
void presentation();
int lancer_le_jeu();
void dessin_du_plateau();
void charger_couleur(int tab_couleur[NB_CASES_X][NB_CASES_Y]);
Personnage creation_du_personnage(int i);
Personnage dessiner_les_points(int i);
void dessiner_l_arrivee(Personnage);
void dessiner_le_personnage(Personnage);
void deplacement_du_personnage();
Personnage second_tentative(int a,Personnage);
Personnage deplacement_horizontal_gauche(Personnage);
Personnage deplacement_horizontal_droit(Personnage);
Personnage deplacement_vertical_haut(Personnage);
Personnage deplacement_vertical_bas(Personnage);
Personnage deplacement_diagonale_haut_droit(Personnage);
Personnage deplacement_diagonale_haut_gauche(Personnage);
Personnage deplacement_diagonale_bas_droit(Personnage);
Personnage deplacement_diagonale_bas_gauche(Personnage);
void effacer_carre(Personnage);
void afficher_score(int);
void charge_peuple(char *fjoueur,Personnage peuple[50]);
void nettoyer_plateau();
void mise_a_jour_des_positions(Personnage vieu_bob , Personnage bob);
/******************************************************************************/
/* main                                                                       */
/******************************************************************************/
int main(void)
{

    ouvrir_fenetre(NB_CASES_X*UNITE,NB_CASES_Y*UNITE);

    //a vous !
    
    presentation();
    
    while(1) //On lance le jeu dans une boucle infinie pour pouvoir quitter le jeu quand on clic sur l'image de sortie
    {
    	if(lancer_le_jeu() ==  0)
          return 0;
        
       charge_plan(FICHIER_PLAN, plateau);

       charge_peuple(FICHIER_JOUEUR,peuple);

    	//preparation du plateaude jeu
    	dessin_du_plateau();
    	actualiser();

    	//deplacement du perssonage

    	deplacement_du_personnage();
    	nettoyer_plateau();

    	actualiser();
    	
    }
    // fin du programme
    attendre_clic();
    fermer_fenetre();

}

/******************************************************************************/
/*  Définitions des autres fonctions                                          */
/******************************************************************************/

void presentation()
{
   char texte1[] = "Projet de  Antoine BERTHIER et Sophie LABOUCHEIX", texte2[] = " S1 informatique groupe C";
   char texte3[] = "SIMULATEUR DE FOULE" ;
   Point p1 = {130,150};
   Point p2 = {230,200};
   Point p3 = {130,350};
   afficher_texte(texte1,20,p1,blanc);
   afficher_texte(texte2,20,p2,blanc);
   afficher_texte(texte3,40,p3,rouge);
   actualiser();
   attendre_clic();
   nettoyer_plateau();
}
   

/*Construit les murs*/
void dessin_du_plateau()
{
    int i,j;
    Point p = {0,0};
    for(i=0;i<NB_CASES_X;i++)
    {
       for(j=0;j<NB_CASES_Y;j++)
       {
          if(plateau[i][j]==1)
          {
             dessiner_rectangle(p,10,10,blanc);
          }
        p.y += 10;
       }
    p.y = 0;
    p.x += 10;
    }
    
    /*création d'un mur invisible pour que les personnages ne puissent pas sortir de l'écran*/
    for(i=0;i<NB_CASES_X;i+=(NB_CASES_X-1))
    {
    	for(j=0;j<NB_CASES_Y;j++)
    	{
        	plateau[i][j] = 1;
    	}
    }
    
    for(j=0;j<NB_CASES_Y;j+=(NB_CASES_Y-1))
    {
    	for(i=0;i<NB_CASES_X;i++)
    	{
        	plateau[i][j] = 1;
    	}
    }
    
    
}

/*cette fonction crée le personnage*/
Personnage creation_du_personnage(int i)
{
  Personnage Bob = peuple[i];//création du personnage en prenant sa structure dans le tableau crée grâce au fichier personnage, vu dans une fonction en fin de programme
  return Bob;
}

/*contient les fonctions de fabrication du carré qui représente le personnage et celui qui représente l'arrivée*/ 
Personnage dessiner_les_points(int i)//dessine le carré personnage.
{
  Personnage bob = creation_du_personnage(i);
  plateau[bob.x][bob.y] = 1; //remplit la case du tableau
  return bob;
}



void dessiner_le_personnage(Personnage bob)
{      
  Point p_perso = {bob.x*UNITE,bob.y*UNITE}; //crée le point ou sera construit le personnage.(je multiplie par 10 pour le convertir en pixel ex un point [2][5] et a x = 20px et y = 50px
  dessiner_rectangle(p_perso,UNITE,UNITE,bob.couleur);//crée le carre personnage.
  dessiner_l_arrivee(bob);
}

//fonction qui crée le rectangle d'arrivée
void dessiner_l_arrivee(Personnage bob)
{
  Point p_arrive = {bob.but_x*UNITE,bob.but_y*UNITE};
  dessiner_rectangle(p_arrive,UNITE,UNITE,vert);
     
}


//deplacement du personnage
void deplacement_du_personnage()
{
  int  nombre_deplacement = 0,i,j;
  j = 49; //on met en place un compteur qui détermine le nombre de personnages
  while(j > 0) // Tant qu'il y a des personnages qui ne sont pas arrivés, on effetue la boucle suivante
  {

    for(i=1;i<=49 ;i++)
    {
      Personnage bob = dessiner_les_points(i); // Mettre en place les personnages
     
      Personnage vieu_bob = bob ;//permet de gardée sa position sa la mis a jour
      
       /* On compare les coordonnées pour être sur que le peronnage n'est pas déjà arrivé */
      if((bob.x != bob.but_x) || (bob.y != bob.but_y)) 
      {
      
      /*On effectue un repérage afin de trouver le déplacement le plus rapide */
      
          if ((bob.x<bob.but_x) && (bob.y<bob.but_y))
          {
            if(plateau[bob.x+1][bob.y+1] == 0) // On vérifie que le déplacement est possible
            {
              bob = deplacement_diagonale_bas_droit(bob); // On déplace le personnage
              
            }
            else // Si le personnage est confronté à un mur, on emploit une méthode aléatoire
            {
              bob = second_tentative(rand()%8,bob);
              
            }
          }
          else if ((bob.x < bob.but_x) && (bob.y == bob.but_y))
          {
            if(plateau[bob.x+1][bob.y] == 0)
            {
              bob = deplacement_horizontal_droit(bob);
              
            }
            else
            {
              bob = second_tentative(rand()%8,bob);
              
            }      
          }
          else if ((bob.x == bob.but_x) && (bob.y < bob.but_y))
          {
            if(plateau[bob.x][bob.y+1] == 0)
            {
              bob = deplacement_vertical_bas(bob);          
              
            }
            else
            {
              bob = second_tentative(rand()%8,bob);
             
            }
          }
          else if ((bob.x > bob.but_x)&&(bob.y < bob.but_y))
          {
            if(plateau[bob.x-1][bob.y+1] == 0)
            {
              bob = deplacement_diagonale_bas_gauche(bob);          
              
            }
            else
            {
              bob = second_tentative(rand()%8,bob);
              actualiser();
            }
          }
          else if ((bob.x > bob.but_x) && (bob.y == bob.but_y))
          {
            if(plateau[bob.x-1][bob.y] == 0)
            {
              bob = deplacement_horizontal_gauche(bob);
             
            }
            else
            {
              bob = second_tentative(rand()%8,bob);
              
            }        
          }
          else if ((bob.x == bob.but_x) && (bob.y > bob.but_y))
          {
            if(plateau[bob.x][bob.y-1] == 0)
            {
              bob = deplacement_vertical_haut(bob);
            }
            else
            {
              bob = second_tentative(rand()%8,bob);
              
            }
          }
          else if ((bob.x < bob.but_x )&&(bob.y > bob.but_y))
          {
            if(plateau[bob.x+1][bob.y-1] == 0)
            {
              bob = deplacement_diagonale_haut_droit(bob);
              
            }
            else
            {
              bob = second_tentative(rand()%8,bob);
             
              
            }
          }
          else if ((bob.x > bob.but_x)&&(bob.y > bob.but_y))
          {
            if(plateau[bob.x-1][bob.y-1] == 0)
            {
              bob = deplacement_diagonale_haut_gauche(bob);
              
            }
            else
            {
              bob = second_tentative(rand()%8,bob);
              
              
            }
          }
        
          if((bob.x == bob.but_x)&&(bob.y == bob.but_y)) //Si le personnage est arrivé, on efffectue l'operation suivante
          {
            j -= 1; //On enlève 1 au compteur car il y a un personnage en moins
            
          }
        peuple[i] = bob; //On rentre les nouvelles coordonnées du personnage qui s'est déplacé
        nombre_deplacement += bob.compteur_de_deplacement;
        bob.compteur_de_deplacement = 1;
        mise_a_jour_des_positions(vieu_bob,bob); 
        actualiser();
      }
      else
        plateau[bob.x][bob.y]=0; //Pour éviter tout blocage de l'arrivée, blocage qui serait causé par des personnages déjà arrivés
    }
  }  
  afficher_score(nombre_deplacement); //Afficher le nombre total de déplacement
  actualiser();
  attendre_clic();
}

/*si le carré est bloqué contre un mur alors cette fonction appelle
au hazard un déplacement pour trouver une porte de secours*/
Personnage second_tentative(int a,Personnage bob)
{
  int i;
  bob.compteur_de_deplacement = 0;
  for(i=0;i<=bob.enervement;i++) //En fonction de l'énervement du personnage, énervement qui augmente lorsqu'il rencontre un mur ou un autre personnage, on  effectue la boucle suivante un nombre de fois donné
  {
    Personnage vieu_bob = bob;
    if(a == 0) // Si le torage aléatoire donne la valeur 0, on effectue la condition qui suit
    {
      if(plateau[bob.x][bob.y-1] == 0) //On vérifie que le déplacement est possible
      {
        bob = deplacement_vertical_haut(bob);
      }
    }
    else if (a == 1)
    {
      if(plateau[bob.x + 1][bob.y] == 0)
      {
        bob = deplacement_horizontal_droit(bob);

      }
    }
    else if (a==7)
    {
      if (plateau[bob.x-1][bob.y]==0)
      {
        bob = deplacement_horizontal_gauche(bob);
      }
    
    }
    else if (a==2)
    {
      if(plateau[bob.x][bob.y+1] == 0)
      {
        bob = deplacement_vertical_bas(bob);
      }
    }
    else if (a==3)
    {
      if(plateau[bob.x-1][bob.y-1] == 0)
      {
        bob = deplacement_diagonale_haut_gauche(bob);
      }

    }
    else if (a == 4)
    {
      if(plateau[bob.x +1][bob.y-1] == 0)
      {
        bob = deplacement_diagonale_haut_droit(bob);
      }
    }
    else if (a == 5)
    {
      if(plateau[bob.x+1][bob.y+1] == 0)
      {
        bob = deplacement_diagonale_bas_droit(bob);
      }
    }
    else if (a == 6)
    {
      if(plateau[bob.x -1][bob.y+1]==0)
      {
        bob = deplacement_diagonale_bas_gauche(bob);
      }
    }
    a = rand()%8; // Choisit un nouveau nombre pour tenter de changer de déplacement
    bob.compteur_de_deplacement += 1;
    mise_a_jour_des_positions(vieu_bob,bob);
    actualiser();
        
  }
  bob.enervement += 1; //Si le personnage recroise un mur ou un autre personnage, son énervement sera plus grand et fera plus de déplacements aléatoires

  return bob;
}

//fonction qui permet de deplacer les personnages

Personnage deplacement_horizontal_gauche(Personnage bob)
{

   //On efface son ancienne position
  plateau[bob.x][bob.y]=0; // On change donc la valeur de son ancienne position dans le tableau, ce qui permet de débloquer la case 
  bob.x -= 1; // On change la position x de la struture du personnage
  plateau[bob.x][bob.y]=1; // On change la valeur de sa nouvelle position dans le tableau, ce qui permet de bloquer la case
   // On dessine sa nouvelle position

  return bob;

}

Personnage deplacement_diagonale_bas_gauche(Personnage bob)
{

  
  plateau[bob.x][bob.y]=0;
  bob.x -= 1;
  bob.y += 1;
  plateau[bob.x][bob.y]=1;
  

  return bob;
}

Personnage deplacement_vertical_bas(Personnage bob)
{

  
  plateau[bob.x][bob.y]=0;
  bob.y += 1;
  plateau[bob.x][bob.y]=1;
  

  return bob;
}


Personnage deplacement_diagonale_bas_droit(Personnage bob)
{

    
    plateau[bob.x][bob.y]=0;
    bob.x += 1;
    bob.y += 1;
    plateau[bob.x][bob.y]=1;
    

    return bob;
}

Personnage deplacement_horizontal_droit(Personnage bob)
{ 
    
    
    plateau[bob.x][bob.y]=0;
    bob.x += 1;
    plateau[bob.x][bob.y]=1;
    

    return bob;
}

Personnage deplacement_diagonale_haut_droit(Personnage bob)
{
    
    
    plateau[bob.x][bob.y]=0;
    bob.x += 1;
    bob.y -= 1;
    plateau[bob.x][bob.y]=1;
    

    return bob;
}

Personnage deplacement_vertical_haut(Personnage bob)
{

    
    plateau[bob.x][bob.y]=0;
    bob.y -= 1;
    plateau[bob.x][bob.y]=1;
    

    return bob;

}

Personnage deplacement_diagonale_haut_gauche(Personnage bob)
{
    
    plateau[bob.x][bob.y]=0;
    bob.x -= 1;
    bob.y -= 1;
    plateau[bob.x][bob.y]=1;
    

    return bob;

}     


//fonction qui permet d'effacer le carré
 
void effacer_carre(Personnage bob)
{
  Point p_perso = {bob.x*UNITE,bob.y*UNITE};
    if((bob.x != bob.but_x)||(bob.y != bob.but_y)) // Si le peronnage n'est pas arrivé, on dessine un carré noir
  {
    dessiner_rectangle(p_perso,UNITE,UNITE,black);
  }
  else //Sinon, on dessine un carré vert, couleur qui représente les arrivées
  {
    dessiner_rectangle(p_perso,UNITE,UNITE,vert);
  }

}

void mise_a_jour_des_positions(Personnage vieu_bob , Personnage bob)
{
    effacer_carre(vieu_bob);
    dessiner_le_personnage(bob);
}
    

/*fonction qui permet d'afficher le score*/

void afficher_score(int nombre_deplacement)
{
  Point p1 = {50,300}; // Point choisi à partir duquel sera afficher le score
  int nombre_de_deplacement = nombre_deplacement;
  char texte1[1000],texte2[] = "Le score est de :";
  sprintf(texte1,"%d",nombre_de_deplacement); //On affecte a texte1 le nombre de déplacement
  strcat(texte2,texte1); //On regroupe les deux textes
  afficher_texte(texte2,40,p1,vert); //On affiche le texte final
}

void charger_couleur(int tab_couleur[NB_CASES_X][NB_CASES_Y])
{
   int i,j,a,b,c;
   
   // On parcourt l'ensemble du tableau pour affecter à chaque case une couleur aléatoire
   
   for(i = 0;i<NB_CASES_X;i++)
   {
      for(j = 0;j<NB_CASES_Y;j++)
      {
          a = rand()%256;
          b = rand()%256;
          c = rand()%256;

          tab_couleur[i][j] = fabrique_couleur(a,b,c); //On fabrique la couleur
       }
     }
}

void charge_peuple(char *fjoueur,Personnage peuple[50])
{

//On s'inspire du document foule.c

    int tab_couleur[NB_CASES_X][NB_CASES_Y] = {{0}};
    charger_couleur(tab_couleur); //On charge la couleur de chaque case
    FILE *f = fopen(fjoueur,"r");
    int dimx,a,b,c,d,j;

    if(!f)
    {
        printf("Fichier '%s': ouverture impossible\n",fjoueur);
        exit(1);
    }

    fscanf(f,"%d\n",&dimx);
    if(dimx!=50)
    {
        printf("Fichier '%s': dimensions du tableau lues dans incorrectes\n",fjoueur);
        exit(1);
    }
    
    for(j=0;j!= 50;j++)
    {
      fscanf(f,"%d %d %d %d\n",&a,&b,&c,&d); //On récupère les valeurs des coordonées du document joueurs
      Personnage bob;
      bob.x = a; //On affecte la première valeur du tableau à l'absisse de départ du personnage
      bob.y = b; //On affecte la deuxième valeur du tableau à l'ordonnée de départ du personnage
      bob.but_x = c; //On affecte la troisième valeur du tableau à l'abscisse d'arrivée du personnage
      bob.but_y = d; //On affecte la quatrième valeur du tableau à l'ordonnée d'arrivée du personnage
      bob.enervement = 1;
      
      bob.compteur_de_deplacement = 1;

      bob.couleur = tab_couleur[a][b]; //On affecte la couleur au peronnage en focntion de leur case de départ

      peuple[j] = bob; // On créer un tableau peuple avec toutes les structures de personnages, qui sera ré-utilisé dans la fonction création_du_personnage
    }

    fclose(f);
}

//Pour commencer le jeu

int lancer_le_jeu()
{
    int a;
    a = 2;
    while(a != 1) //Tant qu'il n'y a pas eu de clic sur une des deux images, il reste sur la page d'accueil
    {
       Point p_start = {290,10}; //Point coin où est affiché l'image de départ
       Point p_sortie = {290,300}; //Point coin où est affiché l'image de sortie
       
       afficher_image(IMAGE_DEPART,p_start); //Afficher le bouton de départ
       afficher_image(IMAGE_SORTIE,p_sortie); //Afficher le bouton de sortie
       actualiser();
       Point p = attendre_clic(); //On attend la volonté du joueur
       if((p.x >= 290 && p.x <= 510)&&(p.y >= 10 && p.y <= 239)) //Si le clic est dans l'image de départ, on effectue la condition
       {
         nettoyer_plateau();
         return 1;
       }
       else if((p.x >= 290 && p.x <= 510)&&(p.y >= 300 && p.y <= 523)) //Si le clic est dans l'image de sortie, on quitte le jeu
         return 0;
    }

}


// Permet de nettoyer le plateau pour remettre un menu de démarage 
void nettoyer_plateau()
{
  Point p0 = {0,0}; //Point qui nous sert à effacer les images pour commencer le jeu
  dessiner_rectangle(p0,NB_CASES_X*UNITE,NB_CASES_Y*UNITE,black);
}

