#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std; //Юзаємо простір імен std
int hash(int id){
	return id%7 +1  ;
}
class Header { //Клас заголовку
	int hash; //І номер його
	unsigned int num; //І кількість записів його
	unsigned int pointer; //І вказівник його
public:
	//Конструктор
	Header(int h = 0,int n = 0,int p = 0){
		this->hash = h;
		this->num = n;
		this->pointer = p;
	}
	int Hash(){return this->hash;}
	unsigned int Num(){return this->num;}
	unsigned int Pointer(){return this->pointer;}
	void Hash(int h){ this->hash = h;}
	void Num(int n){ this->num = n;}
	void Pointer(int p){ this->pointer = p;}
	friend ostream& operator<< (ostream&,Header&);
	friend istream& operator>> (istream&,Header&);
};
class Data { //Клас запису даних
private:
	//Поля
	int id;
	string field1;
	string field2;
	string field3;
	string field4;
	string field5;
	string field6;
	string field7;
	string field8;
	string field9;
public:
	int operation;
	friend ostream& operator<< (ostream&,Data&);
	friend istream& operator>> (istream&,Data&);
	Data& operator= (Data&);
	int Id(){return this->id;}
	string Name(){return this->field1;}
	string Flam_des(){return this->field3;}
	string S_class(){return this->field9;}
	//Конструктор
	Data(int a = 0,string b = "0",string c = "0",string d = "0",string e = "0",string f = "0",string g = "0",string h = "0",string i = "0",string j = "0"){
		this->id = a;
		this->field1 = b;
		this->field2 = c;
		this->field3 = d;
		this->field4 = e;
		this->field5 = f;
		this->field6 = g;
		this->field7 = h;
		this->field8 = i;
		this->field9 = j;
		this->operation = 0;
	}
};
class DataBase{ //Клас бази даних
private:
	string filename;
	//Умовна компіляція
#ifdef _WIN32 
	static const int HeaderSize = 20; //Розмір заголовка
	static const int RecordSize = 156; //Розмір запису
#endif
#ifdef __linux
	static const int HeaderSize = 19; 
	static const int RecordSize = 155;
#endif
        //static const int HeaderSize = 19;
        //static const int RecordSize = 155;
	static const int BlockSize = RecordSize*7+HeaderSize;
public:
	DataBase(const char name[] = " "){//Конструктор класу
		filename = string(name);
		ofstream a(name,ios::app);
		if(!a.is_open()){
			cout << "File were not opened!"<< endl ;
		}
		a.close();
	};
	string Filename(){return this->filename;}
	void Filename(string s){
		this->filename = s;
	}
	//Функції для роботи з базою даних
	int creat();
	int insert(Data&);
	int del(int);
	int find(int);
	int search();
	int modify(Data&);
	int show();
	int alloc(fstream&);
	int garbageCollector(string);
};
ostream& operator<< (ostream& stream,Data& d){ //Вивід запису в потік
	stream.width(5); //Фіксована ширина поля
	stream << d.id <<' ';
	stream.width(10);
	stream << d.field1 << ' ';
	stream.width(2);
	stream << d.field2 << ' ';
	stream.width(8);
	stream << d.field3 << ' ';
	stream.width(20);
	stream << d.field4 << ' ';
	stream.width(20);
	stream << d.field5 << ' ';
	stream.width(20);
	stream << d.field6 << ' ';
	stream.width(20);
	stream << d.field7 << ' ';
	stream.width(20);
	stream << d.field8 << ' ';
	stream.width(20);
	stream << d.field9 << '\n';
	return stream;
}
ostream& operator<< (ostream& stream,Header& h){ //Вивід заголовка в потік
	stream.width(3);
	stream << h.hash <<' ';
	stream.width(3);
	stream << h.num << ' ';
	stream.width(10);
	stream << h.pointer << endl;
	return stream;
}
istream& operator>> (istream& stream,Data& d){ //Читання запису з потоку
	stream >> d.id >> d.field1 >> d.field2 >> d.field3 >> d.field4 >> d.field5 >> d.field6 >> d.field7 >> d.field8 >> d.field9;
	return stream;
}
istream& operator>> (istream& stream,Header& h){ //Читання заголовка з потоку
	stream >> h.hash >> h.num >> h.pointer ;
	return stream;
}
Data& Data::operator= (Data& d){ //Присвоєння записів
	this->id = d.id;
	this->field1 = d.field1;
	this->field2 = d.field2;
	this->field3 = d.field3;
	this->field4 = d.field4;
	this->field5 = d.field5;
	this->field6 = d.field6;
	this->field7 = d.field7;
	this->field8 = d.field8;
	this->field9 = d.field9;
	return *this;
}
int DataBase::alloc(fstream& f){ //Виділення нового блоку
	Header h;
	Data d;
	int pos0 = f.tellp();
	f.seekp(0,ios::end);
	int pos = f.tellp();
	f << h; //Пустий заголовок
	for (int i = 0; i < 7; i++){
		f << d; //Пусті записи
	}
	f.seekp(pos0,ios::beg);
	cout <<"New block was added.\n";
	return pos;
}
int DataBase::creat(){ //Створення бази даних
	ofstream to(filename.c_str());
	for (int i = 0; i < 7; i++){ //7 пустих блоків
		Header h(i+1);
		to << h;
		for (int j = 0; j < 7; j++){
			Data d;
			to << d;
		}
	}
	return 0;
}
int DataBase::insert(Data& d ){ //Вставка запису в БД
	fstream from(filename.c_str()); //Відкриваємо файл
	int a = ::hash(d.Id()); //Обчислюємо хеш
	bool found = false,exist = false;
	from.seekp((a-1)*BlockSize); //Переходимо 
	int pos = from.tellp();
	Header h;
	Data d0;
	while(!found){ // Шукаємо потрібний блок
		from >> h;
		if(h.Num() == 7) {
			if(h.Pointer() == 0){
				unsigned int pos0 = from.tellg();
				for (int i = 0; i < 7; i++){ //Перевірка чи нема таких записів
					from >> d0;
					if(d0.Id() == d.Id()){
						cout << "Insertion error in " << d.Id() << endl;
						from.close();
						return 2;
					}
				}
				from.seekg(pos0,ios::beg);
				h.Pointer(alloc(from)); //Виділяємо блок
				from.seekp(-(HeaderSize-1),ios::cur);
				from << h;
				from.seekp(h.Pointer()); //Перехід в новий блок
				pos = from.tellp();
			}
			else{
				for (int i = 0; i < 7; i++){ //Перевірка чи нема таких записів
					from >> d0;
					if(d0.Id() == d.Id()){
						cout << "Insertion error in " << d.Id() << endl;
						from.close();
						return 2;
					}
				}
				from.seekp(h.Pointer(),ios::beg); //Перехід в блок переповнення
				pos = from.tellp();
			}
		}
		else{
			found = true;
		}
	}
	for (int i = 0; i < 7; i++){ //Пошук в блоці
		from >> d0;
		if(d0.Id() == d.Id()){ //Перевірка чи нема таких записів
			cout << "Insertion error in " << d.Id() << endl;
			exist = true;
			break;
		}
		if (!d0.Id()){ //Вставка
			from.seekp(-(RecordSize-1),ios::cur);
			from << d;
			from.seekp(pos,ios::beg);
			from >> h;
			h.Num(h.Num()+1);
			from.seekp(-(HeaderSize-1),ios::cur);
			from << h;
			break;
		}
	}
	from.close(); //Закриваємо файл БД
	if(exist) return 2;
	cout << "Insertion success.\n";
	return 0;
}
int DataBase::garbageCollector(string filename){ //Видалення пустих блоків
	fstream from(filename.c_str()); //Відкриваємо файл
	Header h;
	Data d;
	unsigned int pos = 0;
	char * a;
	from.seekg(BlockSize*7); //Переходимо до блоків переповнення
	while(true){ //Шукаємо пусті
		from >> h;
		if(from.eof()){
			from.close();
			from.open(filename.c_str());
			break;
		}
		if(!h.Num()) pos = (unsigned int )from.tellg() - (HeaderSize-1);
		from.seekg(RecordSize*7,ios::cur);
	}
	if(from.fail()) return -1;
	from.seekg(-BlockSize,ios::end);
	unsigned b = from.tellg();
	if(!( b == pos ) && pos){ //Якщо останній блок порожній
		a =  new char[BlockSize];
		from.read(a,BlockSize);
		from.seekp(pos,ios::beg);
		from.write(a,BlockSize);
	}
	if(pos){ //Якщо є пусті блоки
		ofstream to("tmp.base",ios::app);
		if (to.fail()) {
			return 0;
		}
		char *c = new char[b];
		from.seekg(0,ios::beg);
		from.read(c,b); //Перезаписуємо файл
		to.write(c,b);
		to.close();
		delete(c);
		fstream to0("tmp.base");
		//for (int i = 0; i < 7; ++i){
		while(true){ //Змінюємо вказівники
			to0 >> h;
			if(to0.eof()) break;
			if(h.Pointer() == b){
				if(b == pos){
					h.Pointer(0);
				}
				else{
					h.Pointer(pos);
				}
				to0.seekp(-(HeaderSize-1),ios::cur);
				to0 << h;
				break;
			}
			to0.seekg(RecordSize*7,ios::cur);
		}
		to0.close(); //Закриваємо файли
		from.close();
		if(remove(filename.c_str()) || rename("tmp.base",filename.c_str())) return -1;
	}
	else{
		from.close();
	}
	return 0 ;
}
int DataBase::del(int id ){ //Видалення запису
	fstream from(filename.c_str()); //Відкриваємо файл
	int a = ::hash(id); //Обчислюємо хеш
	bool found = false,done = false;
	from.seekg((a-1)*BlockSize);
	unsigned int /*pos = from.tellg(),*/rempos = 0,lastpos = 0,headpos = 0;
	Data d,dp;
	Header h;
	while(true){ //Шукаємо запис
		headpos = from.tellg();
		from >> h;
		if(!h.Num()) break;
		for (int i = 0; i < 7; i++){ //Шукаємо в блоці
			from >> d;
			if(d.Id() == id){
				rempos = from.tellg() -(streamoff) (RecordSize-1);
				found = true;
			}
			if(d.Id() == 0 && found){
				from.seekp(rempos,ios::beg);
				from << dp;
				from.seekp(lastpos,ios::beg);
				from << d;
				h.Num(h.Num() - 1);
				from.seekp(headpos,ios::beg);
				from << h;
				done = true;
				break;
			}
			lastpos = from.tellg() - (streamoff)(RecordSize-1);
			dp = d;
		}
		if(done) break;
		if(h.Pointer()){ //Не знайшли? Переходимо в блок переповнення
			from.seekg(h.Pointer(),ios::beg);
		}
		else{
			if(lastpos == rempos){ //Якщо видаляємо останній запис
				from.seekp(rempos,ios::beg);
				from << *(new Data());
				h.Num(h.Num() - 1);
				from.seekp(headpos,ios::beg);
				from << h;
				done = true;
				break;
			}
			else{ //Або ще якийсь коли блок повний
				from.seekp(lastpos,ios::beg);
				from << *(new Data());
				from.seekp(rempos,ios::beg);
				from << dp;
				h.Num(h.Num() - 1);
				from.seekp(headpos,ios::beg);
				from << h;
				done = true;
				break;
			}
		}
	}
	from.close(); //Закриваємо файл
	if(done)garbageCollector(filename); //Можливо є пусті блоки 
	if(!found){
		cout << "Deletion error in " << id << endl;
	}
	else{
		cout << "Deletion success.\n";
	}
	return 0;
}
int DataBase::modify(Data& d){ //Модифікація запису
	fstream from(filename.c_str()); //Відкриваємо файл
	int a = ::hash(d.Id()); //Обчислюємо хеш
	bool found = false;
	from.seekp((a-1)*BlockSize);
	// int pos = from.tellp();
	Data d0;
	Header h;
	while(!found){ //Шукаємо запис
		from >> h;
		for (int i = 0; i < 7; i++){ //В конкретному блоці
			from >> d0;
			if(!d0.Id()) break;
			if(d0.Id() == d.Id()){
				from.seekp(-(RecordSize-1),ios::cur);
				from << d; //Замінюємо
				found = true;
				break;
			}
		}
		if(h.Pointer()!=0){ //Якщо запис в блоці переповнення
			from.seekp(h.Pointer());
			continue;
		}
		if(!found && h.Pointer() == 0){
			cout << "Modifying error in " << d.Id() << endl;
			from.close();
			return 2;
		}
	}
	from.close();
	cout << "Modifying success.\n";
	return 0;
}
int DataBase::find(int id = 0/*,int id0 = 0*/){ //Пошук за ID
	ifstream from(filename.c_str()); //Відкриваємо файл
	Header h;
	Data d;
	bool found = false;
	cout << "\n   ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
	from.seekg((::hash(id)-1)*BlockSize); //Перехід в потрібний блок
	while(!found){
		from >>  h;
		for (int j = 0; j < 7; j++){ //Шукаємо в блоці
			from >> d;
			if(d.Id() == id){
				cout << d; //Виводимо
				found = true;
				break;
			}
		}
		if(h.Pointer()!=0){ //Йдемо в блок переповнення
			from.seekg(h.Pointer());
			continue;
		}
		if(!found && h.Pointer() == 0){
			cout << "Record not found " << id << endl;
			break;
		}
	}
	return 0;
}
int DataBase::search() { //Пошук по полях
	ifstream from(filename.c_str()); //Відкриваємо файл
	Header h;
	Data d;
	string field;
	string value;
	cout << "Enter field to search. 1-Name,2-Flamsteed designation,3-Stellar class\n> ";
	cin >> field; //Зчитуємо номер поля
	if(atoi(field.c_str()) < 1 || atoi(field.c_str()) > 3){
		cout << "Wrong number.\n";
		return -1;
	}
	cout << "Enter value.\n> ";
	cin >> value; //Зчитуємо значення
	bool founded = false,fin = false;
	cout << "\n   ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
	for (int i = 0; i < 7; i++){ //Пошук в блоці
		//Header h(i+1);
		fin = false;
		from.seekg(i*BlockSize,ios::beg);
		from >>  h;
		if(!h.Num()) continue;
		while(!fin){ //Йдемо до кінця блоків переповнення
			for (int j = 0; j < 7; j++){
				from >> d;
				switch (atoi(field.c_str())){ //Пошук по різних полях
					case 1 :{
						if(d.Name().find(value) != string::npos){
							cout << d;
							founded = true;
						}
						break;
					}
					case 2 :{
						if(atoi(d.Flam_des().c_str()) == atoi(value.c_str())){
							cout << d;
							founded = true;
						}
						break;
					}
					case 3 :{
						if(d.S_class().find(value) != string::npos){
							cout << d;
							founded = true;
						}
						break;
					}
					default:
					{
						cout << "Wrong number.\n";
						break;
					}
				}
			}
			if(h.Pointer()){ //В блок переповнення
				from.seekg(h.Pointer(),ios::beg);
			}
			else{
				fin = true;
			}
			from >>  h;
                        if(from.eof())from.clear();
		}
		
	}
	if(!founded){
			cout << "Record not found " << endl;
		}
	return 0;
}
int DataBase::show(){ //Друк бази даних
	ifstream from(filename.c_str());
	bool fin = false;
	Header h;
	Data d;
	cout << "\n   ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
	for (int i = 0; i < 7; i++){ //Йдемо по блоках
		//Header h(i+1);
		fin = false;
		from.seekg(i*BlockSize,ios::beg); //В наступний блок
		from >>  h;
		if(h.Num() && h.Hash())	cout << h;
		if(!h.Num()) continue;
		while(!fin){ //Йдемо до кінця блоків переповнення
			for (int j = 0; j < 7; j++){
				from >> d;
				if(d.Id()){
					cout << d;
				}
			}
			if(h.Pointer()){ //Переходимо в блок переповнення
				cout << '\n';
				from.seekg(h.Pointer(),ios::beg);
			}
			else{
				fin = true;
				continue;
			}
			from >>  h;
		}
	}
	from.close();
	return 0;
}
Data& inputRecord(){ //Введення запису
	Data *d;
	string id;
	string field1;
	string field2;
	string field3;
	string field4;
	string field5;
	string field6;
	string field7;
	string field8;
	string field9;
	cout << "Enter id\n> ";
	cin >> id;
	int id0 = atoi(id.c_str()); 
	if(id0 <= 0 || id0 >=100000) { //Якщо ID невірний
		cout << "Wrong id.\n";
		d = new Data();
		return *d;
	}
	cout << "Enter Name\n> ";
	cin >> field1;
	cout << "Enter Letter in constellar\n> ";
	cin >> field2;
	cout << "Enter Flamsteed designation\n> ";
	cin >> field3;
	cout << "Enter Right_ascension\n> ";
	cin >> field4;
	cout << "Enter Declination\n> ";
	cin >> field5;
	cout << "Enter Apparent_magnitude\n> ";
	cin >> field6;
	cout << "Enter Absolute_magnitude\n> ";
	cin >> field7;
	cout << "Enter Stellar_distance\n> ";
	cin >> field8;
	cout << "Enter Stellar_class\n> ";
	cin >> field9;
	d = new Data(id0,field1,field2,field3,field4,field5,field6,field7,field8,field9);
	return *d;
}
bool yesno(const char s[] ){ //Прийняття рішень
	char c;
	cout << "Do you wanna to " << s << " y/N\n?> ";
	cin >> c;
	if(c == 'Y' || c == 'y') return true;
	else return false;
}
int command(){ //Робота з базою даних
	DataBase a;
	char com = 0;
	string id;
	bool comm = true;
	while(comm){
		id = "";
		cout << "> ";
		cin >> com;
		switch(com){
			case 'c': { //Створення
				cout << "Input name\n> ";
				string name ;
				cin >> name ;
				a.Filename(name);
				a.creat();
				cout << "DataBase created.\n";
				break;
			}
			case 'o':{ //Відкриття
				cout << "Input name\n> ";
				string name ;
				cin >> name ;
				a.Filename(name);
				cout << "DataBase opened.\n";
				break;
			}
			case 'i':{ //Вставка
				Data data = inputRecord();
				if(!data.Id()){
					break;
				}
				if(a.insert(data) == 2 && yesno("modify")){
					a.modify(data);
				}
				break;
			}
			case 'd':{ //Видалення
				cout << "Enter id to delete\n> ";
				cin >> id;
				int id0 = atoi(id.c_str());
				if(id0 <= 0 || id0 >= 70000){
					cout << "Wrong id.\n";
					break;
				}
				a.del(id0);
				break;
			}
			case 'm':{ //Модифікація
				Data data = inputRecord();
				if(!data.Id()){
					break;
				}
				if(a.modify(data) == 2 && yesno("insert")){
					a.insert(data);
				}
				break;
			}
			case 'f':{ //По ID
				cout << "Enter id to find\n> ";
				cin >> id;
				int id0 = atoi(id.c_str());
				if(id0 <= 0 || id0 >= 70000){
					cout << "Wrong id.\n";
					break;
				}
				a.find(id0);
				break;
			}
			case 'p': { //Показати
				a.show();
				break;
			}
			case 's':{ //Пошук по полях
				a.search();
				break;
			}
			case 'h':{ //Допомога
				cout << "c - create database\n"
					<< "o - open database\n"
					<< "i - insert record\n"
					<< "d - delete record\n"
					<< "m - modify record\n"
					<< "f - find record by ID\n"
					<< "s - search by fields.\n"
					<< "p - print database\n"
					<< "e/q - exit/quit\n";
				break;
			}
			case 'e':{ //Вихід
				cout << "Exit.\n";
				exit(EXIT_SUCCESS);
			}
			case 'q':{ //Вихід
				cout << "Quit.\n";
				comm = false;
				break;
				//exit(EXIT_SUCCESS);
			}
			default:{
				cout << "Wrong command.\n";
				break;
			}
		}
	}
	return 0;
}
int main(/*int argc, char const *argv[]*/){
	cout << "Direct access database. 2012.\n"
		<< "h for help.\n";
	command();
	return 0;
}
