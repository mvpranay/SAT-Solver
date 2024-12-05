struct Literal{
    int label;
    bool assigned;
    bool value;

    Literal() : label(0){}

    Literal(int __label){
        label = __label;
        assigned = false;
    }

    Literal(const Literal & other){
        label = other.label;
        assigned = other.assigned;
        value = other.value;
    }

    void assign(bool __value){
        assigned = true;
        value = __value;
    }

    void deassign(){
        assigned = false;
    }
};
