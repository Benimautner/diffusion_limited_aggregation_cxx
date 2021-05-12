#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include <vector>
#include <random>
#include <chrono>
#include "../libs/ThreadPool/ThreadPool.h"
#include "cell_mgr_t.h"

using namespace std;

typedef chrono::high_resolution_clock custom_clock;
typedef chrono::duration<float, std::milli> duration_t;

//TODO: add useful presets to gui
struct config_t {
    int n_nodes = 6000;
    int iterations = 10;
    int horizontal_cells = 80;
    int vertical_cells = 38;
    float radius = 4;
};


class ofApp : public ofBaseApp{

private:
    enum Mode : int {
        STARTUP,
        RUNNING
    };
    Mode current_mode = STARTUP;

    ThreadPool *pool;
    cell_mgr_t cell_mgr;
    ofTrueTypeFont font;

    vector<shared_ptr<node_t>> innocent_vector;
    // vector<node_t> evil_vector;
    std::mutex evil_vector_mutex;
    std::mutex cell_mgr_mutex;
    shared_ptr<node_t> first_node;

    config_t config;

    bool debug_enabled = false;
    bool done = false;
    float rand_range = config.radius / 2;
    float max_rad_of_tree = pow(config.radius, 4);
    custom_clock::time_point start_time;
    custom_clock::time_point end_time;
    duration_t duration = chrono::milliseconds(0);

    //float min_dist = radius * radius * 4;

    ofxPanel gui;
    ofxIntSlider slider_n_nodes;
    ofxIntSlider slider_iterations;
    ofxIntSlider slider_horizontal_cells;
    ofxIntSlider slider_vertical_cells;
    ofxFloatSlider slider_radius;
    ofxButton button_start;

    ofxToggle toggle_enable_respawn;

    void start_sim();


public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void initialize_sim();
};
