#ifndef IDGEN_H
#define IDGEN_H

static unsigned int curr_id = 1;

unsigned int get_unique_id() {return curr_id++;}

#endif