#!/bin/bash

DATA_TYPE="int16_t"
HASH_TYPE="MD5"
SALT_TYPE="client"

# Проверка, какие процессы используют порт 33333 и их завершение
PIDS=$(lsof -t -i:33333)
if [ -n "$PIDS" ]; then
  echo "Завершение процессов, использующих порт 33333: $PIDS..."
  kill -9 $PIDS
else
  echo "Порт 33333 свободен..."
fi

echo "Запуск сервера с параметрами -T $DATA_TYPE -H $HASH_TYPE -S $SALT_TYPE..."

# Запуск сервера в новой консоли
./server -T $DATA_TYPE -H $HASH_TYPE -S $SALT_TYPE

# Выход
exit
