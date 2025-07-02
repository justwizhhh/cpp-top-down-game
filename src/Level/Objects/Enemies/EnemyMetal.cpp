
#include "EnemyMetal.h"

EnemyMetal::EnemyMetal(std::vector<std::vector<int>> map, Vector2 bounds)
{
	initSprite("Data/Graphics/Objects/Enemies/enemyMetalSlime.png");

	if (!chase_texture.loadFromFile(
		"Data/Graphics/Objects/Enemies/enemyMetalSlimeAngry.png"))
	{
		std::cout << "Angry texture for EnemyMetal not found!" << std::endl;
	}

	current_map = map;
	current_room_bounds = bounds;

	health = max_health;
}

void EnemyMetal::getPlayer(std::shared_ptr<GameObject> player)
{
	player_tracker = player;
}

/*
	Use the pathfinder object to generate a path to move along
*/
void EnemyMetal::setPath()
{
	pathfinder.loadMap(current_map, current_room_bounds);
	step_progress = 0;

	pathfinder.findPath
	(getCentrePosition(),
		player_tracker->getCentrePosition(), room_offset);
	move_path = pathfinder.current_path;

	is_thread_active = false;
}

/*
	Call 'setPath' from a detached thread, to allow the rest of the game code 
	to still run
*/
void EnemyMetal::initialiseNewPath()
{
	is_thread_active = true;

	pathfinding_thread = std::thread(&EnemyMetal::setPath, this);
	pathfinding_thread.detach();
}

void EnemyMetal::stun()
{
	onHurt();

	sf::Color cache = sprite.getColor();
	sprite.setColor(
		sf::Color(cache.r, cache.g, cache.b, hurt_opacity));
	
	is_stunned = true;
	stun_timer.restart();
}

void EnemyMetal::update(float dt)
{
	prev_position = position;
	
	// Stunned state
	if (is_stunned)
	{
		if (velocity.getMagnitude() < 0.05)
		{
			velocity.x = 0;
			velocity.y = 0;
		}

		if (stun_timer.getElapsedTime().asSeconds() > stun_time)
		{
			sf::Color cache = sprite.getColor();
			sprite.setColor(
				sf::Color(cache.r, cache.g, cache.b, 255));
			
			initialiseNewPath();
			is_stunned = false;
		}

		position += velocity * dt;
	}
	// Normal walking
	else
	{
		// If the player is near enough
		if ((player_tracker->position - position).getMagnitude() <= 
			move_radius)
		{
			if (!is_thread_active)
			{
				// Lerp through a path, inching closer towards the player, 
				// until the final path node has been reached, where we then 
				// generate a new path
				if (move_path.size() >= 1)
				{
					current_pos = move_path[floor(step_progress)];

					// Make sure the enemy does not keep trying to move to the 
					// same position along the path
					if (ceil(step_progress) >= move_path.size())
					{
						next_pos = move_path[floor(step_progress)];
					}
					else
					{
						next_pos = move_path[ceil(step_progress)];
					}

					position = Vector2::lerp(current_pos,
						next_pos, step_progress - floor(step_progress));
					step_progress += move_speed * dt;

					// Create a new path if the previous one has been fully 
					// explored
					if (current_pos == next_pos)
					{
						if (ceil(step_progress) > move_path.size() - 1)
						{
							initialiseNewPath();
						}
					}
					else
					{
						if (ceil(step_progress) > move_path.size())
						{
							initialiseNewPath();
						}
					}
				}
				else
				{
					initialiseNewPath();
				}

				sprite.setTexture(chase_texture);
			}
		}
		else
		{
			move_path.clear();
			sprite.setTexture(texture);
		}
	}
	
	updatePos();
}