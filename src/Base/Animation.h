
#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

// ------------
//
// UNUSED
// 
// Code for storing all texture data for playing and stopping object animations
// 
// Code taken, and modified, from 'Platformer Game' source code (Games in C++ 
// assigment game)
//
// ------------

class Animation
{
	public:
		Animation(
			const std::string& texture_path, 
			float max_speed, 
			int max_frames);

		void play();
		void updateAnim();
		void stop();

		sf::Texture texture;
		sf::IntRect texture_rect;

	private:
		bool is_playing = true;
		sf::Clock anim_timer;

		int anim_frames = 0;
		float anim_speed = 0;
		float anim_progress = 0;
};
