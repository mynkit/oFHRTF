#include "ofApp.h"

#define BUFFERSIZE 256; // バッファサイズ(256推奨．大きくすると処理に余裕はでるが遅延が長くなる)
#define SAMPLERATE 44100; // サンプルレート(Hz)

//--------------------------------------------------------------
void ofApp::setup(){
    
    // audio setting
    bufferSize = BUFFERSIZE;
    sampleRate = SAMPLERATE;
    
    inputBuffer.resize(bufferSize);
    ofSoundStreamSettings soundSettings;
    soundSettings.setOutListener(this);
    soundSettings.setInListener(this);
    soundSettings.sampleRate = sampleRate;
    soundSettings.numOutputChannels=2;
    soundSettings.numInputChannels=1;
    soundSettings.bufferSize = bufferSize;
    sound_stream.setup(soundSettings);
    
    
    // myHrtf2D = new hrtf2D(512, sampleRate);
    myHrtf3D = new hrtf3D(512, sampleRate);
    hrtfDataIndex = 0;
    index = 0.;
    hrtfOn = true;
    
    // 立方体
    soundSource.set(50);
    soundSource.setPosition(0, 0, 0);
    // 球
    microphone.set(50, 16);
    microphone.setPosition(200, 0, 0);
    // カメラ
    const float cameraPositionX = -100;
    const float cameraPositionY = 300;
    const float cameraPositionZ = 200;
    cam.setPosition(cameraPositionX, cameraPositionY, cameraPositionZ);
    cam.lookAt(ofVec3f(-cameraPositionX, -cameraPositionY, -cameraPositionZ), ofVec3f(0, 0, 1));
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // 座標系を移動
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    cam.begin();
    ofSetColor(255);
    // 立方体
    soundSource.draw();
    // 球
    int azimuth = myHrtf3D->getAzimuth((int)hrtfDataIndex);
    int elev = myHrtf3D->getElev((int)hrtfDataIndex);
    microphone.setPosition(200 * sin(M_PI * azimuth / 180.), 200 * cos(M_PI * azimuth / 180.), 200 * sin(M_PI * elev / 180.));
    microphone.drawWireframe();
    // 円(球の軌道)
    ofNoFill();
    ofDrawCircle(0, 0, 0, 200);
    cam.end();
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &buffer){
    const int frames = buffer.getNumFrames();
    for(int i = 0; i < frames; i++){
        inputBuffer[i] = buffer[i];
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer){
    const int frames = buffer.getNumFrames();
    index += 0.01;
    if(index>10){index=0;}
    if (index - (int)index < 0.01) {
        // indexが整数になるタイミングでhrtfDataIndexを0~76のどれか(乱数)にする
        hrtfDataIndex = ofRandom(0, 76);
        cout << hrtfDataIndex << endl;
    }
    
    for(int i = 0; i < frames; i++){
        const int channels = buffer.getNumChannels();
        float currentSample = inputBuffer[i];
        
        float currentSampleL = currentSample;
        float currentSampleR = currentSample;
        if (hrtfOn) {
            // inputの音をそのまま保持用バッファにいれる
            myHrtf3D->feed(currentSample);
            myHrtf3D->getSample(currentSampleL, currentSampleR, hrtfDataIndex);
        }
        buffer[i*channels+0] = currentSampleL;
        buffer[i*channels+1] = currentSampleR;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case 'h':
        if (hrtfOn) {
            hrtfOn = false;
        } else {
            hrtfOn = true;
        }
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
    ofSoundStreamClose();
}
