#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <random>

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

	//help with Chat GPT
	std::mt19937 rng;                       
	std::vector< std::vector<int> > levels; 
	int current_level;                  // 1-based; 0 means "not started"
	int max_level ;                      // after level 5, you win
	bool has_won;
	bool has_lost;
	float time_left ;            // seconds remaining (while playing)

	float level_time_left ;        // seconds

	std::vector<int> current_input;       // what the player typed this level

	// --- playback queue (played on submit) ---
	std::vector<int> playback_queue;
	std::size_t playback_pos;
	float playback_wait;           // time until next note
	float playback_gap;     // seconds between notes
	std::shared_ptr<Sound::PlayingSample> playing_note;


	// // --- Helpers ---
	std::vector<int> make_random_row(int n , int lo, int hi);
	void start_game();          // create level 1
	void next_level();          // create level (prev + two changed entries)
	bool check_win() const;     // current_level >= max_level

	void backspace_input();               // remove last
	std::string input_as_string() const;  // pretty-print current_input
	void submit_input();                  // queue playback, check answer

	void service_playback(float elapsed);

	std::shared_ptr<Sound::PlayingSample> play_note_value(int v);
	Sound::Sample const* sample_for_value(int v) const;
	// ----- input / gameplay -----
	std::vector<int> input_seq;   // what the player has typed this level

	

	// playback of typed sequence (on Enter)
	std::vector<int> playback_pending;
	float playback_timer;
	float playback_interval;
	bool is_playing;

	//animations
	bool  phone_kick_active ;
bool  phone_kick_success ;   // true = correct, false = wrong
float phone_kick_t   ;
float phone_kick_duration ;   // seconds
float phone_kick_angle_deg;   // how far to tilt

void trigger_phone_kick(bool success);

// ----- helpers -----
void reset_level_state();                 // clears input, resets timer/loss/playback
void reset_timer(float seconds);
void handle_note_press(int v, bool preview);                 
void lose_game(const char* reason);
void start_playback(const std::vector<int>& seq, float interval);
void step_playback(float elapsed);
void current_row_as_string(std::string &out) const;

};
