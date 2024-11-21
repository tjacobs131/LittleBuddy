#include "Arduino.h"
#include "map"

// dictionary niet altijd opnieuw instellen nog maken
std::map<std::string, std::string> dictionary_user = {
    {"211208238166", "Koen Dirckx"},    // tag
    {"35145245238", "Koen Dirckx"},     // card
    {"115156135165", "Koen Wijlick"},   // tag
    {"51231193238", "Koen Wijlick"},    // card
    {"227171228166","James Boogaard"},  // tag
    {"22708243238", "James Boogaard"},  // card
    {"7", "Tom Jacobs"},    // tag
    {"8", "Tom Jacobs"}     // card
};

std::string  check_user(std::string  NFC_code)
{
    std::string  temp_user = dictionary_user[NFC_code];

    if(temp_user.empty())
    {
        std::string not_User = "Not a User";
        return not_User;
    }
    else{
        return temp_user;
    }
}
