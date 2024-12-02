from literal import Literal

class Clause:
    def __init__(self, literals : list):
        self.clause = literals

        self.assigned = False
        self.value = None

        self.literals = set()

        for literal in self.clause:
            # if negation of literal present in the clause, it is always true
            if -literal in self.literals:
                self.assigned = True
                self.value = True
            # ignore copies of the same literal
            elif literal in self.literals:
                continue
            # add a newly found literal
            else:
                self.literals.add(literal)

        self.unassigned_literals = set(self.literals)

    # checks if a clause is unassigned and has only one unassigned literal
    def isUnitClause(self):
        return self.assigned == False and len(self.unassigned_literals) == 1
    
    # gets an unassigned literal
    def getUnassignedLiteral(self):
        return next(iter(self.unassigned_literals))

    # assigns a literal with a value if present in clause
    def assignLiteral(label : int, value : bool):
        pass