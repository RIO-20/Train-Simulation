#include <bits/stdc++.h>
#include <vector>
#include <sstream>
#include <regex>
#include <ctime>
#include <stdexcept>
using namespace std;

// Functii ajutatoare

string getcurrDate() {
  time_t t = time(nullptr);

  tm* ct = localtime(&t);
  char curr_date[11];

  strftime(curr_date, sizeof(curr_date), "%d-%m-%Y", ct);

  return string(curr_date);
}

string getcurrHour(){

    time_t t = time(nullptr);

    tm* ch = localtime(&t);
    char curr_hour[6];

    strftime(curr_hour, sizeof(curr_hour), "%H:%M", ch);

    return string(curr_hour);

}

string getPassStrength(string pass) {
  int lett = 0, num = 0, ch = 0, up = 0, low = 0;

  int len = pass.length();

  if (len <= 6) return "weak";

  for (int i = 0; i < len; i++) {
    if (pass[i] >= 'a' && pass[i] <= 'z') {
      lett = 1;
      low = 1;
    } else if (pass[i] >= 'A' && pass[i] <= 'Z') {
      lett = 1;
      up = 1;
    } else if (pass[i] >= '0' && pass[i] <= '9')
      num = 1;
    else
      ch = 1;
  }

  if ((lett + num + ch + up + low >= 3) && (len >= 8))
    return "ok";
  else if ((lett + num + ch + up + low == 5) && (len >= 12))
    return "good";

  return "weak";
}


int checkInvalidTicket(string a){

    string list = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnoqrstuvwxyz0123456789-:";

    for(char ch: a){
        if(list.find(ch) != string::npos)
          return 0;
    }

    return 1;

}

int checkValidHour(string a){

    string aux = a;
    char *token = strtok(&aux[0], ":");

    if(token == nullptr)
      return 0;
    
    int h = atoi(token);

    token = strtok(nullptr,":");

    if(token == nullptr)
        return 0;
    
    int min = atoi(token);

    if(h < 0 || h >= 24)
        return 0;
    
    if(min < 0 || min >= 60)
      return 0;

    return 1;
  
}


int checkValidDay(string a){

    string aux = a;

    int flag = 0;
    char *token = strtok(&aux[0],"- ");
    char *tokenM = strtok(nullptr,"- ");
    char *tokenY = strtok(nullptr,"- ");

    if(token == nullptr || tokenM == nullptr || tokenY == nullptr)
        return 0;

    int d = atoi(token);
    int month = atoi(tokenM);
    int y = atoi(tokenY);



    if((d < 0 || d > 31) || (month < 0 || month > 12) || (y < 2024 || y > 2025))
        return 0;

    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
        if(d > 31)
            return 0;
    }

    else if(month == 4 || month == 6 || month == 9 || month == 11){
          if(d > 30)
            return 0;
    }

    else{
        if(d > 28)
          return 0;
    }  

    return 1;
}


// Am schimbat putin abordarea algoritmului de criptare Vigenere cypher

string encryptPass(const string& pass) {
  /*Am ales o lista de caractere pe care utilizatorul le poate folosi. La
   * algoritmul original de criptare aveam doar litere , ceea ce insemna ca
   * folosesc doar 26 de caractere. Dar cand vine vorba de criptarea unei parole
   * bazate pe acest algoritm am nevoie de mai multe caractere, asa ca am
   * schimbat putin formatul. Dar idea de baza e aceeasi.*/

  string list =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()"
      "+=[]:;<>.?/|~` ";
  int len = list.length();

  string key = "TR4*n3_!;]";
  string res;

  for (int i = 0; i < pass.length(); i++) {
    size_t pass_idx = list.find(pass[i]);
    size_t key_idx = list.find(key[i % key.length()]);

    size_t idx = (pass_idx + key_idx) % len;

    res += list[idx];
  }

  return res;
}

void printMenu() {
  cout << "---------------------------" << endl;
  cout << "Welcome to CFR-Trips" << endl
       << "Please choose an operation: " << endl;
  cout << "1.Sign in (As operator)" << endl;
  cout << "2.Sign in (As user)" << endl;
  cout << "3.Register account (only for users)" << endl;
  cout << "4.Log out" << endl;
  cout << "5.Exit" << endl;
}

class Operator {
 private:
  string user_id;
  string pass;

 public:
  Operator(const string& ui, const string& p) : user_id(ui), pass(p){};

  string getUserId() const { return user_id; }
  string getPass() const { return pass; }

  int LogIn_Op(const vector<Operator>& operators) {

    cout << "LogIn:"<<endl;
    cout << "User_name: ";
    cin >> user_id;
    cout << "Password: ";
    cin >> pass;

    for (const auto& op : operators) {
      if (user_id == op.getUserId() && encryptPass(pass) == op.getPass()) {
        return 1;
      }
    }

    return 0;
  }

  void addCourse(string src , string dest, string time , string hour,int seatsF, int seatsS,const string& filename ) {

      ofstream fout(filename,ios::app);

      if(!fout.is_open()){
          cout<<"Unable to open the file!"<<endl;
          return;
      }

     while(true){
      try{
        cout<<"Introduce the city from where the trip will start: "<<endl;
        cin>>src;

        if(checkInvalidTicket(src) == 1)
            throw runtime_error("Invalid city name.");

          break;
      }
      catch(const runtime_error& e){
          cout << e.what() << " Please try again." << endl;
      }
     }
    while(true){
        try{
          cout<<"Introduce the city from where the trip will end: "<<endl;
          cin>>dest;

          if (checkInvalidTicket(dest)) {
                throw runtime_error("Invalid city name.");
            }

          break;
        }
        catch(const runtime_error& e){
          cout << e.what() << " Please try again." << endl;
        }
    }
    
    while(true){
        try{
          cout<<"Introduce the time of the trip: "<<endl;
          cin>>time;

        try{
            if(checkValidDay(time) == 0){
              throw runtime_error("Invalid date.");
            }

            if (checkInvalidTicket(time)) { 
                throw runtime_error("Invalid input.");
            }

            try{

              int result = time.compare(getcurrDate());

              if(result < 0)
                  throw runtime_error("The date cannot be added because it is in the past!");
              else
                break;

            }

            catch (const runtime_error& e) {
              cout << e.what() << " Please try again." << endl;
            }

          }

          catch (const runtime_error& e) {
            cout << e.what() << " Please try again." << endl;
          }

        }
        catch (const runtime_error& e) {
            cout << e.what() << " Please try again." << endl;
        }
    }
    
    while(true){
        try{
          cout<<"Introduce the hour of the trip: "<<endl;
          cin>>hour;

            if (checkInvalidTicket(hour)) {
                throw runtime_error("Invalid input.");
            }

            try{

              if(checkValidHour(hour) == 0)
                 throw runtime_error("Invalid hour.");

              try{

                int result = time.compare(getcurrDate());
                int result1 = hour.compare(getcurrHour());

                if(result == 0 && result1 < 0)
                  throw runtime_error("The hour cannot be added because it is in the past!");
                else
                  break;
              }

              catch (const runtime_error& e) {
                cout << e.what() << " Please try again." << endl;
              }
            }
            
          catch (const runtime_error& e) {
                cout << e.what() << " Please try again." << endl;
          }
        } catch (const runtime_error& e) {
            cout << e.what() << " Please try again." << endl;
        }
    }

    while(true){
      try{
        cout<<"Introduce the number of seats in the first class: "<<endl;
        cin>>seatsF;

        if(seatsF <= 0)
          throw runtime_error("You cannot introduce negative or null number of seats!");
        break;
      }
      catch (const runtime_error& e) {
        cout << e.what() << " Please try again." << endl;
      }
  }


    while(true){
      try{
        cout<<"Introduce the number of seats in the second class: "<<endl;
        cin>>seatsS;

        if(seatsS <= 0)
          throw runtime_error("You cannot introduce negative or null number of seats!");

          break;
      }
      catch (const runtime_error& e) {
          cout << e.what() << " Please try again." << endl;
      }
    }

    fout<<src<<","<<dest<<","<<time<<","<<hour<<","<<seatsF<<","<<seatsS<<endl;
    fout.close();

    cout<<"Course from "<<src<<" to "<<dest<<" has been added succesfully!"<<endl;

  }

  void removeCourse(string src , string dest, string time , string hour,const string& filename){

      cout<<"Enter course details that you want to remove: "<<endl;
      
      cout<<"Introduce the city from where the trip will start"<<endl;
      while(true){
        cin>>src;
        if(!checkInvalidTicket(src))
            break;
          cout << "Invalid city name. Try again." << endl;
      } 

       cout<<"Introduce the city from where the trip will end"<<endl;
      while(true){
        cin>>dest;
        if(!checkInvalidTicket(dest))
            break;
          cout << "Invalid city name. Try again." << endl;
      }

      cout<<"Introduce the day for the trip: "<<endl;
      while(true){
        cin>>time;
        int result = time.compare(getcurrDate());
        if(!checkInvalidTicket(time) &&  result >= 0 && checkValidDay(time))
            break;
         cout << "Invalid date or past date. Try again." << endl;
      }

      cout<<"Introduce the hour for the trip: "<<endl;
      while(true){
        cin>>hour;
        

        if(!checkInvalidTicket(time) && checkValidHour(hour))
            break;
         cout << "Invalid or past hour. Try again." << endl;
      }

      ifstream fin(filename);
      vector<string> lines;
      string line;

      int removed = 0;
      while (getline(fin, line)) {
            stringstream ss(line);

            string fileSrc, fileDest, fileTime, fileHour;
            getline(ss, fileSrc, ',');
            getline(ss, fileDest, ',');
            getline(ss, fileTime, ',');
            getline(ss, fileHour, ',');

            if (fileSrc == src && fileDest == dest && fileTime == time && fileHour == hour) {
                removed = 1; 
            } else {
                lines.push_back(line);
            }
        }
        fin.close();

        if (!removed) {
            cout << "Course not found!" << endl;
            return;
        }

        ofstream fout(filename, ios::trunc);
        for (const auto& l : lines) {
            fout << l << endl;
        }
        fout.close();
        cout << "Course removed successfully!" << endl;

  }


  void showAllCourses(const string& filename){

     
      ifstream fin(filename);
      vector<string> lines;
      string line;
      
      if (!fin) {
            cerr << "Error: Unable to open file!" << endl;
            return;
      }
      
      int idx = 1;
      cout<<"All the available courses are: "<<endl;
      while (getline(fin, line)) {
            stringstream ss(line);

            string fileSrc, fileDest, fileTime, fileHour, filesF,filesS;
            getline(ss, fileSrc, ',');
            getline(ss, fileDest, ',');
            getline(ss, fileTime, ',');
            getline(ss, fileHour, ',');
            getline(ss,filesF,',');
            getline(ss,filesS,',');

            cout<<idx<<" "<<fileSrc<<"-"<<fileDest<<" Date: "<<fileTime<<" Hour: "<<fileHour<<" Seats first class: "<<filesF<<" Seats second class: "<<filesS<<endl;

            idx++;
        }
        fin.close();
  }

  
};



class User {
 private:
  string email;
  string pass;

 public:
  User(const string& email, const string& pass)
      : email(email), pass(pass) {}

  string getUserEm() const { return email; }
  string getPass() const { return pass; }

  bool isValidEmail(const string& email) {
    const regex pattern(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");
    return regex_match(email, pattern);
  }
  

   static void saveUsersToFile(const vector<User>& users, const string& filename) {
        ofstream outFile(filename, ios::app);  
        if (!outFile) {
            cout << "Error opening file for writing!" << endl;
            return;
        }

        
        for (const auto& user : users) {
            outFile << user.getUserEm() << "," << user.getPass()<< endl;
        }

        outFile.close();
    }

  int LogIn_Op(const vector<User>& users) {
    cout << "LogIn:" << endl;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> pass;

    for (const auto& us : users) {
      if (email == us.getUserEm() && pass == us.getPass()) {
        return 1;
      }
    }

    return 0;
  }

  void createAccount(vector<User>& users, const string& filename) {
    string new_email, new_pass, pass_aux;

    while (true) {
        try {
            cout << "Enter new email: ";
            cin >> new_email;

            if (!isValidEmail(new_email))
                throw runtime_error("You introduced a wrong email!");

            break;
        } catch (const runtime_error& e) {
            cout << e.what() << " Please try again." << endl;
        }
    }

    for (const auto& e : users) {
        if (e.getUserEm() == new_email) {
            cout << "Email already exists. Please try a different one." << endl;
            return;
        }
    }

    while (true) {
        try {
            cout << "Enter new password: ";
              cin.ignore();  
              getline(cin, new_pass);

            string strength = getPassStrength(new_pass);
            if (strength == "weak")
                throw runtime_error("The password that you introduced is weak!");

            cout << "Please reintroduce the password: ";
            getline(cin, pass_aux);

            if (new_pass != pass_aux)
                throw runtime_error("The passwords do not match!");

            break;
        } catch (const runtime_error& e) {
            cout << e.what() << " Please try again." << endl;
        }
    }

    string encrypted_pass = encryptPass(new_pass);

    
    User newUser(new_email, new_pass);
    users.push_back(newUser);  
    cout << "Account created successfully!" << endl;

    saveUsersToFile(users, filename);
  }

   void findCourse(string src, string dest, string time, string hour, const string& filename) {
    cout << "Enter course details to search: " << endl;

    cout << "Introduce the city from where the trip will start" << endl;
    while (true) {
        cin >> src;
        if (!checkInvalidTicket(src))
            break;
        cout << "Invalid city name. Try again." << endl;
    }

    cout << "Introduce the city from where the trip will end" << endl;
    while (true) {
        cin >> dest;
        if (!checkInvalidTicket(dest))
            break;
        cout << "Invalid city name. Try again." << endl;
    }

    cout << "Introduce the day for the trip: " << endl;
    while (true) {
        cin >> time;
        int result = time.compare(getcurrDate());
        if (!checkInvalidTicket(time) && result >= 0 && checkValidDay(time))
            break;
        cout << "Invalid date or past date. Try again." << endl;
    }

    cout << "Introduce the hour for the trip: " << endl;
    while (true) {
        cin >> hour;

        if (!checkInvalidTicket(time) && checkValidHour(hour))
            break;
        cout << "Invalid or past hour. Try again." << endl;
    }

    ifstream fin(filename);
    string line;
    bool found = false;

    while (getline(fin, line)) {
        stringstream ss(line);

        string fileSrc, fileDest, fileTime, fileHour;
        int fileSeatsFirst, fileSeatsSecond;
        getline(ss, fileSrc, ',');
        getline(ss, fileDest, ',');
        getline(ss, fileTime, ',');
        getline(ss, fileHour, ',');
        ss >> fileSeatsFirst; // Read first class seats
        ss.ignore(); // Ignore the comma
        ss >> fileSeatsSecond; // Read second class seats

        if (fileSrc == src && fileDest == dest && fileTime == time && fileHour == hour) {
            found = true;
            cout << "Course found: " << endl;
            cout << "Source: " << fileSrc << ", Destination: " << fileDest 
                 << ", Date: " << fileTime << ", Hour: " << fileHour << endl;
            cout << "Available seats in First Class: " << fileSeatsFirst << endl;
            cout << "Available seats in Second Class: " << fileSeatsSecond << endl;
        }
    }
    fin.close();

    if (!found) {
        cout << "Course not found!" << endl;
    }
  }


 
   void showAllCourses(const string& filename){

     
      ifstream fin(filename);
      vector<string> lines;
      string line;
      
      if (!fin) {
            cerr << "Error: Unable to open file!" << endl;
            return;
      }
      
      int idx = 1;
      cout<<"All the available courses are: "<<endl;
      while (getline(fin, line)) {
            stringstream ss(line);

            string fileSrc, fileDest, fileTime, fileHour, filesF,filesS;
            getline(ss, fileSrc, ',');
            getline(ss, fileDest, ',');
            getline(ss, fileTime, ',');
            getline(ss, fileHour, ',');
            getline(ss,filesF,',');
            getline(ss,filesS,',');

            cout<<idx<<" "<<fileSrc<<"-"<<fileDest<<" Date: "<<fileTime<<" Hour: "<<fileHour<<" Seats first class: "<<filesF<<" Seats second class: "<<filesS<<endl;

            idx++;
        }
        fin.close();
  }

  void reserveCourse(string src, string dest, string time, string hour, const string& filename) {
        cout << "Enter course details to reserve: " << endl;

        
        cout << "Introduce the city from where the trip will start" << endl;
        while (true) {
            cin >> src;
            if (!checkInvalidTicket(src))
                break;
            cout << "Invalid city name. Try again." << endl;
        }

        cout << "Introduce the city from where the trip will end" << endl;
        while (true) {
            cin >> dest;
            if (!checkInvalidTicket(dest))
                break;
            cout << "Invalid city name. Try again." << endl;
        }

        cout << "Introduce the day for the trip: " << endl;
        while (true) {
            cin >> time;
            int result = time.compare(getcurrDate());
            if (!checkInvalidTicket(time) && result >= 0 && checkValidDay(time))
                break;
            cout << "Invalid date or past date. Try again." << endl;
        }

        cout << "Introduce the hour for the trip: " << endl;
        while (true) {
            cin >> hour;
            if (!checkInvalidTicket(time) && checkValidHour(hour))
                break;
            cout << "Invalid or past hour. Try again." << endl;
        }

        ifstream fin(filename);
        string line;
        bool found = false;
        vector<string> lines;  
        stringstream updatedLine;  

        while (getline(fin, line)) {
            stringstream ss(line);

            string fileSrc, fileDest, fileTime, fileHour;
            int fileSeatsFirst, fileSeatsSecond;
            getline(ss, fileSrc, ',');
            getline(ss, fileDest, ',');
            getline(ss, fileTime, ',');
            getline(ss, fileHour, ',');
            ss >> fileSeatsFirst;  
            ss.ignore();           
            ss >> fileSeatsSecond; 

           
            if (fileSrc == src && fileDest == dest && fileTime == time && fileHour == hour) {
                found = true;
                cout << "Course found: " << endl;
                cout << "Source: " << fileSrc << ", Destination: " << fileDest 
                    << ", Date: " << fileTime << ", Hour: " << fileHour << endl;
                cout << "Available seats in First Class: " << fileSeatsFirst << endl;
                cout << "Available seats in Second Class: " << fileSeatsSecond << endl;

                
                cout << "Which class would you like to reserve? (1 for First Class, 2 for Second Class): ";
                int choice;
                cin >> choice;

                while(true){
                    
                    seat:
                        if(fileSeatsFirst == 0 && fileSeatsSecond == 0){
                          cout<<"There are no available tickets!"<<endl;
                          break;
                        }


                        if (choice == 1) {
                            if (fileSeatsFirst > 0) {
                                fileSeatsFirst--;
                                cout << "First Class seat reserved!" << endl;
                                break;
                            } else {
                                cout << "Sorry, no available seats in First Class!" << endl;
                                
                                if(fileSeatsSecond > 0){
                                  char c;
                                  cout<<"Do you want seats at the second clas?";
                                  cin>>c;

                                  if(c == 'y' || c == 'Y'){
                                    fileSeatsSecond--;
                                    cout << "Second Class seat reserved!" << endl;
                                    break;
                                  }
                                }
                            }
                        } else if (choice == 2) {
                            if (fileSeatsSecond > 0) {
                                fileSeatsSecond--;
                                break;
                                cout << "Second Class seat reserved!" << endl;
                            } else {
                                cout << "Sorry, no available seats in Second Class!" << endl;

                                if(fileSeatsFirst > 0){
                                  
                                  char c;
                                  cout<<"Do you want seats at the first clas?";
                                  cin>>c;

                                  if(c == 'y' || c == 'Y'){
                                    fileSeatsFirst--;
                                    cout << "First Class seat reserved!" << endl;
                                    break;
                                  }
                                  else
                                    goto seat;
                                }

                            }
                        } else {
                            cout << "Invalid class choice!" << endl;
                        }
                }

               
                updatedLine.str(""); 
                updatedLine.clear();
                updatedLine << fileSrc << "," << fileDest << "," << fileTime << "," << fileHour << ","
                            << fileSeatsFirst << "," << fileSeatsSecond;

                
                lines.push_back(updatedLine.str());
            } else {
                lines.push_back(line); 
            }
        }
        fin.close();

        if (!found) {
            cout << "Course not found!" << endl;
            return;
        }

        
        ofstream fout(filename, ios::trunc);
        for (const auto& l : lines) {
            fout << l << endl;
        }
        fout.close();
  }

};


// Functii care se ocupa cu operatii de fisiere

vector<Operator> loadOperatorsFromFile(const string& filename) {
  vector<Operator> operators;
  ifstream file(filename);

  if (!file) {
    cerr << "Error: Cannot open file!" << endl;
    return operators;
  }

  string line;

  while (getline(file, line)) {
    stringstream ss(line);
    string stored_user, stored_pass;
    getline(ss, stored_user, ',');
    getline(ss, stored_pass, ',');
    operators.emplace_back(stored_user, encryptPass(stored_pass));
  }

  file.close();
  return operators;
}

vector<User> loadUsersFromFile(const string& filename) {
  vector<User> users;
  ifstream file(filename);

  if (!file) {
    cerr << "Error: Cannot open file!" << endl;
    return users;
  }

  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    string stored_user, stored_pass;
    getline(ss, stored_user, ',');
    getline(ss, stored_pass, ',');
    users.emplace_back(stored_user, stored_pass);
  }

  file.close();
  return users;
}

int main(void) {

  vector<Operator> operators = loadOperatorsFromFile("operators.csv");
  vector<User> users = loadUsersFromFile("users.csv");


  int choice;
  printMenu();

  while (true) {
    cin >> choice;

       switch (choice) {
            case 1: {
                int tries = 3;
                Operator aux("", "");

                try {
                    while (tries > 0) {
                        try {
                            cout << "Please introduce your credentials:" << endl;

                            if (!aux.LogIn_Op(operators)) {
                                tries--;
                                if (tries == 0) {
                                    throw 0; 
                                }
                                throw tries;  
                            }

                           
                            int c;
                            cout << "Welcome!" << endl;
                            while (true) {
                                cout << "Please choose the following operations:" << endl;
                                cout << "1. Add courses" << endl;
                                cout << "2. Remove courses" << endl;
                                cout<<"3.Show available courses"<<endl;
                                cout<<"4.Log out"<<endl;
                                cout << "5. Exit" << endl;
                                cin >> c;

                                if (c == 1) {
                                    string src,dest,time,hour;
                                    int sF,sS;
                                    aux.addCourse(src,dest,time,hour,sF,sS,"courses.csv");

                                } else if (c == 2) {
                                    string src,dest,time,hour;
                                    aux.removeCourse(src,dest,time,hour,"courses.csv");
                                } 
                                else if(c == 3){
                                  aux.showAllCourses("courses.csv");
                                } 
                                else if(c == 4){
                                  char alg;
                                  cout<<"Do you want to continue? Y/N"<<endl;

                                  cin>>alg;

                                  if(alg == 'y' || alg == 'Y')
                                    break;
                                  else if(alg == 'n' || alg == 'N');
                                    goto finish;                                    

                                }
                                else if (c == 5) {
                                    goto finish;
                                } else {
                                    cout << "Invalid choice, please try again." << endl;
                                }
                            }
                            break; 
                        } catch (int a) {
                            if (a > 0) {
                                cout << "Incorrect user or password!" << endl;
                                cout << "You have " << a << " tries left!" << endl;
                            }
                            if (a == 0) {  
                                throw;
                            }
                        }
                    }
                } catch (int t) {  
                    cout << "Maximum login attempts reached. Please try again later!" << endl;
                }

                break;
            }
            case 2:{
                cout << "Please introduce your credentials for case 2:" << endl;
                int tries = 3;
                User aux("","");

                    try {
                    while (tries > 0) {
                        try {
                            cout << "Please introduce your credentials:" << endl;

                            if (!aux.LogIn_Op(users)) {
                                tries--;
                                if (tries == 0) {
                                    throw 0; 
                                }
                                throw tries;  
                            }

                           
                            int c;
                            cout << "Welcome!" << endl;
                            while (true) {
                                cout << "Please choose the following operations:" << endl;
                                cout << "1. Search courses" << endl;
                                cout << "2. Reserve courses" << endl;
                                cout<<"3.Show available courses"<<endl;
                                cout<<"4.Log out"<<endl;
                                cout << "5. Exit" << endl;
                                cin >> c;

                                if (c == 1) {
                                    string src,dest,time,hour;
                                    int sF,sS;
              
                                  aux.findCourse(src,dest,time,hour,"courses.csv");
                                     
                                } else if (c == 2) {
                                    string src,dest,time,hour;
                                    aux.reserveCourse(src,dest,time,hour,"courses.csv");
                                } 
                                else if(c == 3){
                                  aux.showAllCourses("courses.csv");
                                } 
                                else if(c == 4){
                                  char alg;
                                  cout<<"Do you want to continue? Y/N"<<endl;

                                  cin>>alg;

                                  if(alg == 'y' || alg == 'Y')
                                    break;
                                  else if(alg == 'n' || alg == 'N');
                                    goto finish;                                    

                                }
                                else if (c == 5) {
                                    goto finish;
                                } else {
                                    cout << "Invalid choice, please try again." << endl;
                                }
                            }
                            break; 
                        } catch (int a) {
                            if (a > 0) {
                                cout << "Incorrect user or password!" << endl;
                                cout << "You have " << a << " tries left!" << endl;
                            }
                            if (a == 0) {  
                                throw;
                            }
                        }
                    }
                } catch (int t) {  
                    cout << "Maximum login attempts reached. Please try again later!" << endl;
                }

                break;
            }
            case 3:{
                cout << "Please introduce your new data for the account:" << endl;
                User aux("","");
                aux.createAccount(users,"users.csv");
                break;
            }
            case 4:
                cout << "You've successfully logged out." << endl;
                cout<<"Do you want to continue? Y/N"<<endl;

                char alg;
                cin>>alg;

                if(alg == 'n' || alg == 'N')
                    goto finish;
                
                break;
            case 5:
                cout << "Thank you for using CFR-Trips!" << endl;
                return 0;  
            default:
                cout << "Please introduce one of the options above." << endl;
                break;
        }

        printMenu();
    }

  finish:
    return 0;
}