
#include "EnemyHeavy.h"

EnemyHeavy::EnemyHeavy()
{
	initSprite("Data/Graphics/Objects/Enemies/enemyHeavySlime.png");
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	health = max_health;
}

/*
	This reflects the enemy's velocity when bouncing off of a wall or
	other object
*/
bool EnemyHeavy::bounce(sf::FloatRect hit_rect)
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
void EnemyHeavy::knockBack(GameObject* obj)
{
	Vector2 difference = Vector2(
		getCentrePosition().x - obj->getCentrePosition().x,
		getCentrePosition().y - obj->getCentrePosition().y);

	velocity = difference.getNormalised() *
		(obj->velocity.getMagnitude() * knockback_damping);

	onHurt();
}

bool EnemyHeavy::tileCollision(sf::FloatRect tile_bounds)
{
	collide(tile_bounds);
	return bounce(tile_bounds);
}

void EnemyHeavy::update(float dt)
{
	prev_position = position;
	
	velocity = velocity * decel_scale;

	if (velocity.getMagnitude() < 0.05)
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	position += velocity * dt;

	updatePos();
}