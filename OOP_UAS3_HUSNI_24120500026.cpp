#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstddef>

using namespace std;

class Team;
class Coach;
class Player;
class Staff;
class Match;
class Trainingsession;
class Menu;
class Dataset;
class Account;

class Venue {
private:
    string venueId;
    string venueName;
    string location;
    int capacity;

public:
    Venue(string id, string name, string loc, int cap)
        : venueId(id), venueName(name), location(loc), capacity(cap) {}

    string getVenueName() const { return venueName; }

    void displayVenueInfo() const {
        cout << "Venue ID: " << venueId << ", Name: " << venueName << ", Location: " << location << ", Capacity: " << capacity << endl;
    }
};

class Person {
protected:
    string name;
    string address;
    string dateOfBirth;
    string gender;
    string NoPhone;
    string Email;
    string Nationality;
    string IdCard;
    string TaxId;

public:
    Person(string name, string address, string dob, string gender,
           string phone, string email, string nationality,
           string idCard, string taxId)
        : name(name), address(address), dateOfBirth(dob), gender(gender),
          NoPhone(phone), Email(email), Nationality(nationality),
          IdCard(idCard), TaxId(taxId) {}

    virtual ~Person() = default;

    string getName() const { return name; }
    string getAddress() const { return address; }
    string getEmail() const { return Email; }

    virtual void displayDetails() const = 0;
};

class Coach : public Person {
private:
    string licenseNumber;
    string role;
    int yearsExperience;
    weak_ptr<Team> assignedTeam;

public:
    Coach(string name, string address, string dob, string gender,
          string phone, string email, string nationality,
          string idCard, string taxId,
          string license, string role, int experience)
        : Person(name, address, dob, gender, phone, email, nationality, idCard, taxId),
          licenseNumber(license), role(role), yearsExperience(experience) {}

    void setAssignedTeam(shared_ptr<Team> team) {
        assignedTeam = team;
    }

    void displayDetails() const override {
        cout << "Coach: " << name << ", Role: " << role << ", License: " << licenseNumber << ", Experience: " << yearsExperience << " years" << endl;
    }
};

class Player : public Person {
private:
    string playerNumber;
    string position;
    int goalsScored;
    int assistsMade;
    weak_ptr<Team> assignedTeam;

public:
    Player(string name, string address, string dob, string gender,
           string phone, string email, string nationality,
           string idCard, string taxId,
           string number, string pos, int goals, int assists)
        : Person(name, address, dob, gender, phone, email, nationality, idCard, taxId),
          playerNumber(number), position(pos), goalsScored(goals), assistsMade(assists) {}

    void setAssignedTeam(shared_ptr<Team> team) {
        assignedTeam = team;
    }
    string getPlayerNumber() const { return playerNumber; }

    void displayDetails() const override {
        cout << "Player: " << name << ", Number: " << playerNumber << ", Position: " << position
                     << ", Goals: " << goalsScored << ", Assists: " << assistsMade << endl;
    }
};

class Staff : public Person {
private:
    string StaffPosition;
    string Department;

public:
    Staff(string name, string address, string dob, string gender,
          string phone, string email, string nationality,
          string idCard, string taxId,
          string position, string dept)
        : Person(name, address, dob, gender, phone, email, nationality, idCard, taxId),
          StaffPosition(position), Department(dept) {}

    string getStaffPosition() const { return StaffPosition; }

    void displayDetails() const override {
        cout << "Staff: " << name << ", Position: " << StaffPosition << ", Department: " << Department << endl;
    }
};

class Team : public enable_shared_from_this<Team> {
private:
    string Clubname;
    string league;
    string Division;
    int founded;
    string Homebase;
    string ColorShirt;

    shared_ptr<Coach> headCoach;
    shared_ptr<Coach> assistantCoach;
    vector<shared_ptr<Player>> players;
    vector<shared_ptr<Staff>> staffMembers;

public:
    Team(string name,
         string league,
         string division,
         int year,
         string Homebase,
         string color)
        : Clubname(name), league(league), Division(division),
          founded(year), Homebase(Homebase),
          ColorShirt(color) {}

    string getClubname() const { return Clubname; }

    void setHeadCoach(shared_ptr<Coach> coach) {
        headCoach = coach;
        if (coach) coach->setAssignedTeam(shared_from_this());
    }

    void setAssistantCoach(shared_ptr<Coach> coach) {
        assistantCoach = coach;
        if (coach) coach->setAssignedTeam(shared_from_this());
    }

    void addPlayer(shared_ptr<Player> player) {
        if (players.size() < 15) {
            players.push_back(player);
            if (player) player->setAssignedTeam(shared_from_this());
        } else {
            cout << "Team already has 15 players. Cannot add more." << endl;
        }
    }

    void addStaff(shared_ptr<Staff> staff) {
        staffMembers.push_back(staff);
    }

    void displayTeamInfo() const {
        cout << "\n--- Team Info: " << Clubname << " ---" << endl;
        cout << "League: " << league << ", Division: " << Division << ", Founded: " << founded << endl;
        cout << "Homebase: " << Homebase << ", Color: " << ColorShirt << endl;

        if (headCoach) {
            cout << "  Head Coach: ";
            headCoach->displayDetails();
        }
        if (assistantCoach) {
            cout << "  Assistant Coach: ";
            assistantCoach->displayDetails();
        }
        cout << "  Players (" << players.size() << "):" << endl;
        for (const auto& player : players) {
            if (player) {
                cout << "    - ";
                player->displayDetails();
            }
        }
        cout << "  Staff Members (" << staffMembers.size() << "):" << endl;
        for (const auto& staff : staffMembers) {
            if (staff) {
                cout << "    - ";
                staff->displayDetails();
            }
        }
        cout << "-----------------------------------" << endl;
    }
};

class PersonFactory {
public:
    virtual shared_ptr<Person> createPerson(
        string name, string address, string dob, string gender,
        string phone, string email, string nationality,
        string idCard, string taxId) = 0;

    virtual ~PersonFactory() = default;
};

class CoachFactory : public PersonFactory {
public:
    shared_ptr<Person> createPerson(
        string name, string address, string dob, string gender,
        string phone, string email, string nationality,
        string idCard, string taxId) override {
        throw runtime_error("Use createCoach for CoachFactory to provide specific coach parameters.");
    }

    shared_ptr<Coach> createCoach(
        string name, string address, string dob, string gender,
        string phone, string email, string nationality,
        string idCard, string taxId,
        string license, string role, int experience) {
        return make_shared<Coach>(
            name, address, dob, gender, phone, email, nationality,
            idCard, taxId, license, role, experience);
    }
};

class PlayerFactory : public PersonFactory {
public:
    shared_ptr<Person> createPerson(
        string name, string address, string dob, string gender,
        string phone, string email, string nationality,
        string idCard, string taxId) override {
        throw runtime_error("Use createPlayer for PlayerFactory to provide specific player parameters.");
    }

    shared_ptr<Player> createPlayer(
        string name, string address, string dob, string gender,
        string phone, string email, string nationality,
        string idCard, string taxId,
        string number, string pos, int goals, int assists) {
        return make_shared<Player>(
            name, address, dob, gender, phone, email, nationality,
            idCard, taxId, number, pos, goals, assists);
    }
};

class StaffFactory : public PersonFactory {
public:
    shared_ptr<Person> createPerson(
        string name, string address, string dob, string gender,
        string phone, string email, string nationality,
        string idCard, string taxId) override {
        throw runtime_error("Use createStaff for StaffFactory to provide specific staff parameters.");
    }

    shared_ptr<Staff> createStaff(
        string name, string address, string dob, string gender,
        string phone, string email, string nationality,
        string idCard, string taxId,
        string role, string dept) {
        return make_shared<Staff>(
            name, address, dob, gender, phone, email, nationality,
            idCard, taxId, role, dept);
    }
};

class Match {
private:
    string matchId;
    string matchDate;
    string matchTime;
    string opponentTeam;
    string result;
    shared_ptr<Team> homeTeam;
    shared_ptr<Team> awayTeam;
    shared_ptr<Venue> matchVenue;

public:
    Match(string id, string date, string time, string opponent, string res)
        : matchId(id), matchDate(date), matchTime(time), opponentTeam(opponent), result(res) {}

    void setTeams(shared_ptr<Team> home, shared_ptr<Team> away) {
        homeTeam = home;
        awayTeam = away;
    }

    void setVenue(shared_ptr<Venue> venue) {
        matchVenue = venue;
    }

    void displayMatchInfo() const {
        cout << "Match ID: " << matchId << ", Date: " << matchDate << ", Opponent: " << opponentTeam << ", Result: " << result << endl;
        if (homeTeam) {
            cout << "  Home Team: " << homeTeam->getClubname() << endl;
        }
        if (awayTeam) {
            cout << "  Away Team: " << awayTeam->getClubname() << endl;
        }
        if (matchVenue) {
            cout << "  Venue: " << matchVenue->getVenueName() << endl;
        }
    }
};

class Trainingsession {
private:
    string sessionId;
    string sessionDate;
    string sessionTime;
    string focusArea;
    shared_ptr<Team> teamInTraining;
    shared_ptr<Venue> trainingVenue;

public:
    Trainingsession(string id, string date, string time, string focus)
        : sessionId(id), sessionDate(date), sessionTime(time), focusArea(focus) {}

    void setTeam(shared_ptr<Team> team) {
        teamInTraining = team;
    }

    void setVenue(shared_ptr<Venue> venue) {
        trainingVenue = venue;
    }

    void displayTrainingInfo() const {
        cout << "Training Session ID: " << sessionId << ", Date: " << sessionDate << ", Time: " << sessionTime << ", Focus: " << focusArea << endl;
        if (teamInTraining) {
            cout << "  Team: " << teamInTraining->getClubname() << endl;
        }
        if (trainingVenue) {
            cout << "  Venue: " << trainingVenue->getVenueName() << endl;
        }
    }
};

class Menu {
private:
    string menuId;
    string menuName;
    string description;
    vector<string> items;

public:
    Menu(string id, string name, string desc)
        : menuId(id), menuName(name), description(desc) {}

    void addItem(string item) {
        items.push_back(item);
    }

    void displayMenuInfo() const {
        cout << "Menu ID: " << menuId << ", Name: " << menuName << ", Description: " << description << endl;
        cout << "  Items:" << endl;
        for (const auto& item : items) {
            cout << "    - " << item << endl;
        }
    }
};

class Dataset {
private:
    string datasetId;
    string datasetName;
    string dataType;
    string lastUpdated;

public:
    Dataset(string id, string name, string type, string updated)
        : datasetId(id), datasetName(name), dataType(type), lastUpdated(updated) {}

    void displayDatasetInfo() const {
        cout << "Dataset ID: " << datasetId << ", Name: " << datasetName << ", Type: " << dataType << ", Last Updated: " << lastUpdated << endl;
    }
};

class Account {
private:
    string accountId;
    string username;
    string password;
    string accountType;

public:
    Account(string id, string user, string pw, string type)
        : accountId(id), username(user), password(pw), accountType(type) {}

    void displayAccountInfo() const {
        cout << "Account ID: " << accountId << ", Username: " << username << ", Type: " << accountType << endl;
    }
};

int main() {
    shared_ptr<Team> fcCakrawalaMuda = make_shared<Team>(
        "FC Cakrawala Muda", "Liga Mahasiswa", "U-23", 2020, "Stadion Cakrawala", "Biru-Putih");

    CoachFactory coachFactory;
    PlayerFactory playerFactory;
    StaffFactory staffFactory;

    shared_ptr<Coach> headCoach = coachFactory.createCoach(
        "Gianfranco Zola", "Jl. Bridge No. 25", "1980-01-01", "Male", "0810051905", "Zola@cakra.mail",
        "Indonesia", "3276010101800001", "1234567890", "A-License", "Head Coach", 10);
    shared_ptr<Coach> assistantCoach = coachFactory.createCoach(
        "Van Persie", "Jl. Maju No. 5", "1990-05-10", "Male", "08765432100", "Persie.p@cakra.mail",
        "Indonesia", "3276010510900001", "0987654321", "B-License", "Assistant Coach", 5);

    fcCakrawalaMuda->setHeadCoach(headCoach);
    fcCakrawalaMuda->setAssistantCoach(assistantCoach);

    for (int i = 1; i <= 15; ++i) {
        string playerName = "Player " + to_string(i);
        string playerNum = (i < 10 ? "0" : "") + to_string(i);
        string position = (i % 3 == 0) ? "Forward" : ((i % 2 == 0) ? "Midfielder" : "Defender");
        shared_ptr<Player> player = playerFactory.createPlayer(
            playerName, "Jl. Kampus No." + to_string(i), "2002-0" + to_string(i) + "-01", "Male",
            "081111111" + to_string(i), "player" + to_string(i) + "@cakrawala.ac.id",
            "Indonesia", "3276020" + to_string(i) + "0102000" + to_string(i), "111222333" + to_string(i),
            playerNum, position, 0, 0);
        fcCakrawalaMuda->addPlayer(player);
    }

    shared_ptr<Staff> physiotherapist = staffFactory.createStaff(
        "DR.OZ", "Jl. Angkasa No. 10", "1985-11-20", "Male", "08555555555", "budi.s@cakra.mail",
        "Indonesia", "3276031120850001", "4445556667", "Physiotherapist", "Medical");
    fcCakrawalaMuda->addStaff(physiotherapist);

    fcCakrawalaMuda->displayTeamInfo();

    shared_ptr<Venue> stadionCakrawala = make_shared<Venue>("V001", "Stadion Cakrawala", "Jakarta", 20000);
    stadionCakrawala->displayVenueInfo();

    shared_ptr<Match> match1 = make_shared<Match>("M001", "2025-07-20", "15:00", "ITL TRISAKTI", "Win");
    match1->setTeams(fcCakrawalaMuda, make_shared<Team>("ITL TRISAKTI", "Liga Mahasiswa", "U-23", 2020, "Stadion Sebelah", "Merah-Hitam"));
    match1->setVenue(stadionCakrawala);
    match1->displayMatchInfo();

    shared_ptr<Trainingsession> training1 = make_shared<Trainingsession>("T001", "2025-07-15", "09:00", "Passing Drills");
    training1->setTeam(fcCakrawalaMuda);
    training1->setVenue(stadionCakrawala);
    training1->displayTrainingInfo();

    shared_ptr<Menu> teamMenu = make_shared<Menu>("ME001", "Team Meal Plan", "Healthy and balanced meals for athletes.");
    teamMenu->addItem("Grilled Chicken ");
    teamMenu->addItem("Steamed Vegetables");
    teamMenu->addItem("Fruit Salad");
    teamMenu->displayMenuInfo();

    shared_ptr<Dataset> playerData = make_shared<Dataset>("DS001", "Player Statistics", "CSV", "2025-07-10");
    playerData->displayDatasetInfo();

    shared_ptr<Account> adminAccount = make_shared<Account>("ACC001", "admin_cakra", "secure_password123", "Administrator");
    adminAccount->displayAccountInfo();

    return 0;
}