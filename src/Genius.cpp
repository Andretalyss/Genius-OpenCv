#include "headers/Genius.hpp"
#include <stdlib.h>
#include <time.h>

Genius::Genius(){
    numeroSeq=0;
}

void Genius::setSequencia(){
    
    srand(time(NULL));
    sequencia.push_back(rand() % 4);
    numeroSeq++; 

}

std::vector<int> Genius::getSequencia(){

    return sequencia;
}

int Genius::getNumeroSequencia(){

    return numeroSeq;
}