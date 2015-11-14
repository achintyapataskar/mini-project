/*the game of Blackjack.This is my miniproject for 2015
*    Copyright (C) 2015  Achintya Pataskar
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.*/
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "cards.h"
#include "shoe.h"
#include "dealer.h"
#include "player.h"
#include "config.h"
void initplayer(player *p) {
	/*initialises the player at the start of the game*/
	p->hand = (card *)malloc(sizeof(card) * 10);
	int i = 0;
	card x;
	x.rank = 0;
	while(i < 10) {
		p->hand[i].rank = 0;
		i++;
	}
	p->aux = (card *)malloc(sizeof(card) * 10);
	p->bet = 0.0;
	p->money = 50000.0;
	p->hno = 0;
	p->ano = 0;
}
int playersum(player *p) {
	/*calculates and returns the sum of player's cards*/
	int sum, i, x, ace;
	i = 0;
	sum = 0;
	x = 0;
	ace = 0;
	while(i < p->hno) {
		x = p->hand[i].rank;
		if(x > 10)
			x = 10;
		if(x == 1) {
			x = 10;
			ace++;
		}
		sum = sum + x;
		i++;
	}
	if(sum > 21 && ace != 0)
		sum = sum - (ace * 10) + ace;
	return sum;
}
void dealdispplayer(player *p, GtkGrid *grid) {
	/*used to display the player's hand after a deal*/
	gtk_grid_attach(grid, p->hand[0].image, 1, 0, 1, 1);
	gtk_grid_attach(grid, p->hand[1].image, 2, 0, 1, 1);
}
void display(player *p, GtkGrid *grid) {
	/*used to display the player's hand after hitting*/
	/*displays only the last card in the dealer's hand*/
	gtk_grid_attach(grid, p->hand[p->hno - 1].image, p->hno, 0, 1, 1);
}
void result(player *p, dealer *d, GtkLabel *result, GtkLabel *bamt, GtkLabel *mamt) {
	/*based on player and dealer sums, this function determines the result and prints it*/
	int i, j;
	char str[32];
	i = playersum(p);
	j = dsum(d);
	if(i > 21) {
		gtk_label_set_text(result, "BUST !!!");
		p->bet = 0;
		gtk_label_set_text(bamt, "0.0");
		sprintf(str, "%.2lf", p->money);
		gtk_label_set_text(mamt, str);
		return;
	}
	if(j > 21) {
		gtk_label_set_text(result, "DEALER BUSTS !!!");
		gtk_label_set_text(bamt, "0.0");
		p->money += (2 * p->bet);
		sprintf(str, "%.2lf", p->money);
		gtk_label_set_text(mamt, str);
		return;
	}
	if(i == 21) {
		int flag = 0;
		int k = 0;
		while(k < p->hno) {
			/*checking if all 3 cards are 7's*/
			if(p->hand[k].rank != 7) {
				flag = 1;
				break;
			}
			k++;
		}
		p->money += (2 * p->bet);
		if(flag == 0) {
			/*if all 3 cards are 7's then reward is increased */
			gtk_label_set_text(result, "---BLACKJACK---");
			p->money += (2.5 * p->bet);
			gtk_label_set_text(bamt, "0.0");
			sprintf(str, "%.2lf", p->money);
			gtk_label_set_text(mamt, str);
		} else {
			gtk_label_set_text(result, "WIN !!!");
			gtk_label_set_text(bamt, "0.0");
			sprintf(str, "%.2lf", p->money);
		gtk_label_set_text(mamt, str);
		}
		return;
	}
	if(i == j) {
		/*a push is when the dealer and player have the same sum*/
		/*in this case no one wins anything*/
		p->money += p->bet;
		gtk_label_set_text(result, "PUSH");
		gtk_label_set_text(bamt, "0.0");
		sprintf(str, "%.2lf", p->money);
		gtk_label_set_text(mamt, str);
		return;
	}
	if(i > j) {
		/*player wins as his sum is more than the dealer's*/
		p->money += 2 * p->bet;
		gtk_label_set_text(result, "WIN !!!");
		gtk_label_set_text(bamt, "0.0");
		sprintf(str, "%.2lf", p->money);
		gtk_label_set_text(mamt, str);
		return;
	}
	if(i < j) {
		/*dealer wins as his sum is more than the player's*/
		p->bet = 0;
		gtk_label_set_text(result, "LOSE !!!");
		gtk_label_set_text(bamt, "0.0");
		sprintf(str, "%.2lf", p->money);
		gtk_label_set_text(mamt, str);
		return;
	}
}
void deal(player *p, dealer *d, shoe *s, GtkGrid *grid1, GtkGrid *grid2) {
	/*a deal starts the hand*/
	/*clears out the previous hand's details*/
	gtk_grid_remove_row(grid1, 0);	
	gtk_grid_remove_row(grid2, 0);
	GtkWidget *dealerhand, *playerhand;
	dealerhand = gtk_label_new("Dealer's Hand: ");
	playerhand = gtk_label_new("Player's Hand: ");
	gtk_grid_attach(grid1, dealerhand, 0, 0, 1, 1);
	gtk_grid_attach(grid2, playerhand, 0, 0, 1, 1);
	shoeshuffle(s);
	shoeshuffle(s);
	p->bet = 0.0;
	s->dno = 0;
	/*gives two cards to each player and dealer*/
	d->hand[0] = drawscard(s);
	p->hand[0] = drawscard(s);
	p->hand[1] = drawscard(s);
	d->hand[1] = drawscard(s);
	p->hno = 2;
	d->hno = 2;
}
void hit(shoe *s, player *p) {
	/*if a allowed this function hits i.e. puts one more card in the dealer's hand*/
	if(playersum(p) >= 21)
		return;
	p->hand[p->hno++] = drawscard(s);
		
}
void stand(shoe *s, dealer *d, GtkGrid *grid) {
	/*stand can be thought of as a dealer's turn to hit till his total reaches 17*/
	dhit(d, s, grid);
}
void doubledown(shoe *s, player *p, GtkLabel *bamt, GtkLabel *mamt, GtkGrid *grid) {
	/*only one hit is allowed and bets are doubled*/
	hit(s, p);
	display(p, grid);
	p->money -= p->bet;
	p->bet *= 2;
	char str[32];
	sprintf(str, "%.2lf", p->money);
	gtk_label_set_text(mamt, str);
	sprintf(str, "%.2lf", p->bet);
	gtk_label_set_text(bamt, str);
}
void surrender(player *p, GtkLabel *bamt, GtkLabel *mamt) {
	/*forfeit half the bet*/
	p->money += (p->bet /2);
	p->bet = 0;
	char str[32];
	sprintf(str, "%.2lf", p->bet);
	gtk_label_set_text(bamt, str);
	sprintf(str, "%.2lf", p->money);
	gtk_label_set_text(mamt, str);
}
/*void split_display(player *p, GtkGrid *grid2) {
	gtk_grid_attach(grid2, p->aux[p->ano].image, p->ano, 1, 1, 1);
}
void cb_split_hit(GtkWidget *widget, gpointer data) {
	config *c;
	c = (config *)data;
	if(split_sum(c->p) < 21)
		c->p->aux[c->p->ano++] = drawscard(c->s);	
	else
		return;
}*/
/*void cb_split_stand(GtkWidget *widget, gpointer data) {
	config *c;
	c = (config *)data;
	g_signal_connect(c->hit, "clicked", G_CALLBACK(cb_split_hit), (gpointer)c);
	g_signal_connect(c->stand, "clicked",G_CALLBACK(cb_sec_stand), (gpointer)c);
}
int split_sum(player *p) {
	int i, k;
	i = 0;
	k = 0;
	while(i < p->ano) {
		k = k + p->aux[i].rank;
		i++;
	}
	return k;
}
void cb_sec_stand(GtkWidget *widget, gpointer data) {
	config *c;
	c = (config *)data;
	int i, j, k, l;
	char str[32];
	i = playersum(c->p);
	j = dsum(c->d);
	k = split_sum(c->p);	
	result(c->p, c->d, c->result, c->bamt, c->mamt);
	if(k > 21) {
		c->p->bet = 0;
	}
	if(k <= 21 && k > j) {
		c->p->bet = 0;
		c->p->money += (2 * c->p->bet);
	}
	if(k < 21 && k > j)
		c->p->bet = 0;
	}
	sprintf(str, "%.2lf", c->p->money);
	gtk_label_set_text(c->p->mamt, str);
	sprintf(str, "%.2lf", c->p->bet);
	gtk_label_set_text(c->p->bamt, str);
	g_signal_connect(c->hit, "clicked", G_CALLBACK(cb_hit), (gpointer)c);
	g_signal_connect(c->stand, "clicked", G_CALLBACK(cb_stand), (gpointer)c);
}*/
void split(player *p, GtkGrid *grid2) {
	card x1, x2;
	p->aux[0] = p->hand[1];
	p->ano = 1;
	gtk_grid_remove_row(grid2, 0);
	gtk_grid_insert_row(grid2, 0);
	GtkWidget *playerhand;
	playerhand = gtk_label_new("Player's Hand: ");
	gtk_grid_attach(grid2, playerhand, 0, 0, 1, 1);
	char str[32];
	sprintf(str, "cards_png/%c%d.png", p->hand[0].suite, p->hand[0].rank);	
	x1.image = gtk_image_new_from_file(str); 
	sprintf(str, "cards_png/%c%d.png", p->hand[1].suite, p->hand[1].rank);
	x2.image = gtk_image_new_from_file(str); 
	gtk_grid_attach(grid2, x1.image, 1, 0, 1, 1);
	gtk_grid_attach(grid2, x2.image, 1, 1, 1, 1);
}
