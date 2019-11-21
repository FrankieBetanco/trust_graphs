#include <string> 

class input_generator{
public:
  input_generator(unsigned seed);
  std::string first_name();
  std::string last_name();
  std::string age();
  std::string gender();
  std::string primary_language();
  std::string sns_login_id();
  std::string email(std::string firstName, std::string lastName);
  std::string choose_rand_sublist(const char *list[], int list_len);
  std::string group_membership();
  std::string subscribed_to_services();
  std::string permissions();
  std::string full_user();
};

