#include <iostream>
#include <stack>
#include <vector>
#include <memory>
#include <cmath>


enum class Direction {LEFT, RIGHT, NA};

// Ring class
class Ring {
    public:
        int size;
        Direction dir; // This is the direction the ring is moving in

        Ring(int size) : size{size}, dir{Direction::NA} {}

};

// Post class
class Post {
    public:
        std::stack<std::shared_ptr<Ring>> disks;

        // Constructor
        Post() {}   
        Post(int n) {
            for(int i = 0; i < n; ++i) {
                disks.push(std::make_shared<Ring>(n-i));
            }
        }
        // Destructor
        ~Post() {}

        //Add Ring function
        void addRing(std::shared_ptr<Ring> ring) {
            disks.push(ring);
        }

        //Remove and return the first ring in on the pole
        std::shared_ptr<Ring> removeRing() {
            std::shared_ptr<Ring> temp = disks.top();
            disks.pop();
            return temp;
        }

        // Returns true if post has all rings
        bool full(int total) {
            if (disks.size() == total) return true;
            return false;
        }

        // Print function
        friend std::ostream& operator<<(std::ostream &out, Post &p) {
            std::stack<std::shared_ptr<Ring>> temp = p.disks;
            out << "This Post contains disks of size: ";
            while (!temp.empty()) {
                out << temp.top()->size << " ";
                temp.pop();
            }
            return out;
        }

};

// Helper functions
std::shared_ptr<Post> minTower(std::vector<std::shared_ptr<Post>> &posts, 
                                                            std::shared_ptr<Ring> &lastRing) {
    if (lastRing == nullptr) return posts[0];

    int p1 = (posts[0]->disks.empty()) ? -1 : posts[0]->disks.top()->size;
    int p2 = (posts[1]->disks.empty()) ? -1 : posts[1]->disks.top()->size;
    int p3 = (posts[2]->disks.empty()) ? -1 : posts[2]->disks.top()->size;

    if (lastRing->size == p1) {
        if (p2 == -1) return posts[2];
        if (p3 == -1) return posts[1];
        return (p2 < p3) ? posts[1] : posts[2];
    } else if (lastRing->size == p2) {
        if (p1 == -1) return posts[2];
        if (p3 == -1) return posts[0];
        return (p1 < p3) ? posts[0] : posts[2];
    } else {
        if (p1 == -1) return posts[1];
        if (p2 == -1) return posts[0];
        return (p1 < p2) ? posts[0] : posts[1];
    }
}


int main(int argc, char* argv[]) {
    int moveCnt = 0;
    try {
        int numORings = std::stoi(argv[1]);
        // Make a vector of posts
        std::vector<std::shared_ptr<Post>> posts = {std::make_shared<Post>(numORings),
                                                    std::make_shared<Post>(),
                                                    std::make_shared<Post>()};

        std::shared_ptr<Ring> lastmoved;
        std::shared_ptr<Post> currTower;
        // Solve game
        while (!posts[2]->full(numORings)) {
            currTower = minTower(posts, lastmoved);
            lastmoved = currTower->disks.top();

            if (lastmoved->dir == Direction::NA) {
                if ((currTower->disks.size() % 2) == 0) {
                    lastmoved->dir = Direction::RIGHT;
                } else {
                    lastmoved->dir = Direction::LEFT;
                }
            }
            if (lastmoved->dir == Direction::LEFT) {
                if (currTower == posts[0]) posts[2]->addRing(currTower->removeRing());
                else if (currTower == posts[1]) posts[0]->addRing(currTower->removeRing());
                else posts[1]->addRing(currTower->removeRing());
            } else if (lastmoved->dir == Direction::RIGHT) {
                if (currTower == posts[0]) posts[1]->addRing(currTower->removeRing());
                else if (currTower == posts[1]) posts[2]->addRing(currTower->removeRing());
                else posts[0]->addRing(currTower->removeRing());
            }

            ++moveCnt;
            std::cout << "Move " << moveCnt << std::endl;
            std::cout << *posts[0] << std::endl;
            std::cout << *posts[1] << std::endl;
            std::cout << *posts[2] << std::endl;
        }
    } catch (...) {
        std::cerr << "Invalid Arguments (exited with error code 1)" << std::endl;
        return 1;
    }

    int minMoves = std::pow(2, std::stoi(argv[1])) - 1;
    std::cout << "The total numbers of moves used was: " << moveCnt << std::endl;
    std::cout << "The minimum number of moves needed was: " << minMoves << std::endl; 
    return 0;
}
