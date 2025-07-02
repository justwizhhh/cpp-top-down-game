
#include "Enemy.h"

Enemy::Enemy()
{
	
}

/*
	Update the enemies health
*/
void Enemy::onHurt()
{
	health--;
	if (health <= 0)
	{
		onDeath();
	}
}

/*
	Deactive the enemy. This will stop GameplayState from calling the enemy's 
	update function
*/
void Enemy::onDeath()
{
	is_active = false;
}