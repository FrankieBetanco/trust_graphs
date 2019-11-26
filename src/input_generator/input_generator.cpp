#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <time.h>

#include "input_generator.h"
using namespace std;

const char *firstNames[] = 
{"Liam",
 "Emma", "Noah",
 "Olivia", "William",
 "Ava", "James",
 "Isabella", "Oliver",
 "Sophia", "Benjamin",
 "Charlotte", "Elijah",
 "Mia", "Lucas",
 "Amelia", "Mason",
 "Harper", "Logan",
 "Evelyn", "Alexander",
 "Abigail", "Ethan",
 "Emily", "Jacob",
 "Elizabeth", "Michael",
 "Mila", "Daniel",
 "Ella", "Henry",
 "Avery", "Jackson",
 "Sofia", "Sebastian",
 "Camila", "Aiden",
 "Aria", "Matthew",
 "Scarlett", "Samuel",
 "Victoria", "David",
 "Madison", "Joseph",
 "Luna", "Carter",
 "Grace", "Owen",
 "Chloe", "Wyatt",
 "Penelope", "John",
 "Layla", "Jack",
 "Riley", "Luke",
 "Zoey", "Jayden",
 "Nora", "Dylan",
 "Lily", "Grayson",
 "Eleanor", "Levi",
 "Hannah", "Isaac",
 "Lillian", "Gabriel",
 "Addison", "Julian",
 "Aubrey", "Mateo",
 "Ellie", "Anthony",
 "Stella", "Jaxon",
 "Natalie", "Lincoln",
 "Zoe", "Joshua",
 "Leah", "Christopher",
 "Hazel", "Andrew",
 "Violet", "Theodore",
 "Aurora", "Caleb",
 "Savannah", "Ryan",
 "Audrey", "Asher",
 "Brooklyn", "Nathan",
 "Bella", "Thomas",
 "Claire", "Leo",
 "Skylar", "Isaiah",
 "Lucy", "Charles",
 "Paisley", "Josiah",
 "Everly", "Hudson",
 "Anna", "Christian",
 "Caroline", "Hunter",
 "Nova", "Connor",
 "Genesis", "Eli",
 "Emilia", "Ezra",
 "Kennedy", "Aaron",
 "Samantha", "Landon",
 "Maya", "Adrian",
 "Willow", "Jonathan",
 "Kinsley", "Nolan",
 "Naomi", "Jeremiah",
 "Aaliyah", "Easton",
 "Elena", "Elias",
 "Sarah", "Colton",
 "Ariana", "Cameron",
 "Allison", "Carson",
 "Gabriella", "Robert",
 "Alice", "Angel",
 "Madelyn", "Maverick",
 "Cora", "Nicholas",
 "Ruby", "Dominic",
 "Eva", "Jaxson",
 "Serenity", "Greyson",
 "Autumn", "Adam",
 "Adeline", "Ian",
 "Hailey", "Austin",
 "Gianna", "Santiago",
 "Valentina", "Jordan",
 "Isla", "Cooper",
 "Eliana", "Brayden",
 "Quinn", "Roman",
 "Nevaeh", "Evan",
 "Ivy", "Ezekiel",
 "Sadie", "Xavier",
 "Piper", "Jose",
 "Lydia", "Jace",
 "Alexa", "Jameson",
 "Josephine", "Leonardo",
 "Emery", "Bryson",
 "Julia", "Axel",
 "Delilah", "Everett",
 "Arianna", "Parker",
 "Vivian", "Kayden",
 "Kaylee", "Miles",
 "Sophie", "Sawyer",
 "Brielle", "Jason",
 "Madeline"};

const char *lastNames[] = 
{ "Williams", "Jones",
 "Brown", "Davis",
 "Miller", "Wilson",
 "Moore", "Taylor",
 "Anderson", "Thomas",
 "Jackson", "White",
 "Harris", "Martin",
 "Thompson", "Garcia",
 "Martinez", "Robinson",
 "Clark", "Rodriguez",
 "Lewis", "Lee",
 "Walker", "Hall",
 "Allen", "Young",
 "Hernandez", "King",
 "Wright", "Lopez",
 "Hill", "Scott",
 "Green", "Adams",
 "Baker", "Gonzalez",
 "Nelson", "Carter",
 "Mitchell", "Perez",
 "Roberts", "Turner",
 "Phillips", "Campbell",
 "Parker", "Evans",
 "Edwards", "Collins",
 "Stewart", "Sanchez",
 "Morris", "Rogers",
 "Reed", "Cook",
 "Morgan", "Bell",
 "Murphy", "Bailey",
 "Rivera", "Cooper",
 "Richardson", "Cox",
 "Howard", "Ward",
 "Torres", "Peterson",
 "Gray", "Ramirez",
 "James", "Watson",
 "Brooks", "Sanders",
 "Price", "Bennett",
 "Wood", "Barnes",
 "Ross", "Henderson",
 "Coleman", "Jenkins",
 "Perry", "Powell",
 "Long", "Patterson",
 "Hughes", "Flores",
 "Washington", "Butler",
 "Simmons", "Foster",
 "Gonzales", "Bryant",
 "Alexander", "Russell",
 "Griffin", "Diaz",
 "Hayes"};

const char *languages[] = 
{ "Mandarin", "English", 
  "Hindi", "Urdu", "Spanish",
  "Arabic", "Malay", "Russian",
  "Bengali", "Portuguese", "French" };

const char *domains[] = 
{ "gmail.com", "yahoo.com", "hotmail.net", 
  "comcast.net" };

const char *services[] = 
{ "Youtube", "Facebook", "Twitter", 
  "Google", "Amazon", "Borg" };

const char *groups[] = 
{ "Deep_Space_Enthusiasts", "Golfers", 
  "College_Football_Fans", "OU_Fans", 
  "Okstate_Fans", "University_of_Tennessee_Fans", 
  "Linux_Users", "PC_Users", "Mac_Users", "Vim_Users", 
  "Emacs_Users"};

input_generator::input_generator(unsigned seed) {
  srand(seed);
}

string input_generator::first_name() {
  return string(firstNames[rand() % 200]);
}

string input_generator::last_name() {
  return string(lastNames[rand() % 96]);
}

string input_generator::age() {
  return to_string(rand() % 70 + 13);
}

string input_generator::gender() {
  string rv; 
  if ( rand() % 2 == 0 ) {
    rv = "M";
  } else {
    rv = "F";
  }
  return rv;
}

string input_generator::primary_language() {
  return string(languages[rand() % 11]);
}

string input_generator::sns_login_id() {
  return to_string(rand() % 100000 + 1);
}

string input_generator::email(string firstName, string lastName) {
  return firstName + "." + lastName + "@" + string(domains[rand() % 4]);
}

string input_generator::choose_rand_sublist(const char *list[], int list_len) {
  int num_to_choose = rand() % 3 + 1;
  string sublist; 

  for (int i = 0; i < num_to_choose; i++) {
    sublist += list[rand() % list_len]; 
    if ( i + 1 < num_to_choose ) {
      sublist += " ";
    }
  }
  return sublist;
}

string input_generator::group_membership() {
  return choose_rand_sublist(groups, 11);
}

string input_generator::subscribed_to_services() {
  return choose_rand_sublist(services, 6);
}

string input_generator::permissions() {
  unsigned short anonymity, privacy;
  privacy = rand() % 512;
  anonymity = ~privacy;
  return to_string(privacy) + "," + to_string(anonymity);
}

string input_generator::full_user() {
  string user;
  string firstName = first_name(); 
  string lastName = last_name(); 

  user += firstName;
  user += "," + lastName; 
  user += "," + age();
  user += "," + gender();
  user += "," + primary_language();
  user += "," + sns_login_id();
  user += "," + email(firstName, lastName);
  user += "," + subscribed_to_services();
  user += "," + group_membership();
  user += "," + permissions();
  return user;
}
