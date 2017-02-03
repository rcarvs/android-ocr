/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImageMatrix.cpp
 * Author: labpi
 * 
 * Created on 3 de Fevereiro de 2017, 18:39
 */
#include <string>
#include <fstream>
#include "ImageMatrix.h"

ImageMatrix::ImageMatrix() {
}

ImageMatrix::ImageMatrix(const ImageMatrix& orig) {
}

ImageMatrix::~ImageMatrix() {
}

void loadMatrixFromSource(std::string fileName){
    std::fstream inFile;
    inFile.open(fileName, std::fstream::in);
    
}

