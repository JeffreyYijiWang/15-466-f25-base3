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
	sound1_sample = Sound::loop_3D(*note1_sample, 1.0f, glm::vec4(-1.26137f, -11.861f, 0.0f, 1.0f), 5.0f);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_EVENT_KEY_DOWN) {
		if (evt.key.key == SDLK_ESCAPE) {
			SDL_SetWindowRelativeMouseMode(Mode::window, false);
			return true;
		} else if (evt.key.key == SDLK_A) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_D) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_W) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.key == SDLK_S) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}else if (evt.key.key == SDLK_1 || evt.key.key == SDLK_KP_1) {
   			if (sound1_sample) sound1_sample->stop();
			sound1_sample = Sound::play_3D(*note1_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_2 || evt.key.key == SDLK_KP_2) {
    if (sound2_sample) sound2_sample->stop();
			sound2_sample = Sound::play_3D(*note2_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_3 || evt.key.key == SDLK_KP_3) {
     if (sound3_sample) sound3_sample->stop();
	sound3_sample = Sound::play_3D(*note3_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_4 || evt.key.key == SDLK_KP_4) {
     if (sound4_sample) sound4_sample->stop();
	sound4_sample = Sound::play_3D(*note4_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_5 || evt.key.key == SDLK_KP_5) {
    if (sound5_sample) sound5_sample->stop();
	sound5_sample = Sound::play_3D(*note5_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_6 || evt.key.key == SDLK_KP_6) {
     if (sound6_sample) sound6_sample->stop();
	sound6_sample = Sound::play_3D(*note6_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_7 || evt.key.key == SDLK_KP_7) {
     if (sound7_sample) sound7_sample->stop();
	sound7_sample = Sound::play_3D(*note7_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_8 || evt.key.key == SDLK_KP_8) {
     if (sound8_sample) sound8_sample->stop();
	sound8_sample = Sound::play_3D(*note8_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_9 || evt.key.key == SDLK_KP_9) {
     if (sound9_sample) sound9_sample->stop();
	sound9_sample = Sound::play_3D(*note9_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_0 || evt.key.key == SDLK_KP_0) {
    if (sound0_sample) sound0_sample->stop();
	sound0_sample = Sound::play_3D(*note0_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_Q /* '#' (often Shift+3) */) {
         if (sound11_sample) sound11_sample->stop();
	sound11_sample = Sound::play_3D(*note11_sample, 0.3f, get_base_position()); 
    return true;
} else if (evt.key.key == SDLK_W) {
         if (sound11_sample) sound11_sample->stop();
	sound11_sample = Sound::play_3D(*note11_sample, 0.3f, get_base_position()); 
    return true;
}

	} else if (evt.type == SDL_EVENT_KEY_UP) {
		if (evt.key.key == SDLK_A) {
			left.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_D) {
			right.pressed = false;
			return true;
		} else if (evt.key.key == SDLK_W) {
			up.pressed = false;
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

void PlayMode::update(float elapsed) {

	// //slowly rotates through [0,1):
	// wobble += elapsed / 10.0f;
	// wobble -= std::floor(wobble);

	// hip->rotation = hip_base_rotation * glm::angleAxis(
	// 	glm::radians(5.0f * std::sin(wobble * 2.0f * float(M_PI))),
	// 	glm::vec3(0.0f, 1.0f, 0.0f)
	// );
	
	
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
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
	GL_ERRORS();
}

glm::vec3 PlayMode::get_base_position() {
	//the vertex position here was read from the model in blender:
	return base->make_world_from_local() * glm::vec4(-1.26137f, -11.861f, 0.0f, 1.0f);
}
