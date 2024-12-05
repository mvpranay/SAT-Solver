#include "solver.h"
#include <iostream>

int main() {
    // Create a Solver instance
    Solver solver;

    // Add some test clauses
    std::cout << "Adding test clauses..." << std::endl;

    // Clause 1: x1 OR x2
    solver.addClause({1, 2}); // {1,2}

    // Clause 2: NOT x1 OR x3
    solver.addClause({-1, 2}); // {-1, 3}

    // Clause 3: NOT x2 OR NOT x3
    solver.addClause({1, -2}); // {-2, -3}

    // Clause 4: x1
    solver.addClause({-1, -2}); // {1}

    // Print initial state
    std::cout << "Checking SAT..." << std::endl;

    // Perform SAT solving
    solver.checkSAT();

    // Display result
    std::cout << "Result: " << solver.result << std::endl;

    return 0;
}
