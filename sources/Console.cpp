#include <Console.hpp>

using namespace notes;

QString Console::requestString() noexcept {
    return m_in.readLine();
}

void Console::print(const QString& output) noexcept {
    m_out << output << Qt::endl;
}

QString Console::requestString(std::function<bool(const QString&)> validator) noexcept {
    QString input = requestString();
    while (!validator(input)) {
        input = requestString();
    }
    return input;
}
qint64 Console::requestNumber(std::function<bool(const qint64)> validator) noexcept {
    while(true) {
        QString input = requestString();
        bool ok = false;
        qint64 number = input.toLongLong(&ok);
        if (ok && validator(number)) return number;
    }
}