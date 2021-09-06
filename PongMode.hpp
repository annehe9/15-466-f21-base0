#include "ColorTextureProgram.hpp"

#include "Mode.hpp"
#include "GL.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <random>

/*
 * PongMode is a game mode that implements a single-player game of Not-Pong.
 */

struct PongMode : Mode {
	PongMode();
	virtual ~PongMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	glm::vec2 court_radius = glm::vec2(5.0f, 8.0f);
	glm::vec2 paddle_radius = glm::vec2(0.2f, 1.0f);
	glm::vec2 ball_radius = glm::vec2(0.2f, 0.2f);

	// left paddles logic
	glm::vec2 left_paddles_radius = glm::vec2(court_radius.x / 4.0f, 0.4f);
	glm::vec2 left_paddles[4];
	bool paddles_active[4] = { false };
	glm::u8vec4 left_paddles_colors[4] = { glm::u8vec4(251, 139, 36, 20), glm::u8vec4(217, 3, 104, 20), glm::u8vec4(130, 2, 99, 20), glm::u8vec4(4, 167, 119, 20) }; //start transparent
	float tick_acc = 0.0f;

	uint32_t score = 0;
	std::vector<glm::vec2> balls;
	std::vector<glm::vec2> ball_velocities;
	uint32_t num_balls = 0;
	float gravity = -2.f;
	bool ball_added = false;
	int lives = 10;

	//----- pretty gradient trails -----

	float trail_length = 1.3f;
	std::vector<std::deque< glm::vec3 >> ball_trails; //stores (x,y,age), oldest elements first

	//----- opengl assets / helpers ------

	//draw functions will work on vectors of vertices, defined as follows:
	struct Vertex {
		Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
			Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "PongMode::Vertex should be packed");

	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;

	//matrix that maps from clip coordinates to court-space coordinates:
	glm::mat3x2 clip_to_court = glm::mat3x2(1.0f);
	// computed in draw() as the inverse of OBJECT_TO_CLIP
	// (stored here so that the mouse handling code can use it to position the paddle)

};
