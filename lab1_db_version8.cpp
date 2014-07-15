#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std; //Юзаємо простір імен std
bool yesno(const char[]);
class Data{ //Клас запису
	private:
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
	public:
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
	public:
		DataBase(const char name[]){
			filename = string(name);
			ofstream a(name,ios::app);
			a.close();
		};
		string Filename(){return this->filename;}
		int insert(Data&);
		int del(int);
		int find(int,int);
		int search();
		int modify(Data&);
		int show();
};
class GroupProcessList{ //Клас списку групової обробки
	string filename;
	public:
	GroupProcessList(const char name[]){
		filename = string(name);
		ofstream a(name,ios::app);
		a.close();
	};
	void getName(){ cin >> this->filename; }
	int insert(Data&);
	int modify(Data&);
	int del(int);
	int find(int,int);
	int show();
	int apply(DataBase&);
};
ostream& operator<< (ostream& stream,Data& d){ //Запис запису в потік
	stream.width(3);
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
	stream << d.field9;
	if(d.operation !=0) stream << ' ' << d.operation;
	stream << '\n';
	return stream;
}
istream& operator>> (istream& stream,Data& d){ //Читання запису з потоку
	stream >> d.id >> d.field1 >> d.field2 >> d.field3 >> d.field4 >> d.field5 >> d.field6 >> d.field7 >> d.field8 >> d.field9;
	return stream;
}
Data& Data::operator= (Data& d){ //Присвоєння записів
	this->id = d.id;
	this->field1 = d.field1;
	this->field2 = d.field2;
	this->field1 = d.field3;
	this->field2 = d.field4;
	this->field1 = d.field5;
	this->field2 = d.field6;
	this->field1 = d.field7;
	this->field2 = d.field8;
	this->field1 = d.field9;
	return d;
}
int GroupProcessList::insert(Data& d){ //Вставка в список групової обробки
	ifstream from(filename.c_str());
	ofstream to("tmp.gpl",ios::trunc);
	Data tmp;
	bool written = false ;
	while(true){
		from >> tmp;
		from >> tmp.operation;
		if(from.fail()) {
			if(!written){
				to << d;
				written = true;
			}
			break;
		}
		if (!written && tmp.Id() > d.Id()){
			to << d;
			written = true;
		}
		to << tmp;
	}
	from.close();
	to.close();
	if(remove(filename.c_str()) || rename("tmp.gpl",filename.c_str())) return -1;
	if(written) cout << "Insertion success.\n";
	return 0 ;
}
int GroupProcessList::del(int id){ //Видалення зі списку групової обробки
	ifstream from(filename.c_str());
	ofstream to("tmp.gpl");
	Data tmp;
	bool deleted = false ;
	bool founded = false ;
	while(true){
		from >> tmp;
		from >> tmp.operation;
		if(from.eof()) break;
		if(tmp.Id() == id) {
			founded = true;
			if(yesno("delete this record?")){
				deleted = true;
				continue;
			}
		}
		to << tmp;
	}
	from.close();
	to.close();
	if(remove(filename.c_str()) || rename("tmp.gpl",filename.c_str())) return -1;
	if (!founded){
		cout << "No such record.\n";
		return 2;
	}
	if(deleted){
		cout << "Deletion success.\n";
	}
	return 0 ;
}
int GroupProcessList::modify(Data& d){ //Модифікація запису в списку групової обробки
	ifstream from(filename.c_str());
	ofstream to("tmp.gpl");
	Data tmp;
	bool modified = false ;
	bool founded = false ;
	while(true){
		from >> tmp;
		from >> tmp.operation;
		if(from.eof()) break;
		if(!modified && tmp.Id() == d.Id()){
			founded = true;
			cout << tmp;
			if(yesno("modify exactly this record?")){
				to << d;
				modified = true;
				continue;
			}
		}
		to << tmp;
	}
	from.close();
	to.close();
	if(remove(filename.c_str()) || rename("tmp.gpl",filename.c_str())) return -1;
	if(!founded){
		cout << "No such record.\n";
		return 2;
	}
	if(modified){
		cout << "Modifiying success.\n";
	}
	return 0 ;
}
int GroupProcessList::find(int id,int id0 = 0){ //Пошук запису в списку групової обробки
	ifstream from(filename.c_str());
	Data tmp;
	bool founded = false;
	if(!id0) id0 = id;
	while(true){
		from >> tmp;
		from >> tmp.operation;
		if(from.eof()) break;
		if(tmp.Id() >= id && tmp.Id() <= id0) {
			cout << tmp;
			founded = true;
		}
	}
	from.close();
	if(!founded){
		cout << "Record not found.\n";
		return 2;
	}
	return 0;
}
int GroupProcessList::show(){  //Показати файл групової обробки
	ifstream from(filename.c_str());
	Data tmp;
	bool exist = false;
	cout << "\n ID       Name  L Flam_des       Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class Operation\n";
	while(true){
		from >> tmp;
		from >> tmp.operation;
		if(from.fail()) break;
		cout << tmp;
		exist = true;
	}
	from.close();
	if (!exist){
		cout << "Empty.\n" ;
		return 2;
	}
	return 0;
}
inline int compare(int a,int b){ //Логіка обробки послідовних записів
	 	if(a == 1 && b == 1) return 1;
        if(a == 1 && b == 2) return 2;
        if(a == 1 && b == 3) return 1;
        if(a == 2 && b == 1) return 3;
        if(a == 2 && b == 2) return 2;
        if(a == 2 && b == 3) return 3;
        if(a == 3 && b == 1) return 3;
        if(a == 3 && b == 2) return 2;
        if(a == 3 && b == 3) return 3;
	return 0;
}
int GroupProcessList::apply(DataBase& d){ //Застосування списку групової обробки
	ifstream from(d.Filename().c_str());
	ifstream from0(filename.c_str());
	ofstream to("tmp.base");
	Data tmp;
	Data tmp0p; //Тимчасові записи
	Data tmp0;
	int op = 0;
	int count = 0;
	bool got = false;
	cout << "Start.\n";
	from0 >> tmp0p >> tmp0p.operation;
	count ++;
	op = tmp0p.operation;
	tmp0p.operation = 0;
	while(true){ //Вічно
		if(tmp0p.Id() >= tmp.Id() || from0.eof())
		from >> tmp; //Зчитування з бази даних
		if (from.eof() && from0.eof() && !count){
			break;
		}
		while(!from0.eof() && !got){ //Блок обробки списку групової обробки
			from0 >> tmp0 >> tmp0.operation;
			if(from0.eof()){
				break;
			}
			got = true;
			count ++;
			if(tmp0.Id() == tmp0p.Id()){
				op = compare(op,tmp0.operation);
				if(op == -1) cout << "Error.\n";
				tmp0p = tmp0;
				got = false;
				count --;
				continue;
			}
			if(tmp0.Id() > tmp0p.Id()){
				break;
			}
		}
		if(tmp.Id() > tmp0p.Id()){ //В випадку вставки
			if(op == -1){
				cout << "Error.\n";
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					op = 0;
				}
			}else
			if(op == 1){
				to << tmp0p;
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					op = 0;
				}
			}else
			if(op == 2){
				cout << "Error with deletion.\n";
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					op = 0;
				}
			}else
			if(op == 3){
			cout << "Error with modifiying.\n";
			count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					op = 0;
				}
			}
		}
		else if(tmp.Id() == tmp0p.Id()){ //В випадку видалення або модифікації
			if(op == -1){
				cout << "Error.\n";
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else{
					op = 0;
				}
			}else
			if(op == 0){
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else{
					op = 0;
				}
			}else
			if(op == 1){
				cout << "Error with insertion.\n";
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					op = 0;
				}
			}else
			if(op == 2){
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					op = 0;
				}
				continue;
			}else
			if(op == 3){
				to << tmp0p;
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else{
					op = 0;
				}
				continue;
			}
		}
		else if(tmp.Id() < tmp0p.Id() && from.eof()){ //Якщо база скінчилася
			if(op == -1 ){
				cout << "Error.\n";
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					op = 0;
				}
			}else
			if(op == 0){
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
			}else
			if(op == 1){
				to << tmp0p;
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
			}else
			if(op == 2){
				cout << "Error with deletion.\n";
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					break;
				}
			}else
			if(op == 3){
				cout << "Error with modifiying.\n";
				count --;
				if(got){
					tmp0p = tmp0;
					got = false;
					op = tmp0.operation;
				}
				else {
					break;
				}
			}
		}
                if((tmp0p.Id() >= tmp.Id() || from0.eof()) && !from.eof())
		to << tmp; //Запис назад в базу
	}
	from.close();
	from0.close();
	to.close();
	if(remove(d.Filename().c_str()) || rename("tmp.base",d.Filename().c_str())) return -1; //Ну тут все ясно
	return 0 ;
}
int DataBase::insert(Data& d){ //Вставка запису в базу
	ifstream from(filename.c_str());
	ofstream to("tmp.base",ios::trunc);
	Data tmp;
	bool written = false ;
	while(true){
		from >> tmp;
		if(from.fail()) {
			if(!written){
				to << d;
				written = true;
			}
			break;
		}
		if (!written && tmp.Id() >= d.Id()){
			if(tmp.Id() == d.Id()){
				cout << "Record already exist.\n";
				return 2;
			}
			to << d;
			written = true;
		}
		to << tmp;
	}
	from.close();
	to.close();
	if(remove(filename.c_str()) || rename("tmp.base",filename.c_str())) return -1;
	if(written) cout << "Insertion success.\n";
	return 0 ;
}
int DataBase::del(int id){ //Видалення запису з бази
	ifstream from(filename.c_str());
	ofstream to("tmp.base");
	Data tmp;
	bool deleted = false ;
	while(true){
		from >> tmp;
		if(from.eof()) break;
		if(!deleted && tmp.Id() == id) {
			deleted = true;
			continue;
		}
		to << tmp;
	}
	from.close();
	to.close();
	if(remove(filename.c_str()) || rename("tmp.base",filename.c_str())) return -1;
	if (!deleted){
		cout << "No such record.\n";
		return 2;
	}
	else {
		cout << "Deletion success.\n";
	}
	return 0 ;
}
int DataBase::modify(Data& d){ //Модифікація запису в базі
	ifstream from(filename.c_str());
	ofstream to("tmp.base");
	Data tmp;
	bool modified = false ;
	while(true){
		from >> tmp;
		if(from.eof()) break;
		if(!modified && tmp.Id() == d.Id()){
			to << d;
			modified = true;
			continue;
		}
		to << tmp;
	}
	from.close();
	to.close();
	if(remove(filename.c_str()) || rename("tmp.base",filename.c_str())) return -1;
	if(!modified){
		cout << "No such record.\n";
		return 2;
	}
	else {
		cout << "Modifiying success.\n";
	}
	return 0 ;
}
int DataBase::find(int id,int id0 = 0){ //Пошук в базі
	//TODO: Пошук по полях
	ifstream from(filename.c_str());
	Data tmp;
	bool founded = false;
	if(!id0) id0 = id;
	while(true){
		from >> tmp;
		if(from.eof()) break;
		if(tmp.Id() >= id && tmp.Id() <= id0) {
			cout << tmp;
			founded = true;
			if(id0 - id == 1) break;
		}
	}
	from.close();
	if(!founded){
		cout << "Record not found.\n";
		return 2;
	}
	return 0;
}
int DataBase::search(){ //Пошук в базі
	//TODO: Пошук по полях
	ifstream from(filename.c_str());
	Data tmp;
	string field;
	string value;
	cout << "Enter field to search. 1-Name,2-Flamsteed designation,3-Stellar class\n> ";
	cin >> field;
	if(atoi(field.c_str()) < 1 || atoi(field.c_str()) > 3){
		cout << "Wrong number.\n";
		return -1;
	}
	cout << "Enter value.\n> ";
	cin >> value;
	bool founded = false;
	//if(!id0) id0 = id;
	cout << "\n ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
	while(true){
		from >> tmp;
		if(from.eof()) break;
		switch (atoi(field.c_str())){
			case 1 :{
				if(tmp.Name().find(value) != string::npos){
					cout << tmp;
					founded = true;
				}
				break;
			}
			case 2 :{
				if(atoi(tmp.Flam_des().c_str()) == atoi(value.c_str())){
					cout << tmp;
					founded = true;
				}
				break;
			}
			case 3 :{
				if(tmp.S_class().find(value) != string::npos){
					cout << tmp;
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
	from.close();
	if(!founded){
		cout << "Record not found.\n";
		return 2;
	}
	return 0;
}
int DataBase::show(){ //Показати базу
	ifstream from(filename.c_str());
	Data tmp;
	bool exist = false;
	bool fine = true;
	cout << "\n ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
	while(true){
		from >> tmp;
		if(from.eof()) break;
		if(from.fail()){
			fine = false;
			break;
		}
		cout << tmp;
		exist = true;
	}
	from.close();
	if (!exist){
		cout << "Empty.\n" ;
		return 2;
	}
	if (!fine){
		cout << "Error.\n" ;
		return -1;
	}
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
	if(id0 <= 0) {
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
int gcommand(DataBase *a){ //Робота з списком групової обробки
	string name;
	cout << "Input name\n> ";
	cin >>  name;
	Data data;
	char com = 0;
	int id = 0;
	if(!a){
		cout << "No db opened.\n";
		return 2;
	}
	if(a->Filename() == name){
		cout << "Wrong name.\n";
		return 2;
	}
	GroupProcessList b(name.c_str());
	while(true){
		cout << ">> ";
		cin >> com;
		if(cin.fail()){
			cout << "Wrong command.\n";
			string kostil;
			cin >> kostil;
			continue;
		}
		switch(com){
			case 'c':{ //Створення відкриття
				cout << "Input name.\n>";
				b.getName();
				break;
			}
			case 'i':{ //Вставка
				
				cout << "Enter operation 1-insert,2-delete,3-modify\n> ";
				string in;
				cin >> in;
				int in0 = atoi(in.c_str());
				if(in0 < 1 || in0 > 3 ){
					cout << "Wrong operation.\n";
					break;
				}
				if(cin.fail()){
					cout << "Wrong operation.\n";
					break;
				}
				if(in0 == 2){
					cout << "Enter id.\n> ";
					cin >> in;
					int in0 = atoi(in.c_str());
					data = *(new Data(in0));
				}
				else {
				data = inputRecord();
				if(!data.Id()){
					break;
				}
				}
				data.operation = in0;
				b.insert(data);
				break;
			}
			case 'd':{ //Видалення
				cout << "Enter id to delete\n> ";
				cin >> id;
				if(cin.fail()){
					cout << "Wrong id.\n";
					break;
				}
				b.del(id);
				break;
			}
			case 'm':{ //Модифікація
				Data data = inputRecord();
				if(!data.Id()){
					break;
				}
				cout << "Enter operation 1-insert,2-delete,3-modify\n> ";
				cin >> data.operation;
				if(cin.fail()){
					cout << "Wrong operation.\n";
					break;
				}
				b.modify(data);
				break;
			}
			case 'f':{ //По ID
				cout << "Enter id to find\n> ";
				cin >> id;
				if(cin.fail()){
					cout << "Wrong id.\n";
					break;
				}
				b.find(id);
				break;
			}
			case 'r':{ //По ID в діапазоні
				cout << "Enter first id to find\n> ";
				cin >> id;
				int id0;
				cout << "Enter second id to find\n> ";
				cin >> id0;
				b.find(id,id0);
				break;
			}
			case 'p':{ //Друк
				b.show();
				break;
			}
			case 'h':{ //Допомога
				cout << "c - create/open group process list.\n"
				 << "i - insert record\n"
				 << "d - delete record\n"
				 << "m - modify record\n"
				 << "f - find record\n"
				 << "r - find record in range\n"
				 << "p - print group process list\n"
				 << "a - apply group process list\n"
				 << "e,q - return to database\n";
				 break;
			}
			case 'a':{ //Застосувати
				b.apply(*a);
				break;
			}
			case 'e':{ //Вихід
				cout << "Exit from gpl mode.\n";
				return 0;
			}
			case 'q':{ //Вихід
				cout << "Exit from gpl mode.\n";
				return 0;
			}
			default:{
				cout << "Wrong command.\n";
				break;
			}
		}
	}
	return 0;
}
int command(){ //Робота з базою даних
	DataBase *a = NULL;
	char com = 0;
	string id;
	while(true){
		id = "";
		cout << "> ";
		cin >> com;
		switch(com){
			case 'c': { //Створення відкриття
				cout << "Input name\n> ";
				string name ;
				cin >> name ;
				a = new DataBase(name.c_str());
				cout << "DataBase created/opened.\n";
				break;
			}
			case 'i':{ //Вставка
				Data data = inputRecord();
				if(!data.Id()){
					break;
				}
				if(a){
					if(a->insert(data) == 2 && yesno("modify")){
						a->modify(data);
					}
				}
				else
					cout << "No db opened.\n";
					break;
			}
			case 'd':{ //Видалення
				if(a){
				cout << "Enter id to delete\n> ";
				cin >> id;
				int id0 = atoi(id.c_str());
				if(id0 == 0){
					cout << "Wrong id.\n";
					break;
				}
				a->del(id0);
				}
				else
					cout << "No db opened.\n";
				break;
			}
			case 'm':{ //Модифікація
				Data data = inputRecord();
				if(!data.Id()){
					break;
				}
				if(a){
					if(a->modify(data) == 2 && yesno("insert")){
						a->insert(data);
					}
				}
				else
					cout << "No db opened.\n";
					break;
			}
			case 'f':{ //По ID
				cout << "Enter id to find\n> ";
				cin >> id;
				int id0 = atoi(id.c_str());
				if(id0 == 0){
					cout << "Wrong id.\n";
					break;
				}
				if(a)
					a->find(id0);
				else
					cout << "No db opened.\n";
					break;
			}
			case 'r':{ //По ID в діапазоні
				cout << "Enter first id to find\n> ";
				cin >> id;
				string id1;
				int id0 = atoi(id.c_str());
				cout << "Enter second id to find\n> ";
				cin >> id1;
				int id2 = atoi(id1.c_str());
				if(id0 == 0 && id2 == 0){
					cout << "Wrong id.\n";
					break;
				}
				if(a)
					a->find(id0,id2);
				else
					cout << "No db opened.\n";
					break;
			}
			case 'p': { //Показати
				if(a){
					a->show();
				}
				else{
					cout << "No db opened.\n";
				}
				break;
			}
			case 's':{ //Пошук по полях
				if(a){
					a->search();
				}
				else{
					cout << "No db opened.\n";
				}
				break;
			}
			case 'g':{ //Групова обробка
				gcommand(a);
				break;
			}
			case 'h':{ //Допомога
				cout << "c - create/open database\n"
				 << "i - insert record\n"
				 << "d - delete record\n"
				 << "m - modify record\n"
				 << "f - find record by ID\n"
				 << "r - find record in range\n"
				 << "s - search by fields.\n"
				 << "p - print database\n"
				 << "g - group process list\n"
				 << "e,q - exit\n";
				 break;
			}
			case 'e':{ //Вихід
				cout << "Exit.\n";
				exit(EXIT_SUCCESS);
			}
			case 'q':{ //Вихід
				cout << "Exit.\n";
				exit(EXIT_SUCCESS);
			}
			default:{
				cout << "Wrong command.\n";
				break;
			}
		}
	}
	return 0;
}
int main(int argc, const char *argv[]){
	cout << "Sequental access database. 2012\n"
	<< "h for help.\n";
	command();
    return 0;
}
