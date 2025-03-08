#ifndef OPENSCENE_HEADER
#define OPENSCENE_HEADER

#include <QtCore>

namespace notes {
    class OpenScene : public QObject {
        Q_OBJECT

    public:
        static OpenScene* instance() noexcept {
            static OpenScene instance;
            return &instance;
        }

        void requestOpenInformation() noexcept;
        QByteArray getPassword() noexcept { return m_password; }
        QString getNotesFile() noexcept { return m_notesFile; }
    private:
        OpenScene(QObject* parent = nullptr) : QObject(parent) {}
        ~OpenScene() {}

        QByteArray m_password;
        QString m_notesFile;
    };
}

#endif /* OPENSCENE_HEADER */
