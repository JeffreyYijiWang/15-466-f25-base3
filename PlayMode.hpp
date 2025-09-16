#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, one, two,three,four,five,six,seven,eight,nine,zero,star,pound;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	//hexapod leg to wobble:
	Scene::Transform *base = nullptr;
	Scene::Transform *phone = nullptr;
	Scene::Transform *Numpad = nullptr;

	glm::quat base_rotation;
	glm::quat numpad_rotation;
	glm::quat phone_rotation;

	glm::vec3 get_base_position();

	//base
	std::shared_ptr< Sound::PlayingSample > base_loop;

	//sound honk sound:
	std::shared_ptr< Sound::PlayingSample > sound1_sample;
	std::shared_ptr< Sound::PlayingSample > sound2_sample;
	std::shared_ptr< Sound::PlayingSample > sound3_sample;
	std::shared_ptr< Sound::PlayingSample > sound4_sample;
	std::shared_ptr< Sound::PlayingSample > sound5_sample;
	std::shared_ptr< Sound::PlayingSample > sound6_sample;
	std::shared_ptr< Sound::PlayingSample > sound7_sample;
	std::shared_ptr< Sound::PlayingSample > sound8_sample;
	std::shared_ptr< Sound::PlayingSample > sound9_sample;
	std::shared_ptr< Sound::PlayingSample > sound0_sample;
	std::shared_ptr< Sound::PlayingSample > sound11_sample; //#
	std::shared_ptr< Sound::PlayingSample > sound12_sample; //*
	
	//camera:
	Scene::Camera *camera = nullptr;

	// std::mt19937 rng;                       // PRNG
	// std::vector< std::vector<int> > levels; // level i -> 12-element list
	// int current_level = 0;                  // 1-based; 0 means "not started"
	// int max_level = 5;                      // after level 5, you win
	// bool has_won = false;

	// // --- Helpers ---
	// std::vector<int> make_random_row(int n = 12, int lo = 0, int hi = 12);
	// void start_game();          // create level 1
	// void next_level();          // create level (prev + two changed entries)
	// bool check_win() const;     // current_level >= max_level
	// std::string current_row_as_string() const; // "a b c ..."

};
