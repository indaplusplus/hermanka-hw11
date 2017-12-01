#ifndef FILESYSTEM_CLIENT_H
#define FILESYSTEM_CLIENT_H

#include "tree.h"
#include "server.h"
#include "constants.h"

#include <stdexcept>

class Client {
private:
  Server* server;
  HashType top_hash;
public:

  Client(Server* server): server(server) {}

  void write_byte(int byte_id, byte data) {
    top_hash = server->write_byte(byte_id, data);
  }

  byte read_byte(int byte_id) {
    int block_id = byte_id/BLOCK_SIZE;
    BlockType block;
    std::vector<HashType> hashes;
    std::tie(hashes, block) = server->read_block(block_id);
    if (HashTree().generate_top_hash_from_data_and_hashes(hashes, block, block_id) != top_hash)
      throw std::runtime_error("something wrong with the hashes");
    return block[byte_id % BLOCK_SIZE];
  }

};

#endif //FILESYSTEM_CLIENT_H
