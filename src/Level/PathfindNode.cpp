
#include "PathfindNode.h"

PathfindNode::PathfindNode(
    Vector2 current, 
    Vector2 parent, 
    Vector2 goal, 
    int cost)
    : current_coord(current), 
    parent_coord(parent), 
    goal_coord(goal), 
    move_cost(cost)
{
    distance = 
        std::fabs(current_coord.x - goal_coord.x) + 
        std::fabs(current_coord.y - goal_coord.y);
}
