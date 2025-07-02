
#include "GameObject.h"

GameObject::GameObject()
{
    is_active = true;
    start_active_state = is_active;

    pointer = std::shared_ptr<GameObject>(this);

    start_pos = position;

    sprite.setScale(sf::Vector2f(sprite_scale, sprite_scale));
}

GameObject::~GameObject()
{

}

/*
    Loads in a main texture for the object's sprite to use, and also gives the
    object a new scale
*/

bool GameObject::initSprite(std::string file_path)
{
    if (!texture.loadFromFile(file_path))
    {
        std::cout << "Texture from " << file_path << "not found!" << std::endl;
        return false;
    }
    else
    {
        sprite.setTexture(texture);
        return true;
    }

    sprite.setScale(sf::Vector2f(sprite_scale, sprite_scale));
}

/*
    Get the current position of the object's centre area
*/

Vector2 GameObject::getCentrePosition()
{
    return
        Vector2(
            sprite.getGlobalBounds().left + 
                (sprite.getGlobalBounds().width / 2),
            sprite.getGlobalBounds().top + 
                (sprite.getGlobalBounds().height / 2));
}

Vector2 GameObject::getPrevCentrePosition()
{
    Vector2 difference = position - prev_position;
    
    return
        Vector2(
            (sprite.getGlobalBounds().left +
            (sprite.getGlobalBounds().width / 2)) - difference.x,
            (sprite.getGlobalBounds().top +
            (sprite.getGlobalBounds().height / 2)) - difference.y);
}

/*
    Check if this object's sprite intersects with another object's sprite
*/

bool GameObject::intersects(sf::FloatRect obj_bounds, sf::FloatRect* overlap)
{
    sf::FloatRect intersect_rect;

    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::FloatRect other_bounds = obj_bounds;

    if (bounds.intersects(
        other_bounds, intersect_rect))
    {
        if (overlap != nullptr)
        {
            overlap->left = intersect_rect.left;
            overlap->top = intersect_rect.top;
            overlap->width = intersect_rect.width;
            overlap->height = intersect_rect.height;
        }
        return true;
    }
    else
    {
        return false;
    }
}

/*
    Check if this object's sprite overlaps with a position in-game
*/
bool GameObject::contains(Vector2& pos)
{
    return
        (sprite.getGlobalBounds().contains(Vector2::convertToSFFloat(pos)));
}

/*
    Separates the current object with another potentially-overlapping object
*/
bool GameObject::collide(sf::FloatRect obj_bounds, Vector2* col_side)
{
    // First, check if the two objects are actually overlapping
    sf::FloatRect overlap;

    Vector2 obj_centre(
        (obj_bounds.left) + ((obj_bounds.width) / 2),
        (obj_bounds.top) + ((obj_bounds.height) / 2));

    if (intersects(obj_bounds, &overlap))
    {
        if (overlap.width > 0.f && overlap.height > 0.f)
        {
            Vector2 difference(
                getPrevCentrePosition().x - obj_centre.x,
                getPrevCentrePosition().y - obj_centre.y);
            Vector2 side(0, 0);

            if (overlap.getSize().x < overlap.getSize().y)
            {
                if (difference.x < 0)
                    side.x = -1;
                else
                    side.x = 1;
            }
            else
            {
                if (difference.y < 0)
                    side.y = -1;
                else
                    side.y = 1;
            }

            // Move the player out of the overlap
            if (overlap.width < overlap.height)
            {
                // ... in the horizontal
                position.x += overlap.width * side.x;
            }
            else
            {
                // ... or in the vertical
                position.y += overlap.height * side.y;
            }

            if (col_side != nullptr)
            {
                col_side->x = side.x;
                col_side->y = side.y;
            }
            return true;
        }
    }

    return false;
}

/*
    Apply the position to the player's physical sprite location in-game
*/
void GameObject::updatePos()
{
    sprite.setPosition(position.x, position.y);
}

/*
    Take our position value and assign it to the object's actual sprite
*/
void GameObject::update(float dt)
{
    prev_position = position;
    updatePos();
}