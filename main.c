#include "arraylist.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

typedef struct person_t {
  char name[100];
  int age;
} Person;

al_define(int, int);
al_define(person, Person);
al_define(float, float);
al_define(char, char *);

static int int_to_string(int t, char *buf) {
  return snprintf(buf, AL_MAXBUF, "%d", t);
}
static int float_to_string(float t, char *buf) {
  return snprintf(buf, AL_MAXBUF, "%.2f", t);
}
static int person_to_string(Person t, char *buf) {
  return snprintf(buf, AL_MAXBUF, "%s: age %d", t.name, t.age);
}

static int person_cmp(Person a, Person b) { return (a.age == b.age) ? 0 : 1; }

int main() {
  int *i_array = al_init_int();
  assert(al_length(i_array) == 0);
  assert(al_capacity(i_array) == AL_INIT_CAPACITY);

  for (int i = 0; i < 10; i++) {
    al_push_int(&i_array, i);
  }
  assert(al_length(i_array) == 10);
  al_print_int(i_array, int_to_string);

  Person *p_array = al_init_person();
  assert(al_length(p_array) == 0);
  assert(al_capacity(p_array) == AL_INIT_CAPACITY);
  Person p1 = {"John", 18}, p2 = {"Jane", 20}, p3 = {"Lord", 40};
  al_push_person(&p_array, p1);
  al_push_person(&p_array, p2);
  al_push_person(&p_array, p3);
  al_insert_person(&p_array, 0, (Person){"Lizzy", 27});
  al_insert_person(&p_array, 3, (Person){"Andrea", 17});
  al_insert_person(&p_array, 1, (Person){"Monica", 33});
  assert(al_length(p_array) == 6);

  al_push_person(&p_array, p1);
  al_push_person(&p_array, p1);
  al_push_person(&p_array, p1);
  assert(al_rmall_person(p_array, p1, person_cmp) == 4);
  al_rm_person(p_array, p2, person_cmp);

  al_push_person(&p_array, p1);
  al_push_person(&p_array, p2);
  al_print_person(p_array, person_to_string);
  al_free(i_array);
  al_free(p_array);

  return 0;
}
