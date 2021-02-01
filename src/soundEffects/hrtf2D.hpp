//
//  hrtf2D.hpp
//  oFHRTF
//
//  Created by keita miyano on 2021/01/17.
//

#pragma once

#include "ofMain.h"


class hrtf2D {

    public:
        vector<float> buffer;
        int size;
        int sampleRate;
        int originalSamplePoint;
        vector<vector<float>> hrtfValuesL;
        vector<vector<float>> hrtfValuesR;
    
        hrtf2D(int size, int sampleRate);
        ~hrtf2D();
        void feed(float sample);
        void getSample(float& sampleL, float& sampleR, int azimuth);
    
    private:
        vector<float> splitString(string& input, char delimiter);

};
