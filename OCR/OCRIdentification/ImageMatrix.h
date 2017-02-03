/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImageMatrix.h
 * Author: labpi
 *
 * Created on 3 de Fevereiro de 2017, 18:39
 */

#ifndef IMAGEMATRIX_H
#define IMAGEMATRIX_H


class ImageMatrix {
public:
    ImageMatrix();
    ImageMatrix(const ImageMatrix& orig);
    void loadMatrixFromSource(std::string fileName);
    void setMatrix(unsigned int* m,unsigned int w,unsigned int h);
    int* getMatrix();
    virtual ~ImageMatrix();
private:
    //matrix store the label matrix
    unsigned int* matrix;
    unsigned int height,width;
};

#endif /* IMAGEMATRIX_H */

