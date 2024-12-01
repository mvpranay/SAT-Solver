class Literal:
    def __init__(self, label : int):
        self.label = label
        self.assigned = False
        self.value = None
    
    def assign(self, value : bool):
        self.assigned = True
        self.value = value
