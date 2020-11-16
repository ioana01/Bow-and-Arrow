#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"

#define DEG2RAD 3.14159/180.0

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{

}

void Tema1::Init() {
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// punctul de referinta pentru sageata
	corner = glm::vec3(200, 500, 0);
	// punctul de referinta pentru arc
	corner2 = glm::vec3(150, 500, 0);
	// punctul de referinta pentru power button
	glm::vec3 corner3 = glm::vec3(0, 20, 0);
	// punctul de referinta pentru inimi 
	glm::vec3 corner4 = glm::vec3((float)resolution.x - 50, (float)resolution.y - 100, 0);
	// punctul de referinta pentru balonul galben
	corner5 = glm::vec3((float)resolution.x - 200, 200, 0);
	// punctul de referinta pentru baonul rosu
	corner5_2 = glm::vec3((float)resolution.x - 200, 100, 0);
	// punctul de referinta pentru shuriken
	corner6 = glm::vec3((float)resolution.x, 250, 0);
	glm::vec3 color = glm::vec3(0, 1, 1);
	glm::vec3 color2 = glm::vec3(1, 0, 0);
	// variabila folosita pentru desenare
	length = 20;
	

	// coordonatele centrului arcului
	cx = corner2.x;
	cy = corner2.y;

	// coordonatele centrului power-buttonului
	cx_power = corner3.x + length * 1.5;
	cy_power = corner3.y + length / 2;

	// coordonatele centrului shurikenului
	cx_shuriken = corner6.x;
	cy_shuriken = corner6.y;

	// varibile de translate pentru arc
	translateX = 0;
	translateY = 0;

	// varibile de translate pentru power button
	translateX_power = 0;
	translateY_power = 0;

	// varibile de translate pentru shuriken
	translateX_shuriken = 0;
	translateY_shuriken = 0;

	// varibile de translate pentru sageata 
	translateX_arrow = 0;
	translateY_arrow = 0;

	// varibile de scalare pentru arc
	scaleX = 1;
	scaleY = 1;

	// varibila de scalare pe OX pentru power button
	scaleX_power = 1;

	// variabila pentru viteza sagetii
	arrow_speed = 1;

	// variabila pentru rotirea jumatatii de arc 180 de greade pentru pozitia initiala
	angularStep = -M_PI/2;

	// variabila pentru rotirea sagetii dupa mouse
	angularStep_arrow = 0;
	// variabila pentru rotirea shurikenului
	angularStep_shuriken = 0;

	// ARROW
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length * 3, 0, 0), color),
		VertexFormat(corner + glm::vec3(length * 3, -length, 0), color),
		VertexFormat(corner + glm::vec3(length * 4, 0, 0), color),
		VertexFormat(corner + glm::vec3(length * 3, length, 0), color)
	};

	vector<unsigned short> indices{
		1, 2, 3, 4, 1, 0, 1
	};
	Mesh* arrow = new Mesh("arrow");

	arrow->SetDrawMode(GL_LINE_LOOP);

	arrow->InitFromData(vertices, indices);
	AddMeshToList(arrow);
	

	// BOW
	vector<VertexFormat> vertices_disc;
	vector<unsigned short> indices_disc;

	for (int i = 0; i <= 16; i++) {

		vertices_disc.push_back(VertexFormat(corner2 + glm::vec3(100 * cos(i * 2 * M_PI / 30), 75 * sin(i * 2 * M_PI / 30), 0), glm::vec3(1, 0, 1)));
		indices_disc.push_back(i);

		// aflam maximul distantei dintre centrul ovalului si marginea sa pentru a putea
		// incadra arcul intr-un cerc pentru coliziune 
		int dx = -(150 * cos(i * 2 * M_PI / 30));
		int dy = -(100 * sin(i * 2 * M_PI / 30));
		distance = sqrt(dx * dx + dy * dy);

		if (radius_bow < distance) {
			radius_bow = distance;
		}
	}

	Mesh* disc = new Mesh("disc");
	disc->InitFromData(vertices_disc, indices_disc);
	disc->SetDrawMode(GL_LINE_LOOP);
	AddMeshToList(disc);

	// POWER BUTTON
	std::vector<VertexFormat> vertices_power =
	{
		VertexFormat(corner3, color),
		VertexFormat(corner3 + glm::vec3(length * 3, 0, 0), color),
		VertexFormat(corner3 + glm::vec3(length * 3, length, 0), color),
		VertexFormat(corner3 + glm::vec3(0, length, 0), color)
	};

	vector<unsigned short> indices_power{
		0, 1, 2, 3
	};

	Mesh* power = new Mesh("power");

	power->SetDrawMode(GL_LINE_LOOP);

	power->InitFromData(vertices_power, indices_power);
	AddMeshToList(power);

	// HEART 
	vector<VertexFormat> vertices_heart = {
		VertexFormat(corner4, color2),
		VertexFormat(corner4 + glm::vec3(length, length, 0), color2),
		VertexFormat(corner4 + glm::vec3(length, length * 2, 0), color2),
		VertexFormat(corner4 + glm::vec3(length - 10, length * 2 + 10, 0), color2),
		VertexFormat(corner4 + glm::vec3(0, length * 2, 0), color2),
		VertexFormat(corner4 + glm::vec3(-length + 10, length * 2 + 10, 0), color2),
		VertexFormat(corner4 + glm::vec3(-length, length * 2, 0), color2),
		VertexFormat(corner4 + glm::vec3(-length, length, 0), color2)
	};
	vector<unsigned short> indices_heart = {
		0, 1, 2, 3, 4, 5, 6, 7
	};

	Mesh* heart = new Mesh("heart");

	heart->SetDrawMode(GL_TRIANGLE_FAN);
	heart->InitFromData(vertices_heart, indices_heart);
	AddMeshToList(heart);

	// YELLOW BALLOON
	vector<VertexFormat> vertices_balloon;
	vector<unsigned short> indices_balloon;
	
	distance = 0;

	for (int i = 1; i <= 32; i++) {

		vertices_balloon.push_back(VertexFormat(corner5 + glm::vec3(50 * cos(i * 2 * M_PI / 30), 70 * sin(i * 2 * M_PI / 30), 0), glm::vec3(1, 1, 0)));
		indices_balloon.push_back(i);

		// aflam maximul distantei dintre centrul ovalului si marginea sa pentru a putea
		// incadra balonul intr-un cerc pentru coliziune 
		// aceeasi distanta se foloseste si pentru balonul rosu
		int dx = - (50 * cos(i * 2 * M_PI / 30));
		int dy = - (70 * sin(i * 2 * M_PI / 30));
		distance = sqrt(dx * dx + dy * dy);

		if (radius < distance) {
			radius = distance;
		}
	}

	Mesh* balloon = new Mesh("balloon");
	balloon->InitFromData(vertices_balloon, indices_balloon);
	balloon->SetDrawMode(GL_TRIANGLE_FAN);
	AddMeshToList(balloon);

	// RED BALLOON
	vector<VertexFormat> vertices_balloon_red;
	vector<unsigned short> indices_balloon_red;

	for (int i = 1; i <= 32; i++) {

		vertices_balloon_red.push_back(VertexFormat(corner5_2 + glm::vec3(50 * cos(i * 2 * M_PI / 30), 70 * sin(i * 2 * M_PI / 30), 0), glm::vec3(1, 0, 0)));
		indices_balloon_red.push_back(i);
	}

	Mesh* balloon_red = new Mesh("balloon_red");
	balloon_red->InitFromData(vertices_balloon_red, indices_balloon_red);
	balloon_red->SetDrawMode(GL_TRIANGLE_FAN);
	AddMeshToList(balloon_red);

	// ZIG-ZAG pentru sfoara balonului galben
	vector<VertexFormat> vertices_zigzag = {
		VertexFormat(corner5 + glm::vec3(-10, -length * 2, 0), glm::vec3(1, 1, 0)),
		VertexFormat(corner5 + glm::vec3(20, -length * 3, 0), glm::vec3(1, 1, 0)),
		VertexFormat(corner5 + glm::vec3(-10, -length * 4, 0), glm::vec3(1, 1, 0)),
		VertexFormat(corner5 + glm::vec3(20, -length * 5, 0), glm::vec3(1, 1, 0)),
		VertexFormat(corner5 + glm::vec3(-10, -length * 6, 0), glm::vec3(1, 1, 0)),
	};

	// ZIG-ZAG RED pentru sfoara balonului galben
	vector<VertexFormat> vertices_zigzag_red = {
		VertexFormat(corner5_2 + glm::vec3(-10, -length * 2, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner5_2 + glm::vec3(20, -length * 3, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner5_2 + glm::vec3(-10, -length * 4, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner5_2 + glm::vec3(20, -length * 5, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner5_2 + glm::vec3(-10, -length * 6, 0), glm::vec3(1, 0, 0))
	};

	vector<unsigned short> indices_zigzag = {
		0, 1, 2, 3, 4, 3, 2, 1, 0
	};

	Mesh* zigzag = new Mesh("zigzag");
	zigzag->SetDrawMode(GL_LINE_LOOP);
	zigzag->InitFromData(vertices_zigzag, indices_zigzag);
	AddMeshToList(zigzag);

	Mesh* zigzag_red = new Mesh("zigzag_red");
	zigzag_red->SetDrawMode(GL_LINE_LOOP);
	zigzag_red->InitFromData(vertices_zigzag_red, indices_zigzag);
	AddMeshToList(zigzag_red);

	// SHURIKEN
	vector<VertexFormat> vertices_shuriken = {
		VertexFormat(corner6, glm::vec3(1, 0, 0)),

		VertexFormat(corner6 + glm::vec3(length/2, length/2, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner6 + glm::vec3(0, length, 0), glm::vec3(1, 0, 0)),

		VertexFormat(corner6 + glm::vec3(-length/2, length/2, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner6 + glm::vec3(-length, 0, 0), glm::vec3(1, 0, 0)),

		VertexFormat(corner6 + glm::vec3(-length/2, -length/2, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner6 + glm::vec3(0, -length, 0), glm::vec3(1, 0, 0)),

		VertexFormat(corner6 + glm::vec3(length / 2, -length / 2, 0), glm::vec3(1, 0, 0)),
		VertexFormat(corner6 + glm::vec3(length, 0, 0), glm::vec3(1, 0, 0)),
	};

	vector<unsigned short> indices_shuriken = {
		0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8, 0
	};

	Mesh* shuriken = new Mesh("shuriken");
	shuriken->SetDrawMode(GL_TRIANGLE_FAN);
	shuriken->InitFromData(vertices_shuriken, indices_shuriken);
	AddMeshToList(shuriken);
	
}

void Tema1::FrameStart(){
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

// verificarea coliziunilor cu baloanele 
void Tema1::verifyBalloonCollisions() {
	for (int i = 0; i < 5; i++) {
		//verificarea coordonatelor pentru baloanele galbene
		if (((corner.x + length * 4 + translateX_arrow) > (corner5.x + translateX_balloon[i] + balloonCoordinates[i] - radius)) &&
			(((corner.y + translateY_arrow) < (corner5.y + translateY_balloon[i] + balloonCoordinates[i] + radius)) && ((corner.y + translateY_arrow) > (corner5.y + translateY_balloon[i] + balloonCoordinates[i] - radius))))
		{
			collisionBalloon[i] = true;
			if (verify[i]) {
				// daca a fost lovit un balon galben scorul scade
				score -= 10;
				std::cout << "score: " << score << endl;

				verify[i] = false;
				popped[i] = true;
			}
		}
		else if (collisionBalloon[i]) {
			verify[i] = true;
			collisionBalloon[i] = false;
		}

		//verificarea coordonatelor pentru baloanele rosii
		if (((corner.x + length * 4 + translateX_arrow) > (corner5_2.x + translateX_balloon[i] + balloonCoordinates[i] - radius)) &&
			(((corner.y + translateY_arrow) < (corner5_2.y + translateY_balloon[i] + balloonCoordinates[i] + radius)) && ((corner.y + translateY_arrow) > (corner5_2.y + translateY_balloon[i] + balloonCoordinates[i] - radius))))
		{
			collisionBalloon_red[i] = true;
			if (verify_red[i]) {
				// daca a fost lovit un balon rosu scorul creste
				score += 10;
				std::cout << "score: " << score << endl;

				verify_red[i] = false;
				popped_red[i] = true;
			}
		}
		else if (collisionBalloon_red[i]) {
			verify_red[i] = true;
			collisionBalloon_red[i] = false;
		}
	}
}

// verificarea coliziunilor cu shurikenul
void Tema1::verifyShurikenCollision() {
	//verificarea coordonatelor pentru shuriken
	if(((corner2.x + radius_bow) > (corner6.x - length + translateX_shuriken)) &&
		((((corner2.y + radius_bow + translateY) > (corner6.y + length + translateY_shuriken))
		&& ((corner6.y + length + translateY_shuriken) > (corner2.y + translateY - radius_bow))) ||
			(((corner2.y + radius_bow + translateY) > (corner6.y - length + translateY_shuriken))
				&& ((corner6.y - length + translateY_shuriken) > (corner2.y + translateY - radius_bow)))))
	{
		collision_shuriken = true;
		if (verify_shuriken) {
			// daca s-a lovit un shuriken cu o sageata scorul creste
			hearts--;
			verify_shuriken = false;
		}
	}
	else if (collision_shuriken) {
		verify_shuriken = true;
		collision_shuriken = false;
	}
}

// verificarea coliziunii sagetii cu shurikenul
void Tema1::verifyShurikenArrowCollision() {
	if (((corner.x + length * 4 + translateX_arrow) > (corner6.x - length + translateX_shuriken)) &&
		(((corner.y + translateY_arrow) < (corner6.y + length + translateY_shuriken)) && ((corner.y + translateY_arrow) > (corner6.y - length + translateY_shuriken))) 
		&& released) {

		collision_shuriken_arrow = true;
		if (verify_shuriken_arrow) {
			// daca s-a lovit un shuriken se scade o viata 
			score += 10;
			std::cout << "score: " << score << endl;
			verify_shuriken_arrow = false;
		}
	}
	else if (collision_shuriken_arrow) {
		verify_shuriken_arrow = true;
		collision_shuriken_arrow = false;
	}
}

void Tema1::Update(float deltaTimeSeconds) {

	// apelam functiile pentru verificarea coliziunilor la orice moment de timp
	verifyBalloonCollisions();
	verifyShurikenCollision();
	verifyShurikenArrowCollision();

	// verificarea vietilor ramase
	if (hearts == 0) {
		heart[0] = 100;
		gameOver = true;
	}
	else if (hearts == 1) {
		heart[1] = 100;
	}
	else if (hearts == 2) {
		heart[2] = 100;
	}

	// daca un balon este lovit se va crea o animatie
	for (int i = 0; i < 5; i++) {
		// verificarea coliziunii pentru un balon galben
		if (popped[i]) {
			scaleX_balloon[i] -= deltaTimeSeconds * 5;
			scaleY_balloon[i] -= deltaTimeSeconds * 5;
			count++;

			if (count == 10) {
				popped[i] = false;
				count = 0;
			}
		}

		// verificarea coliziunii pentru un balon rosu
		if (popped_red[i]) {
			scaleX_balloon_red[i] -= deltaTimeSeconds * 5;
			scaleY_balloon_red[i] -= deltaTimeSeconds * 5;
			count_red++;

			if (count_red == 10) {
				popped_red[i] = false;
				count_red = 0;
			}
		}
	}

	// ARROW

	// daca sageata a iesit din ecran se va intoarce in pozitia de pornire
	if (translateX_arrow > resolution.x) {
		exit_arrow = true;
		translateX_arrow = 0;
		translateY_arrow = translateY;
		released = false;
		scaleX_power = 1;
	}

	// daca sageata a fost lansata, sageata se va deplasa cu viteza obtinuta tinand apasat mouseul
	if (released) {
		translateX_arrow += 100 * deltaTimeSeconds * scaleX_power;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(translateX_arrow, translateY_arrow);

	// daca sageata e inca in arc, aceasta se va roti odata cu arcul in functie de miscarea mouseului
	if (!released) {
		modelMatrix *= Transform2D::Translate(cx, cy);
		modelMatrix *= Transform2D::Rotate(angularStep_arrow);
		modelMatrix *= Transform2D::Translate(-cx, -cy);
	}
	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);

	// BOW
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(translateX, translateY);
	modelMatrix *= Transform2D::Translate(cx, cy);
	modelMatrix *= Transform2D::Rotate(angularStep);
	modelMatrix *= Transform2D::Translate(-cx, -cy);
	RenderMesh2D(meshes["disc"], shaders["VertexColor"], modelMatrix);

	// POWER_BUTTON
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(translateX_power, translateY_power);

	// scalarea power buttonului pe axa Ox pana la o limita
	if (scaleX_power < 6) {
		modelMatrix *= Transform2D::Translate(cx_power, cy_power);
		modelMatrix *= Transform2D::Scale(scaleX_power, 1);
		modelMatrix *= Transform2D::Translate(-cx_power, -cy_power);
	}
	// daca mouseul e in continuare tinut apasat si s-a ajuns la limita anterioara
	// butonul e mentinut la o dimensiune constanta
	else if (wasPressed) {
		modelMatrix *= Transform2D::Translate(cx_power, cy_power);
		modelMatrix *= Transform2D::Scale(6, 1);
		modelMatrix *= Transform2D::Translate(-cx_power, -cy_power);
	}
	RenderMesh2D(meshes["power"], shaders["VertexColor"], modelMatrix);

	// HEARTS
	modelMatrix = glm::mat3(1);
	// acelasi mesh pentru inima este afisat de 3 ori
	for (int i = 0; i < 3; i++) {
		modelMatrix *= Transform2D::Translate(translateX_heart[i], heart[i]);
		RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
	}

	time += deltaTimeSeconds;

	// daca jocul nu s-a terminat (jucatorul mai are vieti)
	// se creeaza o animatie de deplasare in zig-zag pentru baloane
	if (!gameOver) {
		for (int i = 0; i < 5; i++) {
			if (((int)time % 2) == 0) {
				translateX_balloon[i] += deltaTimeSeconds * 100;
			}
			else {
				translateX_balloon[i] -= deltaTimeSeconds * 100;
			}
			translateY_balloon[i] += deltaTimeSeconds * 60;

			// daca baloanele au iesit din ecran, se vor intoarce in pozitia de unde au plecat 
			// pentru a crea un flux continuu de baloane 
			if (translateY_balloon[i] >= (float)resolution.y) {
				translateY_balloon[i] = 0;
				scaleX_balloon[i] = 1;
				scaleY_balloon[i] = 1;
				scaleX_balloon_red[i] = 1;
				scaleY_balloon_red[i] = 1;
			}
		}
	}

	// YELLOW BALLOON

	// acelasi mesh pentru balonul galben e afisat de 5 ori in pozitii diferite 
	for (int i = 0; i < 5; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(translateX_balloon[i], translateY_balloon[i] + balloonCoordinates[i]);
		modelMatrix *= Transform2D::Translate(corner5.x, corner5.y);
		modelMatrix *= Transform2D::Scale(scaleX_balloon[i], scaleY_balloon[i]);
		modelMatrix *= Transform2D::Translate(-corner5.x, -corner5.y);
		RenderMesh2D(meshes["balloon"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["zigzag"], shaders["VertexColor"], modelMatrix);
	}
	
	

	// RED_BALLOON

	// acelasi mesh pentru balonul rosu e afisat de 5 ori in pozitii diferite 
	for (int i = 0; i < 5; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(-translateX_balloon[i], translateY_balloon[i] + balloonCoordinates[i]);
		modelMatrix *= Transform2D::Translate(corner5_2.x, corner5_2.y);
		modelMatrix *= Transform2D::Scale(scaleX_balloon_red[i], scaleY_balloon_red[i]);
		modelMatrix *= Transform2D::Translate(-corner5_2.x, -corner5_2.y);
		RenderMesh2D(meshes["balloon_red"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["zigzag_red"], shaders["VertexColor"], modelMatrix);
	}

	// SHURIKEN

	// daca shurikenul a iesit din ecran se intoarce in partea dreapta a ecranului
	if (translateX_shuriken < -resolution.x) {
		exit = true;
		translateX_shuriken = 0;
	}

	modelMatrix = glm::mat3(1);

	// daca shurikenul a isit din ecran se intoarce in partea dreapta a ecranului la 
	// o pozitie aleasa random din cele 4
	if (exit) {
		int t = rand() % 4;
		if (t == 0) {
			translateY_shuriken = 50;
		}
		else if (t == 1) {
			translateY_shuriken = 350;
		}
		else if (t == 2) {
			translateY_shuriken = 0;
		}
		else if (t == 3) {
			translateY_shuriken = 200;
		}
		exit = false;
	}

	// daca jocul nu s-a terminat, shurikenele se deplaseaza in continuare
	if (!gameOver) {
		angularStep_shuriken += deltaTimeSeconds * 5;
		translateX_shuriken -= deltaTimeSeconds * 400;
	}

	modelMatrix *= Transform2D::Translate(translateX_shuriken, translateY_shuriken);
	modelMatrix *= Transform2D::Translate(cx_shuriken, cy_shuriken);
	modelMatrix *= Transform2D::Rotate(angularStep_shuriken);
	modelMatrix *= Transform2D::Translate(-cx_shuriken, -cy_shuriken);
	RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// daca se apasa tastele W sau S, arcul si sageata se deplaseaza pe OY
	if (window->KeyHold(GLFW_KEY_W)) {
		translateY += 300 * deltaTime;

		if (!released) {
			translateY_arrow += 300 * deltaTime;
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		translateY -= 300 * deltaTime;

		if (!released) {
			translateY_arrow -= 300 * deltaTime;
		}
	}

	// daca se tine mouse-ul apasat, se scaleaza power buttonul
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		scaleX_power += deltaTime * 2;
	}
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	mouseOY = mouseY;
	mouseOX = mouseX;

	// daca se misca mouseul pe axa Oy, se rotesc arcul si sageata
	if (deltaY < 0) {
		angularStep += 0.03;
		angularStep_arrow += 0.03;
	}
	else if(deltaY > 0){
		angularStep -= 0.03;
		angularStep_arrow -= 0.03;
		mouseMove = -1;
	}
	else {
		angularStep -= 0;
		angularStep_arrow -= 0;
		mouseMove = 0;
	}
	
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// se verifica daca mouseul e tinut pasat
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		// se modifica scalarea butonului pana la o limita
		if (scaleX_power < 2) {
			wasPressed = true;
			scaleX_power += 0.2;
			arrow_speed += 0.2;
		}
	}
	
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// se verifica daca mouseul nu mai e apasat pentru cabutonul sa revina la scalarea initiala 
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		released = true;
	}

	posX = corner.x + translateX;
	posY = corner.y + translateY;
}