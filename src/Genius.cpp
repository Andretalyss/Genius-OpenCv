#include "headers/Genius.hpp"
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

Genius::Genius(){

        pont = -1;
        numeroSeq = 0;

}

void Genius::Salvar(string name, int pont)
{
        fstream file;
        file.open("Rank.txt", ios::app);

        if(pont > 0){
        file << "Nome: " << name << " | " << endl;
        file << pont << endl;
        }

        file.close();

}

void Genius::Ler()
{
        std::fstream file;
        file.open("Rank.txt", ios::in);
        string nome;
        int pont;
        vector<string> nomes;
        vector<int> points;

        while(1){
                getline(file,nome);
                cout << nome << endl;
                if(file.eof() || file.fail() || !file.good())
                        break;
                
                file >> pont; 
                cout << pont << endl;
                nomes.push_back(nome);
                points.push_back(pont);
                

        }

        file.close();

        int maior = points[0];
        for(int i=0;i<points.size() && nomes.size();i++){
              //  if(points[i] > maior){
                  //      maior = points[i];
                    cout << nomes[i] << "Score: " << points[i] << endl;
                //}
              //  cout << "Loko" << endl;
        }

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
