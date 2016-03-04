#pragma once

struct product{
  int id;
  //char *color;
};

struct node{
  struct product *data;
  struct node *next;
};
