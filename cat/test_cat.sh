#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
TEST_FILE="bytes.txt"
LOG_FILE="log.txt"
echo "" > "$LOG_FILE"

# Функция для выполнения теста
run_test() {
    local options="$1"
    echo "$options"
    ./s21_cat $options "$TEST_FILE" > s21_cat.txt
    cat $options "$TEST_FILE" > cat.txt
    if diff -q s21_cat.txt cat.txt > /dev/null; then
        (( COUNTER_SUCCESS++ ))
    else
        echo "$options" >> "$LOG_FILE"
        (( COUNTER_FAIL++ ))
    fi
    rm s21_cat.txt cat.txt
}

# Тестирование с одной опцией
for var in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
    run_test "$var"
done

# Тестирование с двумя опциями
for var in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
    for var2 in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
        if [ "$var" != "$var2" ]; then
            run_test "$var $var2"
        fi
    done
done

# Тестирование с тремя опциями
for var in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
    for var2 in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
        for var3 in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
            if [ "$var" != "$var2" ] && [ "$var2" != "$var3" ] && [ "$var" != "$var3" ]; then
                run_test "$var $var2 $var3"
            fi
        done
    done
done

# Тестирование с четырьмя опциями
for var in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
    for var2 in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
        for var3 in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
            for var4 in -b -e -n -s -t -v --number-nonblank --number --squeeze-blank; do
                if [ "$var" != "$var2" ] && [ "$var2" != "$var3" ] && [ "$var" != "$var3" ] && [ "$var" != "$var4" ] && [ "$var2" != "$var4" ] && [ "$var3" != "$var4" ]; then
                    run_test "$var $var2 $var3 $var4"
                fi
            done
        done
    done
done
rm -rf log.txt
echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"

exit "$COUNTER_FAIL"
