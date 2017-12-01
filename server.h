#ifndef FILESYSTEM_SERVER_H
#define FILESYSTEM_SERVER_H

#include "constants.h"
#include "tree.h"

class Server {
private:
  HashTree tree;

public:
  Server(): tree() {}

  HashType write_byte(int byte_id, byte data) {
    if (byte_id >= FILESYSTEM_SIZE * BLOCK_SIZE)
      throw std::out_of_range("no such memory");
    int block_id = byte_id/BLOCK_SIZE;
    byte_id %= BLOCK_SIZE;
    tree.write_byte(block_id, byte_id, data);
    return tree.get_top_hash();
  }

  std::pair<std::vector<HashType>, BlockType> read_block(int block_id) {
    return {tree.get_leaf_hashes(), tree.get_block(block_id)};
  };

};


#endif //FILESYSTEM_SERVER_H
