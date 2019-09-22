#include "headers/Genius.hpp"
#include <stdlib.h>
#include <time.h>

Genius::Genius(){

        pont = -1;
        numeroSeq = 0;

}

void Genius::setSequencia(){

        int valor;

        valor = (rand() % 4) + 1;

        if(sequencia.size() > 0)
        {
                while(valor == sequencia.back())
                {
                        valor = (rand() % 4) + 1;

                } // end while

        } // end if

        sequencia.push_back(valor);
        numeroSeq++;

}

void Genius::setName(std::string name)
{

        this->name = name;

}

void Genius::setPont()
{
        this->pont++;

} // end setPont

std::vector<int> Genius::getSequencia(){

        return sequencia;
}

std::string Genius::getName()
{

        return name;

}

int Genius::getNumeroSequencia(){

        return numeroSeq;
}

int Genius::getPont()
{
        return pont;

} // end getPont
