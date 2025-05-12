#include <QtCore>
#include <QCoreApplication>
#include <OpenScene.hpp>
#include <MainScene.hpp>
#include <Console.hpp>

using namespace notes;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    //Open scene cycle
    while(true) {
        OpenScene::instance()->requestOpenInformation();
    
        //Open database
        if (MainScene::instance()->tryOpenDatabase(OpenScene::instance()->getNotesFile(), OpenScene::instance()->getPassword()))
            break;
        else {
            console->print("Cannot open database, try again");
            system("pause");
            system("cls");
        }
    }
    MainScene::instance()->mainCycle();
    return a.exec();
}