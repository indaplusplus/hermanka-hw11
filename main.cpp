#include <iostream>
#include <sstream>
#include "server.h"
#include "client.h"

int main() {

  Server server;
  Client client(&server);

  while (true) {
    std::string line;
    if (!std::getline(std::cin, line)) break;
    std::stringstream stream(line);
    char c;
    stream >> c;
    if (c == 'w') {
      int idx;
      byte data;
      stream >> idx >> std::hex >> data;
      client.write_byte(idx, data);
    }
    if (c == 'r') {
      int idx;
      stream >> idx;
      byte data = client.read_byte(idx);
      std::cout << std::hex << data << std::endl;
    }
    if (c == 'e') break;
  }

}