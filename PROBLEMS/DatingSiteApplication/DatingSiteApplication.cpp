// LLD PROBLEM -->
// DATING SITE APPLICATION

//////////////////////////////////////////////////////////////////
// 					REQUIREMENTS 
//////////////////////////////////////////////////////////////////
/*

FUNCTIONAL REQUIREMENTS

- USER CAN SWIPE LEFT/RIGHT TO A PROFILE.
- USER CAN SETUP THEIR OWN PROFILE.
- USR CAN SET THEIR PEREFERENCES.
- ONCE THERE IS A MATCH THEY CAN CHAT IN CHATROOM.
- USER CAN SEE ALL THE PROFILES NEAR THEIR LOCATION.
  (NEARBY CAN BE BASED ON DIFFERENT STRATEGY)
- USER SHOULD GET NOTIFIED WHEN THERE IS A MATCH OR RECIEVE A NEW MESSAGE.
- USER MATCHING SHOULD BE BASED ON SEVERAL FACTORS & SCORES 
  (LIKE INTEREST MATCH, LOCATION MATCH, ETC)


NON-FUNCTIONAL REQUIREMENTS
- SCALABLE
- PLUG N PLAY
- FOLLOW SOLID PRINCIPLES
- LOOSLY COUPLED

DESIGN PATTERNS USED
- SINGLETON
- FACTORY
- STRATEGY
- FACADE
- NOTIFICATION
- CHAIN OF RESPONSIBILIES


*/
//////////////////////////////////////////////////////////////////

// #include "../../../builtin_files/bits-stdc++.h"
#include <bits/stdc++.h>
using namespace std;

// Forward declarations
class User;
class Profile;
class Message;
class ChatRoom;
class NotificationObserver;
class LocationService;

//////////////////////////////////////////////////////////////////
//                NOTIFICATION SERVICE
/////////////////////////////////////////////////////////////////

class NotificationObserver {
public:
  virtual void update(const string &message) = 0;
};

class UserNotificationObserver : public NotificationObserver {
private:
  string userId;

public:
  UserNotificationObserver(const string &userId) {
    this->userId = userId;
  }

  void update(const string &message) override {
    cout<< "Notification for user "<< userId <<" : " << message << endl;
  }
};


class NotificationService {
private:
  unordered_map<string, shared_ptr<NotificationObserver>> observers;

  static NotificationService *instance;
  static mutex mtx;
  NotificationService() {}

public:

    static NotificationService* getInstance() {
      if(instance == nullptr) {
        lock_guard<mutex> lock(mtx);
        if(instance == nullptr) {
          instance = new NotificationService();
        }
      }

      return instance;
    }

    void registerObserver(const string &userId, shared_ptr<NotificationObserver> observer) {
      observers[userId] = observer;
    }

    void removeObserver(const string &userId) {
      if(observers.find(userId) == observers.end())return;

      observers.erase(userId);
    }

    void notifyUser(const string &userId, const string &message) {
        if(observers.find(userId) == observers.end())return;

        observers[userId]->update(message);
    }


    void notifyAll(const string &message) {
        
        for(auto it : observers) {
          it.second->update(message);
        }
    }
};

NotificationService* NotificationService::instance = nullptr;
mutex NotificationService::mtx;

//////////////////////////////////////////////////////////////////
//                  BASIC MODEL
/////////////////////////////////////////////////////////////////

enum class Gender {
  MALE,
  FEMALE,
  NON_BINARY,
  OTHER
};


class Location {
private:
  double latitude;
  double longitude;

public:
   Location() {
    latitude = 0.0;
    longitude = 0.0;
   }

   Location(double longitute, double latitude) {
    this->latitude = latitude;
    this->longitude = longitute;
   }
   
   double getLatitude() const {
        return latitude;
    }
    
    double getLongitude() const {
        return longitude;
    }
    
    void setLatitude(double lat) {
        latitude = lat;
    }
    
    void setLongitude(double lon) {
        longitude = lon;
    }

   // Calculate distance in kilometers between two locations using Haversine formula
    double distanceInKm(const Location &other) const {
        const double earthRadiusKm = 6371.0;
        double dLat = (other.latitude - latitude) * M_PI / 180.0;
        double dLon = (other.longitude - longitude) * M_PI / 180.0;
        
        double a = sin(dLat/2) * sin(dLat/2) +
                   cos(latitude * M_PI / 180.0) * cos(other.latitude * M_PI / 180.0) *
                   sin(dLon/2) * sin(dLon/2);
        double c = 2 * atan2(sqrt(a), sqrt(1-a));
        return earthRadiusKm * c;
    }
};


class Interest {
private:
  string name;
  string category;

public:

  Interest(const string &name, const string &cat){
    this->name = name;
    this->category = cat;
  }

  string getName() const {
        return name;
    }
    
  string getCategory() const {
      return category;
  }
};


class Preference {
private:
  vector<Gender> interestedIn;
  int minAge;
  int maxAge;
  double maxDistance;
  vector<string> interests;

public:

  Preference() {
      minAge = 18;
      maxAge = 100;
      maxDistance = 100.0;
  }

  void addGenderPreference(Gender gender) {
      interestedIn.push_back(gender);
  }
  
  void removeGenderPreference(Gender gender) {
      interestedIn.erase(std::remove(interestedIn.begin(), interestedIn.end(), gender), interestedIn.end());
  }
  
  void setAgeRange(int min, int max) {
      minAge = min;
      maxAge = max;
  }
  
  void setMaxDistance(double distance) {
      maxDistance = distance;
  }
  
  void addInterest(const std::string& interest) {
      interests.push_back(interest);
  }
  
  void removeInterest(const std::string& interest) {
      interests.erase(std::remove(interests.begin(), interests.end(), interest), interests.end());
  }
  
  bool isInterestedInGender(Gender gender) const {
      return std::find(interestedIn.begin(), interestedIn.end(), gender) != interestedIn.end();
  }
  
  bool isAgeInRange(int age) const {
      return age >= minAge && age <= maxAge;
  }
  
  bool isDistanceAcceptable(double distance) const {
      return distance <= maxDistance;
  }
  
  const std::vector<std::string>& getInterests() const {
      return interests;
  }
  
  const std::vector<Gender>& getInterestedGenders() const {
      return interestedIn;
  }
  
  int getMinAge() const {
      return minAge;
  }
  
  int getMaxAge() const {
      return maxAge;
  }
  
  double getMaxDistance() const {
      return maxDistance;
  }
};

//////////////////////////////////////////////////////////////////
//                  MESSAGE SYSTEM
/////////////////////////////////////////////////////////////////

class Message {
private:
  string senderId;
  string content;
  time_t timestamp;

public:
  Message(const string& sender, const string& msg) {
      senderId = sender;
      content = msg;
      timestamp = time(nullptr);
  }

  string getSenderId() const {
      return senderId;
  }
  
  string getContent() const {
      return content;
  }
  
  time_t getTimestamp() const {
      return timestamp;
  }
  
  string getFormattedTime() const {
    struct tm* timeinfo;
    char buffer[80];
    
    timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
  }
};


class ChatRoom {
public:
  string id;
  vector<string>participantIds;
  vector<shared_ptr<Message>> messages;

public:

  ChatRoom(const string& roomId, const string& user1Id, const string& user2Id) {
      id = roomId;
      participantIds.push_back(user1Id);
      participantIds.push_back(user2Id);
  }

  string getId() const {
      return id;
  }
    
  void addMessage(const string& senderId, const string& content) {
      shared_ptr<Message> msg = make_shared<Message>(senderId, content);
      messages.push_back(msg);
  }
  
  bool hasParticipant(const string& userId) const {
      return find(participantIds.begin(), participantIds.end(), userId) != participantIds.end();
  }
  
  const vector<shared_ptr<Message>>& getMessages() const {
      return messages;
  }
  
  const vector<string>& getParticipants() const {
      return participantIds;
  }
  
  void displayChat() const {
      cout << "===== Chat Room: " << id << " =====" << endl;
      for (const auto& msg : messages) {
          cout << "[" << msg->getFormattedTime() << "] " 
               << msg->getSenderId() << ": " << msg->getContent() << endl;
      }
      cout << "=========================" << endl;
  }  
};


//////////////////////////////////////////////////////////////////
//                  USER PROFILE
/////////////////////////////////////////////////////////////////

class UserProfile {
private:
    string name;
    int age;
    Gender gender;
    string bio;
    vector<string> photos;
    vector<shared_ptr<Interest>> interests;
    Location location;
    
public:
    UserProfile() {
        name = "";
        age = 0;
        gender = Gender::OTHER;
    }
    
    void setName(const string& n) {
        name = n;
    }
    
    void setAge(int a) {
        age = a;
    }
    
    void setGender(Gender g) {
        gender = g;
    }
    
    void setBio(const string& b) {
        bio = b;
    }
    
    void addPhoto(const string& photoUrl) {
        photos.push_back(photoUrl);
    }
    
    void removePhoto(const string& photoUrl) {
        photos.erase(remove(photos.begin(), photos.end(), photoUrl), photos.end());
    }
    
    void addInterest(const string& name, const string& category) {
        shared_ptr<Interest> interest = make_shared<Interest>(name, category);
        interests.push_back(interest);
    }
    
    void removeInterest(const string& name) {
        auto it = find_if(interests.begin(), interests.end(), 
            [&name](const shared_ptr<Interest> interest) {
                return interest->getName() == name;
            });
        
        if (it != interests.end()) {
            interests.erase(it);
        }
    }
    
    void setLocation(const Location& loc) {
        location = loc;
    }
    
    string getName() const {
        return name;
    }
    
    int getAge() const {
        return age;
    }
    
    Gender getGender() const {
        return gender;
    }
    
    string getBio() const {
        return bio;
    }
    
    const vector<string>& getPhotos() const {
        return photos;
    }
    
    const vector<shared_ptr<Interest>>& getInterests() const {
        return interests;
    }
    
    const Location& getLocation() const {
        return location;
    }
    
    void display() const {
        cout << "===== Profile =====" << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: ";
        switch (gender) {
            case Gender::MALE: cout << "Male"; break;
            case Gender::FEMALE: cout << "Female"; break;
            case Gender::NON_BINARY: cout << "Non-binary"; break;
            case Gender::OTHER: cout << "Other"; break;
        }
        cout << endl;
        
        cout << "Bio: " << bio << endl;
        
        cout << "Photos: ";
        for (const auto& photo : photos) {
            cout << photo << ", ";
        }
        cout << endl;
        
        cout << "Interests: ";
        for (const auto& interest : interests) {
            cout << interest->getName() << " (" << interest->getCategory() << "), ";
        }
        cout << endl;
        
        cout << "Location: " << location.getLatitude() << ", " << location.getLongitude() << endl;
        cout << "===================" << endl;
    }
};

//////////////////////////////////////////////////////////////////
//                  USER
/////////////////////////////////////////////////////////////////

enum class SwipeAction {
LEFT,
RIGHT
};

class User {
private:
    string id;
    shared_ptr<UserProfile> profile;
    shared_ptr<Preference> preference;
    map<string, SwipeAction> swipeHistory; // userId -> action
    shared_ptr<NotificationObserver> notificationObserver;
    
public:
    User(const string& userId) {
        id = userId;
        profile = make_shared<UserProfile>();
        preference =  make_shared<Preference>();
        notificationObserver = make_shared<UserNotificationObserver>(userId);
        NotificationService::getInstance()->registerObserver(userId, notificationObserver);
    }
    
    string getId() const {
        return id;
    }
    
    shared_ptr<UserProfile> getProfile() {
        return profile;
    }
    
    shared_ptr<Preference> getPreference() {
        return preference;
    }
    
    void swipe(const string& otherUserId, SwipeAction action) {
        swipeHistory[otherUserId] = action;
    }
    
    bool hasLiked(const string& otherUserId) const {
        auto it = swipeHistory.find(otherUserId);
        if (it != swipeHistory.end()) {
            return it->second == SwipeAction::RIGHT;
        }
        return false;
    }
    
    bool hasDisliked(const string& otherUserId) const {
        auto it = swipeHistory.find(otherUserId);
        if (it != swipeHistory.end()) {
            return it->second == SwipeAction::LEFT;
        }
        return false;
    }
    
    bool hasInteractedWith(const string& otherUserId) const {
        return swipeHistory.find(otherUserId) != swipeHistory.end();
    }
    
    void displayProfile() const {  // Principle of least knowledge
        profile->display();
    }
};


//////////////////////////////////////////////////////////////////
//                  LOCATION SERVICE
//////////////////////////////////////////////////////////////////

class LocationStrategy {
public:
  virtual vector<shared_ptr<User>> findNearbyUsers(const Location& location, double maxDistance, const vector<shared_ptr<User>>& allUsers) = 0;
};

class BasicLocationStrategy : public LocationStrategy {
public:
    vector<shared_ptr<User>> findNearbyUsers(const Location& location, double maxDistance, const vector<shared_ptr<User>>& allUsers) override {
        vector<shared_ptr<User>>nearbyUsers;
        for (shared_ptr<User> user : allUsers) {
            double distance = location.distanceInKm(user->getProfile()->getLocation());
            if (distance <= maxDistance) {
                nearbyUsers.push_back(user);
            }
        }
        return nearbyUsers;
    }
};


// Location service with Strategy Pattern
class LocationService {
private:
    shared_ptr<LocationStrategy> strategy;
    
    // Singleton Pattern
    static LocationService* instance;
    
    LocationService() {
        strategy = make_shared<BasicLocationStrategy>();
    }
    
public:
    static LocationService* getInstance() {
        if (instance == nullptr) {
            instance = new LocationService();
        }
        return instance;
    }
    
    void setStrategy(shared_ptr<LocationStrategy> newStrategy) {
        strategy = newStrategy;
    }
    
    vector<shared_ptr<User>> findNearbyUsers(const Location& location, double maxDistance, const vector<shared_ptr<User>>& allUsers) {
        return strategy->findNearbyUsers(location, maxDistance, allUsers);
    }
};

// Initialize static member
LocationService* LocationService::instance = nullptr;

//////////////////////////////////////////////////////////////////
//                  MATCHING SYSTEM
//////////////////////////////////////////////////////////////////

enum class MatcherType {
    BASIC,
    INTERESTS_BASED,
    LOCATION_BASED
};

//Matcher interface
class Matcher {
public:
    virtual ~Matcher() {}
    virtual double calculateMatchScore(shared_ptr<User> user1, shared_ptr<User> user2) = 0;
};

// Concrete matcher: Basic matcher
class BasicMatcher : public Matcher {
public:
    double calculateMatchScore(shared_ptr<User> user1, shared_ptr<User> user2) override {
        // Basic scoring, just check if preferences align
        bool user1LikesUser2Gender = user1->getPreference()->isInterestedInGender(user2->getProfile()->getGender());
        bool user2LikesUser1Gender = user2->getPreference()->isInterestedInGender(user1->getProfile()->getGender());
        
        if (!user1LikesUser2Gender || !user2LikesUser1Gender) {
            return 0.0;
        }
        
        // Check age preference
        bool user1LikesUser2Age = user1->getPreference()->isAgeInRange(user2->getProfile()->getAge());
        bool user2LikesUser1Age = user2->getPreference()->isAgeInRange(user1->getProfile()->getAge());
        
        if (!user1LikesUser2Age || !user2LikesUser1Age) {
            return 0.0;
        }
        
        // Check distance preference
        double distance = user1->getProfile()->getLocation().distanceInKm(user2->getProfile()->getLocation());
        bool user1LikesUser2Distance = user1->getPreference()->isDistanceAcceptable(distance);
        bool user2LikesUser1Distance = user2->getPreference()->isDistanceAcceptable(distance);
        
        if (!user1LikesUser2Distance || !user2LikesUser1Distance) {
            return 0.0;
        }
        
        // If all basic criteria match, return a base score
        return 0.5; // 50% match
    }
};

    // Concrete matcher: Interests-based matcher
class InterestsBasedMatcher : public Matcher {
public:
    double calculateMatchScore(shared_ptr<User> user1, shared_ptr<User> user2) override {
        // First, check basic compatibility
        BasicMatcher basicMatcher;
        double baseScore = basicMatcher.calculateMatchScore(user1, user2);
        
        if (baseScore == 0.0) {
            return 0.0; // No need to continue if basic criteria don't match
        }
        
        // Calculate score based on shared interests
        std::vector<std::string> user1InterestNames;
        for (const auto& interest : user1->getProfile()->getInterests()) {
            user1InterestNames.push_back(interest->getName());
        }
        
        int sharedInterests = 0;
        for (const auto& interest : user2->getProfile()->getInterests()) {
            if (std::find(user1InterestNames.begin(), user1InterestNames.end(), interest->getName()) != user1InterestNames.end()) {
                sharedInterests++;
            }
        }
        
        // Bonus score based on shared interests (up to 0.5 additional points)
        double maxInterests = std::max(user1->getProfile()->getInterests().size(), user2->getProfile()->getInterests().size());
        double interestScore = maxInterests > 0 ? 0.5 * (sharedInterests / maxInterests) : 0.0;
        
        return baseScore + interestScore;
    }
};
    
// Concrete matcher: Location-based matcher
class LocationBasedMatcher : public Matcher {
public:
    double calculateMatchScore(shared_ptr<User> user1, shared_ptr<User> user2) override {
        // First, check basic compatibility
        InterestsBasedMatcher interestsMatcher;
        double baseScore = interestsMatcher.calculateMatchScore(user1, user2);
        
        if (baseScore == 0.0) {
            return 0.0; // No need to continue if basic criteria don't match
        }
        
        // Calculate score based on proximity
        double distance = user1->getProfile()->getLocation().distanceInKm(user2->getProfile()->getLocation());
        double maxDistance = std::min(user1->getPreference()->getMaxDistance(), user2->getPreference()->getMaxDistance());
        
        // Closer is better, score decreases with distance (up to 0.2 additional points)
        double proximityScore = maxDistance > 0 ? 0.2 * (1.0 - (distance / maxDistance)) : 0.0;
        
        return baseScore + proximityScore;
    }
};


// Factory Pattern: Matcher factory
class MatcherFactory {
public:
    static shared_ptr<Matcher> createMatcher(MatcherType type) {
        switch (type) {
            case MatcherType::BASIC:
                return make_shared<BasicMatcher>();
            case MatcherType::INTERESTS_BASED:
                return make_shared<InterestsBasedMatcher>();
            case MatcherType::LOCATION_BASED:
                return make_shared<LocationBasedMatcher>();
            default:
                return make_shared<BasicMatcher>();
        }
    }
};


//////////////////////////////////////////////////////////////////
//                  DATING APP
//////////////////////////////////////////////////////////////////

// Facade Pattern: Dating app system
class DatingApp {
private:
    vector<shared_ptr<User>> users;
    vector<shared_ptr<ChatRoom>> chatRooms;
    shared_ptr<Matcher> matcher;
    
    // Singleton Pattern
    static DatingApp* instance;
    
    DatingApp() {
        // Default to location-based matcher
        matcher = MatcherFactory::createMatcher(MatcherType::LOCATION_BASED);
    }
    
public:
    static DatingApp* getInstance() {
        if (instance == nullptr) {
            instance = new DatingApp();
        }
        return instance;
    }
        
    void setMatcher(MatcherType type) {
        matcher = MatcherFactory::createMatcher(type);
    }
    
    shared_ptr<User> createUser(const string& userId) {
        shared_ptr<User> user = make_shared<User>(userId);
        users.push_back(user);
        return user;
    }
    
    shared_ptr<User> getUserById(const string& userId) {
        for (auto user : users) {
            if (user->getId() == userId) {
                return user;
            }
        }
        return nullptr;
    }
    
    std::vector<shared_ptr<User>> findNearbyUsers(const std::string& userId, double maxDistance = 5.0) {
        shared_ptr<User> user = getUserById(userId);
        if (user == nullptr) {
            return vector<shared_ptr<User>>();
        }
        
        // Find users within maxDistance km
        vector<shared_ptr<User>> nearbyUsers = LocationService::getInstance()->findNearbyUsers(
            user->getProfile()->getLocation(), maxDistance, users);
        
        // Filter out the user themselves
        nearbyUsers.erase(remove(nearbyUsers.begin(), nearbyUsers.end(), user), nearbyUsers.end());
        
        // Filter out users that don't match preferences or have already been swiped
        vector<shared_ptr<User>> filteredUsers;
        for (shared_ptr<User> otherUser : nearbyUsers) {
            // Skip users that have already been interacted with
            if (user->hasInteractedWith(otherUser->getId())) {
                continue;
            }
            
            // Calculate match score
            double score = matcher->calculateMatchScore(user, otherUser);
            
            // If score is above 0, they meet basic preference criteria
            if (score > 0) {
                filteredUsers.push_back(otherUser);
            }
        }
        
        return filteredUsers;
    }
    
    bool swipe(const string& userId, const string& targetUserId, SwipeAction action) {
        shared_ptr<User> user = getUserById(userId);
        shared_ptr<User> targetUser = getUserById(targetUserId);
        
        if (user == nullptr || targetUser == nullptr) {
            cout << "User not found." << endl;
            return false;
        }
        
        user->swipe(targetUserId, action);
        
        // Check if it's a match
        if (action == SwipeAction::RIGHT && targetUser->hasLiked(userId)) {
            // It's a match!
            string chatRoomId = userId + "_" + targetUserId;
            shared_ptr<ChatRoom> chatRoom = make_shared<ChatRoom>(chatRoomId, userId, targetUserId);
            chatRooms.push_back(chatRoom);
            
            // Notify both users
            NotificationService::getInstance()->notifyUser(userId, "You have a new match with " + targetUser->getProfile()->getName() + "!");
            NotificationService::getInstance()->notifyUser(targetUserId, "You have a new match with " + user->getProfile()->getName() + "!");
            return true;
        }
        return false;
    }
    
    shared_ptr<ChatRoom> getChatRoom(const string& user1Id, const string& user2Id) {
        for (auto chatRoom : chatRooms) {
            if (chatRoom->hasParticipant(user1Id) && chatRoom->hasParticipant(user2Id)) {
                return chatRoom;
            }
        }
        return nullptr;
    }
    
    void sendMessage(const string& senderId, const string& receiverId, const string& content) {
        shared_ptr<ChatRoom> chatRoom = getChatRoom(senderId, receiverId);
        if (chatRoom == nullptr) {
            cout << "No chat room found between these users." << endl;
            return;
        }
        
        chatRoom->addMessage(senderId, content);
        
        // Notify the receiver
        NotificationService::getInstance()->notifyUser(receiverId, "New message from " + getUserById(senderId)->getProfile()->getName());
    }
    
    void displayUser(const string& userId) {
        shared_ptr<User> user = getUserById(userId);
        if (user == nullptr) {
            cout << "User not found." << endl;
            return;
        }
        
        user->displayProfile();
    }
    
    void displayChatRoom(const string& user1Id, const string& user2Id) {
        shared_ptr<ChatRoom> chatRoom = getChatRoom(user1Id, user2Id);
        if (chatRoom == nullptr) {
            cout << "No chat room found between these users." << endl;
            return;
        }
        
        chatRoom->displayChat();
    }
};

// Initialize static member
DatingApp* DatingApp::instance = nullptr;

// Main function
int main() {
    // Get the dating app instance
    DatingApp* app = DatingApp::getInstance();
    
    // Create users
    shared_ptr<User> user1 = app->createUser("user1");
    shared_ptr<User> user2 = app->createUser("user2");

    // Set user1 profile
    shared_ptr<UserProfile> profile1 = user1->getProfile();
    profile1->setName("Rohan");
    profile1->setAge(28);
    profile1->setGender(Gender::MALE);
    profile1->setBio("I am a software developer");
    profile1->addPhoto("rohan_photo1.jpg");
    profile1->addInterest("Coding", "Programming");
    profile1->addInterest("Travel", "Lifestyle");
    profile1->addInterest("Music", "Entertainment");
    
    // Setup user1 preferences
    shared_ptr<Preference> pref1 = user1->getPreference();
    pref1->addGenderPreference(Gender::FEMALE);
    pref1->setAgeRange(25, 30);
    pref1->setMaxDistance(10.0);
    pref1->addInterest("Coding");
    pref1->addInterest("Travel");
    
    // Setup user2 profile
    shared_ptr<UserProfile> profile2 = user2->getProfile();
    profile2->setName("Neha");
    profile2->setAge(27);
    profile2->setGender(Gender::FEMALE);
    profile2->setBio("Art teacher who loves painting and traveling.");
    profile2->addPhoto("neha_photo1.jpg");
    profile2->addInterest("Painting", "Art");
    profile2->addInterest("Travel", "Lifestyle");
    profile2->addInterest("Music", "Entertainment");
    
    // Setup user2 preferences
    shared_ptr<Preference> pref2 = user2->getPreference();
    pref2->addGenderPreference(Gender::MALE);
    pref2->setAgeRange(27, 30);
    pref2->setMaxDistance(15.0);
    pref2->addInterest("Coding");
    pref2->addInterest("Movies");

    // Set location for user1
    Location location1;
    location1.setLatitude(1.01);
    location1.setLongitude(1.02);
    profile1->setLocation(location1);
    
    // Set location for user2 (Close to user1, within 5km)
    Location location2;
    location2.setLatitude(1.03);
    location2.setLongitude(1.04);
    profile2->setLocation(location2);

    // Display user profiles
    std::cout << "---- User Profiles ----" << std::endl;
    app->displayUser("user1");
    app->displayUser("user2");
    
    // Find nearby users for user1 (within 5km)
    std::cout << "\n---- Nearby Users for user1 (within 5km) ----" << std::endl;
    std::vector<shared_ptr<User>> nearbyUsers = app->findNearbyUsers("user1", 5.0);
    std::cout << "Found " << nearbyUsers.size() << " nearby users" << std::endl;
    for (shared_ptr<User> user : nearbyUsers) {
        std::cout << "- " << user->getProfile()->getName() << " (" << user->getId() << ")" << std::endl;
    }
    
    // User1 swipes right on User2
    std::cout << "\n---- Swipe Actions ----" << std::endl;
    std::cout << "User1 swipes right on User2" << std::endl;
    app->swipe("user1", "user2", SwipeAction::RIGHT);
    
    // User2 swipes right on User1 (creating a match)
    std::cout << "User2 swipes right on User1" << std::endl;
    app->swipe("user2", "user1", SwipeAction::RIGHT);
    
    // Send messages in the chat room
    std::cout << "\n---- Chat Room ----" << std::endl;
    app->sendMessage("user1", "user2", "Hi Neha, Kaise ho?");
    
    app->sendMessage("user2", "user1", "Hi Rohan, Ma bdiya tum btao");
    
    // Display the chat room
    app->displayChatRoom("user1", "user2");
    
    return 0;
}
