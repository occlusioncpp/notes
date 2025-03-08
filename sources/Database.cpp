#include <Database.hpp>
#include <Console.hpp>

using namespace notes;

Database::Database() noexcept : QObject(nullptr) {
    m_database = QSqlDatabase::addDatabase("QSQLITE");
}

bool Database::openDatabase(const QString& path, const QByteArray& password) noexcept {
    m_database.setDatabaseName(path);
    m_database.setPassword(password);
    if (!m_database.open()) 
        return false;
    {
        QSqlQuery query(m_database);
        query.exec("PRAGMA foreign_keys = ON;");
    }
    
    return executeSqlFile(":/root/tables.sql");
}

bool Database::executeSqlFile(const QString& path) noexcept {
    QFile tablesFile(path);
    if (!tablesFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        console->print("Cannot open sql file" + path);
        m_database.close();
        return false;
    }
    QTextStream in(&tablesFile);
    QString sql = in.readAll();
    tablesFile.close();

    QSqlQuery query(m_database);
    QStringList queries = sql.split(";");
    queries.removeAll("");
    bool failed = false;
    for (auto i : queries)
        if (!query.exec(i)) {
            console->print("DEBUG: failed to execute query: " + i + ", error: " + query.lastError().text() + "in file: " + path);
            failed = true;
        }
    if (failed)
        return false;
    return true;
}

bool Database::openDatabase(const QString& path) noexcept {
    m_database.setDatabaseName(path);
    if (!m_database.open())
        return false;
    {
        QSqlQuery query(m_database);
        query.exec("PRAGMA foreign_keys = ON;");
    }
    return executeSqlFile(":/root/tables.sql");
}

bool Database::createNote(const Note& note) noexcept {
    {
        QSqlQuery query(m_database);
        query.prepare("INSERT INTO notes(id, title, content, created_at) VALUES(:id, :title, :content, :created_at)");
        query.bindValue(":id", note.id());
        query.bindValue(":title", note.title());
        query.bindValue(":content", note.content());
        query.bindValue(":created_at", note.date());
        if (!query.exec())
            return false;
    }

    {
        QSqlQuery query(m_database);
        query.prepare("INSERT INTO tags(note_id, tag) VALUES(:note_id, :tag)");
        for (auto i : note.tags()) {
            query.bindValue(":note_id", note.id());
            query.bindValue(":tag", i);
            if (!query.exec())
                return false;
        }
    }
    
    return true;
}
bool Database::updateNote(const Note& note) noexcept {
    QSqlQuery query(m_database);
    query.prepare("UPDATE notes SET title = :title, content = :content, created_at = :created_at WHERE id = :id");
    query.bindValue(":id", note.id());
    query.bindValue(":title", note.title());
    query.bindValue(":content", note.content());
    query.bindValue(":created_at", note.date());
    return query.exec();
}
bool Database::deleteNote(const QUuid& id) noexcept {
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}
bool Database::deleteNotesByTag(const QString& tag) noexcept {
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM notes WHERE id IN (SELECT note_id FROM tags WHERE tag = :tag)");
    query.bindValue(":tag", tag);
    return query.exec();
}
std::optional<QList<Note>> Database::getNotesByTag(const QString& tag) noexcept {
    QList<Note> notes;
    {
        QSqlQuery query(m_database);
        query.prepare("SELECT * FROM notes WHERE id IN (SELECT note_id FROM tags WHERE tag = :tag)");
        query.bindValue(":tag", tag);
        
        if (!query.exec())
            return std::nullopt;

        while(query.next()) {
            Note note;
            note.setId(query.value(0).toUuid());
            note.setTitle(query.value(1).toString());
            note.setContent(query.value(2).toString());
            note.setDate(query.value(3).toDateTime());
            notes << note;
        }
    }
    QSqlQuery tagQuery(m_database);
    tagQuery.prepare("SELECT tag FROM tags WHERE note_id = :note_id");
    for (auto& note : notes) {
        tagQuery.bindValue(":note_id", note.id().toString());
        
        if (tagQuery.exec()) {
            while (tagQuery.next()) {
                auto tags = note.tags() << tagQuery.value(0).toString();
                note.setTags(tags);
            }
        }
    }
    return notes;
}
std::optional<QList<QString>> Database::getTags() noexcept {
    QList<QString> tags;
    QSqlQuery query(m_database);
    query.prepare("SELECT DISTINCT tag FROM tags");
    if (!query.exec())
        return std::nullopt;
    while (query.next())
        tags.append(query.value(0).toString());
    return tags;
}

bool Database::changeTags(const QUuid& noteId, const QStringList& tags) noexcept {
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM tags WHERE note_id = :note_id");
    query.bindValue(":note_id", noteId);
    if (!query.exec())
        return false;
    for (auto i : tags) {
        query.prepare("INSERT INTO tags(note_id, tag) VALUES(:note_id, :tag)");
        query.bindValue(":note_id", noteId);
        query.bindValue(":tag", i);
        if (!query.exec())
            return false;
    }
    return true;
}