#include "ofApp.h"

#define N_ARRAY(arr) sizeof(arr)/sizeof(arr[0])

//--------------------------------------------------------------
void ofApp::setup() {
    // oF setup
    ofSetWindowShape(ofGetScreenWidth() / 2, ofGetScreenHeight() / 2);
    ofSetWindowPosition(ofGetScreenWidth() / 4, ofGetScreenHeight() / 4);
    ofSetFrameRate(60);

    first_node.pos[0] = ofGetWidth() / 2.0f;
    first_node.pos[1] = ofGetHeight() / 2.0f;
    first_node.stuck = true;
    evil_vector.emplace_back(first_node);
    innocent_vector.reserve(n_nodes);
    for (int j = 0; j < n_nodes; ++j) {
        node_t new_node;

        new_node.pos[0] = ofRandomWidth();
        new_node.pos[1] = ofRandomHeight();

        innocent_vector.emplace_back(new_node);
    }
}
//--------------------------------------------------------------

float dstSqr(const float v1[], const float v2[]) {
    // if(v1_size < 2 || v2_size < 2) return MAXFLOAT;
    // float p1 = v2[0]-v1[0];
    // float p2 = v2[1]-v1[1];
    // return (   p1       *       p1      +       p2      *       p2      );
    return ( (v2[0] - v1[0]) * (v2[0] - v1[0]) + (v2[1] - v1[1]) * (v2[1] - v1[1]) );
}


void ofApp::update(){
    const auto win_width = (float) ofGetWidth();
    const auto win_height = (float) ofGetHeight();
    int num_checking = 0;
    for (int j = 0; j < iterations; ++j) {
        for (auto &node : innocent_vector) {
            if(node.stuck)
                continue;
            node.checking = false;

            float x = ofRandom(-rand_range,rand_range);
            float y = ofRandom(-rand_range, rand_range);
            node.vel[0] = x;
            node.vel[1] = y;
            node.pos[0] += node.vel[0];
            node.pos[0] = ofClamp(node.pos[0], radius * 2, win_width - radius * 2);

            node.pos[1] += node.vel[1];
            node.pos[1] = ofClamp(node.pos[1], radius * 2, win_height - radius * 2);


            if(dstSqr(node.pos, first_node.pos) > max_rad_of_tree + min_dist * min_dist)
                continue;
            node.checking = true;
            for (auto &other : evil_vector) {
                if(dstSqr(node.pos, other.pos) < min_dist) {
                    node.stuck = true;
                    node.to_be_removed = true;
                    evil_vector.emplace_back(node);
                    max_rad_of_tree = MAX(max_rad_of_tree, dstSqr(node.pos, first_node.pos) + radius * 2);
                }
            }
        }
    }

    for(int i = 0; i < innocent_vector.size(); i++) {
        if (innocent_vector[i].to_be_removed)
            innocent_vector.erase(innocent_vector.begin() + i);
    }
    // cout << "Innocent length: " << innocent_vector.size() << endl << "Evil length: " << evil_vector.size() << endl;
    cout << "FPS: " << ofGetFrameRate() << endl;

}

//--------------------------------------------------------------
void ofApp::draw(){
    for(auto node : innocent_vector) {
        if(node.checking)
            ofSetColor(0, 255, 0);
        else
            ofSetColor(255);
        ofDrawCircle(node.pos[0], node.pos[1], radius);
    }
    ofSetColor(255,0,0);
    for(auto node : evil_vector) {
        ofDrawCircle(node.pos[0], node.pos[1], radius);
    }
    ofNoFill();
    ofDrawCircle(first_node.pos[0], first_node.pos[1], sqrt(max_rad_of_tree));
    ofFill();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    cout << "Key: " << key << endl;
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
