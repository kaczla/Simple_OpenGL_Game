## Simple_OpenGL_Game

### Opis:

Projekt przedstawia przykładowe wykorzystanie grafiki 3D w OpenGL.</br>
Stworzony dla zajęć **Grafika komputerowa i wizualizacja** w 2015 roku.</br>
Wersja druga, odchudzona znajduje się pod linkiem: [LINK](https://github.com/kaczla/Simple_OpenGL_Slim)
</br>
Możesz zobaczyć również: [LINK](https://github.com/kaczla/OpenGL)
</br>
</br>
Prosta gra, w której zbieramy leżące monety na mapie.
</br>
</br>

### Testowane na:

**Ubuntu 15.04**
</br>
x64
</br>
g++ (Ubuntu 4.9.2-10ubuntu13) 4.9.2
</br>
</br>
**Windows 7 SP1**
</br>
x64
</br>
[MinGW + MSYS](http://www.mingw.org/)
</br>
g++ (GCC) 4.8.1
</br>
</br>
Testowano na kartach graficznych: NVIDIA GeForce 710M, Intel HD Graphics 4000.
</br>
</br>

### Wymagania:

1) **Zaktualizaowane sterowniki graficzne**</br>
2) **[SDL2](https://www.libsdl.org)**</br>
3) **[GLEW](http://www.glew.sourceforge.net)**</br>
4) **[GLM](http://www.glm.g-truc.net)**</br>
5) **[DevIL](http://www.openil.sourceforge.net)**</br>
6) **[Assimp](http://assimp.sourceforge.net)**</br>

#### Pakiety dla Ubuntu:

sudo apt-get install libsdl2-dev glew-utils libglew-dev libglm-dev libdevil-dev libassimp-dev
</br>
</br>

### Budowanie:

Zbudować poleceniem **make** w głównym katalogu repozytorium.
</br>
</br>

### Uruchomienie:

Uruchomić aplikację **game.app**\*\* w głównym katalogu repozytorium ( wcześniej budując ! ).
</br>
\*\* Dla systemu Windows, należy uruchomić aplikację **game.exe**.
</br>
</br>

### Sterowanie:

**W** / **Strzałka w górę** - Poruszanie się do porzodu (przed siebie)
</br>
**S** / **Strzałka w dół** - Poruszanie się do tyłu
</br>
**A** / **Strzałka w lewo** - Poruszanie się w lewo
</br>
**D** / **Strzałka w prawo** - Poruszanie się w prawo
</br>
**E** / **Lewy przycisk myszy** - Zbieranie pobliskich monet
</br>
**Prawy przycisk myszy** - Zbliżenie / Zoom
</br>
**F7** - Włączenie / Wyłączenie trybu wolnej kamery
</br>
**F10** - Powrót na domyślną pozycję
</br>
**F12** - Włączenie / Wyłączenie trybu pełnoekranowego
</br>
**SPACJA** - Poruszanie się do góry w trybie wolnej kamery
</br>
**C** / **Lewy CTRL** - Poruszanie się w dół w trybie wolnej kamery
</br>
**8** (na klawiaturze numerycznej) - Rozglądanie się do góry
</br>
**2** (na klawiaturze numerycznej) - Rozglądanie się w dół
</br>
**4** (na klawiaturze numerycznej) - Rozglądanie się w lewo
</br>
**6** (na klawiaturze numerycznej) - Rozglądanie się w prawo
</br>
**ESC** - wyjście z gry
</br>
</br>

### Krótka dokumentacja:

Znajduje się w katalogu **doxygen/html/****[index.html](doxygen/html/index.html)**
</br>
</br>

### Dodawanie kolejnych elementów 3D:

Każdy obiekt musi posiadać (w folderze **data**):
<ul>
<li>plik .obj</li>
<li>plik .mtl</li>
<li>teksture główną</li>
<li>teksture spektralną</li>
</ul>

Dodać ww pliki do pliku **data.init** w katalogu **data** według kolejności:

> NAZWA_OBIEKTU PLIK_.OBJ PLIK_Z_MATERIAŁEM PLIK_GŁÓWNEJ_TEKSTURY PLIK_SPEKTRALNEJ_TEKSTURY

np.

> tree tree.obj tree.mtl tree.jpg tree.jpg

</br>
</br>

### Screenshot:

![screenshot_1](screenshot/1.jpg "screenshot_1")
</br>
![screenshot_2](screenshot/2.jpg "screenshot_2")
</br>
![screenshot_3](screenshot/3.jpg "screenshot_3")
</br>
![screenshot_4](screenshot/4.jpg "screenshot_4")
</br>
</br>

### Uwagi:

Pierwsze zetknięcie z OpenGL.
</br>
Część plików .obj i tekstur zostało pobranych z [tf3dm.com](http://tf3dm.com).
</br>
</br>
