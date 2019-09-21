#ifndef GENIUS_HPP
#define GENIUS_HPP
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>


class Genius {
    protected:
            std::string name;
            double pont;
            std::vector<int> sequencia;
            int numeroSeq;

    public:
        Genius();
        void setSequencia();
        std::vector<int> getSequencia();
        int getNumeroSequencia();
        


};


#endif