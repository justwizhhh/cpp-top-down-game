
#include "EnemySlime.h"

EnemySlime::EnemySlime()
{
	initSprite("Data/Graphics/Objects/Enemies/enemySlime.png");
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	health = max_health;
}

/*
	This reflects the enemy's velocity when bouncing off of a wall or
	other object
*/
bool EnemySlime::bounce(sf::FloatRect hit_rect)
{
	Vector2 hit_centre(
		hit_rect.left + (hit_rect.width / 2),
		hit_rect.top + (hit_rect.height / 2));

	Vector2 bounce_side(0, 0);

	if (collide(hit_rect, &bounce_side))
	{
		position += bounce_side;
		updatePos();

		velocity = Vector2(
			bounce_side.x != 0 ? velocity.x * -1 : velocity.x,
			bounce_side.y != 0 ? velocity.y * -1 : velocity.y);

		return true;
	}
	return false;
}

/*
	Apply knockback force upon being hit
*/
void EnemySlime::knockBack(GameObject* obj)
{
	Vector2 difference = Vector2(
		getCentrePosition().x - obj->getCentrePosition().x,
		getCentrePosition().y - obj->getCentrePosition().y);

	velocity = difference.getNormalised() * 
		(obj->velocity.getMagnitude() * knockback_damping);

	is_knocked_back = true;

	sf::Color cache = sprite.getColor();
	sprite.setColor(
		sf::Color(cache.r, cache.g, cache.b, hurt_opacity));

	onHurt();
}

bool EnemySlime::tileCollision(sf::FloatRect tile_bounds)
{
	collide(tile_bounds);
	return bounce(tile_bounds);
}

void EnemySlime::setMoveBounds(sf::FloatRect bounds)
{
	move_bounds = bounds;
}

/*
	Change where the enemy is going to automatically move to next
*/
void EnemySlime::setMoveTarget(sf::FloatRect tile_col)
{
	// Generates two random numbers for which direction the enemy moves, as 
	// well as how far along in that direction
	// Original code: https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
	
	std::random_device random;
	random_seed.seed(random());
	std::uniform_int_distribution<> dist_range
		(0, 3);
	std::uniform_real_distribution<> move_range
		(min_move_dist * 3.0f, max_move_dist * 3.0f);
	
	if (tile_col == sf::FloatRect())
	{
		// Pick a random direction to move in
		Vector2 move_directions[] =
		{
			Vector2(1, 0),
			Vector2(-1, 0),
			Vector2(0, 1),
			Vector2(0, -1)
		};

		int index = dist_range(random_seed);
		current_move_dir = move_directions[index];
	}
	else
	{
		// But if the enemy just collided with a tile, try to move away from it
		Vector2 tile_side = Vector2(
			tile_col.getPosition().x - position.x,
			tile_col.getPosition().y - position.y).getNormalised();

		if (tile_side.x > tile_side.y)
		{
			if (tile_side.x > 0)
				current_move_dir = Vector2(-1, 0);
			else
				current_move_dir = Vector2(1, 0);
		}
		else
		{
			if (tile_side.y > 0)
				current_move_dir = Vector2(0, -1);
			else
				current_move_dir = Vector2(0, 1);
		}
	}
	
	current_move_dist = move_range(random_seed);

	prev_target = position;
	move_target = position + (current_move_dir * current_move_dist);
	t = 0;

	is_moving = true;
	updatePos();
}

/*
	Move the enemy around in random directions
*/
void EnemySlime::movement(float dt)
{
	if (is_moving && !is_knocked_back)
	{
		t += move_speed * dt;
		position = Vector2::lerp(prev_target, move_target, t);
	}
	else
	{
		setMoveTarget();
		is_moving = true;
	}

	if (t >= 1)
	{
		is_moving = false;
	}
}

void EnemySlime::update(float dt)
{
	prev_position = position;
	
	movement(dt);

	velocity = velocity * decel_scale;

	if (velocity.getMagnitude() < stop_speed)
	{
		velocity.x = 0;
		velocity.y = 0;

		if (is_knocked_back)
		{
			is_knocked_back = false;

			sf::Color cache = sprite.getColor();
			sprite.setColor(
				sf::Color(cache.r, cache.g, cache.b, 255));

			setMoveTarget();
		}
	}

	// Make sure the enemy stays inside of the current room
	if (!move_bounds.contains(Vector2::convertToSFFloat(
		position + (current_move_dir * sprite.getGlobalBounds().width))))
	{
		// Unless the player is doing damage to them. In that case, defeat them
		if (!is_knocked_back)
		{
			sf::FloatRect reflect_rect = sprite.getGlobalBounds();
			if (current_move_dir.x != 0)
			{
				reflect_rect.left += 
					current_move_dir.x * sprite.getGlobalBounds().width;
			}
			else if (current_move_dir.y != 0)
			{
				reflect_rect.top += 
					current_move_dir.y * sprite.getGlobalBounds().height;
			}

			setMoveTarget(reflect_rect);
		}
		else
		{
			onDeath();
		}
	}

	position += velocity * dt;

	updatePos();
}