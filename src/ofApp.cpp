#include "ofApp.h"

#define N_ARRAY(arr) sizeof(arr)/sizeof(arr[0])

//--------------------------------------------------------------
void ofApp::setup() {
    // oF setup
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofSetWindowPosition(ofGetScreenWidth() / 4, ofGetScreenHeight() / 4);
    ofSetFrameRate(60);

    font.load("ubuntu.tff",40);

    start_time = custom_clock::now();


    float vertical_cell_size = (float) ofGetHeight() / (float) vertical_cells;
    float horizontal_cell_size = (float) ofGetWidth() / (float) horizontal_cells;
    cell_mgr.set_width(vertical_cells);
    cell_mgr.set_height(horizontal_cells);
    int ctr = 0;
    for(int x = 0; x < horizontal_cells; x++) {
        for (int y = 0; y < vertical_cells; ++y) {
            cell_t* new_cell = new cell_t;
            new_cell->id = ctr;
            new_cell->pos[0] = (float) x * horizontal_cell_size;
            new_cell->pos[1] = (float) y * vertical_cell_size;
            new_cell->size[0] = horizontal_cell_size;
            new_cell->size[1] = vertical_cell_size;
            new_cell->pos_in_grid[0] = x;
            new_cell->pos_in_grid[1] = y;
            cell_mgr.add_cell(new_cell);
            ctr++;
        }
    }

    first_node = new node_t;
    first_node->id = 0;
    first_node->pos[0] = ofGetWidth() / 2.0f;
    first_node->pos[1] = ofGetHeight() / 2.0f;
    first_node->stuck = true;

    cell_mgr.add_node(first_node);

    innocent_vector.reserve(n_nodes);
    for (int j = 1; j < n_nodes; ++j) {
        node_t* new_node = new node_t;
        new_node->id = j;
        new_node->pos[0] = ofRandomWidth();
        new_node->pos[1] = ofRandomHeight();
        innocent_vector.emplace_back(new_node);
    }

    pool = new ThreadPool(thread::hardware_concurrency() - 2);
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
    // int num_checking = 0;
    for (int j = 0; j < iterations; ++j) {
        vector<future<int>> thread_results;

        for (int i = 0; i < innocent_vector.size(); i++) {
            node_t* node = innocent_vector[i];
            if(node->stuck)
                continue;
            node->checking = false;

            float x = ofRandom(-rand_range,rand_range);
            float y = ofRandom(-rand_range, rand_range);

            node->vel[0] = x;
            node->vel[1] = y;
            node->pos[0] += node->vel[0];
            node->pos[0] = ofClamp(node->pos[0], radius * 2, win_width - radius * 2);

            node->pos[1] += node->vel[1];
            node->pos[1] = ofClamp(node->pos[1], radius * 2, win_height - radius * 2);


            if(dstSqr(node->pos, first_node->pos) > max_rad_of_tree + min_dist * min_dist)
                continue;
            node->checking = true;
            auto handle_evil_vectors = [&] () {
                //vector<node_t> cell_vector_part;
                int cell_vector_len = 0;
                //vector<node_t*> cell_vector_part;
                auto cell_vector_part = cell_mgr.get_nodes_to_check_by_position(node->pos, cell_vector_len);

                // auto cell_vector_part = cell_mgr.get_all_nodes();
                // int cell_vector_len = cell_vector_part.size();

                for (int i = 0; i < cell_vector_len; i++) {
                    node_t* other = cell_vector_part[i];
                    if(other == nullptr) continue;
                    if(other->id == -1 || node->id == -1 || node->to_be_removed)
                        continue;
                    if(dstSqr(node->pos, other->pos) < min_dist + radius * 2) {
                        cout << "Adding node: " << node->id << endl;
                        evil_vector_mutex.lock();
                        node->stuck = true;
                        node->to_be_removed = true;

                        cell_mgr.add_node(node);
                        max_rad_of_tree = MAX(max_rad_of_tree, dstSqr(node->pos, first_node->pos) + radius * 2);
                        evil_vector_mutex.unlock();
                    }
                }
                cell_vector_part.clear();
                return 0;
            };
            handle_evil_vectors();
            //thread_results.push_back(pool->enqueue(handle_evil_vectors));
        }
        //for(auto & thread_result : thread_results) {
        //    thread_result.wait();
       // }
    }

    for(long unsigned int i = 0; i < innocent_vector.size(); i++) {
        if (innocent_vector[i]->to_be_removed)
            innocent_vector.erase(innocent_vector.begin() + (long) i);
    }
    // cout << "Innocent length: " << innocent_vector.size() << endl << "Evil length: " << evil_vector.size() << endl;
    if(innocent_vector.size() < n_nodes) {
        node_t* new_node = new node_t;
        new_node->id = 2;
        new_node->pos[0] = ofRandomWidth();
        new_node->pos[1] = ofRandomHeight();
        innocent_vector.emplace_back(new_node);
    }
    if(innocent_vector.empty()) {
        done = true;
        if(duration == duration_t(chrono::milliseconds(0))) {
            end_time = custom_clock::now();
            duration = end_time - start_time;
        }
        cout << "It took " << duration.count() << " milliseconds." << endl;
    } else {
        cout << "FPS: " << ofGetFrameRate() << endl;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    for(auto node : innocent_vector) {
        if(node->checking && debug_enabled)
            ofSetColor(0, 255, 0);
        else
            ofSetColor(255);
        ofDrawCircle(node->pos[0], node->pos[1], radius);
    }
    ofSetColor(255,0,0);
    for(auto node : cell_mgr.get_all_nodes()) {
        ofDrawCircle(node->pos[0], node->pos[1], radius);
    }
    if(debug_enabled) {
        ofNoFill();
        ofDrawCircle(first_node->pos[0], first_node->pos[1], sqrt(max_rad_of_tree));

        for(const auto& cell : cell_mgr.get_cell_vector()) {
            ofDrawRectangle(cell->pos[0], cell->pos[1], cell->size[0], cell->size[1]);
        }
        ofFill();
        ofSetColor(ofGetBackgroundColor());
        ofDrawRectangle(0,0,100, 50);
        ofSetColor(255);
        font.drawString(to_string((int)round(ofGetFrameRate())),10,45);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case(0x64): // d
            debug_enabled = !debug_enabled;
            break;
        default:
            cout << "Key: " << key << endl;
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
