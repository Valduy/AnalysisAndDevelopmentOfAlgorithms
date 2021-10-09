#include <fstream>
#include <sstream>
#include "StopNode.h"

StopNode::StopNode(Messanger& messanger) 
	: messanger_(messanger)
{}

bool StopNode::Act(Model& model) {
	if (messanger_.RequestYesOrNo(question_, error_notification_)) {
		std::ofstream ofs(GameConstants::kSaveFileName, std::ios_base::out | std::ios_base::trunc);
		model.Serialize(ofs);
		ofs.flush();
		ofs.close();
		return false;
	}
	
	return true;
}