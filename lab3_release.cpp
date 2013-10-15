#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std; //Юзаємо простір імен std
class Index {
    int max;
    unsigned int num;
    unsigned int pointer;
public:
    Index(int h = 0,unsigned int n = 0,unsigned int p = 0):max(h),num(n),pointer(p){} //Конструктор
    inline int Max()const{return this->max;}
    inline unsigned int Num()const{return this->num;}
    inline unsigned int Pointer()const{return this->pointer;}
    void Max(int h){ this->max = h;}
    void Num(unsigned int n){ this->num = n;}
    void Pointer(unsigned int p){ this->pointer = p;}
    friend ostream& operator<< (ostream&,Index&);
    friend istream& operator>> (istream&,Index&);
};
class Data { //Клас запису
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
    friend ostream& operator<< (ostream&,Data&);
    friend istream& operator>> (istream&,Data&);
    Data& operator= (Data&);
    inline int Id()const{return this->id;}
    //Конструктор
    string Name(){return this->field1;}
    string Flam_des(){return this->field3;}
    string S_class(){return this->field9;}
    Data(int a = 0,string b = "0",string c = "0",string d = "0",string e = "0",string f = "0",string g = "0",string h = "0",string i = "0",string j = "0")
        :id(a),field1(b),field2(c),field3(d),field4(e),field5(f),field6(g),field7(h),field8(i),field9(j){}
};
class DataBase{ //Клас бази даних
private:
    string filename;
    static const int IndexSize = 23;
    static const int RecordSize = 155;
    static const int BlockSize = RecordSize*10;
public:
    DataBase(const char name[] = ""):filename(name){ //Конструктор
        if(!filename.empty()){
            ofstream a(name,ios::app);
            if(!a.is_open()){
                cout << "File were not opened!"<< endl ;
            }
            a.close();
        }
    };
    string Filename(){return this->filename;}
    void Filename(string s){
        this->filename = s;
    }
    //Функції для роботи з базою даних
    int creat();
    int split(fstream&,fstream&,unsigned int);
    int insert(Data&);
    int del(int);
    int find(int);
    int search();
    int modify(Data&);
    int show();
    int alloc(fstream&);
    int garbageCollector(string);
};
ostream& operator<< (ostream& stream,Data& d){ //Запис запису в потік
    stream.width(5);
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
    stream << d.field9 << endl;
    return stream;
}
ostream& operator<< (ostream& stream,Index& h){ //Запис запису в потік
    stream.width(5);
    stream << h.max <<' ';
    stream.width(5);
    stream << h.num << ' ';
    stream.width(10);
    stream << h.pointer << endl;
    return stream;
}
istream& operator>> (istream& stream,Data& d){ //Читання запису з потоку
    stream >> d.id >> d.field1 >> d.field2 >> d.field3 >> d.field4 >> d.field5 >> d.field6 >> d.field7 >> d.field8 >> d.field9;
    return stream;
}
istream& operator>> (istream& stream,Index& h){ //Читання запису з потоку
    stream >> h.max >> h.num >> h.pointer ;
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
int DataBase::creat(){//Створення бази даних
    ofstream to(filename.c_str());
    ofstream to0((filename+".index").c_str());
    Index x;
    Data d;
    to0 << x;
    for (int j = 0; j < 10; j++){
        to << d;
    }
    to.close();
    to0.close();
    return 0;
}
int DataBase::split(fstream& from,fstream& from0,unsigned int p){//Функція поділу блоків
    int pos0 = static_cast<int>(from.tellp());
    char *a,*b;
    Data d,d0;
    Index x,x0;
    from.clear();
    from.seekp(0,ios::end);
    int PosEnd = static_cast<int>(from.tellp());
    PosEnd -=static_cast<int>(p);//Шукаємо що зсувати
    a = new char[PosEnd];
    from.seekp(p,ios::beg);
    from.read(a,PosEnd); //Читаємо
    from.seekp(p+BlockSize,ios::beg);
    from.write(a,PosEnd); //Пишемо
    from.seekp(p+BlockSize,ios::beg);
    b = a+5*RecordSize;
    from.write(b,RecordSize*5);
    for (int i = 0; i < 5; ++i){ //Занулюємо записи
        from << d;
    }
    from.seekp(p+RecordSize*5,ios::beg);
    for (int i = 0; i < 5; ++i){ //Занулюємо записи
        from << d;
    }
    from.seekg(p+RecordSize*4);
    from >> d;
    int newmax = d.Id(); //Новий максимальний елемент для блоку
    from0.seekp(0,ios::end);
    int indexend =static_cast<int>(from0.tellp());
    from0.seekg(0,ios::beg);
    while(true){ //Зсуваємо індекси
        from0 >> x;
        if(from0.eof())break;
        if(x.Pointer() == p){ //Якщо найшли потрібний
            int oldpos = static_cast<int>(from0.tellp());
            oldpos++;
            int count = indexend - oldpos;
            char *c = new char[count];
            from0.seekg(1,ios::cur);
            from0.read(c,count);
            from0.seekp(oldpos,ios::beg);
            x0.Max(x.Max()); //Будуємо запис індекса для нового блоку
            x0.Num(5);
            x0.Pointer(p + BlockSize);
            from0 << x0;
            from0.write(c,count);
            from0.seekp(oldpos - IndexSize,ios::beg);
            x.Max(newmax); //Змінюємо запис індексу для старого блоку
            x.Num(5);
            from0 << x;
            delete c;
            from.clear();
            from0.seekp(0,ios::beg);
            break;
        }
    }
    from.clear();
    bool first = false;
    while(true){ //Правимо вказівники в індексі
        from0 >> x;
        if(from0.eof())break;
        if(x.Pointer() > p ){ //Якщо більший нашого
            if(!first){ //Наш новий не враховуємо
                from0.seekp(-(IndexSize-1),ios::cur);
                from0 << x;
                int test = from0.tellp();
                from0.clear();
                first = true;
                continue;
            }
            x.Pointer(x.Pointer()+BlockSize);
            from0.seekp(-(IndexSize-1),ios::cur);
            from0 << x;
            int test = from0.tellp();
        }
        else{ //В іншому випадку
            from0.seekp(-(IndexSize-1),ios::cur);
            from0 << x;
            int test = from0.tellp();
            from0.clear();
        }
    }
    from.clear();
    from.seekp(pos0);
    delete a;
    //delete b;
    return 0;
}
int DataBase::insert(Data& d){ //Вставка
    fstream from(filename.c_str());
    fstream from0((filename+".index").c_str());
    Index x;
    Data d0,d1;
    bool done = false;
    while(!done){ //Головний цикл
        from0 >> x;
        if(!x.Max()){ //Якщо пустий блок
            from << d;
            x.Num(x.Num()+1);
            x.Max(d.Id());
            from0.seekp(-(IndexSize-1),ios::cur);
            from0 << x;
            done = true;
        }
        else if(x.Max() < d.Id() && x.Num() < 5){ //Якщо елемент більший максимального
            from.seekp(x.Pointer());
            from.seekp(RecordSize*x.Num(),ios::cur);
            from << d;
            x.Num(x.Num()+1);
            x.Max(d.Id());
            from0.seekp(-(IndexSize-1),ios::cur);
            from0 << x;
            done = true;
        }
        else if(x.Max() > d.Id() && x.Num() < 9){ //Якщо менший максимального
            from.seekg(x.Pointer());
            int id = d.Id();
            for(int i = 0; i < 10; i++){
                from >> d0;
                if(d0.Id() == d.Id()){ //Вже такий маємо
                    cout << "Such record already exists.\n";
                    from.close();
                    from0.close();
                    return 2;
                }
                if(d0.Id() > id){ //Не маємо
                    int oldpos = static_cast<int>(from.tellp());
                    int count = x.Pointer() + BlockSize - oldpos - RecordSize;
                    char *c = new char[count];
                    from.seekg(oldpos-RecordSize+1,ios::beg);
                    from.read(c,count);
                    from.seekp(oldpos-RecordSize+1,ios::beg);
                    from << d;
                    from.write(c,count);
                    from.seekg(0,ios::beg);
                    break;
                }
            }
            x.Num(x.Num()+1); //Змінюємо індекс
            from0.seekp(-(IndexSize-1),ios::cur);
            from0 << x;
            done = true;
        }
        else if(x.Max() < d.Id() && x.Num() >= 5){ //Потрібен новий блок
            if(!from0.eof()) continue;
            else{
                x.Pointer(alloc(from)); //Виділяємо блок
                x.Num(1);
                x.Max(d.Id());
                from0.clear();
                from0.seekp(0,ios::end);
                from0 << x; //Новий запис в індексі
                from.seekp(-BlockSize,ios::end);
                from << d;
                done = true;
            }
        }
        else if(x.Max() > d.Id() && x.Num() >= 9){ //Пора ділити блок
            from.seekg(x.Pointer());
            int id = d.Id();
            for(int i = 0; i < 10; i++){ //Просто вставляємо
                from >> d0;
                if(d0.Id() == d.Id()){
                    cout << "Such record already exists.\n";
                    from.close();
                    from0.close();
                    return 2;
                }
                if(d0.Id() > id){
                    int oldpos = static_cast<int>(from.tellp());
                    int count = x.Pointer() + BlockSize - oldpos ;//- RecordSize;
                    char *c = new char[count];
                    from.seekg(oldpos-RecordSize+1,ios::beg);
                    from.read(c,count);
                    from.seekp(oldpos-RecordSize+1,ios::beg);
                    from << d;
                    from.write(c,count);
                    from.seekg(0,ios::beg);
                    break;
                }
            }
            x.Num(x.Num()+1);
            from0.seekp(-(IndexSize-1),ios::cur);
            from0 << x;
            done = true;
            split(from,from0,x.Pointer()); //Ділимо блок
            from.close();
            from0.close();
            return 0;//insert(d);
        }
        else if(x.Max() == d.Id()){ //Помилка, запис існує
            cout << "Such record already exists.\n";
            from.close();
            from0.close();
            return 2;
        }
    }
    from.close();
    from0.close();
    if(done){
        cout << "Insertion success.\n";
    }
    return 0;
}
int DataBase::modify(Data& d){ //Модифікація запису
    fstream from(filename.c_str());
    fstream from0((filename+".index").c_str());
    Index x;
    Data d0;
    int max = 0;
    bool modifyed = false;
    while(true){ //Пошук в індексі
        from0 >> x;
        if(from0.eof()) break;
        if(x.Max() >= d.Id()){
            from.seekg(x.Pointer());
            break;
        }
    }
    for (int i = 0; i < 10; i++){ //Пошук в блоці
        max = d0.Id();
        from >> d0;
        if(from.eof()) break;
        if(!d0.Id())break;
        if(d0.Id() == d.Id()){
            from.seekp(-(RecordSize-1),ios::cur);
            from << d; //Заміна
            modifyed = true;
            break;
        }
    }

    from.close();
    from0.close();
    if(!modifyed){
        cout << "No such record.\n";
        return 2;
    }
    else{
        cout << "Modifying success.\n";
    }
    return 0;
}
int DataBase::garbageCollector(string filename){ //Видалення пустих блоків
    fstream from(filename.c_str());
    fstream from0((filename+".index").c_str());
    Index x;
    Data d;
    char *a/*,*b*/;
    int oldpos = 0 ;
    bool done = false;
    from.seekg(0,ios::end);
    int dbend = static_cast<int>(from.tellp());
    from0.seekg(0,ios::end);
    int indexend = static_cast<int>(from0.tellp());
    if(dbend == BlockSize || indexend == IndexSize){
	from.close();
	from0.close();
	return 0;
    }
    from.seekg(0,ios::beg);
    from0.seekg(0,ios::beg);
    while(true){ //Головний цикл
        from0 >> x;
        if(from0.eof())break;
        if(!x.Num()){ //Знайдено пустий блок
            int count = dbend - x.Pointer() - BlockSize;
            int count0 = indexend - (static_cast<int>(from0.tellp())+1);
            from.seekg(x.Pointer()+BlockSize);
            a = new char[count]; //Тимчасовий масив
            from.read(a,count);
            from.seekp(x.Pointer());
            from.write(a,count);
            delete a;
            a = new char[count0];
            from0.seekp(1,ios::cur);
            int test = from0.tellp();
            from0.read(a,count0);
            from0.clear();
            from0.seekp(indexend - count0 - IndexSize,ios::beg);
            test = from0.tellp();
            from0.write(a,count0);
            test = from0.tellp();
            delete a;
            ofstream to("tmp.base",ios::trunc),to0("tmp.index",ios::trunc);
            a = new char[dbend - BlockSize];
            from.seekg(0,ios::beg);
            from.read(a,dbend - BlockSize);
            to.write(a,dbend - BlockSize);
            delete a;
            a = new char[indexend - IndexSize];
            from0.seekg(0,ios::beg);
            test = from0.tellp();
            from0.read(a,indexend - IndexSize);
            to0.write(a,indexend - IndexSize);
            delete a;
            from.close();
            from0.close();
            if(remove(filename.c_str()) || rename("tmp.base",filename.c_str())) return -1;
            if(remove((filename+".index").c_str()) || rename("tmp.index",(filename+".index").c_str())) return -1;
            oldpos = indexend - count0 - IndexSize;
            done = true;
            break;
        }
    }
    if(done){
        from0.open((filename+".index").c_str());
        while(true){ //Правимо вказівники в індексі
            from0 >> x;
            if(from0.eof())break;
            if(x.Pointer() > static_cast<unsigned>((oldpos/23)*BlockSize) ){ //Якщо більший нашого
                x.Pointer(x.Pointer()-BlockSize);
                from0.seekp(-(IndexSize-1),ios::cur);
                from0 << x;
                int test = from0.tellp();
            }
            else{ //В іншому випадку
                from0.seekp(-(IndexSize-1),ios::cur);
                from0 << x;
                int test = from0.tellp();
                from0.clear();
            }
        }
    }
    return 0;
}
int DataBase::del(int id){ //Видалення запису
    fstream from(filename.c_str());
    fstream from0((filename+".index").c_str());
    Index x;
    Data d,d0;
    bool done = false;
    while(true){
        from0 >> x;
        if(from0.eof())break;
        if(x.Max() >= id){
            if(x.Max() == id){ //Видалення максимального з блоку
                from.seekp(x.Pointer() + (x.Num()-1)*RecordSize ,ios::beg);
                from << d;
                from.seekg(-(RecordSize*2),ios::cur);
                from >> d;
                x.Max(d.Id()); //Правка індекса
                x.Num(x.Num()-1);
                from0.seekp(-(IndexSize-1),ios::cur);
                from0 << x;
                done = true;
                break;
            }
            else{ //Видалення звичайного
                from.seekg(x.Pointer());
                if(from.eof())break;
                while(true){ 
                    from >> d;
                    if(from.eof())break;
                    if (d.Id() == id){ //Знайдено потрібний
                        int oldpos = static_cast<int>(from.tellp()) + 1;
                        from.seekp(x.Pointer()+BlockSize,ios::beg);
                        int count = static_cast<int>(from.tellp()) - oldpos;
                        from.seekg(oldpos,ios::beg);
                        char *a = new char[count];
                        from.read(a,count); //Зсув записів в блоці
                        from.seekp(oldpos - RecordSize,ios::beg);
                        from.write(a,count);
                        delete a;
                        done = true;
                        break;
                    }
                }
                if(done){
                    x.Num(x.Num()-1); //Правимо індекс
                    from0.seekp(-(IndexSize-1),ios::cur);
                    from0 << x;
                }
                else{
                    cout << "Record not found.\n";
                }
                break;
            }
        }
    }
    from.close();
    from0.close();
    garbageCollector(filename); //Видаляємо пусті блоки
    if(done){
        cout << "Deletion success.\n";
    }
    return 0;
}
int DataBase::alloc(fstream& f){ //Виділення нового блоку
    Data d;
    int pos0 = static_cast<int>(f.tellp());
    f.seekp(0,ios::end);
    int pos = static_cast<int>(f.tellp());
    for (int i = 0; i < 10; i++){
        f << d;
    }
    f.seekp(pos0,ios::beg);
    return pos;
}
int DataBase::find(int id){//Пошук запису
    ifstream from(filename.c_str());
    ifstream from0((filename+".index").c_str());
    Index x;
    Data d;
    bool found = false;
    cout << "\n   ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
    while(true){ //Пошук в індексі
        from0 >> x;
        if(from0.eof()) break;
        if(x.Max() >= id){
            from.seekg(x.Pointer());
            break;
        }
    }
    for (int i = 0; i < 10; i++){ //Пошук в блоці
        from >> d;
        if(from.eof()) break;
        if(!d.Id())break;
        if(d.Id() == id){
            cout << d;
            found = true;
            break;
        }
    }
    if(!found){
        cout << "Record not found.\n";
    }
    from.close();
    from0.close();
    return 0;
}
int DataBase::search(){ //Пошук по полях
    ifstream from(filename.c_str());
    ifstream from0((filename+".index").c_str());
    Index x;
    Data d;
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
    bool found = false;
    cout << "\n   ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
    while(true){//Йдемо по базі
        from0 >> x; //Читаємо індекс
        if(from0.eof()) break;
        from.seekg(x.Pointer()); //Переходимо в відповідний блок
        for (int i = 0; i < 10; i++){ //Пошук в блоці
            from >> d;
            if(from.eof()) break;
            if(!d.Id())break;
            switch (atoi(field.c_str())){
            case 1 :{
                if(d.Name().find(value) != string::npos){
                    cout << d;
                    found = true;
                }
                break;
            }
            case 2 :{
                if(atoi(d.Flam_des().c_str()) == atoi(value.c_str())){
                    cout << d;
                    found = true;
                }
                break;
            }
            case 3 :{
                if(d.S_class().find(value) != string::npos){
                    cout << d;
                    found = true;
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
    }
    if(!found){
        cout << "Record not found.\n";
    }
    return 0;
}
int DataBase::show(){ //Вивід на екран
    ifstream from(filename.c_str());
    Data d;
    int counter = 0;
    bool NotEmpty = false;
    cout << "\n   ID       Name L Flam_des      Right_ascension         Declination   Apparent_magnitude   Absolute_magnitude     Stellar_distance        Stellar_class\n";
    while(true){ //Йдемо по базі і виводимо не пусті записи
        from >> d;
        if(from.eof())break;
        if(counter%10 == 0) cout << "Block " << counter/10+1 << endl ;
        counter++;
        if(!d.Id())continue;
        if(d.Id()){
            NotEmpty = true;
            cout << d;
        }
    }
    if(!NotEmpty){
        cout << "Empty.\n";
    }
    from.close();
    return 0;
}
Data inputRecord(){ //Введення запису
    //Data *d;
    string id;
    string field1,field2,field3,field4,field5,field6,field7,field8,field9;
    cout << "Enter id\n> ";
    cin >> id;
    int id0 = atoi(id.c_str());
    if(id0 <= 0 || id0 >= 100000) {
        cout << "Wrong id.\n";
        Data d;
        return d;
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
    Data d(id0,field1,field2,field3,field4,field5,field6,field7,field8,field9);
    return d;
}
bool yesno(const char s[] ){ //Прийняття рішень
    char c;
    cout << "Do you wanna to " << s << " y/N\n?> ";
    cin >> c;
    //cin.read(&c,1);
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
        case 'c': { //Створення відкриття
            cout << "Input name\n> ";
            string name ;
            cin >> name ;
            a.Filename(name);
            a.creat();
            cout << "DataBase created.\n";
            break;
        }
        case 'o':{
            cout << "Input name\n> ";
            string name ;
            cin >> name ;
            ifstream test(name.c_str());
            if(!test.is_open()){
                cout << "No such file.\n";
                break;
            }
            a.Filename(name);
            cout << "DataBase opened.\n";
            break;
        }
        case 'i':{ //Вставка
            Data data = inputRecord();
            if(!data.Id()){
                break;
            }
            if(a.Filename().empty()){
                cout << "No database.\n";
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
            if(id0 <= 0 ){
                cout << "Wrong id.\n";
                break;
            }
            if(a.Filename().empty()){
                cout << "No database.\n";
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
            if(a.Filename().empty()){
                cout << "No database.\n";
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
            if(id0 == 0){
                cout << "Wrong id.\n";
                break;
            }
            if(a.Filename().empty()){
                cout << "No database.\n";
                break;
            }
            a.find(id0);
            break;
        }
        case 'p': { //Показати
            if(a.Filename().empty()){
                cout << "No database.\n";
                break;
            }
            a.show();
            break;
        }
        case 's':{ //Пошук по полях
            if(a.Filename().empty()){
                cout << "No database.\n";
                break;
            }
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
                 << "e,q - exit\n";
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
int main(/*int argc, char const *argv[]*/)
{
    cout << "Index-Sequentual database. Lizanets Danylo © 2012.\nh for help\n";
    command();
    return 0;
}
