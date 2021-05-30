#ifndef _SAVE_H_
#define _SAVE_H_

void fichier_savep1(struct game* game, struct player* player);
void clear();
void ouverture();
int niveausave();
int lecturefichier(int u);
void fichier_savemap(struct map* map, struct game* game, int niveau);
void affichebin(unsigned n);
void load_map(struct map* map, struct game* game, int niveau);



#endif /* _MUSIC_H_ */
