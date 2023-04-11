#pragma warning (disable: 4996)
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

struct Student {
	char name[256];
	Student* ptrNext;
	Student* ptrPrev;
};

Student* head = NULL;		// элементы отсюда выходят
Student* tail = NULL;		// элементы сюда поступают
int n = 0;
bool flag1 = false;

void create(const int);
void pop(char*);
void push(char*);
void output();				// Перешло от двунаправленных списков. (поправить)
void outputRight();			// 1 - выход из очереди; n - вход
void destroy();
void pluralPush(const int);
void add(const int pos, const int K, FILE*);
void chop(const int, char*);				// удаление элемента из очереди
void restore(FILE*);
void write(FILE*);
void menu(const int);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	while (true) {
		system("pause");
		system("cls");
		cout << "Здравствуйте, что вы хотите сделать?" << endl;
		cout << "1. Создать очередь" << endl;
		cout << "2. Добавить по K элементов к элементу на позиции pos слева и справа" << endl;
		cout << "3. Удалить элемент из очереди" << endl;
		cout << "4. Напечатать очередь" << endl;
		cout << "5. Записать очередь в файл" << endl;
		cout << "6. Уничтожить очередь" << endl;
		cout << "7. Восстановить очередь из файла" << endl; cout << endl;
		cout << "Введите c" << endl;
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	/*
	int b;
	cout << "Введите n" << endl;
	cin >> b;			// Глобальная переменная увеличивается с push, поэтому переменная b для функции create
	cout << "Введите идентификаторы " << b << " пользователей" << endl; cin.ignore();
	create(b);
	cout << "1 - выходит из очереди, n - входит" << endl;
	output();
	outputRight();

	FILE* file = NULL;
	int pos, K;
	cout << "pos, K? " << "pos относительно выхода (1)" << endl;
	cin >> pos >> K;
	
	cin.get();
	add(pos, K, file);
	
	output();
	outputRight();

	
	write(file);
	destroy();
	if ((file = fopen("myFile", "rb+")) == NULL) exit(2);
	restore(file);
	fclose(file);
	output();
	outputRight();
	
	*/
	return 0;
}
void create(const int num) {
	for (int i = 0; i < num; i++) {
		char nameT[255 + 1];
		cin.getline(nameT, 255);
		push(nameT);
	}
}
void pop(char* data) {					// попится из head
	Student* popper = head;
	head = head->ptrNext;
	strcpy(data, popper->name);
	delete popper;
	if (head != NULL) head->ptrPrev = NULL;
	n--;
	if (n == 0) {
		head = NULL; tail = NULL;
	}
}
void pop() {							// попится из head
	Student* popper = head;
	head = head->ptrNext;
	delete popper;
	if (head != NULL) head->ptrPrev = NULL;
	n--;
	if (n == 0) {
		head = NULL; tail = NULL;
	}
}
void push(char* data) {					// пушится в tail
	Student* newElem = new(Student);
	Student* ptr = tail;
	tail = newElem;
	tail->ptrPrev = ptr;
	tail->ptrNext = NULL;
	if (ptr != NULL) ptr->ptrNext = tail;
	strcpy(tail->name, data);
	n++;
	if (n == 1) { head = newElem; }
}
void output() {								// не метод очереди
	cout << "Ваша очередь: " << endl;
	int counter = 1;
	for (Student* ptr = head; ptr != NULL;) {
		cout << counter++ << " " << ptr->name << endl;
		ptr = ptr->ptrNext;
	}
}
void outputRight() {					// не метод очереди
	cout << "Ваша очередь: " << endl;
	int counter = n;
	for (Student* ptr = tail; ptr != NULL;) {
		cout << counter-- << " " << ptr->name << endl;
		ptr = ptr->ptrPrev;
	}
}
void destroy() {		// не метод очереди
	while (head != NULL) {
		Student* ptr = head;
		head = head->ptrNext;
		delete ptr;
	}
	tail = NULL;
	n = 0;
}
void pluralPush(const int K) {
	char tmp[256];
	int i = 0;
	while (i < K) {
		cout << "Идентификатор? " << endl;
		gets_s(tmp, 256);
		push(tmp);
		i++;
	}
}
void add(const int pos, const int K, FILE* file) {
	char tmp[256];

	if ((file = fopen("tmpFile", "wb+")) == NULL) exit(1);
	while (head != NULL) {
		pop(tmp);
		fwrite(tmp, sizeof(tmp), 1, file);
	}
	fclose(file);

	if ((file = fopen("tmpFile", "rb+")) == NULL) exit(1);
	int i = 1;
	while (!feof(file) && i < pos) {
		fread(tmp, sizeof(tmp), 1, file);
		push(tmp);
		i++;
	}
	pluralPush(K);
	fread(tmp, sizeof(tmp), 1, file);
	push(tmp);
	pluralPush(K);

	while (!feof(file)) {
		fread(tmp, sizeof(tmp), 1, file);
		if (!feof(file)) push(tmp);
		i++;
	}
	fclose(file);

}
void chop(const int pos, char* data=NULL) {
	char tmp[256];
	for (int i = 1; i < pos; i++) {
		pop(tmp);
		push(tmp);
	}
	pop();
	for (int a = 0; a < n - pos + 1; a++) {
		pop(tmp);
		push(tmp);
	}
}
void restore(FILE* file) {
	while (!feof(file)) {
		char tmp[256];
		fread(tmp, sizeof(tmp), 1, file);
		if (!feof(file)) { push(tmp); }
	}
}
void write(FILE* file) {
	char tmp[256];
	if ((file = fopen("myFile", "wb+")) == NULL) exit(2);
	while (head != NULL) {
		pop(tmp);
		fwrite(tmp, sizeof(tmp), 1, file);
	}
	fclose(file);
	if ((file = fopen("myFile", "rb+")) == NULL) exit(2);
	restore(file);
	fclose(file);
}
void menu(const int c) {
	FILE* file = NULL;

	if ((c == 1 || c == 7) && flag1) {					// для случая, когда список уже создан
		cout << "Вы уже создали очередь" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !flag1) {				// для случая, когда список еще не создан
		cout << "Создайте/ восстановите очередь" << endl;
		return;
	}
	else if (c >= 2 && c <= 5 && n == 0) { cout << "Очередь пуста." << endl; return; }
	switch (c) {
	case 1:
	{	
		int b;			// инт b, так как креэйт работает через пуш
		flag1 = true;
		cout << "Введите количество узлов очереди: ";
		cin >> b;
		while (b <= 0) {
			cout << "Нельзя ввести столько узлов" << endl;
			cin >> b;
		}
		cout << "Введите индетификатор для " << b << " пользователей: " << endl; cin.ignore();
		create(b);
		break;
	}
	case 2:
	{
		cout << "Введите позицию для добавления и количество элементов для добавления" << endl;
		int pos, K;
		cin >> pos >> K;
		cin.get();									// нормализация буфера для работы без перебоев
		while (pos > n) { cout << "В очереди всего " << n << "позиций" << endl; cin >> pos; cin.get(); }
		add(pos, K, file);			// добавление элементов с двух сторон
		cout << endl;
		output();
		break;
	}
	case 3:
	{
		int pos;
		cout << "Введите позицию для удаления" << endl;
		cin >> pos;
		while (pos > n) { cout << "В списке всего " << n << " позиций" << endl; cin >> pos; }
		chop(pos, NULL);
		cout << endl;
		output();
		break;
	}
	case 4:
	{
		output();
		outputRight();
		cout << endl;
		break;
	}
	case 5:
	{
		if ((file = fopen("myFile", "wb+")) == NULL) exit(5);
		write(file);
		fclose(file);

		break;
	}
	case 6:
	{
		destroy();
		cout << "Очередь удалена" << endl;
		flag1 = false;
		break;
	}

	case 7:
	{
		cout << "Восстановление очереди из файла" << endl;
		int n1 = n;
		if ((file = fopen("myFile", "rb+")) == NULL) {
			if ((file = fopen("myFile", "wb+")) == NULL) exit(6);
			if ((file = fopen("myFile", "rb+")) == NULL) exit(6);
		}
		restore(file);
		fclose(file);
		if (n - n1 == 0) cout << "Нечего восстанавливать" << endl;
		else flag1 = true;
		break;
	}
	default:
	{
		cout << "Выход из меню" << endl;
		destroy();
		exit(777);
	}
	}
}