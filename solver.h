#include "clause.h"

class Solver{
public:
    Solver(){
        done = false;
        literals = std::make_shared<std::map<int, Literal>>();
    }

    void addClause(std::vector<int> atoms){
        Clause new_clause(literals);

        // first add to labels if not present
        for (int atom : atoms){
            int pos = (atom > 0) ? atom : -atom;

            // found a new literal
            if (labels.contains(pos) == false){
                labels.insert(pos);
                literals->emplace(pos, Literal(pos));
            }

            new_clause.addAtom(atom);
        }

        clauses.push_back(new_clause);
    }

    // check if all clauses have been assigned the value true
    void checkDone(){
        for (Clause c : clauses){
            if (c.assigned == false || c.value == false){
                done = false;
                return;
            }
        }

        done = true;
        return;
    }

    // gets the index of unit clause in vector clauses
    int getUnitClause(){
        int nclauses = clauses.size();
        for (int i = 0; i < nclauses; i++){
            if (clauses[i].isUnitClause())
                return i;
        }

        return -1;
    }

    // assigns the literal to the value in every clause, and recalculates status
    void assignLiteral(int label, bool value){
        Literal L = (*literals)[label];
        L.assign(value);

        for (Clause c : clauses){
            c.assignAtom(label, value);
            c.recalculateStatus();
        }

        return;
    }

    void deassignLiteral(int label){
        Literal L = (*literals)[label];
        L.deassign();

        for (Clause c : clauses){
            c.deassignAtom(label);
            c.recalculateStatus();
        }
    }

private:
    std::vector<Clause> clauses;
    // only positive versions
    std::set<int> labels;
    std::shared_ptr<std::map<int, Literal>> literals;

    bool done;
    
    // literal's label -> value chosen
    std::vector<std::pair<int, bool>> decisions;

};