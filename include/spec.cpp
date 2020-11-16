#include "spec.hpp"

//~~~~~~~spec~~~~~~~~

spec::spec(std::string a, list<clique> &cliqueContainer) {
    id = a;
    myClique = new clique(this);
    cliqueContainer.insert(myClique);
}

spec::~spec() {}

std::string spec::getId() { return id; }

clique* spec::getClique() { return myClique; }

void spec::setClique(clique *newClique) { myClique = newClique; }

void spec::merge(spec *second) {
/*merges current spec's clique with the <second>'s one,
  chooses the smaller list to be merged into the larger one 
  to avoid unnecessary computations*/
    if (myClique == second->getClique()) //if they belong to the same clique...
        return; //...do nothing
    
    list<spec> *l1 = myClique->getContentList(), 
            *l2 = second->getClique()->getContentList();

    if (l1->getCount() >= l2->getCount()) //merge the smaller list into the bigger //TODO improve, 1 2 3 -> 1+2+(1+2), 3 2 1 -> 2+1
        myClique->merge(second->getClique());
    else
        second->getClique()->merge(myClique);
}

void spec::print() {
    std::cout << id << std::endl;
}

//~~~~~~~~~~~clique~~~~~~~~~~~

clique::clique(spec* initialSpec) {
    content.insert(initialSpec);
}

clique::~clique() {
    content.emptyList(false);
}

list<spec>* clique::getContentList() { return &content; }

void clique::merge(clique *second) {
/*merges <second> clique into current clique*/
    listNode<spec> *current = second->getContentList()->getStart();
    while (current != NULL) { //change clique in specs
        current->getContent()->setClique(this); 
        current = current->getNext();
    }
    list<spec>::merge(content, *(second->getContentList()));
}

void clique::writePairs(std::ofstream &ofile) {
/*outputs into <ofile> all the pairs of current clique in csv format
ofile must be an ofstream opened outside of here*/
    listNode<spec> *first = content.getStart();
    while (first != NULL) {
        listNode<spec> *second = first->getNext();
        while (second != NULL) {
            if (first == second) {
                std::cerr << "Bad clique in printPairs" << std::endl;
            }
            ofile << first->getContent()->getId() <<
            "," << second->getContent()->getId() << "\n";

            second = second->getNext();
        }

        first = first->getNext();
    }

}