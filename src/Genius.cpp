#include "headers/Genius.hpp"
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

bool compare_rank(Player a, Player b)
{
        return a.score > b.score;

} // end compare_insertion

Genius::Genius(){

        pont = -1;
        numeroSeq = 0;

}

void Genius::Salvar(string name, int pont)
{
        fstream file;
        file.open("files/Rank.txt", ios::app);

        if(pont > 0) {

                file << name << endl;
                file << pont << endl;
        }

        file.close();

}

void Genius::Ler()
{
        std::fstream file;
        file.open("files/Rank.txt", ios::in);
        std::string line;

        Player player;

        while(file.peek() != EOF)
        {
                int i = 0;

                std::getline(file, line);
                player.name = line;

                std::getline(file, line);
                player.score = stoi(line);

                rank.push_back(player);

                i++;

        } // end while

        sort(rank.begin(), rank.end(), compare_rank);

        file.close();

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

void Genius::setRankName(int i, std::string name)
{
        this->rank[i].name = name;

} // end setRankName

void Genius::setRankScore(int i, int score)
{

        this->rank[i].score = score;

} // end setRankScore

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

std::string Genius::getRankName(int i)
{
        return this->rank[i].name;

} // end getRankName

int Genius::getRankScore(int i)
{
        return this->rank[i].score;

} // end getRankScore
