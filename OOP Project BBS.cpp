#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<istream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<cmath>
#include<ctime>
#pragma warning(disable : 4996)
#define _USE_MATH_DEFINES
#define PI 3.14
using namespace std;

class User;//1
class Admin;//2
class Citizen;//3
class Donor;//4
class Date;//5
class Receiver;//6
class Blood;//7
class Aprrover;//8
class Hospital;//9
class Transport;//10
template<class T>
class Records;//11
class Menu;//12

void showCompatibility(string grp)
{
	if (grp == "O+")
	{
		cout << "\nTry requesting for other compatible types for your blood group: O+ , O-";
	}
	else if (grp == "A+")
	{
		cout << "\nTry requesting for other compatible types for your blood group: O+ , O- , A+ , A-";
	}
	else if (grp == "A-")
	{
		cout << "\nTry requesting for other compatible types for your blood group: A- , O-";
	}
	else if (grp == "B+")
	{
		cout << "\nTry requesting for other compatible types for your blood group: O+ , O- , B+ , B-";
	}
	else if (grp == "B-")
	{
		cout << "\nTry requesting for other compatible types for your blood group: B- , O-";
	}
	else if (grp == "AB-")
	{
		cout << "\nTry requesting for other compatible types for your blood group: A- , B- , AB- , O- ";
	}
}
class Blood
{
	static int BloodPacket_ID;
	vector<string> group;
	
	static int UpdateIndex;
	static int UpdatePacks;
	static string UpdateGroup;
public:
	static int Packets[8];
	friend class Donor;
	friend class Receiver;
	friend class Hospital;
	Blood() {
		group.push_back("O+");
		group.push_back("O-");
		group.push_back("A+");
		group.push_back("A-");
		group.push_back("B+");
		group.push_back("B-");
		group.push_back("AB+");
		group.push_back("AB-");
		BloodPacket_ID++;
	}
	void setPack() 
	{
		int var;
		static int count = 0;
		string str;
		ifstream file;
		file.open("Blood.txt");
		while (getline(file, str))
		{
			var = stoi(str);
			Packets[count] = var;
			count++;
		}
		file.close();
	}
	int UpdateInfo(string bloodgrp)
	{
		for (int i = 0; i < group.size(); i++)
		{
			if (group[i] == bloodgrp)
			{
				UpdateGroup = bloodgrp;
				UpdateIndex = i;
			}
		}
		return UpdateIndex;
	}
	bool confirmRequest()
	{
		bool confirm = false;
		int opt, flag = 0;
		cout << "\nDo you still want to proceed with the order?(Enter 0 for No,1 for Yes) : ";
		cin >> opt;
		do
		{
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "\nPlease enter either 1 or 0" << endl;
				cin >> opt;
			}
			else if (opt == 1 || opt == 0)
			{
				flag = 1;
				if (opt == 1) confirm = true;
			}
		} while (flag == 0);
		return confirm;
	}
	bool checkStorage(int temp, char ch, int packs)
	{
		bool confirm = false;
		int i = temp;
		if (ch == 'd')//d for donator -> to check if we have enough storage	
		{
			if (Packets[i] < 5)
			{
				if (packs <= (5 - Packets[i]))
				{
					cout << "\nDonation Successful";
					confirm = true;
					UpdatePacks = packs;
				}
				else
				{
					cout << "\nWe can only accept " << (packs - (5 - Packets[i] )) << " packets at the moment due to full storage";
					confirm = confirmRequest();
					if (confirm == 1) UpdatePacks = (packs - (5 - Packets[i]));
				}
			}
			else if (Packets[i] == 5)
			{
				cout << "\nWe cannot accept any packets at the moment due to full storage";
				confirm = false;
			}
		}
		else if (ch == 'r' || ch=='h')//r for receiver and h for hospital -> to check if we have enough packets+TRANSPORT KO YAHAN BULAANA HE
		{
			if (packs <= Packets[i])
			{
				cout << "\nReception Successful" << endl;
				confirm = true;
				UpdatePacks = packs;
			}
			else if (Packets[i] == 0)
			{
				cout << "\nWe are out of reserves for Group " << UpdateGroup;
				confirm = false;
			}
			else if (packs > Packets[i])
			{
				cout << "\nWe can only provide " << Packets[i] << " packets at the moment due to limited reserves" << endl;
				confirm = confirmRequest();
				if (confirm == 1) UpdatePacks = Packets[i];

			}
		}
		return confirm;
	}
	static void UpdateBloodReserves()
	{
		int var=0; string str;
		fstream file;
		file.open("Blood.txt");
		for (int i = 0; i < 8; i++)
		{
			file << Packets[i] << endl;
		}
		file.close();
	}
	friend void operator ++(Blood&, int);
	friend void operator --(Blood&, int);
	friend ostream& operator <<(ostream& s, const Blood&);
};
void operator --(Blood& blood, int)
{
	blood.Packets[blood.UpdateIndex]--;
}
void operator ++(Blood& blood, int)
{
	blood.Packets[blood.UpdateIndex]++;
}
ostream& operator <<(ostream& s, const Blood& blood)
{
	for (int i = 0; i < blood.group.size(); i++)
	{
		s << blood.group[i];
		s << blood.Packets[i] << endl;
	}
	return s;
}
int Blood::Packets[8] = { 0,0,0,0,0,0,0,0 };
int Blood::BloodPacket_ID = 0;
int Blood::UpdateIndex = 0;
int Blood::UpdatePacks = 0;
string Blood::UpdateGroup = "";


class User
{
protected:
	string userName, password;
public:
	User(){}
	virtual bool login() = 0;
	User(string userName, string password) :userName(userName), password(password) {}
	void setPassWord(string password) { this->password = password; }
	string get_name() {return userName;}
	string get_password() {return password;}
};

class Admin :public User
{
public:
	Admin()
	{
		userName = "admin";
		password = "pass";
	}
	Admin(string userName, string password) :User(userName, password) {}
	bool login()
	{
		int flag = 0;
		string user, pass;
		system("cls");
		cout << "\n\n\t\t\tLOGIN ";
		cout << "\n\t\t\t-----";
		cout << "\n\n\t\t\tUSERNAME :";
		cin >> user;
		cout << "\n\t\t\tPASSWORD :";
		cin >> pass;

		
		if (user == userName && pass == password)
		{
			cout << "\n\n\t\t\tLogging In...";
			Sleep(1000);
			system("cls");
			return true;
		}	
		else return false;
	}
};



class Citizen :public User
{
protected:
	int   age;
	char  gender;
	float weight;
	string bloodgroup;
	float  HB;
	int   pulse;
	float SystolicBP;
	float DiastolicBP;
	Blood blood;
public:
	Citizen()
	{
		age=0;
		gender=' ';
		weight=0;
		bloodgroup="";
		HB=0;
		pulse=0;
		SystolicBP=0;
		DiastolicBP=0;
	}
	Citizen( int age, char gender, float weight, string blood_type, float HB, int pulse, float SystolicBP, float DiastolicBP) : age(age), gender(gender), weight(weight), bloodgroup(bloodgroup), HB(HB), pulse(pulse), SystolicBP(SystolicBP), DiastolicBP(DiastolicBP) { }
	
	void setAge(int age) 
	{
		int flag = 0;
		do
		{
			if (age>0)
			{
				this->age = age;
				flag = 1;
			}
			else
			{
				cout << "Please enter a number greater than 0 " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> age;
			}
		} while (flag == 0);
	}
	void setGen(char gender) 
	{ 
		int flag = 0;
		do
		{
			if (gender == 'M' || gender == 'F')
			{
				this->gender = gender;
				flag = 1;
			}
			else
			{
				cout << "Please enter M or F only " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> gender;
			}
		} while (flag == 0);
	}
	void setWeight(float weight)
	{ 
		int flag = 0;
		do
		{
			if (weight >0)
			{
				this->weight = weight;
				flag = 1;
			}
			else
			{
				cout << "Please enter a number greater than 0 " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> weight;
			}
		} while (flag == 0);
	}
	void setpulse(int pulse) 
	{ 
		int flag = 0;
		do
		{
			if (pulse > 0)
			{
				this->pulse = pulse;
				flag = 1;
			}
			else
			{
				cout << "Please enter a number greater than 0 " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> pulse;
			}
		} while (flag == 0);
	}
	void setHB(float HB) 
	{ 
		int flag = 0;
		do
		{
			if (HB > 0)
			{
				this->HB = HB;
				flag = 1;
			}
			else
			{
				cout << "Please enter a number greater than 0 " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> HB;
			}
		} while (flag == 0);
	}
	void setSBP(float SystolicBP) 
	{
		int flag = 0;
		do
		{
			if (SystolicBP > 0)
			{
				this->SystolicBP = SystolicBP;
				flag = 1;
			}
			else
			{
				cout << "Please enter a number greater than 0 " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> SystolicBP;
			}
		} while (flag == 0);
	}
	void setDBP(float DiastolicBP) 
	{
		int flag = 0;
		do
		{
			if (DiastolicBP > 0)
			{
				this->DiastolicBP = DiastolicBP;
				flag = 1;
			}
			else
			{
				cout << "Please enter a number greater than 0 " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> DiastolicBP;
			}
		} while (flag == 0);
	}
	void setbloodgroup(string bloodgroup)
	{
		int flag = 0;
		do
		{
			if (bloodgroup == "O+" || bloodgroup == "O-" || bloodgroup == "A+" || bloodgroup == "A-" || bloodgroup == "B+" || bloodgroup == "B-" || bloodgroup == "AB+" || bloodgroup == "AB-")
			{
				this->bloodgroup = bloodgroup;
				flag = 1;
			}
			else
			{
				cout << "This blood group doesn't exists.Please enter a correct blood group " << endl;
				cin >> bloodgroup;
			}
		} while (flag == 0);
	}
	int get_age()
	{
		return age;
	}
	char getgen()
	{
		return gender;
	}
	float getweight()
	{
		return weight;
	}
	string getbloodgrp()
	{
		return bloodgroup;
	}
	float getHB()
	{
		return HB;
	}
	int getpulse()
	{
		return pulse;
	}
	float getsysBP()
	{
		return SystolicBP;
	}
	float getdiBP()
	{
		return DiastolicBP;
	}


	void registration()
	{
		int flag = 0;
		string user, pass;
		system("cls");
		cout << "\n\n\t\t\tREGISTERATION ";
		cout << "\n\t\t\t-------------";
		cout << "\n\n\t\t\tUSERNAME :";
		cin >>  userName;

		ifstream input("Citizen.txt");
		while (input >> user )                    
		{
			if (user == userName )
			{
				flag = 1;
			}
		}
		input.close();     

		if (flag == 1) { cout << "\n\n\t\t\tUser Name Already Exists. Please re-enter a unique name."; Sleep(2000); }
		else
		{
			cout << "\n\t\t\tEnter the password :";
			cin >> password;
			ofstream f1("Citizen.txt", ios::app);
			f1 << userName << ' ' << password << endl;
			f1.close();
			cout << "\n\n\t\t\tRegistration is Successful\n";
			Sleep(2000);
		}

	}
	bool login()
	{
		int flag = 0;
		string user, pass;
		system("cls");
		cout << "\n\n\t\t\tLOGIN ";
		cout << "\n\t\t\t-----";
		cout << "\n\n\t\t\tUSERNAME :";
		cin >> userName;
		cout << "\n\t\t\tPASSWORD :";
		cin >> password;

		ifstream input("Citizen.txt");
		while (input >> user >> pass)                     
		{
			if (user == userName && pass == password)
			{
				flag = 1;
				cout << "\n\n\t\t\tLogging In...";
				Sleep(1000);
				system("cls");
			}
		}
		input.close();     

		if (flag == 1) return true;
		else return false;

	}
};

class Hospital :public User
{
	string requiredBloodGroup;
	long double latitude, longitude;
	bool EmergencyStatus;
	Blood blood;
public:
	Hospital()
	{
		latitude = 0;
		longitude = 0;
		EmergencyStatus = false;
	}
	void setLocation(string name)
	{
		if (name == "Indus")
		{
			latitude = 75.31152343750001;
			longitude = 30.123472222;
		}
		else if (name == "Liaquat")
		{
			latitude = 70.31152343750001;
			longitude = 31.123472222;
		}
		else if (name == "Dow")
		{
			latitude = 50.31152343750001;
			longitude = 21.123472222;
		}
		EmergencyStatus = false;
	}

	void setName(string name) { userName = name; }
	string getName() { return userName; }
	string getBloodGroup() { return requiredBloodGroup; }
	long double getLat() { return latitude; }
	long double getLong() { return longitude; }
	bool getEmergencyStatus() { return EmergencyStatus; }
	Blood& getBlood() { return blood; }
	
	void setEmergencyStatus(bool EmergencyStatus)
	{
		if (EmergencyStatus == 0 || EmergencyStatus == 1)
		{
			this->EmergencyStatus = EmergencyStatus;
		}
	}
	void setBloodGroup(string bloodgroup)
	{
		int flag = 0;
		do
		{
			if (bloodgroup == "O+" || bloodgroup == "O-" || bloodgroup == "A+" || bloodgroup == "A-" || bloodgroup == "B+" || bloodgroup == "B-" || bloodgroup == "AB+" || bloodgroup == "AB-")
			{
				this->requiredBloodGroup = bloodgroup;
				flag = 1;
			}
			else
			{
				cout << "This blood group doesn't exists.Please enter a correct blood group " << endl;
				cin >> bloodgroup;
			}
		} while (flag == 0);
	}
	bool login()
	{
		int flag = 0;
		string user, pass;
		system("cls");
		cout << "\n\n\t\t\tLOGIN ";
		cout << "\n\t\t\t-----";
		cout << "\n\n\t\t\tUSERNAME :";
		cin >> userName;
		cout << "\n\t\t\tPASSWORD :";
		cin >> password;

		ifstream input("Hospital.txt");
		while (input >> user >> pass)                 
		{
			if (user==userName && pass== password)
			{
				flag = 1;
				cout << "\n\n\t\t\tLogging In...";
				Sleep(1000);
				system("cls");
			}
		}
		input.close();     
		
		if (flag == 1) return true;
		else return false;
		
	}
	bool orderBlood(int temp)
	{
		int packs;
		cout << "\nHow many packets do you want to receive?";
		do
		{
			cout << "(Limit from 1-10 only) : ";
			cin >> packs;
		} while (packs <= 0 || packs > 10);

		if (blood.checkStorage(temp, 'h', packs) == 1)
		{
			for (int i = 0; i < blood.UpdatePacks; i++)
			{
				blood--;
				blood.UpdateBloodReserves();
			}
			return true;
		}
		else return false;
	}
	friend ostream& operator<<(ostream&, Hospital&);
};


ostream& operator<<(ostream& s, Hospital& h)
{
	cout << h.userName;
	cout << h.requiredBloodGroup;
	cout << h.latitude ;
	cout << h.longitude;
	cout << h.EmergencyStatus;
	return s;
 }

class Transport
{
	string branchCity[3];
	long double latitude[3], longitude[3];

	 long double getdistance(long double lat1, long double long1, long double lat2, long double long2)
	{
		lat1 = ((PI) / 180) * lat1;       // Convert the latitudes
		long1 = ((PI) / 180) * long1;	 // and longitudes
		lat2 = ((PI) / 180) * lat2;		// from degree to radians.
		long2 = ((PI) / 180) * long2;

		// Haversine Formula
		long double ans = pow(sin((lat2 - lat1) / 2), 2) + cos(lat1) * cos(lat2) * pow(sin((long2 - long1) / 2), 2);
		ans = 2 * asin(sqrt(ans)) * 6731; // Radius of Earth=6371 km 
		return ans;
	}
	 void EmergencyTransport(Hospital& h)
	{
		int temp=0;
		long double minDistance = 100000;
		for (int i = 0; i < 3; i++)
		{
			if (getdistance(latitude[i], longitude[i], h.getLat(), h.getLong()) < minDistance)
			{
				minDistance = getdistance(latitude[i], longitude[i], h.getLat(), h.getLong());
				temp = i;
			}
		}
		cout << "\n\nEmergency Detected: You will soon be receiving your order from our nearest branch in " << branchCity[temp] << ".\n";
	}
public:
	Transport()
	{
		latitude[0] = 30.3308401;
		longitude[0] = 71.247499;
		branchCity[0] = "Multan";
		latitude[1] = 24.856534339310674;
		longitude[1] = 66.99462890625001;
		branchCity[1] = "Karachi";
		latitude[2] = 31.541089879585837;
		longitude[2] = 74.31152343750001;
		branchCity[2] = "Lahore";

	}
	 void dispatchOrder(Hospital& h)
	{
		if (h.getEmergencyStatus() == true) EmergencyTransport(h);
		else cout << "\n\nOur Transport Service has been Notified about your order. We appreciate your patience till they contact you.\n";
	}
};


class Date
{
	int d, m, y;
	// To store number of days in 
// all months from January to Dec.
	const int monthDays[12]
		= { 31, 28, 31, 30, 31, 30,
		   31, 31, 30, 31, 30, 31 };
	friend class Approval;
public:
	Date():d(0),m(0),y(0) {}
	Date(int d, int m, int y) :d(d), m(m), y(y) {}

	void setDate(int d) { this->d = d; }
	void setMonth(int m) { this->m = m; }
	void setYear(int y) { this->y = y; }
	// This function counts number of 
// leap years before the given date
	int countLeapYears(Date d)
	{
		int years = d.y;

		// Check if the current year needs to be
		//  considered for the count of leap years
		// or not
		if (d.m <= 2)
			years--;

		// An year is a leap year if it 
		// is a multiple of 4,
		// multiple of 400 and not a 
		 // multiple of 100.
		return years / 4
			- years / 100
			+ years / 400;
	}
	// This function returns number of 
// days between two given dates
	int getDifference(Date dt1, Date dt2)
	{
		// COUNT TOTAL NUMBER OF DAYS
		// BEFORE FIRST DATE 'dt1'

		// initialize count using years and day
		long int n1 = dt1.y * 365 + dt1.d;

		// Add days for months in given date
		for (int i = 0; i < dt1.m - 1; i++)
			n1 += monthDays[i];

		// Since every leap year is of 366 days,
		// Add a day for every leap year
		n1 += countLeapYears(dt1);

		// SIMILARLY, COUNT TOTAL NUMBER OF
		// DAYS BEFORE 'dt2'

		long int n2 = dt2.y * 365 + dt2.d;
		for (int i = 0; i < dt2.m - 1; i++)
			n2 += monthDays[i];
		n2 += countLeapYears(dt2);

		// return difference between two counts
		return (n2 - n1);
	}
	Date& operator =(Date& date)
	{
		d=date.d ;
		m=date.m ;
		y=date.y ;
		return *this;
	}
	friend ostream& operator<<(ostream&,const Date&);
};
ostream& operator<<(ostream& s, const Date&date)
{
	s << date.d << "/" << date.m << "/" << date.y;
	return s;
}
	

class Donor :public Citizen
{
	 Date lastDonate;
public:
	Donor()
	{
		lastDonate.setDate(0);
		lastDonate.setMonth(0);
		lastDonate.setYear(0);
	}

	void setName(string name) { userName=name; }
	string getName() { return userName; }
	void setLastDonate(Date d) { lastDonate = d; }
	Date& getLastDonate() { return lastDonate; }
	void updateLastDonate()
	{
		time_t currentTime = time(0);
		tm* local_time = localtime(&currentTime);
		lastDonate.setDate(local_time->tm_mday);
		lastDonate.setMonth(local_time->tm_mon + 1);
		lastDonate.setYear(local_time->tm_year + 1900);
	}
	bool donateBlood(int temp)
	{
		int packs;
		cout << "\nHow many packets do you want to donate?";
		do
		{
			cout << "(Limit from 1-5 only) : ";
			cin >> packs;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "\nPlease enter an integer only : " ;
				cin >> packs;
			}
		} while (packs <= 0 || packs > 5);
		if (blood.checkStorage(temp, 'd', packs) == 1)
		{
			for (int i = 0; i < blood.UpdatePacks; i++)
			{
				updateLastDonate();
				blood++;
				blood.UpdateBloodReserves();
			}
			return true;
		}
		else return false;
	}
	friend ostream& operator <<(ostream& , Donor &);
};

ostream& operator <<(ostream &s, Donor &donor)
{
	cout << endl;
	s << donor.userName << "\t";
	s << donor.age << "\t";
	s << donor.gender << "\t";
	s << donor.weight << "\t    ";
	s << donor.bloodgroup << "\t\t";
	s << donor.HB << "\t";
	s << donor.pulse << "\t    ";
	s << donor.SystolicBP << "\t\t";
	s << donor.DiastolicBP << "\t\t";
	s << donor.lastDonate;
	return s;
}


class Receiver :public Citizen
{
	 Date lastReception;
public:
	Receiver()
	{
		lastReception.setDate(0);
		lastReception.setMonth(0);
		lastReception.setYear(0);
	}
	void setName(string name) { userName = name; }
	string getName() { return userName; }
	void setLastReception(Date d) { lastReception = d; }
	Date& getLastReception() { return lastReception; }

	void updateLastReception()
	{
		time_t currentTime = time(0);
		tm* local_time = localtime(&currentTime);
		lastReception.setDate(local_time->tm_mday);
		lastReception.setMonth(local_time->tm_mon + 1);
		lastReception.setYear(local_time->tm_year + 1900);
	}
	bool receiveBlood(int temp)
	{
		int packs;
		cout << "\nHow many packets(1-5) do you want to receive?";
		do
		{
			cout << "(Limit from 1-5 only) : ";
			cin >> packs;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "\nPlease enter an integer only : " << endl;
				cin >> packs;
			}
		} while (packs <= 0 || packs > 5);

		if (blood.checkStorage(temp, 'r', packs) == 1)
		{
			for (int i = 0; i < blood.UpdatePacks; i++)
			{
				updateLastReception();
				blood--;
				blood.UpdateBloodReserves();
			}
			return true;
		}
		else return false;
	}
	friend ostream& operator <<(ostream&, Receiver&);
};
 ostream& operator <<(ostream&s, Receiver&receiver)
 {
	 cout << endl;
	 s << receiver.userName << "\t";
	 s << receiver.age << "\t";
	 s << receiver.gender << "\t";
	 s << receiver.weight << "\t    ";
	 s << receiver.bloodgroup << "\t\t";
	 s << receiver.HB << "\t";
	 s << receiver.pulse << "\t    ";
	 s << receiver.SystolicBP << "\t\t";
	 s << receiver.DiastolicBP << "\t\t";
	 s << receiver.lastReception;
	 return s;
}

class Approver
{
	Date date;
public:
	bool CheckDonorHealthProfile(Donor& d)
	{
		int criteria = 0;
		if (d.getgen() == 'F')
		{
			if (d.getHB() > 12.5 && d.getHB() < 20.0) criteria++; 
		}
		else if (d.getgen() == 'M')
		{
			if (d.getHB() > 13.0 && d.getHB() < 20.0) criteria++; 
				
		}
		if (d.getweight() > 45) criteria++; 
		
		if (d.getpulse() > 50 && d.getpulse() < 100) criteria++;
		
		if (d.get_age() >= 18 && d.get_age() <= 65) criteria++; 
		
		if (d.getsysBP() < 180 && d.getdiBP() < 100) criteria++;
		if (criteria == 5)
		{
			cout << "\nHealth Profile Verified\n";
			Sleep(1500);
			return true;
		}
		else
		{
			cout << "\nHealth Profile Not Verified\n";

			Sleep(1500);
			return false;
		}	
	}

	bool verifyDonation(Donor& d)
	{
		Date now;
		time_t currentTime = time(0);
		tm* local_time = localtime(&currentTime);
		now.setDate(local_time->tm_mday);
		now.setMonth(local_time->tm_mon + 1);
		now.setYear(local_time->tm_year + 1900);

		if (date.getDifference(d.getLastDonate(), now) > 56)
			return true;//56 days between donations
		else return false;
	}
	bool verifyTransfusion(Receiver& r)
	{
		Date now;
		time_t currentTime = time(0);
		tm* local_time = localtime(&currentTime);
		now.setDate(local_time->tm_mday);
		now.setMonth(local_time->tm_mon + 1);
		now.setYear(local_time->tm_year + 1900);

		if (date.getDifference(r.getLastReception(), now) > 7)  return true;//7 days between tranfusions
		else return false;
	}
};

template<class T>
class Records
{
public:
	static void add(string name, T &data)
	{
		ofstream file;
		file.open(name, ios::app);
		file.write((char*)&data, sizeof(data));
		file.close();
	}

	static void show(string name, T &data)
	{
		ifstream file;
		file.open(name, ios::in);
		if (!file) 
			cout << "Error while creating the file";
		else
		{
			file.seekg(0);
			file.read((char*)&data, sizeof(data));
			while (!file.eof())
			{
				cout << data;
				file.read((char*)&data, sizeof(data));
			}
			file.close();
		}
	}
	static bool search(string filename, T& data, string tempnm)
	{
		int flag;
		flag= 0;
		ifstream file;
		file.open(filename, ios::in);
		file.seekg(0);
		while (file.read((char*)&data, sizeof(data)))
		{
			if (data.getName() == tempnm)
			{
				flag = 1;
				cout << data;
			}
		}
		file.close();
		if (flag == 1)
			return true;
		else return false;
	}
	static bool confirmPresence(string filename, T& data, string tempnm)
	{
		int flag;
		flag = 0;
		ifstream file;
		file.open(filename, ios::in);
		file.seekg(0);
		while (file.read((char*)&data, sizeof(data)))
		{
			if (data.getName() == tempnm)
			{
				flag = 1;
			}
		}
		file.close();
		if (flag == 1)
			return true;
		else return false;
	}
	static bool del(char filename[], T& data, string tempnm)
	{
		int flag = 0;
		ofstream temp;
		temp.open("temp.txt", ios::app);
		ifstream file;
		file.open(filename, ios::in);
		file.seekg(0);
		if(confirmPresence( filename, data,tempnm))
		{
			while (!file.eof())
			{
				if (tempnm != data.getName())
				{
					flag = 1;
					temp.write((char*)&data, sizeof(data));
				}
				file.read((char*)&data, sizeof(data));
			}
			file.close();
			temp.close();
			remove(filename);
			rename("temp.txt", filename);
			return true;
		}
		else
		{
			remove("temp.txt");
			return false;
		}
	}
	static void updatePassword(char filename[], string oldPass)
	{
		string newPass,str,name;
		int flag = 0;

		cout << "\nPlease Enter New Password : ";
		cin >> newPass;

		ofstream temp;
		temp.open("temp.txt", ios::app);

		ifstream file;
		file.open(filename, ios::in);
		file.seekg(0);
		while (file>>name>>str)
		{
			if (str == oldPass)
			{
				str=newPass;
				flag = 1;
			}
			temp << name << ' ' << str << endl;
		}
		file.close();
		temp.close();
		if (flag==1)
		{
			remove(filename);
			rename("temp.txt", filename);
		}
		else 
		{
			remove("temp.txt");
		}
	}
	static Date& readlastDonate(string filename, Donor &donor, string tempnm,Date &d)
	{
		int flag=0;
		ifstream file;
		file.open(filename, ios::in);
		file.seekg(0);
		while (file.read((char*)&donor, sizeof(donor)))
		{
			if (donor.getName() == tempnm)
			{
				flag = 1;
				break;
			}
		}
		file.close();
		if (flag == 1)
			return donor.getLastDonate();
		else return d;
		
	}
	static Date& readlastRecept(string filename, Receiver &receiver, string tempnm,Date &d )
	{
		int flag=0;
		ifstream file;
		file.open(filename, ios::in);
		file.seekg(0);
		while (file.read((char*)&receiver, sizeof(receiver)))
		{
			if (receiver.getName() == tempnm)
			{
				flag = 1;
				break;
			}
		}
		file.close();
		if (flag == 1)
			return receiver.getLastReception();
		else return d;
	}
};


class Menu
{
public:
	static void showMainPage()
	{
		system("cls");
		cout << "\t\t\t                                                                 \n\n\n";
		cout << "\t\t\t______________________        MAIN        ______________________\n\n";
		cout << "                                                                                \n\n";
		cout << "\t\t\t\t\t |          1 Admin           |" << endl;
		cout << "\t\t\t\t\t |          2 Citizen         |" << endl;
		cout << "\t\t\t\t\t |          3 Hospital        |" << endl;
		cout << "\t\t\t\t\t |          4 Exit            |" << endl;
		cout << "\n\t\t\t\t\t   Please Enter your choice :";
	}
	static void showAdminMenu(string name)
	{
		system("cls");
		cout << "\n\n\t\t\t\t\t\t      WELCOME ";
		cout << "\n\n\t\t\t  _______________________      " << name << "     ______________________\n\n";
		cout << "\n\n\t\t\t______________________         MENU        ______________________\n\n";
		cout << "                                                                                \n\n";
		cout << "\t\t\t\t   |            1 See Records		            |" << endl;
		cout << "\t\t\t\t   |            2 Search Records                    |" << endl;
		cout << "\t\t\t\t   |            3 Remove Records                    |" << endl;
		cout << "\t\t\t\t   |            4 See Blood Reserves                |" << endl;
		cout << "\t\t\t\t   |            5 Log Out			    |" << endl;
		cout << "\n\t\t\t\t\t   Please Enter your choice :";
	}
	static void showCitizenMain()
	{
		system("cls");
		cout << "\n\n\t\t\t______________________        MENU        ______________________\n\n";
		cout << "                                                                                \n\n";
		cout << "\t\t\t\t\t|           1 Register         |" << endl;
		cout << "\t\t\t\t\t|           2 Log In           |" << endl;
		cout << "\t\t\t\t\t|           3 Go Back          |" << endl;
		cout << "\n\t\t\t\t\t   Please Enter your choice :";
	}
	static void showCitizenMenu(string name)
	{
		system("cls");
		cout << "\n\n\t\t\t\t\t\t     WELCOME ";
		cout << "\n\n\t\t\t______________________         "<<name<<"        ______________________\n\n";
		cout << "\n\n\t\t\t______________________         MENU        ______________________\n\n";
		cout << "                                                                                \n\n";
		cout << "\t\t\t\t\t|          1 Donate Blood           |" << endl;
		cout << "\t\t\t\t\t|          2 Receive Blood          |" << endl;
		cout << "\t\t\t\t\t|          3 Change Password        |" << endl;
		cout << "\t\t\t\t\t|          4 Log Out                |" << endl;
		cout << "\n\t\t\t\t\t   Please Enter your choice :";
	}
	template<typename T>
	static T& setCitizendetails(T &c)
	{
		char gender;
		int age,pulse;
		float weight,HB,SBP,DBP;
		string blood_type;
		cout << "\n\nEnter your age: ";
		cin >> age;
		c.setAge(age);
		cout << "Enter your gender (M/F): ";
		cin >> gender;
		c.setGen(gender);
		cout << "Enter your weight in kg: ";
		cin >> weight;
		c.setWeight(weight);
		cout << "Enter your blood type(A-,A+,B-,B+,AB-,AB+,O-,O+): " ;
		cin >> blood_type;
		c.setbloodgroup(blood_type);
		cout << "Enter your HB: ";
		cin >> HB;
		c.setHB(HB);
		cout << "Enter your pulse rate: ";
		cin >> pulse;
		c.setpulse(pulse);
		cout << "Enter your systolic Blood Pressure: ";
		cin >> SBP;
		c.setSBP(SBP);
		cout << "Enter your diastolic Blood Pressure: ";
		cin >> DBP;
		c.setDBP(DBP);	
		return c;
	}
	static void showHospitalMenu(string name)
	{
		system("cls");
		cout << "\n\n\t\t\t\t\t\t      WELCOME ";
		cout << "\n\n\t\t\t______________________    " << name << " Hospital    ______________________\n\n";
		cout << "\n\n\t\t\t______________________         MENU        ______________________\n\n";
		cout << "                                                                                \n\n";
		cout << "\t\t\t\t|            1 Update Emergency Status         |" << endl;
		cout << "\t\t\t\t|            2 Order Blood		       |" << endl;
		cout << "\t\t\t\t|            3 Log Out                         |" << endl;
		cout << "\n\t\t\t\t\t   Please Enter your choice :";
	}
	static void showCompatibility(string grp)
	{
		if (grp == "O+")
		{
			cout << "\nTry requesting for other compatible types for your blood group: O+ , O-";
		}
		else if (grp == "A+")
		{
			cout << "\nTry requesting for other compatible types for your blood group: O+ , O- , A+ , A-";
		}
		else if (grp == "A-")
		{
			cout << "\nTry requesting for other compatible types for your blood group: A- , O-";
		}
		else if (grp == "B+")
		{
			cout << "\nTry requesting for other compatible types for your blood group: O+ , O- , B+ , B-";
		}
		else if (grp == "B-")
		{
			cout << "\nTry requesting for other compatible types for your blood group: B- , O-";
		}
		else if (grp == "AB-")
		{
			cout << "\nTry requesting for other compatible types for your blood group: A- , B- , AB- , O- ";
		}
	}
	static void showBloodGroups(int c)
	{
		cout << endl<<endl;
		if (c == 1) cout << "Group O+    =   ";
		if (c == 2) cout << "Group O-    =   ";
		if (c == 3) cout << "Group A+    =   ";
		if (c == 4) cout << "Group A-    =   ";
		if (c == 5) cout << "Group B+    =   ";
		if (c == 6) cout << "Group B-    =   ";
		if (c == 7) cout << "Group AB+   =   ";
		if (c == 8) cout << "Group AB-   =   ";
	}
};

int main()
{
	system("cls");
	cout<<"\n\n\n\n\t\t\t\t\t\t B L O O D  B A N K\n\n\t\t\t\t\t\t  M A N A G M E N T\n\n\t\t\t\t\t\t     S Y S T E M";
	printf("\n\n\n\t\t\t\t\t\t     loading....");
	Sleep(3000);
	Admin admin;
	Approver approver;
	Citizen citizen;
	Donor donor;
	Receiver receiver;
	Hospital hospital;
	Transport transport;
	Blood blood;
	Date d;
	int op=0,opt = 0, opt1 = 0, opt2 = 0, opt3 = 0, temp= 0,temp_c=0, flag = 0,var=0;
	static int count = 0;
	string bloodgroup,name,str;
	fstream file;
	char filename1[] = "Donor.txt";
	char filename2[] = "Receiver.txt";
	char filename3[] = "Citizen.txt";
	blood.setPack();
	
	while (1)
	{
		temp = 0;
		opt = 0;
		flag = 0;
		Menu::showMainPage();
		cin >> opt;
		cout << endl;
		switch (opt)
		{
		case 1://Admin
			if (admin.login())
			{
				while (1)
				{
					if (flag == 1) break;
					Menu::showAdminMenu(admin.get_name());
					cin >> op;
					switch (op)
					{
					case 1://see records
						system("cls");
						cout << "Name\t" << "Age\t" << "Gender\t" << "Weight\t" << "BloodGroup\t" << "HB\t" << "Pulse\t" << "SystolicBP\t" << "DiastolicBP\t" << "Last Activity";
						Records<Donor>::show("Donor.txt", donor);
						Records<Receiver>::show("Receiver.txt", receiver);
						cout<<endl<<endl<<endl;
						system("pause");
						break;
					case 2://search records
						cout << "\nPlease enter the username whose records you want to search: ";
						cin >> name;
						if (Records<Donor>::search(filename1, donor, name)==0 && Records<Receiver>::search(filename2, receiver, name)==0)
						{
							cout << "\nRecords for "<< name<<" not found";
						}
						cout << endl << endl << endl;
						system("pause");
						break;
					case 3://remove records
						cout << "\nPlease enter the username whose records you want to remove: ";
						cin >> name;
						if (Records<Donor>::del(filename1, donor, name) == 0 && Records<Receiver>::del(filename2, receiver, name) == 0)
						{
							cout << "\nRecords for " << name << " not found";
						}
						else cout << "\nRecords removed successfully";
						cout << endl << endl << endl;
						system("pause");
						break;
					case 4://see blood reserves
						count = 0;
						file.open("Blood.txt");
						while (getline(file, str))
						{
							count++;
							Menu::showBloodGroups(count);
							var = stoi(str);
							cout << var << '\n';
						}
						file.close();
						cout << endl << endl << endl;
						system("pause");
						break;
					case 5:
						flag = 1;
						cout << "\n\n\t\t\t\t\t         Logging Off...\n\n";
						Sleep(1000);
						break;
					default:
						cout << "\n\n\t\t\t\t   Please select from the options given above.\n" << endl;
						Sleep(1500);
						cin.clear();
						cin.ignore(1000, '\n');
					}
				}
			}
			else cout << "\n\t\t\t-Please enter the correct username and password";
			Sleep(1500);
			break;
		case 2://Citizen(Donator or Receiver)
			while (1)
			{
				if (flag == 1) break;
				flag = 0;
				Menu::showCitizenMain();
				cin >> opt1;
				switch (opt1)
				{
				case 1://registeration
					citizen.registration();
					break;
				case 2://login
					if (citizen.login())
					{
						while (1)
						{
							if (flag == 2) break;
							flag = 0;
							Menu::showCitizenMenu(citizen.get_name());
							cin >> opt2;
							switch (opt2)
							{
							case 1://Donate blood
								donor = (Menu::setCitizendetails(donor));
								donor.setName(citizen.get_name());
								cout << "\n\nPlease wait while we receive approval for donation\n";
								Sleep(2000);
								donor.setLastDonate(Records<Donor>::readlastDonate("Donor.txt", donor, donor.getName(),d));
								if (approver.verifyDonation(donor))
								{
									if (approver.CheckDonorHealthProfile(donor))
									{
										temp_c = blood.UpdateInfo(donor.getbloodgrp());
										if (donor.donateBlood(temp_c))
										{
											Records<Donor>::add("Donor.txt", donor);
										}
										Sleep(2000);
									}
									else
									{
										cout << "\n\nSorry! We cannot approve your request at the moment. Please ensure your information was entered correctly.";
										Sleep(2000);
									}
								}
								else
								{
									cout << "\n\nYour last donation was on " << donor.getLastDonate();
									cout << "\nSince there must be a gap of 8 weeks between donations, we cannot approve your request now.";
									Sleep(5000);
								}
								
								break;
							case 2://Receive blood
								receiver = (Menu::setCitizendetails(receiver));
								receiver.setName(citizen.get_name());
								cout << "\n\nPlease wait while we receive approval for reception\n";
								Sleep(2000);
								receiver.setLastReception(Records<Receiver>::readlastRecept("Receiever.txt", receiver, receiver.getName(),d));
								if (approver.verifyTransfusion(receiver))
								{
									temp_c = blood.UpdateInfo(receiver.getbloodgrp());
									if (receiver.receiveBlood(temp_c))
										Records<Receiver>::add("Receiver.txt", receiver);
									else Menu::showCompatibility(receiver.getbloodgrp());
									Sleep(2000);
								}
								else
								{
									cout << "\n\nYour last reception was on " << receiver.getLastReception();
									cout << "\nSince there must be a gap of 1 week between tranfusions, we cannot approve your request now.";
								}
								Sleep(3000);
								break;
							case 3://change password
									Records<Citizen>::updatePassword(filename3, citizen.get_password());
									cout << "\nPassword Updated!";
									Sleep(1000);
								break;
							case 4://log off
								flag = 2;
								cout << "\n\n\t\t\t\t\t         Logging Off...\n\n";
								Sleep(1000);
								break;
							default:
								cout << "\n\n\t\t\t\t   Please select from the options given above.\n" << endl;
								Sleep(2000);
								cin.clear();
								cin.ignore(1000, '\n');
							}
						}
					}
					else cout << "\n\t\t\t-Please enter the correct username and password";
					Sleep(2000);
					break;
				case 3://going back to main page
					flag = 1;
					break;
				default:
					cout << "\n\n\t\t\t\t   Please select from the options given above.\n" << endl;
					Sleep(2000);
					cin.clear();
					cin.ignore(1000, '\n');
					break;
				}
			}
			break;
		case 3://Hospital
			if (hospital.login())
			{
				hospital.setLocation(hospital.getName());
				while (1)
				{
					if (flag == 1) break;
					Menu::showHospitalMenu(hospital.getName());
					cin >> opt3;
					switch (opt3)
					{
					case 1://Update emergency status
						cout << "\n\nAll your orders are made according to respective Emergency Status. By default, the status is set OFF upon login\n";
						cout << "\n\nIf you wish to Switch it ON, enter 1. If you wish to Switch it OFF, enter 0\n";
						cout << "(If you accidentally enter anything other than 1 and 0, your previous emergency status will be retained unless you log out)\n";
						cout << "\n\nYour current Emergency Status is " << hospital.getEmergencyStatus();
						cout << "\nSwitch ON/OFF Emergency Status :  ";
						cin >> temp;
						hospital.setEmergencyStatus(temp);
						break;
					case 2:
						cout << "\n\n\t\t\t\tWhich Blood Group do you want? :  ";
						cin >> bloodgroup;
						hospital.setBloodGroup(bloodgroup);
						temp = hospital.getBlood().UpdateInfo(hospital.getBloodGroup());
						if (hospital.orderBlood(temp) == 1)
						{
							transport.dispatchOrder(hospital);
							Sleep(5000);
						}
						else Sleep(2000);
						break;
					case 3:
						flag = 1;
						cout << "\n\n\t\t\t\t\t         Logging Off...\n\n";
						Sleep(1000);
						break;
					default:
						cout << "\n\n\t\t\t\t   Please select from the options given above.\n" << endl;
						Sleep(2000);
						cin.clear();
						cin.ignore(1000, '\n');
						break;
					}
				}
			}
			else  cout << "\nLOGIN ERROR\nPlease check if you are affiliated with our blood bank(The Only Hospitals under our contract are Indus, Liaquat, Agha Khan)\nEnsure that you have entered the correct Spelling and Registration Number.";
			Sleep(5000);
			break;
		case 4:
			cout << "\t\t\t\t\t\t    Thank You!\n\n";
			exit(0);
			break;
		default:
			cout << "\n\n\t\t\t\t    Please select from the options given above.\n" << endl;
			Sleep(2000);
			cin.clear();
			cin.ignore(1000, '\n');
			break;
		}
	}
}