//
// Created by Renan Carvalho
//
#include <jni.h>
#include "../../../ParallelUS/runtime/include/parallelus/ParallelUS.hpp"
#include <android/bitmap.h>
#include <parallelocr/ParallelOCR.hpp>
#ifndef OPTICALCHARACTERRECOGNITIONPARALLEL_IMAGE_HPP
#define OPTICALCHARACTERRECOGNITIONPARALLEL_IMAGE_HPP
namespace parallelocr{
class Image {
    public:
        Image(JNIEnv *env,jobject *bitmap,JavaVM *jvm);

        inline unsigned int getHeight(){
            return this->_height;
        };

        JNIEnv* getEnv(){
            return this->_env;
        }

        jobject getBitmap(){
            return *this->_bitmap;
        }

        inline void setHeight(unsigned int height){
            this->_height = height;
        };

        inline unsigned int getWidth(){
            return this->_width;
        };

        inline void setWidth(unsigned int width){
            this->_width = width;
        };

        inline void* getStoredPixels(){
            return this->_storedPixels;
        };

        /*inline void setPixels(Pixel *pixels){
            this->_pixels = pixels;
        }*/
        inline void setStoredPixels(void *storedPixels){
            this->_storedPixels = storedPixels;
        }

        inline void setEnv(JNIEnv *env){
            this->_env = env;
        }

        inline std::shared_ptr<Coach> getCoach(){
            return this->_coach;
        }
        inline std::shared_ptr<parallelus::Runtime> getRuntime(){
            return this->_runtime;
        }
        inline std::shared_ptr<parallelus::Program> getProgram(){
            return this->_program;
        }
        inline void setCoach(std::shared_ptr<Coach> coach){
            this->_coach = coach;
        }
        inline void setRuntime(std::shared_ptr<parallelus::Runtime> runtime){
            this->_runtime = runtime;
        }

        inline void setProgram(std::shared_ptr<parallelus::Program> program){
            this->_program = program;
        }
        inline uint32_t getStride(){
            return this->_stride;
        }
        inline void setStride(uint32_t stride){
            this->_stride = stride;
        }

        inline Letter* getLetters(){
            return this->_letters;
        }

        void setLetters(Letter* letters) {
            this->_letters = letters;
        }

        inline unsigned int getLetterCount(){
            return this->_letterCount;
        }
        inline Letter getLetter(int index){
            return this->_letters[index];
        }

        void setLetterCount(unsigned int letterCount) {
            this->_letterCount = letterCount;
        }



        void bitmapTransformBlackAndWhite();
        void extractPixelsRGB();
        unsigned int createDumblyLabels();
        void relabelAndSearchLetters(unsigned int uplabel);
        void checkLabel(unsigned int index);
        void toLabel();
        JavaVM *_jvm;

    private:
        JNIEnv* _env;
        jobject *_bitmap;
        std::shared_ptr<Coach> _coach;
        std::shared_ptr<parallelus::Runtime> _runtime;
        std::shared_ptr<parallelus::Program> _program;


        unsigned int _height,_width;
        void *_storedPixels;
        uint32_t _stride;
        unsigned int _letterCount;
        //Pixel *_pixels;
        Letter *_letters;



        unsigned int *_r,*_g,*_b,*_label,*_checked;




};
}
#endif //OPTICALCHARACTERRECOGNITIONPARALLEL_IMAGE_HPP
