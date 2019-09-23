#ifndef GENIUS_HPP
#define GENIUS_HPP
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>


class Genius {

protected:
std::string name;
int pont;
std::vector<int> sequencia;
int numeroSeq;

public:

Genius();

void setSequencia();
void setName(std::string name);
void setPont();

std::vector<int> getSequencia();
std::string getName();
void Salvar(std::string name, int pont);
void Ler();
int getNumeroSequencia();
int getPont();

};


#endif
