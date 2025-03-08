#ifndef DATABASE_HEADER
#define DATABASE_HEADER

#include <QtCore>
#include <QtSql>
#include <Note.hpp>

namespace notes {
    class Database : public QObject {
        Q_OBJECT

    public:
        static Database* instance() noexcept {
            static Database instance;
            return &instance;
        }
        bool openDatabase(const QString& path, const QByteArray& password) noexcept;
        bool openDatabase(const QString& path) noexcept;
        bool isOpen() noexcept { return m_database.isOpen(); }
        void closeDatabase() noexcept { return m_database.close(); }

        bool createNote(const Note& note) noexcept;
        bool updateNote(const Note& note) noexcept;
        bool deleteNote(const QUuid& id) noexcept;
        bool deleteNotesByTag(const QString& tag) noexcept;

        std::optional<QList<Note>> getNotesByTag(const QString& tag) noexcept;
        std::optional<QStringList> getTags() noexcept;

        bool changeTags(const QUuid& noteId, const QStringList& tags) noexcept;
    private:
        Database() noexcept;
        bool executeSqlFile(const QString& path) noexcept;

        QSqlDatabase m_database;
    };
}

#endif /* DATABASE_HEADER */
