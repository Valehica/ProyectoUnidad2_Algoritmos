#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unordered_map>

using namespace std;

struct Guardian{
    //Estructura para guardianes {nombre, nivel de experiencia, maestro, aldea, aprendices}
    std::string name; 
    int powerLevel;
    std::string mainMaster;
    std::string village; 
    std::vector<Guardian*> aprendices;
};

//Crear una clase para el arbol de guardianes
class GuardiansTree{
    public: 
        GuardiansTree() : root(nullptr) {}//Inicializar la funcion

        //Buscar un guardian
        Guardian* findGuardian(const string name){
            for(Guardian* guardian: guardians){//buscar por el nombre
                if(guardian->name == name){
                    return guardian;
                }
            }
            return nullptr;//retornar null si no se encuentra
        }
        //Guardar un guardian
        void insertGuardian(const string& name, const string& powerLevel,const string& mainMaster, const string& village){
            Guardian* guardian = new Guardian;
            guardian->name = name;
            guardian->powerLevel = stoi(powerLevel);//converit string power leven en int power level
            guardian->mainMaster = mainMaster; 
            guardian->village = village;
            guardians.push_back(guardian);

            //comprueba si el arbol esta vacio
            if(root == nullptr){
                //convierte al primer guardian en el lider
                root = guardian;
            }else{
                //busca el guardian maestro y cpnvierte al nuevo guardian en su aprendiz
                Guardian* master = findGuardian(mainMaster);
                if(master != nullptr){
                    master->aprendices.push_back(guardian);
                }
            }
        }
        bool addMap(const string& village, int cont){//Esta funcion sirve ademas para saber cuantas villas hay
            //Agregar villas al mapa, si ya se encuentra la villa dentro del mapa no la agrega
            if(cont == 0){
                MapVillages[village] = cont;
                return true;
            }
            bool valor = true;
            for (const auto& pair : MapVillages) {
                const string& vilageName = pair.first;
                if(village == vilageName){
                    valor = false;
                }
            }
            if(valor){
                MapVillages[village] = cont;
            }
            return valor;
        }   
        
        //cargar el archivo guardianes
        void loarGuardianFromFile(const string& filename){
            int cont = 0;
            ifstream file(filename);//abre el archivo y lo llama file 
            if(!file){//Comprueba que file existe
                cerr << "Fallo al abrir el archivo: " << filename << endl;
                return;
            }
            string line;
            getline(file, line);//Lee la primera linea hasta el \n de file
            while(getline(file, line)){//recorrer todas las lineas del archivo
                istringstream iss(line);//se crea un objeto
                string name,powerLevel, mainMaster, village;//se declaran variables para fuardar la informacion
                //leer entre comas y guardarlo en sus respectuvas variables
                getline(iss, name, ',');
                getline(iss, powerLevel , ',');
                getline(iss, mainMaster, ',');
                getline(iss, village);
                if(addMap(village,cont)){
                    cont++;
                }
                insertGuardian(name, powerLevel, mainMaster, village);
            }
            file.close();
        }

        //coneccion villas
        void coneccionVillages(string village, string coneccionVillages){
            int indiceInicial;
            int indiceConeccion;
            for (const auto& pair : MapVillages) {
                const string& villageName = pair.first;
                const int& villageIndice = pair.second;
                if(villageName == village){
                    indiceInicial = villageIndice; 
                }
                if(villageName == coneccionVillages){
                    indiceConeccion = villageIndice;
                }
            }
            matrizVillage[indiceInicial][indiceConeccion] = 1;
            matrizVillage[indiceConeccion][indiceInicial] = 1;
        }
        
        //cargar archivo villas
        void loadVillagesFromFile(const string& filename){
            //inicializar la matriz
            matrizVillage.resize(MapVillages.size(), vector<int>(MapVillages.size(), 0));

            ifstream file(filename);//abre el archivo y lo llama file 
            if(!file){//Comprueba que file existe
                cerr << "Fallo al abrir el archivo: " << filename << endl;
                return;
            }
            string line;
            getline(file, line);//Lee la primera linea hasta el \n de file
            while(getline(file, line)){//recorrer todas las lineas del archivo
                istringstream iss(line);//se crea un objeto
                string village,conectedVillage;//se declaran variables para fuardar la informacion
                //leer entre comas y guardarlo en sus respectuvas variables
                getline(iss, village, ',');
                getline(iss, conectedVillage , ',');
                coneccionVillages(village, conectedVillage);
            }
            file.close();
        }
        bool buscarPorName(string name){
            for (const auto& pair : MapVillages) {
                const string& villageName = pair.first;
                if(villageName == name){
                    return true;
                }
            }
            return false;
        }
        string buscarPorIndice(int indice){
            for (const auto& pair : MapVillages) {
                    const string& villageName = pair.first;
                    const int& villageIndice = pair.second;
                    if(villageIndice == indice){
                        return villageName;
                    }
            }
            return nullptr;
        }
        /*FUNCIONES DE IMPRIMIR*/
        void printMap(){
            for (const auto& pair : MapVillages) {
                cout << pair.first << endl;
            }
        }
        void printVillage(int i){
            string villageNameAux;
            villageNameAux = buscarPorIndice(i);
                cout<<villageNameAux << endl;
                for(int k= 0; k < MapVillages.size(); k++){
                    if(matrizVillage[i][k] == 1){
                        cout << "    >> " << buscarPorIndice(k)<<endl;
                    }
                }
        }

        void printAllVillages(){
            string villageNameAux;
            for(int i= 0; i < MapVillages.size(); i++){
                printVillage(i);
                cout<<endl;
            }
        }
        void printGuardians() {
            printGuardian(root, 0);
        }
        
    private:
        vector<Guardian*> guardians;
        vector<vector<int>> matrizVillage;
        std::unordered_map<string, int> MapVillages;
        Guardian* root;

        //Funcion para imprimir private
        void printGuardian(Guardian* guardian, int indent) {
        if (guardian != nullptr) {
            std::cout << std::string(indent, ' ') << ">> " << guardian->name << " (Power Level: " << guardian->powerLevel << ", Village: " << guardian->village << ")" << std::endl;
            for (Guardian* aprendiz : guardian->aprendices) {
                printGuardian(aprendiz, indent + 4);
            }
        }
    }
};
/*Funciones basicas*/
void pause(string text){
    std::cout << text;
    std::cin.ignore();
}
void cls() {
    #ifdef _WIN32 // Verificar si se está compilando en Windows
        system("cls"); // Comando para limpiar la pantalla en Windows
    #else
        system("clear"); // Comando para limpiar la pantalla en sistemas Unix/Linux
    #endif
}
std::string capitalizeFirstLetter(const std::string& str) {
    std::string result = str;

    if (!result.empty()) {
        result[0] = std::toupper(result[0]);
    }

    return result;
}


/*FUNCIONES DE JUEGO*/
Guardian elegirPersonaje(){
    /*Implementar despues*/
    Guardian player;
    return player;
}
Guardian crearPersonaje(){
    Guardian player;
    GuardiansTree tree;
    tree.loarGuardianFromFile("guardians.txt");
    tree.loadVillagesFromFile("villages.txt");
    string name, mainMaster, village;
    int powerLevel = 50;
    int  valor = 0;
    while(valor < 3){
        cout << "            ╔═══════════════════════════════════╗            "<<endl;
        cout << "║═══════════║       CREACION DE PERSONAJE       ║═══════════║"<<endl;
        cout << "            ╚═══════════════════════════════════╝            "<<endl<<endl;
        if(valor == 0){
            cout <<"Ingrese un nommbre: ";
            cin >> name;
            if(!name.empty()){
                valor++;
            }
            cls();        
        }else if(valor == 1){
            cout << "A continuacion se mostrara una lista de maestros" <<endl;
            cout << "Deberá escoger entre los maestros presentes para que sea su maestro" <<endl;
            cout << "Debe tener en cuanta que un maestro debe tener mayor nivel que usted" <<endl;
            cout << "Nivel actual de su personaje: 50" <<endl;
            std::cin.ignore();
            pause("Presione una tecla para continuar...");
            cls();
            
            tree.printGuardians();
            cout << endl << "Ingrese en nombre del maestro ecogido: ";
            cin >> mainMaster;
            //buscar maestro
            Guardian* mainMasterGuardian = tree.findGuardian(capitalizeFirstLetter(mainMaster));
            if(mainMasterGuardian != nullptr){
                if(mainMasterGuardian->powerLevel > powerLevel){
                    valor++;
                }else{
                    pause("El maestro singresado no cumple con los requisitos, intetalo nuevamente...");
                }
            }else{
                pause("El maestro ingresado no cumple con los requisitos, intetalo nuevamente...");
            }
        }else if(valor == 2){
            cout << "A continuacion se mostrara una lista de las villas disponibles" <<endl;
            cout << "Deberá escoger entre las villas disponibles" <<endl;
            cout << "Esta será la villa donde iniciara su viaje" <<endl;
            std::cin.ignore();
            pause("Presione una tecla para continuar...");
            cls();

            tree.printMap();
            cout << endl<< "(Solo ingrese la primera palabra, es decir si desea escoger River Village solo escriba River)";
            cout << endl << "Ingrese en nombre de la villa escogida: ";
            cin >> village;
            if(tree.buscarPorName(capitalizeFirstLetter(village))+ "Village"){
                valor++;
            }
        }
    }
    player.name = name;
    player.powerLevel = powerLevel;//converit string power leven en int power level
    player.mainMaster = mainMaster; 
    player.village = village;
    return player;
}
Guardian menuPersonaje(){
    bool valor = true;
    string opcion;
    Guardian player;
    while(valor){
        cout<< "Ingrese una opcion: " <<endl; 
        cout << "  >> 1 para elegir un personaje" << endl;
        cout << "  >> 2 para añadir un personaje" << endl;
        cin >> opcion;
        switch (opcion[0])
        {
        case '1':
            cls();
            return elegirPersonaje();
            valor = false;
            break;
        case '2':
            cls();
            return crearPersonaje();
            valor = false;
            break;
        default:
            cls();
            std::cin.ignore();
            pause("El valor ingresado es incorrecto, intentetlo nuevamente...");
            cls();
            break;
        }
    }
    return player;
    
}

int main(){
    //PRIMERO LECTURA DE ARCHIVOS
    GuardiansTree tree;
    Guardian Player;
    tree.loarGuardianFromFile("guardians.txt");
    tree.loadVillagesFromFile("villages.txt");
    
    /*Inicio del juego*/
    cls();
    cout << "            ╔═══════════════════════════════════╗            "<<endl;
    cout << "║═══════════║ BIENVENIDO A THE GUARDIAN JOURNEY ║═══════════║"<<endl;
    cout << "            ╚═══════════════════════════════════╝            "<<endl;
    pause("Presiona Enter para continuar...");
    cls();

    Player = menuPersonaje();

    return 0;
}