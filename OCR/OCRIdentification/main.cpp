/* 
 * File:   main.cpp
 * Author: Renan Carvalho
 * 
 */
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <iostream>
#include <CL/cl.h>
#include "FeatureExtraction.h"
#include "ImageMatrix.h"
using namespace std;
int main() {
    ImageMatrix imageMatrix;    
    std::string fileName = "out.txt";
    imageMatrix.loadMatrixFromSource(fileName.);
    
    
    return 0;
}

