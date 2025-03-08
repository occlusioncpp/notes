#ifndef NOTE_HEADER
#define NOTE_HEADER

#include <QtCore>

namespace notes {
    class Note {

    public:
        explicit Note() {}
        ~Note() {}

        QString title() const { return m_title; }
        void setTitle(const QString& title) { m_title = title; }

        QString content() const { return m_content; }
        void setContent(const QString& content) { m_content = content; }

        QDateTime date() const { return m_date; }
        void setDate(const QDateTime& date) { m_date = date; }

        QList<QString> tags() const { return m_tags; }
        void setTags(const QList<QString>& tags) { m_tags = tags; }

        QUuid id() const { return m_id; }
        void setId(const QUuid& id) { m_id = id; }
    private:
        QUuid m_id;
        QString m_title;
        QString m_content;
        QDateTime m_date;
        QList<QString> m_tags;
    };
}

#endif /* NOTE_HEADER */
