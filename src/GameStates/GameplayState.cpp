
#include "Game.h"

#include "GameplayState.h"

void GameplayState::beginState()
{
	window->setView(window->getDefaultView());
	
	// Set up all of the objects and level info for main gameplay
	initLevel();
	initObjects();
	initUI();

	cameraReset();

	transition_to_title = false;
	transition_to_win = false;
	transition_to_gameover = false;
}

bool GameplayState::initObjects()
{
	player.getCue(&player_cue);
	player.setToBall(false);

	player.health = player.max_health;
	player.is_invincibile = false;
	player.is_dead = false;

	player.position = Vector2(200, 200);
	player.velocity = Vector2(0, 0);
	player.updatePos();
	player.cancelMove();

	player.is_active = true;

	player_cue_area.setRadius(player.ball_hit_range);
	player_cue_area.setOrigin(
		player_cue_area.getRadius() - 
			player.sprite.getGlobalBounds().width / 2,
		player_cue_area.getRadius() - 
			player.sprite.getGlobalBounds().height / 2);
	player_cue_area.setFillColor(sf::Color(165, 201, 255, 50));
	player_cue_area.setOutlineColor(sf::Color(102, 69, 244, 50));
	player_cue_area.setOutlineThickness(7);

	if (level_rooms.size() != 0)
	{
		for (auto& room : level_rooms)
		{
			room->reEnableObjects();
		}
	}
	
	return true;
}

/*
	Creates all of the rooms the level will contain
*/
bool GameplayState::initLevel()
{
	if (level_rooms.empty())
	{
		int level_count = 12;
		std::vector<std::vector<std::shared_ptr<LevelRoom>>> 
			local_level_rooms(level_count);

		// Threads are used to load in select levels in the game at the same
		// time, for faster loading times upon first boot
		std::vector<std::thread> loading_threads;
		loading_threads.emplace_back([&local_level_rooms]()
			{
				local_level_rooms[0].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room0-0.csv",
						"Data/LevelMaps/RoomObjects0-0.csv",
						Vector2(0, 0)));
				local_level_rooms[1].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room1-0.csv",
						"Data/LevelMaps/RoomObjects1-0.csv",
						Vector2(1, 0)));
				local_level_rooms[2].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room2-0.csv",
						"Data/LevelMaps/RoomObjects2-0.csv",
						Vector2(2, 0)));
			});
		loading_threads.emplace_back([&local_level_rooms]()
			{
				local_level_rooms[3].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room3-0.csv",
						"Data/LevelMaps/RoomObjects3-0.csv",
						Vector2(3, 0)));
				local_level_rooms[4].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room0-1.csv",
						"Data/LevelMaps/RoomObjects0-1.csv",
						Vector2(0, -1)));
				local_level_rooms[5].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room1-1.csv",
						"Data/LevelMaps/RoomObjects1-1.csv",
						Vector2(1, -1)));
			});
		loading_threads.emplace_back([&local_level_rooms]()
			{
				local_level_rooms[6].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room2-1.csv",
						"Data/LevelMaps/RoomObjects2-1.csv",
						Vector2(2, -1)));
				local_level_rooms[7].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room3-1.csv",
						"Data/LevelMaps/RoomObjects3-1.csv",
						Vector2(3, -1)));
				local_level_rooms[8].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room1-2.csv",
						"Data/LevelMaps/RoomObjects1-2.csv",
						Vector2(1, -2)));
			});
		loading_threads.emplace_back([&local_level_rooms]()
			{
				local_level_rooms[9].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room2-2.csv",
						"Data/LevelMaps/RoomObjects2-2.csv",
						Vector2(2, -2)));
				local_level_rooms[10].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/Room3-2.csv",
						"Data/LevelMaps/RoomObjects3-2.csv",
						Vector2(3, -2)));
				local_level_rooms[11].emplace_back(
					std::make_shared<LevelRoom>("Data/LevelMaps/RoomFinal.csv",
						"Data/LevelMaps/RoomObjectsFinal.csv",
						Vector2(4, -2)));
			});

		for (auto& thread : loading_threads)
		{
			thread.join();
		}

		level_rooms.reserve(level_count);
		for (auto& local : local_level_rooms)
		{
			level_rooms.insert(level_rooms.end(), local.begin(), local.end());
		}
	}

	current_loaded_room = level_rooms[0];
	current_loaded_room->reEnableObjects();

	return current_loaded_room != nullptr;
}

bool GameplayState::initUI()
{
	ui.initValues(&player.health, &player.score);
	ui.reset();
	
	return true;
}

/*
	Puts the camera back to its default starting position
*/
void GameplayState::cameraReset()
{
	camera.reset(sf::FloatRect(
		0,
		0,
		window->getView().getSize().x,
		window->getView().getSize().y));
	ui_view.reset(sf::FloatRect(
		0,
		0,
		window->getView().getSize().x,
		window->getView().getSize().y));
}

/*
	Checks for all keyboard-based inputs
*/
void GameplayState::keyInput(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Space)
		{
			player.setToBall(!player.is_ball);
		}

		// Debug key to boot the player back to the titlescreen
		if (event.key.code == sf::Keyboard::Escape
			|| event.key.code == sf::Keyboard::R)
		{
			transition_to_title = true;
		}
	}
}

/*
	Checks for all mouse-based inputs
*/
void GameplayState::mouseInput(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (!player.is_ball)
			{
				player.setMoveTarget(mouse_pos);
			}
			else
			{
				// If in ball mode, move the player through aiming the cue
				if (player.velocity.checkIfNull() && player.is_cue_nearby)
				{
					player.hitBall(mouse_pos);
				}
			}
		}
	}
}

/*
	Finds which room the player is currently in 
	and re-activates all of the objects inside of it
*/
void GameplayState::loadNewRoom()
{
 	for (auto& room : level_rooms)
	{
		if (room->level_tiles.sprite.getGlobalBounds().contains(
			Vector2::convertToSFFloat(player.position)))
		{
			current_loaded_room = room;
			break;
		}
	}
}

/*
	Updates the position of the camera when the player moves to another room
*/
void GameplayState::cameraUpdate()
{
	// Upwards transition
	if (player.position.y - cam_trans_margin
		< (camera.getCenter().y - (camera.getSize().y / 2)))
	{
		camera.move(0, -window->getView().getSize().y);

		player.position -= Vector2(
			0, 
			player.sprite.getGlobalBounds().height + cam_trans_offset);
		player.cancelMove();
		loadNewRoom();
	}

	// Downwards transition
	if ((player.position.y + player.sprite.getGlobalBounds().height 
			+ cam_trans_margin)
		> (camera.getCenter().y + (camera.getSize().y / 2)))
	{
		camera.move(0, window->getView().getSize().y);

		player.position += Vector2(
			0, 
			player.sprite.getGlobalBounds().height + cam_trans_offset);
		player.cancelMove();
		loadNewRoom();
	}
	
	// Left-side transition
	if (player.position.x - cam_trans_margin
		< (camera.getCenter().x - (camera.getSize().x / 2)))
	{
		camera.move(-window->getView().getSize().x, 0);

		player.position -= Vector2(
			player.sprite.getGlobalBounds().width + cam_trans_offset, 
			0);
		player.cancelMove();
		loadNewRoom();
	}
	
	// Right-side transition
	if ((player.position.x + player.sprite.getGlobalBounds().width 
			+ cam_trans_margin)
		> (camera.getCenter().x + (camera.getSize().x / 2)))
	{
		camera.move(window->getView().getSize().x, 0);
		
		player.position += Vector2(
			player.sprite.getGlobalBounds().width + cam_trans_offset, 
			0);
		player.cancelMove();
		loadNewRoom();
	}
}

int GameplayState::update(float dt)
{
	// Retreiving game-wide mouse info
	sf::Vector2i mouse_pixels = static_cast<sf::Vector2i>(
		window->mapPixelToCoords(
		sf::Mouse::getPosition(*window), camera));
	
	mouse_pos = Vector2(mouse_pixels.x, mouse_pixels.y);
	
	// Object updates
	// Code adapted from my 'Point and Click Puzzle Game' assignment
	// Original code: https://github.com/UWEGames-GiC2/point-and-click-puzzle-game-DominikHHH/blob/main/src/Game.cpp
	for (int i = 0; 
		i < current_loaded_room->level_objects.size(); 
		i++)
	{
		// Use the smart pointer directly without extracting raw pointers
		std::shared_ptr<GameObject> obj =
			current_loaded_room->level_objects.at(i)->pointer;

		if (obj->is_active)
		{
			// Heart objects restore the player's health
			std::shared_ptr<Heart> heart =
				std::dynamic_pointer_cast<Heart>(obj);

			if (heart != nullptr)
			{
				if (player.intersects(heart->sprite.getGlobalBounds()))
				{
					heart->is_active = false;
					player.changeHealth(1);
					ui.updateHealth();
				}
			}

			// Gem objects give the player score
			std::shared_ptr<Gem> gem =
				std::dynamic_pointer_cast<Gem>(obj);

			if (gem != nullptr)
			{
				if (player.intersects(gem->sprite.getGlobalBounds()))
				{
					gem->is_active = false;
					player.changeScore(100);
					ui.updateScore();
				}
			}

			// Bush objects can be hit by the player to expose other objects
			std::shared_ptr<Bush> bush =
				std::dynamic_pointer_cast<Bush>(obj);

			if (bush != nullptr)
			{
				if (player.intersects(bush->sprite.getGlobalBounds()))
				{
					if (player.is_ball)
					{
						bush->is_active = false;
						player.bounceWithBall(bush->sprite.getGlobalBounds());
						bush->revealObject();
					}
					else
					{
						player.collide(bush->sprite.getGlobalBounds());
					}
				}
			}

			// The End Potion triggers the game's win state
			std::shared_ptr<EndPotion> end_potion =
				std::dynamic_pointer_cast<EndPotion>(obj);

			if (end_potion != nullptr)
			{
				if (player.intersects(end_potion->sprite.getGlobalBounds()))
				{
					end_potion->is_active = false;
					transition_to_win = true;
				}
			}

			// A generic enemy, moving around in random directions
			std::shared_ptr<EnemySlime> slime_enemy =
				std::dynamic_pointer_cast<EnemySlime>(obj);

			if (slime_enemy != nullptr)
			{
				// Update the enemy's physics like with the player
				slime_enemy->update(dt);

				for (auto& tile : current_loaded_room->level_tiles.colliders)
				{
					if (slime_enemy->tileCollision(tile))
					{
						slime_enemy->setMoveTarget(tile);
						break;
					}
				}

				// Collision with other bush objects
				for (int i = 0; 
					i < current_loaded_room->level_objects.size(); 
					i++)
				{
					std::shared_ptr<Bush> bush = 
						std::dynamic_pointer_cast<Bush>(
							current_loaded_room->level_objects.at(i)->pointer);

					if (bush != nullptr)
					{
						if (slime_enemy->collide(
							bush->sprite.getGlobalBounds()))
						{
							if (slime_enemy->is_knocked_back)
							{
								slime_enemy->bounce(
									bush->sprite.getGlobalBounds());
							}
							else
							{
								slime_enemy->setMoveTarget(
									bush->sprite.getGlobalBounds());
							}
							break;
						}
					}
				}

				// Enemy or the player taking damage
				if (!slime_enemy->is_knocked_back)
				{
					if (player.collide(slime_enemy->sprite.getGlobalBounds()))
					{
						if (!player.is_invincibile)
						{
							if (player.is_ball)
							{
								if (player.is_attacking)
								{
									slime_enemy->knockBack(&player);
									player.bounceWithBall(
										slime_enemy->sprite.getGlobalBounds());

									if (slime_enemy->health <= 0)
									{
										player.changeScore(250);
										ui.updateScore();
									}
								}
								else
								{
									player.knockBackBall(
										slime_enemy->position);
									player.changeHealth(-1);
									ui.updateHealth();
								}
							}
							else
							{
								player.setToBall(true);
								player.knockBackBall(slime_enemy->position);
								player.changeHealth(-1);
								ui.updateHealth();
							}
						}
					}
				}
				
			}

			// A more dangerous enemy, who uses pathfinding to find the player
			std::shared_ptr<EnemyMetal> metal_enemy =
				std::dynamic_pointer_cast<EnemyMetal>(obj);

			if (metal_enemy != nullptr)
			{
				if (metal_enemy->player_tracker == nullptr)
				{
					metal_enemy->getPlayer(player.pointer);
				}

				// Update the enemy's physics like with the player
				metal_enemy->update(dt);

				// Enemy or the player taking damage
				if (!metal_enemy->is_stunned)
				{
					if (!player.is_invincibile)
					{
						if (player.collide(
							metal_enemy->sprite.getGlobalBounds()))
						{
							if (player.is_ball)
							{
								if (player.is_attacking)
								{
									metal_enemy->stun();
									player.bounceWithBall(
										metal_enemy->sprite.getGlobalBounds());

									if (metal_enemy->health <= 0)
									{
										player.changeScore(500);
										ui.updateScore();
									}
								}
								else
								{
									player.knockBackBall(
										metal_enemy->position);
									player.changeHealth(-1);
									ui.updateHealth();
								}
							}
							else
							{
								player.setToBall(true);
								player.knockBackBall(metal_enemy->position);
								player.changeHealth(-1);
								ui.updateHealth();
							}
						}
					}
				}
			}

			// A stationary, tank-like enemy
			std::shared_ptr<EnemyHeavy> heavy_enemy =
				std::dynamic_pointer_cast<EnemyHeavy>(obj);

			if (heavy_enemy != nullptr)
			{
				heavy_enemy->update(dt);

				for (auto& tile : current_loaded_room->level_tiles.colliders)
				{
					if (heavy_enemy->tileCollision(tile))
					{
						break;
					}
				}

				// Enemy or the player taking damage
				if (player.collide(heavy_enemy->sprite.getGlobalBounds()))
				{
					if (!player.is_invincibile)
					{
						if (player.is_ball)
						{
							if (player.is_attacking)
							{
								heavy_enemy->knockBack(&player);
								player.bounceWithBall(
									heavy_enemy->sprite.getGlobalBounds());

								if (heavy_enemy->health <= 0)
								{
									player.changeScore(250);
									ui.updateScore();
								}
							}
							else
							{
								player.knockBackBall(heavy_enemy->position);
								player.changeHealth(-1);
								ui.updateHealth();
							}
						}
						else
						{
							player.setToBall(true);
							player.knockBackBall(heavy_enemy->position);
							player.changeHealth(-1);
							ui.updateHealth();
						}
					}
				}
			}
		}
	}

	// General player updates
	if (player.is_active)
	{
		player.update(dt);

		if (player.is_ball)
		{
			player_cue.position = mouse_pos;
			player_cue.sprite.setRotation((atan2(
				player.getCentrePosition().y - mouse_pos.y,
				player.getCentrePosition().x - mouse_pos.x) * 180 / pi) + 90);
			player_cue.updatePos();

			player_cue_area.setPosition(
				Vector2::convertToSFFloat(player.position));
		}

		if (player.is_dead)
		{
			transition_to_gameover = true;
		}
	}

	// Player collision with the level terrain
	for (auto& tile : current_loaded_room->level_tiles.colliders)
	{
		if (player.tileCollision(tile))
		{
			break;
		}
	}

	// Camera updates
	cameraUpdate();

	if (transition_to_title)
	{
		return Game::States::START;
	}
	if (transition_to_win)
	{
		return Game::States::WIN;
	}
	if (transition_to_gameover)
	{
		return Game::States::GAMEOVER;
	}

	return Game::States::GAMEPLAY;
}

void GameplayState::render()
{
	window->setView(camera);

	window->draw(current_loaded_room->level_tiles.sprite);

	for (auto obj : current_loaded_room->level_objects)
	{
		if (obj->pointer->is_active)
		{
			window->draw(obj->pointer->sprite);
		}
	}

	if (player.is_active)
	{
		window->draw(player.sprite);
		if (player.is_ball)
		{
			if (player.is_cue_nearby)
			{
				window->draw(player_cue.sprite);
			}

			if (player.is_attacking)
			{
				window->draw(player.attack_sprite);
			}

			if (player.velocity.checkIfNull())
			{
				window->draw(player_cue_area);
			}
		}
	}

	window->setView(ui_view);
	window->draw(ui.gem_icon);
	window->draw(ui.score_display);
	for (auto& heart : ui.health_display)
	{
		window->draw(*heart);
	}
}

void GameplayState::endState()
{
	
}


