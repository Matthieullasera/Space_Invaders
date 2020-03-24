#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <stdint.h>

void fenetre();
void menu();
void monstres();
void chrono();
void regle();
void Tir_vaisseau();
void Vaisseau();
void Delai(unsigned long n);
void hitbox();
void deplacement_monstre();
void droite_monstre(uint8_t x);
void bas_droite_monstre(uint8_t y);
void gauche_monstre(uint8_t x);
void bas_gauche_monstre(uint8_t y);
uint8_t fin_win();
uint8_t fin_loose();


#endif /* FUNCTIONS_H_ */
