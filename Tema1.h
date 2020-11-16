#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <cmath> 

class Tema1 : public SimpleScene {
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void verifyBalloonCollisions();
		void verifyShurikenCollision();
		void verifyShurikenArrowCollision();

	protected:
		glm::mat3 modelMatrix;
		glm::mat3 modelMatrix_heart;

		float translateX, translateY, translateX_power, translateY_power, translateX_shuriken, translateY_shuriken;
		float angularStep, angularStep_arrow, angularStep_shuriken;

		// vectori de transatie/scalare pentru obiectele afisate de mai multe ori in pozitii diferite
		float translateX_balloon[5] = { 0, 0, 0, 0, 0 }, translateY_balloon[5] = { 0, 0, 0, 0, 0 }, translateX_arrow, translateY_arrow;
		float scaleX, scaleY, scaleX_power, scaleX_balloon[5] = { 1, 1, 1, 1, 1 }, scaleY_balloon[5] = { 1, 1, 1, 1, 1 };
		float scaleX_balloon_red[5] = { 1, 1, 1, 1, 1 }, scaleY_balloon_red[5] = { 1, 1, 1, 1, 1 };
		float translateX_heart[3] = { 0, -60, -60 }, heart[3] = { 0, 0, 0 };

		float cx, cy, cx_power, cy_power, cx_shuriken, cy_shuriken;
		bool fill = true, wasPressed = false;
		float time = 0;
		float t = 0;
		glm::ivec2 resolution;
		int p;
		bool exit = false, released = false, exit_arrow = false, initial = false;
		int arrow_speed;
		glm::vec3 corner, corner5, corner5_2, corner2, corner6;
		float length;
		float radius = 0, distance, radius_bow = 0;
		int score = 0;

		// vectori de verificare a coliziunii pentru baloane si shurikene
		bool verify[5] = { true, true, true, true, true }, collisionBalloon[5] = { false, false, false, false, false };
		bool verify_red[5] = { true, true, true, true, true }, collisionBalloon_red[5] = { false, false, false, false, false };
		bool verify_shuriken = true, collision_shuriken = false, popped[5] = { false, false, false, false, false };
		bool verify_shuriken_arrow = true, collision_shuriken_arrow = false;
		bool popped_red[5] = { false, false, false, false, false };

		int count = 0, count_red = 0;
		int balloonCoordinates[5] = { -750, -500, -250, 0, 250 };
		int hearts = 3;
		bool gameOver = false;
		float mouseOY, mouseOX, posX, posY;
		int mouseMove, countFrame = 0;
};