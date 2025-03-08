#ifndef EDITINGNOTESCENE_HEADER
#define EDITINGNOTESCENE_HEADER

#include <QtCore>
#include <Note.hpp>

namespace notes {
    class EditingNoteScene : public QObject {
        Q_OBJECT
    public:
        static EditingNoteScene* instance() noexcept {
            static EditingNoteScene instance;
            return &instance;
        }

        enum Actions : quint8 {
            EditContent = 1,
            EditTag = 2,
            EditTitle = 3,
            BackToView = 4,
            Exit = 5
        };

        //Return false if user wants to exit
        bool startEditingScene() noexcept;
    private:
        Note m_note;
        
        Actions requestActions() noexcept;
        bool requestEditingInformation() noexcept;

        void editContent() noexcept;
        void editTag() noexcept;
        void editTitle() noexcept;

        explicit EditingNoteScene(QObject* parent = nullptr) : QObject(parent) {}
        ~EditingNoteScene() {}
    };
}

#endif /* EDITINGNOTESCENE_HEADER */
