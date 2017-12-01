#ifndef FILESYSTEM_HASHTREE_H
#define FILESYSTEM_HASHTREE_H

#include <cryptopp/sha.h>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>

#include "constants.h"


class HashTree {
private:
  int n_leaves;

  std::vector<BlockType> blocks;
  std::vector<HashType> node_hashes;

  void generate_leaf_hashes() {
    for (int block_id = 0; block_id < n_leaves; block_id++) {
      node_hashes[n_leaves + block_id] = get_block_hash(block_id);
    }
  }

  void propogate_hashes() {
    for (int node_id = n_leaves - 1; node_id > 0; node_id--) {
      HashType left_hash = node_hashes[node_id * 2];
      HashType right_hash = node_hashes[node_id * 2 + 1];
      node_hashes[node_id] = get_string_hash(left_hash + right_hash);
    }
  }

  static HashType get_string_hash(std::string string_to_hash) {
    const byte* string_data = (byte*) string_to_hash.data();
    unsigned int data_size = string_to_hash.size();
    byte hash[CryptoPP::SHA256::DIGESTSIZE];

    CryptoPP::SHA256().CalculateDigest(hash, string_data, data_size);

    return std::string((char*) hash);
  }

  static HashType get_block_hash(BlockType& block) {
    std::string block_string_repr(block.begin(), block.end());
    return get_string_hash(block_string_repr);
  }

  HashType get_block_hash(int block_id) {
    return get_block_hash(blocks[block_id]);
  }

  void rehash_block(int block_id) {
    int curr_node = n_leaves + block_id;
    node_hashes[curr_node] = get_block_hash(block_id);
    while ((curr_node /= 2) > 0) {
      HashType left_hash = node_hashes[curr_node*2];
      HashType right_hash = node_hashes[curr_node*2 + 1];
      node_hashes[curr_node] = get_string_hash(left_hash + right_hash);
    }
  }

  void tree_init() {
    generate_leaf_hashes();
    propogate_hashes();
  }

  HashTree(const std::vector<HashType>& hashes) {
    for (int block_id = 0; block_id < n_leaves; block_id++) {
      node_hashes[n_leaves + block_id] = hashes[block_id];
    }
    propogate_hashes();
  }

public:

  HashTree():
      n_leaves(FILESYSTEM_SIZE),
      blocks(n_leaves, emptyBlock),
      node_hashes(2*n_leaves) {
    tree_init();
  }

  void write_block(int block_id, BlockType data) {
    blocks[block_id] = data;
    rehash_block(block_id);
  }

  void write_byte(int block_id, int byte_id, byte data) {
    blocks[block_id][byte_id] = data;
    rehash_block(block_id);
  }

  BlockType get_block(int block_id) {
    return blocks[block_id];
  }

  byte get_byte(int block_id, int byte_id) {
    return blocks[block_id][byte_id];
  }

  HashType get_top_hash() {
    return node_hashes[1];
  }

  std::vector<HashType> get_leaf_hashes() {
    return std::vector<HashType>(node_hashes.begin()+n_leaves, node_hashes.end());
  }

  static HashType generate_top_hash_from_init_hashes(const std::vector<HashType>& hashes) {
    HashTree tmp_tree(hashes);
    return tmp_tree.get_top_hash();
  }

  static HashType generate_top_hash_from_data_and_hashes(std::vector<HashType> hashes, BlockType block, int block_id) {
    if (block_id >= hashes.size())
      throw std::out_of_range("invalid block id");
    hashes[block_id] = get_block_hash(block);
    return generate_top_hash_from_init_hashes(hashes);
  }

};

#endif //FILESYSTEM_HASHTREE_H