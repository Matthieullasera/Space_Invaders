#include "fonction.h"

#include "serial.h"

#include "vt100.h"

#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	uint8_t monstre_l1[7][3]; // 7 correspond au ligne et donc au nombre de monstres; et 3 correspond a x =0,y=1 et à l'état d'un seul monstre=2
	uint8_t monstre_l2[7][3];
} tab_monstre;

uint8_t y_vaisseau = 27;
uint8_t x_vaisseau = 42;
uint8_t y_tir = 26;
uint8_t x_tir;
uint8_t x_monstre_l1 = 5;
uint8_t x_monstre_l2 = 5;
uint8_t y_monstre_l1 = 5;
uint8_t y_monstre_l2 = 8;
tab_monstre monstre;
tab_monstre *p_m = &monstre;
uint8_t sens = 0; // sens =0 vers la droite ; sens =1 vers la gauche
uint8_t t = 0;
uint8_t delai_monstre = 0;
uint8_t u_sec = 0;
uint8_t d_sec = 0;
uint8_t n = 0;

void fenetre() {
	vt100_clear_screen();
	uint8_t x;
	uint8_t y;
	for (x = 1; x < 90; x++) {
		vt100_move(x, 1);
		serial_puts("#");
		vt100_move(x, 30);
		serial_puts("#");

	}
	for (y = 1; y < 30; y++) {
		vt100_move(1, y);
		serial_puts("#");
		vt100_move(90, y);
		serial_puts("#");
	}
}

void menu() {
	/* Encadrage */
	unsigned char index_x;
	unsigned char index_y;

	for (index_x = 38; index_x < 54; index_x++) {
		vt100_move(index_x, 6);
		serial_puts("#");
		vt100_move(index_x, 8);
		serial_puts("#");
	}
	for (index_y = 6; index_y < 9; index_y++) {
		vt100_move(38, index_y);
		serial_puts("#");
		vt100_move(53, index_y);
		serial_puts("#");
	}

	/* Ecriture du menu */
	vt100_move(39, 7);
	serial_puts("SPACE INVADERS");
	vt100_move(32, 15);
	serial_puts("Press any touch to start");
}

void monstres() {
	uint8_t i = 0;

	for (i = 0; i < 7; i++) {
		p_m->monstre_l1[i][0] = x_monstre_l1;
		x_monstre_l1 += 7;
		p_m->monstre_l1[i][1] = y_monstre_l1;
		vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
		serial_puts("/-0-\\");
		p_m->monstre_l2[i][0] = x_monstre_l2;
		x_monstre_l2 += 7;
		p_m->monstre_l2[i][1] = y_monstre_l2;
		vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
		serial_puts("/-0-\\");
		p_m->monstre_l1[i][2] = 0; // les monstres sont en vie
		p_m->monstre_l2[i][2] = 0;
	}
}

void deplacement_monstre() {
	Tir_vaisseau();
	droite_monstre(1);
	Delai(3);
	Tir_vaisseau();
	bas_droite_monstre(1);
	Delai(3);
	Tir_vaisseau();
	gauche_monstre(1);
	Delai(3);
	Tir_vaisseau();
	bas_gauche_monstre(1);
	Vaisseau();

}

void droite_monstre(uint8_t x) {
	uint8_t i = 0;

	if (p_m->monstre_l1[6][0] < 82 && sens == 0) {
		for (i = 0; i < 7; i++) {
			if (p_m->monstre_l1[i][2] == 0) { // pour ligne1 test pour voir si le montre i de la ligne 1 est vivant, si oui il incremente le monstre de x vers la droite
				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("     ");
				p_m->monstre_l1[i][0] += x;
				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("/-0-\\");
				Vaisseau();
				Tir_vaisseau();

			}

			if (p_m->monstre_l2[i][2] == 0) {				// pour ligne 2
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("     ");
				p_m->monstre_l2[i][0] += x;
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("/-0-\\");
			}

		}

	}

}

void bas_droite_monstre(uint8_t y) {
	uint8_t i = 0;

	if (p_m->monstre_l1[6][0] == 82 && sens == 0) {	// // si le x du monstre 0 est egale à 82 il decendra de y
		for (i = 0; i < 7; i++) {
			if (p_m->monstre_l1[i][2] == 0) {// si le monstre est vivant il l'incremente de y
				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("     ");
				p_m->monstre_l1[i][1] += y;
				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("/-0-\\");
				Vaisseau();
				Tir_vaisseau();

			}

			if (p_m->monstre_l2[i][2] == 0) {
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("     ");
				p_m->monstre_l2[i][1] += y;
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("/-0-\\");
			}
		}
		sens = 1;
	}

}

void gauche_monstre(uint8_t x) {
	uint8_t i = 0;

	if (p_m->monstre_l1[0][0] > 5 && sens == 1) {
		for (i = 0; i < 7; i++) {
			if (p_m->monstre_l1[i][2] == 0) { // pour ligne1 test pour voir si le montre i de la ligne 1 est vivant, si oui il incremente le monstre de x vers la gauche
				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("     ");
				p_m->monstre_l1[i][0] -= x;
				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("/-0-\\");
				Vaisseau();
				Tir_vaisseau();

			}

			if (p_m->monstre_l2[i][2] == 0) {				//pour ligne 2
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("     ");
				p_m->monstre_l2[i][0] -= x;
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("/-0-\\");
			}

		}
	}
}

void bas_gauche_monstre(uint8_t y) {
	uint8_t i = 0;

	if (p_m->monstre_l1[t][0] == 5 && sens == 1 && p_m->monstre_l1[t][2] == 0) {// si le x du monstre 0 est egale à 5 il decendra de y
		for (i = 0; i < 7; i++) {
			if (p_m->monstre_l1[i][2] == 0) {// si le monstre est vivant il l'incremente de y
				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("     ");

				p_m->monstre_l1[i][1] += y;

				vt100_move(p_m->monstre_l1[i][0], p_m->monstre_l1[i][1]);
				serial_puts("/-0-\\");

				Vaisseau();
				Tir_vaisseau();
			}

			if (p_m->monstre_l2[i][2] == 0) {
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("     ");
				p_m->monstre_l2[i][1] += y;
				vt100_move(p_m->monstre_l2[i][0], p_m->monstre_l2[i][1]);
				serial_puts("/-0-\\");
			}
		}
		sens = 0;
	}
	fin_loose();
}

void Vaisseau() {
	uint8_t touche = serial_get_last_char();

	if (x_vaisseau > 2 && x_vaisseau <= 83 && touche == 'q') { // deplacement vers la gauche
		vt100_move(x_vaisseau, y_vaisseau);
		serial_puts("[--o--]");
		vt100_move(x_vaisseau, y_vaisseau);
		serial_puts("       ");
		x_vaisseau -= 1;
		vt100_move(x_vaisseau, y_vaisseau);
		serial_puts("[--o--]");

	}
	if (x_vaisseau >= 2 && x_vaisseau < 83 && touche == 'd') { // deplacement vers la droite
		vt100_move(x_vaisseau, y_vaisseau);
		serial_puts("[--o--]");
		vt100_move(x_vaisseau, y_vaisseau);
		serial_puts("       ");
		x_vaisseau += 1;
		vt100_move(x_vaisseau, y_vaisseau);
		serial_puts("[--o--]");

	}
}
void Tir_vaisseau() {

	uint8_t touche = serial_get_last_char();
	uint8_t x_tir = x_vaisseau + 3;
	uint8_t i = 0;
	uint8_t t = 0;
	if (y_tir == 26 && touche == 'p') {

		for (i = 0; i < 22; i++) {
			if (y_tir == 5) {//si mon y tir = 5 on sort de la boucle for pour arreter d'incrementer le tir
				break;
			}
			vt100_move(x_tir, y_tir);			// incrementation du tir
			serial_puts("|");
			Delai(2);
			vt100_move(x_tir, y_tir);
			serial_puts(" ");
			y_tir -= 1;
			vt100_move(x_tir, y_tir);
			serial_puts("|");
			vt100_move(x_tir, y_tir);
			serial_puts(" ");
			Vaisseau();

			// hitbox
			for (t = 0; t < 7; t++) {// si le  x et y du tir et egale au (x, x+1,x+2,x+3,x+4) et y du monstre il s'effacera
				if ((p_m->monstre_l1[t][1] == y_tir
						&& (p_m->monstre_l1[t][0] == x_tir
								|| p_m->monstre_l1[t][0] + 1 == x_tir
								|| p_m->monstre_l1[t][0] + 2 == x_tir
								|| p_m->monstre_l1[t][0] + 3 == x_tir
								|| p_m->monstre_l1[t][0] + 4 == x_tir)
						&& p_m->monstre_l1[t][2] == 0)) {
					vt100_move(p_m->monstre_l1[t][0], p_m->monstre_l1[t][1]);
					serial_puts("     ");
					p_m->monstre_l1[t][2] = 1;		// le monstre est mort
					y_tir = 5;
				}

				if ((p_m->monstre_l2[t][1] == y_tir
						&& (p_m->monstre_l2[t][0] == x_tir
								|| p_m->monstre_l2[t][0] + 1 == x_tir
								|| p_m->monstre_l2[t][0] + 2 == x_tir
								|| p_m->monstre_l2[t][0] + 3 == x_tir
								|| p_m->monstre_l2[t][0] + 4 == x_tir)
						&& p_m->monstre_l2[t][2] == 0)) {
					vt100_move(p_m->monstre_l2[t][0], p_m->monstre_l2[t][1]);
					serial_puts("     ");
					p_m->monstre_l2[t][2] = 1;  // le monstre est mort
					y_tir = 5;
				}

			}

		}
		y_tir = 26;
	}
}

void chrono() {
	uint8_t i = 0;

	vt100_move(3, 2);
	serial_puts("Time : ");

	for (i = 0; i < 60; i++) {
		vt100_move(13, 2);
		serial_putchar('0' + u_sec / 10);		// permet d'afficher les unités de mon compteur
		vt100_move(12, 2);
		serial_putchar('0' + d_sec % 10);		// permet d'afficher les dizaines de mon compteur
		u_sec = (u_sec + 1) % 101;
		Tir_vaisseau();
		Vaisseau();
		Delai(10);
		if (u_sec == 100) {
			d_sec = (d_sec + 1) % 91;
			vt100_move(12, 2);
			serial_putchar('0' + d_sec % 10);
		}
	}
}
void regle() {
	vt100_move(25, 5);
	serial_puts("Le but de ta venu est simple, sauver la terre !");
	vt100_move(10, 8);
	serial_puts(
			"Nous avons 90 secondes avant que les aliens arrivent sur notre planete. ");
	vt100_move(10, 10);
	serial_puts("Nous comptons sur toi pour nous sauver !! bonne chance !");
	vt100_move(10, 12);
	serial_puts(
			"Voici les touches pour sauver la Terre : - Q pour aller a gauche ");
	vt100_move(51, 14);
	serial_puts("- D pour aller a droite");
	vt100_move(51, 16);
	serial_puts("- P pour tirer");
}
uint8_t fin_win() {
	if (p_m->monstre_l1[0][2] == 1 && p_m->monstre_l1[1][2] == 1
			&& p_m->monstre_l1[2][2] == 1 && p_m->monstre_l1[3][2] == 1
			&& p_m->monstre_l1[4][2] == 1 && p_m->monstre_l1[5][2] == 1
			&& p_m->monstre_l1[6][2] == 1 && p_m->monstre_l2[0][2] == 1
			&& p_m->monstre_l2[1][2] == 1 && p_m->monstre_l2[2][2] == 1
			&& p_m->monstre_l2[3][2] == 1 && p_m->monstre_l2[4][2] == 1
			&& p_m->monstre_l2[5][2] == 1 && p_m->monstre_l2[6][2] == 1) {
		vt100_clear_screen();
		fenetre();
		vt100_move(30, 10);
		serial_puts("LA TERRE A ETE SAUVE !");
		vt100_move(30, 15);
		serial_puts(
				"appuie sur une touche pout tenter de sauver la terre a nouveau !");
		while (serial_get_last_char() == -1) {
		}
		return 1;
	}
	return 0;
}

uint8_t fin_loose() {
	if (p_m->monstre_l1[0][1] == 25 || p_m->monstre_l1[1][1] == 25
			|| p_m->monstre_l1[2][1] == 25 || p_m->monstre_l1[3][1] == 25
			|| p_m->monstre_l1[4][1] == 25 || p_m->monstre_l1[5][1] == 25
			|| p_m->monstre_l1[6][1] == 25 || p_m->monstre_l2[0][1] == 25
			|| p_m->monstre_l2[1][1] == 25 || p_m->monstre_l2[2][1] == 25
			|| p_m->monstre_l2[3][1] == 25 || p_m->monstre_l2[4][1] == 25
			|| p_m->monstre_l2[5][1] == 25 || p_m->monstre_l2[6][1] == 25) {
		vt100_clear_screen();
		fenetre();
		vt100_move(30, 10);
		serial_puts("LA TERRE A ETE DETRUITE...");
		vt100_move(10, 15);
		serial_puts(
				"Appuie sur une touche pout tenter de sauver la terre a nouveau et tache de reussir");
		vt100_move(10, 16);
		serial_puts("Cette fois c'est pas un jeu !");
		vt100_move(10, 25);
		serial_puts(" Pour recommencer une partie redemarrer la carte...");
		return 1;
	}
	return 0;
}

void Delai(unsigned long n) {
int i = 0;
droite_monstre(1);
bas_droite_monstre(1);
gauche_monstre(1);
bas_gauche_monstre(1);
Vaisseau();
unsigned long int max = n * 100000;
do {
	i++;
} while (i <= max);
}

