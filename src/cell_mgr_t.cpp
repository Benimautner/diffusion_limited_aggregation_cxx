//
// Created by beni on 10.05.21.
//

#include "cell_mgr_t.h"

int cell_mgr_t::get_index(int x, int y){
    return y + width * x;
}

int cell_mgr_t::get_id_by_position(pos_t *pos) {

    return -1;
}

void cell_mgr_t::set_width(int width) {
    this->width = width;
}
void cell_mgr_t::set_height(int height) {
    this->height = height;
}


vector<shared_ptr<cell_t>> cell_mgr_t::get_cell_vector() {
    return cell_vector;
}

bool cell_mgr_t::get_cell_by_position(pos_t *pos, shared_ptr<cell_t>& return_cell) {

    float x_pos_scale_0_1 = pos[0] / (float)ofGetWidth();
    float y_pos_scale_0_1 = pos[1] / (float)ofGetHeight();

    int x = floor(x_pos_scale_0_1 * (float)height);
    int y = floor(y_pos_scale_0_1 * (float)width);
    int index = get_index(x, y);
    if(index > cell_vector.size())
        return false;
    return_cell = cell_vector[index];
    if (pos[0] > return_cell->pos[0] && pos[0] <= return_cell->pos[0] + return_cell->size[0] &&
        pos[1] > return_cell->pos[1] && pos[1] <= return_cell->pos[1] + return_cell->size[1]) {
        return true;
    }
    cout << "something went wrong. brute-force searching for it" << endl;
    for (auto& cell : cell_vector) {
        if (pos[0] > cell->pos[0] && pos[0] <= cell->pos[0] + cell->size[0] &&
            pos[1] > cell->pos[1] && pos[1] <= cell->pos[1] + cell->size[1]) {
            return_cell = cell;
            return true;
        }
    }
    return false;
}

vector<shared_ptr<node_t>> cell_mgr_t::get_nodes_to_check_by_position(pos_t *pos, int& len) {
    auto containing_cell = make_shared<cell_t>();
    vector<shared_ptr<cell_t>> cells_to_check;
    cells_to_check.reserve(9);

    vector<shared_ptr<node_t>> return_vector;
    bool result = get_cell_by_position(pos, containing_cell);
    if(!result) {
        return return_vector;
    }
    cells_to_check.emplace_back(containing_cell);

    int containing_cell_x = containing_cell->pos_in_grid[0];
    int containing_cell_y = containing_cell->pos_in_grid[1];


    for( auto index_to_check : indices_to_check) {
        int x = containing_cell_x + index_to_check[0];
        int y = containing_cell_y + index_to_check[1];
        if(x < 0 || x > height || y < 0 || y > width)
            continue;
        int index = get_index(x, y);
        cells_to_check.emplace_back(cell_vector[index]);
    }

    for (auto& cell : cells_to_check) {
        return_vector.reserve(cell->evil_vector_part.size());
        return_vector.insert(return_vector.end(), cell->evil_vector_part.begin(), cell->evil_vector_part.end());
    }
    len = return_vector.size();
    return return_vector;
}

vector<shared_ptr<node_t>> cell_mgr_t::get_all_nodes() {
    vector<shared_ptr<node_t>> return_vector;
    for (const auto& cell : cell_vector) {
        for(const auto& node : cell->evil_vector_part)
            return_vector.emplace_back(node);
    }
    return return_vector;
}

void cell_mgr_t::add_cell(float x, float y, float width, float height) {

}

void cell_mgr_t::add_cell(const shared_ptr<cell_t>& cell) {
    cell_vector.emplace_back(cell);
}

void cell_mgr_t::add_node(const shared_ptr<node_t>& node) {
    auto pos = node->pos;
    for (auto& cell : cell_vector) {
        if(pos[0] > cell->pos[0] && pos[0] <= cell->pos[0] + cell->size[0] &&
           pos[1] > cell->pos[1] && pos[1] <= cell->pos[1] + cell->size[1]) {
            cell->evil_vector_part.emplace_back(node);
        }
    }
}

void cell_mgr_t::reset() {
    cell_vector.clear();
}

cell_mgr_t::cell_mgr_t() = default;
