/*************************************************************************************/

// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>


/*************************************************************************************/

void randomColor() {
	glColor3f((rand() % 101) / 100.0, (rand() % 101) / 100.0, (rand() % 101) / 100.0);
}

float randomPosition(float a, float vertex) {
	float offset = (a * 10) / 100.0f;
	int intOffsetTimes1000 = (int)(offset * 1000);
	if (intOffsetTimes1000 == 0) return vertex;
	return vertex + (rand() % (2 * intOffsetTimes1000) - intOffsetTimes1000) / 1000.0f;
}

void createRect(float x1, float y1, float x2, float y2, int n) {
	
	if (n < 0) return;
	n--;
	
	float a = (x2 - x1) / 3;
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) continue;

			float newX1 = x1 + (i * a);
			float newY1 = y1 - (j * a);
			float newX2 = newX1 + a;
			float newY2 = newY1 + a;

			createRect(newX1, newY1, newX2, newY2, n);

			if (n == 0) {
				glBegin(GL_POLYGON);
				randomColor();
				glVertex2f(randomPosition(a, newX1), randomPosition(a, newY1));
				randomColor();
				glVertex2f(randomPosition(a, newX2), randomPosition(a, newY1));
				randomColor();
				glVertex2f(randomPosition(a, newX2), randomPosition(a, newY2));
				randomColor();
				glVertex2f(randomPosition(a, newX1), randomPosition(a, newY2));
				glEnd();
			}
			
		}
	}
}

void RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT);

	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	srand(time(NULL));
	createRect(-800.0f, 800.0f, 800.0f, -800.0f, 4);

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

}

/*************************************************************************************/


// Funkcja ustalaj�ca stan renderowania
void MyInit(void) {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Kolor okna wn�trza okna - ustawiono na szary
	glutReshapeWindow(1000, 1000);

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

		glOrtho(-1000.0, 1000.0, -1000.0 / AspectRatio, 1000.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-1000.0 * AspectRatio, 1000.0 * AspectRatio, -1000.0, 1000.0, 1.0, -1.0);

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



	glutCreateWindow("Dywan Sierpi�skiego");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

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