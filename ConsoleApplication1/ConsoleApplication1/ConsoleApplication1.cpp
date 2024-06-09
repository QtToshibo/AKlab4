#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

// Функция вывода инструкции
void print_usage() {
    wcout << L"Usage: change_attributes <directory> <pattern> <attributes> [options]\n";
    wcout << L"Options:\n";
    wcout << L"  -h, --help      Show this help message and exit\n";
    wcout << L"Attributes:\n";
    wcout << L"  +h  Set hidden attribute\n";
    wcout << L"  -h  Remove hidden attribute\n";
    wcout << L"  +r  Set read-only attribute\n";
    wcout << L"  -r  Remove read-only attribute\n";
    wcout << L"  +a  Set archive attribute\n";
    wcout << L"  -a  Remove archive attribute\n";
    wcout << L"Exit codes:\n";
    wcout << L"  0   Success\n";
    wcout << L"  1   Error\n";
}

// Функция изменений атриибутов в файле
void change_file_attributes(const wstring& file_path, const wstring& attributes) {
    DWORD fileAttributes = GetFileAttributesW(file_path.c_str());
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        wcerr << L"Could not get file attributes for: " << file_path << L"\n";
        return;
    }

    for (size_t i = 0; i < attributes.size(); ++i) {
        wchar_t sign = attributes[i];
        wchar_t attribute = attributes[++i];

        if (attribute == L'h') {
            if (sign == L'+') {
                fileAttributes |= FILE_ATTRIBUTE_HIDDEN;
            }
            else {
                fileAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
            }
        }
        if (attribute == L'r') {
            if (sign == L'+') {
                fileAttributes |= FILE_ATTRIBUTE_READONLY;
            }
            else {
                fileAttributes &= ~FILE_ATTRIBUTE_READONLY;
            }
        }
        if (attribute == L'a') {
            if (sign == L'+') {
                fileAttributes |= FILE_ATTRIBUTE_ARCHIVE;
            }
            else {
                fileAttributes &= ~FILE_ATTRIBUTE_ARCHIVE;
            }
        }
    }

    if (!SetFileAttributesW(file_path.c_str(), fileAttributes)) {
       wcerr << L"Could not set file attributes for: " << file_path << L"\n";
    }
}

// Функция поиска файла по шаблону и изменений атрибутов в нем
void find_files_and_change_attributes(const wstring& directory, const wstring& pattern, const wstring& attributes) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW((directory + L"\\" + pattern).c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        wcerr << L"FindFirstFile failed in directory: " << directory << L"\n";
        return;
    }
    else {
        do {
            const wstring file_path = directory + L"\\" + findFileData.cFileName;
            change_file_attributes(file_path, attributes);
        } while (FindNextFileW(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
}

int main() {

    print_usage();
    cout << endl;

    wstring directory, pattern, attributes;
    wcout << L"Enter directory: ";
    getline(wcin, directory);
    wcout << L"Enter pattern: ";
    getline(wcin, pattern);
    wcout << L"Enter attributes: ";
    getline(wcin, attributes);

    find_files_and_change_attributes(directory, pattern, attributes);

    return 0;
}
