#include <ncurses.h>
#include <cstdlib>
#include <thread>
#include <vector>

class Philosopher;

enum Frok_state {
	LAYING,
	HOLD,
	IN_USE
};

enum Philosopher_status {
	THINKING,
	WAITING,
	EATING
};

class Fork
{
	int id;
	Frok_state status;
	Philosopher* user;
	public:
		Fork() : id(0), status(LAYING), user(nullptr) {}
		Fork(int id_) : id(id_), status(LAYING), user(nullptr) {}
		Fork(int id_, Frok_state is_being_used, Philosopher* user_) : id(id_), status(is_being_used), user(user_) {}
	int get_id() const {
		return this->id;
	}
	Frok_state get_status() const {
		return this->status;
	}
	Philosopher* get_user() const {
		return this->user;
	}

};

class Philosopher {
	int id;
	Fork* left_fork;
	Fork* right_fork;
	Philosopher_status status;
	public:
		Philosopher(int id_=0) {
			this->id = id_;
			this->left_fork = nullptr;
			this->right_fork = nullptr;
			this->status = THINKING;
		}
		Philosopher(int id_, Fork* left_fork_, Fork* right_fork_, Philosopher_status stauts_)
		: id(id_), left_fork(left_fork_), right_fork(right_fork_), status(stauts_) {}
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
};

void visualization(std::vector<Philosopher>& philosophers, std::vector<Fork>& forks) {
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
            if(p.get_left_fork()->get_status() != LAYING && p.get_left_fork()->get_user()->get_id() == p.get_id())
			{
				printw("L%d ",p.get_left_fork()->get_id());
			}
			else if(p.get_right_fork()->get_status() != LAYING && p.get_right_fork()->get_user()->get_id() == p.get_id()) {
				printw("R%d ",p.get_right_fork()->get_id());
			}
            printw("\n");
        }
		printw("\n TABLE:\n | ");
		for (auto& f : forks) {
			if(f.get_status() == LAYING)
				printw("F%d | ",f.get_id());
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
		
		int amount_of_philosophers = std::atoi(argv[1]);
		std::vector <Philosopher> philosopher_table;
		std::vector <Fork> fork_table;
		for (int i = 0 ; i < amount_of_philosophers ; i++)
		{
				fork_table.emplace_back(i,LAYING,nullptr);
		}
		philosopher_table.push_back(Philosopher(0,&fork_table[0],&fork_table[amount_of_philosophers-1], EATING));
		for(int i = 1 ; i < amount_of_philosophers ; i++) {
				philosopher_table.push_back(Philosopher(i, &fork_table[i],&fork_table[i-1],WAITING));
		}
		std::thread vis_thread(visualization, std::ref(philosopher_table), std::ref(fork_table));
		

		vis_thread.join();
		return 0;
	}
}