#include <OpenScene.hpp>
#include <Console.hpp>

using namespace notes;

void OpenScene::requestOpenInformation() noexcept {
    console->print("Notes by darkmind");
    console->print("Enter path to file, if it not created, it will be created (do not use extension of file)");
    m_notesFile = (console->requestString([](const QString& path) {
        return !path.isEmpty();
    })) + ".notes";
    console->print("Enter password for existing file or new password for new file, or press enter for no password");
    m_password = QCryptographicHash::hash(console->requestString().toUtf8(), QCryptographicHash::Md5);
    system("cls");
}