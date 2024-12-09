#!/bin/bash

# Тестовые файлы
echo -e "Hello World\nThis is a test file.\nThis line contains the word test.\n\nThis is a new line.\nAnother test line." > file1.txt
echo -e "Another test file.\nWith some content.\nThis line contains the word test.\n\nAnd some empty lines.\nTest again." > file2.txt
echo -e "Test file for grep.\nThis line should match.\nThis line should not match.\nAnother line with test." > file3.txt

# Функция для выполнения теста
run_test() {
    local flags="$1"
    local pattern="$2"
    local input_file="$3"
    local grep_output="grep_output_${flags// /_}.txt"
    local custom_grep_output="custom_grep_output_${flags// /_}.txt"

    grep $flags "$pattern" "$input_file" > "$grep_output"
    ./s21_grep $flags "$pattern" "$input_file" > "$custom_grep_output"
    
    diff "$grep_output" "$custom_grep_output"
    if [ $? -eq 0 ]; then
        echo "Test with flags '$flags' and pattern '$pattern' passed."
    else
        echo "Test with flags '$flags' and pattern '$pattern' failed."
    fi

    # Удаление временных файлов
    rm -rf "$grep_output" "$custom_grep_output"
}

# Запуск тестов с одиночными флагами
run_test "-e" "test" "file1.txt"
run_test "-i" "TEST" "file1.txt"
run_test "-v" "test" "file1.txt"
run_test "-c" "test" "file2.txt"
run_test "-l" "test" "file2.txt"
run_test "-n" "test" "file3.txt"
run_test "-h" "test" "file3.txt"
run_test "-o" "test" "file1.txt"
run_test "-s" "nonexistent" "file1.txt"

# Создание файла с паттернами для теста с флагом -f
echo -e "test\nnew" > patterns.txt
run_test "-f" "patterns.txt" "file1.txt"

# Запуск тестов с сочетаниями флагов
run_test "-i -v" "test" "file1.txt"  # Игнорировать регистр и исключить строки с "test"
run_test "-c -n" "test" "file3.txt"  # Подсчет совпадений с номерами строк
run_test "-o" "test" "file1.txt"  # Показать только совпадения
run_test "-n -h" "test" "file3.txt"  # Показать номера строк без имени файла
run_test "-s -e" "nonexistent" "file1.txt"  # Игнорировать ошибки и искать несуществующий паттерн

# Удаление тестовых файлов
rm -rf file1.txt file2.txt file3.txt patterns.txt
