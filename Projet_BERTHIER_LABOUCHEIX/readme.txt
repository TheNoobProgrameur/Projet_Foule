On a ajouté pour vous quelques fonctions.

////////////////////////////////////////////////////////////////////////////////
1. Fonction 'charge_plan'
Le fichier lib/lib_foule.c contient une fonction 'charge_plan' que l'on utilise dans la fonction main pour charger à partir d'un fichier (dont le nom et l'emplacement sont fixés par la variable globale FICHIER_PLAN) contenant des caractères _ et *,  un tableau 'plateau' contenant des 1 et des 0.
La fonction est appelée au début du main et le tableau est chargé avec le plan ; libre à vous de modifier comme bon vous semble les variables et fonctions.

////////////////////////////////////////////////////////////////////////////////
2. Affichage de texte
pour fonctionner, la police doit se trouver dans le dossier lib (la police peut être changée en changeant la variable globale NOM_POLICE ) dans libgraphique.h


//affiche du texte de taille de police donnée ; coin est le coin haut gauche du texte
void afficher_texte(char *texte, int taille, Point coin, Couleur couleur);


//renvoie un point qui contient la taille en pixels que prendrait ce texte si on l'affichait (largeur,hauteur) à la taille de police donnée
Point taille_texte(char *texte, int taille);


////////////////////////////////////////////////////////////////////////////////
3. Hasard

//renvoie un entier au hasard entre 0 et n-1
int entier_aleatoire(int n) ;
