/* This file is part of the KDE libraries
   Copyright (C) 2019 Tommy Lincoln <pajamapants3000@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef SPELLINGMENU_H
#define SPELLINGMENU_H

#include <QMenu>
#include <QString>

namespace Sonnet {
    class Highlighter;
}

/**
 * @class SpellingMenu spellingmenu.h
 *
 * @short A menu of spelling suggestions and actions associated with a
 * single specific, possibly misspelled, word.
 *
 * This menu can easily be embedded in a context menu, and/or queried for
 * information such as spelling suggestions provided by the Sonnet
 * Highlighter provided at construction. @c replaceWordBySuggestion
 * signals when spell correction is requested by the end-user, and provides
 * the replacement word to the slot.
 *
 * Dictionary updates ("add word to dictionary" and "ignore word")
 * are handled internally, without any extra effort required from the parent.
 * Slots are available to facilitate these actions on-demand if needed.
 *
 * This class was originally written to support integrated spelling
 * suggestions in the KTextEdit context menu. It is intended to be usable
 * with any menu; subclasses may override populateMenu() to customize
 * available actions, their behavior, and their layout.
 *
 * @see QMenu
 * @see KTextEdit
 * @see Sonnet::Highlighter
 * @author Tommy Lincoln <pajamapants3000@gmail.com>
 */
class SpellingMenu : public QMenu
{
    Q_OBJECT
    Q_PROPERTY(QString word READ word CONSTANT)
    Q_PROPERTY(QStringList suggestions READ suggestions CONSTANT)
    Q_PROPERTY(bool isWordMisspelled READ isWordMisspelled CONSTANT)

public:
    /**
     * @brief Constructs a SpellingMenu
     * @param parent the parent menu which will contain this SpellingMenu instance
     * @param highlighter the Sonnet highlighter which will provide spelling
     * functionality and information
     * @param word the word for which spelling options will be provided in this
     * SpellingMenu instance
     */
    SpellingMenu(QMenu* parent, Sonnet::Highlighter *highlighter, const QString &word);
    /**
     * @brief Destroys the SpellingMenu object and components
     */
    virtual ~SpellingMenu();

    /**
     * @return the @c word argument provided at construction
     */
    QString word() const;

    /**
     * @return list of spelling suggestions provided by the Sonnet highlighter
     * for @c word
     */
    QStringList suggestions() const;

    /**
     * @return true if @c word is misspelled according to the Sonnet highlighter,
     * false otherwise
     */
    bool isWordMisspelled() const;

protected:
    /**
     * @brief loads spelling suggestions and other available
     * actions into the menu
     */
    virtual void populateMenu();

Q_SIGNALS:
    /**
     * @brief emitted when the end-user selects a spelling suggestion
     * @param suggestion the suggestion selected by the end user
     */
    void replaceWordBySuggestion(const QString &suggestion);

protected Q_SLOTS:
    void addWordToDictionary();
    void ignoreWord();

private:
    Sonnet::Highlighter *m_highlighter;
    QString m_word;
    QStringList m_suggestions;
};

#endif // SPELLINGMENU_H
