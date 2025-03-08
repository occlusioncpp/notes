#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <QtCore>

namespace notes {
    inline QStringList clearTags(const QStringList& tags) noexcept {
        QStringList clearedTags = tags;
        clearedTags.removeDuplicates();
        clearedTags.removeAll("");
        for (auto& tag : clearedTags) {
            tag = tag.trimmed();
        }
        clearedTags.replaceInStrings(QRegularExpression("\\s+"), "-");
        return clearedTags;
    }
}

#endif /* UTILS_HEADER */
