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
#include <stdlib.h>
#include "cards.h"
#include "shoe.h"
#include "dealer.h"
#include "player.h"
void initdealer(dealer *d) {
	d->hand = (card *)malloc(sizeof(card) * 10);
}
void dealdisplay(dealer *d, GtkGrid *grid) {
	/*used to display the dealer's hole card, right after the deal*/
	GtkWidget *image;
	image = gtk_image_new_from_file("cards_png/b1fv.png");
	gtk_grid_attach(grid, image, 1, 0, 1, 1);
	gtk_grid_attach(grid, d->hand[1].image, 2, 0, 1, 1);
}
int dsum(dealer *d) {
	/*calculates and returns the dealer's sum*/
	int i, sum, x, ace;
	i = 0;
	sum = 0;
	ace = 0;
	while(i < d->hno) {
		x = d->hand[i].rank;
		if(x > 10)
			x = 10;
		if(x == 1) {
			x = 10;
			ace++;
		}
		sum = sum + x;
		i++;
	}
	if(sum > 21 && ace > 0)
		sum = sum - (ace * 10) + ace;
	return sum;
}
void dhit(dealer *d, shoe *s, GtkGrid *grid) {
	gtk_grid_remove_column(grid, 1);
	/*the dealer has no option but to keep hitting till he reaches 17 or higher*/
	while(dsum(d) < 17) {
		d->hand[d->hno] = drawscard(s);
		gtk_grid_attach(grid, d->hand[d->hno].image, d->hno++, 0, 1, 1);
	}
	gtk_grid_attach(grid, d->hand[0].image, d->hno, 0, 1, 1);
}
