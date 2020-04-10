#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios;
using std::basic_string;
using std::string;

class account_query
{
private:

    struct AccountData
    {
    char account_number[20];
    char firstName[10];
    char lastName[10];
    float total_Balance;
    } Account;

    string welcome;
public:

    account_query(const string& text);
    ~account_query();

    void read_data();
    void show_data();
    void write_record();
    void read_record();
    void search_record();
    void edit_record();
    void delete_record();
};

void account_query::read_data()
{
    cout<<"\nEnter Account Number: ";
    cin>>Account.account_number;
    cout<<"Enter First Name: ";
    cin>>Account.firstName;
    cout<<"Enter Last Name: ";
    cin>>Account.lastName;
    cout<<"Enter Balance: ";
    cin>>Account.total_Balance;
    cout<<endl;
}
void account_query::show_data()
{
    cout<<"Account Number: "<<Account.account_number<<endl;
    cout<<"First Name: "<<Account.firstName<<endl;
    cout<<"Last Name: "<<Account.lastName<<endl;
    cout<<"Current Balance: Rs.  "<<Account.total_Balance<<endl;
    cout<<"-------------------------------"<<endl;
}
void account_query::write_record()
{
    ofstream outfile;
    outfile.open("record.bank", ios::binary|ios::app);
    read_data();
    outfile.write(reinterpret_cast<char *>(this), sizeof(*this));
    outfile.close();
}
void account_query::read_record()
{
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if(!infile)
    {
        cout<<"Error in Opening! File Not Found!!"<<endl;
        return;
    }
    cout<<"\n****Data from file****"<<endl;
    while(!infile.eof())
    {
        if(infile.read(reinterpret_cast<char*>(this), sizeof(*this))>0)
        {
            show_data();
        }
    }
    infile.close();
}
void account_query::search_record()
{
    int n;
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if(!infile)
    {
        cout<<"\nError in opening! File Not Found!!"<<endl;
        return;
    }
    infile.seekg(0,ios::end); // a "kurzort" a file végére teszi
    int count = infile.tellg()/sizeof(*this); // a "kurzor" a szöveg végén van, a tellg egy értéket ad vissza az adatmennyiségrõl, melyet osztunk, az osztás byte mennyiséggel történik.
    cout<<"\n There are "<<count<<" record in the file";
    cout<<"\n Enter Record Number to Search: ";
    cin>>n;
    infile.seekg((n-1)*sizeof(*this));
    infile.read(reinterpret_cast<char*>(this), sizeof(*this));
    show_data();
}
void account_query::edit_record()
{
    int n;
    fstream iofile;
    iofile.open("record.bank", ios::in|ios::binary);
    if(!iofile)
    {
        cout<<"\nError in opening! File Not Found!!"<<endl;
        return;
    }
    iofile.seekg(0, ios::end); //Sets the position of the next character to be extracted from the input stream.
    int count = iofile.tellg()/sizeof(*this); //Returns the position of the current character in the input stream.
    cout<<"\n There are "<<count<<" record in the file";
    cout<<"\n Enter Record Number to edit: ";
    cin>>n;
    iofile.seekg((n-1)*sizeof(*this));
    iofile.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout<<"Record "<<n<<" has following data"<<endl;
    show_data();
    iofile.close();
    iofile.open("record.bank", ios::out|ios::in|ios::binary);
    iofile.seekp((n-1)*sizeof(*this));
    cout<<"\nEnter data to Modify "<<endl;
    read_data();
    iofile.write(reinterpret_cast<char*>(this), sizeof(*this));
}
void account_query::delete_record()
{
    int n;
    ifstream infile;
    infile.open("record.bank", ios::binary);
    if(!infile)
    {
        cout<<"\nError in opening! File Not Found!!"<<endl;
        return;
    }
    infile.seekg(0,ios::end);
    int count = infile.tellg()/sizeof(*this);
    cout<<"\n There are "<<count<<" record in the file";
    cout<<"\n Enter Record Number to Delete: ";
    cin>>n;
    fstream tmpfile;
    tmpfile.open("tmpfile.bank", ios::out|ios::binary);
    infile.seekg(0);
    for(int i=0; i<count; i++)
    {
        infile.read(reinterpret_cast<char*>(this),sizeof(*this));
        if(i==(n-1))
            continue;
        tmpfile.write(reinterpret_cast<char*>(this), sizeof(*this));
    }
    infile.close();
    tmpfile.close();
    remove("record.bank");
    rename("tmpfile.bank", "record.bank");
}

account_query::account_query(const string& text) : welcome(text)
{
    cout<<welcome<<std::endl;
}

account_query::~account_query()
{
    cout<<"\n\n*******Good Bye*******\n"<<std::endl;
}

int program()
{
    int choice;

    account_query* A = new account_query("***Acount Information System***");

    while(true)
    {
        cout<<"Select one option below ";
        cout<<"\n\t1-->Add record to file";
        cout<<"\n\t2-->Show record from file";
        cout<<"\n\t3-->Search Record from file";
        cout<<"\n\t4-->Update Record";
        cout<<"\n\t5-->Delete Record";
        cout<<"\n\t6-->Quit";
        cout<<"\nEnter your choice: ";
        cin>>choice;
        switch(choice)
        {
        case 1:
            A->write_record();
            break;
        case 2:
            A->read_record();
            break;
        case 3:
            A->search_record();
            break;
        case 4:
            A->edit_record();
            break;
        case 5:
            A->delete_record();
            break;
        case 6:
            delete A;
            exit(0);
            break;
        default:
            cout<<"\nEnter correct choice";
            exit(0);
        }
    }
}

int main()
{
    program();

    system("pause");

    return 0;
}
