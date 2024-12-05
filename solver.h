#include "clause.h"
#include <string>

class Solver{
    // pointers to clauses
    std::vector<Clause *> clauses;
    // only positive versions
    std::set<int> labels;
    std::shared_ptr<std::map<int, Literal>> literals;

    bool done;
    
    // literal -> level at which it was assigned    
    std::map<int, int> decision_level;

    int current_decision_level = 0;

    // stores what decision
    std::map<int, std::set<int>> decision_sources;

public:
    std::string result;
    Solver(){
        done = false;
        literals = std::make_shared<std::map<int, Literal>>();
    }

    ~Solver(){
        for (Clause * c : clauses){
            delete c;
        }
    }

    void addClause(std::vector<int> atoms){
        // Clause new_clause(literals);
        int size = atoms.size();
        
        // don't allow addition of an empty class
        if (size == 0) return;

        Clause * new_clause = new Clause(literals);

        // first add to labels if not present
        for (int atom : atoms){
            int pos = (atom > 0) ? atom : -atom;

            // found a new literal
            if (labels.contains(pos) == false){
                labels.insert(pos);
                literals->emplace(pos, Literal(pos));
            }

            new_clause->addAtom(atom);
        }

        clauses.push_back(new_clause);
    }

    // gets the index of unit clause in vector clauses
    int getUnitClause(){
        int nclauses = clauses.size();
        for (int i = 0; i < nclauses; i++){
            if (clauses[i]->isUnitClause())
                return i;
        }

        return -1;
    }

    // check if all clauses have been assigned the value true
    bool allClausesSatisfied(){
        for (Clause * c : clauses){
            if (c->assigned == false || c->value == false)
                return false;
        }
        return true;
    }

    void makeDecision(){
        for (auto & literal_pair : * literals){
            if (literal_pair.second.assigned == false){
                assignLiteral(literal_pair.first, true);
                current_decision_level++;
                return;
            }
        }
    }

    // assigns the literal to the value in every clause, and recalculates status
    void assignLiteral(int label, bool value){
        Literal & L = (*literals)[label];
        L.assign(value);

        for (Clause * c : clauses){
            c->assignAtom(label);
            c->recalculateStatus();
        }

        decision_level[label] = current_decision_level;
        return;
    }

    void deassignLiteral(int label){
        Literal & L = (*literals)[label];
        L.deassign();

        for (Clause * c : clauses){
            c->deassignAtom(label);
            c->recalculateStatus();
        }
        // decision_level.erase(label); 
    }

    void backtrack(int backtrack_level){
        // erase all assignments with level > backtrack_level
        std::set<int> to_deassign;

        for (auto it = decision_level.begin(); it != decision_level.end(); it++){
            if (it->second > backtrack_level){
                deassignLiteral(it->first);
                to_deassign.insert(it->first);
            }
        }

        for (int literal : to_deassign)
            decision_level.erase(literal);

        // recalculate status
        for (Clause * c : clauses)
            c->recalculateStatus();
        
        current_decision_level = backtrack_level;
    }

    Clause * analyzeConflict(Clause * conflict_clause){
        std::set<int> learnt_atoms;

        for (int atom : conflict_clause->atoms){
            learnt_atoms.insert(decision_sources[std::abs(atom)].begin(), decision_sources[std::abs(atom)].end());
        }

        Clause * learnt_clause = new Clause(literals);
        for (int atom : learnt_atoms){
            learnt_clause->addAtom(-atom);
        }

        if (learnt_clause == nullptr || learnt_clause->atoms.empty()){
            delete learnt_clause;
            return nullptr;
        }

        return learnt_clause;
    }

    void checkSAT(){
        while (done == false){
            // assign the unit clauses
            int index_unit_clause = getUnitClause();

            while(index_unit_clause != -1){
                // assign the unassigned literals in the unit clauses
                int unassigned = clauses[index_unit_clause]->getUnassignedAtom();

                // need to assign true to make clause true
                if (unassigned > 0){
                    (*literals)[unassigned].assign(true);
                }
                // need to assign false to make clause true
                else{
                    (*literals)[-unassigned].assign(false);
                }

                // set decision_sources
                // decision_sources[unassigned].clear();
                
                for (int atom : clauses[index_unit_clause]->atoms){
                    if (atom != unassigned){
                        decision_sources[unassigned].insert(decision_sources[atom].begin(), decision_sources[atom].end());
                    }
                }

                // recalculate status and check if any clause was assigned false
                for (Clause * c : clauses){
                    c->assignAtom(unassigned);
                    c->recalculateStatus();

                    // conflict here
                    if (c->assigned == true && c->value == false){
                        Clause * learnt_clause = analyzeConflict(c);

                        if (learnt_clause == nullptr){
                            done = true;
                            result = "UNSAT";
                            return;
                        }

                        // add the new clause
                        clauses.push_back(learnt_clause);

                        // find the backtrack_level(second highest level)
                        std::set<int> levels;
                        for (int atom : learnt_clause->atoms)
                            levels.insert(decision_level[atom]);
                        levels.erase(--levels.end());

                        int backtrack_level = * levels.rbegin();

                        backtrack(backtrack_level);
                        break;
                    }
                }

                index_unit_clause = getUnitClause();
            }  

            if (allClausesSatisfied()){
                done = true;
                result = "SAT";
                return;
            }

            makeDecision();
        }
    }
};