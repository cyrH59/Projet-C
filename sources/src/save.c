#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <music.h>
#include <window.h>
#include <monster.h>
#include <player.h>
#include <game.h>

// On sauvegarde toutes les valeurs entières utiles
void fichier_savep1(struct game* game, struct player* player){

    FILE *fichierp1=NULL;
    fichierp1 = fopen("src/savep1.txt", "r+");

    if (fichierp1 != NULL)
    {
        printf("Bravo \n");
        fprintf(fichierp1, "%d\n", game_level(game));
        fprintf(fichierp1, "%d\n", player_get_x(player));
        fprintf(fichierp1, "%d\n", player_get_y(player));
        fprintf(fichierp1, "%d\n", player_get_nb_bomb(player));
        fprintf(fichierp1, "%d\n", player_get_range_bomb(player));
        fprintf(fichierp1, "%d\n", player_get_nb_life(player));

        for (int i=0; i<8;i++){
             fprintf(fichierp1, "%d\n", state_tab_door(i));

        }
        for (int i=0; i<8;i++){
             fprintf(fichierp1, "%d\n", state_tab_key(i));
             printf("%d",state_tab_key(i));

        }
        for (int i=0 ;i<8;i++){
             fprintf(fichierp1, "%d\n", state_tab_pokeflute(i));

        }

      


        fclose(fichierp1);
        // On peut lire et écrire dans le fichier
    }
    else
    {
        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier test.txt \n");
    }


    return ;

}

// Pas utilisée finalement
void affichebin(unsigned n)
{
	unsigned bit = 0 ;
	unsigned mask = 1 ;
	for (int i = 0 ; i < 32 ; ++i)
	{
		bit = (n & mask) >> i ;
		printf("%d", bit) ;
		mask <<= 1 ;
	}
}

//On sauvegarde les maps de notre partie en cours
void fichier_savemap(struct map* map, struct game* game, int niveau){
    
    
    
     FILE *fichiermap=NULL;
    if (niveau ==1){ fichiermap = fopen("src/maps/map1.txt", "r+");}
    if (niveau ==2){ fichiermap = fopen("src/maps/map2.txt", "r+");}
    if (niveau ==3){ fichiermap = fopen("src/maps/map3.txt", "r+");}
    if (niveau ==4){ fichiermap = fopen("src/maps/map4.txt", "r+");}
    if (niveau ==5){ fichiermap = fopen("src/maps/map5.txt", "r+");}
    if (niveau ==6){ fichiermap = fopen("src/maps/map6.txt", "r+");}
    if (niveau ==7){ fichiermap = fopen("src/maps/map7.txt", "r+");}
    if (niveau ==8){ fichiermap = fopen("src/maps/map8.txt", "r+");}

    if (fichiermap != NULL)
    {
    
    for (int i=0;i<12;i++){
        for (int j=0;j<12;j++){

         fprintf(fichiermap,"%d ",map_get_cell_type(map,j,i)+ map_get_scenery_type(map,j,i));
        if (j==11){
            fprintf(fichiermap,"%d \n",map_get_cell_type(map,j,i)+map_get_scenery_type(map, j, i));
        }
        
        
    
   
    }}}
    fclose(fichiermap);
    
    
    
  

}

//On charge la map que l'on avait préalablement sauvegardé
void load_map(struct map* map, struct game* game, int niveau){

     FILE *fichiermap=NULL;
    if (niveau ==1){ fichiermap = fopen("src/maps/map1.txt", "r+");}
    if (niveau ==2){ fichiermap = fopen("src/maps/map2.txt", "r+");}
    if (niveau ==3){ fichiermap = fopen("src/maps/map3.txt", "r+");}
    if (niveau ==4){ fichiermap = fopen("src/maps/map4.txt", "r+");}
    if (niveau ==5){ fichiermap = fopen("src/maps/map5.txt", "r+");}
    if (niveau ==6){ fichiermap = fopen("src/maps/map6.txt", "r+");}
    if (niveau ==7){ fichiermap = fopen("src/maps/map7.txt", "r+");}
    if (niveau ==8){ fichiermap = fopen("src/maps/map8.txt", "r+");}

     if (fichiermap != NULL){
       int x;
       

        for (int i=0;i<12;i++){
           for (int j=0;j<12;j++){
                fscanf(fichiermap,"%d ",&x);
                map_set_cell_type(map,j,i,x);
                if (j==11){
                
                fscanf(fichiermap,"%d \n",&x);
                map_set_cell_type(map,j,i,x);
                
        }

           }}}



}
void clear(){

    FILE *fp = fopen("src/savep1.txt", "w"); // l'ouverture en écriture permet de supprimer le contenu d'un fichier. 
    fclose(fp); 
}


// ON charge le fichier savep1.txt
int lecturefichier(int u){
  
       FILE* fp;
     fp = fopen("src/savep1.txt","r+");
   

    int level,x,y,bomb,range,life=0;
    int key[8]={0};
    int pokeflute[8]={0};
    int door[8]={0};
    int i,j,h=0;
 
 
    fscanf(fp,"%d%d%d%d%d%d",&level,&x,&y,&bomb,&range,&life);

    for (i=0;i<8;i++){
         fscanf(fp,"%d",&door[i]);
    }
    for (j=0;j<8;j++){
         fscanf(fp,"%d",&key[j]);
    }
    for (h=0;h<8;h++){
         fscanf(fp,"%d",&pokeflute[h]);
    }
if (u ==0){
    return level;
}
if (u ==1){
    return x;
}
if (u ==2){
    return y;
}
if (u ==3){
    return bomb;
}
if (u ==4){
    return range;
}
if (u ==5){
    return life;
}
if ((u == 6) || (u == 7) || (u==8) |(u == 9) || (u == 10 )|| (u==11)||(u == 12) || (u == 13) ){
    return key[u-6];
}
if ((u == 14) || (u == 15 )|| (u==16) ||(u == 17) || (u == 18) || (u==19)||(u == 20) || (u == 21) ){
    return pokeflute[u-14];
}
if ( (u == 22) || (u == 23) || (u==24) ||(u == 25) || (u == 26) || (u==27)||(u == 28) ||( u == 29) ){
    return door[u-22];
}

     
 return 0;
}


 



    


