#ifndef Console_HEADER
#define Console_HEADER

#include <QtCore>

#define console notes::Console::instance()

namespace notes {
    class Console : public QObject {
        Q_OBJECT

        QTextStream m_in;
        QTextStream m_out;

        explicit Console(QObject *parent = nullptr) : QObject(parent), m_out(stdout), m_in(stdin) {}
    public:
        static Console* instance() noexcept { 
            static Console instance;
            return &instance;
        }
        ~Console() {}
        QString requestString(std::function<bool(const QString&)> validator) noexcept;
        qint64 requestNumber(std::function<bool(const qint64)> validator) noexcept;
        QString requestString() noexcept;
        void print(const QString& output) noexcept;
    };
}

#endif /* Console_HEADER */
