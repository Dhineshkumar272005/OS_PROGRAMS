#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BLOCK_SIZE 4096
#define NUM_BLOCKS 1024
#define NUM_INODES 128

// Structure for an inode
typedef struct {
    int size;                // Size of the file
    int direct_blocks[10];   // Direct block pointers (up to 10 blocks)
} inode;

// Structure for the superblock
typedef struct {
    int num_blocks;         // Total number of blocks
    int num_inodes;         // Total number of inodes
    int free_blocks;        // Number of free blocks
    int free_inodes;        // Number of free inodes
    int inode_map[NUM_INODES]; // Inode allocation map
    int block_map[NUM_BLOCKS]; // Block allocation map
} superblock;

// Global variables
superblock sb;               // Superblock instance
inode inodes[NUM_INODES];    // Array of inodes
char disk[NUM_BLOCKS][BLOCK_SIZE]; // Simulated disk

// Function to initialize the file system
void init_fs() {
    sb.num_blocks = NUM_BLOCKS;
    sb.num_inodes = NUM_INODES;
    sb.free_blocks = NUM_BLOCKS;
    sb.free_inodes = NUM_INODES;
    memset(sb.inode_map, 0, sizeof(sb.inode_map)); // Initialize inode map
    memset(sb.block_map, 0, sizeof(sb.block_map)); // Initialize block map
}

// Function to allocate an inode
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

// Function to allocate a block
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

// Function to create a file
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

// Function to read a file
void read_file(int inode_index) {
    if (inode_index < 0 || inode_index >= NUM_INODES || sb.inode_map[inode_index] == 0) {
        printf("File not found\n");
        return;
    }

    inode *node = &inodes[inode_index];
    printf("Reading file with inode %d: %s\n", inode_index, disk[node->direct_blocks[0]]);
}

// Function to delete a file
void delete_file(int inode_index) {
    if (inode_index < 0 || inode_index >= NUM_INODES || sb.inode_map[inode_index] == 0) {
        printf("File not found\n");
        return;
    }

    inode *node = &inodes[inode_index];
    for (int i = 0; i < 10; i++) {
        if (node->direct_blocks[i] != -1) {
            sb.block_map[node->direct_blocks[i]] = 0; // Free the block
            sb.free_blocks++;
        }
    }
    sb.inode_map[inode_index] = 0; // Free the inode
    sb.free_inodes++;
    printf("File with inode %d deleted\n", inode_index);
}

// Main function
int main() {
    init_fs(); // Initialize the file system

    // Create a file
    create_file("file1.txt", "Hello, World!");
    
    // Read the file
    read_file(0); // Read the file with inode 0
    
    // Delete the file
    delete_file(0); // Delete the file with inode 0

    return 0;
}
