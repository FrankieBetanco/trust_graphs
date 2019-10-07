#include "input_generator.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  int n_users;
  if ( argc < 2 ) {
    cout << "Usage: input_gen [number of users to generate]\n";
    return -1;
  }
  n_users = stoi(argv[1]);

  input_generator gen(time(NULL));
  for (int i = 0; i < n_users; i++) {
    cout << gen.full_user() << '\n';
  }

  return 0;
}
