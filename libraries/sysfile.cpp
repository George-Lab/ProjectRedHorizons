#include "drob.h"
// Конструктор. Инициализируем атрибуты.
SystemFile::SystemFile(){
  result = 0; //указатель на структуру 0
  id = -1; // если файл не открыт, дескриптор -1
  
};

// Деструктор. При удаленни объкта типа SystemFile
// файл должен быть закрыт.
SystemFile::~SystemFile(){

    delete [] filenames;
    if (id > -1){
       close(id);
       id = -1;
    } 
};

//функция класса для открытия файла
unsigned char SystemFile::openf(char* name)
{
   unsigned char er;
// Связывание дескриптора id с именем файла
  id = open(name, O_RDWR| O_CREAT |O_EXCL,0775);
  er = errno;
//Проверка открылся ли файл
    if (errno){
      if (errno == EEXIST){
       id = open(name, O_RDWR );
       er = 0;    
      }else{   
       return er;
      }
   }
// заполнение структуры stat информацией о файле   
   fstat(id, &sb);
// получение имени пользователя по uid
   result = getpwuid(sb.st_uid);  

// получаем inode файла	
	 inode =  sb.st_ino;

// в массиве жестких ссылок пока только одна известная 
	 filenames = new string[sb.st_nlink];
	 filenames[0] = name;
	 hlinks = 1;

// возвращаем код ошибки
   return er;
};

// Печатаем все известные жесткие ссылки
void SystemFile::showAllHardLinks(){
	fstat(id, &sb);
	for (int i = 0; i < hlinks; i++)
		std::cout << filenames[i] << std:: endl;

};

// Проверка является ли этот файл жесткой ссылкой,
// и, если является, добавляем его в массив
int SystemFile::addHardLink(string  pathname){
	int hlink = sb.st_nlink;
	fstat(id, &sb);
	if ( sb.st_nlink < 2)
		return 0;
	if ( hlink < sb.st_nlink )
	{
		string *newFname = new string [sb.st_nlink];
		size_t size = sizeof (string ) * hlink;
		memmove( newFname, filenames, size);
		delete[] filenames;
		filenames = newFname;
	}
	struct stat sbC;
	fstat(id, &sbC);
	if (inode == sbC.st_ino){
		filenames[hlinks] = pathname;
		hlinks++;
	}
	return 1;
};

// запись в файл всего что есть в буфере (побайтно)
bool SystemFile::writef(void* buf, long size){
    long skolko = 0;
// системный вызов 
    skolko = write(id, buf, size);
    if (skolko == size)
      return 1;
    else
      return 0;
};

 bool SystemFile::readf(void* buf, long size) {
    long skolko = 0;
// системный вызов 
    skolko = read(id, buf, size);
    if (skolko == size)
      return 1;
    else
      return 0;
 }

// закрыть файл
bool SystemFile::closef(){
     if (id>-1){
       close(id);
       id = -1;
       return 1;
     }
    return 0;
};

// печать информации о файле
void SystemFile::about(){
    printf("user: %s\n",result->pw_name);
   switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  printf("block device\n");
                 break;
        case S_IFCHR:  printf("character device\n");
            break;
        case S_IFDIR:  printf("directory\n");
               break;
       case S_IFIFO:  printf("FIFO/pipe\n");
               break;
       case S_IFLNK:  printf("symlink\n");
                 break;
       case S_IFREG:  printf("regular file\n"); 
           break;
       case S_IFSOCK: printf("socket\n");
                  break;
       default:       printf("unknown?\n");
   }

// Для получения той же информации можно воспользоваться макросами:
// S_ISREG(m) -обычный файл?, S_ISDIR(m) -  каталог? и др. 

   printf("I-node number:  %ld\n", (long) sb.st_ino);

   printf("Mode:           %lo (octal)\n",(unsigned long) sb.st_mode);

   printf("Link count:    %ld\n", (long) sb.st_nlink);
   printf("Ownership:  UID=%ld   GID=%ld\n",(long) sb.st_uid, (long) sb.st_gid);
   printf("File size:  %lld bytes\n",(long long) sb.st_size);
   printf("Last status change:  %s", ctime(&sb.st_ctime));
   printf("Last file access: %s", ctime(&sb.st_atime));
   printf("Last file modification:   %s",ctime(&sb.st_mtime));
     
           
};

long SystemFile::getSize() {
  return (long) sb.st_size;
}