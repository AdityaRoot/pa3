//  Notice that unlike the BT/BST examples, we don't want to create copies of the tree or node since each node should correspond to a unique board configuration. Therefore the copy constructors and assignment operators are all deleted. In addition, there is no insert or remove member functions. The tree can only be expanded by calling getOptimalMove() (or getOptimalMoveAlphaBeta()) to explore unexplored nodes, or via getSubTree() if the subtree is unexplored.
//   This class represents the search tree of the minimax algorithm. Its structure is similar to the BT and BST examples given in the course materials: the class contains a single data member which is a pointer to a struct BoardNode. This struct contains a const Board instance and a 2D array of BoardTree representing the subtrees of the current node. A leaf node is an empty node (root == nullptr), which can either mean the node has not been explored, or the board/move represented by that node is illegal.

// Example: Consider a BoardTree representing a 3x3 board with an X played in the middle. This means root->board is the board configuration, and root->subtree is a 3x3 array of BoardTree representing the 9 possible moves of O. If the next step has not been explored, all 9 array elements are default initialized and will be empty leaf nodes. Once we explore this node fully, all array elements will be non-empty, except root->subtree[1][1], since O cannot make a move in the middle cell occupied by X.
#include "boardtree.h"
#include "const.h"

// BoardTree constructor
// Initialize root using the provided board
BoardTree::BoardTree(const Board& board)
{
    root = new BoardNode(board);
}

// BoardTree destructor
BoardTree::~BoardTree()
{
    delete root;
}

// BoardTree::getSubTree
//  Returns a pointer to the sub-tree indicated by the given coordinates. This represents the child node corresponding to the move with the given coordinates. If the sub-tree is empty, you should try to build the sub-tree first (hint: if play() returns true, the new board is valid and can be build) by initializing its root data member accordingly.
//   Note: You can assume this function will never be called on an empty tree (root == nullptr). However, if you would like to call this function in your other implementations, you can add a check to see if the tree is empty - if it is, return nullptr as the tree has no subtrees.

BoardTree* BoardTree::getSubTree(const BoardCoordinate& coords)
{
    // If the tree is empty, return nullptr
    if (isEmpty()) {
        return nullptr;
    }

    // Get the board at the root of the tree

    // Get the board at the root of the tree
    Board board = root->board;

    // Get the sub-tree at the given coordinates
    BoardTree* subTree = &root->subTree[coords.row][coords.col];

    // If the move is invalid, still return a pointer to the subtree but do not build it
    if (!board.play(coords)) {
        subTree->root = nullptr;
        return subTree;
    }

    // If the sub-tree is empty, initialize it
    if (subTree->isEmpty()) {
        subTree->root = new BoardNode(board);
    }

    // Play the move on the board

    // Return a pointer to the sub-tree
    return subTree;
}

// class BoardHashTable {
//     private:
//         struct BoardHashNode {
//             unsigned long long id;
//             int depth;
//             BoardOptimalMove optimalMove;
//             BoardHashNode* next;
//
//             BoardHashNode(const unsigned long long id, const int depth, const BoardOptimalMove& optimalMove):
//                 id(id), depth(depth), optimalMove(optimalMove), next(nullptr) {}
//         };
//
//         BoardHashNode* table[TABLE_SIZE] {nullptr};
//
//         BoardHashTable() = default;
//         ~BoardHashTable() { clearTable(); }
//
//     public:
//         BoardHashTable(const BoardHashTable&) = delete;
//         BoardHashTable& operator=(const BoardHashTable&) = delete;
//
//         // Return the only instance of BoardHashTable
//         static BoardHashTable& getInstance()
//         {
//             static BoardHashTable instance;
//             return instance;
//         }
//
//         // Return the stored BoardOptimalMove for the given id and depth. If it is not stored, return "illegal" (default) BoardOptimalMove.
//         BoardOptimalMove getHashedMove(const unsigned long long id, const int depth);
//
//         // Update the table with the optimal move for the given id and depth.
//         // If id does not exist, create a new linked list node at (id % TABLE_SIZE).
//         // Else, if stored depth is lower, update the optimal move with the parameter. Otherwise, do nothing.
//         void updateTable(const unsigned long long id, const int depth, const BoardOptimalMove& optimalMove);
//
//         // Clear all dynamic memory and reset table array to all nullptr.
//         void clearTable();
// };

// Implement this function using the pseudocode above.
//  The core minimax algorithm function. Returns a BoardOptimalMove struct representing the best possible score the current player can reach, and the corresponding move that can reach that score, as calculated up to the given depth. You may refer to the following pseudocode:
BoardOptimalMove BoardTree::getOptimalMove(const unsigned int depth)
{
    // Get instance of hashtable
    BoardHashTable& hashTable = BoardHashTable::getInstance();

    //  if (tree is empty) {
    //   return BoardOptimalMove(); // Returns a dummy illegal move
    // }

    if (root == nullptr) {
        return BoardOptimalMove();
    }

    // if (depth == 0 || board is finished) {
    // If depth is 0 or if the game has already finished, we cannot search further
    // return the score of this board with any move coordinate since we will not use it
    // return BoardOptimalMove(board score, any coordinates);
    // }
    if (depth == 0 || root->board.isFinished()) {
        return BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0, 0));
    }

    // ADDED: Implement hashtable feature to speed up function
    // if (board and depth are in hashtable) {
    // return the optimal move stored in the hashtable }
    BoardOptimalMove hashedMove = hashTable.getHashedMove(root->board.getID(), depth);
    if (hashedMove.score != ILLEGAL) {
        return hashedMove;
    }

    // Else, we find the estimated score and optimal move of this node by calculating the score of each children node
    // Player X is trying to maximize the score, so the estimated score is the maximum of children scores
    // Vice versa, player O is trying to minimize the score

    // int estimatedScore = -∞ if current player is X, else +∞;

    int estimatedScore = (root->board.getCurPlayer() == X) ? -50000 : 50000;

    // BoardOptimalMove bestMove;
    BoardOptimalMove bestMove;
    // The function should work for any BOARD_SIZE
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            BoardTree* subtree = getSubTree(BoardCoordinate(i, j));
            if (subtree->root == nullptr) {
                continue;
            }
            BoardOptimalMove childMove = subtree->getOptimalMove(depth - 1);
            if (childMove.score == ILLEGAL) {
                continue;
            }
            // std::cout << "Player: " << root->board.getCurPlayer() << std::endl
            //           << " Row, Col: " << i << ", " << j << std::endl;
            // std::cout << "childMove.score: " << childMove.score << std::endl;
            // std::cout << "estimatedScore: " << estimatedScore << std::endl;
            if (root->board.getCurPlayer() == X) {
                if (childMove.score > estimatedScore) {
                    // std::cout << "X_SCORE TRIGGERED FOR: " << childMove.score << "and " << estimatedScore << std::endl;
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
                }
            } else {
                if (childMove.score < estimatedScore) {
                    // std::cout << "O_SCORE TRIGGERED FOR: " << childMove.score << "and " << estimatedScore << std::endl;
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
                }
            }
        }
    }

    // ADDED: Store the optimal move in the hashtable
    hashTable.updateTable(root->board.getID(), depth, bestMove);

    return bestMove;
}

// Alpha-beta pruning
// In this case, we keep track of the "best score found by the opponent"
// Implement an alternative version of getOptimalMove() that uses alpha-beta pruning.
// Do this by keeping track of two extra variables: alpha and beta.
// You can copy the getoptimalmove function and edit it sa per the pseudocode below.
// DO NOT use transposition tables for this part.
// Alpha is the best score found by the maximizing player (X) so far.
// Beta is the best score found by the minimizing player (O) so far.
// Depending on the current player, if a child node is found to have a "worse" score for the opponent, we can exit early.
// Here is the modified pseudocode:
//
// ...

// Else, we find the estimated score and optimal move of this node by calculating the score of each children node
// int estimatedScore = -∞ if current player is X, else +∞;
// BoardOptimalMove bestMove;
// for each subtree {
//   build the subtree if it is empty;
//
//   // MODIFIED: Call the alpha-beta pruning variant
//   BoardOptimalMove childMove = subtree.getOptimalMoveAlphaBeta(depth - 1, alpha, beta);
//
//   ...
//
//   // ADDED: Update alpha and beta according to newest child score found
//   if (current player is X) {
//     if (optimalScore > beta) {
//       // Found a move that would be worse for O, so O will not consider this subtree
//       exit loop;
//     }
//     // Update the best move found by X
//     alpha = max(optimalScore, alpha);
//   }
//   else {
//     // Same logic but for current player O
//     if (optimalScore < alpha) {
//       exit loop;
//     }
//     beta = min(optimalScore, beta);
//   }
// }
//
// return bestMove;
//
//
// Note that, we exit the loop if a "worse" board state for the opponent is found.
// Careful when using 'break' as it will only break from 1 for loop

BoardOptimalMove BoardTree::getOptimalMoveAlphaBeta(const unsigned int depth, int alpha, int beta)
{
    //  if (tree is empty) {
    //   return BoardOptimalMove(); // Returns a dummy illegal move
    // }

    if (root == nullptr) {
        return BoardOptimalMove();
    }

    // if (depth == 0 || board is finished) {
    // If depth is 0 or if the game has already finished, we cannot search further
    // return the score of this board with any move coordinate since we will not use it
    // return BoardOptimalMove(board score, any coordinates);
    // }
    if (depth == 0 || root->board.isFinished()) {
        return BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0, 0));
    }

    // Else, we find the estimated score and optimal move of this node by calculating the score of each children node
    // Player X is trying to maximize the score, so the estimated score is the maximum of children scores
    // Vice versa, player O is trying to minimize the score

    // int estimatedScore = -∞ if current player is X, else +∞;

    int estimatedScore = (root->board.getCurPlayer() == X) ? -50000 : 50000;

    // BoardOptimalMove bestMove;
    BoardOptimalMove bestMove;
    // The function should work for any BOARD_SIZE
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            BoardTree* subtree = getSubTree(BoardCoordinate(i, j));
            if (subtree->root == nullptr) {
                continue;
            }
            // MODIFIED: Call the alpha-beta pruning variant
            BoardOptimalMove childMove = subtree->getOptimalMoveAlphaBeta(depth - 1, alpha, beta);

            if (childMove.score == ILLEGAL) {
                continue;
            }
            if (root->board.getCurPlayer() == X) {
                if (childMove.score > estimatedScore) {
                    // std::cout << "X_SCORE TRIGGERED FOR: " << childMove.score << "and " << estimatedScore << std::endl;
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
                }
            } else {
                if (childMove.score < estimatedScore) {
                    // std::cout << "O_SCORE TRIGGERED FOR: " << childMove.score << "and " << estimatedScore << std::endl;
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
                }
            }
        // ADDED: Update alpha and beta according to newest child score found
        if (root->board.getCurPlayer() == X) {
            if (bestMove.score > beta) {
                // Found a move that would be worse for O, so O will not consider this subtree
                return bestMove;
            }
            // Update the best move found by X
            alpha = std::max(bestMove.score, alpha);
        } else {
            // Same logic but for current player O
            if (bestMove.score < alpha) {
                return bestMove;
            }
            beta = std::min(bestMove.score, beta);
        }
        }
    }

    return bestMove;
}
