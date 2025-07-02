
#include "Player.h"

Player::Player()
{
	initSprite("Data/Graphics/Player/playerIdle.png");

	health = max_health;

	human_texture = texture;
	ball_texture.loadFromFile("Data/Graphics/Player/playerIdleBall.png");

	// Load attack effect visuals
	attack_texture.loadFromFile("Data/Graphics/Player/playerAttackEffect.png");
	attack_sprite.setTexture(attack_texture);
	attack_sprite.setOrigin(
		attack_texture.getSize().x / 2, 
		attack_texture.getSize().y / 2);
	attack_sprite.setScale(3, 3);
}

void Player::changeHealth(int value)
{
	health += value;

	if (value < 0)
	{
		// Start temporary invincibility timer, and turn player transluscent
		is_invincibile = true;

		// Kill player if health runs out
		if (health <= 0)
		{
			is_dead = true;
		}
		else
		{
			sf::Color cache = sprite.getColor();
			sprite.setColor(
				sf::Color(cache.r, cache.g, cache.b, invincibility_opacity));

			invincibility.restart();
		}
	}
}

void Player::changeScore(int value)
{
	score += value;
}

bool Player::getCue(PlayerCue* cue_obj)
{
	if (cue == nullptr)
	{
		if (cue_obj != nullptr)
		{
			cue = std::shared_ptr<PlayerCue>(cue_obj);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
	Give the player a new "target" position to start moving towards to
*/
void Player::setMoveTarget(Vector2 new_target)
{
	move_target = new_target;
	is_moving = true;
}

/*
	Add an offset to the current move target position, 
	in the case of a camera transition
*/
void Player::offsetMoveTarget(Vector2 offset)
{
	move_target += offset;
}

/*
	Move the player to the new position specified by the user's cursor
*/
void Player::moveToTarget(float dt)
{
	float distance =
		Vector2(
			getCentrePosition().x - move_target.x,
			getCentrePosition().y - move_target.y).getMagnitude();

	if (!move_target.checkIfNull() && floor(distance) > move_speed * dt)
	{
		Vector2 dir =
			Vector2(
				getCentrePosition().x - move_target.x,
				getCentrePosition().y - move_target.y);
		dir = dir.getNormalised() * -1;

		velocity = Vector2(dir.x * move_speed, dir.y * move_speed);
	}
	else
	{
		// Stop moving if the player is mostly close enough
		position = move_target -
			Vector2(
				sprite.getGlobalBounds().width / 2,
				sprite.getGlobalBounds().height / 2);
		cancelMove();
	}
}

/*
	Stop the player from moving along their current path
*/
void Player::cancelMove()
{
	if (!is_ball)
	{
		velocity = Vector2(0, 0);
		is_moving = false;
	}

	updatePos();
}

/*
	Toggle between walking mode and ball mode
*/
void Player::setToBall(bool ball_state)
{
	if (ball_state)
	{
		cancelMove();
		is_ball = true;

		sprite.setTexture(ball_texture);
	}
	else
	{
		velocity = Vector2(0, 0);
		is_ball = false;

		sprite.setTexture(human_texture);
	}
}

/*
	This sets the player's velocity and moves it to another position
	when getting hit by the player's cue
*/
void Player::hitBall(Vector2 hit_pos)
{
	Vector2 difference = Vector2(
		getCentrePosition().x - hit_pos.x,
		getCentrePosition().y - hit_pos.y);

	velocity = difference * ball_speed_scale;
}

void Player::knockBackBall(Vector2 hit_pos)
{
	Vector2 difference = Vector2(
		getCentrePosition().x - hit_pos.x,
		getCentrePosition().y - hit_pos.y);

	velocity = difference.getNormalised() * ball_knockback_speed;
}

/*
	This reflects the player's velocity when bouncing off of a wall or 
	other object
*/
bool Player::bounceWithBall(sf::FloatRect hit_rect)
{
	if (is_ball)
	{
		Vector2 hit_centre(
			hit_rect.left + (hit_rect.width / 2),
			hit_rect.top + (hit_rect.height / 2));
		
		Vector2 bounce_side(0, 0);
		
		if (collide(hit_rect, &bounce_side))
		{
 			velocity = Vector2(
				bounce_side.x != 0 ? velocity.x * -1 : velocity.x,
				bounce_side.y != 0 ? velocity.y * -1 : velocity.y);

			position += bounce_side;
			updatePos();

			return true;
		}
	}
	return false;
}

bool Player::tileCollision(sf::FloatRect tile_bounds)
{
	// The previous tile collision is buffered, to ensure the player 
	// only ever collides with the closest tile next to them

	bool has_collided = false;

	Vector2 tile_pos_a = Vector2(
		tile_bounds.left + (tile_bounds.width / 2),
		tile_bounds.top + (tile_bounds.height / 2));
	Vector2 tile_pos_b = Vector2(
		tile_buffer.left + (tile_buffer.width / 2),
		tile_buffer.top + (tile_buffer.height / 2));

	float distance_a = (tile_pos_a - getCentrePosition()).getMagnitude();
	float distance_b = (tile_pos_b - getCentrePosition()).getMagnitude();

	if (distance_a < distance_b)
	{
		collide(tile_bounds);
		has_collided = bounceWithBall(tile_bounds);
	}
	else
	{
		collide(tile_buffer);
		has_collided = bounceWithBall(tile_buffer);
	}

	tile_buffer = tile_bounds;
	
	return has_collided;
}

void Player::update(float dt)
{
	prev_position = position;

	// Normal walking mode
	if (is_moving)
	{
		moveToTarget(dt);
	}

	// Ball mode
	if (is_ball)
	{
		velocity = velocity * ball_decel_scale;

		is_attacking = velocity.getMagnitude() >= ball_attack_speed 
			? true : false;

		if (velocity.getMagnitude() < ball_stop_speed)
		{
			velocity.x = 0;
			velocity.y = 0;
		}
	}

	// Cue graphics updating
	Vector2 cue_difference = Vector2(
		getCentrePosition().x - cue->position.x,
		getCentrePosition().y - cue->position.y);
	float cue_distance = cue_difference.getMagnitude();

	is_cue_nearby = cue_distance < ball_hit_range;

	// Attack animation updating
	attack_sprite.setPosition(Vector2::convertToSFFloat(getCentrePosition()));
	attack_sprite.setRotation((atan2(velocity.y, velocity.x) * 180 / pi) + 90);

	// Updating invincibility status after getting hurt
	if (is_invincibile && 
		invincibility.getElapsedTime().asSeconds() >= invincibility_time)
	{
		sf::Color cache = sprite.getColor();
		sprite.setColor(
			sf::Color(cache.r, cache.g, cache.b, 255));

		is_invincibile = false;
	}

	// Final position updating
	position += velocity * dt;

	updatePos();
}
