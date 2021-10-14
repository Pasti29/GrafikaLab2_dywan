/*************************************************************************************/

// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>
#include <iostream>

using namespace std;


/*************************************************************************************/

//Funkcja losuj�ca kolory
void randomColor() {
	glColor3f((rand() % 101) / 100.0, (rand() % 101) / 100.0, (rand() % 101) / 100.0);
}

// Funkcja tworz�ca perturbacje we wsp�rz�dnych kwadratu
float randomPosition(float a, float vertex) {
	float offset = (a * 20) / 100.0f;
	int intOffsetTimes1000 = (int)(offset * 1000);
	if (intOffsetTimes1000 == 0) return vertex;
	return vertex + (rand() % (2 * intOffsetTimes1000) - intOffsetTimes1000) / 1000.0f;
}

void choseVertex(float a, float x, float y, float deformation) {
	if (deformation == 1) {
		float offset = (a * 20) / 100.0f;
		int intOffsetTimes1000 = (int)(offset * 1000);
		if (intOffsetTimes1000 != 0) {
			x = x + (rand() % (2 * intOffsetTimes1000) - intOffsetTimes1000) / 1000.0f;
			y = y + (rand() % (2 * intOffsetTimes1000) - intOffsetTimes1000) / 1000.0f;
		}
	}
	glVertex2f(x, y);
}

void choseColor(int color) {
	if (color == 1) {
		glColor3f((rand() % 101) / 100.0, (rand() % 101) / 100.0, (rand() % 101) / 100.0);
	}
	else {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void newCreateRect(float x1, float y1, float x2, float y2, float a, int n, int deformation, int color) {
	if (n < 0) return;
	n--;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) continue;

			float newX1 = x1 + (i * a);
			float newY1 = y1 - (j * a);
			float newX2 = newX1 + a;
			float newY2 = newY1 - a;

			newCreateRect(newX1, newY1, newX2, newY2, a, n, deformation, color);

			if (n == 0) {
				glBegin(GL_POLYGON);
				choseColor(color);
				choseVertex(a, newX1, newY1, deformation);
				choseColor(color);
				choseVertex(a, newX2, newY1, deformation);
				choseColor(color);
				choseVertex(a, newX2, newY2, deformation);
				choseColor(color);
				choseVertex(a, newX1, newY2, deformation);
			}
		}
	}
}

void newCreateCarpet(float a, int n, int deformation, int color) {
	float x1 = a / (-2.0f);
	float y1 = a / (2.0f);
	float x2 = a / (2.0f);
	float y2 = a / (-2.0f);

	newCreateRect(x1, y1, x2, y2, a / 3.0f, n, deformation, color);
}

// Funkcja tworz�ca kwadrat, zele�nie od parametru def tworzy tak�e zak��cenia
void createRect(float x1, float y1, float x2, float y2, float a, int def, int color) {
	if (def == 1) {
		glBegin(GL_POLYGON);
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(randomPosition(a, x1), randomPosition(a, y1));
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(randomPosition(a, x2), randomPosition(a, y1));
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(randomPosition(a, x2), randomPosition(a, y2));
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(randomPosition(a, x1), randomPosition(a, y2));
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(x1, y1);
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(x2, y1);
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(x2, y2);
		if (color == 1) {
			randomColor();
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glVertex2f(x1, y2);
		glEnd();
	}
}

// Funkcja rekurencyjna tworz�ca kwadraty
void createCarpet(float x1, float y1, float x2, float y2, int n, int def, int color) {

	if (n < 0) return;
	n--;

	float a = (x2 - x1) / 3;
	// Obliczenie d�ugo�ci boku dla mniejszych kwadrat�w

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) continue;
			// Omini�cie p�tli dla �rodkowego kwadratu

			float newX1 = x1 + (i * a);
			float newY1 = y1 - (j * a);
			float newX2 = newX1 + a;
			float newY2 = newY1 - a;
			// Obliczenie wsp�rz�dnych dla nowego kwadratu

			createCarpet(newX1, newY1, newX2, newY2, n, def, color);

			if (n == 0) {
				createRect(newX1, newY1, newX2, newY2, a, def, color);
			}

		}
	}
}

void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym


	int n;
	do {
		cout << "Ile krokow ma wykonac algorytm" << endl;
		cin >> n;
	} while (n < 0);
	// Odcztanie liczby krok�w wpisanej przez u�ytkownika

	int def;
	do {
		cout << "Czy dywan ma byc zdeformowany? (0 - nie; 1 - tak)" << endl;
		cin >> def;
	} while (def < 0 || def > 1);

	int color;
	do {
		cout << "Czy dywan ma byc kolorowy? (0 - nie; 1 - tak)" << endl;
		cin >> color;
	} while (color < 0 || color > 1);

	float a = 160.0f;

	srand(time(NULL));
	createCarpet(-80.0f, 80.0f, 80.0f, -80.0f, n, def, color);

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

}

/*************************************************************************************/


// Funkcja ustalaj�ca stan renderowania
void MyInit(void) {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Kolor okna wn�trza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego

// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna
void ChangeSize(GLsizei horizontal, GLsizei vertical) {

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna



	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ci okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora

	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okre�lenie okna obserwatora.
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)



	if (horizontal <= vertical)

		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych     

	glLoadIdentity();

}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(void) {

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB



	glutCreateWindow("Dywan Sierpi�skiego z perturbacjami");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutReshapeWindow(800, 800);

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno


	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczne przed przyst�pieniem do renderowania


	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/