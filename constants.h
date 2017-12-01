#ifndef FILESYSTEM_CONSTANTS_H
#define FILESYSTEM_CONSTANTS_H

#include <vector>
#include <string>
#include <cryptopp/config.h>


typedef std::string HashType;
typedef std::vector<byte> BlockType;

constexpr int BLOCK_SIZE = 1<<10;
constexpr int FILESYSTEM_SIZE = 1<<10; //Number of blocks
const BlockType emptyBlock = BlockType(BLOCK_SIZE);


#endif //FILESYSTEM_CONSTANTS_H
