
#pragma once

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cmath>

// ------------
//
// Basic struct code for all 2-dimensional coordinates in the game
//
// Code taken, and modified, from 'Point and Click Game' source code (More Games in C++ assigment game)
//
// ------------

struct Vector2
{
	// Construction
	explicit Vector2(float x_ = 0.0, float y_ = 0.0);
	Vector2(const Vector2& rhs);

	// Basic operations
	Vector2& operator=(const Vector2& rhs);
	void set(float new_x, float new_y);

	bool operator==(const Vector2& vector) const;
	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	Vector2 operator+(const Vector2& rhs);
	Vector2 operator-(const Vector2& rhs);
	Vector2 operator*(float scalar);
	Vector2 operator/(float scalar);
	
	static Vector2 lerp(Vector2 a, Vector2 b, float t);
	bool checkIfNull();
	float getMagnitude() const;
	Vector2 getNormalised();

	// Conversion functions for SFML's own vector variables
	static sf::Vector2f convertToSFFloat(const Vector2& vector);
	static sf::Vector2i convertToSFInt(const Vector2& vector);

	// Data
	float x = 0;
	float y = 0;
};