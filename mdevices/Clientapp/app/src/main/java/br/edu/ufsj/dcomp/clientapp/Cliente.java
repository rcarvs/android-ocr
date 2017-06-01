package br.edu.ufsj.dcomp.clientapp;

/**
 * Created by labpi on 01/06/17.
 */

public class Cliente {
    private long dataPointer = nativeInit();
    private native long nativeInit();


    static {
        System.loadLibrary("Cliente");
    }
}
