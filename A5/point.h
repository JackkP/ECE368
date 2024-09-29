/*
 * Jack Porter, Purdue ECE368
 * Assignment 5, point struct
 */

#ifndef POINT_H
#define POINT_H

typedef struct point
{
  int x;
  int y;
  struct point * next;
} point;

#endif