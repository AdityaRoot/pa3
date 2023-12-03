#include "hashtable.h"
#include "const.h"

// BoardOptimalMove BoardHashTable::getHashedMove(const unsigned long long id, const int depth)
//  Returns the BoardOptimalMove stored in the hash table for the given board ID and search depth.
//  If the board ID does not exist in the table, or the ID is stored but the search depth is lower than the parameter, then return an illegal move indicating that the table does not contain the queried board ID or the stored calculation is not sufficient (if the search depth parameter is equal to or lower than the stored search depth, return the stored move).
BoardOptimalMove BoardHashTable::getHashedMove(const unsigned long long id, const int depth)
{
    // Get the head of the linked list at the index of the table
    BoardHashNode* head = table[id % TABLE_SIZE];
    // Traverse the linked list
    while (head != nullptr) {
        // If the board ID is found
        if (head->id == id) {
            // If the stored depth is lower than the parameter
            if (head->depth < depth) {
                // Return an illegal move
                return BoardOptimalMove();
            }
            // Else, return the stored move
            return head->optimalMove;
        }
        // Move to the next node
        head = head->next;
    }
    // If the board ID is not found, return an illegal move
    return BoardOptimalMove();
}

// void BoardHashTable::updateTable(const unsigned long long id, const int depth, const BoardOptimalMove &optimalMove)
//  Updates the table with the optimal move for the given board ID and search depth.
//  If the board ID does not exist in the table, create a new node at the corresponding linked list (the node can be inserted anywhere in the list).
//  Otherwise, if the stored depth of the board ID is lower than the depth parameter, update the node's depth and optimalMove.
void BoardHashTable::updateTable(const unsigned long long id, const int depth, const BoardOptimalMove& optimalMove)
{
    // Get the head of the linked list at the index of the table
    BoardHashNode* head = table[id % TABLE_SIZE];
    // Traverse the linked list
    while (head != nullptr) {
        // If the board ID is found
        if (head->id == id) {
            // If the stored depth is lower than the parameter
            if (head->depth < depth) {
                // Update the depth and optimal move
                head->depth = depth;
                head->optimalMove = optimalMove;
            }
            // Return
            return;
        }
        // Move to the next node
        head = head->next;
    }
    // If the board ID is not found, create a new node at the head of the linked list
    head = new BoardHashNode(id, depth, optimalMove);
    head->next = table[id % TABLE_SIZE];
    table[id % TABLE_SIZE] = head;
}

// void BoardHashTable::clearTable()
//  Free all dynamic memories in the hash table and reset the linked lists to nullptr.
void BoardHashTable::clearTable(){
// Traverse the table
    for (int i = 0; i < TABLE_SIZE; i++) {
        // Get the head of the linked list
        BoardHashNode* head = table[i];
        // Traverse the linked list
        while (head != nullptr) {
            // Store the next node
            BoardHashNode* next = head->next;
            // Delete the current node
            delete head;
            // Move to the next node
            head = next;
        }
        // Reset the linked list to nullptr
        table[i] = nullptr;
    }
}
