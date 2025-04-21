#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BLOCK_SIZE 4096
#define NUM_BLOCKS 1024
#define NUM_INODES 128

typedef struct {
    int size;                // Size of the file
    int direct_blocks[10];   // Direct block pointers (up to 10 blocks)
} inode;

typedef struct {
    int num_blocks;         // Total number of blocks
    int num_inodes;         // Total number of inodes
    int free_blocks;        // Number of free blocks
    int free_inodes;        // Number of free inodes
    int inode_map[NUM_INODES]; // Inode allocation map
    int block_map[NUM_BLOCKS]; // Block allocation map
} superblock;

superblock sb;               // Superblock instance
inode inodes[NUM_INODES];    // Array of inodes
char disk[NUM_BLOCKS][BLOCK_SIZE]; // Simulated disk

// Initialize the file system
void init_fs() {
    sb.num_blocks = NUM_BLOCKS;
    sb.num_inodes = NUM_INODES;
    sb.free_blocks = NUM_BLOCKS;
    sb.free_inodes = NUM_INODES;
    memset(sb.inode_map, 0, sizeof(sb.inode_map)); // Initialize inode map
    memset(sb.block_map, 0, sizeof(sb.block_map)); // Initialize block map
}

// Allocate an inode
int allocate_inode() {
    for (int i = 0; i < NUM_INODES; i++) {
        if (sb.inode_map[i] == 0) { // Check for free inode
            sb.inode_map[i] = 1;    // Mark inode as allocated
            sb.free_inodes--;
            return i;               // Return allocated inode index
        }
    }
    return -1; // No free inodes
}

// Allocate a block
int allocate_block() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (sb.block_map[i] == 0) { // Check for free block
            sb.block_map[i] = 1;    // Mark block as allocated
            sb.free_blocks--;
            return i;               // Return allocated block index
        }
    }
    return -1; // No free blocks
}

// Create a file with the given name and content
void create_file(const char *filename, const char *content) {
    int inode_index = allocate_inode();
    if (inode_index == -1) {
        printf("No free inodes\n");
        return;
    }

    inode *node = &inodes[inode_index];
    node->size = strlen(content);
    
    // Allocate a block for the file content
    int block_index = allocate_block();
    if (block_index == -1) {
        printf("No free blocks\n");
        return;
    }
    
    node->direct_blocks[0] = block_index; // Assign block to inode
    strcpy(disk[block_index], content);    // Store content in the block
    printf("File '%s' created with inode %d and block %d\n", filename, inode_index, block_index);
}

int main() {
    init_fs(); // Initialize the file system
    create_file("hello.txt", "Hello, World!"); // Create a file
    return 0;
}
