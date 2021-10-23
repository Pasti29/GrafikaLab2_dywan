//*********************************************************************
//
//  PLIK èR”D£OWY:		Source.cpp
//
//  OPIS:				Program do Laboratorium nr. 2 rysujπcy na
//						ekranie Dywan SierpiÒskiego.
//
//  AUTOR:				Karol Pastewski, 252798@student.edu.pl
//
//  DATA				17.10.2021r.
//	MODYFIKACJI:
//
//  PLATFORMA:			System operacyjny:  Microsoft Windows 11
//						årodowisko:         Microsoft Visual 2019
//
//  MATERIA£Y			Dokumentacja OpenGL
//	èR”D£OWE:			Dokumentacja GLUT
//						www.zsk.ict.pwr.wroc.pl
//		
//  UØYTE BIBLIOTEKI	Nie uøywano.
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
// Funkcja losujπca kolor lub ustawiajπcy go na bia≥y zaleønie od wyboru uøytkownika
// podczas uruchomienia siÍ programu.
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
// Funkcja przesuwajπca podane wspÛ≥rzÍdne x i y o ustalonπ skalÍ perturbacji.
//
void setCoordinate(int a, float& x, float& y) {

	float rangeOfError = 0.20f;
	//
	//Skala perturbacji wyraøona przedzia≥em od 0 do 1
	//

	float error = a * rangeOfError;
	int intErrorTimes1000 = (int)(error * 1000);
	if (intErrorTimes1000 != 0) {
		x = x + (rand() % (2 * intErrorTimes1000) - intErrorTimes1000) / 1000.0f;
		y = y + (rand() % (2 * intErrorTimes1000) - intErrorTimes1000) / 1000.0f;
	}
}

//
// Funkcja rysujπca na ekranie kwadrat. Przyjmuje dwie wspÛ≥rzÍdne, ktÛre odpowiadajπ
// za wspÛ≥rzÍdne gÛrnego lewego wierzcho≥ka, bok kwadratu oraz dwie opcje okreúlajπcy
// czy kwadrat ma zostaÊ przesuniÍty oraz czy ma byÊ kolorowy.
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
// Funkcja rekurencyjna, ktÛra dzieli kwadrat na 9 mniejszych kwadratÛw i powtarza
// ten proces aø  n bÍdzie rÛwne 0.
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

// Funkcja inicjalizujπca poczπtkowe wspÛ≥rzÍdne pierwszego kwadratu, czyli jego
// gÛrny lewy wierzcho≥ek. Reszta wierzcho≥kÛw jest obliczana uøywajπc tego
// wierzcho≥ka oraz d≥ugoúci boku kwadratu.
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
	// Przekazanie poleceÒ rysujπcych do wykonania
	//

}

//
// Funkcja ustalajπca stan renderowania
//
void MyInit(void) {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//
	// Kolor okna wnÍtrza okna - ustawiono na szary
	//

}

//
// Funkcja s≥uøπca do kontroli zachowania proporcji rysowanych obiektÛw
// niezaleønie od rozmiarÛw okna graficznego
//
// Parametry horizontal i vertical (szerokoúÊ i wysokoúÊ okna) sπ
// przekazywane do funkcji za kaødym razem, gdy zmieni siÍ rozmiar okna
//
void ChangeSize(GLsizei horizontal, GLsizei vertical) {

	GLfloat AspectRatio;
	//
	// Deklaracja zmiennej AspectRatio okreúlajπcej proporcjÍ wymiarÛw okna
	//



	if (vertical == 0)
		//
		// Zabezpieczenie pzred dzieleniem przez 0
		//

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	//
	// Ustawienie wielkoúci okna urzπdzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)
	//

	glMatrixMode(GL_PROJECTION);
	//
	// Okreúlenie uk≥adu wspÛ≥rzÍdnych obserwatora
	//

	glLoadIdentity();
	//
	// Okreúlenie przestrzeni ograniczajπcej
	//

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	//
	// Wyznaczenie wspÛ≥czynnika proporcji okna
	//
	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okreúlenie okna obserwatora.
	// Pozwala to zachowaÊ w≥aúciwe proporcje rysowanego obiektu
	// Do okreúlenia okna obserwatora s≥uøy funkcja glOrtho(...)
	//



	if (horizontal <= vertical)

		glOrtho(-500.0, 500.0, -500.0 / AspectRatio, 500.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-500.0 * AspectRatio, 500.0 * AspectRatio, -500.0, 500.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	//
	// Okreúlenie uk≥adu wspÛ≥rzÍdnych
	//

	glLoadIdentity();

}


//
// G≥Ûwny punkt wejúcia programu. Program dzia≥a w trybie konsoli
//
void main(void) {

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	//
	// Ustawienie trybu wyúwietlania
	// GLUT_SINGLE - pojedynczy bufor wyúwietlania
	// GLUT_RGBA - model kolorÛw RGB
	//



	glutCreateWindow("Dywan SierpiÒskiego z perturbacjami | 252798");
	//
	// Utworzenie okna i okreúlenie treúci napisu w nag≥Ûwku okna
	//

	glutReshapeWindow(800, 800);
	//
	// Zmiana rozmiaru okna na 800px na 800px
	//

	glutDisplayFunc(RenderScene);
	//
	// Okreúlenie, øe funkcja RenderScene bÍdzie funkcjπ zwrotnπ (callback)
	// Biblioteka GLUT bÍdzie wywo≥ywa≥a tπ funkcjÍ za kaødym razem, gdy
	// trzeba bÍdzie przerysowaÊ okno
	//


	glutReshapeFunc(ChangeSize);
	//
	// Dla aktualnego okna ustala funkcjÍ zwrotnπ odpowiedzialnπ za
	// zmiany rozmiaru okna
	//

	MyInit();
	//
	// Funkcja MyInit (zdefiniowana powyøej) wykonuje wszelkie 
	// inicjalizacje konieczne przed przystπpieniem do renderowania
	//


	glutMainLoop();
	//
	// Funkcja uruchamia szkielet biblioteki GLUT
	//

}