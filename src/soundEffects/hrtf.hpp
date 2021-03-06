//
//  hrtf.hpp
//  oFHRTF
//
//  Created by keita miyano on 2021/01/17.
//

#pragma once

#include "ofMain.h"


class hrtf {

    public:
        vector<float> buffer;
        int size;
        int sampleRate;
        int originalSamplePoint;
        vector<vector<float>> hrtfValuesL;
        vector<vector<float>> hrtfValuesR;
    
        hrtf(int _size, int _sampleRate);
        ~hrtf();
        void feed(float sample);
        float getSample(float& sampleL, float& sampleR, int azimuth);
    
    private:
        vector<float> splitString(string& input, char delimiter);

};
