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

#include "spellingmenu.h"

#include <KLocalizedString>

#include "ktextedit.h"

SpellingMenu::SpellingMenu(QMenu* parent, Sonnet::Highlighter *highlighter, const QString &word) :
    QMenu { parent },
    m_highlighter { highlighter },
    m_word { word },
    m_suggestions {}
{
    setTitle(i18n("Spelling"));

    setEnabled(m_highlighter->isWordMisspelled(m_word));
    if (isEnabled()) {
        m_suggestions = m_highlighter->suggestionsForWord(m_word);
    }

    connect(this, &QMenu::aboutToShow, this, &SpellingMenu::populateMenu);
}

SpellingMenu::~SpellingMenu() { }

QString SpellingMenu::word() const
{
    return m_word;
}

QStringList SpellingMenu::suggestions() const
{
    return m_suggestions;
}

bool SpellingMenu::isWordMisspelled() const
{
    return m_highlighter->isWordMisspelled(m_word);
}

void SpellingMenu::populateMenu()
{
    for (const QString& suggestion : m_suggestions) {
        QAction *action = new QAction(suggestion, this);
        connect(action, &QAction::triggered,
            [&](bool) { emit replaceWordBySuggestion(suggestion); });
        addAction(action);
    }

    addSeparator();

    QAction* ignoreWord = new QAction(i18n("Ignore Word"), this);
    connect(ignoreWord, &QAction::triggered,
        this, &SpellingMenu::ignoreWord);
    addAction(ignoreWord);

    QAction* addToDict = new QAction(i18n("Add to Dictionary"), this);
    connect(addToDict, &QAction::triggered,
        this, &SpellingMenu::addWordToDictionary);
    addAction(addToDict);

    // make sure we don't do this again
    disconnect(this, &QMenu::aboutToShow, this, &SpellingMenu::populateMenu);
}

void SpellingMenu::addWordToDictionary()
{
    m_highlighter->addWordToDictionary(m_word);
    m_highlighter->rehighlight();
}

void SpellingMenu::ignoreWord()
{
    m_highlighter->ignoreWord(m_word);
    m_highlighter->rehighlight();
}
