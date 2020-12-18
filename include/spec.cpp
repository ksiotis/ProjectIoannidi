#include "spec.hpp"

//~~~~~~~spec~~~~~~~~

spec::spec(std::string a, list<clique> &cliqueContainer)
:generic(a) {
    myClique = new clique(this);
    cliqueContainer.insert(myClique);
}

spec::~spec() {}

// std::string spec::getId() { return id; }

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

    clique *merger, *mergee;
    if (l1->getCount() >= l2->getCount()) { //merge the smaller list into the bigger
        merger = myClique;
        mergee = second->getClique();
    }
    else {
        merger = second->getClique();
        mergee = myClique;
    }
    merger->merge(mergee);
}

void spec::unsimilar(spec *second) {
    /*adds the clique of one to the negative part of the other
    and the other way around*/
    if (myClique == second->getClique()) { //if they belong to the same clique...
        std::cerr << "Called unsimilar on specs that are similar!" << std::endl;
        return;
    }

    //TODO remove
    // if (id == "www.ebay.com//25120" || second->getId() == "www.ebay.com//25120") {
    //     std::cout << "yas";
    // }
    myClique->addNegative(second->getClique());
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
    negative.emptyList(false);
}

list<spec>* clique::getContentList() { return &content; }

list<clique>* clique::getNegativeList() { return &negative; }

void clique::merge(clique *second) {
/*merges <second> clique into current clique*/

    //go to each clique in the mergee(second) and remove the old negative cliques and add the new one
    listNode<clique> *currentListNode = second->getNegativeList()->getStart();
    while (currentListNode != NULL) {
        list<clique> *currentNegativeList = currentListNode->getContent()->getNegativeList();
        currentNegativeList->remove(this, second);
        currentNegativeList->insert(this);

        //TODO remove debugging
        listNode<clique> *tempNode = currentNegativeList->getStart();
        int i = 0;
        while (tempNode != NULL) {
            i++;
            tempNode = tempNode->getNext();
        }
        if (currentNegativeList->getCount() != i) {
            std::cout << "I WANNA DIE" << std::endl;
        }

        currentListNode = currentListNode->getNext();
    }

    // //go to each clique in the merger(this) and remove the mergee //TODO if things go bad, this is to be uncommented
    // currentListNode = negative.getStart();
    // while (currentListNode != NULL) {
    //     list<clique> *currentNegativeList = currentListNode->getContent()->getNegativeList();
    //     currentNegativeList->remove(second);
    // }

    //insert elements from the second list to the first if they dont exist already
    listNode<clique> *currentN = second->getNegativeList()->getStart();
    while (currentN != NULL) {
        if (!negative.search(currentN->getContent()))
            negative.insert(currentN->getContent());
        currentN = currentN->getNext();
    }
    
    listNode<spec> *current = second->getContentList()->getStart();
    while (current != NULL) { //change clique in specs
        current->getContent()->setClique(this); 
        current = current->getNext();
    }
    list<spec>::merge(content, *(second->getContentList()));
}

void clique::addNegative(clique *second) { //TODO clean debuging
    bool secondInFirst = negative.search(second);
    bool firstInSecond = second->getNegativeList()->search(this);

    if (!secondInFirst) { //if second is not already inside this
        negative.insert(second);
    }
    if (!firstInSecond) { //if this is not already inside second
        second->getNegativeList()->insert(this);
    }

    //debugging
    if (firstInSecond != secondInFirst) {
        std::cerr << "One clique is unsimilar to the other without the opposite" << std::endl;
    } //if this is never shown, further optimization is possible
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