/*
BUILD A MUSIC PLAYER SYSTEM 

FUNCTIONAL REQUIREMENTS:

- USER CAN PLAY, PAUSE SONGS.
- USER CAN CREATE PLAYLIST, ADD SONGS TO PL,
  PLAY ENTIRE PLAYLIST (SEQUENTIAL MANNER, RANDOM MANNER, CUSTOM, etc)
- APP SHOULD SUPPORT MULTIPLE OUTPUT DEVICES ( BLUETOOTH SPEAKER, 
	WIRER SPEAKER, HEADPHONES, etc)


NON-FUNCTIONAL REQUIREMENTS:

- ENTIRE DESIGN SHOULD BE EASILY SCALABLE.
- NEW FEATURE (NEW OUTPUT DEVICE, NEW WAY OF PLAYING SONGS FROM PLAYLIST, ETC)
	CAN BE EASILY ADDED.


DESIGN PATTERNS USED
- STRATEGY
- SINGLETON
- FACTORY
- ADAPTER
- FACADE

*/

#include <bits/stdc++.h>
// #include "../../../builtin_files/bits-stdc++.h"
using namespace std;

/////////////////////////////////////////////////
//          ENUMS
////////////////////////////////////////////////

enum class DeviceType { 
    BLUETOOTH, 
    WIRED,
    HEADPHONES 
};

enum class PlayStrategyType { 
    SEQUENTIAL, 
    RANDOM, 
    CUSTOM_QUEUE 
};


/////////////////////////////////////////////////
//           SONG
////////////////////////////////////////////////

class Song {
private:
    string title;
    string artist;
    string filePath;
public:
    Song(string t, string a, string f) {
        title = t;
        artist = a;
        filePath = f;
    }
    string getTitle() { 
        return title; 
    }
    string getArtist() { 
        return artist;      
    }
    string getFilePath() { 
        return filePath;  
    }
};


/////////////////////////////////////////////////
//          PLAYLIST
////////////////////////////////////////////////

class Playlist {
private:
    string playlistName;
    vector<shared_ptr<Song>> songList;
public:
    Playlist(string name) {
        playlistName = name;
    }
    string getPlaylistName() {
        return playlistName;
    }
    const vector<shared_ptr<Song>> getSongs() {
        return songList;
    }
    int getSize() {
        return (int)songList.size();
    }
    void addSongToPlaylist(shared_ptr<Song> song) {
        if (song == nullptr) {
            throw runtime_error("Cannot add null song to playlist.");
        }
        songList.push_back(song);
    }
};

/////////////////////////////////////////////////
//          PLAYLIST STRATEGY
////////////////////////////////////////////////

class PlayStrategy {
public:
    virtual ~PlayStrategy() {}
    virtual void setPlaylist(shared_ptr<Playlist> playlist) = 0;
    virtual	shared_ptr<Song> next() = 0;
    virtual bool hasNext() = 0;
    virtual	shared_ptr<Song> previous() = 0;
    virtual bool hasPrevious() = 0;
    virtual void addToNext (shared_ptr<Song> song) {}
};


class SequentialPlayStrategy : public PlayStrategy {
private:
    shared_ptr<Playlist> currentPlaylist;
    int currentIndex;
public:
    SequentialPlayStrategy() {
        currentPlaylist = nullptr;
        currentIndex = -1;
    }

    void setPlaylist(shared_ptr<Playlist> playlist) override {
        currentPlaylist = playlist;
        currentIndex = -1;
    }

    bool hasNext() override {
        return ((currentIndex + 1) < currentPlaylist->getSize());
    }

    // Next in Loop
    shared_ptr<Song> next() override {
        if (!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }
        currentIndex = currentIndex + 1;
        return currentPlaylist->getSongs()[currentIndex];
    }

    bool hasPrevious() override {
        return (currentIndex - 1 > 0);
    }

    // previous in Loop
    shared_ptr<Song> previous() override {
        if (!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }
        currentIndex = currentIndex - 1;
        return currentPlaylist->getSongs()[currentIndex];
    }
};


class RandomPlayStrategy : public PlayStrategy {
private:
    shared_ptr<Playlist> currentPlaylist;
    vector<shared_ptr<Song>> remainingSongs; 
    stack<shared_ptr<Song>> history; 

public:
    RandomPlayStrategy() {
        currentPlaylist = nullptr;
        srand((unsigned)time(nullptr));
    }

    void setPlaylist(shared_ptr<Playlist> playlist) override {
        currentPlaylist = playlist;
        if (!currentPlaylist || currentPlaylist->getSize() == 0) return;

        remainingSongs = currentPlaylist->getSongs();
        history = stack<shared_ptr<Song>>(); 
    }

    bool hasNext() override {
        return currentPlaylist && !remainingSongs.empty();
    }

    // Next in Loop
    shared_ptr<Song> next() override {
        if (!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }
        if (remainingSongs.empty()) {
            throw runtime_error("No songs left to play");
        }

        int idx = rand() % remainingSongs.size();
        shared_ptr<Song> selectedSong = remainingSongs[idx];

        // Remove the selectedSong from the list. (Swap and pop to remove in O(1))
        swap(remainingSongs[idx], remainingSongs.back());
        remainingSongs.pop_back();

        history.push(selectedSong);
        return selectedSong;
    }

    bool hasPrevious() override {
        return history.size() > 0;
    }

    shared_ptr<Song> previous() override {
        if (history.empty()) {
            throw std::runtime_error("No previous song available.");
        }

        shared_ptr<Song> song = history.top();
        history.pop();
        return song;
    }
};

class CustomQueueStrategy : public PlayStrategy {
private:
    shared_ptr<Playlist> currentPlaylist;
    int currentIndex;
    queue<shared_ptr<Song>> nextQueue;
    stack<shared_ptr<Song>> prevStack;

   	shared_ptr<Song> nextSequential() {
        if (currentPlaylist->getSize() == 0) {
            throw runtime_error("Playlist is empty.");
        }
        currentIndex = currentIndex + 1;
        return currentPlaylist->getSongs()[currentIndex];
    }

   	shared_ptr<Song> previousSequential() {
        if (currentPlaylist->getSize() == 0) {
            throw runtime_error("Playlist is empty.");
        }
        currentIndex = currentIndex - 1;
        return currentPlaylist->getSongs()[currentIndex];
    }

public:
    CustomQueueStrategy() {
        currentPlaylist = nullptr;
        currentIndex = -1;
    }

    void setPlaylist(shared_ptr<Playlist> playlist) override {
        currentPlaylist = playlist;
        currentIndex = -1;
        while (!nextQueue.empty()) {
            nextQueue.pop();
        }
        while(!prevStack.empty()) {
            prevStack.pop();
        }
    }

    bool hasNext() override {
        return ((currentIndex + 1) < currentPlaylist->getSize());
    }

   	shared_ptr<Song> next() override {
        if (!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }

        if (!nextQueue.empty()) {
           	shared_ptr<Song> s = nextQueue.front();
            nextQueue.pop();
            prevStack.push(s);

            // update index to match queued song
            auto& list = currentPlaylist->getSongs();
            for (int i = 0; i < (int)list.size(); ++i) {
                if (list[i] == s) {
                    currentIndex = i;
                    break;
                }
            }
            return s;
        }

        // Otherwise sequential
        return nextSequential();
    }

    bool hasPrevious() override {
        return (currentIndex - 1 > 0);
    }

   	shared_ptr<Song> previous() override {
        if (!currentPlaylist || currentPlaylist->getSize() == 0) {
            throw runtime_error("No playlist loaded or playlist is empty.");
        }

        if (!prevStack.empty()) {
           	shared_ptr<Song> s = prevStack.top();
            prevStack.pop();

            // update index to match stacked song
            auto& list = currentPlaylist->getSongs();
            for (int i = 0; i < (int)list.size(); ++i) {
                if (list[i] == s) {
                    currentIndex = i;
                    break;
                }
            }
            return s;
        }

        // Otherwise sequential
        return previousSequential();
    }

    void addToNext(shared_ptr<Song> song) override {
        if (!song) {
            throw runtime_error("Cannot enqueue null song.");
        }
        nextQueue.push(song);
    }
};


/////////////////////////////////////////////////
//          EXTERNAL AUDIO DEVICES
////////////////////////////////////////////////

class BluetoothSpeakerAPI {
public:
  void playSoundViaBluetooth(const string& data) {
      cout << "[BluetoothSpeaker] Playing: " << data << "\n";
      // mimics playing music
  }
};

class HeadphonesAPI {
public:
    void playSoundViaJack(const string& data) {
        cout << "[Headphones] Playing: " << data << "\n";
        // mimics playing music
    }
};

class WiredSpeakerAPI {
public:
    void playSoundViaCable(const string& data) {
        cout << "[WiredSpeaker] Playing: " << data << "\n";
        // mimics playing music
    }
};



/////////////////////////////////////////////////
//          DEVICE ADAPTERS
////////////////////////////////////////////////

class IAudioOutputDevice {
public:
	virtual ~IAudioOutputDevice() {}
	virtual void playAudio(shared_ptr<Song> song) = 0;
};


class BluetoothSpeakerAdapter : public IAudioOutputDevice {
private:
	shared_ptr<BluetoothSpeakerAPI> bluetoothApi;

public:
	BluetoothSpeakerAdapter(shared_ptr<BluetoothSpeakerAPI> api){
		this->bluetoothApi = api;
	}

	void playAudio(shared_ptr<Song> song) override {
		string payload = song->getTitle() + " by " + song->getArtist();
    bluetoothApi->playSoundViaBluetooth(payload);
	}
};

class HeadphonesAdapter : public IAudioOutputDevice {
private:
    shared_ptr<HeadphonesAPI> headphonesApi;
public:
    HeadphonesAdapter(shared_ptr<HeadphonesAPI> api) {
        headphonesApi = api;
    }

    void playAudio(shared_ptr<Song> song) override {
        string payload = song->getTitle() + " by " + song->getArtist();
        headphonesApi->playSoundViaJack(payload);
    }
};

class WiredSpeakerAdapter : public IAudioOutputDevice {
private:
    shared_ptr<WiredSpeakerAPI> wiredApi;
public:
    WiredSpeakerAdapter(shared_ptr<WiredSpeakerAPI> api) {
        wiredApi = api;
    }

    void playAudio(shared_ptr<Song> song) override {
        string payload = song->getTitle() + " by " + song->getArtist();
        wiredApi->playSoundViaCable(payload);
    }
};


/////////////////////////////////////////////////
//          DEVICE FACTORY
////////////////////////////////////////////////

class DeviceFactory {
public:
    static shared_ptr<IAudioOutputDevice> createDevice(DeviceType deviceType) {
        if (deviceType == DeviceType::BLUETOOTH) {
            return make_shared<BluetoothSpeakerAdapter>(make_shared<BluetoothSpeakerAPI>());
        } else if (deviceType == DeviceType::WIRED) {
            return make_shared<WiredSpeakerAdapter>(make_shared<WiredSpeakerAPI>());
        } else { // HEADPHONES
            return make_shared<HeadphonesAdapter>(make_shared<HeadphonesAPI>());
        }
    }
};


/////////////////////////////////////////////////
//          STRATEGY MANAGER
////////////////////////////////////////////////

class StrategyManager {
private:
    static StrategyManager* instance;
    static mutex mtx;
    shared_ptr<PlayStrategy> sequentialStrategy;
    shared_ptr<PlayStrategy> randomStrategy;
    shared_ptr<PlayStrategy> customQueueStrategy;

    StrategyManager() {
        sequentialStrategy = make_shared<SequentialPlayStrategy>();
        randomStrategy = make_shared<RandomPlayStrategy>();
        customQueueStrategy = make_shared<CustomQueueStrategy>();
    }
public:
    static StrategyManager* getInstance() {
        if (!instance) {
            lock_guard<mutex> lock(mtx);
        		if (instance == nullptr) {
            	instance = new StrategyManager();
            }
        }
        return instance;
    }
    shared_ptr<PlayStrategy> getStrategy(PlayStrategyType type) {
        if (type == PlayStrategyType::SEQUENTIAL) {
            return sequentialStrategy;
        } else if (type == PlayStrategyType::RANDOM) {
            return randomStrategy;
        } else {
            return customQueueStrategy;
        }
    }
};

StrategyManager* StrategyManager::instance = nullptr;
mutex StrategyManager::mtx;


/////////////////////////////////////////////////
//          DEVICE MANAGER
////////////////////////////////////////////////

class DeviceManager {
private:
    static DeviceManager* instance;
    static mutex mtx;
    shared_ptr<IAudioOutputDevice> currentOutputDevice;
    DeviceManager() {
        currentOutputDevice = nullptr;
    }
public:
    static DeviceManager* getInstance() {
        if (instance == nullptr) {
        		lock_guard<mutex> lock(mtx);
        		if (instance == nullptr) {
            	instance = new DeviceManager();
            }
        }
        return instance;
    }
    void connect(DeviceType deviceType) {
        currentOutputDevice = DeviceFactory::createDevice(deviceType);

        switch(deviceType) {
            case DeviceType::BLUETOOTH:
                cout<< "Bluetooth device connected \n";
                break;
            case DeviceType::WIRED:
                cout<< "Wired device connected \n";
                break;
            case DeviceType::HEADPHONES:
                cout<< "Headphones connected \n";
        }
    }

    shared_ptr<IAudioOutputDevice> getOutputDevice() {
        if (!currentOutputDevice) {
            throw runtime_error("No output device is connected.");
        }
        return currentOutputDevice;
    }

    bool hasOutputDevice() {
        return currentOutputDevice != nullptr;
    }
};

DeviceManager* DeviceManager::instance = nullptr;
mutex DeviceManager::mtx;


/////////////////////////////////////////////////
//          PLAYLIST MANAGER
////////////////////////////////////////////////

class PlaylistManager {
private:
    static PlaylistManager* instance;
    static mutex mtx;
    map<string, shared_ptr<Playlist>> playlists;
    PlaylistManager() {}
public:
    static PlaylistManager* getInstance() {
        if (!instance) {
          lock_guard<mutex> lock(mtx);
      		if (instance == nullptr) {
          	instance = new PlaylistManager();
          }
        }
        return instance;
    }

    void createPlaylist(const string& name) {
        if (playlists.count(name)) {
            throw runtime_error("Playlist \"" + name + "\" already exists.");
        }
        playlists[name] =  make_shared<Playlist>(name);
    }

    void addSongToPlaylist(const string& playlistName, shared_ptr<Song> song) {
        if (!playlists.count(playlistName)) {
            throw runtime_error("Playlist \"" + playlistName + "\" not found.");
        }  
        playlists[playlistName]->addSongToPlaylist(song);
    }

    shared_ptr<Playlist> getPlaylist(const string& name) {
        if (!playlists.count(name)) {
            throw runtime_error("Playlist \"" + name + "\" not found.");
        }
        return playlists[name];
    }
};

PlaylistManager* PlaylistManager::instance = nullptr;
mutex PlaylistManager::mtx;

/////////////////////////////////////////////////
//          AUDIO ENGINE
////////////////////////////////////////////////

class AudioEngine {
private:
    shared_ptr<Song> currentSong;
    bool songIsPaused;
public:
    AudioEngine() {
        currentSong = nullptr;
        songIsPaused = false;
    }
    string getCurrentSongTitle() const {
        if (currentSong) {
            return currentSong->getTitle();
        }
        return "";
    }
    bool isPaused() const {
        return songIsPaused;
    }
    void play(shared_ptr<IAudioOutputDevice> aod, shared_ptr<Song> song) {
        if (song == nullptr) {
            throw runtime_error("Cannot play a null song.");
        }
        // Resume if same song was paused
        if (songIsPaused && song == currentSong) {
            songIsPaused = false;
            cout << "Resuming song: " << song->getTitle() << "\n";
            aod->playAudio(song);
            return;
        }

        currentSong = song;
        songIsPaused = false;
        cout << "Playing song: " << song->getTitle() << "\n";
        aod->playAudio(song);
    }

    void pause() {
        if (currentSong == nullptr) {
            throw runtime_error("No song is currently playing to pause.");
        }
        if (songIsPaused) {
            throw runtime_error("Song is already paused.");
        }
        songIsPaused = true;
        cout << "Pausing song: " << currentSong->getTitle() << "\n";
    }
};


/////////////////////////////////////////////////
//          MUSIC PLAYER FACADE
////////////////////////////////////////////////


class MusicPlayerFacade {
private:
    static MusicPlayerFacade* instance;
    static mutex mtx;
    shared_ptr<AudioEngine> audioEngine;
    shared_ptr<Playlist> loadedPlaylist;
    shared_ptr<PlayStrategy> playStrategy;

    MusicPlayerFacade() {
        loadedPlaylist = nullptr;
        playStrategy   = nullptr;
        audioEngine = make_shared<AudioEngine>();
    }

public:
    static MusicPlayerFacade* getInstance() {
        if (!instance) {
            lock_guard<mutex> lock(mtx);
        		if (instance == nullptr) {
            	instance = new MusicPlayerFacade();
            }
        }
        return instance;
    }

    void connectDevice(DeviceType deviceType) {
        DeviceManager::getInstance()->connect(deviceType);
    }

    void setPlayStrategy(PlayStrategyType strategyType) {
        playStrategy = StrategyManager::getInstance()->getStrategy(strategyType);
    }

    void loadPlaylist(const string& name) {
        loadedPlaylist = PlaylistManager::getInstance()->getPlaylist(name);
        if (!playStrategy) {
            throw runtime_error("Play strategy not set before loading.");
        }
        playStrategy->setPlaylist(loadedPlaylist);
    }
    
    void playSong(shared_ptr<Song> song) {
        if (!DeviceManager::getInstance()->hasOutputDevice()) {
            throw runtime_error("No audio device connected.");
        }
        shared_ptr<IAudioOutputDevice> device = DeviceManager::getInstance()->getOutputDevice();
        audioEngine->play(device, song);
    }

    void pauseSong(shared_ptr<Song> song) {
        if (audioEngine->getCurrentSongTitle() != song->getTitle()) {
            throw runtime_error("Cannot pause \"" + song->getTitle() + "\"; not currently playing.");
        }
        audioEngine->pause();
    }

    void playAllTracks() {
        if (!loadedPlaylist) {
            throw runtime_error("No playlist loaded.");
        }
        while (playStrategy->hasNext()) {
            shared_ptr<Song> nextSong = playStrategy->next();
            shared_ptr<IAudioOutputDevice> device = DeviceManager::getInstance()->getOutputDevice();
            audioEngine->play(device, nextSong);
        }
        cout << "Completed playlist: " << loadedPlaylist->getPlaylistName() << "\n";
    }

    void playNextTrack() {
        if (!loadedPlaylist) {
            throw runtime_error("No playlist loaded.");
        }
        if(playStrategy->hasNext()) {
            shared_ptr<Song> nextSong = playStrategy->next();
            shared_ptr<IAudioOutputDevice> device = DeviceManager::getInstance()->getOutputDevice();
            audioEngine->play(device, nextSong);
        }
        else {
            cout << "Completed playlist: " << loadedPlaylist->getPlaylistName() << "\n";
        }
    }

    void playPreviousTrack() {
        if (!loadedPlaylist) {
            throw runtime_error("No playlist loaded.");
        }
        if(playStrategy->hasPrevious()) {
            shared_ptr<Song> prevSong = playStrategy->previous();
            shared_ptr<IAudioOutputDevice> device = DeviceManager::getInstance()->getOutputDevice();
            audioEngine->play(device, prevSong);
        }
        else {
            cout << "Completed playlist: " << loadedPlaylist->getPlaylistName() << "\n";
        }
    }

    void enqueueNext(shared_ptr<Song> song) {
        playStrategy->addToNext(song);
    }
};

MusicPlayerFacade* MusicPlayerFacade::instance = nullptr;
mutex MusicPlayerFacade::mtx;


/////////////////////////////////////////////////
//          MUSIC PLAYER APPLICATION
////////////////////////////////////////////////

class MusicPlayerApplication {
private:
    static MusicPlayerApplication* instance;
    static mutex mtx;
    vector<shared_ptr<Song>> songLibrary;
    MusicPlayerApplication() {}

public:
    static MusicPlayerApplication* getInstance() {
        if (!instance) {
            lock_guard<mutex> lock(mtx);
        		if (instance == nullptr) {
            	instance = new MusicPlayerApplication();
            }
        }
        return instance;
    }

    void createSongInLibrary(const string& title, const string& artist,
                                const string& path) {
        shared_ptr<Song> newSong = make_shared<Song>(title, artist, path);
        songLibrary.push_back(newSong);
    }

    shared_ptr<Song> findSongByTitle(const string& title) {
        for (shared_ptr<Song> s : songLibrary) {
            if (s->getTitle() == title) {
                return s;
            }
        }
        return nullptr;
    }
    void createPlaylist(const string& playlistName) {
        PlaylistManager::getInstance()->createPlaylist(playlistName);
    }

    void addSongToPlaylist(const string& playlistName,
                            const string& songTitle) {
        shared_ptr<Song> song = findSongByTitle(songTitle);
        if (!song) {
            throw runtime_error("Song \"" + songTitle + "\" not found in library.");
        }
        PlaylistManager::getInstance()
            ->addSongToPlaylist(playlistName, song);
    }

    void connectAudioDevice(DeviceType deviceType) {
        MusicPlayerFacade::getInstance()->connectDevice(deviceType);
    }

    void selectPlayStrategy(PlayStrategyType strategyType) {
        MusicPlayerFacade::getInstance()->setPlayStrategy(strategyType);
    }

    void loadPlaylist(const string& playlistName) {
        MusicPlayerFacade::getInstance()->loadPlaylist(playlistName);
    }

    void playSingleSong(const string& songTitle) {
        shared_ptr<Song> song = findSongByTitle(songTitle);
        if (!song) {
            throw runtime_error("Song \"" + songTitle + "\" not found.");
        }
        MusicPlayerFacade::getInstance()->playSong(song);
    }

    void pauseCurrentSong(const string& songTitle) {
        shared_ptr<Song> song = findSongByTitle(songTitle);
        if (!song) {
            throw runtime_error("Song \"" + songTitle + "\" not found.");
        }
        MusicPlayerFacade::getInstance()->pauseSong(song);
    }

    void playAllTracksInPlaylist() {
        MusicPlayerFacade::getInstance()->playAllTracks();
    }

    void playPreviousTrackInPlaylist() {
        MusicPlayerFacade::getInstance()->playPreviousTrack();
    }

    void queueSongNext(const string& songTitle) {
        shared_ptr<Song> song = findSongByTitle(songTitle);
        if (!song) {
            throw runtime_error("Song \"" + songTitle + "\" not found.");
        }
        MusicPlayerFacade::getInstance()->enqueueNext(song);
    }
};

MusicPlayerApplication* MusicPlayerApplication::instance = nullptr;
mutex MusicPlayerApplication::mtx;


/////////////////////////////////////////////////
//          MAIN
////////////////////////////////////////////////

int main() {
    try {
        auto application = MusicPlayerApplication::getInstance();

        // Populate library
        application->createSongInLibrary("Kesariya",  "Arijit Singh",  "/music/kesariya.mp3");
        application->createSongInLibrary("Chaiyya Chaiyya",   "Sukhwinder Singh",  "/music/chaiyya_chaiyya.mp3");
        application->createSongInLibrary("Tum Hi Ho",  "Arijit Singh",  "/music/tum_hi_ho.mp3");
        application->createSongInLibrary("Jai Ho",  "A. R. Rahman",  "/music/jai_ho.mp3");
        application->createSongInLibrary("Zinda",  "Siddharth Mahadevan",  "/music/zinda.mp3");

        // Create playlist and add songs
        application->createPlaylist("Bollywood Vibes");
        application->addSongToPlaylist("Bollywood Vibes", "Kesariya");
        application->addSongToPlaylist("Bollywood Vibes", "Chaiyya Chaiyya");
        application->addSongToPlaylist("Bollywood Vibes", "Tum Hi Ho");
        application->addSongToPlaylist("Bollywood Vibes", "Jai Ho");

        // Connect device
        application->connectAudioDevice(DeviceType::BLUETOOTH);

        //Play/pause a single song
        application->playSingleSong("Zinda");
        application->pauseCurrentSong("Zinda");
        application->playSingleSong("Zinda");  // resume

        cout << "\n-- Sequential Playback --\n";
        application->selectPlayStrategy(PlayStrategyType::SEQUENTIAL);
        application->loadPlaylist("Bollywood Vibes");
        application->playAllTracksInPlaylist();

        cout << "\n-- Random Playback --\n";
        application->selectPlayStrategy(PlayStrategyType::RANDOM);
        application->loadPlaylist("Bollywood Vibes");
        application->playAllTracksInPlaylist();

        cout << "\n-- Custom Queue Playback --\n";
        application->selectPlayStrategy(PlayStrategyType::CUSTOM_QUEUE);
        application->loadPlaylist("Bollywood Vibes");
        application->queueSongNext("Kesariya");
        application->queueSongNext("Tum Hi Ho");
        application->playAllTracksInPlaylist();

        cout << "\n-- Play Previous in Sequential --\n";
        application->selectPlayStrategy(PlayStrategyType::SEQUENTIAL);
        application->loadPlaylist("Bollywood Vibes");
        application->playAllTracksInPlaylist();

        application->playPreviousTrackInPlaylist();
        application->playPreviousTrackInPlaylist();

    } catch (const exception& error) {
        cerr << "Error: " << error.what() << endl;
    }
    return 0;
}
