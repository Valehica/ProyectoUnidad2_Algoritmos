#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
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

        //Buscar un guardian
        Guardian* findGuardian(const string name){
            for(Guardian* guardian: guardians){//buscar por el nombre
                if(guardian->name == name){
                    return guardian;
                }
            }
            return nullptr;//retornar null si no se encuentra
        }

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

int main(){
    //PRIMERO LECTURA DE ARCHIVOS
    GuardiansTree tree;
    tree.loarGuardianFromFile("guardians.txt");
    tree.loadVillagesFromFile("villages.txt");
    tree.printAllVillages();
    tree.printGuardians();
    return 0;
}