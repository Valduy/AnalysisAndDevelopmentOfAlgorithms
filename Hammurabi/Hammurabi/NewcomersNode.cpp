#include <algorithm>
#include "NewcomersNode.h"

bool NewcomersNode::Act(Model& model) {
    model.info.newcomers =  std::max(
        std::min(
            model.info.hunger_deaths / 2 + (5 - model.info.bushels_per_acre) * model.info.bushels_in_stocks / 600 + 1, 
            GameConstants::kMaxNewcomers), 
        GameConstants::kMinNewcomers);
    model.info.population += model.info.newcomers;
    return true;
}
