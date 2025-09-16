#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>

GLuint hexapod_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > hexapod_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("phone.pnct"));
	hexapod_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > phone_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("phone.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = hexapod_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = hexapod_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});

Load< Sound::Sample > background_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("background.wav"));
});




Load< Sound::Sample > note0_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note0.wav"));
});
Load< Sound::Sample > note1_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note1.wav"));
});
Load< Sound::Sample > note2_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note2.wav"));
});
Load< Sound::Sample > note3_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note3.wav"));
});
Load< Sound::Sample > note4_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note4.wav"));
});
Load< Sound::Sample > note5_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note5.wav"));
});
Load< Sound::Sample > note6_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note6.wav"));
});
Load< Sound::Sample > note7_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note7.wav"));
});
Load< Sound::Sample > note8_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note8.wav"));
});
Load< Sound::Sample > note9_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note9.wav"));
});
Load< Sound::Sample > note11_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note11.wav"));
});
Load< Sound::Sample > note12_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("note12.wav"));
});


void PlayMode::start_game() {
  has_won = false;
  has_lost = false;
  levels.clear();
  levels.push_back(make_random_row(12, 0, 12));
  current_level = 1; // start at level 1
  rng.seed(std::random_device{}());
  reset_level_state();  
}

PlayMode::PlayMode() : scene(*phone_scene) {
	//get pointers to leg for convenience:
	for (auto &transform : scene.transforms) {
		if (transform.name == "bot") base = &transform;
		else if (transform.name == "Phone") phone = &transform;
		else if (transform.name == "NumPad") Numpad = &transform;
	}
	if (base == nullptr) throw std::runtime_error("Hip not found.");
	if (phone == nullptr) throw std::runtime_error("Upper leg not found.");
	if (Numpad == nullptr) throw std::runtime_error("Lower leg not found.");

	base_rotation = base->rotation;
	numpad_rotation = Numpad->rotation;
	phone_rotation = phone->rotation;

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	// //start music loop playing:
	// // (note: position will be over-ridden in update())
	base_loop = Sound::loop_3D(*background_sample, 1.0f, glm::vec4(-1.26137f, -11.861f, 0.0f, 1.0f), 3.0f);
	level_time_left = 60.0f; 
  playback_pos = 0;
  playback_wait = 0.0f;       
  playback_gap = 0.18f;   
  time_left = 60.0f;
rng.seed(std::random_device{}());


  start_game();

}


PlayMode::~PlayMode() {
}

void PlayMode::reset_level_state() {
  input_seq.clear();
  reset_timer(60.0f);
  is_playing = false;
  playback_timer = 0.0f;
  playback_pending.clear();
}

void PlayMode::reset_timer(float seconds) {
  time_left = seconds;
  has_lost = false;
}

void PlayMode::handle_note_press(int v, bool preview) {
  if (has_won || has_lost) return;
  if (v < 0 || v > 12) return;
  if (input_seq.size() >= 12) return;      // cap length to 12 to match target
  input_seq.push_back(v);

  if (preview) {
    if (auto s = sample_for_value(v)) {
      Sound::play_3D(*s, 0.25f, get_base_position());
    }
  }
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	auto press_digit = [&](int d){
    handle_note_press(d, /*preview=*/true);
  };

	if (evt.type == SDL_EVENT_KEY_DOWN) {
		if (evt.key.key == SDLK_ESCAPE) {
			SDL_SetWindowRelativeMouseMode(Mode::window, false);
			return true;
		} else if (evt.key.key == SDLK_1 || evt.key.key == SDLK_KP_1) {
   			if (sound1_sample) sound1_sample->stop();
			sound1_sample = Sound::play_3D(*note1_sample, 0.3f, get_base_position()); 
    press_digit(1);
			return true;
} else if (evt.key.key == SDLK_2 || evt.key.key == SDLK_KP_2) {
    if (sound2_sample) sound2_sample->stop();
			sound2_sample = Sound::play_3D(*note2_sample, 0.3f, get_base_position()); 
	press_digit(2);
			return true;
} else if (evt.key.key == SDLK_3 || evt.key.key == SDLK_KP_3) {
     if (sound3_sample) sound3_sample->stop();
	sound3_sample = Sound::play_3D(*note3_sample, 0.3f, get_base_position()); 
	press_digit(3);
    return true;
} else if (evt.key.key == SDLK_4 || evt.key.key == SDLK_KP_4) {
     if (sound4_sample) sound4_sample->stop();
	sound4_sample = Sound::play_3D(*note4_sample, 0.3f, get_base_position());
	press_digit(4); 
    return true;
} else if (evt.key.key == SDLK_5 || evt.key.key == SDLK_KP_5) {
    if (sound5_sample) sound5_sample->stop();
	sound5_sample = Sound::play_3D(*note5_sample, 0.3f, get_base_position());
	press_digit(5); 
    return true;
} else if (evt.key.key == SDLK_6 || evt.key.key == SDLK_KP_6) {
     if (sound6_sample) sound6_sample->stop();
	sound6_sample = Sound::play_3D(*note6_sample, 0.3f, get_base_position()); 
	press_digit(6);
    return true;
} else if (evt.key.key == SDLK_7 || evt.key.key == SDLK_KP_7) {
     if (sound7_sample) sound7_sample->stop();
	sound7_sample = Sound::play_3D(*note7_sample, 0.3f, get_base_position());
	press_digit(7); 
    return true;
} else if (evt.key.key == SDLK_8 || evt.key.key == SDLK_KP_8) {
     if (sound8_sample) sound8_sample->stop();
	sound8_sample = Sound::play_3D(*note8_sample, 0.3f, get_base_position());
	press_digit(8); 
    return true;
} else if (evt.key.key == SDLK_9 || evt.key.key == SDLK_KP_9) {
     if (sound9_sample) sound9_sample->stop();
	sound9_sample = Sound::play_3D(*note9_sample, 0.3f, get_base_position()); 
	press_digit(9);
	return true;
} else if (evt.key.key == SDLK_0 || evt.key.key == SDLK_KP_0) {
    if (sound0_sample) sound0_sample->stop();
	sound0_sample = Sound::play_3D(*note0_sample, 0.3f, get_base_position()); 
    press_digit(0);
	return true;
} else if (evt.key.key == SDLK_Q /* '#' (often Shift+3) */) {
         if (sound11_sample) sound11_sample->stop();
	sound11_sample = Sound::play_3D(*note11_sample, 0.3f, get_base_position()); 
    press_digit(11);
	return true;
} else if (evt.key.key == SDLK_W) {
         if (sound11_sample) sound11_sample->stop();
	sound11_sample = Sound::play_3D(*note11_sample, 0.3f, get_base_position()); 
    press_digit(12);
	return true;
} else if (evt.key.key == SDLK_BACKSPACE) { backspace_input(); return true; }
  else if (evt.key.key == SDLK_RETURN || evt.key.key == SDLK_KP_ENTER) {
    submit_input();
    return true;
  }

	} else if (evt.type == SDL_EVENT_KEY_UP) {
		if (evt.key.key == SDLK_A) {
			left.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_D) {
			right.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_S) {
			down.pressed = false;
			return true;
		}
	} else if (evt.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (SDL_GetWindowRelativeMouseMode(Mode::window) == false) {
			SDL_SetWindowRelativeMouseMode(Mode::window, true);
			return true;
		}
	} 

	return false;
}

void PlayMode::step_playback(float elapsed) {
  if (!is_playing) return;
  playback_timer -= elapsed;
  if (playback_timer <= 0.0f) {
    if (playback_pending.empty()) {
      is_playing = false;
      return;
    }
    int v = playback_pending.back();
    playback_pending.pop_back();
    if (auto s = sample_for_value(v)) {
      Sound::play_3D(*s, 0.3f, get_base_position());
    }
    playback_timer += playback_interval;
  }
}

Sound::Sample const* PlayMode::sample_for_value(int v) const {
  switch (v) {
    case 0:  return note0_sample;
    case 1:  return note1_sample;
    case 2:  return note2_sample;
    case 3:  return note3_sample;
    case 4:  return note4_sample;
    case 5:  return note5_sample;
    case 6:  return note6_sample;
    case 7:  return note7_sample;
    case 8:  return note8_sample;
    case 9:  return note9_sample;
    case 11: return note11_sample;
    case 12: return note12_sample;
    default: return nullptr;
  }
}

void PlayMode::backspace_input() {
  if (has_won || has_lost) return;
  if (!input_seq.empty()) input_seq.pop_back();
}


void PlayMode::lose_game(const char* /*reason*/) {
  has_lost = true;
  is_playing = false;
  playback_pending.clear();
}

void PlayMode::start_playback(const std::vector<int>& seq, float interval) {
  playback_pending = seq; // copy
  std::reverse(playback_pending.begin(), playback_pending.end()); // pop_back = next note
  playback_interval = interval;
  playback_timer = 0.0f;
  is_playing = !playback_pending.empty();
}

void PlayMode::submit_input() {
  if (has_won || has_lost) return;

  // Play the typed song:
  start_playback(input_seq, 0.18f);

  // Validate vs target row:
  const std::vector<int>& target = levels.back();
  bool ok = (input_seq.size() == target.size()) &&
            std::equal(input_seq.begin(), input_seq.end(), target.begin());

  if (ok) {
    next_level();     // this also resets the input + timer
  } else {
    lose_game("wrong");
  }
}

void PlayMode::update(float elapsed) {

	// countdown (only while active)
  if (!has_won && !has_lost) {
    time_left -= elapsed;
    if (time_left <= 0.0f) {
      time_left = 0.0f;
      lose_game("timeout");
    }
  }

  // advance audio playback queue
  step_playback(elapsed);
	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		lines.draw_text("play sounds with 1,2,3,4,5,6,7,8,9, Q(for #) and W(for *)",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		
			
		int secs = int(std::ceil(time_left));
			int mm = secs / 60, ss = secs % 60;
			char timer_buf[16];
			std::snprintf(timer_buf, sizeof(timer_buf), "%02d:%02d", mm, ss);

			std::string timer_text = (has_lost ? std::string("YOU LOSE ") :
									has_won   ? std::string("YOU WIN ")  :
												std::string("TIME ")) + timer_buf;

			// draw centered at top:
			lines.draw_text(timer_text,
			glm::vec3(0.0f - 0.5f * H, +1.0f - 1.4f * H, 0.0f), // x≈0 is center; tweak as needed
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00,0x00,0x00,0x00));
			lines.draw_text(timer_text,
			glm::vec3(0.0f - 0.5f * H + ofs, +1.0f - 1.4f * H + ofs, 0.0f),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff,0xff,0xff,0x00));
	

		// --- Typed input line ---
		std::string typed = "[";
		for (size_t i = 0; i < input_seq.size(); ++i) {
		if (i) typed += ' ';
		typed += std::to_string(input_seq[i]);
		}
		typed += "]  (Backspace=delete, Enter=submit)";

		lines.draw_text(typed,
		glm::vec3(-aspect + 0.1f * H, -1.0f + 0.5f * H, 0.0f),
		glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
		glm::u8vec4(0x00,0x00,0x00,0x00));
		lines.draw_text(typed,
		glm::vec3(-aspect + 0.1f * H + ofs, -1.0f + 0.5f * H + ofs, 0.0f),
		glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
		glm::u8vec4(0xff,0xff,0xff,0x00));
			}
			GL_ERRORS();
}

glm::vec3 PlayMode::get_base_position() {
	//the vertex position here was read from the model in blender:
	return base->make_world_from_local() * glm::vec4(-1.26137f, -11.861f, 0.0f, 1.0f);
}

std::vector<int> PlayMode::make_random_row(int n, int lo, int hi) {
  static const int allowed[] = {0,1,2,3,4,5,6,7,8,9,11,12}; // no 10 (no key / sample wired)
  std::uniform_int_distribution<int> pick(0, int(sizeof(allowed)/sizeof(allowed[0])) - 1);
  std::vector<int> row(n);
  for (int i = 0; i < n; ++i) row[i] = allowed[pick(rng)];
  return row;
}



void PlayMode::next_level() {
  if (has_won || has_lost) return;

  if (current_level >= max_level) {
    has_won = true;
    return;
  }

  // Copy previous, change two random positions:
  std::vector<int> next = levels.back();

  std::uniform_int_distribution<int> idx(0, 11);
  std::uniform_int_distribution<int> val(0, 12);

  int a = idx(rng), b = idx(rng);
  while (b == a) b = idx(rng);

  int newA = val(rng); while (newA == next[a]) newA = val(rng);
  int newB = val(rng); while (newB == next[b]) newB = val(rng);

  next[a] = newA; next[b] = newB;

  levels.push_back(std::move(next));
  ++current_level;

  if (current_level >= max_level) has_won = true;

  reset_level_state(); 
}
bool PlayMode::check_win() const {
  return has_won;
}


// void PlayMode::reset_level_state() {
//   level_time_left = 60.0f;
//   current_input.clear();
//   playback_queue.clear();
//   playback_pos = 0;
//   playback_wait = 0.0f;
//   playing_note.reset();
// }