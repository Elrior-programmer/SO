#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <chrono>

#define wait_time_max 15000
#define wait_time_min 5000 

enum Fork_state {
    NOT_USED,
    USED
};

enum Philosopher_status {
    THINKING,
    WAITING,
    EATING
};

class Fork {
    int id;
    Fork_state status;
    std::mutex mtx;
    std::condition_variable cv;
    
public:
    Fork() : id(0), status(NOT_USED) {}
    Fork(int id_) : id(id_), status(NOT_USED) {}
    Fork(int id_, Fork_state is_being_used) : id(id_), status(is_being_used) {}
    
    int get_id() const { return this->id; }
    void set_id(int id_) { this->id = id_; }
    
    Fork_state get_status() const {
        return this->status;
    }
    
    std::mutex& get_mutex() { return mtx; }
    
    void acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        while(status == USED) {
            cv.wait(lock);
        }
        status = USED;
    }
    
    void release() {
        std::unique_lock<std::mutex> lock(mtx);
        status = NOT_USED;
        cv.notify_all();
    }
};

class Philosopher {
    int id;
    Fork* left_fork;
    Fork* right_fork;
    Philosopher_status status;
    mutable std::mutex status_mtx;
    
public:
    Philosopher() : id(0), left_fork(nullptr), right_fork(nullptr), status(THINKING) {}
    Philosopher(int id_, Fork* left_fork_, Fork* right_fork_, Philosopher_status status_) 
        : id(id_), left_fork(left_fork_), right_fork(right_fork_), status(status_) {}
    
    int get_id() const { return this->id; }
    void set_id(int id_) { this->id = id_; }
    
    Philosopher_status get_status() const { 
        std::lock_guard<std::mutex> lock(status_mtx);
        return this->status; 
    }
    
    void set_status(Philosopher_status s) { 
        std::lock_guard<std::mutex> lock(status_mtx);
        this->status = s; 
    }
    
    Fork* get_left_fork() const { return this->left_fork; }
    Fork* get_right_fork() const { return this->right_fork; }
    void set_left_fork(Fork* fork) { this->left_fork = fork; }
    void set_right_fork(Fork* fork) { this->right_fork = fork; }
    
    void Do(int stat[], std::mutex& stat_mtx) {
        int wait_time = wait_time_min + std::rand() % (wait_time_max - wait_time_min + 1);
        
        // Asymmetric fork ordering to prevent deadlock
        Fork* first_fork;
        Fork* second_fork;
        
        if(this->id % 2 == 0) {
            // Even philosophers: left then right
            first_fork = this->left_fork;
            second_fork = this->right_fork;
        } else {
            // Odd philosophers: right then left
            first_fork = this->right_fork;
            second_fork = this->left_fork;
        }
        
        while(1) {
            int think_or_eat = std::rand() % 2; // Eat:1 Think:0
            
            if(!think_or_eat) {
                // Think
                set_status(THINKING);
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
            }
            else {
                // Try to eat
                set_status(WAITING);
                
                first_fork->acquire();
                second_fork->acquire();
                
                // Eat
                set_status(EATING);
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
                
                {
                    std::lock_guard<std::mutex> lock(stat_mtx);
                    stat[this->id] += 1;
                }
                
                // Release forks
                second_fork->release();
                first_fork->release();
            }
        }
    }
};

void visualization(Fork f_tab[], Philosopher p_tab[], int amount, int stat[], std::mutex& stat_mtx) {
    initscr();
    while (true) {
        clear();
        
        // Display philosophers
        printw("=== PHILOSOPHERS ===\n");
        for (int i = 0; i < amount; i++) {
            printw("Filozof %d | Status: ", p_tab[i].get_id() + 1);
            Philosopher_status ps = p_tab[i].get_status();
            switch (ps) {
                case THINKING: printw("THINKING"); break;
                case WAITING:  printw("WAITING"); break;
                case EATING:   printw("EATING"); break;
            }
            printw("\n");
        }
        
        // Display forks
        printw("\n=== FORKS ===\n");
        for (int i = 0; i < amount; i++) {
            Fork_state fs;
            {
                std::lock_guard<std::mutex> lock(f_tab[i].get_mutex());
                fs = f_tab[i].get_status();
            }
            printw("Fork %d | Status: ", f_tab[i].get_id() + 1);
            switch (fs) {
                case NOT_USED: printw("FREE"); break;
                case USED:     printw("USED"); break;
            }
            printw("\n");
        }
        
        // Display statistics
        printw("\n=== STATISTICS ===\n");
        {
            std::lock_guard<std::mutex> lock(stat_mtx);
            for (int i = 0; i < amount; i++) {
                printw("Filozof %d - zjadl: %d razy\n", p_tab[i].get_id() + 1, stat[i]);
            }
        }
        
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    endwin(); 
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("zla ilosc parametrow - zrestartuj program\n");
        return 1;
    }
    
    srand(time(nullptr));
    int amount_of_philosophers = std::atoi(argv[1]);
    
    int* stat = new int[amount_of_philosophers]{0};
    Philosopher* p_tab = new Philosopher[amount_of_philosophers];
    Fork* f_tab = new Fork[amount_of_philosophers];
    std::vector<std::thread> threads;
    std::mutex stat_mtx;
    
    for (int i = 0; i < amount_of_philosophers; i++) {
        f_tab[i].set_id(i);
    }
    
    p_tab[0].set_id(0);
    p_tab[0].set_status(THINKING);
    p_tab[0].set_left_fork(&f_tab[0]);
    p_tab[0].set_right_fork(&f_tab[amount_of_philosophers - 1]);

    for (int i = 1; i < amount_of_philosophers; i++) {
        p_tab[i].set_id(i);
        p_tab[i].set_status(THINKING);
        p_tab[i].set_left_fork(&f_tab[i]);
        p_tab[i].set_right_fork(&f_tab[i - 1]);
    }

    for (int i = 0; i < amount_of_philosophers; ++i) {
        threads.emplace_back(&Philosopher::Do, &p_tab[i], stat, std::ref(stat_mtx));
    }
    
    std::thread vis_thread(visualization, f_tab, p_tab, amount_of_philosophers, stat, std::ref(stat_mtx));
    
    for (auto &t : threads) {
        t.join();
    }
    vis_thread.join();
    
    delete[] stat;
    delete[] p_tab;
    delete[] f_tab;
    
    return 0;
}