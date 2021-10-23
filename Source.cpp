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

void setColor(int color) {
	if (color == 1) {
		glColor3f((rand() % 101) / 100.0, (rand() % 101) / 100.0, (rand() % 101) / 100.0);
	}
	else {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void setCoordinate(int a, float x, float y, float deformation) {
	if (deformation == 1) {
		float rangeOfError = 0.20f;
		//Skala perturbacji wyra�ona przedzia�em od 0 do 1

		float error = a * rangeOfError;
		int intErrorTimes1000 = (int)(error * 1000);
		if (intErrorTimes1000 != 0) {
			x = x + (rand() % (2 * intErrorTimes1000) - intErrorTimes1000) / 1000.0f;
			y = y + (rand() % (2 * intErrorTimes1000) - intErrorTimes1000) / 1000.0f;
		}
	}
	glVertex2f(x, y);
}



void createRect(float x1, float y1, float x2, float y2, int a, int deformation, int color) {
	glBegin(GL_POLYGON);
	setColor(color);
	setCoordinate(a, x1, y1, deformation);
	setColor(color);
	setCoordinate(a, x2, y1, deformation);
	setColor(color);
	setCoordinate(a, x2, y2, deformation);
	setColor(color);
	setCoordinate(a, x1, y2, deformation);
	glEnd();
}

void divideRect(float x1, float y1, int a, int n, int deformation, int color) {
	if (n == 0) return;
	n--;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) continue;

			float newX1 = x1 + (i * a);
			float newY1 = y1 - (j * a);

			divideRect(newX1, newY1, a / 3, n, deformation, color);

			if (n == 0) {
				createRect(newX1, newY1, newX1 + a, newY1 - a, a, deformation, color);
			}
		}
	}
}

void initializeCarpet(int a, int n, int deformation, int color) {
	float x1 = a / (-2.0f);
	float y1 = a / (2.0f);

	divideRect(x1, y1, a / 3, n, deformation, color);
}

void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	int n, def, color;
	int a = 729;

	do {
		cout << "Ile krokow ma wykonac algorytm?" << endl;
		cin >> n;
	} while (n < 0);
	// Odcztanie liczby krok�w wpisanej przez u�ytkownika

	do {
		cout << "Czy dywan ma byc zdeformowany? (0 - nie; 1 - tak)?" << endl;
		cin >> def;
	} while (def < 0 || def > 1);

	do {
		cout << "Czy dywan ma byc kolorowy? (0 - nie; 1 - tak)?" << endl;
		cin >> color;
	} while (color < 0 || color > 1);

	srand(time(NULL));
	initializeCarpet(a, n, def, color);

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

		glOrtho(-500.0, 500.0, -500.0 / AspectRatio, 500.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-500.0 * AspectRatio, 500.0 * AspectRatio, -500.0, 500.0, 1.0, -1.0);

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

	glutReshapeWindow(1000, 1000);

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