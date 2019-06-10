#ifndef CLASSIFIER_EXCEPTION_H
#define CLASSIFIER_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

#define DEFAULT_MESS "Classfier exception accured"

class Classfier_except : public std::exception {
    private:
        std::string mess;
    public:
        Classfier_except() : mess(DEFAULT_MESS) {}
        Classfier_except(const char * messege) :mess(messege) {}
        Classfier_except(std::string str): mess(str) {}
        virtual const char* what() const noexcept { 
            return mess.c_str(); 
        }
};

#endif
