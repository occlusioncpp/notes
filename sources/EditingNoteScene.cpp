#include <EditingNoteScene.hpp>
#include <Console.hpp>
#include <Database.hpp>
#include <Utils.hpp>

using namespace notes;

bool EditingNoteScene::startEditingScene() noexcept {
    if (!requestEditingInformation())
        return true;
    while(true) {
        console->print("Title: " + m_note.title());
        console->print("Date: " + m_note.date().toString());
        console->print("Tags: " + m_note.tags().join(", "));
        console->print("Content: \n\n" + m_note.content() + "\n");

        Actions action = requestActions();
        switch (action) {
            case Actions::EditContent:
                editContent();
                system("pause");
                system("cls");
                break;
            case Actions::EditTag:
                editTag();
                system("pause");
                system("cls");
                break;
            case Actions::EditTitle:
                editTitle();
                system("pause");
                system("cls");
                break;
            case Actions::BackToView:
                system("cls");
                return true;
            case Actions::Exit:
                return false;
        }
    }
}

bool EditingNoteScene::requestEditingInformation() noexcept {
    auto tags = Database::instance()->getTags();
    if (!tags.has_value() || tags.value().isEmpty()) {
        console->print("Tags not found");
        return false;
    }
    console->print("Enter tag for note");
    for (auto tag : tags.value())
        console->print(tag);
    QString tag = console->requestString([tags](const QString& tag) -> bool {
        return tags.value().contains(tag);
    });
    console->print("Enter index of note you want to edit");
    auto notes = Database::instance()->getNotesByTag(tag);
    if (!notes.has_value()) {
        console->print("Notes not found");
        return false;
    }
    for (qsizetype x = 0; x < notes.value().size(); x++)
        console->print("Note " + QString::number(x + 1) + ": " + notes.value().at(x).title());
    qint64 index = console->requestNumber([notes](const qint64 index) -> bool {
        return index >= 1 && index <= notes.value().size();
    }) - 1;
    m_note = notes.value().at(index);
    system("cls");
    return true;
}

void EditingNoteScene::editContent() noexcept {
    console->print("Enter new content: ");
    m_note.setContent(console->requestString());
    Database::instance()->updateNote(m_note);
}
void EditingNoteScene::editTag() noexcept {
    console->print("Enter new tags by comma:");
    QString notParsedTags = console->requestString([](const QString& tag) -> bool {
        QStringList tags = clearTags(tag.split(","));
        return !tags.isEmpty();
    });
    QStringList tags = clearTags(notParsedTags.split(","));
    m_note.setTags(tags);
    Database::instance()->changeTags(m_note.id(), tags);
}
void EditingNoteScene::editTitle() noexcept {
    console->print("Enter new title:");
    m_note.setTitle(console->requestString());
    Database::instance()->updateNote(m_note);
}

EditingNoteScene::Actions EditingNoteScene::requestActions() noexcept {
    console->print("Select action:");
    console->print("1. Edit content");
    console->print("2. Edit tag");
    console->print("3. Edit title");
    console->print("4. Back to view");
    console->print("5. Exit");
    return static_cast<Actions>(console->requestNumber([](const qint64 number) { return number >= 1 && number <= 5; }));
}