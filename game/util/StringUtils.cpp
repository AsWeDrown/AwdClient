// Разные константы
#define W_DOUBLE_SPACE L"  "
#define W_SPACE_LN L" \n"


#include <regex>
#include <iostream>
#include "StringUtils.hpp"

namespace awd::game {

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *
     *   PUBLIC
     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::wstring StringUtils::encodeFormatting(std::wstring message) {
        // Стили
        message = std::regex_replace(message, std::wregex(L"\\{RESET\\}"),         L"&r");
        message = std::regex_replace(message, std::wregex(L"\\{BOLD\\}"),          L"&l");
        message = std::regex_replace(message, std::wregex(L"\\{ITALIC\\}"),        L"&o");
        message = std::regex_replace(message, std::wregex(L"\\{UNDERLINED\\}"),    L"&n");
        message = std::regex_replace(message, std::wregex(L"\\{STRIKETHROUGH\\}"), L"&m");

        // Цвета
        message = std::regex_replace(message, std::wregex(L"\\{WHITE\\}"),         L"&f");
        message = std::regex_replace(message, std::wregex(L"\\{GRAY\\}"),          L"&7");
        message = std::regex_replace(message, std::wregex(L"\\{BLACK\\}"),         L"&0");
        message = std::regex_replace(message, std::wregex(L"\\{YELLOW\\}"),        L"&e");
        message = std::regex_replace(message, std::wregex(L"\\{GOLD\\}"),          L"&6");
        message = std::regex_replace(message, std::wregex(L"\\{GREEN\\}"),         L"&a");
        message = std::regex_replace(message, std::wregex(L"\\{RED\\}"),           L"&c");
        message = std::regex_replace(message, std::wregex(L"\\{AQUA\\}"),          L"&b");
        message = std::regex_replace(message, std::wregex(L"\\{BLUE\\}"),          L"&9");
        message = std::regex_replace(message, std::wregex(L"\\{MAGENTA\\}"),       L"&d");

        return message;
    }

    std::wstring StringUtils::fixSpacing(std::wstring message) {
        // Избавляемся от ситуаций, когда пробел стоит перед символом перехода на новую строку.
        message = std::regex_replace(message, std::wregex(W_SPACE_LN), L"\n");

        // Избавляемся от двух идущих подряд пробелов. Делаем это в цикле,
        // чтобы также избавиться от трёх, четырёх, ... N пробелов подряд.
        while (message.find(W_DOUBLE_SPACE) != std::wstring::npos)
            message = std::regex_replace(message, std::wregex(W_DOUBLE_SPACE), L" ");

        return message;
    }

    std::wstring StringUtils::wrapByLineLength(std::wstring message,
                                               uint32_t maxLineLen, uint32_t maxLinesNum) {
        // Выделяем из сообщения отдельные слова (с учётом пробелов и символов новой строки '\n';
        // кроме того, чересчур длинные слова будут разбиваться на несколько отдельных).
        message = fixSpacing(message);
        message += L' '; // добавляем в конец пробел, чтобы нормально выделились все слова (в т.ч. последнее)
        std::vector<std::wstring> words;
        std::wstring currentWord;

        for (const wchar_t ch : message) {
            if (ch == L' ' || ch == L'\n' || currentWord.length() == maxLineLen) {
                if (!currentWord.empty()) { // может быть, если в сообщении несколько пробелов или '\n' идут подряд
                    words.push_back(ch == L'\n' ? currentWord + L'\n' : currentWord);
                    currentWord = L"";
                }
            } else
                currentWord += ch;
        }

        // Для корректной, упрощённой обработки строк (костыль, короче).
        if (!words.empty())
            words.emplace_back(L"\n");

        // Разбиваем слова на строки (с учётом символов '\n' и
        // переноса строки при превышении максимальной длины).
        std::wstring result;
        std::wstring currentLine;
        uint32_t     currentLinesNum  = 0;
        bool         needLeadingSpace = false;

        for (const auto& word : words) {
            std::wstring::size_type lineLenAfter = currentLine.length() + word.length();

            if (needLeadingSpace)
                lineLenAfter++; // пробелы тоже участвуют в подсчёте длины

            if (lineLenAfter > maxLineLen) {
                // Слово не влезает в эту строку.
                if (currentLinesNum < maxLinesNum) {
                    // Есть ещё свободные строки. Переходим на строку ниже.
                    result += currentLine + L'\n';
                    currentLinesNum++;
                    currentLine = word; // следующую строку начинаем с этого (не влезшего) слова
                } else {
                    std::wcerr << L"Cannot insert any more lines, "
                                  L"stopped before: \"" << currentLine << "\"" << std::endl;
                    break;
                }
            } else {
                // Слово влезает в эту строку.
                if (needLeadingSpace)
                    currentLine += L' ';
                else
                    needLeadingSpace = true;

                // Дополняем текущую строку новым словом (котророе, как мы убедились, влезает).
                wchar_t lastWordChar = word[word.length() - 1];
                bool forceNewLine = lastWordChar == L'\n';

                if (forceNewLine) {
                    if (word.length() > 1)
                        currentLine += word.substr(0, word.length() - 1);
                } else
                    currentLine += word;

                if (forceNewLine) {
                    // Принудительный переход на новую строку или последнее слово в списке.
                    if (currentLinesNum < maxLinesNum) {
                        // Есть ещё свободные строки. Переходим на строку ниже.
                        result += currentLine + L'\n';
                        currentLinesNum++;
                        currentLine = L""; // следующая строка будет пустой (принудительный переход)
                        needLeadingSpace = false; // по этой ^ причине перед первым словом не должно быть пробела
                    } else {
                        std::wcerr << L"Cannot insert any more lines, "
                                      L"stopped before: \"" << currentLine << "\"" << std::endl;
                        break;
                    }
                }
            }
        }

        // Удаляем хвост (лишние символы '\n' и/или пробелы в конце).
        if (!result.empty()) {
            wchar_t lastChar = result[result.length() - 1];

            while (lastChar == L'\n' || lastChar == L' ') {
                result = result.substr(0, result.length() - 1);
                lastChar = result[result.length() - 1];
            }
        }

        return result;
    }

}

