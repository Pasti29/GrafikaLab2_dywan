//*********************************************************************
//
//  PLIK �R�D�OWY:		Source.cpp
//
//  OPIS:				Program do Laboratorium nr. 2 rysuj�cy na
//						ekranie Dywan Sierpi�skiego.
//
//  AUTOR:				Karol Pastewski, 252798@student.edu.pl
//
//  DATA				17.10.2021r.
//	MODYFIKACJI:
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 11
//						�rodowisko:         Microsoft Visual 2019
//
//  MATERIA�Y			Dokumentacja OpenGL
//	�R�D�OWE:			Dokumentacja GLUT
//						www.zsk.ict.pwr.wroc.pl
//		
//  U�YTE BIBLIOTEKI	Nie u�ywano.
//  NIESTANDARDOWE
//		
//*********************************************************************

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>
#include <iostream>

using namespace std;

//
// Funkcja losuj�ca kolor lub ustawiaj�cy go na bia�y zale�nie od wyboru u�ytkownika
// podczas uruchomienia si� programu.
//
void setColor(int color) {
	if (color == 1) {
		glColor3f((rand() % 101) / 100.0, (rand() % 101) / 100.0, (rand() % 101) / 100.0);
	}
	else {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

//
// Funkcja przesuwaj�ca podane wsp�rz�dne x i y o ustalon� skal� perturbacji.
//
void setCoordinate(int a, float& x, float& y) {

	float rangeOfError = 0.20f;
	//
	//Skala perturbacji wyra�ona przedzia�em od 0 do 1
	//

	float error = a * rangeOfError;
	int intErrorTimes1000 = (int)(error * 1000);
	if (intErrorTimes1000 != 0) {
		x = x + (rand() % (2 * intErrorTimes1000) - intErrorTimes1000) / 1000.0f;
		y = y + (rand() % (2 * intErrorTimes1000) - intErrorTimes1000) / 1000.0f;
	}
}

//
// Funkcja rysuj�ca na ekranie kwadrat. Przyjmuje dwie wsp�rz�dne, kt�re odpowiadaj�
// za wsp�rz�dne g�rnego lewego wierzcho�ka, bok kwadratu oraz dwie opcje okre�laj�cy
// czy kwadrat ma zosta� przesuni�ty oraz czy ma by� kolorowy.
//
void createRect(float x, float y, int a, int deformation, int color) {

	glBegin(GL_POLYGON);
	setColor(color);
	if (deformation == 1) {
		setCoordinate(a, x, y);
	}
	glVertex2f(x, y);
	setColor(color);
	glVertex2f(x + a, y);
	setColor(color);
	glVertex2f(x + a, y - a);
	setColor(color);
	glVertex2f(x, y - a);
	glEnd();
}

//
// Funkcja rekurencyjna, kt�ra dzieli kwadrat na 9 mniejszych kwadrat�w i powtarza
// ten proces a�  n b�dzie r�wne 0.
//
void divideRect(float x1, float y1, int a, int n, int deformation, int color) {
	if (n == 0 || a < 1) return;
	n--;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) continue;

			float newX1 = x1 + (i * a);
			float newY1 = y1 - (j * a);

			divideRect(newX1, newY1, a / 3, n, deformation, color);

			if (n == 0) {
				createRect(newX1, newY1, a, deformation, color);
			}
		}
	}
}

// Funkcja inicjalizuj�ca pocz�tkowe wsp�rz�dne pierwszego kwadratu, czyli jego
// g�rny lewy wierzcho�ek. Reszta wierzcho�k�w jest obliczana u�ywaj�c tego
// wierzcho�ka oraz d�ugo�ci boku kwadratu.
void initializeCarpet(int a, int n, int deformation, int color) {
	float x1 = a / (-2.0f);
	float y1 = a / (2.0f);

	divideRect(x1, y1, a / 3, n, deformation, color);
}

void RenderScene(void) {

	system("cls");
	glClear(GL_COLOR_BUFFER_BIT);

	int n, def, color;
	int a = 729;

	cout << "Dywan Sierpinskiego" << endl;
	cout << "Dlugosc poczatkowego kwadratu:             729 px" << endl;
	cout << "Krok, w ktorym figura zmiknie z obrazu:    7" << endl << endl;

	do {
		cout << "Ile krokow ma wykonac algorytm? (minimum to 1)" << endl;
		cin >> n;
	} while (n < 1);

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
	cout << endl << "Algorytm sie wykonal";


	glFlush();
	//
	// Przekazanie polece� rysuj�cych do wykonania
	//

}

//
// Funkcja ustalaj�ca stan renderowania
//
void MyInit(void) {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//
	// Kolor okna wn�trza okna - ustawiono na szary
	//

}

//
// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego
//
// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna
//
void ChangeSize(GLsizei horizontal, GLsizei vertical) {

	GLfloat AspectRatio;
	//
	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna
	//



	if (vertical == 0)
		//
		// Zabezpieczenie pzred dzieleniem przez 0
		//

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	//
	// Ustawienie wielko�ci okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)
	//

	glMatrixMode(GL_PROJECTION);
	//
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora
	//

	glLoadIdentity();
	//
	// Okre�lenie przestrzeni ograniczaj�cej
	//

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	//
	// Wyznaczenie wsp�czynnika proporcji okna
	//
	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okre�lenie okna obserwatora.
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)
	//



	if (horizontal <= vertical)

		glOrtho(-500.0, 500.0, -500.0 / AspectRatio, 500.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-500.0 * AspectRatio, 500.0 * AspectRatio, -500.0, 500.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	//
	// Okre�lenie uk�adu wsp�rz�dnych
	//

	glLoadIdentity();

}


//
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
//
void main(void) {

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	//
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB
	//



	glutCreateWindow("Dywan Sierpi�skiego z perturbacjami | 252798");
	//
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna
	//

	glutReshapeWindow(800, 800);
	//
	// Zmiana rozmiaru okna na 800px na 800px
	//

	glutDisplayFunc(RenderScene);
	//
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno
	//


	glutReshapeFunc(ChangeSize);
	//
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna
	//

	MyInit();
	//
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczne przed przyst�pieniem do renderowania
	//


	glutMainLoop();
	//
	// Funkcja uruchamia szkielet biblioteki GLUT
	//

}