# API c++ для работы с CSV

###  data_interface.h
```c++
class  IDataStorage
```
Интерфейс описывает базовые методы работы с табличными данными, не специфические конкретно для CSV.

### csv_lib.h
```c++
class TCSVParser
```
Класс - парсер (сериализация и десериализация) данных

Десериализация:
```c++
[[nodiscard]] TCSVStorage  Deserialize(
	const  std::string&  fileName,
	const  char  symbolSeparator  =  ',',
	const  char  symbolQuote  =  '"',
	const  EDataHasTitle  titleMode  =  NO_TITLE,
	const  std::vector<std::pair<uint64_t, std::string>>&  specialColumnNames  = {}
) const;
```
*  fileName - имя файла с данными для чтения
*  symbolSeparator - символ-разделитель (по умолчанию ',')
*  symbolQuote - символ-кавычка (по умолчанию '"') 
*  titleMode - флаг наличия названий столбцов у данных (принимает HAS_TITLE/NO_TITLE, по умолчанию NO_TITLE) 
*  specialColumnNames - выбор новых имен для колонок (вектор пар <индекс_столбца, новое_название>)
У этого метода есть перегрузки для различной конфигурации параметров.

Сериализация:
```c++
void  Serialize(
	TCSVStorage&  currentStorage,
	const  std::string&  fileName,
	const  char  symbolSeparator  =  ',',
	const  char  symbolQuote  =  '"'
) const;
```
*  currentStorage - данные (хранилище) для записи
*  fileName - имя файла с данными для чтения
*  symbolSeparator - символ-разделитель (по умолчанию ',')
*  symbolQuote - символ-кавычка (по умолчанию '"') 

```c++
class TCSVStorage
```
Класс - хранилище (доступ и модификация) данных.

========
Методы получения данных:

Ячейка:
```c++
const  std::string&  GetCell(const  uint64_t  lineIndex, const  std::string&  columnName) const;
const  std::string&  GetCell(const  uint64_t  lineIndex, const  uint64_t  columnIndex) const  override;
```
* lineIndex - номер строки
*  columnName/columnIndex - индекс или имя столбца

Столбец:
```c++
const  std::vector<std::string>&  GetColumn(const  std::string&  fieldName) const;
const  std::vector<std::string>&  GetColumn(const  uint64_t  fieldIndex) const  override;
const  std::string&  GetColumnName(const  uint64_t  fieldIndex) const;
```
* fieldIndex/fieldName - индекс или имя столбца
* GetColumnName(...) - получение имени столбца по индексу

Строка:
```c++
std::vector<std::string> GetLine(const  uint64_t  fieldIndex) const  override;
```
* fieldIndex - индекс строки

Размеры:
```c++
uint64_t  ColumnCount() const  override; //количество столбцов
uint64_t  Size() const  override; //количество строк (не считая title)
```

========
Модификация данных:

Удаление:
```c++
void  DeleteLine(const  uint64_t  fieldIndex) override; //удаление строки
void  DeleteColumn(const  std::string&  fieldName); //удаление столбца по названию
void  DeleteColumn(const  uint64_t  fieldIndex) override; //удаление столбца по индексу
```
Изменение ячейки:
```c++
void  SetCell(const  uint64_t  lineIndex, const  uint64_t  columnIndex, const  std::string&  value) override; // по индексу строки и столбца
void  SetCell(const  uint64_t  lineIndex, const  std::string&  columnName, const  std::string&  value); // по индексу строки и названию столбца
```
* value -  новое значение

Изменение названий столбцов
```c++
void  SetColumnNames(const  std::vector<std::pair<uint64_t, std::string>>&  specialColumnNames);
```
* specialColumnNames - выбор новых имен для колонок (вектор пар <индекс_столбца, новое_название>)

### csv_lib.сpp - файл реализации методов классов
### example.cpp - пример использования API




