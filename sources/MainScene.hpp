#ifndef MAINSCENE_HEADER
#define MAINSCENE_HEADER

#include <QtCore>

namespace notes {
    class Note;
    class MainScene : public QObject {
        Q_OBJECT

    public:
        static MainScene* instance() noexcept {
            static MainScene instance;
            return &instance;
        }

        enum Actions : quint8 {
            CreateNote = 1,
            EditingNote = 2,
            DeleteNote = 3,
            ViewNotesByTag = 4,
            DeleteNotesByTag = 5,
            Exit = 6
        };

        bool tryOpenDatabase(const QString& notesFile, const QByteArray& password) noexcept;
        void mainCycle();
    private:
        Actions requestActions() noexcept;    

        ///@brief return true if tags is exists
        bool showTags() noexcept;
        void showNotesByTag() noexcept;
        void createNote() noexcept;
        void deleteNote() noexcept;
        void deleteNotesByTag() noexcept;

        MainScene(QObject* parent = nullptr) : QObject(parent) {}
        ~MainScene() {}
    };
}

#endif /* MAINSCENE_HEADER */
