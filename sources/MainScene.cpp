#include <MainScene.hpp>
#include <Database.hpp>
#include <Console.hpp>
#include <EditingNoteScene.hpp>
#include <Utils.hpp>

using namespace notes;

bool MainScene::tryOpenDatabase(const QString& notesFile, const QByteArray& password) noexcept {
    if (password.isEmpty()) {
        return Database::instance()->openDatabase(notesFile);
    } else {
        return Database::instance()->openDatabase(
            notesFile,
            password.toHex()
        );
    }
}

MainScene::Actions MainScene::requestActions() noexcept {
    console->print("Select action:");
    console->print("1. Create note");
    console->print("2. Edit note");
    console->print("3. Delete note");
    console->print("4. View notes by tag");
    console->print("5. Delete notes by tag");
    console->print("6. Exit");
    return static_cast<Actions>(console->requestNumber([](const qint64 number) { return number >= 1 && number <= 6; }));
}

void MainScene::mainCycle() {
    if (!Database::instance()->isOpen())
        throw std::runtime_error("Database is not open");
    while(true) {
        if (showTags())
            console->print("");

        Actions action = requestActions();
        switch(action) {
            case Actions::CreateNote:
                createNote();
                system("pause");
                system("cls");
                break;
            case Actions::EditingNote:
                if (!EditingNoteScene::instance()->startEditingScene())
                    return;
                break;
            case Actions::DeleteNote:
                deleteNote();
                system("pause");
                system("cls");
                break;
            case Actions::DeleteNotesByTag:
                deleteNotesByTag();
                system("pause");
                system("cls");
                break;
            case Actions::ViewNotesByTag:
                showNotesByTag();
                system("pause");
                system("cls");
                break;
            case Actions::Exit:
                Database::instance()->closeDatabase();
                std::terminate();
                return;
        }
    }
}

void MainScene::deleteNotesByTag() noexcept {
    auto tags = Database::instance()->getTags();
    if (!tags.has_value() || tags.value().isEmpty()) {
        console->print("Tags not found");
        return;
    }
    console->print("Select tag:");
    for (auto tag : tags.value())
        console->print(tag);
    QString tag = console->requestString([tags](const QString& tag) -> bool {
        return tags.value().contains(tag);
    });
    console->print("Are you sure that you want to delete notes by tag \"" + tag + "\"? (y/n)");
    QString answer = console->requestString([](const QString& answer) -> bool {
        return answer == "y" || answer == "n";
    });
    if (answer == "y") {
        Database::instance()->deleteNotesByTag(tag);
        console->print("Notes by " + tag + " deleted");
    }
}

bool MainScene::showTags() noexcept {
    auto tags = Database::instance()->getTags();
    if (tags.has_value() && !tags.value().isEmpty()) {
        console->print("Available tags:");
        for (auto tag : tags.value()) {
            console->print(tag);
        }
        return true;
    }
    return false;
}
void MainScene::showNotesByTag() noexcept {
    auto tags = Database::instance()->getTags();
    if (!tags.has_value() || tags.value().isEmpty()) {
        console->print("Tags not found");
        return;
    }
    console->print("Select tag:");
    for (auto tag : tags.value())
        console->print(tag);
    QString tag = console->requestString([tags](const QString& tag) -> bool {
        return tags.value().contains(tag);
    });
    auto notes = Database::instance()->getNotesByTag(tag);
    if (!notes.has_value()) {
        console->print("Notes not found");
        return;
    }
    console->print("Notes by " + tag + ":");
    for (qsizetype x = 0; x < notes.value().size(); x++) {
        console->print("Note " + QString::number(x + 1) + ": " + notes.value().at(x).title());
    }
}

void MainScene::createNote() noexcept {
    console->print("Enter tags by comma:");
    QString tag = console->requestString([](const QString& tag) -> bool {
        QStringList tags = clearTags(tag.split(","));
        return !tags.isEmpty();
    });
    QStringList tags = clearTags(tag.split(","));
    console->print("Enter title for note:");
    QString title = console->requestString([](const QString& title) -> bool {
        return !title.isEmpty();
    });

    console->print("Enter content for note:");
    QString content = console->requestString([](const QString& content) -> bool {
        return !content.isEmpty();
    });

    //Create note
    Note note;
    note.setContent(content);
    note.setDate(QDateTime::currentDateTime());
    note.setId(QUuid::createUuid());
    note.setTitle(title);
    note.setTags(tags);
    if (Database::instance()->createNote(note)) {
        console->print("Note created");
    } else {
        console->print("Note creating is failed");
    }
}
void MainScene::deleteNote() noexcept {
    auto tags = Database::instance()->getTags();
    if (!tags.has_value() || tags.value().isEmpty()) {
        console->print("Tags not found");
        return;
    }
    console->print("Type tag of note you want to delete:");
    for (auto i : tags.value())
        console->print(i);

    QString tag = console->requestString([tags](const QString& tag) -> bool {
        return tags.value().contains(tag);
    });
    auto notes = Database::instance()->getNotesByTag(tag);
    if (!notes.has_value()) {
        console->print("Notes not found");
        return;
    }
    console->print("Type index of note you want to delete:");
    for (qsizetype x = 0; x < notes.value().size(); x++) {
        console->print("Note " + QString::number(x + 1) + ": " + notes.value().at(x).title());
    }
    qsizetype index = console->requestNumber([notes](const qsizetype index) -> bool {
        return index > 0 && index <= notes.value().size();
    }) - 1;

    console->print("Are you sure that you want to delete note \"" + notes.value().at(index).title() + "\"? (y/n)");
    QString answer = console->requestString([](const QString& answer) -> bool {
        return answer == "y" || answer == "n";
    });
    if (answer == "n")
        return;
    Note note = notes.value().at(index);
    if (Database::instance()->deleteNote(note.id())) {
        console->print("Note deleted");
    } else {
        console->print("Note deleting is failed");
    }
}