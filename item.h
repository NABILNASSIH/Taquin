#ifndef ITEM_H
#define ITEM_H

struct Item_s;

typedef struct Item_s {
    char size; // size of board (< 255)
	char *board; // board is an array of small int
	char blank;
	float f, g, h; // cost, heuristic, ...
  int depth;
  struct Item_s *parent; // needed for tree structure in game search
  struct Item_s *prev, *next; // needed for chained list 
} Item;

#endif 
