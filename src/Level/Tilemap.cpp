
#include "Tilemap.h"

// ------------
//
// This stores a list of all currently-loaded tiles and objects in a level
//
// ------------

/*
   Load in the tile ID information from the chosen '.csv' file
*/
void Tilemap::loadMap(const std::string& tile_file_path)
{
    // Original code for reading from '.csv' file:
    // https://medium.com/@ryan_forrester_/reading-csv-files-in-c-how-to-guide-35030eb378ad
    
    std::ifstream map_file(tile_file_path);

    if (map_file.is_open())
    {
        std::string current_tile_line;

        while (std::getline(map_file, current_tile_line))
        {
            std::vector<std::string> row;
            std::stringstream stream(current_tile_line);
            std::string cell;

            while (std::getline(stream, cell, ','))
            {
                row.push_back(cell);
            }

            map_data.push_back(row);
        }
    }

    map_file.close();
}

/*
   Load in object type/position info from another '.csv' file
*/
void Tilemap::loadObjects(const std::string& objects_file_path)
{
    std::ifstream object_file(objects_file_path);

    if (object_file.is_open())
    {
        std::string current_object_line;

        while (std::getline(object_file, current_object_line))
        {
            std::vector<std::string> row;
            std::stringstream stream(current_object_line);
            std::string cell;

            while (std::getline(stream, cell, ','))
            {
                row.push_back(cell);
            }

            objects_data.push_back(row);
        }
    }

    object_file.close();
}

void Tilemap::loadTileTextures(const std::string& file_path)
{
    if (!tilemap_texture.loadFromFile(file_path))
    {
        std::cout << "Texture from " << file_path << "not found!" << std::endl;
    }
    else
    {
        tilemap_texture_path = file_path;
    }
}

/*
   Take the tile data string and create individual sprites out of each tile
*/
void Tilemap::createMap(float tile_size, float map_scale, sf::Color bg_color)
{
    size = tile_size;
    
    width = map_data[0].size() * tile_size;
    height = map_data.size() * tile_size;
    bounds = Vector2(width * map_scale, height * map_scale);
    
    texture.create(width, height);
    texture.clear(bg_color);

    scale = map_scale;
    
    int tilesheet_width = (tilemap_texture.getSize().x / tile_size);

    // Iterating through columns
    for (int i = 0; i < map_data.size(); i++)
    {
        // Iterating through rows
        for (int j = 0; j < map_data[0].size(); j++)
        {
            int tile_id = std::stoi(map_data[i][j]);

            if (tile_id != -1)
            {
                // Create a texture and sprite for each tile
                sf::Texture tile_texture;
                tile_texture.loadFromFile(tilemap_texture_path);

                sf::Sprite tile(tile_texture);
                tile.setTextureRect(sf::IntRect(
                    (tile_id % tilesheet_width) * tile_size,
                    (tile_id / tilesheet_width) * tile_size,
                    tile_size,
                    tile_size));
                tile.setPosition(j * tile_size, i * tile_size);

                // Add the tile's sprite to the whole tilemap's render texture, 
                // one by one, with the rest of the map's tiles
                texture.draw(tile);

                // Create a collision box to represent this new tile
                if (std::find(
                    non_solid_tiles.begin(), 
                    non_solid_tiles.end(), 
                    tile_id) == non_solid_tiles.end())
                {
                    colliders.emplace_back(
                        tile.getPosition() * map_scale,
                        tile.getGlobalBounds().getSize() * map_scale);
                }
            }
        }
    }

    // Create a cost map, using only 0s and 1s, while still using map_data
    for (auto& map_row : map_data) 
    {
        std::vector<int> cost_row;
        for (auto& value : map_row) 
        {
            int cost_id = std::find(
                non_solid_tiles.begin(), 
                non_solid_tiles.end(), 
                std::stoi(value)) 
                    == non_solid_tiles.end() && std::stoi(value) != -1 ? 1 : 0;
            
            cost_row.push_back(cost_id);
        }
        cost_map.push_back(cost_row);
    }

    texture.display();
    sprite.setTexture(texture.getTexture());
    sprite.setScale(map_scale, map_scale);
}

void Tilemap::setNonSolidTile(int tile_id)
{
    non_solid_tiles.push_back(tile_id);
}

/*
   Take the object data string and create new objects for each number entry
*/
void Tilemap::createObjects(
    float tile_size, 
    float map_scale, 
    std::vector<std::shared_ptr<GameObject>>& objects_list)
{
    std::random_device random;
    
    for (int i = 0; i < objects_data.size(); i++)
    {
        for (int j = 0; j < objects_data[0].size(); j++)
        {
            int tile_id = std::stoi(objects_data[i][j]);

            if (tile_id != -1)
            {
                // Create and initialise all objects in the game depending on
                // their ID. 
                // 
                // 'push_back' is used in place of 'insert' for
                // objects we want to prioritise rendering in front
                
                Vector2 new_position = Vector2(
                    ((j * tile_size) + tile_size / 2) * map_scale,
                    ((i * tile_size) + tile_size / 2) * map_scale);
                
                switch (tile_id)
                {
                    case 0:
                    {
                        // Create a heart object
                        auto* new_heart = new Heart();

                        new_heart->start_active_state = true;
                        new_heart->start_pos = new_position;
                        new_heart->position = new_position;
                        new_heart->updatePos();
                        objects_list.insert(objects_list.begin(),
                            std::make_shared<GameObject>(*new_heart));
                        break;
                    }

                    case 1:
                    {
                        // Create a gem object
                        auto* new_gem = new Gem();

                        new_gem->start_active_state = true;
                        new_gem->start_pos = new_position;
                        new_gem->position = new_position;
                        new_gem->updatePos();
                        objects_list.insert(objects_list.begin(),
                            std::make_shared<GameObject>(*new_gem));
                        break;
                    }

                    case 2:
                    case 3:
                    {
                        // Create a bush object
                        auto* new_bush = new Bush();

                        new_bush->start_active_state = true;
                        new_bush->start_pos = new_position;
                        new_bush->position = new_position;
                        new_bush->updatePos();
                        objects_list.insert(objects_list.begin(), 
                            std::make_shared<GameObject>(*new_bush));

                        random_seed.seed(random());
                        std::uniform_real_distribution<> is_obj_stored
                            (0.0f, 1.0f);
                        float obj_chance = is_obj_stored(random_seed);

                        if (obj_chance 
                            < new_bush->heart_chance)
                        {
                            // Create a heart object hidden inside the bush
                            auto* stashed_heart = new Heart();

                            new_bush->initStoredObject(*stashed_heart);
                            objects_list.insert(objects_list.begin(), 
                                std::make_shared<GameObject>(*stashed_heart));
                        }
                        else if (obj_chance <
                            new_bush->gem_chance + new_bush->heart_chance)
                        {
                            // Create a gem object hidden inside the bush
                            auto* stashed_gem = new Gem();

                            new_bush->initStoredObject(*stashed_gem);
                            objects_list.insert(objects_list.begin(),
                                std::make_shared<GameObject>(*stashed_gem));
                        }

                        break;
                    }

                    case 4:
                    {
                        // Create an end goal potion object
                        auto* new_end_potion = new EndPotion();

                        new_end_potion->start_active_state = true;
                        new_end_potion->start_pos = new_position;
                        new_end_potion->position = new_position;
                        new_end_potion->updatePos();
                        objects_list.insert(objects_list.begin(),
                            std::make_shared<GameObject>(*new_end_potion));
                        break;
                    }

                    case 5:
                    {
                        // Create a generic slime enemy object
                        auto* new_slime_enemy = new EnemySlime();

                        new_slime_enemy->start_active_state = true;
                        new_slime_enemy->start_pos = new_position;
                        new_slime_enemy->position = new_position;
                        new_slime_enemy->updatePos();
                        objects_list.push_back(
                            std::make_shared<GameObject>(*new_slime_enemy));
                        break;
                    }
                        
                    case 6:
                    {
                        // Create a metal slime enemy object
                        auto* new_metal_enemy = new EnemyMetal
                            (cost_map, bounds);

                        Vector2 spawn_offset = Vector2(
                            new_metal_enemy->
                            sprite.getGlobalBounds().width / 2,
                            new_metal_enemy->
                            sprite.getGlobalBounds().height / 2
                        );

                        new_metal_enemy->start_active_state = true;
                        new_metal_enemy->start_pos = 
                            new_position + spawn_offset;
                        new_metal_enemy->position = 
                            new_position + spawn_offset;
                        new_metal_enemy->updatePos();
                        objects_list.push_back(
                            std::make_shared<GameObject>(*new_metal_enemy));
                        break;
                    }

                    case 7:
                    {
                        // Create a heavy slime enemy object, and make sure it
                        // is aligned properly with the level's tile grid
                        auto* new_heavy_enemy = new EnemyHeavy();

                        Vector2 spawn_offset = Vector2(
                            new_heavy_enemy->
                            sprite.getGlobalBounds().width / 4,
                            new_heavy_enemy->
                            sprite.getGlobalBounds().height / 4
                        );

                        new_heavy_enemy->start_active_state = true;
                        new_heavy_enemy->start_pos = 
                            new_position + spawn_offset;
                        new_heavy_enemy->position = 
                            new_position + spawn_offset;
                        new_heavy_enemy->updatePos();
                        objects_list.push_back(
                            std::make_shared<GameObject>(*new_heavy_enemy));
                        break;
                    }
                }
            }
        }
    }
}

