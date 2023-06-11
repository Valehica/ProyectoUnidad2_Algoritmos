#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;
struct Guardian{
    //Estructura para guardianes {nombre, nivel de experiencia, maestro, aldea, aprendices}
    std::string name; 
    int powerLevel;
    std::string mainMaster;
    std::string village; 
    int lvlAprendiz;
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
        void modificarNameGuardian(const string& buscado, GuardiansTree*& tree, const string& newName){
            modificarNameRecursion(tree->root, 0,buscado, newName);
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
                guardian->lvlAprendiz = 0;
                root = guardian;
            }else{
                //busca el guardian maestro y cpnvierte al nuevo guardian en su aprendiz
                Guardian* master = findGuardian(mainMaster);
                if(master != nullptr){
                    guardian->lvlAprendiz = master->lvlAprendiz+1;
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
        /*COMPROBAR SI EXISTE UNA VILLA Y UN APRENDIZ POR ALDEA*/
        void addAprendiz(string maestro){
            Guardian* mainMaster = findGuardian(maestro);
            if (mainMaster == nullptr) {
                // No se encontró el maestro, manejar el error
                return;
            }
            int levelTope = mainMaster->powerLevel;
            srand(time(nullptr));
            int randomIndice;
            int randomLevel = (rand()%(levelTope-50)) +50;
            vector<string> posiblesNombres= {"Salvador","Ireneo", "Mureo", "Marcial", "Florentino", "Salomon", "Eliana", "Abdon", "Bartolome"};
            randomIndice = rand()%posiblesNombres.size();
            insertGuardian(posiblesNombres[randomIndice], std::to_string(randomLevel), maestro, mainMaster->village);
            
        }
        void comprobarMaestroAprendices(){
            string guardianNull = " ";
            vector<Guardian>* aprendicesPtr = nullptr;
            vector<string>VilagesPasadas;
            for (const auto& pair : MapVillages) {
                vector<Guardian> aprendices;
                aprendicesPtr = &aprendices;
                const string& villageName = pair.first;
                aprendicesPorVilla(root, 0, villageName, aprendicesPtr,guardianNull);
                if(aprendices.size() < 2){
                    //Se debe crear un nuevo aprendiz
                    string mainMaster = mainMasterVillage(villageName);
                    addAprendiz(mainMaster);
                }
            }
            
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
            int indiceInicial = 0;
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
        int buscarIndice(string village){
            for (const auto& pair : MapVillages) {
                    const string& villageName = pair.first;
                    const int& villageIndice = pair.second;
                    if(village == villageName){
                        return villageIndice;
                    }
            }
            return -1;
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
        string mainMasterVillage(string village){
            vector<Guardian> guardianesSaved;
            Guardian guardianMasAlto;
            
            guardianesPorVilla(root,0,village, &guardianesSaved);

            for (size_t i = 0; i < guardianesSaved.size(); i++) {
                if(i == 0){
                    guardianMasAlto = (guardianesSaved)[i];
                }else{
                    if(guardianMasAlto.lvlAprendiz>((guardianesSaved)[i]).lvlAprendiz){
                        guardianMasAlto = (guardianesSaved)[i];
                    }
                }
            }
            return guardianMasAlto.name;
        }
        void addAprendiz(string village, vector<Guardian>* aprendices){
            string mainMaster = mainMasterVillage(village);
            aprendicesPorVilla(root, 0, village,aprendices,mainMaster);
        }
        void addVillagesAotherMap(unordered_map<string, int>* cantAprendicesGanar){
            for (const auto& pair : MapVillages) {
                (*cantAprendicesGanar)[pair.first] = 0;
            }
        }
        vector<string> createMap(){
            vector<string> map;
            for (const auto& pair : MapVillages) {
                map.push_back(pair.first);
            }
            return map;
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
        void printVillagesVecinas(string village){
            int indice = buscarIndice(village);
            cout<<"Desde " << village <<" puedes ir a:"<< endl;
                for(int k= 0; k < MapVillages.size(); k++){
                    if(matrizVillage[indice][k] == 1){
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
        void printMaestrosPorVilla(string village){
            string master = mainMasterVillage(village);
        }
        void printAprendices(vector<Guardian> aprendices){
            cout << "APRENDICES DISPONIBLES: "<< endl;    
            for (size_t i = 0; i < aprendices.size(); i++) {
                cout << "  >> " <<(aprendices[i]).name << "[Village:" << aprendices[i].village <<"; Power Level: "<< aprendices[i].powerLevel <<"]"<< endl;
            }
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
        void guardianesPorVilla(Guardian* guardian, int indent, string village,vector<Guardian>* guardianesSaved){
            if (guardian != nullptr) {
                if(guardian->village == village){
                    guardianesSaved->push_back(*guardian);
                }
                for (Guardian* aprendiz : guardian->aprendices) {
                    guardianesPorVilla(aprendiz, indent + 4, village, guardianesSaved);
                }
            }
        }
        void aprendicesPorVilla(Guardian* guardian, int indent, string village,vector<Guardian>*& guardianesSaved, string mainMaster){
            if (guardian != nullptr) {
                if(guardian->village == village && mainMaster != guardian->name){
                    //cout<< "NAME GUARDIAN: "<< guardian->name << "  VS  "<< "NAME MASTER: "<< mainMaster<<endl;
                    guardianesSaved->push_back(*guardian);
                }
                for (Guardian* aprendiz : guardian->aprendices) {
                    aprendicesPorVilla(aprendiz, indent + 4, village, guardianesSaved, mainMaster);
                }
            }
        }
        void modificarNameRecursion(Guardian* guardian, int indent, string buscado, string newName) {
            if (guardian != nullptr) {
                if ((guardian)->name == buscado) {
                    (guardian)->name = newName;
                    return;
                } else {
                    for (Guardian* aprendiz : (guardian)->aprendices) {
                        modificarNameRecursion(aprendiz, indent + 4, buscado, newName);
                    }
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
std::string capitalizeFirstLetter(const std::string& input) {
    std::string result = input;

    if (!result.empty()) {
        // Convertir la primera letra a mayúscula
        result[0] = std::toupper(result[0]);

        // Convertir las demás letras a minúsculas
        for (size_t i = 1; i < result.length(); i++) {
            result[i] = std::tolower(result[i]);
        }
    }
    return result;
}
void printBorde(string str){
    cls();
    cout << "            |===================================|            "<<endl;
    cout << "|===========|"<< str <<"|===========|"<<endl;
    cout << "            |===================================|            "<<endl<<endl;
}
string convertVillageString(string village){
    //funcion para concertir a entrada de valle en un string valido
    string resultVillage;
    if(capitalizeFirstLetter(village) == "Tesla"){
        return capitalizeFirstLetter(village);
    }else if(capitalizeFirstLetter(village) == "Capital"){
        resultVillage = capitalizeFirstLetter(village) + " City";
        return resultVillage;
    }else if(capitalizeFirstLetter(village) == "Capital City"){
        return village;
    }else{
        resultVillage = capitalizeFirstLetter(village) + " Village";
        return resultVillage;
    }
}
string convertStringIn35Size(string letter){
    int extencion = letter.size();
    extencion = 35 - extencion;
    int espaciosIz;
    int espaciosDer;
    string resultado;
    if(extencion%2 != 0){
        //impar
        espaciosIz = trunc(extencion/2); 
        espaciosDer = espaciosIz + 1;
    }else{
        //par
        espaciosIz = trunc(extencion/2); 
        espaciosDer = espaciosIz; 
    }
    for(int i = 0; i<espaciosIz; i++){
        if(i== 0){
            resultado = " ";
        }else{
            resultado = resultado + " ";
        }
    }
    resultado = resultado + letter;
    for(int i = 0; i<espaciosDer; i++){        
        resultado = resultado + " ";
    }
    return resultado;

}
/*FUNCIONES DE JUEGO*/
Guardian elegirPersonaje(GuardiansTree* tree){
    /*Implementar despues*/
    Guardian player;
    string guardianName;
    string nameRemplace = "Thadio"; 
    bool valor = true;
    while(valor){
        printBorde(convertStringIn35Size("ESCOGE TU PERSONAJE"));
        cout << "A continuacion se mostrara una lista de guerrero" <<endl;
        cout << "Deberá escoger entre los guerreros presentes para que sea su personaje" <<endl;
        std::cin.ignore();
        pause("Presione una Enter para continuar...");
        cout<<endl << "GUERREROS DISPONIBLES: "<<endl;
        tree->printGuardians();
        cout << endl << "Ingrese en nombre del maestro ecogido: ";
        cin >> guardianName;
        Guardian* playerPosible = tree->findGuardian(capitalizeFirstLetter(guardianName));
        if(playerPosible != nullptr){
            player = *playerPosible;
            player.powerLevel = 50;
            valor = false;
            tree->modificarNameGuardian(capitalizeFirstLetter(guardianName), tree, nameRemplace);
        }else{
            std::cin.ignore();
            pause("El Guerrero ingresado no cumple con los requisitos, intetalo nuevamente...");
            cls();
        }
    }
    return player;
}
bool crearNombre(string* name){
    printBorde(convertStringIn35Size("CREACION DE PERSONAJE"));
    std::cout <<"Ingrese un nommbre: ";
    cin >> *name;
    if(!name->empty()){
        cls();
        return false;
    }
    return true;
    cls(); 
}
bool crearMaestro(Guardian* mainMasterGuardian, string* mainMaster, int powerLevel, GuardiansTree* tree){
    printBorde("       CREACION DE PERSONAJE       ");
    cout << "A continuacion se mostrara una lista de maestros" <<endl;
    cout << "Deberá escoger entre los maestros presentes para que sea su maestro" <<endl;
    cout << "Debe tener en cuanta que un maestro debe tener mayor nivel que usted" <<endl;
    cout << "Nivel actual de su personaje: 50" <<endl;
    std::cin.ignore();
    pause("Presione una Enter para continuar...");
    
    cout<<endl << "MAESTROS DISPONIBLES: "<<endl;
    tree->printGuardians();
    cout << endl << "Ingrese en nombre del maestro ecogido: ";
    cin >> *mainMaster;
    //buscar maestro
    mainMasterGuardian = tree->findGuardian(capitalizeFirstLetter(*mainMaster));
    if(mainMasterGuardian != nullptr){
        if(mainMasterGuardian->powerLevel > powerLevel){
            cls();
            return false;
        }else{
            std::cin.ignore();
            pause("El maestro singresado no cumple con los requisitos, intetalo nuevamente...");
            cls();
        }
    }else{
        std::cin.ignore();
        pause("El maestro ingresado no cumple con los requisitos, intetalo nuevamente...");
        cls();
    }
    cls();
    return true;
}
bool crearVilla(string village, string* villageNameComplete, GuardiansTree* tree){
    printBorde("       CREACION DE PERSONAJE       ");
    cout << "A continuacion se mostrara una lista de las villas disponibles" <<endl;
    cout << "Deberá escoger entre las villas disponibles" <<endl;
    cout << "Esta será la villa donde iniciara su viaje" <<endl;
    std::cin.ignore();
    pause("Presione una tecla para continuar...");
    
    cout<<endl << "VILLAS DISPONIBLES: "<<endl;
    tree->printMap();
    cout << endl<< "(Solo ingrese la primera palabra, es decir si desea escoger River Village solo escriba River)";
    cout << endl << "Ingrese en nombre de la villa escogida: ";
    cin >> village;
    *villageNameComplete = convertVillageString(village); 
    if(tree->buscarPorName(*villageNameComplete)){
        return false;
        //break;
    }else{
        std::cin.ignore();
        pause("La villa ingresada no cumple con los requisitos, intetalo nuevamente..."); 
        cls();
    }
    return true;
    cls();
}
Guardian crearPersonaje(GuardiansTree* tree){
    Guardian player;
    Guardian mainMasterGuardian;
    string name, mainMaster, village, villageNameComplete;
    int powerLevel = 50;
    bool  valor = true;
    //name
    while(valor){
        valor = crearNombre(&name);
    }
    valor = true;
    while(valor){
        valor = crearMaestro(&mainMasterGuardian, &mainMaster, powerLevel, tree);
    }         
    valor = true;
    while(valor){
        valor = crearVilla(village, &villageNameComplete, tree);
    }      
    
    player.name = name;
    player.powerLevel = powerLevel;
    player.mainMaster = mainMaster;
    player.village = villageNameComplete;
    return player;
}
Guardian menuPersonaje(GuardiansTree* tree){
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
            return elegirPersonaje(tree);
            valor = false;
            break;
        case '2':
            cls();
            return crearPersonaje(tree);
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
void avanzarCiudad(string* villageActual, GuardiansTree* tree, Guardian* player, vector<string>* historialCompleto, vector<string>* historialVillage){
    string villageChose;
    string villageChoseComplete;
    bool valor = true;
    while(valor){
        printBorde("       THE GUARDIAN JOURNEY        ");
        tree->printVillagesVecinas(*villageActual);
        cout << endl<< "(Solo ingrese la primera palabra, es decir si desea escoger River Village solo escriba River)";
        cout << endl << "Ingrese en nombre de la villa escogida: ";
        cin >> villageChose;
        villageChoseComplete = convertVillageString(villageChose); 
        if(tree->buscarPorName(villageChoseComplete)){
            valor = false;
            player->powerLevel += 1;
            std::cin.ignore();
            historialCompleto->push_back(villageChoseComplete);
            historialVillage->push_back(villageChoseComplete);
            cout<< endl<<"Has caminado hacia un nuevo lugar, ganas un punto"<<endl<<endl;
            pause("Presione enter para continuar...");
            cls();
            *villageActual = villageChoseComplete;
        }else{
            std::cin.ignore();
            pause("La villa ingresada no cumple con los requisitos, intetalo nuevamente..."); 
            cls();
        }
    }
}
void eliminarIguales(std::vector<string>* vectorEliminado, std::vector<string>* vectorAEliminar) {
    // Utilizar la función erase-remove idiom para eliminar los elementos
    vectorEliminado->erase(std::remove_if(vectorEliminado->begin(), vectorEliminado->end(),
        [vectorAEliminar](const std::string& elemento) {
            return std::find(vectorAEliminar->begin(), vectorAEliminar->end(), elemento) != vectorAEliminar->end();
        }), vectorEliminado->end());
}
void eliminarDerrotados(std::vector<Guardian>* guardianes, std::vector<Guardian>* derrotados) {
    if(derrotados != nullptr){
        guardianes->erase(std::remove_if(guardianes->begin(), guardianes->end(), [&](const Guardian& guardian) {
            return std::find_if(derrotados->begin(), derrotados->end(), [&](const Guardian& derrotado) {
                return derrotado.name == guardian.name;
            }) != derrotados->end();
        }), guardianes->end());
    }
}
string lvlMenor(vector<Guardian>* aprendices){
    Guardian levelMenor;
    for (size_t i = 0; i < aprendices->size(); i++) {
      if(i == 0){
        levelMenor = (*aprendices)[i];
      }else{
        if(levelMenor.powerLevel > (*aprendices)[i].powerLevel){
            levelMenor = (*aprendices)[i];
        }
      }
    }
    return levelMenor.name;
}
bool buscarStringInVector(vector<Guardian> aprendices, string aprendiz){
    for (size_t i = 0; i < aprendices.size(); i++) {
        if(aprendices[i].name == aprendiz){
            return true;
        }
    }
    return false;
}

bool dados(Guardian* player, Guardian oponente){
    srand(time(nullptr));
    int random;
    int levelPlayer = trunc(player->powerLevel/10);
    int levelOponente = trunc(oponente.powerLevel/10);
    int carasDados;
    int ganancia;
    float porcentaje;
    if(levelOponente > levelPlayer){
        carasDados = levelOponente - levelPlayer + 2;
        porcentaje = (100 * (1.0 / carasDados));
        random = rand()%carasDados;
    }else if(levelOponente == levelPlayer){
        carasDados = 2;
        porcentaje = 50;
        random = rand()%carasDados;
    }else{
        carasDados = levelOponente - levelPlayer + 2;
        random = rand()%carasDados;
        ganancia = carasDados - 1 - trunc(carasDados/3);
        porcentaje = (100 * (ganancia / static_cast<float>(carasDados)));
        for(int i = 0; i < ganancia; i++){
            if(i == random){
                return true;
            }
        }
    }
    cout<<"Tienes una probabilidad de %"<< fixed << setprecision(0) << porcentaje << " de ganar"<< endl;
    std::cin.ignore();
    pause("Presiona enter para lanzar el dado...");
    
    if(random == 1){
        return true;
    }else{
        return false;
    }
}
void informacionPelea(Guardian player, Guardian oponente){
    cout <<endl<< "ESTADISTICAS: "<<endl;
    cout << "  >> " <<(player).name << "[Village:" << player.village <<"; Power Level: "<< player.powerLevel <<"]"<< endl;
    cout << "  >> " <<(oponente).name << "[Village:" << oponente.village <<"; Power Level: "<< oponente.powerLevel <<"]"<< endl<<endl;
}
bool enfrentamiento(Guardian* player, GuardiansTree* tree, std::vector<Guardian>*& Derrotados, Guardian oponente){
    printBorde(convertStringIn35Size(player->name +" VS " + oponente.name));
    informacionPelea(*player, oponente);
    bool resultado = dados(player, oponente);
    if(resultado){
        cout << "Felicidades, derrotaste a "<< oponente.name;
        Derrotados->push_back(oponente);
        return true;

    }else{
        cout<< "Que lastima, fuiste derrotado por "<< oponente.name<<endl;
        return false;
    }
}
void enfrentamientoAprendiz(Guardian* player, GuardiansTree* tree, std::vector<Guardian>* aprendicesDerrtados, string village, unordered_map<string, int>* cantAprDerrotados, vector<string>* historialCompleto, vector<string>* historialGuerreros){
    string oponente;
    Guardian* oponenteGuardian;
    bool valor = true;
    while(valor){
        printBorde("      ENFRENTAMIENTO APRENDIZ      ");
        vector<Guardian> aprendices;
        tree->addAprendiz(village,&aprendices);
        eliminarDerrotados(&aprendices,aprendicesDerrtados);
        tree->printAprendices(aprendices);
        
        cout <<endl << "(Le sugerimos que pelee contra " << lvlMenor(&aprendices) << " ya que presenta un menor nivel)" << endl;
        cout << "Ingrese el nombre del aprendiz que desee enfrensarse: ";
        cin >> oponente;
        oponente = capitalizeFirstLetter(oponente);
        //comprobar si pertence 
        if(buscarStringInVector(aprendices, oponente)){
            valor = false;
        }else{
            std::cin.ignore();
            pause("El valor ingresado es incorrecto, intentelo nuevamente...");
        }
        oponenteGuardian = tree->findGuardian(oponente);
    }
    if(enfrentamiento(player,tree,aprendicesDerrtados,*oponenteGuardian)){
        cout<< " Y has ganado 1 punto" <<endl;
        std::cin.ignore();
        pause("Presione enter para continuar...");
        cls();
        player->powerLevel += 1;
        (*cantAprDerrotados)[village] =+ 1;
        historialCompleto->push_back(player->name+" vs Aprendiz:"+ oponente + ", Resultado: Gano, Puntos obtenidos: 1, Puntos actuales: " + std::to_string(player->powerLevel));
        historialGuerreros->push_back(player->name+" vs Aprendiz:"+ oponente + ", Resultado: Gano, Puntos obtenidos: 1, Puntos actuales: " + std::to_string(player->powerLevel));

    }else{
        historialCompleto->push_back(player->name+" vs Aprendiz:"+ oponente + ", Resultado: Perdio, Puntos obtenidos: 0, Puntos actuales: " + std::to_string(player->powerLevel));
        historialGuerreros->push_back(player->name+" vs Aprendiz:"+ oponente + ", Resultado: Perdio, Puntos obtenidos: 0, Puntos actuales: " + std::to_string(player->powerLevel));
        std::cin.ignore();
        pause("Presione enter para continuar...");
    }
    
}
void enfrentamientoMaestro(Guardian* player, GuardiansTree* tree, std::vector<Guardian>* maestrosDerrotados, string oponente, vector<string>* historialCompleto, vector<string>* historialGuerreros){
    printBorde(convertStringIn35Size("ENFRENTAMIENTO MAESTRO"));
    cout <<endl<< "El maestro de esta villa es: " << oponente <<endl;
    pause("Presione Enter para iniciar el enfrentamiento...");
    Guardian* oponenteGuardian;
    oponenteGuardian = tree->findGuardian(oponente);
    if(enfrentamiento(player,tree,maestrosDerrotados,*oponenteGuardian)){
            cout<< " Y has ganado 2 punto" <<endl;
            std::cin.ignore();
            pause("Presione enter para continuar...");
            cls();
            player->powerLevel += 2;
        historialCompleto->push_back(player->name+" vs Maestro:"+ oponente + ", Resultado: Gano, Puntos obtenidos: 2, Puntos actuales: " + std::to_string(player->powerLevel));
        historialGuerreros->push_back(player->name+" vs Maestro:"+ oponente + ", Resultado: Gano, Puntos obtenidos: 2, Puntos actuales: " + std::to_string(player->powerLevel));

    }else{
        historialCompleto->push_back(player->name+" vs Maestro:"+ oponente + ", Resultado: Perdio, Puntos obtenidos: 0, Puntos actuales: " + std::to_string(player->powerLevel));
        historialGuerreros->push_back(player->name+" vs Maestro:"+ oponente + ", Resultado: Perdio, Puntos obtenidos: 0, Puntos actuales: " + std::to_string(player->powerLevel));
        pause("Presione enter para continuar...");
    }
}
void printHistorialComplete(vector<string> historial, GuardiansTree* tree){
    printBorde(convertStringIn35Size("HISTORIAL COMPLETO"));
    for (size_t i = 0; i < historial.size(); i++) {
        if(tree->buscarPorName(historial[i])){
            if(i == 0){
                cout << "Empezaste en: " << historial[i]<< endl;
            }else{
                cout <<"Caminaste hacia: " <<historial[i] <<endl;
            }
        }else{
            cout <<"  >> Enfrentamiento: " << historial[i]<<endl;;
        }
    }
    cout << endl;
    std::cin.ignore();
    pause("Pesiona Enter para continuar...");
}
void printHitorialVillage(vector<string> historial, GuardiansTree* tree){
    printBorde(convertStringIn35Size("HISTORIAL CIUDADES"));
    for (size_t i = 0; i < historial.size(); i++) {
        if(tree->buscarPorName(historial[i])){
            if(i == 0){
                cout << "Empezaste en: " << historial[i]<< endl;
            }else{
                cout << "Caminaste hacia: " <<historial[i] <<endl;
            }
        }
    }
    cout << endl;
    std::cin.ignore();
    pause("Pesiona Enter para continuar...");
}
void printHistorialGuerreros(vector<string> historial, GuardiansTree* tree){
    printBorde(convertStringIn35Size("HISTORIAL GUERREROS"));
    for (size_t i = 0; i < historial.size(); i++) {
        cout <<">> Enfrentamiento: " << historial[i]<<endl;;
    }
    cout << endl;
    pause("Pesiona Enter para continuar...");
}
void historial(vector<string> historialVillage, vector<string> historialCompleto, vector<string> historialGuerreros, GuardiansTree* tree){
    bool valor = true;
    string opcion;
    while (valor){    
        printBorde(convertStringIn35Size("HISTORIAL"));
        cout <<"Escoge una opción: "<<endl;
        cout <<"  >> 1 para mostrar el historial completo"<<endl;
        cout <<"  >> 2 para mostrar el historial de guerreros derrotados"<<endl;
        cout <<"  >> 3 para mostrar el historial de ciudades recorridas"<<endl;
        cout <<"  >> 4 para salir"<<endl;
        cin >> opcion;
    
        if(stoi(opcion) == 1){
            //historial completo
            printHistorialComplete(historialCompleto, tree);
        }else if(stoi(opcion) == 2){
            //historial guerrero
            printHistorialGuerreros(historialGuerreros, tree);
        }else if(stoi(opcion) == 3){
            //historial ciudades
            printHitorialVillage(historialVillage, tree);
        }else if(stoi(opcion) == 4){
            return;
        }else{
            std::cin.ignore();
            pause("El valor ingresado es incorrecto, intentelo nuevamente...");
            cls();
        }
    }
}
void menuFinal(vector<string> historialVillage, vector<string> historialCompleto, vector<string> historialGuerreros, GuardiansTree* tree){
    bool valor = true;
    string opcion;
    while (valor){    
        cout <<"Escoge una opción: "<<endl;
        cout <<"  >> 1 para mostrar el historial"<<endl;
        cout <<"  >> 2 para finalizar el juego"<<endl;
        cin >> opcion;
    
        if(stoi(opcion) == 1){
            historial(historialVillage,historialCompleto,historialGuerreros, tree);
            opcion = "2";
        }else if(stoi(opcion) == 2){
            printBorde(convertStringIn35Size("GRACIAS POR JUGAR"));
            pause("Presiona Enter para salir del juego...");
            return;
        }else{
            std::cin.ignore();
            pause("El valor ingresado es incorrecto, intentelo nuevamente...");
            cls();
        }
    }
}
vector<string> comprobarRecoridoVillas(vector<string> historilVillas, GuardiansTree* tree){
    vector<string> villasFatantes = tree->createMap(); 
    eliminarIguales(&villasFatantes, &historilVillas);
    return villasFatantes;
}
void menuEnfrentamientos(Guardian* player, GuardiansTree* tree, std::vector<Guardian>* GuerreroDerrotados, unordered_map<string, int>* cantAprDerrotados, string villageActual, bool*valor, vector<string>* historialCompleto, vector<string>* historialGuerreros, vector<string> historialVillage){
    string opcion;
    string MaestroVillage = tree->mainMasterVillage(villageActual);
    string villasRecorridas; 
    printBorde("       THE GUARDIAN JOURNEY        ");
    cout <<"Escoge una opción: "<<endl;
    cout <<"  >> 1 para enfrentase a un aprendiz"<<endl;
    cout <<"  >> 2 para enfrentase a un Maestro"<<endl;
    if(villageActual == "Tesla"){
        cout <<"  >> 3 para enfrentarse a Stromheart"<<endl;
    }
    cin >> opcion;
    if(stoi(opcion) == 1){
        if((*cantAprDerrotados)[villageActual] < 2){
            enfrentamientoAprendiz(player,tree,GuerreroDerrotados,villageActual,cantAprDerrotados, historialCompleto, historialGuerreros);    
        }else{
            cout << "Ya no puedes derrotar mas aprendices en esta ciudad";
            pause("Presione enter para continuar...");
            cls();
        }
    }else if(stoi(opcion) == 2){
        if(!buscarStringInVector(*GuerreroDerrotados, MaestroVillage)){
            enfrentamientoMaestro(player,tree,GuerreroDerrotados,MaestroVillage, historialCompleto, historialGuerreros);
        }else{
            cout << "Ya derrotaste al guardian de esta villa";
            pause("Presione enter para continuar...");
            cls();
        }
    }else if(stoi(opcion) == 3 && villageActual == "Tesla"){
        vector<string> faltante = comprobarRecoridoVillas(historialVillage, tree);
        if(faltante.size() == 0 && player->powerLevel>= 90){
            Guardian* stormheart = tree->findGuardian("Stormheart");
            printBorde(convertStringIn35Size("FELECIDADES, GANASTE"));
            cout <<endl << "Eres capaz de enfrentarte a stromheart"<<endl;
            informacionPelea(*player, *stormheart);
            menuFinal(historialVillage,*historialCompleto,*historialGuerreros, tree);
            *valor = false;
        }else{
            printBorde(convertStringIn35Size("QUE LASTIMA"));
            cout << "Aun no puedes enfrentar a Stromheart"<<endl;
            cout << "REQUISITOS: "<<endl;
            cout << "  >> Nivel mayor o igual a 90"<<endl;
            cout << "  >> Haber recorrido todas las villas"<<endl;
            cout << endl << "TIENES: "<<endl;
            cout << "  >> Nivel: " << player->powerLevel<<endl;
            cout << "  >> Villas faltantes: "<<faltante.size() <<endl;
            if(faltante.size()>0){
                for (size_t i = 0; i < faltante.size(); i++) {
                    cout << "    >> " <<faltante[i] <<endl;
                }
            }
            cout << endl;
            std::cin.ignore();
            pause("Presiona enter para continuar...");
        }
    }else{
        std::cin.ignore();
        pause("El valor ingresado es incorrecto, intentelo nuevamente...");
        cls();
    }
}
void menuJuego(Guardian* player, GuardiansTree* tree, std::vector<Guardian>* GuerreroDerrotados, unordered_map<string, int>* cantAprDerrotados){
    string villageActual = player->village;
    string opcion;
    vector<string> historialCiudades;
    vector<string> historialGerreros;
    vector<string> historialCompleto;
    historialCompleto.push_back(villageActual);
    historialCiudades.push_back(villageActual);
    
    bool valor = true;
    while(valor){
        printBorde("       THE GUARDIAN JOURNEY        ");
        cout << "INFORMACION: "<< endl;
        cout << "Nombre: "<< player->name << endl;
        cout << "Puntos: " << player->powerLevel <<endl;
        cout << "Ahora te encuentras en: " << villageActual <<endl<<endl;
        cout << "Escoge una opción: "<<endl;
        cout << "  >> 1 para enfrentamiento"<<endl;
        cout << "  >> 2 para avanzar a la siguiente ciudad"<<endl;
        
        cin >> opcion;
        //Es entero
        if(stoi(opcion) == 1){
            menuEnfrentamientos(player, tree, GuerreroDerrotados, cantAprDerrotados, villageActual, &valor, &historialCompleto, &historialGerreros, historialCiudades);
        }else if(stoi(opcion) == 2){
            cls();
            avanzarCiudad(&villageActual, tree, player, &historialCompleto, &historialCiudades);
        }else{
            std::cin.ignore();
            pause("El valor ingresado es incorrecto, intentelo nuevamente...");
            cls();
        }   
    }
} 
int main(){
    //PRIMERO LECTURA DE ARCHIVOS
    GuardiansTree tree;
    Guardian Player;
    vector<Guardian> GuerreroDerrotados;
    unordered_map<string,int>cantAprDerrotados;

    tree.loarGuardianFromFile("guardians.txt");
    tree.loadVillagesFromFile("villages.txt");
    tree.addVillagesAotherMap(&cantAprDerrotados);
    tree.comprobarMaestroAprendices();
    /*Inicio del juego*/
    cls();
    printBorde(" BIENVENIDO A THE GUARDIAN JOURNEY ");
    pause("Presiona Enter para continuar...");
    cls();

    Player = menuPersonaje(&tree);

    printBorde("          INICIO DE JUEGO          ");
    pause("Presiona Enter para continuar...");
    cls();

    menuJuego(&Player, &tree, &GuerreroDerrotados, &cantAprDerrotados);
    

    return 0;
}