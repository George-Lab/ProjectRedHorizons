#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

// Подключить блок разделяемой памяти,
// связаныый с файлом. Если блока нет,
// создать его
int* attach_memory_block(const char* filename, int size);

bool detach_memory_block(int* block);

bool destroy_memory_block(const char* filename);

#endif