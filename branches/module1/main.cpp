#include <iostream>
#include "ExtracteurFichier.h"

int main()
{
    //On crée l'extracteur avec un fichier de test
    ExtracteurFichier extracteur("test3.docx", "EXIGENCE_[A-Z0-9_]+");

    //On lance la lecture

    std::vector<ExigenceExtraite> resultats = extracteur.extraire();
    std::cout << "Nombre d'exigences trouvees : " << resultats.size() << std::endl;
    //Affichage 
    for (size_t i = 0; i < resultats.size(); i++) {
        std::cout << "------------------------------------" << std::endl;
        std::cout << "ID      : " << resultats[i].identifiant << std::endl;
        std::cout << "LIGNE   : " << resultats[i].numeroLigne << std::endl;
        std::cout << "CONTENU : " << resultats[i].contenuBrut << std::endl;
    }
    std::cout << "--- Fin du programme ---" << std::endl;

    return 0;
}