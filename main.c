#include "arraylist.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

typedef struct person_t {
  char name[100];
  int age;
} Person;

al_define(str, char *);
al_define(head, al_head);

static int int_to_string(int t, char *buf) {
  return snprintf(buf, AL_MAXBUF, "%d", t);
}
static int float_to_string(float t, char *buf) {
  return snprintf(buf, AL_MAXBUF, "%.2f", t);
}
static int head_to_string(al_head t, char *buf) {
  return snprintf(buf, AL_MAXBUF, "len: %zu - cap: %zu", t.length, t.capacity);
}

static int string_to_string(const char *t, char *buf) {
  return snprintf(buf, AL_MAXBUF, "%s", t);
}

static int person_cmp(Person a, Person b) { return (a.age == b.age) ? 0 : 1; }

int main() {

  al_head *arr = al_init_head();
  al_push_head(&arr, (al_head){10, AL_INIT_CAPACITY});
  al_push_head(&arr, (al_head){0, 128});
  al_push_head(&arr, (al_head){3, 9});
  al_push_head(&arr, (al_head){7, 16});
  al_print_head(arr, head_to_string);

  return 0;
}
