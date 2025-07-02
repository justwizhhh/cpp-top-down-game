
#include "Vector2.h"

// ------------
//
// Basic struct code for all 2-dimensional coordinates in the game
//
// Code taken, and modified, from my 'Point and Click Game' source code 
// (Mores Games in C++ assigment game)
//
// ------------

/**
 *   @brief   Constructor.
 *   @details Requires values for it's x and y magnitudes.
 *   @return  void
 */
Vector2::Vector2(float x_, float y_) : x(x_), y(y_) {}

/**
 *   @brief   Copies a vector.
 *   @details Constructs a new Vector2 from an existing.
 *   @return  void
 */
Vector2::Vector2(const Vector2& rhs)
{
    this->x = rhs.x;
    this->y = rhs.y;
}

/**
 *   @brief   Copies a vector.
 *   @details Updates Vector2 from an existing.
 *   @return  void
 */
Vector2& Vector2::operator=(const Vector2& rhs)
= default;


/**
 *   @brief   Sets vector.
 *   @details Allows for manual x and y toggling of the vector.
 *   @return  void
 */
void Vector2::set(float new_x, float new_y)
{
    x = new_x;
    y = new_y;
}

/**
 *   @brief   Lerps between two vectors.
 *   @details Linearly interpolates between vectors a and b by t.
 *   @return  Vector2
 */
Vector2 Vector2::lerp(Vector2 a, Vector2 b, float t)
{
    float x = a.x + (b.x - a.x) * t;
    float y = a.y + (b.y - a.y) * t;
    return Vector2(x, y);
}

/**
 *   @brief   Checks if the vector is emtpy/null.
 *   @details Returns true or false depending on the vector is equal to (0, 0) 
 *            or not.
 *   @return  Vector2
 */
bool Vector2::checkIfNull()
{
    if (x == 0 && y == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 *   @brief   Turns SFML vector to our own Vector2 variable.
 *   @details Converts SFML's float-based vector value into our custom Vector2 
 *            class value.
 *   @return  Vector2
 */
sf::Vector2f Vector2::convertToSFFloat(const Vector2& vector)
{
    return sf::Vector2f(vector.x, vector.y);
}

/**
 *   @brief   Turns SFML vector to our own Vector2 variable.
 *   @details Converts SFML's integer-based vector value into our custom 
 *            Vector2 class value.
 *   @return  Vector2
 */
sf::Vector2i Vector2::convertToSFInt(const Vector2& vector)
{
    return sf::Vector2i(vector.x, vector.y);
}

/**
 *   @brief   Returns magnitude.
 *   @details Returns the magnitude scale of the current vector.
 *   @return  float
 */
float Vector2::getMagnitude() const
{
    float magnitude = sqrtf((x * x) + (y * y));
    return magnitude;
}

/**
 *   @brief   Normalises vector.
 *   @details Turns the vector into a unit vector.
 *   @return  void
 */
Vector2 Vector2::getNormalised()
{
    float magnitude = getMagnitude();
    Vector2 vec(*this);

    vec.x /= magnitude;
    vec.y /= magnitude;

    if (std::isnan(vec.x))
    {
        vec.x = 0;
    }
    if (std::isnan(vec.y))
    {
        vec.y = 0;
    }

    return vec;
}

bool Vector2::operator==(const Vector2& rhs) const
{
    return (this->x == rhs.x && this->y == rhs.y);
}

/**
 *   @brief   Adds to the vector.
 *   @details Adds another vector's x and y values to the current vector.
 *   @return  void
 */
Vector2& Vector2::operator+=(const Vector2& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;

    return *this;
}

/**
 *   @brief   Adds two vectors together.
 *   @details Returns the value of two vectors' x and y values being added 
 *            together.
 *   @return  Vector2
 */
Vector2 Vector2::operator+(const Vector2& rhs)
{
    Vector2 vec(*this);
    vec.x += rhs.x;
    vec.y += rhs.y;
    return vec;
}

/**
 *   @brief   Subtracts the vector.
 *   @details Subtracts another vector's x and y values to the current vector.
 *   @return  void
 */
Vector2& Vector2::operator-=(const Vector2& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;

    return *this;
}

/**
 *   @brief   Subtracts two vectors.
 *   @details Returns the value of two vectors' x and y values being 
 *            subtracted away from each other.
 *   @return  Vector2
 */
Vector2 Vector2::operator-(const Vector2& rhs)
{
    Vector2 vec(*this);
    vec.x -= rhs.x;
    vec.y -= rhs.y;
    return vec;
}

/**
 *   @brief   Scales the vector.
 *   @details Uses a single scalar value to adjust the vector.
 *   @return  void
 */
Vector2 Vector2::operator*(float scalar)
{
    Vector2 vec(*this);
    vec.x *= scalar;
    vec.y *= scalar;
    return vec;
}

/**
 *   @brief   Divides the vector.
 *   @details Uses a single scalar value to divide the vector.
 *   @return  void
 */
Vector2 Vector2::operator/(float scalar)
{
    Vector2 vec(*this);
    vec.x /= scalar;
    vec.y /= scalar;
    return vec;
}