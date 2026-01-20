#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>
#include <ctime>
#include <mutex>
#include <condition_variable>
#define wait_time_max 7000
#define wait_time_min 2000 
class Philosopher;


enum Holding {
	NO,
	YES,
	ERROR
};

enum Fork_state {
	LAYING,
	HOLD
};

enum Philosopher_status {
	THINKING,
	WAITING,
	EATING
};

class Fork
{
	int id;
	Fork_state status;
	Philosopher* user;
	std::mutex* mtx = new std::mutex;
	std::condition_variable cv;
	public:
		Fork() : id(0), status(LAYING), user(nullptr) {}
		Fork(int id_) : id(id_), status(LAYING), user(nullptr) {}
		Fork(int id_, Fork_state is_being_used, Philosopher* user_) : id(id_), status(is_being_used), user(user_) {}
	int get_id() const {
		return this->id;
	}
	Fork_state get_status() const {
		return this->status;
	}
	Philosopher* get_user() const {
		return this->user;
	}
	std::mutex* get_mutex() const {
		return this->mtx;
	}


	Holding pick_up(Philosopher* wanna_be_user) { // return 1 for succesful pick up else 0
		if(this->status==LAYING && this->user == nullptr) {
			this->status=HOLD;
			this->user=wanna_be_user;
			return YES;
		}
		else
			return NO;
	}

	Holding put_down(Philosopher* wanna_be_user) {
		if(this->status != LAYING && this->user == wanna_be_user) {
			this->status = LAYING;
			this->user = nullptr;
			return NO;
		}
		else if(this->status == LAYING) {
			return NO;
		}
		else
			return ERROR; // tho it only goes if you wanted to put_down not your own fork
	}

	~Fork() {
		delete mtx;
	}
};

class Philosopher {
	int id;
	Fork* left_fork;
	Fork* right_fork;
	Philosopher_status status;
	Holding holding_left;
	Holding holding_right;
	public:
		Philosopher(int id_=0) {
			this->id = id_;
			this->left_fork = nullptr;
			this->right_fork = nullptr;
			this->status = THINKING;
			this->holding_left = NO;
			this->holding_right = NO;
		}
		Philosopher(int id_, Fork* left_fork_, Fork* right_fork_, Philosopher_status stauts_)
		: id(id_), left_fork(left_fork_), right_fork(right_fork_), status(stauts_) {
			this->holding_left = NO;
			this->holding_right = NO;
		}
	int get_id() const {
		return this->id;
	}
	Philosopher_status get_status() const {
		return this->status;
	}
	Fork* get_left_fork() const {
		return this->left_fork;
	}
	Fork* get_right_fork() const {
		return this->right_fork;
	}

	void Think(int wait_time) {
		this->status = THINKING;
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
	}

	void Eat(int wait_time) {
		this->status = EATING;
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
		this->holding_left = this->left_fork->put_down(this);
		this->holding_right = this->right_fork->put_down(this); 
		this->status= THINKING;
	}

	void Do() {
		int wait_time = wait_time_min + std::rand() % (wait_time_max - wait_time_min + 1 );
		while(1) {
			int think_or_eat = std::rand()%2; // if 1 eat, 0 - think
			if (!think_or_eat) {
				this->Think(wait_time);
			}
			else {
				std::mutex* mtx1 = this->get_left_fork()->get_mutex();
				std::mutex* mtx2 = this->get_right_fork()->get_mutex();
				std::unique_lock<std::mutex> left_fork_lock(*mtx1);
				std::unique_lock<std::mutex> right_fork_lock(*mtx2);
				this->Eat(wait_time);
			}
		}

		}
};

void visualization(std::vector<Philosopher>& philosophers, std::vector<std::unique_ptr<Fork>>& forks) {
    initscr(); 
    while (true) {
        clear();
        for (auto& p : philosophers) {
            printw("Filozof %d | Status: ", p.get_id());
            switch (p.get_status()) {
                case THINKING: printw("THINKING"); break;
                case WAITING:  printw("WAITING"); break;
                case EATING:   printw("EATING"); break;
            }
			printw(" | ");
            if(p.get_left_fork()->get_status() != LAYING && p.get_left_fork()->get_user() != nullptr && p.get_left_fork()->get_user()->get_id() == p.get_id())
			{
				printw("F%d, ",p.get_left_fork()->get_id());
			}
			if(p.get_right_fork()->get_status() != LAYING &&  p.get_right_fork()->get_user() != nullptr && p.get_right_fork()->get_user()->get_id() == p.get_id()) {
				printw("F%d |",p.get_right_fork()->get_id());
			}
            printw("\n");
        }
		printw("\n TABLE:\n | ");
		for (auto& f : forks) {
			if(f->get_status() == LAYING)
				printw("F%d | ",f->get_id());
		}
		printw("\n");
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    endwin(); 
}

int main(int argc, char* argv[])
{	
	if(argc < 2 ) {
		printf("zla ilosc parametrow - zrestartuj program\n");
		return 1;
	}
	else {
		srand(time(nullptr));
		int amount_of_philosophers = std::atoi(argv[1]);
		std::vector <Philosopher> philosopher_table;
		std::vector<std::unique_ptr<Fork>> fork_table;
		std::vector<std::thread> threads;
		for (int i = 0 ; i < amount_of_philosophers ; i++)
		{
				fork_table.push_back(std::make_unique<Fork>(i, LAYING, nullptr));
		}
		philosopher_table.push_back(Philosopher(0,fork_table[0].get(),fork_table[amount_of_philosophers-1].get(), EATING));
		for(int i = 1 ; i < amount_of_philosophers ; i++) {
				philosopher_table.push_back(Philosopher(i, fork_table[i].get(),fork_table[i-1].get(),WAITING));
		}

		for (int i = 0; i < amount_of_philosophers; ++i) {
			threads.emplace_back(&Philosopher::Do, &philosopher_table[i]);
		}
		std::thread vis_thread(visualization, std::ref(philosopher_table), std::ref(fork_table));
		for (auto &t : threads) {
			t.join();
		}
		vis_thread.join();
		return 0;
	}
}