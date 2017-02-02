package br.edu.ufsj.dcomp.ocr;

import android.graphics.Bitmap;

import java.util.ArrayList;

/**
 * Created by Renan de Carvalho on 11/01/17.
 */

public class Controller {

    /*
    * Store the result of native init in this variable for pass this as parameter for other functions in low level
     */
    private long dataPointer = nativeInit();



    /*
    * Native init will start parallelme runtime components
     */
    private native long nativeInit();

    /*
    * Native set image will put the image in jni
     */
    private native void nativeCreateImageLabels(long dataPointer,Bitmap image);



    public void createLabels(Bitmap image){
        this.nativeCreateImageLabels(this.dataPointer,image);
    }

    static {
        System.loadLibrary("OCR");
    }
}
