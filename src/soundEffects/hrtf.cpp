//
//  hrtf.cpp
//  oFHRTF
//
//  Created by keita miyano on 2021/01/17.
//

#include "soundEffects/hrtf.hpp"

hrtf::hrtf(int _size, int _sampleRate) {
    //! サンプルレート
    sampleRate = _sampleRate;
    //! 保持されている音のサンプルサイズ
    size = _size;
    //! バッファ
    buffer.resize(size);
    for (int i = 0; i < buffer.size(); i++) {
        buffer[i] = 0.;
    }
    //! 元のsampleが保持用bufferの今どの位置か
    originalSamplePoint = 0;
    
    hrtfValuesL.resize(72);
    hrtfValuesR.resize(72);
    // HRTFのデータセット読み込み
    for (int i = 0; i < 72; i++) {
        int azimuth = i * 5;
        std::ostringstream sout;
        sout << std::setfill('0') << std::setw(3) << azimuth;
        // hrtfデータベースのファイルパスを取得
        std::string filePathL = "hrtfs/elev0/L0e" + sout.str() + "a.dat";
        std::string filePathR = "hrtfs/elev0/R0e" + sout.str() + "a.dat";
        ofBuffer fileBufferL = ofBufferFromFile(filePathL);
        ofBuffer fileBufferR = ofBufferFromFile(filePathR);
        hrtfValuesL[i].resize(size);
        hrtfValuesR[i].resize(size);
        string _textL = fileBufferL.getText();
        hrtfValuesL[i] = splitString(_textL, '\n');
        string _textR = fileBufferR.getText();
        hrtfValuesR[i] = splitString(_textR, '\n');
    }

}

float hrtf::getSample(float& sampleL, float& sampleR, int azimuth) {
    int index = azimuth / 5;
    float delayTime; // 単位はms
    int point;
    sampleL = 0.;
    sampleR = 0.;
    for (int i = 0; i <  hrtfValuesL[index].size(); i++) {
        delayTime = i * 1000. / 44100.;
        point = originalSamplePoint + size - (delayTime*0.001*sampleRate) - 1;
        if(point >= size){point -= size;}
        if (buffer[point] != 0) {
            sampleL += buffer[point] * hrtfValuesL[index][i] * 3.; // 音が原音に比べて小さすぎるので3倍する
            sampleR += buffer[point] * hrtfValuesR[index][i] * 3.;
        }
    }
}

// sampleを保持用バッファにいれていく
void hrtf::feed(float sample) {
    buffer[originalSamplePoint] = sample;
    originalSamplePoint++;
    // indexがバッファサイズを超えたら0に戻す
    if(originalSamplePoint >= size){ originalSamplePoint = 0;}
}

//文字列delimiterでspitしてvector配列を返すメソッド(文字列を全てfloatに変換する)
vector<float> hrtf::splitString(string& input, char delimiter) {
    istringstream stream(input);
    string field;
    vector<float> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(stof(field));
    }
    return result;
}
