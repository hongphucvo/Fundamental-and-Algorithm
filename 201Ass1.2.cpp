/* this version contain bus BusSystem and bussystem*/
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;


#include <sstream>
class BusSystem
{
public:
    class Bus;
    friend class Bus;
protected:
    Bus* head;
    int MaxBus;
    int count;
public:
    BusSystem(int N = -1) {
        this->MaxBus = N;
        this->count = 0;
        this->head = NULL;
    }
    ~BusSystem();
    Bus* Jumpto(string Code);
    string insertBus(Bus* added);
    int removeBus(string Code, int, int);
    string query(string instruction);
public:
    class Bus
    {
        friend class BusSystem;
    protected:
        string ID;
        string Code;
        int Case;
        int Start;
        int Finish;
        Bus* next;
        Bus* prev;

    public:
        Bus()
        {
            ID = "";
            Code = "";
            Case = -1;
            Start = 0;
            Finish = 0;
            next = 0;
            prev = 0;
        }
        Bus(string ID = "", string CODE = "", int Case = -1, int Start = 0, int Finish = 0, Bus* next = NULL, Bus* prev = NULL)
        {
            this->ID = ID;
            this->Code = CODE;
            this->Case = Case;
            this->Start = Start;
            this->Finish = Finish;
            this->next = next;
            this->prev = prev;
        }
    };
public:
    string SQ(int N);
    string INS(string CODE, string LP, int Case, int TimeA, int TimeB);
    string DEL(string CODE, int, int);
    int CS(string CODE, int Time, int Case);
    int CE(string CODE, int Time, int Case);
    string GS(string CODE, int Time, int Case);
    string GE(string CODE, int Time, int Case);
    string toStr(int);
    bool valid(string S) {
        if ((S[0] == ' ') || (S[S.length() - 1] == ' '))
            return false;
        for (unsigned i = 1; i < S.length() - 1; i++) {
            if (S[i] == ' ') {
                if (S[i - 1] == ' ')
                    return false;
            }
        }
        return true;
    }
};
BusSystem::~BusSystem() {
    Bus* temp = head;
    Bus* rmv;
    while (temp->next != NULL) {
        rmv = temp->next;
        delete temp;
        temp = rmv;
    }
    delete temp;
}
BusSystem::Bus* BusSystem::Jumpto(string Code) {
    Bus* tempNode = this->head;
    while ((tempNode->next != NULL) && (tempNode->Code < Code)) {
        tempNode = tempNode->next;
    }
    return tempNode;
}
string BusSystem::insertBus(Bus* added) {
    if (this->head == NULL) {
        this->head = added;
        return "1";//0 element
    }
    else {
        int a = 0;
        Bus* tempNode = Jumpto(added->Code);
        if (tempNode->Code > added->Code) {
            added->prev = tempNode->prev;
            added->next = tempNode;
            if (tempNode->prev != NULL) {
                tempNode->prev->next = added;
            }
            else this->head = added;
            tempNode->prev = added;
            return "1";
        }
        else if (tempNode->Code == added->Code) {
            //con add vo truoc head node
            while (tempNode->Code == added->Code) {
                a++;
                if (a == this->MaxBus)return "-1";
                if ((tempNode->Start == added->Start) && (tempNode->Case == added->Case))
                    return "-1";
                // cung chieu co duoc cung gio khong
                if (tempNode->ID == added->ID) {
                    if (tempNode->Finish > added->Start)
                        return "-1";
                }
                if (tempNode->next == NULL) {
                    //add sau NULL
                    added->prev = tempNode;
                    tempNode->next = added;
                    a++;
                    this->count++;
                    return toStr(a);
                }
                tempNode = tempNode->next;
            }  // A             B
                //       c
            added->next = tempNode;
            added->prev = tempNode->prev;
            tempNode->prev->next = added;
            tempNode->prev = added;
            a++;
            this->count++;
            return toStr(a);
        }
        return toStr(a + 1);
    }
}
int BusSystem::removeBus(string Code, int TimeA = -1, int TimeB = 1000000) {
    int value = 0;
    Bus* tempNode = Jumpto(Code);//nhay toi code thu >= code cu . jumto19 =19
    Bus* rmv;
    while (tempNode != NULL) {
        if (tempNode->Code == Code) {
            bool valid = ((TimeB == -1) && (tempNode->Start == TimeA)) || ((tempNode->Start > TimeA) && (tempNode->Finish < TimeB));
            rmv = tempNode;
            if (valid) {
                if (tempNode == this->head) {
                    this->head = this->head->next;
                    this->head->prev = NULL;
                    tempNode = head;
                }
                else {
                    tempNode->prev->next = tempNode->next;
                    if (tempNode->next != NULL)
                        tempNode->next->prev = tempNode->prev;
                    tempNode = tempNode->next;
                }

                value++;
                this->count--;
                delete rmv;
            }
            else tempNode = tempNode->next;
        }
        else tempNode = NULL;
    }
    return value;
}
string BusSystem::SQ(int N) {
    if (this->MaxBus != -1)return "-1";
    this->MaxBus = N;
    return "1";
}
string BusSystem::INS(string CODE, string LP, int Case, int TimeA, int TimeB) {
    Bus* added = new Bus(LP, CODE, Case, TimeA, TimeB);
    return this->insertBus(added);
}
string BusSystem::DEL(string CODE, int TimeA = -1, int TimeB = 1000000) {
    return toStr(this->removeBus(CODE, TimeA, TimeB));
}
int BusSystem::CS(string CODE, int Time, int Case) {
    int number0 = 0;
    Bus* tempNode = Jumpto(CODE);
    do {
        if (tempNode->Finish >= Time) {
            if (((Case == -2) || (tempNode->Case == Case)) && (tempNode->Start <= Time))
                number0++;
        }
        tempNode = tempNode->next;
    } while ((tempNode != NULL) && (tempNode->Code == CODE));
    return number0;
}
int BusSystem::CE(string CODE, int Time, int Case) {
    int number0 = 0;
    Bus* tempNode = Jumpto(CODE);
    do {
        if (tempNode->Finish < Time) {
            if ((Case == -2) || (tempNode->Case == Case))
                number0++;
        }
        tempNode = tempNode->next;
    } while ((tempNode != NULL) && (tempNode->Code == CODE));
    return number0;
}
string BusSystem::GS(string CODE, int Time, int Case) {
    Bus* tempNode = Jumpto(CODE);
    string name = "-1";
    int abs = Time + 1;
    do {
        if (Time - tempNode->Start >= 0) {
            if (Time - tempNode->Start < abs) {
                if ((Case == -2) || (tempNode->Case == Case)) {
                    abs = Time - tempNode->Start;
                    name = tempNode->ID;
                }
            }
            else if (Time - tempNode->Start == abs) {
                if (tempNode->Case == 0) {
                    abs = Time - tempNode->Start;
                    name = tempNode->ID;
                }
            }
        }
        tempNode = tempNode->next;
    } while ((tempNode != NULL) && (tempNode->Code == CODE));
    return name;
}
string BusSystem::GE(string CODE, int Time, int Case) {
    Bus* tempNode = Jumpto(CODE);
    string name = "-1";
    int abs = Time + 1;
    do {
        if (Time - tempNode->Finish > 0) {
            if (Time - tempNode->Finish < abs) {
                if ((Case == -2) || (tempNode->Case == Case)) {
                    abs = Time - tempNode->Finish;
                    name = tempNode->ID;
                }
            }
            else if (Time - tempNode->Finish == abs) {
                if (tempNode->Case == 0) {
                    abs = Time - tempNode->Finish;
                    name = tempNode->ID;
                }
            }
        }
        tempNode = tempNode->next;
    } while ((tempNode != NULL) && (tempNode->Code == CODE));
    return name;
}
string BusSystem::toStr(int number) {
    stringstream S;
    S << number;
    return S.str();
}
string BusSystem::query(string instruction) {
    // TODO: Your implementation
    if (!this->valid(instruction)) return "-1";
    stringstream ss;
    ss << instruction;
    string Text;
    ss >> Text;
    string LP = "NULL";
    string Code = "";
    int Case = -1;
    int Start = -1;
    int Finish = 1000000;
    if (Text == "SQ") {
        int N;
        ss >> N;
        return SQ(N);
    }
    ss >> Code;
    if (Text == "INS") {
        ss >> LP;
        ss >> Case;
        ss >> Start;
        ss >> Finish;
        if (Finish != 1000000) {
            if (Finish > Start)
                return this->INS(Code, LP, Case, Start, Finish);
            return "-1";
        }
        if (Start > Case)
            return this->INS(Code, LP, -1, Case, Start);
        return "-1";
    }
    if (Text == "DEL") {
        ss >> Start;
        ss >> Finish;
        if (Start == -1)return this->DEL(Code, -1, 1000000);
        if (Finish == 1000000)return this->DEL(Code, Start, -1);
        return this->DEL(Code, Start, Finish);
    }
    ss >> Start;
    ss >> Case;
    if (Case == -1)     Case = -2;
    if (Text == "CS") {
        return toStr(this->CS(Code, Start, Case));
    }
    if (Text == "CE") {
        return toStr(this->CE(Code, Start, Case));
    }
    if (Text == "GS") {
        return this->GS(Code, Start, Case);
    }
    return this->GE(Code, Start, Case);
}

int main()
{
    BusSystem bs = BusSystem();
    cout << bs.query("SQ 10") << endl;
    cout << bs.query("INS 50 A 1 5") << endl;
    cout << bs.query("CS 50 4");
    cout << bs.query("INS 50 B 3 4") << endl;
    cout << bs.query("INS 50 C 5 7") << endl;
    cout << bs.query("CS 50 4") << endl;
    cout << bs.query("CE 50 5") << endl;
    cout << bs.query("INS 49 AB 6 7") << endl;
    cout << bs.query("INS 50 D 7 8") << endl;
    cout << bs.query("INS 50 E 9 10") << endl;
    cout << bs.query("INS 50 F 11 12") << endl;
    cout << bs.query("CS 50 11") << endl << endl;
    cout << bs.query("DEL 49") << endl;
    cout << bs.query("CE 50 14") << endl;
    cout << bs.query("INS 49 AA 0 1") << endl;
    cout << bs.query("INS 49 AB 2 3") << endl;
    cout << bs.query("INS 49 AB 4 5") << endl;
    cout << bs.query("INS 49 AB 8 9") << endl;
    cout << bs.query("GS 49 5") << endl;
    cout << bs.query("DEL 49") << endl;




}

//du dau cach o dau cuoi va giua
