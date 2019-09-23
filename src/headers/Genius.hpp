#ifndef GENIUS_HPP
#define GENIUS_HPP
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>

struct Player
{

std::string name;
int score;

};

class Genius {

protected:

std::string name;
int pont;
std::vector<int> sequencia;
int numeroSeq;

public:

std::vector<Player> rank;


Genius();

void setSequencia();
void setName(std::string name);
void setPont();
void setRankName(int i, std::string name);
void setRankScore(int i, int score);

std::vector<int> getSequencia();
std::string getName();
void Salvar(std::string name, int pont);
void Ler();
int getNumeroSequencia();
int getPont();
std::string getRankName(int i);
int getRankScore(int i);

};


#endif
