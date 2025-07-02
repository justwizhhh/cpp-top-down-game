
#include "Animation.h"

Animation::Animation(
    const std::string& texture_path, 
    float max_speed, 
    int max_frames)
{
    if (!texture.loadFromFile(texture_path))
    {
        std::cout << " Texture from " 
                  << texture_path << " unable to be found!" 
                  << std::endl;
    }
    else
    {
        anim_speed = max_speed;
        anim_frames = max_frames;

        texture_rect = sf::IntRect(
            0,
            0,
            (int)texture.getSize().x / anim_frames,
            (int)texture.getSize().y);
    }
}

/*
    Plays the currently used animation
*/
void Animation::play()
{
    is_playing = true;
    anim_progress = 0;
}

/*
    Progresses through the current animation
*/
void Animation::updateAnim()
{
    if (is_playing)
    {
        anim_progress += anim_speed;
        if (anim_progress >= (float)anim_frames)
        {
            anim_progress = 0;
        }

        float frame_width = (float)texture.getSize().x / (float)anim_frames;

        texture_rect.left =
            (frame_width * std::floor(anim_progress) == 0.0f)
            ? 0
            : (int)(frame_width * std::floor(anim_progress));
    }
}

/*
    Stop playing the currently used animation
*/
void Animation::stop()
{
    is_playing = false;
}