#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

typedef struct {
    //Estructura para guardianes {nombre, nivel de experiencia, maestro, aldea, aprendices}
    string name; 
    int powerLevel;
    string mainMaster;
    string village; 
    vector<Guardian*> aprendices;
}Guardian;

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
        
        //cargar el archivo guardianes
        void loarGuardianFromFile(const string& filename){
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
                insertGuardian(name, powerLevel, mainMaster, village);
            }
            file.close();
        }

        //cargar archivo villas
        void loadVillagesFromFile(const string& filename){
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
                insertGuardian(name, powerLevel, mainMaster, village);
            }
            file.close();
        }
    private:
        vector<Guardian*> guardians;
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
};

int main(){
    //PRIMERO LECTURA DE ARCHIVOS
    

    return 0;
}