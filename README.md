CVK 2 Framework  
_Autoren: Kevin Keul, Detlev Droege, Gerrit Lochmann (2016)_  


Es handelt sich um das Framework, welches die Vorlesung "Computergrafik 2" begleitet. Das Framework enthält alles, was Sie zur Erstellung von Programmen mit OpenGL 3.3 oder höher benötigen. Dazu gehören unter anderem Klassen für Geometrie, Shader, Kameras, Projektionen und Texturen.
Eine Dokumentation befindet sich unter folgendem Link: [http://userpages.uni-koblenz.de/~cg/cvk/](http://userpages.uni-koblenz.de/~cg/cvk/).  

Mithilfe von CMake werden die Build-Umgebungen unabhängig von dem vorhandenen Betriebssystem erzeugt. Bei Versionierung mit SVN oder GIT muss nur der Quellcode versioniert werden. Die vom Setup-Script von CMake erzeugten BUILD_*-Verzeichnisse werden automatisch auf jedem System erzeugt und sollten daher nicht versioniert werden.  

Es ist möglich anstatt Eclipse auch andere IDEs zu verwenden. In der Klausur
wird jedoch ausschließlich Eclipse zur Verfügung gestellt.


----------------------------------------------------------------------------



## Installation

#### Windows

Voraussetzungen:
1. CMake muss installiert sein und das Verzeichnis von _cmake.exe_ im PATH eingetragen sein.
2. Eine der folgenden IDEs muss installiert sein:
   - Visual Studio (2010, 2012, 2013, 2015, 2017)
   - Eclipse CDT mit MinGW Compiler (Wird in der Vorlesung verwendet)
3. Die verwendeten Bibliotheken müssen installiert sein: GLM, GLFW3, GLEW, ASSIMP.
   Diese Dependencies können hier heruntergeladen werden: [https://owncloud.uni-koblenz-landau.de/owncloud/index.php/s/NtWWVb2ITT2wB3x](https://owncloud.uni-koblenz-landau.de/owncloud/index.php/s/NtWWVb2ITT2wB3x).
   Führen Sie die darin vorhandene _SetEnvironment.bat_ aus.
   Anschließend sollten folgende Umgebungsvariablen gesetzt sein:
   ```
   OpenGL_ROOT
   GLM_ROOT
   GLFW3_ROOT
   ASSIMP_ROOT
   ```
   Das Verzeichnis _dependencies_ kann an einen beliebigen Pfad verschoben werden (z.B. um Redundanzen zu vermeiden), sofern die Umgebungsvariablen GLFW3\_ROOT, GLM\_ROOT, OpenGL\_ROOT und ASSIMP\_ROOT korrekt gesetzt werden. Das Script _SetEnvironment.bat_ führt dies für Sie aus.

Zur Generierung des Projektes, führen Sie _setup.bat_ aus und wählen Sie die von Ihnen verwendete IDE.



#### Mac

Voraussetzungen:
1. [XCode](https://developer.apple.com/xcode/) installieren inkl. Command Line Interface (CLI)
2. Als IDE sollte installiert sein:
   - [Eclipse](http://eclipse.org) und das zusätzliche CDT-Modul für C++
   - (optional) das Plugin Subversive (oder SubClipse) für SVN
3. CMake, GLEW, GLFW (Version 3.*), GLM und Assimp müssen installiert werden.
   Das geht am einfachsten mit den Paket-Verwaltungssystemen _MacPorts_ oder _Homebrew_, mit denen sich viele der unter Linux existierenden Bibliotheken und Programme unter Mac OS X installieren lassen.
   - Homebrew:  
     Die Anleitung zur Installation lässt sich unter [http://brew.sh/](http://brew.sh/) finden.
     Derzeit (Mai 2017) gehört GLFW3 nicht zur Standard-Paketliste von Homebrew, weshalb ein weiteres Kommando benötigt wird:  
     ``` brew tap homebrew/versions ```  
     Anschließend findet die Installation der Bibliotheken statt:  
     ``` brew install cmake glew glfw3 glm assimp ```
   - MacPorts:  
     Die Installation ist unter [https://www.macports.org/install.php](https://www.macports.org/install.php) beschrieben.  
     Installation der Bibliotheken: ```sudo port install cmake glew glfw glm assimp```.

Zur Generierung des Projektes, führen Sie `sh ./setupmac.sh` in einem Terminal-Fenster im Verzeichnis auf.



#### Linux

ArchLinux:
```bash
sudo pacman -S git gcc cmake glew glfw glm assimp eclipse-cpp
```

Ubuntu:
```bash
sudo add-apt-repository universe
sudo apt-get update
sudo apt-get install git gcc cmake glew-utils libglew-dev libglfw3-dev libglm-dev libassimp-dev eclipse
```

Generierung des Projektes:
```bash
git clone "https://gitlab.uni-koblenz.de/CVK/CVK_2"
cd CVK_2
sh ./setuplinux.sh
```


----------------------------------------------------------------------------



## Eigene Änderungen


#####  Executables
1. Kopieren Sie eines der Verzeichnisse aus _src/executables/_.
2. Stellen Sie insbesondere sicher, dass die Datei _src/executables/%NAME%/CMakeLists.txt_ existiert.
3. Führen Sie erneut die setup-Datei aus.


##### Neue C++ Dateien (_*.cpp, *.h_)
1. Erstellen Sie die Dateien in den Verzeichnissen _src/libraries/%NAME%_ oder _src/executables/%NAME%_.
   **Achtung:** Nicht in einem Unterverzeichnis von _BUILD\_%IDE%_!
2. Führen Sie gegebenenfalls `cmake rebuild_cache` oder erneut die setup-Datei aus.


##### Libraries
Alle eigenen Libraries werden automatisch gegen alle Executables und gegenseitig verlinkt. Dadurch können Funktionen programmübergreifend verwendet werden.
1. Kopieren Sie eines der Verzeichnisse aus _src/libraries/_.
2. Stellen Sie insbesondere sicher, dass die Datei _src/libraries/%NAME%/CMakeLists.txt_ existiert.
3. Führen Sie erneut die setup-Datei aus.


##### Neue Shader
1. Erstellen Sie die Dateien im Verzeichnis _src/shaders/_.

In C++ ist das Präprozessor Makro _SHADERS\_PATH_ definiert. Es enthält den absoluten Verzeichnispfad zu _src/shaders/_. Aus diesem werden zur Laufzeit die Shader geladen. Folgende Endungen werden berücksichtigt: _*.glsl *.vert *.vertex *.vs *.tcs *.cont *.t\_cont *.tes *.eval *.t_eval *.geom *.geo *.geometry *.gs *.frag *.fragment *.fs *.ps *.comp *.compute *.cs_.


##### Neue Ressourcen
1. Legen Sie die entsprechenden Dateien in _resources/_ ab.

In C++ ist das Präprozessor Makro _RESOURCES\_PATH_ definiert. Es enthält den absoluten Verzeichnispfad zu _resources/_. Aus diesem werden zur Laufzeit die Ressourcen geladen.


----------------------------------------------------------------------------



## Weiteres
- Generierte Projektdateien werden in _BUILD\_%IDE%_ generiert.
- Ausführbare Dateien werden in _BUILD\_%IDE%/bin/_ erzeugt.
- Alle Quellcode Dateien bleiben in _src_ gespeichert. In der IDE sind diese Verzeichnisse verlinkt. Dadurch bleiben alle Dateien in _src_ versionierbar. Versionieren Sie daher lediglich das Verzeichnis _src_.
- In C++ sind folgende Präprozessor Makros definiert:  
  _SHADERS\_PATH_ - Enthält den absoluten Verzeichnispfad zu _src/shaders_. Aus diesem werden zur Laufzeit die Shader geladen.  
  _RESOURCES\_PATH_ - Enthält den absoluten Verzeichnispfad zu _resources_. Aus diesem werden zur Laufzeit Texturen, Modelle usw. geladen.  
